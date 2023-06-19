
project(primes)

add_library(primes primes.cpp)
target_include_directories(primes PUBLIC .)


#target_link_libraries(sample_project_tests gtest_main)
