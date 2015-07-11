#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <random>

#include "population.hpp"
#include "individual.hpp"

int main() {
   std::srand (time(nullptr));
   genetic::population pop(50);

   int gen = 0;
   int max = genetic::individual::get_max_fitness();
   int best;
   do {
      genetic::individual fittest = pop.get_fittest();
      best = fittest.get_fitness();
      printf("Generation %d: Fittness: %d ", gen++, best);
      fittest.print_genes();
      if (gen > 500) {
         break;
      }
      pop.evolve();
   } while (best < max);
   return 0;
}