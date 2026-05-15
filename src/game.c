#include "game.h"

#include <stdio.h>

#include "Button.h"
#include "chessBoard.h"
#include "function.h"
void LocalTwoPlayerMode(void) {
  int row;
  int col;
  HandleChessPlacement(&row, &col);
  if (CheckWin(&g_chessBoard, row, col) == EMPTY &&
      IsBoardFull(&g_chessBoard) == FALSE) {
    return;
  }
  if (CheckWin(&g_chessBoard, row, col) == PLAYER_1) {
    printf("black wins\n");
  } else if (CheckWin(&g_chessBoard, row, col) == PLAYER_2) {
    printf("white wins\n");
  } else {
    printf("draw\n");
  }
  ButtonPage = 0;
}