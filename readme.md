# geneticpp

geneticpp is an experimental attempt at implementing evolutionary algorithms leveraging C++11 features and the Standard Template Library.

Inspired by the ease-of-use and flexibility of https://github.com/DEAP/deap (a Python project).

## Installation

CMake is used for all builds. `build.sh` bash script is used to run all CMake targets and execute simple tests.

## Tests

[Catch](https://github.com/philsquared/Catch) is used for testing. All test files are found in `test/`.
 Current tests for `random`, `sequence`, and `population`.
 
### TODO
* Dynamic individual attribute count
* Logging
* Switch to GTest and re-write tests to improve coverage
* Mutation functions (ES log normal)
* Crossover functions (partially matched, simulated binary)