cmake_minimum_required(VERSION 2.8)
project(geneticpp)

enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(include)

set(SOURCE_FILES
   src/individual.cpp
   src/population.cpp
   src/random.cpp)

add_library(geneticpp SHARED ${SOURCE_FILES})

add_executable(geneticpp-example
        example/hello_world.cpp
)
target_link_libraries(geneticpp-example geneticpp)
add_executable(geneticpp-test
    test/main.cpp
    test/tests.cpp
)
target_link_libraries(geneticpp-test geneticpp)

add_test(NAME genetic_short COMMAND geneticpp-example -e -m 0.1 -p 30 -g 10)
add_test(NAME genetic_test  COMMAND geneticpp-test)