cmake_minimum_required(VERSION 3.2)
project(genetic)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(EXE genetic)

set(SOURCE_FILES
   src/population.cpp
   src/individual.cpp
   src/random.cpp)
set(TEST_FILES
   test/tests.cpp
   test/catch.hpp)
add_executable(${EXE} src/main.cpp ${SOURCE_FILES})
add_executable(test_${EXE} test/main.cpp ${TEST_FILES} ${SOURCE_FILES})