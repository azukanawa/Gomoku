#include "game.h"

#include <stdio.h>

#include "Button.h"
#include "Rendering.h"
#include "chessBoard.h"
#include "function.h"
GameResult LocalTwoPlayerMode(void) {
  int row;
  int col;
  HandleChessPlacement(&row, &col);
  if (CheckWin(&g_chessBoard, row, col) == EMPTY &&
      IsBoardFull(&g_chessBoard) == FALSE) {
    return NOTEND;  // 游戏未结束
  }
  if (CheckWin(&g_chessBoard, row, col) == PLAYER_1) {
    return PLAYER_1_WIN;  // 游戏结束，黑棋获胜
  } else if (CheckWin(&g_chessBoard, row, col) == PLAYER_2) {
    return PLAYER_2;  // 游戏结束，白棋获胜
  } else {
    return DRAW;  // 游戏结束，平局
  }
}