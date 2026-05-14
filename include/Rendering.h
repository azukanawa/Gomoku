#ifndef RENDERING_H
#define RENDERING_H

#define CELL_SIZE 40      // 每个格子的像素大小（标准围棋格子约40x40）
#define BOARD_MARGIN 30   // 棋盘边缘留白（防止棋子超出边界）
#define LINE_WIDTH 2.0f   // 网格线宽度
#define LINE_COLOR BLACK  // 网格线颜色

#include "raylib.h"

// 窗口大小结构体定义
struct window_size {
  int width;
  int height;
  int width_half;
  int height_half;
};
// Button.C文件依赖这里定义的窗口大小结构体
#include "Button.h"

typedef struct {
  Font chineseFont;
  Texture2D chessBoard13;
  Texture2D wood1;
  Music MenuBGM;  // 背景音乐
} GameResources;

// 渲染相关函数声明(按钮渲染在Button)
int InitWindowSize(struct window_size* size);
void InitChineseFont(const char* fontPath, int fontSize, const char* allText,
                     Font* g_chineseFont);

int InitGameResources(
    GameResources* res,
    struct window_size* winSize);  // 初始化所有资源，成功返回0，失败返回-1

void UpdateWindowSize(struct window_size* winSize);

void UnloadGameResources(GameResources* res);  // 卸载所有资源

int DrawChessBoard(
    const GameResources* res, int boardSize,
    const struct window_size* winSize);  // 绘制棋盘，成功返回0，失败返回-1

// 全局变量声明（使用extern）
extern Color color_white;
extern Color color_black;

#endif  // RENDERING_H