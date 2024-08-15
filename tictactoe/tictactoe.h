#pragma once

#include <array>
#include <random>
#include <vector>
#include <utility>

namespace tictactoe{

constexpr char X = 'X';
constexpr char O = 'O';
constexpr char _ = '.';

struct miniboard
{
    std::array<char, 9> cells = {_, _, _, _, _, _, _, _, _};
    char winner = 0;
    int free_cells = 9;
    std::array<bool, 9> highlights = {false, false, false, false, false, false, false, false, false};
};

struct board_position{
    int big = 0, small = 0;
};

struct board
{
    char player = X;
    int target = -1;
    std::array<miniboard, 9> miniboards;
};

struct game
{
    board board;    
    std::vector<board_position> moves;
    std::mt19937 rnd_gen = std::mt19937{42};
};

void print(const board& b);
void print(const game& g, const board_position& cursor, const board_position& last_move);
void shift_cursor(board_position&cursor, int target, int offset);
bool is_empty(const board& b, const board_position& cursor);
void set(game& g, const board_position& cursor);
int find_empty_cell(const miniboard& b);

board_position pick_move(game& g);
board_position pick_random_move(game& g);

void replay(game& g, std::string_view moves);
}