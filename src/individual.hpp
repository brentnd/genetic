#ifndef GENETIC_INDIVIDUAL_HPP
#define GENETIC_INDIVIDUAL_HPP

#include <algorithm> // std::min, std::swap_ranges
#include <functional> // std::function
#include <iostream>
#include <utility> // std::swap
#include <vector>

#include "random.hpp"
#include "attribute.hpp"

namespace genetic {

class individual {
public:
   individual();

   // Both modified in place (two-point crossover)
   static void mate(individual * ind1, individual * ind2);

   static void set_evaluation_function(std::function<int(individual const &)> && fcn);

   // Mutate in-place by with random attributes
   void mutate(float mutation_rate);

   int evaluate();

   bool operator<(individual const & other) const;

   bool operator>(individual const & other) const;

   bool operator==(individual const & other) const;

   std::vector<attribute>::iterator begin();

   std::vector<attribute>::iterator end();

   std::size_t size() const;

   friend std::ostream& operator<<(std::ostream & stream, const individual & ind) {
      stream << "individual @ (" << static_cast<const void *>(&ind) << ") f=" << ind.fitness;
      return stream;
   }

private:
   static std::function<int(individual const &)> evaluation_function;
   static std::size_t attribute_count;
   void throw_if_fitness_invalid() const;

private:
   int fitness;
   bool valid_fitness;
   std::vector<attribute> attributes;
};

}

#endif //GENETIC_INDIVIDUAL_HPP
