#include <geneticpp/random.hpp>

#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <stdexcept>

/*static*/ unsigned random::seed_value = 0;

void random::seed() {
   seed(static_cast<unsigned>(std::time(0)));
}

void random::seed(unsigned seed) {
   random::seed_value = seed;
   initialize();
}

void random::reset() {
   initialize();
}

bool random::probability(float probability_) {
   float r = uniform(0.0f, 1.0f);
   return r <= probability_;
}

int random::randint(int a, int b) {
   if (b < a) {
      throw std::runtime_error("Invalid range");
   }
   int range = b - a;
   return std::rand() % range + a;
}

float random::uniform(float a, float b) {
   if (b < a) {
      throw std::runtime_error("Invalid range");
   }
   float range = b - a;
   float r = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
   return r * range - a;
}

std::vector<int> random::sample(int a, int b, unsigned k, bool unique) {
   if (unique && (b - a) < k) {
      throw std::runtime_error("random vector unique but range is less than count");
   }

   std::vector<int> rand_is;
   while (rand_is.size() < k) {
      int rand_i = randint(a, b);
      if (!unique || std::find(rand_is.begin(), rand_is.end(), rand_i) == rand_is.end()) {
         rand_is.push_back(rand_i);
      }
   }
   return rand_is;
}

/*static*/ void random::initialize() {
   std::srand(seed_value);
}
