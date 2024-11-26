#include "tictactoe.h"
#include <curses.h>
#include <iostream>
#include <string>
#include <vector>

std::string read_key() {
  std::string result;
  for (;;) {
    auto ch = getch();
    if (ch == -1) {
      break;
    }

    result.append(1, (char)ch);
    if (result == "\033" || result == "\033[")
      continue;
    break;
  }
  return result;
}

int main() {

  initscr();
  cbreak();
  noecho();

  const auto HOME = "\033[H";
  const auto CLEAR = "\033[2J";
  const auto UP = "\033[A";
  const auto DOWN = "\033[B";
  const auto RIGHT = "\033[C";
  const auto LEFT = "\033[D";
  const auto TAB = "\t";

  tictactoe::game game;
  tictactoe::board_position cursor = {4, 4};
  tictactoe::board_position last_move = {-1, -1};
  tictactoe::game_tree root_tree;

  std::cout << HOME << CLEAR;
  std::cout.flush();
  print(game, cursor, last_move);
  std::cout.flush();

  using tictactoe::O;
  using tictactoe::X;

  // auto premoves =
  // "40077221166888866001188778855551176333300445522335533458800884777561";
  auto premoves = "";
  replay(game, premoves);

  root_tree = tictactoe::build_game_tree(game, 1000, 1);
  int tree_cursor = 0;
  int mode = 0; 

  std::vector<const tictactoe::game_tree*> tree_stack;
  tree_stack.push_back(&root_tree);

  while (true) {
    auto ch = read_key();
    if (ch == "")
      continue;

    if (ch == "q")
      break;
    if (ch == " ") {
      if (game.outcome == tictactoe::UNDEFINED &&
          is_empty(game.board, cursor)) {
        set(game, cursor);
        last_move = tictactoe::pick_move(game);
        if (last_move.big != -1) {
          set(game, last_move);
          root_tree = tictactoe::build_game_tree(game, 1000, 1);
          tree_cursor = 0;
        }
        game.stat = tictactoe::calulate_stat(game.board, game.rnd);
        if (game.stat.empty())
          cursor = {-1, -1};
        else
          cursor = game.stat.front().move;
      }
    }
    if (ch == TAB) {
      mode = (mode + 1) % 2;
    }

    if (mode == 0) {
      if (cursor.big != -1) {
        if (ch == UP) {
          shift_cursor(cursor, game.board.target, -3);
        } else if (ch == DOWN) {
          shift_cursor(cursor, game.board.target, 3);
        } else if (ch == LEFT) {
          shift_cursor(cursor, game.board.target, -1);
        } else if (ch == RIGHT) {
          shift_cursor(cursor, game.board.target, 1);
        }
      }
    } else if (mode == 1) {
      if (ch == UP) {
        tree_cursor =
            (tree_cursor + root_tree.children.size() - 1) % root_tree.children.size();
      } else if (ch == DOWN) {
        tree_cursor = (tree_cursor + 1) % root_tree.children.size();
      } else if (ch == LEFT) {
        if (tree_stack.size() > 1) tree_stack.pop_back();
      } else if (ch == RIGHT) {
        auto tree = tree_stack.back();
        if (tree_cursor < tree->children.size()) {
          tree_stack.push_back(&tree->children[tree_cursor]);
          tree_cursor = 0;
        }
      }
    }
    std::cout << HOME << CLEAR;
    if (mode == 0)
      print(game, cursor, last_move);
    else
      print(root_tree, tree_cursor);

    std::cout << "Pressed char with code ";
    for (auto code : ch)
      std::cout << (int)code << " ";
    std::cout << ch.back();
    std::cout.flush();
  }
  endwin();
}
