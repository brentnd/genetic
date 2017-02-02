#include <stdio.h>
#include <cstdlib>
#include <iostream>

#include <geneticpp.hpp>
#include <randomcpp.hpp>

int main( int argc, char * const * argv ) {
   randomcpp::seed();

   // Population configuration
   std::size_t tournament_size = 3;
   float crossover_rate = 0.5;
   float ind_mutation_rate = 0.1;
   genetic::population::selection_method(&genetic::population::select_tournament, tournament_size);
   genetic::population::evolution_method(&genetic::population::crossover_and_mutate, crossover_rate, ind_mutation_rate);

   // Individual configuration
   char min_attr = ' '; // ASCII 33
   char max_attr = 'z'; // ASCII 122
   float attr_mutation_rate = 0.1;
   char solution[] = "hello world!";
   genetic::individual::crossover_method(&genetic::individual::two_point_crossover);
   genetic::individual::mutation_method(&genetic::individual::uniform_int, attr_mutation_rate, static_cast<int>(min_attr), static_cast<int>(max_attr));
   genetic::individual::attribute_count = sizeof solution - 1;
   genetic::individual::evaluation_method([solution] (genetic::individual const & ind) -> std::vector<float> {
      float fitness = 0.0;
      for (unsigned i=0; i < sizeof solution - 1; i++) {
         char correct = solution[i];
         char attr = ind.at(i);
         if (attr == correct) {
            fitness += 100;
         } else {
            fitness -= std::abs(correct - attr);
         }
      }
      return {fitness};
   });
   genetic::attribute::seed_method([min_attr, max_attr] () {
      return randomcpp::randint(min_attr, max_attr);
   });
   genetic::attribute::display_method([] (std::ostream & stream, genetic::attribute const & attr) -> std::ostream & {
      stream << static_cast<char>(attr);
      return stream;
   });
   genetic::attribute::display_delimiter = "" /* empty string */;

   unsigned generations = 40;
   std::size_t population_size = 500;
   genetic::population pop(population_size);
   pop.evolve(generations);
   std::printf("population evolved for %d generations\n", generations);
   return 0;
}