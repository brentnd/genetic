#include <stdio.h>
#include <cstdlib>
#include <getopt.h>
#include <iostream>

#include <geneticpp.hpp>

int main( int argc, char * const * argv ) {
   random::reset();

   // Population configuration
   std::size_t tournament_size = 3;
   float crossover_rate = 0.5;
   float ind_mutation_rate = 0.1;
   genetic::population::selection_method(&genetic::population::select_best);
   genetic::population::evolution_method(&genetic::population::crossover_and_mutate, crossover_rate, ind_mutation_rate);

   // Individual configuration
   int min_attr = ' '; // ASCII 33
   int max_attr = 'z'; // ASCII 122
   float attr_mutation_rate = 0.1;
   char solution[] = "hello world!";
   genetic::individual::mating_method(&genetic::individual::two_point_crossover);
   genetic::individual::mutation_method(&genetic::individual::uniform_int, attr_mutation_rate, min_attr, max_attr);
   genetic::individual::attribute_count = sizeof solution - 1;
   genetic::individual::evaluation_method([solution] (genetic::individual const & ind) -> int {
      int fitness = 0;
      for (unsigned i=0; i < sizeof solution - 1; i++) {
         char correct = solution[i];
         char attr = ind.at(i);
         if (attr == correct) {
            fitness += 50;
         } else {
            fitness -= std::abs(correct - attr);
         }
      }
      return fitness;
   });

   unsigned generations = 200;
   std::size_t population_size = 5000;
   genetic::population pop(population_size);
   pop.evolve(generations);
   std::printf("population evolved for %d generations\n", generations);
   return 0;
}