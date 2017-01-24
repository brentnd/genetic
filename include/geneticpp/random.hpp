#pragma once

#include <vector>
#include <random>

class random {
public:
   /*
    * Initialize the basic random number generator.
    *    Optional argument x can be any hashable object.
    *    If x is omitted or None, current system time is used
    */
   static void seed();
   static void seed(unsigned x);

   /*
    * Reseed the random generator with the stored seed.
    */
   static void reset();
   /*
    * Return value has a <probability_> chance of being true
    */
   static bool probability(float probability_);

   /*
    * Return a random integer N such that a <= N <= b.
    */
   static int randint(int a, int b);

   /*
    * Return a k length list of unique (or not) elements chosen from the range.
    */
   static std::vector<int> sample(int a, int b, unsigned k, bool unique = false);

   /*
    * Return a random floating point number N such that a <= N <= b.
    * Assumes: a <= b
    * TODO: support (b <= N <= a for b < a).
    */
   static float uniform(float a, float b);

   /*
    * Gaussian distribution. mu is the mean, and sigma is the standard deviation.
    */
   static float gauss(float mu, float sigma);
private:
   static void initialize();
   static unsigned seed_value;
   static std::random_device rd;
};
