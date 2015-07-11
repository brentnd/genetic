#include "population.hpp"

#include <cstdlib>

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

void population::store_individual(int index, individual const &ind) {
   individuals[index] = ind;
}

individual population::tournament_selection(std::size_t tsize) const {
   population tournament(tsize);
   for (unsigned int i=0; i < tsize; i++) {
      auto random_individual = individuals.at( std::rand() % individuals.size() );
      tournament.store_individual(i, random_individual);
   }
   return tournament.get_fittest();
}

void population::evolve() {
   std::vector<individual> new_pop;

   /* Elitism phase */
   if (true /* elite? */) {
      new_pop.push_back(individuals.front());
   }

   /* Crossover phase */
   for (unsigned int i = new_pop.size(); i < individuals.size(); i++) {
      auto a = tournament_selection(25);
      auto b = tournament_selection(25);
      individual child = individual::crossover(a,b);
      new_pop.push_back(child);
   }

   individuals = new_pop;
   /* Mutation phase */
   mutate();
}

void population::mutate() {
   for (auto& ind : individuals) {
      if ((std::rand() % 10) > 5) {
         ind.mutate();
      }
   }
}

}