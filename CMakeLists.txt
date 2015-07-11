cmake_minimum_required(VERSION 3.2)
project(genetic)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(genetic ${SOURCE_FILES} population.cpp population.hpp individual.cpp individual.hpp)