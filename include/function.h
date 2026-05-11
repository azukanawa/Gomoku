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
#endif