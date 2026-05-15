#ifndef FUNCTION_H
#define FUNCTION_H
#include "main.h"
typedef struct {
  int row;
  int col;
  int score;
} Move;
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
 * @brief 释放棋盘数组
 *
 * @param board 棋盘指针
 */
void FreeBoard(ChessBoard* board);

/**
 * @brief 判断棋盘是否被下满了
 *
 * @param board 棋盘指针
 * @return TRUE 被下满了 FALSE 没被下满
 */
Bool IsBoardFull(const ChessBoard* board);

/**
 * @brief 判断是否有人胜利
 *
 * @param board 棋盘指针
 * @param current_row 当前落子行坐标
 * @param current_col 当前落子纵坐标
 * @return Piece 胜者是谁，EMPTY表示还没有人获胜
 */
Piece CheckWin(const ChessBoard* board, int current_row, int current_col);

/**
 * @brief 获得一个棋形的分数
 *
 * @param piece_count 棋形里包含几个子
 * @param has_skip 是否有跳子，1表示有，0表示无
 * @param left_open 棋形的左边是否开放没被堵住，1表示开放，0表示被堵
 * @param right_open 棋形的右边是否开放没被堵住，1表示开放，0表示被堵
 * @return score 棋形的分数
 */
int GetPatternScore(int piece_count, int has_skip, int left_open,
                    int right_open);

/**
 * @brief 扫描从一个起点开始的一段棋形的分数
 *
 * @param board 棋盘指针
 * @param start_row 起点行
 * @param start_col 起点列
 * @param dr 行方向参量
 * @param dc 列方向参量
 * @param piece 玩家是谁
 * @return int
 */
int ScanSegment(const ChessBoard* board, int start_row, int start_col, int dr,
                int dc, Piece piece);

/**
 * @brief 判断该位置是否是一个棋形的开始点
 *
 * @param board 棋盘指针
 * @param row 位置行坐标
 * @param col 位置纵坐标
 * @param dr 行方向参量
 * @param dc 列方向参量
 * @param piece 玩家是谁
 * @return TRUE表示是，FALSE表示否
 */
Bool IsStartOfSegment(const ChessBoard* board, int row, int col, int dr, int dc,
                      Piece piece);

/**
 * @brief 计算整块棋盘当前玩家的净胜分
 *
 * @param board 棋盘指针
 * @param view_player 当前玩家
 * @return score 当前棋盘状态当前玩家的净胜分
 */
int Evaluate(const ChessBoard* board, Piece view_player);

/**
 * @brief alpha-beta决策函数
 *
 * @param board 棋盘指针
 * @param depth 试下深度
 * @param alpha 最大层最小值
 * @param beta 最小层最大值
 * @param current_player 当前模拟下的玩家
 * @param maximizing_player 决策收益玩家
 * @return int 经过模拟博弈后的玩家的净分数
 */
int AlphaBeta(ChessBoard* board, int depth, int alpha, int beta,
              Piece current_player, Piece maximizing_player);

/**
 * @brief 获取最优解
 *
 * @param board 棋盘指针
 * @param player 当前玩家
 * @param best_row 最优位置行坐标
 * @param best_col 最优位置列坐标
 */
void GetBestMove(ChessBoard* board, Piece player, int* best_row, int* best_col);

/**
 * @brief 初始化棋盘、窗口、游戏资源
 *
 */
void InitAll(void);

/**
 * @brief 对候选走法进行降序排序
 *
 * @param board   当前棋盘状态指针
 * @param rows    候选走法行坐标数组（既是输入也是输出，排序后重排）
 * @param cols    候选走法列坐标数组（既是输入也是输出，排序后重排）
 * @param count   候选走法总数
 * @param player  当前走棋方
 */
void SortMoves(ChessBoard* board, int* rows, int* cols, int count,
               Piece player);

/**
 * @brief 生成所有有邻居的合法走法
 *
 * @param board  当前棋盘状态指针（只读）
 * @param rows   输出参数，存放合法走法行坐标的数组
 * @param cols   输出参数，存放合法走法列坐标的数组
 * @return int   合法走法的数量
 */
int GenerateMoves(const ChessBoard* board, int* rows, int* cols);

/**
 * @brief 快速局部评估：计算在某空位落子后的即时收益
 *
 * @param board   当前棋盘状态指针
 * @param row     落子行坐标
 * @param col     落子列坐标
 * @param player  落子方（PLAYER_1 或 PLAYER_2）
 * @return int    四条线段的棋型评分总和
 */
int QuickScore(const ChessBoard* board, int row, int col, Piece player);
#endif