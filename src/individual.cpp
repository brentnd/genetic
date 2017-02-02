#include <geneticpp/individual.hpp>
#include <randomcpp.hpp>
#include <numeric>

namespace genetic {

/*static*/ std::function<std::vector<float>(individual const &)> individual::evaluation_function = &individual::eval_sum;
/*static*/ std::function<void(individual *, individual *)> individual::crossover_function = &individual::two_point_crossover;
/*static*/ std::function<void(individual &)> individual::mutation_function =
      std::bind(&individual::shuffle_indexes, std::placeholders::_1, static_cast<float>(0.05));
/*static*/ std::size_t individual::attribute_count = 100;

individual::individual() :
      fit(),
      attributes(attribute_count) {
}

/*static*/ void individual::mate(individual * ind1, individual * ind2) {
   crossover_function(ind1, ind2);
   ind1->fit.dirty();
   ind2->fit.dirty();
}

/*static*/ void individual::one_point_crossover(individual * ind1, individual * ind2) {
   std::size_t size, point;
   size = std::min(ind1->size(), ind2->size());
   point = randomcpp::randrange(1, size);
   // Swap point1: between individual 1 and 2
   std::swap_ranges(ind1->attributes.begin() + point, ind1->attributes.end(), ind2->attributes.begin() + point);
}

/*static*/ void individual::two_point_crossover(individual * ind1, individual * ind2) {
   std::size_t size, point1, point2;
   size = std::min(ind1->size(), ind2->size());
   point1 = randomcpp::randrange(1, size);
   point2 = randomcpp::randrange(1, size - 1);
   if (point2 >= point1) {
      ++point2;
   } else {
      std::swap(point1, point2);
   }
   // Swap point1:point2 between individual 1 and 2
   std::swap_ranges(ind1->attributes.begin() + point1, ind1->attributes.begin() + point2, ind2->attributes.begin() + point1);
}

/*static*/ void individual::uniform_crossover(individual *ind1, individual *ind2, float indpb) {
   std::size_t size = std::min(ind1->size(), ind2->size());
   for (unsigned i=0; i < size; i++) {
      if (randomcpp::probability(indpb)) {
         std::swap(ind1->attributes[i], ind2->attributes[i]);
      }
   }
}

/*static*/ void individual::ordered_crossover(individual * ind1, individual * ind2) {
   std::size_t size, a, b;
   size = std::min(ind1->size(), ind2->size());
   a = randomcpp::randrange(size);
   b = randomcpp::randrange(size);

   if (a > b) {
      std::swap(a,b);
   }

   std::vector<bool> holes1(size, true), holes2(size, true);
   try {
      for (unsigned i = 0; i < size; i++) {
         if (i < a || i > b) {
            holes1.at(ind2->at(i)) = false;
            holes2.at(ind1->at(i)) = false;
         }
      }
   } catch (std::out_of_range const & e) {
      throw std::out_of_range("ordered_crossover can only be used when the attribute value never exceeds attribute count");
   }

   auto temp1 = ind1->attributes;
   auto temp2 = ind2->attributes;
   std::size_t k1 = b + 1, k2 = b + 1;
   for (unsigned i=0; i < size; i++) {
      if (!holes1[temp1[(i + b + 1) % size]]) {
         ind1->attributes[k1 % size] = temp1[(i + b + 1) % size];
         ++k1;
      }
      if (!holes2[temp2[(i + b + 1) % size]]) {
         ind2->attributes[k2 % size] = temp2[(i + b + 1) % size];
         ++k2;
      }
   }

   for (unsigned i=a; i <= b; i++) {
      std::swap(ind1->attributes[i], ind2->attributes[i]);
   }
}

/*static*/ void individual::blend_crossover(individual * ind1, individual * ind2, float alpha) {
   std::size_t size = std::min(ind1->size(), ind2->size());
   float gamma, x1, x2;
   for (unsigned i=0; i < size; i++) {
      x1 = ind1->attributes[i];
      x2 = ind2->attributes[i];
      gamma = (1.0f + 2.0f * alpha) * randomcpp::uniform(0.0, 1.0) - alpha;
      ind1->attributes[i] = (1.0f * gamma) * x1 + gamma * x2;
      ind2->attributes[i] = gamma * x1 + (1.0f - gamma) * x2;
   }
}

/*static*/ void individual::evaluation_method(std::function<std::vector<float>(individual const &)> && fcn) {
   evaluation_function = std::move(fcn);
}

void individual::seed() {
   unsigned i=0;
   for (auto & attr : attributes) {
      attr.seed(i++);
   }
   fit.dirty();
}

void individual::mutate() {
   mutation_function(*this);
   fit.dirty();
}

void individual::uniform_int(float mutation_rate, int min, int max) {
   for (auto & attr : attributes) {
      if (randomcpp::probability(mutation_rate)) {
         attr.randomize_int(min, max);
      }
   }
}

void individual::flip_bit(float mutation_rate) {
   for (auto & attr : attributes) {
      if (randomcpp::probability(mutation_rate)) {
         attr.flip();
      }
   }
}

void individual::shuffle_indexes(float mutation_rate) {
   unsigned swap_index;
   for (unsigned i=0; i < size(); ++i) {
      if (randomcpp::probability(mutation_rate)) {
         swap_index = static_cast<unsigned>(randomcpp::randrange(size() - 1));
         if (swap_index >= i) {
            ++swap_index;
         }
         std::swap(attributes.at(i), attributes.at(swap_index));
      }
   }
}

void individual::gaussian(float mutation_rate, float mu, float sigma) {
   for (auto & attr : attributes) {
      if (randomcpp::probability(mutation_rate)) {
         attr = static_cast<float>(attr) + randomcpp::gauss(mu, sigma);
      }
   }
}

void individual::polynomial_bounded(float mutation_rate, float eta, float xl, float xu) {
   for (auto & attr : attributes) {
      // Adopted from real_mutate_ind in NSGA2 by Dr. Kalyanmoy Deb
      // https://github.com/amirmasoudabdol/nsga2/blob/master/mutation.c#L63
      if (randomcpp::probability(mutation_rate)) {
         float x = attr;
         float delta_1 = (x - xl) / (xu - xl);
         float delta_2 = (xu - x) / (xu - xl);
         float mut_pow = 1.0f / (eta - 1.0f);

         float rand = randomcpp::uniform(0.0, 1.0);
         float delta_q, xy, val;
         if (rand < 0.5f) {
            xy = 1.0f - delta_1;
            val = 2.0f * rand + (1.0f - 2.0f * rand) * std::pow(xy, eta + 1.0f);
            delta_q = std::pow(val, mut_pow) - 1.0f;
         } else {
            xy = 1.0f - delta_2;
            val = 2.0f * (1.0f - rand) + 2.0f * (rand - 0.5f) * std::pow(xy, eta + 1.0f);
            delta_q = 1.0f - std::pow(val, mut_pow);
         }
         x += delta_q * (xu - xl);
         x = std::min(std::max(x, xl), xu);
         attr = x;
      }
   }
}

void individual::evaluate() {
   if (!fit.valid()) {
      fit.update(evaluation_function(*this));
   }
}

float individual::weighted_fitness() const {
   return fit.weighted_fitness();
}

/*static*/ std::vector<float> individual::eval_sum(individual const & ind) {
   return {ind.sum_attributes()};
}

float individual::sum_attributes() const {
   return std::accumulate(attributes.begin(), attributes.end(), 0.0f,
                          [] (float current_sum, attribute const & attr) -> float {
                             return current_sum + static_cast<float>(attr);
                          });
}

bool individual::operator<(individual const & other) const {
   return fit < other.fit;
}

bool individual::operator>(individual const & other) const {
   return fit > other.fit;
}

bool individual::operator==(individual const & other) const {
   return attributes == other.attributes;
}

std::vector<attribute>::const_iterator individual::begin() const {
   return attributes.begin();
}

std::vector<attribute>::iterator individual::begin() {
   return attributes.begin();
}

std::vector<attribute>::const_iterator individual::end() const {
   return attributes.end();
}

std::vector<attribute>::iterator individual::end() {
   return attributes.end();
}

attribute const & individual::at(std::size_t pos) const {
   return attributes.at(pos);
}

std::size_t individual::size() const {
   return attributes.size();
}

} // namespace genetic