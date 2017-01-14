#include "individual.hpp"
#include <numeric>

namespace genetic {

/*static*/ std::function<int(individual const &)> individual::evaluation_function = [] (individual const & ind) -> int {
   return std::accumulate(ind.attributes.begin(), ind.attributes.end(), 0, [](int current_sum, attribute const & attr) {
      return current_sum + attr.value;
   });
};
/*static*/ std::function<void(individual *, individual *)> individual::mating_function = &individual::two_point_crossover;
/*static*/ std::size_t individual::attribute_count = 100;

individual::individual() :
      fitness(0),
      valid_fitness(false),
      attributes(attribute_count) {
}

/*static*/ void individual::mate(individual * ind1, individual * ind2) {
   mating_function(ind1, ind2);
}

/*static*/ void individual::one_point_crossover(individual * ind1, individual * ind2) {
   std::size_t size, point;
   size = std::min(ind1->size(), ind2->size());
   point = random::randint(1, size - 1);
   // Swap point1: between individual 1 and 2
   std::swap_ranges(ind1->begin() + point, ind1->end(), ind2->begin() + point);

   ind1->valid_fitness = false;
   ind2->valid_fitness = false;
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
   std::swap_ranges(ind1->begin() + point1, ind1->begin() + point2, ind2->begin() + point1);

   ind1->valid_fitness = false;
   ind2->valid_fitness = false;
}

/*static*/ void individual::evaluation_method(std::function<int(individual const &)> && fcn) {
   evaluation_function = std::move(fcn);
}

void individual::mutate(float mutation_rate) {
   for (auto & attr : *this) {
      if (random::probability(mutation_rate)) {
         attr = attribute();
      }
   }
   valid_fitness = false;
}

int individual::evaluate() {
   if (!evaluation_function) {
      throw std::runtime_error("individual has no evaluation function");
   }
   if (!valid_fitness) {
      fitness = evaluation_function(*this);
      valid_fitness = true;
   }
   return fitness;
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

std::vector<attribute>::iterator individual::begin() {
   return attributes.begin();
}

std::vector<attribute>::iterator individual::end() {
   return attributes.end();
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