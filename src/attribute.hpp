#ifndef GENETIC_ATTRIBUTE_HPP
#define GENETIC_ATTRIBUTE_HPP

#include "random.hpp"

namespace genetic {

struct attribute {
   int value;

   attribute() {
      value = random::randint(0, 100);
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
