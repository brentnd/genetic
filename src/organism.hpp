#ifndef GENETIC_ORGANISM_HPP
#define GENETIC_ORGANISM_HPP

#include <algorithm> // std::min, std::swap_ranges
#include <iostream>
#include <iterator>
#include <utility> // std::swap
#include <vector>

#include "random.hpp"

namespace genetic {

template <typename TAttr>
class organism {
public:
   organism() :
         fitness(0),
         valid_fitness(false) {
   }

   // Both modified in place (two-point crossover)
   static void mate(organism * ind1, organism * ind2) const {
      std::size_t size, point1, point2;
      size = std::min(ind1.size(), ind2.size());
      point1 = random::randint(1, size);
      point2 = random::randint(1, size - 1);
      if (point2 >= point1) {
         ++point2;
      } else {
         std::swap(point1, point2);
      }
      // Swap point1:point2 between individual 1 and 2
      std::swap_ranges(ind1.begin() + point1, ind1.begin() + point2, ind2.begin() + point1);
      ind1->valid_fitness = false;
      ind2->valid_fitness = false;
   }

   // Mutate in-place by selecting random integer
   // TODO: partial specialization for int, float?
   void mutate(std::function<TAttr()> attr_random, float mutation_rate) {
      for (auto & attr : *this) {
         if (random::probability(mutation_rate)) {
            attr = attr_random();
         }
      }
      valid_fitness = false;
   }

   int evaluate() {
      if (!evaluation_function) {
         throw std::runtime_error("organism has no evaluation function");
      }
      if (!valid_fitness) {
         fitness = evaluation_function(*this);
         valid_fitness = true;
      }
      return fitness;
   }

   friend std::ostream& operator<< (std::ostream& stream, const organism& ind) {
      stream << "individual @ (" << static_cast<const void *>(&ind) << ") f=" << ind.fitness;
      return stream;
   }

   bool operator<(organism const &other) const {
      if (!valid_fitness || !other.valid_fitness) {
         std::runtime_error("attempting to compare organisms with invalid fitness");
      }
      return fitness < other.fitness;
   }

   std::iterator begin() {
      return attributes.begin();
   }

   std::iterator end() {
      return attributes.end();
   }

   std::size_t size() {
      return attributes.size();
   }

private:
   std::vector<TAttr> attributes;
   std::function<int(organism const &)> evaluation_function;
   int fitness;
   bool valid_fitness;
};

}

#endif //GENETIC_ORGANISM_HPP
