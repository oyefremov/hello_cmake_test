#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "tictactoe.h"

using namespace testing;

using namespace tictactoe;

TEST(tictactoe, game1) {
  /*

OOX  X..  X..
.O.  .O.  ...
O.O  .OO  ..O

...  XXO  ...
...  .X.  ...
...  ...  ...

X..  X..  XXX
...  ...  ...
...  ...  ...

  */

  std::string moves = "411770011008800660000228811882144004444220";
  game game;
  for (int i = 1; i < moves.size(); i += 2) {
    auto big = moves[i - 1] - '0';
    auto small = moves[i] - '0';
    board_position pos = {big, small};
    set(game, pos);
    if (game.board.miniboards.at(pos.small).free_cells > 0) {
      EXPECT_EQ(game.board.target, pos.small)
          << "i=" << i << "\n"
          << moves << "\n"
          << std::string(i - 1, ' ') << "^^";
    } else {
      EXPECT_EQ(game.board.target, -1) << "i=" << i << "\n"
                                       << moves << "\n"
                                       << std::string(i - 1, ' ') << "^^";
    }
  }
  auto next_move = pick_random_move(game);
  EXPECT_EQ(next_move.big, 3);
  EXPECT_EQ(next_move.small, 7);
}

TEST(tictactoe, game2) {
  /*

.O.  ...  .O.
.X.  ...  O..
.OO  XXX  ...

O..  X..  .OX
XXX  ..O  O.X
...  ...  ..O

XO.  ..X  X..
O..  ..X  .XO
..O  .OX  OOX

  */

  std::string moves =
      "40077221166888866001188778855551176333300445522335533458800884777561";
  game game;
  for (int i = 1; i < moves.size(); i += 2) {
    auto big = moves[i - 1] - '0';
    auto small = moves[i] - '0';
    board_position pos = {big, small};
    set(game, pos);
    if (game.board.miniboards.at(pos.small).free_cells > 0) {
      EXPECT_EQ(game.board.target, pos.small)
          << "i=" << i << "\n"
          << moves << "\n"
          << std::string(i - 1, ' ') << "^^";
    } else {
      EXPECT_EQ(game.board.target, -1) << "i=" << i << "\n"
                                       << moves << "\n"
                                       << std::string(i - 1, ' ') << "^^";
    }
  }
  EXPECT_EQ(game.board.target, -1);
  for (int i:{1, 3, 7, 8})
  {
    EXPECT_EQ(game.board.miniboards[i].free_cells, 0) << i;
  }
  auto next_move = pick_random_move(game);
  EXPECT_EQ(next_move.big, 2);
  EXPECT_EQ(next_move.small, 7);
}
