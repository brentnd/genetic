#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <ctime>
#include <random>
#include <stdexcept>

class random {
public:
   static void set_seed(unsigned int seed) {
      random::seed = seed;
      initialize();
   }
   static void reset() {
      if (seed == 0) {
         seed = static_cast <unsigned int> (time(0));
      }
      initialize();
   }
   static bool probability(double prob) {
      float r = f_range(0,1);
      return r <= prob;
   }
   static int i_range(int low, int high) {
      if (high < low) {
         throw std::runtime_error("Invalid range");
      }
      int range = high - low;
      return std::rand() % range + low;
   }
   static float f_range(float low, float high) {
      if (high < low) {
         throw std::runtime_error("Invalid range");
      }
      float range = high - low;
      float r = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
      return r * range - low;
   }
private:
   static void initialize() {
      std::srand (seed);
   }
   static unsigned int seed;
};


#endif //RANDOM_HPP
