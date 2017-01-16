#ifndef GENETIC_POPULATION_HPP
#define GENETIC_POPULATION_HPP

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <functional>
#include <vector>

#include "individual.hpp"
#include "random.hpp"

namespace genetic {

class population {
public:
   population(std::size_t size_);

   population(population const & pop);

   template <typename... Args>
   static void selection_method(population (population::* fcn)(std::size_t, Args...) const, Args... args) {
      selection_function = std::bind(fcn, std::placeholders::_1, std::placeholders::_2, std::forward<Args>(args)...);
   }

   template <typename... Args>
   static void evolution_method(void (population::* fcn)(Args...), Args... args) {
      evolution_function = std::bind(fcn, std::placeholders::_1, std::forward<Args>(args)...);
   }

   // Generate an entirely new population by random selection
   population select_random(std::size_t k) const;

   // Generate an entirely new population by selecting best
   population select_best(std::size_t k) const;

   // Generate an entirely new population by selecting worst
   population select_worst(std::size_t k) const;

   // Generate an entirely new population by tournament selection
   population select_tournament(std::size_t k, std::size_t tournament_size) const;

   void crossover_and_mutate(float crossover_rate, float mutation_rate);
   void crossover_or_mutate(std::size_t lambda, float crossover_rate, float mutation_rate);
   void crossover(float crossover_rate);
   void mutate(float mutation_rate);

   void evolve(unsigned generations);

   // Evaluate all individuals
   void evaluate();

   // Get the size of the population
   std::size_t size() const;

   individual & operator[] (int i);
private:
   void print_stats(unsigned generation=0) const;

private:
   static std::function<population(population const &, std::size_t k)> selection_function;
   static std::function<void(population &)> evolution_function;
   // Storage for all individuals in this population
   std::vector<individual> individuals;
};

} // namespace genetic

#endif //GENETIC_POPULATION_HPP
