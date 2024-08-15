add_executable(tests
    tictactoe_test.cpp
)

target_link_libraries(tests gmock_main tictactoe)