# 3.8 is required for CMAKE_CXX_STANDARD to be properly set to 17
cmake_minimum_required(VERSION 3.15)

project(PrimesProject)

set(CMAKE_CXX_STANDARD 17)

# To enamble gtest sources use following git commands in your project
# git submodule add git@github.com:google/googletest.git external/googletest
# git submodule update --init --recursive
add_subdirectory(external/googletest)

add_subdirectory(primes)

add_subdirectory(app)

add_subdirectory(tests)
