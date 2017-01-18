#include <geneticpp/attribute.hpp>

namespace genetic {

/*static*/ std::function<double()> attribute::seed_function = [] () {
   return random::uniform(0.0, 100.0);
};

/*static*/ std::function<std::ostream &(std::ostream & stream, attribute const & attr)> attribute::display_function =
      [] (std::ostream & stream, attribute const & attr) -> std::ostream & {
         stream << attr.value;
         return stream;
      };

/*static*/ std::string attribute::display_delimiter = ", ";

/*static*/ void attribute::seed_method(std::function<double()> && fcn) {
   seed_function = std::move(fcn);
}

/*static*/ void attribute::display_method(std::function<std::ostream &(std::ostream &, attribute const &)> && fcn){
   display_function = std::move(fcn);
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