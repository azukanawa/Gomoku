#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "Rendering.h"
#include "main.h"

extern GameResources gameResources;
// 棋盘相关函数声明
int DrawChessBoard(
    const GameResources* res, int boardSize,
    const struct window_size* winSize);  // 绘制棋盘，成功返回0，失败返回-1

Bool GetMouseChessPosition(int boardSize, const struct window_size* winSize,
                           int* row, int* col);

Bool HandleChessPlacement(int* row_out, int* col_out);
// 处理鼠标落子逻辑，成功落子返回TRUE，并通过输出参数返回行列

void DrawChessPreview(int boardSize, const struct window_size* winSize);

void DrawAllChessPieces(const GameResources* res, int boardSize,
                        const struct window_size* winSize);

#endif  // CHESSBOARD_H