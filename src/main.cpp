#include <stdio.h>
#include <cstdlib>
#include <getopt.h>
#include <iostream>

#include "population.hpp"
#include "random.hpp"

int main( int argc, char * const * argv ) {
   // Parameter defaults
   bool elitism = false;
   unsigned max_generations = 500;
   unsigned population_size = 999;
   double mutation_rate = 0.1;

   ::opterr = 0;
   char c;
   while ((c = ::getopt(argc, argv, "eg:m:p:")) != -1) {
      switch (c) {
      case 'e':
         elitism = true;
         break;
      case 'm':
         mutation_rate = std::atof(::optarg);
         break;
      case 'g':
         max_generations = std::atoi(::optarg);
         break;
      case 'p':
         population_size = std::atoi(::optarg);
         break;
      case '?':
         if (optopt == 'g' || optopt == 'p') {
            std::fprintf(stderr, "Option -%c requires an argument.\n", optopt);
         } else if (isprint (optopt)) {
            std::fprintf(stderr, "Unknown option `-%c'.\n", optopt);
         } else {
            std::fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
         }
         break;
      }
   }

   random::reset();
   std::size_t tournament_size = 3;
   genetic::population::selection_method(&genetic::population::select_tournament, tournament_size);
   genetic::population pop(population_size);
   pop.evolve(max_generations);
   std::printf("population evolved for %d generations\n", max_generations);
   return 0;
}