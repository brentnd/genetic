#include <geneticpp/fitness.hpp>
#include <stdexcept>

namespace genetic {

/*static*/ std::vector<float> fitness::weights = { 1.0 };

/*static*/ void fitness::objective_weights(std::initializer_list<float> && weights_) {
   weights = std::move(weights_);
}

fitness::fitness() :
      values(weights.size()),
      valid_(false) {
}

bool fitness::valid() const {
   return valid_;
}

void fitness::dirty() {
   valid_ = false;
}

void fitness::update(std::vector<float> && values_) {
   values = std::move(values_);
   valid_ = true;
}

float fitness::weighted_fitness() const {
   throw_if_invalid();
   float fitness = 0.0;
   for (unsigned i=0; i < weights.size(); i++) {
      fitness += weights[i] * values[i];
   }
   return fitness;
}

void fitness::throw_if_invalid() const {
   if (!valid_) {
      throw std::runtime_error("attempting to use invalid fitness");
   }
}

bool fitness::operator<(fitness const & other) const {
   return weighted_fitness() < other.weighted_fitness();
}

bool fitness::operator>(fitness const & other) const {
   return weighted_fitness() > other.weighted_fitness();
}

} // namespace genetic