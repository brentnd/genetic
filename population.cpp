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

void population::evolve(double mutation_rate, bool elitism) {
   std::vector<individual> new_pop;

   individual best = get_fittest();

   /* Crossover phase */
   for (unsigned int i = (elitism) ? 1 : 0; i < individuals.size(); i++) {
      auto a = tournament_selection(individuals.size()/10);
      auto b = tournament_selection(individuals.size()/10);
      individual child = individual::crossover(a,b);
      new_pop.push_back(child);
   }

   individuals = new_pop;
   /* Mutation phase */
   mutate(mutation_rate);

   /* Elitism phase */
   if (elitism) {
      individuals.push_back(best);
   }
}

void population::mutate(double mutation_rate) {
   for (auto& ind : individuals) {
      float r = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
      if (r <= mutation_rate) {
         ind.mutate();
      }
   }
}

}
