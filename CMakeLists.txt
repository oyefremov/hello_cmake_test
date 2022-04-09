# 3.8 is required for CMAKE_CXX_STANDARD to be properly set to 17
cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)

# Turn on the ability to create folders to organize projects (.vcproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake
# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# To enamble gtest sources use following git commands in your project
# git submodule add git@github.com:google/googletest.git external/googletest
# git submodule update --init --recursive
add_subdirectory(external/googletest)

project(sample_project)

add_executable(sample_project_tests sample_tests.cpp)

# Enable MSVC static library support
set_property(TARGET sample_project_tests
			 PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_link_libraries(sample_project_tests gtest_main)
