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

population population::select_roulette(std::size_t k) const {
   population chosen(0);
   auto sorted_individuals(individuals);
   std::sort(sorted_individuals.begin(), sorted_individuals.end(), std::greater<individual>());
   float population_sum = std::accumulate(individuals.begin(), individuals.end(), 0.0f,
                                          [] (float current_sum, individual const & ind) -> float {
                                             return current_sum + ind.weighted_fitness();
                                          });
   float target_sum, spin_sum;
   for (unsigned i=0; i < k; i++) {
      target_sum = random::uniform(0.0, 1.0) * population_sum;
      spin_sum = 0.0;
      for (auto const & ind : sorted_individuals) {
         spin_sum += ind.weighted_fitness();
         if (spin_sum > target_sum) {
            chosen.individuals.push_back(ind);
            break;
         }
      }
   }
   assert(chosen.size() == k);
   return std::move(chosen);
}

void population::crossover_and_mutate(float crossover_rate, float mutation_rate) {
   crossover(crossover_rate);
   mutate(mutation_rate);
}

void population::crossover_or_mutate(std::size_t lambda, float crossover_rate, float mutation_rate) {
   if (crossover_rate + mutation_rate > 1.0) {
      throw std::logic_error("The sum of the crossover and mutation probabilities must be smaller or equal to 1.0");
   }
   population offspring(0);
   offspring.individuals.reserve(lambda);
   for (unsigned i=0; i < lambda; i++) {
      float op_choice = random::uniform(0.0f, 1.0f);
      if (op_choice < crossover_rate) {
         auto mate_pop(select_random(2));
         individual::mate(&mate_pop.individuals[0], &mate_pop.individuals[1]);
         offspring.individuals.push_back(std::move(mate_pop.individuals[0]));
      } else if (op_choice < (crossover_rate + mutation_rate)) {
         auto mut_pop(select_random(1));
         mut_pop[0].mutate();
         offspring.individuals.push_back(std::move(mut_pop.individuals[0]));
      } else {
         auto repro(select_random(1));
         offspring.individuals.push_back(std::move(repro.individuals[0]));
      }
   }
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
      auto best(select_best(1)[0]);
      hall_of_fame_individuals.push_back(std::move(best));
      print_stats();
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

void population::print_stats() const {
   std::printf("Population generation=%lu size=%lu, best=%f\n", hall_of_fame_individuals.size(), size(), hall_of_fame_individuals.back().weighted_fitness());
   std::cout << "Best:" << hall_of_fame_individuals.back() << std::endl;
}

individual & population::operator[] (int i) {
   return individuals.at(i);
}

individual const & population::hall_of_fame(unsigned generation) {
   return hall_of_fame_individuals.at(generation);
}

} // namespace genetic