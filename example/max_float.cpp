#include <stdio.h>
#include <cstdlib>
#include <iostream>

#include <geneticpp.hpp>

int main( int argc, char * const * argv ) {
   random::seed();

   // Population configuration
   std::size_t tournament_size = 3;
   float crossover_rate = 0.5;
   float ind_mutation_rate = 0.1;
   genetic::population::selection_method(&genetic::population::select_tournament, tournament_size);
   genetic::population::evolution_method(&genetic::population::crossover_and_mutate, crossover_rate, ind_mutation_rate);

   // Individual configuration
   float min_attr = 0.0;
   float max_attr = 10.0;
   float attr_mutation_rate = 0.1;
   genetic::individual::mating_method(&genetic::individual::one_point_crossover);
   genetic::individual::mutation_method(&genetic::individual::uniform_int, attr_mutation_rate, static_cast<int>(min_attr), static_cast<int>(max_attr));
   genetic::individual::attribute_count = 10;
   genetic::attribute::seed_method([min_attr, max_attr] () {
      return random::uniform(min_attr, max_attr);
   });

   unsigned generations = 40;
   std::size_t population_size = 500;
   genetic::population pop(population_size);
   pop.evolve(generations);
   std::printf("population evolved for %d generations\n", generations);
   return 0;
}