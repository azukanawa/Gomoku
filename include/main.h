#ifndef MAIN_H
#define MAIN_H
#define BOARD_SIZE 15

typedef enum {
  EMPTY,     // 空
  PLAYER_1,  // 玩家一
  PLAYER_2,  // 玩家二
} Piece;

typedef struct {
  Piece board
      [BOARD_SIZE]
      [BOARD_SIZE];  // 棋盘，用于存储当前格子的状态（是空的，玩家一的还是玩家二的）
  int emptyCeils;  // 当前剩余空格数
} ChessBoard;
#endif