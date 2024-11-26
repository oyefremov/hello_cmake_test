#include "tictactoe.h"
#include <algorithm>
#include <cassert>
#include <iomanip>
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
  auto has_new_target = cursor.big != -1 && is_empty(b, cursor);
  auto new_target = -1;
  if (has_new_target) {
    if (b.miniboards[cursor.small].free_cells) {
      new_target = cursor.small;
    }
  }
  size_t line_idx = 0;
  auto print_stat_ln = [&]() {
    if (line_idx < game.stat.size()) {
      auto &stat = game.stat[line_idx];
      line_idx++;
      if (stat.move == cursor) {
        std::cout << YELLOWB;
      }
      std::cout << "   " << stat.move.big << stat.move.small;
      for (auto n : stat.outcome)
        std::cout << std::setw(4) << n;
      std::cout << RESET;
    }
    std::cout << NEXT_LINE;
  };
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
          std::cout << "  ";
      }
      print_stat_ln();
      // std::cout << NEXT_LINE;
    }
    if (big_row < 2)
      std::cout << "             ";
    //      std::cout << NEXT_LINE;
    print_stat_ln();
  }
  auto color = 0;
  const char *colors[2] = {RED, BLUE};
  for (auto &move : game.moves) {
    std::cout << colors[color] << move.big << move.small;
    color ^= 1;
  }
  std::cout << RESET << NEXT_LINE;
}


void print(const game_tree &tree, int move_idx) {
  const board &b = tree.board;

  auto last_move = tree.stat.move;
  board_position cursor;

  auto has_new_target = move_idx < tree.children.size();
  auto new_target = -1;
  if (has_new_target) {
    cursor = tree.children[move_idx].stat.move;
    if (b.miniboards[cursor.small].free_cells) {
      new_target = cursor.small;
    }
  }

  size_t line_idx = 0;
  auto print_stat_ln = [&]() {
    if (line_idx < tree.children.size()) {
      if (move_idx == line_idx) {
        std::cout << YELLOWB;
      }
      auto &stat = tree.children[line_idx].stat;
      line_idx++;
      std::cout << "   " << stat.move.big << stat.move.small;
      for (auto n : stat.outcome)
        std::cout << std::setw(4) << n;
      std::cout << RESET; 
    }
    std::cout << NEXT_LINE;
  };
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
          std::cout << "  ";
      }
      print_stat_ln();
    }
    if (big_row < 2)
      std::cout << "             ";
    print_stat_ln();
  }
  // auto color = 0;
  // const char *colors[2] = {RED, BLUE};
  // for (auto &move : game.moves) {
  //   std::cout << colors[color] << move.big << move.small;
  //   color ^= 1;
  // }
  // std::cout << RESET << NEXT_LINE;
}

static bool can_not_shift(int idx, int offset) {
  return (idx + offset + 3) / 3 != (idx + 3) / 3;
}

void shift_cursor(board_position &cursor, target_miniboard target, int offset) {
  if (!target.has_valid_move()){
    return;
  }
  if (target.is_unrestricted()) {
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

static game_outcome validate_board(const board &b) {
  struct line {
    int start;
    int offset;
  };
  line lines[] = {{0, 1}, {3, 1}, {6, 1}, {0, 3},
                  {1, 3}, {2, 3}, {0, 4}, {2, 2}};
  for (auto [pos, offset] : lines) {
    int x = 0, o = 0;
    for (int i = 0; i < 3; ++i) {
      auto mini = b.miniboards.at(pos);
      pos += offset;
      x += mini.winner == X;
      o += mini.winner == O;
    }
    if (x == 3)
      return WIN_X;
    if (o == 3)
      return WIN_O;
  }
  int x = 0, o = 0;
  for (auto &mini : b.miniboards) {
    if (mini.free_cells > 0)
      return UNDEFINED;
    x += mini.winner == X;
    o += mini.winner == O;
  }
  if (x > o)
    return WIN_X;
  if (x < o)
    return WIN_O;
  return DRAW;
}

game_outcome apply_move(board &b, const board_position &move) {
  assert(is_empty(b, move));
  auto &mini = b.miniboards.at(move.big);
  mini.cells.at(move.small) = b.player;
  mini.free_cells -= 1;
  b.player ^= X ^ O;
  if (validate(mini)) {
    auto outcome = validate_board(b);
    if (outcome != UNDEFINED) {
      b.target.set_outcome(outcome);
      return outcome;
    }
  }
  if (b.miniboards.at(move.small).free_cells == 0)
    b.target.set_unrestricted();
  else
    b.target.set_miniboard(move.small);
  return UNDEFINED;
}

void set(game &game, const board_position &cursor) {
  board &b = game.board;
  game.moves.push_back(cursor);
  auto outcome = apply_move(game.board, cursor);
  if (outcome != UNDEFINED)
    game.outcome = outcome;
}

int find_empty_cell(const miniboard &b) {
  if (b.free_cells == 0)
    return 9;
  return std::find(b.cells.begin(), b.cells.end(), _) - b.cells.begin();
}

board_position pick_move(game &g) {
  auto stat = calulate_stat(g.board, g.rnd, 1000);
  if (stat.empty())
    return {-1, -1};
  return stat.front().move;
}

static int rnd(rnd_gen &gen, int max) {
  std::uniform_int_distribution dist(0, max - 1);
  return dist(gen);
}

board_position pick_random_move(const board &board, rnd_gen &gen) {
  auto target = board.target;
  assert(target.has_valid_move());
  if (target.is_unrestricted()) {
    int mini_count = 0;
    for (auto &mini : board.miniboards)
      mini_count += mini.free_cells > 0;
    assert(mini_count > 0);
    auto random_mini = rnd(gen, mini_count);
    auto random_mini_copy = random_mini;
    for (int mini_idx = 0; mini_idx < 9; ++mini_idx) {
      auto &mini = board.miniboards.at(mini_idx);
      if (mini.free_cells > 0 && random_mini-- == 0) {
        target.set_miniboard(mini_idx);
        break;
      }
    }
    if (target.is_unrestricted())
      (std::cerr << "Mini count = " << mini_count
                 << "Random_mini = " << random_mini << "\n")
          .flush();
  }
  assert(target.is_restricted_to_miniboard());
  auto &mini = board.miniboards.at(target.get_miniboard());
  assert(mini.free_cells > 0);
  auto random_idx = rnd(gen, mini.free_cells);
  for (int i = 0; i < 9; ++i)
    if (mini.cells[i] == _ && random_idx-- == 0)
      return {target.get_miniboard(), i};
  assert(false);
}

board_position pick_random_move(game &g) {
  if (g.outcome != UNDEFINED)
    return {-1, -1};
  return pick_random_move(g.board, g.rnd);
}

game_tree build_game_tree_impl(const game &g, int iterations, int depth)
{
  game_tree result;
  result.board = g.board;
  if (depth == 0)
    return result;
  auto rnd = g.rnd;
  auto moves = calulate_stat(g.board, rnd, iterations);
  for (auto &move : moves)
  {
    auto copy = g;
    set(copy, move.move);
    auto child = build_game_tree_impl(copy, iterations, depth - 1);
    child.stat = move;
    result.children.push_back(std::move(child));
  }
  return result;
}


game_tree build_game_tree(const game &g, int iterations, int depth){
  auto result = build_game_tree_impl(g, iterations, depth);
  if (!g.moves.empty())
    result.stat.move = g.moves.back();
  if (result.board.target.has_valid_move()) {
    auto rnd = g.rnd;
    for (int i = 0; i < iterations; ++i) {
      auto outcome = random_play(g.board, rnd);
      result.stat.outcome.at(static_cast<int>(outcome))++;
    }
  } else {
    auto outcome = result.board.target.get_outcome();
    result.stat.outcome.at(static_cast<int>(outcome)) += iterations;
  }
  return result;
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
      assert(game.board.target.get_miniboard() == pos.small);
    } else {
      assert(game.board.target.is_unrestricted());
    }
  }
}

game_outcome random_play(const board &board, rnd_gen &gen) {
  auto board_copy = board;
  auto outcome = UNDEFINED;
  do {
    auto random_move = pick_random_move(board_copy, gen);
    outcome = apply_move(board_copy, random_move);
  } while (outcome == UNDEFINED);
  return outcome;
}

std::vector<move_stat> calulate_stat(const board &b, rnd_gen &gen,
                                     size_t count) {
  std::vector<move_stat> result;
  auto gen_moves = [&](int idx) {
    auto &mini = b.miniboards.at(idx);
    if (mini.free_cells == 0)
      return;
    for (int i = 0; i < 9; ++i)
      if (mini.cells.at(i) == _)
        result.push_back({{idx, i}, {0, 0, 0}});
  };
  if (b.target.is_unrestricted())
    for (int i = 0; i < 9; ++i)
      gen_moves(i);
  else if (b.target.is_restricted_to_miniboard())
    gen_moves(b.target.get_miniboard());

  for (auto &record : result) {
    auto copy = b;
    auto outcome = apply_move(copy, record.move);
    if (outcome != UNDEFINED) {
      record.outcome.at(static_cast<int>(outcome)) += count;
    } else {
      for (int i = 0; i < count; ++i) {
        auto outcome = random_play(copy, gen);
        record.outcome.at(static_cast<int>(outcome))++;
      }
    }
  }
  std::sort(result.begin(), result.end());
  if (b.player == X)
    std::reverse(result.begin(), result.end());

  return result;
}

} // namespace tictactoe