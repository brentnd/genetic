#include "catch.hpp"
#include "../src/individual.hpp"
#include "../src/population.hpp"

TEST_CASE( "Test for genetic::individual", "[individual]" ) {
   genetic::individual a,b;
   // Two random individuals don't match
   REQUIRE( !(a == b) );
   REQUIRE( a.get_genes() != b.get_genes() );

   // Copy constructor
   genetic::individual a_dup = a;
   REQUIRE( a_dup == a );
   // Mutate (0 = none)
   a.mutate(0.0);
   REQUIRE( a_dup == a );
   REQUIRE( a_dup.get_fitness() == a.get_fitness() );
   // Mutation > 0.0 should modify genes
   a.mutate(0.5);
   REQUIRE( !(a_dup == a) );
   REQUIRE( a_dup.get_fitness() != a.get_fitness() );
   // Mutation == 1.0 should modify again
   a.mutate(1.0);
   REQUIRE( !(a_dup == a) );
   REQUIRE( a_dup.get_fitness() != a.get_fitness() );

   // Crossover (child) doesn't exactly match either parent
   genetic::individual child = genetic::individual::crossover(a, b);
   REQUIRE( !(child == a) );
   REQUIRE( !(child == b) );

   // Crossover (siblings) shouldn't match
   genetic::individual sibling = genetic::individual::crossover(a, b);
   REQUIRE( !(sibling == child) );
   REQUIRE( !(sibling == a) );

   // Genes don't match, fitness shouldn't
   REQUIRE( a.get_fitness() != b.get_fitness() );
}

TEST_CASE( "Test for genetic::population", "[population]" ) {
   // Initialization
   genetic::population pop(100);
   REQUIRE( pop.get_size() == 100 );

   // Evolution should improve
   auto pop_old = pop;
   pop.evolve(0.5, true);
   REQUIRE( pop.get_fittest().get_fitness() >= pop_old.get_fittest().get_fitness() );
}