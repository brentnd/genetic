#include <geneticpp/attribute.hpp>

namespace genetic {

int value;

attribute::attribute() :
      value(0) {
}

void attribute::seed() {
   // TODO: attribute seeding function
   randomize(' ', 'z');
}

void attribute::randomize(int min, int max) {
   value = random::randint(min, max);
}

void attribute::flip() {
   value = !value;
}

bool attribute::operator==(attribute const & other) const {
   return value == other.value;
}

int attribute::operator+(attribute const & other) const {
   return value + other.value;
}

attribute::operator int() const {
   return value;
}
attribute::operator char() const {
   return static_cast<char>(value);
}

} // namespace genetic