#include <stdio.h>
#include <cstdlib>
#include <iostream>

#include <cmath>

#include <geneticpp.hpp>
#include <map>

class city {
public:
   city() :
         x(random::randint(0, 400)),
         y(random::randint(0, 400)) {
   }

   city(unsigned x, unsigned y) :
         x(x),
         y(y) {
   }

   float distance_to(city const & other) const {
      unsigned x_dist = std::abs(x - other.x);
      unsigned y_dist = std::abs(y - other.y);
      return std::sqrt( x_dist*x_dist + y_dist*y_dist );
   }

private:
   unsigned x, y;
};

int main( int argc, char * const * argv ) {
   random::seed();

   std::array<city, 30> cities;

   // Population configuration
   std::size_t tournament_size = 3;
   genetic::population::selection_method(&genetic::population::select_tournament, tournament_size);

   // Fitness config
   genetic::fitness::objective_weights({-1.0f});

   // Individual configuration
   float attr_mutation_rate = 0.2;
   genetic::individual::crossover_method(&genetic::individual::ordered_crossover);
   genetic::individual::mutation_method(&genetic::individual::shuffle_indexes, attr_mutation_rate);
   genetic::individual::attribute_count = cities.size();
   genetic::individual::evaluation_method([&cities] (genetic::individual const & ind) -> std::vector<float> {
      float distance = 0.0;
      unsigned i = ind.at(0);
      city * last_city = &cities.at(i);
      for (auto const & attr : ind) {
         i = attr;
         city * this_city = &cities.at(i);
         distance += last_city->distance_to(*this_city);
         last_city = this_city;
      }
      // Return home
      distance += last_city->distance_to(cities.at(ind.at(0)));
      return {distance};
   });

   genetic::attribute::seed_method([] (unsigned index) {
      // Index is necessary for seeding in this case to ensure individual attributes are unique
      return static_cast<float>(index);
   });
   genetic::attribute::display_method([] (std::ostream & stream, genetic::attribute const & attr) -> std::ostream & {
      stream << static_cast<unsigned>(attr);
      return stream;
   });

   unsigned generations = 100;
   std::size_t population_size = 100;
   genetic::population pop(population_size);
   pop.evolve(generations);
   std::printf("population evolved for %d generations\n", generations);
   return 0;
}