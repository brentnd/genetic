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
      randomize(' ', 'z');
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

   operator int() const {
      return value;
   }
   operator char() const {
      return static_cast<char>(value);
   }
};

}

#endif //GENETIC_ATTRIBUTE_HPP
