#include <geneticpp/attribute.hpp>

namespace genetic {

/*static*/ std::function<double()> attribute::seed_function = [] () {
   return random::uniform(0.0, 100.0);
};

/*static*/ void attribute::seed_method(std::function<double()> && fcn) {
   seed_function = std::move(fcn);
}

attribute::attribute() :
      value(0.0) {
}

void attribute::seed() {
   value = seed_function();
}

void attribute::randomize(double min, double max) {
   value = random::uniform(min, max);
}

void attribute::flip() {
   value = !value;
}

bool attribute::operator==(attribute const & other) const {
   return value == other.value;
}

double attribute::operator+(attribute const & other) const {
   return value + other.value;
}

} // namespace genetic