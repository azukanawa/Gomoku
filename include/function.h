#ifndef FUNCTION_H
#define FUNVTION_H
#include "main.h"
/**
 * @brief 求两个int中较大的数是多少
 *
 * @param a 第一个int数
 * @param b 第二个int数
 * @return int 返回其中更大的那一个
 */
int MaxInt(int a, int b);

/**
 * @brief 求两个int中较小的数是多少
 *
 * @param a 第一个int数
 * @param b 第二个int数
 * @return int 返回其中更小的那一个
 */
int MinInt(int a, int b);

/**
 * @brief 初始化棋盘，将每一个格子都赋值为空
 *
 * @param board 棋盘指针
 */
void InitBoard(ChessBoard* board);

/**
 * @brief 判断是否有人胜利
 *
 * @param board 棋盘指针
 * @param current_row 当前落子行坐标
 * @param current_col 当前落子纵坐标
 * @return Piece 胜者是谁，EMPTY表示还没有人获胜
 */
Piece CheckWin(const ChessBoard* board, int current_row, int current_col);

#endif