#include <catch.hpp>
#include <geneticpp/individual.hpp>
#include <randomcpp.hpp>

TEST_CASE( "Test for genetic::individual", "[individual]" ) {
   // Predictable random tests (that passed before)
   randomcpp::seed(1);
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
      genetic::individual::mutation_method(&genetic::individual::flip_bit, 0.0f);
      a.mutate();
      a.evaluate();
      REQUIRE(a_dup == a);
      REQUIRE(a_dup.weighted_fitness() == a.weighted_fitness());
      // Mutation > 0.0 should modify genes
      genetic::individual::mutation_method(&genetic::individual::flip_bit, 0.5f);
      a.mutate();
      a.evaluate();
      REQUIRE_FALSE(a_dup == a);
      REQUIRE(a_dup.weighted_fitness() != a.weighted_fitness());
      // Mutation == 1.0 should modify again
      genetic::individual::mutation_method(&genetic::individual::flip_bit, 1.0f);
      a.mutate();
      a.evaluate();
      REQUIRE_FALSE(a_dup == a);
      REQUIRE(a_dup.weighted_fitness() != a.weighted_fitness());
   }

   SECTION ( "crossover results in different children" ) {
      genetic::individual a, b;
      a.seed(); b.seed();
      a.evaluate(); b.evaluate();
      genetic::individual a_old = a, b_old = b;
      genetic::individual::mate(&a, &b);
      a.evaluate(); b.evaluate();
      REQUIRE_FALSE(a == a_old);
      REQUIRE_FALSE(b == b_old);
   }

   SECTION ( "custom crossover function can be set" ) {
      genetic::individual::crossover_method(&genetic::individual::one_point_crossover);
      genetic::individual a, b;
      a.seed(); b.seed();
      a.evaluate(); b.evaluate();
      genetic::individual a_old = a, b_old = b;
      genetic::individual::mate(&a, &b);
      a.evaluate(); b.evaluate();
      REQUIRE_FALSE(a == a_old);
      REQUIRE_FALSE(b == b_old);
   }

   SECTION ( "custom crossover function ordered throws when invalid" ) {
      genetic::individual::crossover_method(&genetic::individual::ordered_crossover);
      genetic::individual::attribute_count = 6;
      genetic::attribute::seed_method([] () {
         return 10; // higher than attribute count
      });
      genetic::individual a, b;
      a.seed(); b.seed();
      a.evaluate(); b.evaluate();
      genetic::individual a_old = a, b_old = b;
      REQUIRE_THROWS(genetic::individual::mate(&a, &b));
   }

   SECTION ( "custom crossover function ordered works" ) {
      genetic::individual::crossover_method(&genetic::individual::ordered_crossover);
      genetic::individual::attribute_count = 10;
      genetic::attribute::seed_method([] () {
         auto i(randomcpp::randint(0, 9));
         return i;
      });
      genetic::individual a, b;
      a.seed(); b.seed();
      a.evaluate(); b.evaluate();
      genetic::individual a_old = a, b_old = b;
      REQUIRE_NOTHROW(genetic::individual::mate(&a, &b));
      a.evaluate(); b.evaluate();
      REQUIRE_FALSE(a == a_old);
      REQUIRE_FALSE(b == b_old);
   }

   SECTION ( "custom crossover function ordered can be set" ) {
      genetic::individual::crossover_method(&genetic::individual::ordered_crossover);
      genetic::individual a, b;
      a.seed(); b.seed();
      a.evaluate(); b.evaluate();
      genetic::individual a_old = a, b_old = b;
      genetic::individual::mate(&a, &b);
      a.evaluate(); b.evaluate();
      REQUIRE_FALSE(a == a_old);
      REQUIRE_FALSE(b == b_old);
   }

   SECTION ( "custom crossover function blend can be set" ) {
      genetic::individual::crossover_method(&genetic::individual::blend_crossover, 0.4f);
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
      genetic::individual::mutation_method(&genetic::individual::flip_bit, 0.5f);
      genetic::individual a;
      a.evaluate();
      REQUIRE(a.weighted_fitness() == 0);
      a.mutate();
      a.evaluate();
      REQUIRE(a.weighted_fitness() != 0);
   }

   SECTION ( "custom mutation gaussian no change" ) {
      genetic::individual::mutation_method(&genetic::individual::gaussian, 1.0f, 0.0f, 0.0f);
      genetic::individual a;
      a.evaluate();
      REQUIRE(a.weighted_fitness() == 0);
      a.mutate();
      a.evaluate();
      REQUIRE(a.weighted_fitness() == 0);
   }

   SECTION ( "custom mutation gaussian with change" ) {
      genetic::individual::mutation_method(&genetic::individual::gaussian, 1.0f, 0.0f, 1.0f);
      genetic::individual a;
      a.evaluate();
      REQUIRE(a.weighted_fitness() == 0);
      a.mutate();
      a.evaluate();
      REQUIRE(a.weighted_fitness() != 0);
   }

   SECTION ( "custom mutation polynomial bound with change" ) {
      genetic::individual::mutation_method(&genetic::individual::polynomial_bounded, 1.0f, 0.2f, 0.1f, 0.9f);
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