#pragma once

#include <array>
#include <cassert>
#include <cstdint>
#include <random>
#include <sys/types.h>
#include <vector>

namespace tictactoe {

constexpr char X = 'X';
constexpr char O = 'O';
constexpr char _ = '.';

enum class game_outcome { UNDEFINED = -1, WIN_X = 0, DRAW = 1, WIN_O = 2 };

constexpr auto UNDEFINED = game_outcome::UNDEFINED;
constexpr auto WIN_X = game_outcome::WIN_X;
constexpr auto WIN_O = game_outcome::WIN_O;
constexpr auto DRAW = game_outcome::DRAW;

struct miniboard {
  std::array<char, 9> cells = {_, _, _, _, _, _, _, _, _};
  char winner = 0;
  int free_cells = 9;
  std::array<bool, 9> highlights = {false, false, false, false, false,
                                    false, false, false, false};
};

struct board_position {
  int big = 0, small = 0;
  friend bool operator==(board_position a, board_position b){
    return a.big == b.big && a.small == b.small;
  }
};

class target_miniboard{
  uint8_t raw_value;

public:
  target_miniboard() : raw_value(9) {}
  explicit target_miniboard(int mini) : raw_value(mini) {}
  explicit target_miniboard(game_outcome outcome)
      : raw_value(static_cast<uint8_t>(outcome) + 10) {}

  bool is_restricted_to_miniboard() const {
    return raw_value < 9;
  }
  int get_miniboard() const {
    assert(is_restricted_to_miniboard());
    return raw_value;
  }
  bool is_unrestricted() const {
    return raw_value == 9;
  }
  bool has_valid_move() const {
    return raw_value <= 9;
  }
  game_outcome get_outcome() const {
    assert(!has_valid_move());
    return static_cast<game_outcome>(raw_value - 10);
  }
  void set_miniboard(int miniboard) {
    assert(miniboard >= 0 && miniboard < 9);
    raw_value = miniboard;
  }
  void set_outcome(game_outcome outcome) {
    raw_value = static_cast<uint8_t>(outcome) + 10;
  }
  void set_unrestricted() {
    raw_value = 9;
  }
};

struct board {
  char player = X;
  target_miniboard target;
  std::array<miniboard, 9> miniboards;
};

struct move_stat {
  board_position move;
  std::array<int, 3> outcome;

  friend bool operator<(const move_stat& a, const move_stat& b){
    return a.outcome < b.outcome;
  }
};

struct game_tree{
  move_stat stat;
  board board;
  std::vector<game_tree> children;
};

using rnd_gen = std::mt19937;

struct game {
  board board;
  game_outcome outcome = UNDEFINED;
  std::vector<board_position> moves;
  std::vector<move_stat> stat;
  rnd_gen rnd = rnd_gen{42};
};

void print(const board &b);
void print(const game_tree &tree, int cursor);
void print(const game &g, const board_position &cursor,
           const board_position &last_move);
void shift_cursor(board_position &cursor, target_miniboard target, int offset);
bool is_empty(const board &b, const board_position &cursor);
void set(game &g, const board_position &cursor);
int find_empty_cell(const miniboard &b);

board_position pick_move(game &g);
board_position pick_random_move(game &g);
game_tree build_game_tree(const game &g, int iterations = 100, int depth = 0);

game_outcome random_play(const board &board, rnd_gen &rnd);

void replay(game &g, std::string_view moves);


std::vector<move_stat> calulate_stat(const board &b, rnd_gen& gen, size_t count = 1000);
} // namespace tictactoe