project(tests)

add_executable(tests
    primes_test.cpp
    prime_twins_test.cpp
    is_prime_test.cpp
)

target_link_libraries(tests gmock_main primes)