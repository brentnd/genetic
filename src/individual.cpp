#include <geneticpp/individual.hpp>
#include <numeric>

namespace genetic {

/*static*/ std::function<int(individual const &)> individual::evaluation_function = &individual::eval_sum;
/*static*/ std::function<void(individual *, individual *)> individual::mating_function = &individual::two_point_crossover;
/*static*/ std::function<void(individual &)> individual::mutation_function =
      std::bind(&individual::shuffle_indexes, std::placeholders::_1, static_cast<float>(0.05));
/*static*/ std::size_t individual::attribute_count = 100;

individual::individual() :
      fitness(0),
      valid_fitness(false),
      attributes(attribute_count) {
}

/*static*/ void individual::mate(individual * ind1, individual * ind2) {
   mating_function(ind1, ind2);
   ind1->valid_fitness = false;
   ind2->valid_fitness = false;
}

/*static*/ void individual::one_point_crossover(individual * ind1, individual * ind2) {
   std::size_t size, point;
   size = std::min(ind1->size(), ind2->size());
   point = random::randint(1, size - 1);
   // Swap point1: between individual 1 and 2
   std::swap_ranges(ind1->attributes.begin() + point, ind1->attributes.end(), ind2->attributes.begin() + point);
}

/*static*/ void individual::two_point_crossover(individual * ind1, individual * ind2) {
   std::size_t size, point1, point2;
   size = std::min(ind1->size(), ind2->size());
   point1 = random::randint(1, size);
   point2 = random::randint(1, size - 1);
   if (point2 >= point1) {
      ++point2;
   } else {
      std::swap(point1, point2);
   }
   // Swap point1:point2 between individual 1 and 2
   std::swap_ranges(ind1->attributes.begin() + point1, ind1->attributes.begin() + point2, ind2->attributes.begin() + point1);
}

/*static*/ void individual::evaluation_method(std::function<int(individual const &)> && fcn) {
   evaluation_function = std::move(fcn);
}

void individual::seed() {
   for (auto & attr : attributes) {
      attr.seed();
   }
   valid_fitness = false;
}

void individual::mutate() {
   mutation_function(*this);
   valid_fitness = false;
}

void individual::uniform_int(float mutation_rate, double min, double max) {
   for (auto & attr : attributes) {
      if (random::probability(mutation_rate)) {
         attr.randomize(min, max);
      }
   }
}

void individual::flip_bit(float mutation_rate) {
   for (auto & attr : attributes) {
      if (random::probability(mutation_rate)) {
         attr.flip();
      }
   }
}

void individual::shuffle_indexes(float mutation_rate) {
   unsigned swap_index;
   for (unsigned i=0; i < size(); ++i) {
      if (random::probability(mutation_rate)) {
         swap_index = static_cast<unsigned>(random::randint(0, size() - 2));
         if (swap_index >= i) {
            ++swap_index;
         }
         std::swap(attributes.at(i), attributes.at(swap_index));
      }
   }
}

int individual::evaluate() {
   if (!valid_fitness) {
      fitness = evaluation_function(*this);
      valid_fitness = true;
   }
   return fitness;
}

/*static*/ int individual::eval_sum(individual const & ind) {
   return static_cast<int>(std::accumulate(ind.attributes.begin(), ind.attributes.end(), 0.0,
                                           [] (double current_sum, attribute const & attr) -> double {
                                              return current_sum + static_cast<double>(attr);
                                           }));
}

bool individual::operator<(individual const & other) const {
   throw_if_fitness_invalid(); other.throw_if_fitness_invalid();
   return fitness < other.fitness;
}

bool individual::operator>(individual const & other) const {
   throw_if_fitness_invalid(); other.throw_if_fitness_invalid();
   return fitness > other.fitness;
}

bool individual::operator==(individual const & other) const {
   throw_if_fitness_invalid(); other.throw_if_fitness_invalid();
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

void individual::throw_if_fitness_invalid() const {
   if (!valid_fitness) {
      throw std::runtime_error("attempting to compare individuals with invalid fitness");
   }
}

} // namespace genetic