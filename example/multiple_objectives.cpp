#include <stdio.h>
#include <cstdlib>
#include <iostream>

#include <geneticpp.hpp>

int main( int argc, char * const * argv ) {
   random::seed();

   // Population configuration
   std::size_t tournament_size = 3;
   genetic::population::selection_method(&genetic::population::select_tournament, tournament_size);

   // Individual configuration
   genetic::individual::attribute_count = 10;
   genetic::fitness::objective_weights({1.0, 0.5});
   genetic::individual::evaluation_method([] (genetic::individual const & ind) -> std::vector<float> {
      // Maximize even numbers
      float evenness = 0.0;
      for (auto const & attr : ind) {
         if (!(static_cast<int>(attr) % 2)) {
            evenness += 100;
         }
      }
      return {ind.sum_attributes(), evenness};
   });

   unsigned generations = 40;
   std::size_t population_size = 500;
   genetic::population pop(population_size);
   pop.evolve(generations);
   std::printf("population evolved for %d generations\n", generations);
   return 0;
}