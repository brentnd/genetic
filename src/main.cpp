#include <stdio.h>
#include <cstdlib>

#include "population.hpp"
#include "individual.hpp"

int main() {

   int max = genetic::individual::get_max_fitness();
   genetic::population pop(999);
   int gen = 0;
   int best;
   do {
      genetic::individual fittest = pop.get_fittest();
      best = fittest.get_fitness();
      printf("Generation %d: Fittness: %d ", gen, best);
      fittest.print_genes();
      if (gen++ > 500) {
         break;
      }
      pop.evolve(0.1, true);
   } while (best < max);
   return 0;
}