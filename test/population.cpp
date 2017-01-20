#include <catch.hpp>

#include <geneticpp/population.hpp>

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

   SECTION ( "evolution with select_roulette and extra args" ) {
      std::size_t tournament_size = 3;
      genetic::population::selection_method(&genetic::population::select_roulette);
      genetic::population pop(50);
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
      genetic::individual::mutation_method(&genetic::individual::uniform_int, 0.1f, 0, 100);
      genetic::population pop(99);
      pop.evaluate();
      auto pop_old = pop;
      pop.evolve(20);
      REQUIRE(pop.select_best(1)[0].weighted_fitness() >= pop_old.select_best(1)[0].weighted_fitness());
      REQUIRE(pop.size() == pop_old.size());
   }
}