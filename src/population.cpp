#include "population.hpp"

namespace genetic {

population::population(std::size_t size_) :
      organisms(size_) {
}

population::population(population const & pop) :
      organisms(pop.organisms) {
}

population population::select_random(std::size_t k) const {
   auto selected(random::sample(0, size() - 1, k, false /* not unique */));
   population random_organisms(k);
   unsigned i=0;
   for (auto const & index : selected) {
      // Copy from this population to the new one
      random_organisms.organisms[i] = organisms[k];
   }
   return std::move(random_organisms);
}

population population::select_best(std::size_t k) const {
   population best_organisms(k);
   std::partial_sort_copy(organisms.begin(), organisms.end(),
                          best_organisms.organisms.begin(), best_organisms.organisms.end(),
                          std::greater<organism>());
   return std::move(best_organisms);
}

population population::select_worst(std::size_t k) const {
   population worst_organisms(k);
   std::partial_sort_copy(organisms.begin(), organisms.end(),
                          worst_organisms.organisms.begin(), worst_organisms.organisms.end());
   return std::move(worst_organisms);
}

population population::select_tournament(std::size_t k, std::size_t tournament_size) const {
   population tournament_champions(0);
   while (k-- > 0) {
      auto aspirants(select_random(tournament_size));
      tournament_champions.organisms.push_back(
            std::move(*std::max_element(aspirants.organisms.begin(), aspirants.organisms.end())));
   }
   assert(tournament_champions.size() == k);
   return std::move(tournament_champions);
}

void population::crossover_and_mutate(float crossover_rate, float mutation_rate) {
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

void population::evolve(unsigned generations) {
   for (unsigned gen=0; gen < generations; gen++) {
      print_stats(gen);
      // TODO: how to vary select type?
      auto offspring(select_best(size()));
      offspring.crossover_and_mutate(0.2 /* TODO: crossover rate */, 0.08 /* TODO: mutation rate */);
      offspring.evaluate();
      organisms = offspring.organisms;
   }
}

void population::evaluate() {
   for (auto & organism : organisms) {
      organism.evaluate();
   }
}

std::size_t population::size() const {
   return organisms.size();
}

void population::print_stats(unsigned generation /*=0*/) const {
   auto best1(select_best(1)[0]);
   std::printf("Population generation=%d size=%lu, best=%d\n", generation, size(), best1.evaluate());
}

organism & population::operator[] (int i) {
   return organisms.at(i);
}

} // namespace genetic