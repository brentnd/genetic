#ifndef GENETIC_ORGANISM_HPP
#define GENETIC_ORGANISM_HPP

#include <algorithm> // std::min, std::swap_ranges
#include <functional> // std::function
#include <iostream>
#include <utility> // std::swap
#include <vector>

#include "random.hpp"
#include "attribute.hpp"

namespace genetic {

class organism {
public:
   organism();

   // Both modified in place (two-point crossover)
   static void mate(organism * ind1, organism * ind2);

   static void set_evaluation_function(std::function<int(organism const &)> && fcn);

   // Mutate in-place by with random attributes
   void mutate(float mutation_rate);

   int evaluate();

   bool operator<(organism const & other) const;

   bool operator>(organism const & other) const;

   bool operator==(organism const & other) const;

   std::vector<attribute>::iterator begin();

   std::vector<attribute>::iterator end();

   std::size_t size() const;

   friend std::ostream& operator<<(std::ostream & stream, const organism & ind) {
      stream << "individual @ (" << static_cast<const void *>(&ind) << ") f=" << ind.fitness;
      return stream;
   }

private:
   static std::function<int(organism const &)> evaluation_function;
   static std::size_t attribute_count;

   int fitness;
   bool valid_fitness;
   std::vector<attribute> attributes;
};

}

#endif //GENETIC_ORGANISM_HPP
