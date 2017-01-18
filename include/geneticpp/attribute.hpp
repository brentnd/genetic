#ifndef GENETIC_ATTRIBUTE_HPP
#define GENETIC_ATTRIBUTE_HPP

#include "random.hpp"

#include <functional>

namespace genetic {

class attribute {
public:
   // Custom function for evaluation
   static void seed_method(std::function<double()> && fcn);

   attribute();
   void seed();
   void randomize(double min, double max);
   void flip();
   bool operator==(attribute const & other) const;
   double operator+(attribute const & other) const;

   template <typename T>
   operator T() const {
      return static_cast<T>(value);
   }
private:
   static std::function<double()> seed_function;
   double value;
};

} // namespace genetic

#endif //GENETIC_ATTRIBUTE_HPP
