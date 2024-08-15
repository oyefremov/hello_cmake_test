#include "tictactoe.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>

namespace {

const auto YELLOWB = "\033[43m";
const auto GREENB = "\033[42m";
const auto GRAY = "\033[90m";
const auto RED = "\033[91m";
const auto GREEN = "\033[92m";
const auto BLUE = "\033[96m";
const auto RESET = "\033[0m";
const auto NEXT_LINE = "\033[E";
} // namespace

namespace tictactoe {

void print(const board &b) {
  for (auto big_row = 0; big_row < 3; ++big_row) {
    for (auto small_row = 0; small_row < 3; ++small_row) {
      for (auto big_col = 0; big_col < 3; ++big_col) {
        auto &mini = b.miniboards[big_row * 3 + big_col];
        for (auto small_col = 0; small_col < 3; ++small_col) {
          auto cell = mini.cells[small_col * 3 + small_row];
          std::cout << cell;
        }
        if (big_col < 2)
          std::cout << "|";
      }
      std::cout << "\n\r";
    }
    if (big_row < 2)
      std::cout << "--- --- ---\n\r";
  }
  std::cout << "test board\n\r";
}

void print(const game &game, const board_position &cursor,
           const board_position &last_move) {
  const board &b = game.board;
  auto has_new_target = is_empty(b, cursor);
  auto new_target = -1;
  if (has_new_target) {
    if (b.miniboards[cursor.small].free_cells) {
      new_target = cursor.small;
    }
  }
  for (auto big_row = 0; big_row < 3; ++big_row) {
    for (auto small_row = 0; small_row < 3; ++small_row) {
      for (auto big_col = 0; big_col < 3; ++big_col) {
        auto big_idx = big_row * 3 + big_col;
        auto &mini = b.miniboards[big_idx];
        for (auto small_col = 0; small_col < 3; ++small_col) {
          auto small_idx = small_row * 3 + small_col;
          auto cell = mini.cells[small_idx];

          if (last_move.big == big_idx && last_move.small == small_idx)
            std::cout << GREENB;

          if (cursor.big == big_idx && cursor.small == small_idx)
            std::cout << YELLOWB;

          if (mini.free_cells == 0) {
            if (mini.highlights[small_idx]) {
              std::cout << (cell == X ? RED : BLUE);
            } else {
              std::cout << GRAY;
            }
          } else if (has_new_target && cell == _ &&
                     (big_idx == new_target || new_target == -1)) {
            std::cout << GREEN;
          }

          std::cout << cell;

          std::cout << RESET;
        }
        if (big_col < 2)
          //          std::cout << "|";
          std::cout << "  ";
      }
      std::cout << NEXT_LINE;
    }
    if (big_row < 2)
      //      std::cout << "--- --- ---" << NEXT_LINE;
      std::cout << NEXT_LINE;
  }
  auto color = 0;
  const char *colors[2] = {RED, BLUE};
  for (auto &move : game.moves) {
    std::cout << colors[color] << move.big << move.small;
    color ^= 1;
  }
  std::cout << RESET << NEXT_LINE;
}

static bool can_not_shift(int idx, int offset) {
  return (idx + offset + 3) / 3 != (idx + 3) / 3;
}

void shift_cursor(board_position &cursor, int target, int offset) {
  if (target == -1) {
    if (offset == 1 || offset == -1) {
      if (can_not_shift(cursor.small, offset)) {
        cursor.small -= 2 * offset;
        if (can_not_shift(cursor.big, offset)) {
          cursor.big -= 2 * offset;
        } else {
          cursor.big += offset;
        }
      } else {
        cursor.small += offset;
      }
    } else {
      cursor.small += offset;
      if (cursor.small < 0 || cursor.small >= 9) {
        cursor.small = (cursor.small + 9) % 9;
        cursor.big = (cursor.big + offset + 9) % 9;
      }
    }
  } else {
    if (offset == 1 || offset == -1) {
      if (can_not_shift(cursor.small, offset)) {
        cursor.small -= 2 * offset;
      } else {
        cursor.small += offset;
      }
    } else {
      cursor.small = (cursor.small + 9 + offset) % 9;
    }
  }
}

bool is_empty(const board &b, const board_position &cursor) {
  auto &mini = b.miniboards.at(cursor.big);
  return mini.free_cells && mini.cells.at(cursor.small) == _;
}

static bool validate(miniboard &b) {
  struct line {
    int start;
    int offset;
  };
  line lines[] = {{0, 1}, {3, 1}, {6, 1}, {0, 3},
                  {1, 3}, {2, 3}, {0, 4}, {2, 2}};
  for (auto [pos, offset] : lines) {
    int x = 0, o = 0;
    for (int i = 0; i < 3; ++i) {
      auto cell = b.cells.at(pos);
      pos += offset;
      x += cell == X;
      o += cell == O;
    }
    if (x == 3 || o == 3) {
      b.free_cells = 0;
      b.winner = x == 3 ? X : O;
      pos -= offset * 3;
      for (int i = 0; i < 3; ++i) {
        b.highlights.at(pos) = true;
        pos += offset;
      }
    }
  }
  return b.free_cells == 0;
}

void set(game &game, const board_position &cursor) {
  board &b = game.board;
  game.moves.push_back(cursor);
  assert(is_empty(b, cursor));
  auto &mini = b.miniboards.at(cursor.big);
  mini.cells.at(cursor.small) = b.player;
  mini.free_cells -= 1;
  validate(mini);
  b.player ^= X ^ O;
  if (b.miniboards.at(cursor.small).free_cells == 0)
    b.target = -1;
  else
    b.target = cursor.small;
}

int find_empty_cell(const miniboard &b) {
  if (b.free_cells == 0)
    return 9;
  return std::find(b.cells.begin(), b.cells.end(), _) - b.cells.begin();
}

board_position pick_move(game &g) { return pick_random_move(g); }

static int rnd(game &g, int max) {
  std::uniform_int_distribution dist(0, max - 1);
  return dist(g.rnd_gen);
}

board_position pick_random_move(game &g) {
  auto target = g.board.target;
  if (target == -1) {
    int mini_count = 0;
    for (auto &mini : g.board.miniboards)
      mini_count += mini.free_cells > 0;
    if (mini_count == 0) {
      return {-1, -1};
    }
    auto random_mini = rnd(g, mini_count);
    auto random_mini_copy = random_mini;
    for (int mini_idx = 0; mini_idx < 9; ++mini_idx) {
      auto &mini = g.board.miniboards.at(mini_idx);
      if (mini.free_cells > 0 && random_mini-- == 0) {
        target = mini_idx;
        break;
      }
    }
    if (target == -1)
      (std::cerr << "Mini count = " << mini_count
                 << "Random_mini = " << random_mini << "\n")
          .flush();
    assert(target != -1);
  }
  auto &mini = g.board.miniboards.at(target);
  assert(mini.free_cells > 0);
  auto random_idx = rnd(g, mini.free_cells);
  for (int i = 0; i < 9; ++i)
    if (mini.cells[i] == _ && random_idx-- == 0)
      return {target, i};
  assert(false);
}

void replay(game &game, std::string_view moves) {
  assert(moves.size() % 2 == 0);
  for (int i = 1; i < moves.size(); i += 2) {
    auto big = moves[i - 1] - '0';
    auto small = moves[i] - '0';
    assert(big >= 0 && big < 9);
    assert(small >= 0 && small < 9);
    board_position pos = {big, small};
    set(game, pos);
    if (game.board.miniboards.at(pos.small).free_cells > 0) {
      assert(game.board.target == pos.small);
    } else {
      assert(game.board.target == -1);
    }
  }
}

} // namespace tictactoe