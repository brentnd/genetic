cmake_minimum_required(VERSION 2.8)
project(geneticpp)

enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(include)
include_directories(third-party/Catch/include)
include_directories(third-party/variant/include)

set(SOURCE_FILES
   src/fitness.cpp
   src/attribute.cpp
   src/individual.cpp
   src/population.cpp
   src/random.cpp)

add_library(geneticpp SHARED ${SOURCE_FILES})

add_executable(geneticpp-hello-world
        example/hello_world.cpp
)
target_link_libraries(geneticpp-hello-world geneticpp)
add_executable(geneticpp-max-float
        example/max_float.cpp
        )
target_link_libraries(geneticpp-max-float geneticpp)
add_executable(geneticpp-multiple-objectives
        example/multiple_objectives.cpp
        )
target_link_libraries(geneticpp-multiple-objectives geneticpp)

find_package( OpenCV QUIET )
if (OpenCV_FOUND)
    set(SALESMAN_LIBS ${OpenCV_LIBS})
    add_definitions(-DWITH_OPENCV2)
else()
    # None
    set(SALESMAN_LIBS "")
endif()
add_executable(geneticpp-traveling-salesman
        example/traveling_salesman.cpp
        )
target_link_libraries(geneticpp-traveling-salesman geneticpp ${SALESMAN_LIBS})

add_executable(geneticpp-test
    test/main.cpp
        test/population.cpp
    test/random.cpp
    test/individual.cpp
)
target_link_libraries(geneticpp-test geneticpp)

add_test(NAME genetic_short COMMAND geneticpp-hello-world -e -m 0.1 -p 30 -g 10)
add_test(NAME genetic_test  COMMAND geneticpp-test)