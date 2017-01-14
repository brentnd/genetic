#include "catch.hpp"

#include <algorithm>

#include "../src/population.hpp"
#include "../src/random.hpp"

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
      a.evaluate(); b.evaluate();
      // Two random individuals don't match
      REQUIRE_FALSE(a == b);
      REQUIRE_FALSE(a.evaluate() == b.evaluate());
   }

   SECTION ( "mutations with different rates" ) {
      // Copy constructor
      genetic::individual a;
      a.evaluate();
      genetic::individual a_dup = a;
      REQUIRE(a_dup == a);
      // Mutate (0 = none)
      a.mutate(0.0);
      a.evaluate();
      REQUIRE(a_dup == a);
      REQUIRE(a_dup.evaluate() == a.evaluate());
      // Mutation > 0.0 should modify genes
      a.mutate(0.5);
      a.evaluate();
      REQUIRE_FALSE(a_dup == a);
      REQUIRE(a_dup.evaluate() != a.evaluate());
      // Mutation == 1.0 should modify again
      a.mutate(1.0);
      a.evaluate();
      REQUIRE_FALSE(a_dup == a);
      REQUIRE(a_dup.evaluate() != a.evaluate());
   }

   SECTION ( "breed results in different children" ) {
      genetic::individual a, b;
      a.evaluate(); b.evaluate();
      genetic::individual a_old = a, b_old = b;
      genetic::individual::mate(&a, &b);
      a.evaluate(); b.evaluate();
      REQUIRE_FALSE(a == a_old);
      REQUIRE_FALSE(b == b_old);
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
      a.mutate(0.5);
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

   SECTION ( "population size constant through evoluation, factor of 3" ) {
      genetic::population pop(99);
      REQUIRE(pop.size() == 99);
      pop.evolve(1);
      REQUIRE(pop.size() == 99);
   }

   SECTION ( "evaluation should improve fitness" ) {
      genetic::population pop(99);
      pop.evaluate();
      auto pop_old = pop;
      pop.evolve(1);
      REQUIRE(pop.select_best(1)[0].evaluate() >= pop_old.select_best(1)[0].evaluate());
   }

   SECTION ( "evaluation with select_worst might improve fitness" ) {
      genetic::population::selection_method(&genetic::population::select_worst);
      genetic::population pop(99);
      pop.evaluate();
      auto pop_old = pop;
      pop.evolve(1);
      REQUIRE(pop.select_best(1)[0].evaluate() == pop_old.select_best(1)[0].evaluate());
   }

   SECTION ( "20 evoluation cycles" ) {
      genetic::population pop(99);
      pop.evaluate();
      auto pop_old = pop;
      pop.evolve(20);
      REQUIRE(pop.select_best(1)[0].evaluate() >= pop_old.select_best(1)[0].evaluate());
      REQUIRE(pop.size() == pop_old.size());
   }
}