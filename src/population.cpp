#include <geneticpp/population.hpp>

namespace genetic {

/*static*/ std::function<population(population const &, std::size_t k)> population::selection_function = &population::select_best;
/*static*/ std::function<void(population &)> population::evolution_function =
      std::bind(&population::crossover_and_mutate, std::placeholders::_1, static_cast<float>(0.5), static_cast<float>(0.1));

population::population(std::size_t size_) :
      individuals(size_) {
   for (auto & ind : individuals) {
      ind.seed();
   }
}

population::population(population const & pop) :
      individuals(pop.individuals) {
}

population population::select_random(std::size_t k) const {
   auto selected(random::sample(0, size() - 1, k, false /* not unique */));
   population random_individuals(0);
   for (auto const & index : selected) {
      // Copy from this population to the new one
      random_individuals.individuals.push_back(individuals[index]);
   }
   return std::move(random_individuals);
}

population population::select_best(std::size_t k) const {
   population best_individuals(k);
   std::partial_sort_copy(individuals.begin(), individuals.end(),
                          best_individuals.individuals.begin(), best_individuals.individuals.end(),
                          std::greater<individual>());
   return std::move(best_individuals);
}

population population::select_worst(std::size_t k) const {
   population worst_individuals(k);
   std::partial_sort_copy(individuals.begin(), individuals.end(),
                          worst_individuals.individuals.begin(), worst_individuals.individuals.end());
   return std::move(worst_individuals);
}

population population::select_tournament(std::size_t k, std::size_t tournament_size) const {
   population tournament_champions(0);
   for (unsigned i=0; i < k; i++) {
      auto aspirants(select_random(tournament_size));
      tournament_champions.individuals.push_back(
            std::move(*std::max_element(aspirants.individuals.begin(), aspirants.individuals.end())));
   }
   assert(tournament_champions.size() == k);
   return std::move(tournament_champions);
}

void population::crossover_and_mutate(float crossover_rate, float mutation_rate) {
   crossover(crossover_rate);
   mutate(mutation_rate);
}

void population::crossover(float crossover_rate) {
   for (unsigned i=1; i < size(); i += 2) {
      if (random::probability(crossover_rate)) {
         individual::mate(&individuals[i - 1], &individuals[i]);
      }
   }
}

void population::mutate(float mutation_rate) {
   for (auto & individual : individuals) {
      if (random::probability(mutation_rate)) {
         individual.mutate();
      }
   }
}

void population::evolve(unsigned generations) {
   evaluate();
   for (unsigned gen=0; gen < generations; gen++) {
      print_stats(gen);
      auto offspring(selection_function(*this, size()));
      evolution_function(offspring);
      individuals = std::move(offspring.individuals);
      evaluate();
   }
}

void population::evaluate() {
   for (auto & ind : individuals) {
      ind.evaluate();
   }
}

std::size_t population::size() const {
   return individuals.size();
}

void population::print_stats(unsigned generation /*=0*/) const {
   auto best1(select_best(1)[0]);
   std::printf("Population generation=%d size=%lu, best=%d\n", generation, size(), best1.evaluate());
   std::cout << "Best:" << best1 << std::endl;
}

individual & population::operator[] (int i) {
   return individuals.at(i);
}

} // namespace genetic