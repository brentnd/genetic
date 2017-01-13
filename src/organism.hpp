#ifndef GENETIC_ORGANISM_HPP
#define GENETIC_ORGANISM_HPP

#include <algorithm> // std::min, std::swap_ranges
#include <iostream>
#include <iterator>
#include <utility> // std::swap
#include <vector>

#include "random.hpp"

namespace genetic {

class organism_base {
public:
   organism_base() :
         fitness(0),
         valid_fitness(false) {
   }

   // Both modified in place (two-point crossover)
   static void mate(organism_base * ind1, organism_base * ind2) const {
      std::size_t size, point1, point2;
      size = std::min(ind1.size(), ind2.size());
      point1 = random::randint(1, size);
      point2 = random::randint(1, size - 1);
      if (point2 >= point1) {
         ++point2;
      } else {
         std::swap(point1, point2);
      }
      // Swap point1:point2 between individual 1 and 2
      swap(ind1, ind2, point1, point2);

      ind1->valid_fitness = false;
      ind2->valid_fitness = false;
   }

   int evaluate() {
      if (!evaluation_function) {
         throw std::runtime_error("organism has no evaluation function");
      }
      if (!valid_fitness) {
         fitness = evaluation_function();
         valid_fitness = true;
      }
      return fitness;
   }

   friend std::ostream& operator<< (std::ostream& stream, const organism& ind) {
      stream << "individual @ (" << static_cast<const void *>(&ind) << ") f=" << ind.fitness;
      return stream;
   }

   // TODO: how is operator < going to be applied to pointers :(
   bool operator<(organism_base const &other) const {
      if (!valid_fitness || !other.valid_fitness) {
         std::runtime_error("attempting to compare organisms with invalid fitness");
      }
      return fitness < other.fitness;
   }

   void set_evaluation_function(std::function<int()> && fcn) {
      evaluation_function = std::move(fcn);
   }

   virtual void mutate(float mutation_rate) =0;

protected:
   virtual std::size_t size() =0;
   virtual void swap(organism_base * ind1, organism_base * ind1, std::size_t point1, std::size_t point2) =0;

private:
   int fitness;
   bool valid_fitness;
   std::function<int()> evaluation_function;
};

template <typename TAttr>
class organism : public organism_base {
public:
   // Mutate in-place by with random attributes
   virtual void mutate(float mutation_rate) override {
      if (!attribute_generator) {
         throw std::runtime_error("organism has no attribute generator");
      }
      for (auto & attr : *this) {
         if (random::probability(mutation_rate)) {
            attr = attribute_generator();
         }
      }
      valid_fitness = false;
   }

   void set_evaluation_function(std::function<int(organism<TAttr> const &)> && fcn) {
      // Shim lambda to allow base to hold std::function without TAttr
      set_evaluation_function([fcn] () -> int {
         return fnc(*this);
      });
   }

   std::iterator begin() {
      return attributes.begin();
   }

   std::iterator end() {
      return attributes.end();
   }

   virtual std::size_t size() override {
      return attributes.size();
   }

private:
   virtual void swap(organism_base * ind1, organism_base * ind1, std::size_t point1, std::size_t point2) override {
      auto t_ind1 = dynamic_cast<organism<TAttr> *>(ind1);
      auto t_ind2 = dynamic_cast<organism<TAttr> *>(ind2);
      std::swap_ranges(t_ind1->begin() + point1, t_ind1->begin() + point2, t_ind2->begin() + point1);
   }

private:
   std::vector<TAttr> attributes;
   // TODO: default partial specialization for int, float?
   std::function<TAttr()> attribute_generator;
};

}

#endif //GENETIC_ORGANISM_HPP
