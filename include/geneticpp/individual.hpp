#pragma once

#include <algorithm> // std::min, std::swap_ranges
#include <functional> // std::function
#include <iostream>
#include <utility> // std::swap
#include <vector>

#include "random.hpp"
#include "attribute.hpp"
#include "fitness.hpp"

namespace genetic {

class individual {
public:
   individual();

   // crossover
   template <typename... Args>
   static void crossover_method(void (*fcn)(individual *, individual *, Args...), Args... args) {
      crossover_function = std::bind(fcn, std::placeholders::_1, std::placeholders::_2, std::forward<Args>(args)...);
   }

   // Mate two individuals using static method
   static void mate(individual * ind1, individual * ind2);
   // crossover variants for crossover_method
   static void one_point_crossover(individual * ind1, individual * ind2);
   static void two_point_crossover(individual * ind1, individual * ind2);
   static void uniform_crossover(individual * ind1, individual * ind2, float indpb);
   static void ordered_crossover(individual * ind1, individual * ind2);
   static void blend_crossover(individual * ind1, individual * ind2, float alpha);

   // Evaluate this individuals fitness
   void evaluate();
   float weighted_fitness() const;
   static std::vector<float> eval_sum(individual const & ind);
   float sum_attributes() const;
   // Custom function for evaluation
   static void evaluation_method(std::function<std::vector<float>(individual const &)> && fcn);

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

   std::vector<attribute>::const_iterator begin() const;
   std::vector<attribute>::iterator begin();
   std::vector<attribute>::const_iterator end() const;
   std::vector<attribute>::iterator end();
   attribute const & at(std::size_t pos) const;
   std::size_t size() const;

   friend std::ostream& operator<<(std::ostream & stream, individual const & ind) {
      stream << "individual @ (" << static_cast<const void *>(&ind) << ") f=" << ind.weighted_fitness();

      stream << " attr=[";
      for (auto const & attr : ind.attributes) {
         stream << attr << attribute::display_delimiter;
      }
      stream << "]";
      return stream;
   }

public:
   static std::size_t attribute_count;

private:
   static std::function<std::vector<float>(individual const &)> evaluation_function;
   static std::function<void(individual *, individual *)> crossover_function;
   static std::function<void(individual &)> mutation_function;
   void throw_if_fitness_invalid() const;

private:
   fitness fit;
   std::vector<attribute> attributes;
};

} // namespace genetic
