#include "catch.hpp"

#include <algorithm>

#include "../include/geneticpp/population.hpp"
#include "../include/geneticpp/random.hpp"

TEST_CASE( "Test for random", "[random]" ) {
   random::seed(1);

   SECTION ( "random float inside range" ) {
      float rand_f = random::uniform(0, 0.5);
      REQUIRE(rand_f < 0.5);
      REQUIRE(rand_f > 0.0);
   }

   SECTION ( "random integer inside positive range" ) {
      int rand_i = random::randint(0, 5);
      REQUIRE(rand_i <= 5);
      REQUIRE(rand_i >= 0);
   }

   SECTION ( "random integer inside negative range" ) {
      int rand_i = random::randint(-10, -5);
      REQUIRE(rand_i <= -5);
      REQUIRE(rand_i >= -10);
   }

   SECTION ( "random integer range with both" ) {
      int rand_neg = random::randint(-5, 5);
      REQUIRE(rand_neg <= 5);
      REQUIRE(rand_neg >= -5);
   }

   SECTION ( "repeat random integers not equal" ) {
      int rand_1 = random::randint(0, 100);
      int rand_2 = random::randint(0, 100);
      REQUIRE(rand_1 != rand_2);
   }

   SECTION ( "random reset produces repeatable results" ) {
      random::reset();
      int rand_1 = random::randint(0, 100);
      random::reset();
      int rand_2 = random::randint(0, 100);
      REQUIRE(rand_1 == rand_2);
   }

   SECTION ( "different seeds, different results" ) {
      random::seed(2);
      int rand_1 = random::randint(0, 100);
      random::seed(3);
      int rand_2 = random::randint(0, 100);
      REQUIRE(rand_1 != rand_2);
   }

   SECTION ( "random vector all within range" ) {
      random::seed(2);
      auto rand_set = random::sample(0, 100, 20);
      // Check the size
      REQUIRE(rand_set.size() == 20);
      // Check the range
      for (auto i : rand_set) {
         CHECK(i <= 100);
         CHECK(i >= 0);
      }
   }

   SECTION ( "random vector all unique" ) {
      random::reset();
      auto rand_set = random::sample(0, 100, 20, true);
      // Check the size
      REQUIRE(rand_set.size() == 20);
      // Check for uniqueness
      sort( rand_set.begin(), rand_set.end() );
      REQUIRE( adjacent_find(rand_set.begin(), rand_set.end() ) == rand_set.end() );
      // Check the range
      for (auto i : rand_set) {
         CHECK(i <= 100);
         CHECK(i >= 0);
      }
   }

   SECTION ( "random vector invalid range" ) {
      random::reset();
      REQUIRE_THROWS(random::sample(0, 5, 20, true));
   }
}

TEST_CASE( "Test for genetic::individual", "[individual]" ) {
   // Predictable random tests (that passed before)
   random::seed(1);
   SECTION ( "genes of two rand inits are different" ) {
      genetic::individual a, b;
      a.seed(); b.seed();
      a.evaluate(); b.evaluate();
      // Two random individuals don't match
      REQUIRE_FALSE(a == b);
      REQUIRE_FALSE(a.weighted_fitness() == b.weighted_fitness());
   }

   SECTION ( "mutations with different rates" ) {
      // Copy constructor
      genetic::individual a;
      a.evaluate();
      genetic::individual a_dup = a;
      REQUIRE(a_dup == a);
      // Mutate (0 = none)
      genetic::individual::mutation_method(&genetic::individual::flip_bit, static_cast<float>(0.0));
      a.mutate();
      a.evaluate();
      REQUIRE(a_dup == a);
      REQUIRE(a_dup.weighted_fitness() == a.weighted_fitness());
      // Mutation > 0.0 should modify genes
      genetic::individual::mutation_method(&genetic::individual::flip_bit, static_cast<float>(0.5));
      a.mutate();
      a.evaluate();
      REQUIRE_FALSE(a_dup == a);
      REQUIRE(a_dup.weighted_fitness() != a.weighted_fitness());
      // Mutation == 1.0 should modify again
      genetic::individual::mutation_method(&genetic::individual::flip_bit, static_cast<float>(1.0));
      a.mutate();
      a.evaluate();
      REQUIRE_FALSE(a_dup == a);
      REQUIRE(a_dup.weighted_fitness() != a.weighted_fitness());
   }

   SECTION ( "mating results in different children" ) {
      genetic::individual a, b;
      a.seed(); b.seed();
      a.evaluate(); b.evaluate();
      genetic::individual a_old = a, b_old = b;
      genetic::individual::mate(&a, &b);
      a.evaluate(); b.evaluate();
      REQUIRE_FALSE(a == a_old);
      REQUIRE_FALSE(b == b_old);
   }

   SECTION ( "custom mating function can be set" ) {
      genetic::individual::mating_method(&genetic::individual::one_point_crossover);
      genetic::individual a, b;
      a.seed(); b.seed();
      a.evaluate(); b.evaluate();
      genetic::individual a_old = a, b_old = b;
      genetic::individual::mate(&a, &b);
      a.evaluate(); b.evaluate();
      REQUIRE_FALSE(a == a_old);
      REQUIRE_FALSE(b == b_old);
   }

   SECTION ( "custom mutation function can be set" ) {
      genetic::individual::mutation_method(&genetic::individual::flip_bit, static_cast<float>(0.5));
      genetic::individual a;
      a.evaluate();
      REQUIRE(a.weighted_fitness() == 0);
      a.mutate();
      a.evaluate();
      REQUIRE(a.weighted_fitness() != 0);
   }

   SECTION ( "custom evaluation function can be set" ) {
      genetic::individual::evaluation_method([] (genetic::individual const & ind) -> std::vector<float> {
         return {42};
      });
      genetic::individual a;
      a.evaluate();
      REQUIRE(a.weighted_fitness() == 42);
   }

   SECTION ( "fitness evaluation" ) {
      // Both invalid
      genetic::individual a, b;
      REQUIRE_THROWS(a == b);
      REQUIRE_THROWS(a > b);
      REQUIRE_THROWS(a < b);

      // B invalid
      a.evaluate();
      REQUIRE_THROWS(a == b);
      REQUIRE_THROWS(a > b);
      REQUIRE_THROWS(a < b);

      // A mutation caused invalid, but B valid
      a.mutate();
      b.evaluate();
      REQUIRE_THROWS(a == b);
      REQUIRE_THROWS(a > b);
      REQUIRE_THROWS(a < b);

      a.evaluate();
      REQUIRE_NOTHROW(a == b);
      REQUIRE_NOTHROW(a > b);
      REQUIRE_NOTHROW(a < b);
   }
}

TEST_CASE( "Test for genetic::population", "[population]" ) {
   // Predictable random tests (that passed before)
   random::seed(1);
   genetic::individual::evaluation_method(&genetic::individual::eval_sum);

   SECTION ( "population size constant through evolution, factor of 3" ) {
      genetic::population pop(99);
      REQUIRE(pop.size() == 99);
      pop.evolve(1);
      REQUIRE(pop.size() == 99);
   }

   SECTION ( "evolution should improve fitness" ) {
      genetic::population pop(99);
      pop.evaluate();
      auto pop_old = pop;
      pop.evolve(1);
      REQUIRE(pop.select_best(1)[0].weighted_fitness() >= pop_old.select_best(1)[0].weighted_fitness());
   }

   SECTION ( "evolution with select_worst might improve fitness" ) {
      genetic::population::selection_method(&genetic::population::select_worst);
      genetic::population pop(99);
      pop.evaluate();
      auto pop_old = pop;
      pop.evolve(1);
   }

   SECTION ( "evolution with select_tournament and extra args" ) {
      std::size_t tournament_size = 3;
      genetic::population::selection_method(&genetic::population::select_tournament, tournament_size);
      genetic::population pop(99);
      pop.evaluate();
      auto pop_old = pop;
      pop.evolve(1);
   }

   SECTION ( "evolution with custom evolution function and extra args" ) {
      std::size_t tournament_size = 3;
      float crossover_rate = 0.5;
      float mutation_rate = 0.1;
      genetic::population::evolution_method(&genetic::population::crossover_and_mutate, crossover_rate, mutation_rate);
      genetic::population pop(99);
      pop.evaluate();
      auto pop_old = pop;
      pop.evolve(1);
      REQUIRE(pop.select_best(1)[0].weighted_fitness() >= pop_old.select_best(1)[0].weighted_fitness());
   }

   SECTION ( "20 evolution cycles" ) {
      genetic::individual::mutation_method(&genetic::individual::uniform_int, static_cast<float>(0.1), 0.0, 100.0);
      genetic::population pop(99);
      pop.evaluate();
      auto pop_old = pop;
      pop.evolve(20);
      REQUIRE(pop.select_best(1)[0].weighted_fitness() >= pop_old.select_best(1)[0].weighted_fitness());
      REQUIRE(pop.size() == pop_old.size());
   }
}