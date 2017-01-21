#include <geneticpp/attribute.hpp>

namespace genetic {

/*static*/ std::function<float(unsigned)> attribute::seed_function = [] (unsigned) {
   return random::uniform(0.0, 100.0);
};

/*static*/ std::function<std::ostream &(std::ostream & stream, attribute const & attr)> attribute::display_function =
      [] (std::ostream & stream, attribute const & attr) -> std::ostream & {
         stream << attr.value;
         return stream;
      };

/*static*/ std::string attribute::display_delimiter = ", ";

/*static*/ void attribute::seed_method(std::function<float(unsigned)> && fcn) {
   seed_function = std::move(fcn);
}

/*static*/ void attribute::seed_method(std::function<float()> && fcn) {
   seed_function = [fcn] (unsigned index) {
      static_cast<void>(index);
      return fcn();
   };
}

/*static*/ void attribute::display_method(std::function<std::ostream &(std::ostream &, attribute const &)> && fcn){
   display_function = std::move(fcn);
}

attribute::attribute() :
      value(0.0) {
}

void attribute::seed(unsigned index) {
   value = seed_function(index);
}

void attribute::randomize(float min, float max) {
   value = random::uniform(min, max);
}

void attribute::randomize_int(int min, int max) {
   value = random::randint(min, max);
}

void attribute::flip() {
   value = !value;
}

void attribute::operator=(float val) {
   value = val;
}

bool attribute::operator==(attribute const & other) const {
   return value == other.value;
}

float attribute::operator+(attribute const & other) const {
   return value + other.value;
}

} // namespace genetic