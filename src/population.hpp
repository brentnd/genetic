#ifndef GENETIC_POPULATION_HPP
#define GENETIC_POPULATION_HPP

#include <algorithm>
#include <cstdlib>
#include <vector>

#include "organism.hpp"
#include "random.hpp"

namespace genetic {

template <class T>
class population {
public:
   // New population using T default constructor
   population(std::size_t size) : organisms(size) {
   }
   // Get any specific organism
   T get_organism(int index) const {
      organisms.at(index);
   }
   // Evolve the population one death, reproduction, and mutation cycle
   bool evolve(unsigned int generations, int goal, double mutation_rate, bool elitism) {
      int best;
      unsigned int current_generation = 0;
      do {
         auto fittest = get_fittest();
         best = fittest.get_fitness();
         std::cout << current_generation << ": " << fittest << std::endl;
         evolution_cycle(mutation_rate, elitism);
      } while ((best < goal) && (++current_generation < generations));
      std::printf("Took %d geneations\n",current_generation);
      return best >= goal;
   }
   // Get the fitness of the entire population
   int get_fitness() const {
      int fitness = 0;
      for (auto ind : organisms) {
         fitness += ind.get_fitness();
      }
      return fitness/get_size();
   }
   // Get the fittest organism in the population
   T get_fittest() const {
      T fittest = organisms[0];
      for (auto ind : organisms) {
         if (fittest.get_fitness() < ind.get_fitness()) {
            fittest = ind;
         }
      }
      return std::move(fittest);
   }
   // Get the size of the population
   std::size_t get_size() const {
      return organisms.size();
   }

private:
   // Remove one third of the population
   void degenerate()  {
      std::sort (organisms.begin(), organisms.end());
      auto end_itr = organisms.begin()+(organisms.size()/3);

      for (auto itr = organisms.begin(); itr != end_itr; itr++) {
         organisms.erase(itr);
      }
   }
   // Evolve the population one death, reproduction, and mutation cycle
   void evolution_cycle(double mutation_rate, bool elitism) {
      /* Death phase */
      degenerate();

      /* Reproduction phase */
      regenerate();

      /* Mutation phase */
      mutate(mutation_rate);
   }
   // Mutate entire population slightly
   void mutate(double mutation_rate) {
      for (auto& ind : organisms) {
         if (random::probability(mutation_rate)) {
            ind.mutate(mutation_rate);
         }
      }
   }
   // Use 2/3 population to regenerate missing 1/3
   void regenerate() {
      auto end_itr = organisms.end();
      end_itr -= organisms.size() % 3;

      for (auto itr = organisms.begin(); itr != end_itr; itr+=2) {
         auto child = T::breed(*itr, *(itr+1));
         organisms.push_back(std::move(child));
      }
   }

   // Storage for all organisms
   std::vector<T> organisms;
};

} // namespace genetic

#endif //GENETIC_POPULATION_HPP
