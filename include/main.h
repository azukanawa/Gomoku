#ifndef MAIN_H
#define MAIN_H
#include <limits.h>
// 棋型评分常量
#define SCORE_WIN INT_MAX  // 五子连珠
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

typedef enum {
  NOTEND,        // 游戏未结束
  PLAYER_1_WIN,  // 玩家一获胜
  PLAYER_2_WIN,  // 玩家二获胜
  DRAW           // 平局
} GameResult;

// 棋盘结构体
typedef struct {
  Piece** board;   // 棋盘（是空的，玩家一的还是玩家二的）
  int emptyCeils;  // 当前剩余空格数
} ChessBoard;      // 棋盘结构体

extern ChessBoard g_chessBoard;
extern Piece g_currentPlayer;  // 当前执子玩家
extern int g_boardSize;

#endif
