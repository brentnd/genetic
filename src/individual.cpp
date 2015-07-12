#include "individual.hpp"
#include "random.hpp"

namespace genetic {

const std::string individual::solution = "123456789012345678901234567980";

individual::gene::gene() {
   chromosome = random::i_range(32, 126);
}

void individual::gene::mutate() {
   chromosome += random::i_range(-10, 10);
}

individual::gene::~gene() {
}

individual::individual() :
   genes(individual::solution.size()),
   fitness(0) {
}

int individual::get_gene(int index) const {
   return genes.at(index).chromosome;
}

void individual::print_genes() const {
   printf("Gene: ");
   for (auto gene : genes) {
      printf("%c",gene.chromosome);
   }
   printf("\n");
}

void individual::set_gene(int index, int const chro) {
   genes[index].chromosome = chro;
}

int individual::get_fitness() const {
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

int individual::get_max_fitness() {
   return (int)solution.length()*150;
}

void individual::mutate(double mutation_rate) {
   for (int i=0; i < genes.size(); i++) {
      if (random::probability(mutation_rate)) {
         genes[i].mutate();
      }
   }
   fitness = 0;
}

individual individual::crossover(individual a, individual b) {
   if (a == b) {
      return a;
   }
   individual child;
   for (unsigned int i=0; i < individual::solution.size(); i++) {
      if (random::probability(0.5)) {
         child.set_gene(i, a.get_gene(i));
      } else {
         child.set_gene(i, b.get_gene(i));
      }
   }
   return child;
}

}