#ifndef GENETIC_POPULATION_HPP
#define GENETIC_POPULATION_HPP

#include <algorithm>
#include <cstdlib>
#include <vector>

#include "individual.hpp"
#include "random.hpp"

namespace genetic {

template <class T>
class population {
public:
   population(std::size_t size) : individuals(size) {
   }

   T get_individual(int index) const {
      individuals.at(index);
   }

   T get_fittest() const {
      T fittest = individuals[0];
      for (auto ind : individuals) {
         if (fittest.get_fitness() < ind.get_fitness()) {
            fittest = ind;
         }
      }
      return std::move(fittest);
   }

   int get_fitness() const {
      int fitness = 0;
      for (auto ind : individuals) {
         fitness += ind.get_fitness();
      }
      return fitness/get_size();
   }

   std::size_t get_size() const {
      return individuals.size();
   }

   void store_individual(int index, T const &ind)  {
      individuals[index] = ind;
   }

   void evolve(double mutation_rate, bool elitism)  {
      /* Death phase */
      death();

      /* Reproduction phase */
      repopulate();

      /* Mutation phase */
      mutate(mutation_rate);
   }

   void death()  {
      std::sort (individuals.begin(), individuals.end());
      auto end_itr = individuals.begin()+(individuals.size()/3);

      for (auto itr = individuals.begin(); itr != end_itr; itr++) {
         individuals.erase(itr);
      }
   }
   void repopulate() {
      auto end_itr = individuals.end();
      end_itr -= individuals.size() % 3;

      for (auto itr = individuals.begin(); itr != end_itr; itr+=2){
         auto child = T::crossover(*itr, *(itr+1));
         individuals.push_back(std::move(child));
      }
   }

private:
   void mutate(double mutation_rate) {
      for (auto& ind : individuals) {
         if (random::probability(mutation_rate)) {
            ind.mutate(mutation_rate);
         }
      }
   }

   std::vector<T> individuals;
};
}

#endif //GENETIC_POPULATION_HPP
