#ifndef GENETIC_SEQUENCE_HPP
#define GENETIC_SEQUENCE_HPP

#include "individual.hpp"

namespace genetic {

class sequence : public individual {
public:
   struct gene {
      int chromosome;

      gene();
      void mutate();
      ~gene();
      bool operator==(gene const &other) const {
         return chromosome==other.chromosome;
      }
   };

   static int get_max_fitness();
   static sequence crossover(sequence const &a, sequence const &b);

   sequence();

   int get_fitness() const override;
   void mutate(double mutation_rate) override;
   bool operator==(sequence const &other) const;

   friend std::ostream& operator<< (std::ostream& stream, const sequence& ind) {
      stream << "individual f=" << ind.get_fitness() << " (";
      for (auto gene : ind.genes) {
         stream << static_cast<char>(gene.chromosome);
      }
      stream << ")";
      return stream;
   }

   int get_gene(int index) const;
   std::vector<gene> const & get_genes() const {
      return genes;
   }
   void set_gene(int index, int const gene);

private:

   static std::string const solution;

   std::vector<gene> genes;
   int fitness;
};

}

#endif //GENETIC_SEQUENCE_HPP
