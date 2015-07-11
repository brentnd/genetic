#ifndef GENETIC_INDIVIDUAL_HPP
#define GENETIC_INDIVIDUAL_HPP

#include <string>
#include <vector>

namespace genetic {

class individual {
public:

   static std::string const solution;
   static std::size_t const gene_size = 8;

   static int get_max_fitness();

   individual();

   int get_gene(int index) const;
   void set_gene(int index, int const gene);

   int get_fitness() const;

   void print_genes() const;

   void mutate();

   static individual crossover(individual a, individual b);

private:
   struct gene {
      int chromosome;

      gene();
      void mutate();
      ~gene();
   };
   std::vector<gene> genes;
};

}

#endif //GENETIC_INDIVIDUAL_HPP
