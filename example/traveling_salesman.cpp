#include <stdio.h>
#include <cstdlib>
#include <iostream>

#include <cmath>

#ifdef WITH_OPENCV2
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#endif

#include <geneticpp.hpp>
#include <randomcpp.hpp>

static int x_size = 400;
static int y_size = 400;

struct city {
   city() :
         x(randomcpp::randint(0, x_size)),
         y(randomcpp::randint(0, y_size)) {
   }

   city(unsigned x, unsigned y) :
         x(x),
         y(y) {
   }

   float distance_to(city const & other) const {
      unsigned x_dist = std::abs(x - other.x);
      unsigned y_dist = std::abs(y - other.y);
      return std::sqrt( x_dist*x_dist + y_dist*y_dist );
   }

   unsigned x, y;
};

static std::array<city, 20> cities;

#ifdef WITH_OPENCV2
static void display(genetic::individual const & ind) {
   // Create black empty images
   cv::Mat image = cv::Mat::zeros(x_size, y_size, CV_8UC3);
   // White background
   image.setTo(cv::Scalar(255, 255, 255));

   city * last = &cities[ind.at(0)];
   city * first = last;
   for (auto const & attr : ind) {
      city *now = &cities[attr];
      // Draw a line
      cv::line(image, cv::Point(last->x, y_size - last->y) /*start*/, cv::Point(now->x, y_size - now->y) /*end*/, cv::Scalar(50, 50, 50) /*gray*/);
      last = now;
   }
   cv::line(image, cv::Point(last->x, y_size - last->y) /*start*/, cv::Point(first->x, y_size - first->y) /*end*/, cv::Scalar(50, 50, 50) /*gray*/);

   for (auto const & city : cities) {
      cv::circle(image, cv::Point(city.x, y_size - city.y), 4 /*radius*/, cv::Scalar(0, 0, 255) /*red*/, -1 /*filled*/);
   }

   imshow("Traveling Salesman Route", image);
   cv::waitKey(0);
}
#endif

int main( int argc, char * const * argv ) {
   randomcpp::seed();

   // Population configuration
   std::size_t tournament_size = 3;
   genetic::population::selection_method(&genetic::population::select_tournament, tournament_size);

   // Fitness config
   genetic::fitness::objective_weights({-1.0f});

   // Individual configuration
   float attr_mutation_rate = 0.2;
   genetic::individual::crossover_method(&genetic::individual::ordered_crossover);
   genetic::individual::mutation_method(&genetic::individual::shuffle_indexes, attr_mutation_rate);
   genetic::individual::attribute_count = cities.size();
   genetic::individual::evaluation_method([] (genetic::individual const & ind) -> std::vector<float> {
      float distance = 0.0;
      unsigned i = ind.at(0);
      city * last_city = &cities.at(i);
      for (auto const & attr : ind) {
         i = attr;
         city * this_city = &cities.at(i);
         distance += last_city->distance_to(*this_city);
         last_city = this_city;
      }
      // Return home
      distance += last_city->distance_to(cities.at(ind.at(0)));
      return {distance};
   });

   genetic::attribute::seed_method([] (unsigned index) {
      // Index is necessary for seeding in this case to ensure individual attributes are unique
      return static_cast<float>(index);
   });
   genetic::attribute::display_method([] (std::ostream & stream, genetic::attribute const & attr) -> std::ostream & {
      stream << static_cast<unsigned>(attr);
      return stream;
   });

   unsigned generations = 100;
   std::size_t population_size = 100;
   genetic::population pop(population_size);
   pop.evolve(generations);
   std::printf("population evolved for %d generations\n", generations);

#ifdef WITH_OPENCV2
   display(pop.select_best(1)[0]);
#endif
   return 0;
}