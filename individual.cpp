#include "individual.hpp"

namespace genetic {

const std::string individual::solution = "solution";

individual::gene::gene() {
   chromosome = std::rand() % 94 + 32;
}

void individual::gene::mutate() {
   chromosome += std::rand() % 20 - 10;
}

individual::gene::~gene() {
}

individual::individual() : genes(individual::gene_size) {
}

int individual::get_gene(int index) const {
   return genes.at(index).chromosome;
}

void individual::print_genes() const {
   printf("Gene: (%d)",(int)genes.size());
   for (auto gene : genes) {
      printf("|%c",gene.chromosome);
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
         fitness += 10;
      }
      fitness -= std::abs(gene.chromosome - solution.at(i++));
   }
   return fitness;
}

int individual::get_max_fitness() {
   return (int)solution.length()*110;
}

void individual::mutate() {
   for (int i=0; i < genes.size(); i++) {
      if ((std::rand() % 10) > 5) {
         genes[i].mutate();
      }
   }
}

individual individual::crossover(individual a, individual b) {
   individual child;
   float r = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
   for (unsigned int i=0; i < a.gene_size; i++) {
      if (r < 0.5) {
         child.set_gene(i, a.get_gene(i));
      } else {
         child.set_gene(i, b.get_gene(i));
      }
   }

   return child;
}

}