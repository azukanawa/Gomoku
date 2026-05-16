#include "game.h"

#include <stdio.h>

#include "Button.h"
#include "Rendering.h"
#include "chessBoard.h"
#include "function.h"
GameResult LocalTwoPlayerMode(void) {
  int row;
  int col;
  Piece who_wins;
  if (HandleChessPlacement(&row, &col) == FALSE) {
    return NOTEND;
  }
  InPositionStack(row, col, &g_positionStack);
  who_wins = CheckWin(&g_chessBoard, row, col);
  if (who_wins == EMPTY && IsBoardFull(&g_chessBoard) == FALSE) {
    return NOTEND;
  }
  if (who_wins == PLAYER_1) {
    return PLAYER_1_WIN;
  } else if (who_wins == PLAYER_2) {
    return PLAYER_2_WIN;
  } else {
    return DRAW;
  }
}

GameResult AiMode(void) {
  int row;
  int col;
  Piece who_wins;
  if (g_currentPlayer == PLAYER_1) {
    if (HandleChessPlacement(&row, &col) == FALSE) {
      return NOTEND;
    }
    InPositionStack(row, col, &g_positionStack);
  } else {
    GetBestMove(&g_chessBoard, g_currentPlayer, &row, &col);
    g_chessBoard.board[row][col] = g_currentPlayer;
    InPositionStack(row, col, &g_positionStack);
    g_chessBoard.emptyCeils;
    g_currentPlayer = (g_currentPlayer == PLAYER_1) ? PLAYER_2 : PLAYER_1;
  }
  who_wins = CheckWin(&g_chessBoard, row, col);
  if (who_wins == EMPTY && IsBoardFull(&g_chessBoard) == FALSE) {
    return NOTEND;
  }
  if (who_wins == PLAYER_1) {
    return PLAYER_1_WIN;
  } else if (who_wins == PLAYER_2) {
    return PLAYER_2_WIN;
  } else {
    return DRAW;
  }
}