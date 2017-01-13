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
   population(std::size_t size_) :
         organisms(size_) {
   }

   population(population const & pop) :
         organisms(pop.organisms) {
   }

   // Generate an entirely new population by random selection
   population select_random(std::size_t k) {
      auto selected(random::sample(0, size() - 1, k, false /* not unique */));
      population random_organisms(k);
      unsigned i=0;
      for (auto const & index : selected) {
         // Copy from this population to the new one
         random_organisms.organisms[i] = organisms[k];
      }
      return std::move(random_organisms);
   }

   // Generate an entirely new population by selecting best
   population select_best(std::size_t k) {
      population best_organisms(k);
      std::partial_sort_copy(organisms.begin(), organisms.end(),
                             best_organisms.organisms.begin(), best_organisms.organisms.end(),
                             std::greater<organism>());
      return std::move(best_organisms);
   }

   // Generate an entirely new population by selecting worst
   population select_worst(std::size_t k) {
      population worst_organisms(k);
      std::partial_sort_copy(organisms.begin(), organisms.end(),
                             worst_organisms.organisms.begin(), worst_organisms.organisms.end());
      return std::move(worst_organisms);
   }

   // Generate an entirely new population by tournament selection
   population select_tournament(std::size_t k, std::size_t tournament_size) {
      population tournament_champions(0);
      while (k-- > 0) {
         auto aspirants(select_random(tournament_size));
         tournament_champions.organisms.push_back(
               std::move(*std::max_element(aspirants.organisms.begin(), aspirants.organisms.end())));
      }
      assert(tournament_champions.size() == k);
      return std::move(tournament_champions);
   }

   void crossover_and_mutate(float crossover_rate, float mutation_rate) {
      for (unsigned i=1; i < size(); i += 2) {
         if (random::probability(crossover_rate)) {
            organism::mate(&organisms[i - 1], &organisms[i]);
         }
      }

      for (auto & individual : organisms) {
         if (random::probability(mutation_rate)) {
            individual.mutate(mutation_rate /* TODO: should be separate attr mutation prob */);
         }
      }
   }

   void evolve(unsigned generations) {
      for (unsigned gen=0; gen < generations; gen++) {
         // TODO: how to vary select type?
         auto offspring(select_best(size()));
         offspring.crossover_and_mutate(0.2 /* TODO: crossover rate */, 0.08 /* TODO: mutation rate */);
         offspring.evaluate();
         organisms = offspring.organisms;
      }
   }

   // Evaluate all individuals
   void evaluate() {
      std::for_each(organisms.begin(), organisms.end(), std::bind1st(std::mem_fun(&organism::evaluate), this));
   }

   // Get the size of the population
   std::size_t size() const {
      return organisms.size();
   }

private:
   // Storage for all individuals in this population
   std::vector<organism> organisms;
};

} // namespace genetic

#endif //GENETIC_POPULATION_HPP
