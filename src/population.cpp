#include "population.hpp"

#include <cstdlib>
#include <algorithm>

namespace genetic {

population::population(std::size_t size) : individuals(size) {
}

individual population::get_fittest() const {
   individual fittest = individuals[0];
   for (auto ind : individuals) {
      if (fittest.get_fitness() < ind.get_fitness()) {
         fittest = ind;
      }
   }
   return std::move(fittest);
}

int population::get_fitness() const {
   int fitness = 0;
   for (auto ind : individuals) {
      fitness += ind.get_fitness();
   }
   return fitness/get_size();
}

void population::store_individual(int index, individual const &ind) {
   individuals[index] = ind;
}

void population::death() {
   std::sort (individuals.begin(), individuals.end());
   auto end_itr = individuals.begin()+(individuals.size()/3);

   for (auto itr = individuals.begin(); itr != end_itr; itr++) {
      individuals.erase(itr);
   }
}

void population::repopulate() {
   auto end_itr = individuals.end();
   end_itr -= individuals.size() % 3;

   for (auto itr = individuals.begin(); itr != end_itr; itr+=2){
      auto child = individual::crossover(*itr, *(itr+1));
      individuals.push_back(std::move(child));
   }
}

void population::evolve(double mutation_rate, bool elitism) {
   /* Death phase */
   death();

   /* Reproduction phase */
   repopulate();

   /* Mutation phase */
   mutate(mutation_rate);
}

void population::mutate(double mutation_rate) {
   for (auto& ind : individuals) {
      float r = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
      if (r <= mutation_rate) {
         ind.mutate(mutation_rate);
      }
   }
}

}
