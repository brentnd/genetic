#ifndef GENETIC_ATTRIBUTE_HPP
#define GENETIC_ATTRIBUTE_HPP

#include "random.hpp"

namespace genetic {

class attribute {
public:
   attribute();
   void seed();
   void randomize(int min, int max);
   void flip();
   bool operator==(attribute const & other) const;
   int operator+(attribute const & other) const;
   operator int() const;
   operator char() const;
private:
   int value;
};

} // namespace genetic

#endif //GENETIC_ATTRIBUTE_HPP
