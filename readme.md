# geneticpp

geneticpp is a non-production ready attempt at implementing evolutionary algorithms to leverage use
 of C++11 features and the Standard Template Library.

Inspired by the ease-of-use and flexibility of https://github.com/DEAP/deap (a Python project).

## Installation

CMake is used for all builds. `build.sh` bash script is used to run all CMake targets and execute simple tests.

## Tests

[Catch](https://github.com/philsquared/Catch) is used for testing. All test files are found in `test/`.
 Current tests for `random`, `sequence`, and `population`. This is an area for improvement or re-writing maybe with
 GTest.