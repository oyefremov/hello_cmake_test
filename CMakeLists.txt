# 3.8 is required for CMAKE_CXX_STANDARD to be properly set to 17
cmake_minimum_required(VERSION 3.15)

project(primes)

set(CMAKE_CXX_STANDARD 20)

# enable compile_commands.json for clangd extension   
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# To enamble gtest sources use following git commands in your project
# git submodule add git@github.com:google/googletest.git external/googletest
# git submodule update --init --recursive
add_subdirectory(external/googletest)

add_subdirectory(primes)
add_subdirectory(tictactoe)
add_subdirectory(app)
add_subdirectory(tests)
