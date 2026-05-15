#include "game.h"

#include <stdio.h>

#include "Button.h"
#include "chessBoard.h"
#include "function.h"
void LocalTwoPlayerMode(void) {
  int row;
  int col;
  if (HandleChessPlacement(&row, &col) == FALSE) {
    return;
  }
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

void AiMode(void) {
  int row;
  int col;
  if (g_currentPlayer == PLAYER_1) {
    if (HandleChessPlacement(&row, &col) == FALSE) {
      return;
    }
  } else {
    GetBestMove(&g_chessBoard, g_currentPlayer, &row, &col);
    g_chessBoard.board[row][col] = g_currentPlayer;
    g_chessBoard.emptyCeils;
    g_currentPlayer = (g_currentPlayer == PLAYER_1) ? PLAYER_2 : PLAYER_1;
  }
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
  g_currentPlayer = PLAYER_1;
}