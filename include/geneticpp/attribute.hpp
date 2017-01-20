#pragma once

#include "random.hpp"

#include <iostream>
#include <functional>

namespace genetic {

class attribute {
public:
   // Custom function for evaluation
   static void seed_method(std::function<float()> && fcn);
   // Custom attribute display
   static void display_method(std::function<std::ostream&(std::ostream &, attribute const &)> && fcn);

   attribute();
   void seed();
   void randomize(float min, float max);
   void randomize_int(int min, int max);
   void flip();
   void operator=(float val);
   bool operator==(attribute const & other) const;
   float operator+(attribute const & other) const;

   template <typename T>
   operator T() const {
      return static_cast<T>(value);
   }

   friend std::ostream& operator<<(std::ostream & stream, attribute const & attr) {
      return display_function(stream, attr);
   }

public:
   static std::string display_delimiter;
private:
   static std::function<float()> seed_function;
   static std::function<std::ostream&(std::ostream &, attribute const &)> display_function;
   float value;
};

} // namespace genetic
