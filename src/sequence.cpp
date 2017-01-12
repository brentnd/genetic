#include "sequence.hpp"
#include "random.hpp"

namespace genetic {

// Default solution
/*static*/ std::string sequence::solution = "123456789012345678901234567980";

/*static*/ sequence sequence::breed(sequence const & a, sequence const & b) {
   sequence child;
   for (unsigned i=0; i < sequence::solution.size(); i++) {
      if (random::probability(0.5)) {
         child.set_gene(i, a.get_gene(i));
      } else {
         child.set_gene(i, b.get_gene(i));
      }
   }
   return child;
}

/*static*/ int sequence::get_max_fitness() {
   return static_cast<int>(solution.length() * 150);
}

/*static*/ void sequence::set_solution(std::string const & solution) {
   sequence::solution = solution;
}

sequence::gene::gene() {
   chromosome = random::randint(32, 126);
}

void sequence::gene::mutate() {
   chromosome += random::randint(-10, 10);
}

sequence::gene::~gene() {
}

sequence::sequence() :
   genes(sequence::solution.size()),
   fitness(0) {
}

int sequence::get_fitness() const {
   if (!fitness) {
      fitness = static_cast<int>(genes.size() * 100);
      unsigned i = 0;
      for (auto const & gene : genes) {
         if (gene.chromosome == solution.at(i)) {
            // Higher fitness for exact match
            fitness += 50;
         }
         fitness -= std::abs(gene.chromosome - solution.at(i));
         ++i;
      }
   }
   return fitness;
}

void sequence::mutate(double mutation_rate) /*override*/ {
   for (int i=0; i < genes.size(); i++) {
      if (random::probability(mutation_rate)) {
         genes[i].mutate();
      }
   }
   fitness = 0;
}

bool sequence::operator==(sequence const & other) const /*override*/ {
   return genes == other.genes;
}

std::ostream& operator<< (std::ostream& stream, const sequence & individual) {
   stream << "individual f=" << individual.get_fitness() << " (";
   for (auto gene : individual.genes) {
      stream << static_cast<char>(gene.chromosome);
   }
   stream << ")";
   return stream;
}

int sequence::get_gene(int index) const {
   return genes.at(index).chromosome;
}
void sequence::set_gene(int index, int const chromosome) {
   genes[index].chromosome = chromosome;
}

}