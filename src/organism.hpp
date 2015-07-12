#ifndef GENETIC_ORGANISM_HPP
#define GENETIC_ORGANISM_HPP

#include <string>
#include <vector>
#include <iostream>

namespace genetic {

class organism {
public:
   virtual int get_fitness() const =0;
   virtual void mutate(double mutation_rate) =0;

   friend std::ostream& operator<< (std::ostream& stream, const organism& ind) {
      stream << "individual @ (" << static_cast<const void *>(&ind) << ") f=" << ind.get_fitness();
      return stream;
   }
   bool operator<(organism const &other) const {
      return get_fitness() < other.get_fitness();
   }
};

}

#endif //GENETIC_ORGANISM_HPP
