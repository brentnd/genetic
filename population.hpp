#ifndef GENETIC_POPULATION_HPP
#define GENETIC_POPULATION_HPP

#include <cstdlib>
#include <vector>

#include "individual.hpp"

namespace genetic {

class population {
public:
   population(std::size_t size);

   individual get_individual(int index) const {
      individuals.at(index);
   }

   individual get_fittest() const;

   std::size_t get_size() const {
      return individuals.size();
   }

   void store_individual(int index, individual const &ind);

   void evolve();

   individual tournament_selection(std::size_t tsize) const;

private:
   void mutate();

   std::vector<individual> individuals;
};

}

#endif //GENETIC_POPULATION_HPP
