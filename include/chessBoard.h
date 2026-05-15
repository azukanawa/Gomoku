#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include "Rendering.h"

extern GameResources gameResources;
// 棋盘相关函数声明
int DrawChessBoard(
    const GameResources* res, int boardSize,
    const struct window_size* winSize);  // 绘制棋盘，成功返回0，失败返回-1

Bool GetMouseChessPosition(int boardSize, const struct window_size* winSize,
                           int* row, int* col);

Bool HandleChessPlacement(int boardSize, const struct window_size* winSize);

void DrawChessPreview(int boardSize, const struct window_size* winSize);

void DrawAllChessPieces(const GameResources* res, int boardSize,
                        const struct window_size* winSize);

void InitChessBoard(void);

#endif  // CHESSBOARD_H