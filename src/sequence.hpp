#ifndef GENETIC_SEQUENCE_HPP
#define GENETIC_SEQUENCE_HPP

#include "organism.hpp"

namespace genetic {

class sequence : public organism {
public:
   static int get_max_fitness();
   static sequence crossover(sequence const &a, sequence const &b);

private:
   struct gene {
      int chromosome;

      gene();
      void mutate();
      ~gene();
      bool operator==(gene const &other) const {
         return chromosome==other.chromosome;
      }
   };

public:
   sequence();

   int get_fitness() const override;
   void mutate(double mutation_rate) override;

   bool operator==(sequence const &other) const;
   friend std::ostream& operator<< (std::ostream& stream, const sequence& ind);

private:
   int get_gene(int index) const;
   void set_gene(int index, int const gene);

private:
   static std::string solution;
   std::vector<gene> genes;
   int fitness;
};

}

#endif //GENETIC_SEQUENCE_HPP
