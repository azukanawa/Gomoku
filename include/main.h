#ifndef MAIN_H
#define MAIN_H
#define BOARD_SIZE 15  // 棋盘尺寸
// 棋型评分常量
#define SCORE_WIN 10000
#define SCORE_LIVE4 5000
#define SCORE_BLOCK4 2500
#define SCORE_LIVE3 1000
#define SCORE_LIVE2 100
typedef enum { FALSE, TRUE } Bool;
typedef enum {
  EMPTY,     // 空
  PLAYER_1,  // 玩家一
  PLAYER_2,  // 玩家二
} Piece;     // 格子状态

typedef struct {
  Piece board
      [BOARD_SIZE]
      [BOARD_SIZE];  // 棋盘，用于存储当前格子的状态（是空的，玩家一的还是玩家二的）
  int emptyCeils;  // 当前剩余空格数
} ChessBoard;      // 棋盘结构体
#endif