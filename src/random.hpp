#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <ctime>
#include <random>
#include <stdexcept>

class random {
public:
   static void set_seed(unsigned int seed);
   static void reset();
   static bool probability(double prob);
   static int i_range(int low, int high);
   static float f_range(float low, float high);
private:
   static void initialize();
   static unsigned int seed;
};


#endif //RANDOM_HPP
