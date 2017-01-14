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

   static void one_point_crossover(individual * ind1, individual * ind2);
   static void two_point_crossover(individual * ind1, individual * ind2);

   static void evaluation_method(std::function<int(individual const &)> && fcn);

   template <typename... Args>
   static void mating_method(void (*fcn)(individual *, individual *, Args...), Args... args) {
      mating_function = std::bind(fcn, std::placeholders::_1, std::placeholders::_2, std::forward<Args>(args)...);
   }

   template <typename... Args>
   static void mutation_method(void (individual::* fcn)(Args...), Args... args) {
      mutation_function = std::bind(fcn, std::placeholders::_1, std::forward<Args>(args)...);
   }

   void seed();

   // Mutate in-place by with random attributes
   void mutate();
   void uniform_int(float mutation_rate, int min, int max);
   void flip_bit(float mutation_rate);
   void shuffle_indexes(float mutation_rate);

   int evaluate();

   bool operator<(individual const & other) const;

   bool operator>(individual const & other) const;

   bool operator==(individual const & other) const;

   std::size_t size() const;

   friend std::ostream& operator<<(std::ostream & stream, const individual & ind) {
      stream << "individual @ (" << static_cast<const void *>(&ind) << ") f=" << ind.fitness;
      return stream;
   }

private:
   static std::function<int(individual const &)> evaluation_function;
   static std::function<void(individual *, individual *)> mating_function;
   static std::function<void(individual &)> mutation_function;
   static std::size_t attribute_count;
   void throw_if_fitness_invalid() const;

private:
   int fitness;
   bool valid_fitness;
   std::vector<attribute> attributes;
};

}

#endif //GENETIC_INDIVIDUAL_HPP
