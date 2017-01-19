#pragma once

#include <vector>

namespace genetic {

class fitness {
public:
   static void objective_weights(std::initializer_list<float> && weights_);

   fitness();
   bool valid() const;
   void dirty();
   void update(std::vector<float> && values);
   float weighted_fitness() const;

   bool operator<(fitness const & other) const;
   bool operator>(fitness const & other) const;

private:
   void throw_if_invalid() const;

private:
   static std::vector<float> weights;
   std::vector<float> values;
   bool valid_;
};

} // namespace genetic