#include "tictactoe.h"
#include <curses.h>
#include <iomanip>
#include <iostream>
#include <string>

std::string read_key() {
  std::string result;
  for (;;) {
    auto ch = getch();
    if (ch == -1) {
      std::cout << "-1" << ".";
      std::cout.flush();
      break;
    }

    std::cout << ch << ".";
    std::cout.flush();
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
  //  clear();

  const auto HOME = "\033[H";
  const auto CLEAR = "\033[2J";
  const auto UP = "\033[A";
  const auto DOWN = "\033[B";
  const auto RIGHT = "\033[C";
  const auto LEFT = "\033[D";

  tictactoe::game game;
  tictactoe::board_position cursor = {4, 4};
  tictactoe::board_position last_move = {-1, -1};

  // print(board, {4, 4+3});
  // print(board, {4+3, 4+3});
  // print(board, {4-1, 4-1});

  std::cout << HOME << CLEAR;
  std::cout.flush();
  print(game, cursor, last_move);
  std::cout.flush();

  using tictactoe::O;
  using tictactoe::X;

  auto premoves = "40077221166888866001188778855551176333300445522335533458800884777561";
  replay(game, premoves);

  while (true) {
    (std::cout << "Press control key").flush();
    auto ch = read_key();
    if (ch == "")
      continue;

    if (ch == "q")
      break;
    if (ch == " ") {
      if (is_empty(game.board, cursor)) {
        set(game, cursor);
        last_move = tictactoe::pick_move(game);
        if (last_move.big != -1)
          set(game, last_move);
        if (game.board.target == -1)
          cursor = {4, 4};
        else
          cursor = {game.board.target,
                    find_empty_cell(game.board.miniboards[game.board.target])};
      }
    }

    if (ch == UP)
      shift_cursor(cursor, game.board.target, -3);
    if (ch == DOWN)
      shift_cursor(cursor, game.board.target, 3);
    if (ch == LEFT)
      shift_cursor(cursor, game.board.target, -1);
    if (ch == RIGHT)
      shift_cursor(cursor, game.board.target, 1);

    std::cout << HOME << CLEAR;
    print(game, cursor, last_move);

    std::cout << "Pressed char with code ";
    for (auto code : ch)
      std::cout << (int)code << " ";
    std::cout << ch.back();
    std::cout.flush();
    //    break;
  }
  endwin();
}
