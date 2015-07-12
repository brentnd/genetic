#include <stdio.h>
#include <cstdlib>
#include <getopt.h>
#include <iostream>

#include "population.hpp"
#include "sequence.hpp"
#include "random.hpp"

int main( int argc, char * const * argv ) {
   // Parameter defaults
   bool elitism = false;
   unsigned int max_generations = 500;
   unsigned int population_size = 999;
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

   int max = genetic::sequence::get_max_fitness();
   random::reset();
   genetic::population<genetic::sequence> pop(population_size);
   bool success = pop.evolve(max_generations, max, mutation_rate, elitism);
   if (success) {
      std::printf ("successfully reached maximum fitness with solution:\n\t");
   } else {
      std::printf ("Evolved for %d generations and found:\n\t",max_generations);
   }
   std::cout << pop.get_fittest() << std::endl;
   return 0;
}