cmake_minimum_required(VERSION 2.6)
project(Evaluator)

file(GLOB_RECURSE sources      src/*.cpp src/*.h)
add_executable(Evaluator ${sources})

SET(CMAKE_CXX_FLAGS "-std=c++14 -Wall -Wextra -Werror -pedantic")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
SET(CMAKE_CXX_FLAGS_RELEASE "-O2")
SET(CMAKE_CXX_FLAGS_DEBUG  "-O0 -g")

# target_compile_options(Evaluator PUBLIC -std=c++14 -Wall -Wextra -Werror -pedantic)

find_package(Boost 1.36.0 COMPONENTS regex REQUIRED)

target_link_libraries(Evaluator ${Boost_LIBRARIES})

