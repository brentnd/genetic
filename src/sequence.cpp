#include "sequence.hpp"
#include "random.hpp"

namespace genetic {

const std::string sequence::solution = "123456789012345678901234567980";

sequence::gene::gene() {
   chromosome = random::i_range(32, 126);
}

void sequence::gene::mutate() {
   chromosome += random::i_range(-10, 10);
}

sequence::gene::~gene() {
}

sequence::sequence() :
   genes(sequence::solution.size()),
   fitness(0) {
}

int sequence::get_gene(int index) const {
   return genes.at(index).chromosome;
}

void sequence::set_gene(int index, int const chro) {
   genes[index].chromosome = chro;
}

int sequence::get_fitness() const {
   if (fitness != 0) {
      return fitness;
   } else {
      auto fitness_ptr = const_cast<int *>(&fitness);
      *fitness_ptr = genes.size() * 100;
      int i = 0;
      for (auto gene : genes) {
         if (gene.chromosome == solution.at(i)) {
            *fitness_ptr += 50;
         }
         *fitness_ptr -= std::abs(gene.chromosome - solution.at(i++));
      }
      return fitness;
   }
}

int sequence::get_max_fitness() {
   return (int)solution.length()*150;
}

void sequence::mutate(double mutation_rate) {
   for (int i=0; i < genes.size(); i++) {
      if (random::probability(mutation_rate)) {
         genes[i].mutate();
      }
   }
   fitness = 0;
}

sequence sequence::crossover(sequence const &a, sequence const &b) {
   sequence child;
   for (unsigned int i=0; i < sequence::solution.size(); i++) {
      if (random::probability(0.5)) {
         child.set_gene(i, a.get_gene(i));
      } else {
         child.set_gene(i, b.get_gene(i));
      }
   }
   return child;
}

bool sequence::operator==(sequence const &other) const /*override*/ {
   return genes == other.genes;
}

}