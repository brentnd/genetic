#include <stdexcept>
#include "individual.hpp"

namespace genetic {

const std::string individual::solution = "123456789012345678901234567980";

individual::gene::gene() {
   chromosome = std::rand() % 94 + 32;
}

void individual::gene::mutate() {
   chromosome += std::rand() % 20 - 10;
}

individual::gene::~gene() {
}

individual::individual() : genes(individual::solution.size()) {
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
   int fitness = genes.size()*100;
   int i = 0;
   for (auto gene : genes) {
      if (gene.chromosome == solution.at(i)) {
         fitness += 50;
      }
      fitness -= std::abs(gene.chromosome - solution.at(i++));
   }
   return fitness;
}

int individual::get_max_fitness() {
   return (int)solution.length()*150;
}

void individual::mutate(double mutation_rate) {
   for (int i=0; i < genes.size(); i++) {
      float r = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
      if (r < mutation_rate) {
         genes[i].mutate();
      }
   }
}

individual individual::crossover(individual a, individual b) {
   if (a == b) {
      return a;
   }
   individual child;
   for (unsigned int i=0; i < individual::solution.size(); i++) {
      float r = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
      if (r < 0.5) {
         child.set_gene(i, a.get_gene(i));
      } else {
         child.set_gene(i, b.get_gene(i));
      }
   }

   return child;
}

}