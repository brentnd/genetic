#include <catch.hpp>

#include <geneticpp/individual.hpp>

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
}