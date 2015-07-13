#ifndef GENETIC_SEQUENCE_HPP
#define GENETIC_SEQUENCE_HPP

#include "organism.hpp"

namespace genetic {

class sequence : public organism {
public:
   // Breed two sequences together (crossover)
   static sequence breed(sequence const &a, sequence const &b);
   // Calculate fitness of correct sequence
   static int get_max_fitness();
   // Change the solution
   static void set_solution(std::string const & solution);

private:
   // Struct for default (random) construction
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
   // Random sequence
   sequence();

   // Get closeness to solution
   int get_fitness() const override;
   // Mutate genes according to rate
   void mutate(double mutation_rate) override;

   // Override == operator for comparison of genes
   bool operator==(sequence const &other) const;
   friend std::ostream& operator<< (std::ostream& stream, const sequence& ind);

private:
   int get_gene(int index) const;
   void set_gene(int index, int const gene);

private:
   // Solution sequence
   static std::string solution;
   // Vecotr of genes
   std::vector<gene> genes;
   // Keep iternal fitness to avoid recalculations
   int fitness;
};

}

#endif //GENETIC_SEQUENCE_HPP
