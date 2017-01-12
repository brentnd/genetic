#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <ctime>
#include <random>
#include <stdexcept>
#include <vector>

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
   static bool probability(double probability_);

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
private:
   static void initialize();
   static unsigned seed_value;
};


#endif //RANDOM_HPP
