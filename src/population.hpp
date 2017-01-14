#ifndef GENETIC_POPULATION_HPP
#define GENETIC_POPULATION_HPP

#include <algorithm>
#include <cstdlib>
#include <vector>
#include <cassert>

#include "organism.hpp"
#include "random.hpp"

namespace genetic {

class population {
public:
   population(std::size_t size_);

   population(population const & pop);

   // Generate an entirely new population by random selection
   population select_random(std::size_t k) const;

   // Generate an entirely new population by selecting best
   population select_best(std::size_t k) const;

   // Generate an entirely new population by selecting worst
   population select_worst(std::size_t k) const;

   // Generate an entirely new population by tournament selection
   population select_tournament(std::size_t k, std::size_t tournament_size) const;

   void crossover_and_mutate(float crossover_rate, float mutation_rate);

   void evolve(unsigned generations);

   // Evaluate all individuals
   void evaluate();

   // Get the size of the population
   std::size_t size() const;

   organism & operator[] (int i);
private:
   void print_stats(unsigned generation=0) const;

private:
   // Storage for all individuals in this population
   std::vector<organism> organisms;
};

} // namespace genetic

#endif //GENETIC_POPULATION_HPP
