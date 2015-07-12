#include <stdio.h>
#include <cstdlib>
#include <getopt.h>

#include "population.hpp"
#include "individual.hpp"
#include "random.hpp"

int main( int argc, char * const * argv ) {
   // Parameter defaults
   bool elitism = false;
   int max_generations = 50;
   int population_size = 99;
   double mutation_rate = 0.1;

   opterr = 0;
   char c;
   while ((c = getopt(argc, argv, "eg:m:p:")) != -1) {
      switch (c) {
      case 'e':
         elitism = true;
         break;
      case 'm':
         mutation_rate = atof(optarg);
         break;
      case 'g':
         max_generations = atoi(optarg);
         break;
      case 'p':
         population_size = atoi(optarg);
         break;
      case '?':
         if (optopt == 'g' || optopt == 'p')
            fprintf (stderr, "Option -%c requires an argument.\n", optopt);
         else if (isprint (optopt))
            fprintf (stderr, "Unknown option `-%c'.\n", optopt);
         else
            fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
         break;
      }
   }

   int max = genetic::individual::get_max_fitness();
   random::set_seed(1);
   genetic::population pop(population_size);

   int generation = 0;
   int best;
   do {
      genetic::individual fittest = pop.get_fittest();
      best = fittest.get_fitness();
      printf("Generation %d: Fittness: %d ", generation, best);
      fittest.print_genes();
      if (generation++ > max_generations) {
         break;
      }
      pop.evolve(mutation_rate, elitism);
   } while (best < max);
   return 0;
}