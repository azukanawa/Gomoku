#ifndef MAIN_H
#define MAIN_H

#define BOARD_SIZE 17  // 棋盘尺寸

// 棋型评分常量
#define SCORE_WIN 10000    // 五子连珠
#define SCORE_LIVE4 5000   // 活四
#define SCORE_BLOCK4 2500  // 冲四（死四、跳冲四）
#define SCORE_LIVE3 1000   // 活三（包括跳活三）
#define SCORE_BLOCK3 200   // 眠三（只有一端开放的三）
#define SCORE_LIVE2 100    // 活二（包括跳活二）
#define SCORE_BLOCK2 30    // 眠二（只有一端开放的二）

typedef enum { FALSE, TRUE } Bool;
typedef enum {
  EMPTY,     // 空
  PLAYER_1,  // 玩家一
  PLAYER_2,  // 玩家二
} Piece;     // 格子状态

// 棋盘结构体
typedef struct {
  Piece board
      [BOARD_SIZE]
      [BOARD_SIZE];  // 棋盘，用于存储当前格子的状态（是空的，玩家一的还是玩家二的）
  int emptyCeils;  // 当前剩余空格数
} ChessBoard;      // 棋盘结构体

extern ChessBoard g_chessBoard;
extern Piece g_currentPlayer;  // 当前执子玩家

#endif
