#include <stdio.h>
#include <cstdlib>
#include <iostream>

#include <cmath>

#include <geneticpp.hpp>
#include <map>

class city {
public:
   city() :
         x(random::randint(0, 200)),
         y(random::randint(0, 200)) {
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

   std::array<city, 20> cities;
   // Create and add our cities
   cities[0] = city(60, 200);
   cities[1] = city(180, 200);
   cities[2] = city(80, 180);
   cities[3] = city(140, 180);
   cities[4] = city(20, 160);
   cities[5] = city(100, 160);
   cities[6] = city(200, 160);
   cities[7] = city(140, 140);
   cities[8] = city(40, 120);
   cities[9] = city(100, 120);
   cities[10] = city(180, 100);
   cities[11] = city(60, 80);
   cities[12] = city(120, 80);
   cities[13] = city(180, 60);
   cities[14] = city(20, 40);
   cities[15] = city(100, 40);
   cities[16] = city(200, 40);
   cities[17] = city(20, 20);
   cities[18] = city(60, 20);
   cities[19] = city(160, 20);

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