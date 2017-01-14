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

   // Mating
   template <typename... Args>
   static void mating_method(void (*fcn)(individual *, individual *, Args...), Args... args) {
      mating_function = std::bind(fcn, std::placeholders::_1, std::placeholders::_2, std::forward<Args>(args)...);
   }

   // Mate two individuals using static method
   static void mate(individual * ind1, individual * ind2);
   // Mating variants for mating_method
   static void one_point_crossover(individual * ind1, individual * ind2);
   static void two_point_crossover(individual * ind1, individual * ind2);

   // Evaluate this individuals fitness
   int evaluate();
   static int eval_sum(individual const & ind);
   // Custom function for evaluation
   static void evaluation_method(std::function<int(individual const &)> && fcn);

   // Mutation
   template <typename... Args>
   static void mutation_method(void (individual::* fcn)(Args...), Args... args) {
      mutation_function = std::bind(fcn, std::placeholders::_1, std::forward<Args>(args)...);
   }
   // Mutate this individual using mutation method
   void mutate();
   // Mutation variants for mutation_method
   void uniform_int(float mutation_rate, int min, int max);
   void flip_bit(float mutation_rate);
   void shuffle_indexes(float mutation_rate);

   // Initialize the attributes by seeding them
   void seed();

   bool operator<(individual const & other) const;

   bool operator>(individual const & other) const;

   bool operator==(individual const & other) const;

   std::vector<attribute>::iterator begin();
   std::vector<attribute>::iterator end();
   attribute const & at(std::size_t pos) const;
   std::size_t size() const;

   friend std::ostream& operator<<(std::ostream & stream, const individual & ind) {
      stream << "individual @ (" << static_cast<const void *>(&ind) << ") f=" << ind.fitness << " attributes=[";
      for (auto const & attr : ind.attributes) {
         char val = attr;
         if (val > ' ' && val < '~') {
            stream << static_cast<char>(attr);
         } else {
            stream << attr.value;
         }
         stream << ", ";
      }
      stream << "]";
      return stream;
   }

public:
   static std::size_t attribute_count;

private:
   static std::function<int(individual const &)> evaluation_function;
   static std::function<void(individual *, individual *)> mating_function;
   static std::function<void(individual &)> mutation_function;
   void throw_if_fitness_invalid() const;

private:
   int fitness;
   bool valid_fitness;
   std::vector<attribute> attributes;
};

}

#endif //GENETIC_INDIVIDUAL_HPP
