cmake_minimum_required(VERSION 2.6)
project(Evaluator)

SET(CMAKE_CXX_FLAGS "-std=c++14 -Wall -Wextra -Werror -pedantic")
include_directories(lib)

file(GLOB_RECURSE sources src/*.cc src/*.h)
file(GLOB_RECURSE hand-isomorphism-sources lib/hand-isomorphism/src/*.c)

add_library(hand-isomorphism SHARED ${hand-isomorphism-sources})
add_executable(Evaluator ${sources})

find_package(gflags REQUIRED)
find_package(Boost 1.36.0 COMPONENTS regex REQUIRED)
target_link_libraries(Evaluator ${Boost_LIBRARIES} gflags hand-isomorphism)
