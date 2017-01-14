#ifndef GENETIC_ATTRIBUTE_HPP
#define GENETIC_ATTRIBUTE_HPP

#include "random.hpp"

namespace genetic {

struct attribute {
   int value;

   attribute() :
         value(0) {
   }

   void seed() {
      // TODO: attribute seeding function
      randomize(0, 100);
   }

   void randomize(int min, int max) {
      value = random::randint(min, max);
   }

   void flip() {
      value = !value;
   }

   bool operator==(attribute const & other) const {
      return value == other.value;
   }

   int operator+(attribute const & other) const {
      return value + other.value;
   }
};

}

#endif //GENETIC_ATTRIBUTE_HPP
