cmake_minimum_required(VERSION 2.8)
project(genetic)

enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(EXE genetic)
set(TEXT_EXE test_${EXE})

set(SOURCE_FILES
   src/organism.cpp
   src/population.cpp
   src/random.cpp)

set(TEST_FILES
   test/tests.cpp
   /usr/local/include/catch.hpp)

add_executable(${EXE} src/main.cpp ${SOURCE_FILES})
add_executable(${TEXT_EXE} test/main.cpp ${TEST_FILES} ${SOURCE_FILES})

add_test(NAME genetic_short COMMAND ${EXE} -e -m 0.1 -p 30 -g 10)
add_test(NAME genetic_test  COMMAND ${TEXT_EXE})