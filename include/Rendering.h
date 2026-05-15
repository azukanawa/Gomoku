#ifndef RENDERING_H
#define RENDERING_H

#define CELL_SIZE 40      // 每个格子的像素大小（标准围棋格子约40x40）
#define BOARD_MARGIN 30   // 棋盘边缘留白（防止棋子超出边界）
#define LINE_WIDTH 2.0f   // 网格线宽度
#define LINE_COLOR BLACK  // 网格线颜色

#include "raylib.h"

// 窗口大小结构体定义
typedef struct window_size {
  int width;
  int height;
  int width_half;
  int height_half;
} WindowSize;
// Button.C文件依赖这里定义的窗口大小结构体
#include "Button.h"

typedef struct {
  Font chineseFont;
  Texture2D wood1;
  Texture2D title;
  Texture2D Background;
  Texture2D Table;
  Music MenuBGM;
  Music GameBGM;
  Music* currentBGM;
  Sound DownSound;
} GameResources;

extern GameResources g_gameResources;  // 全局游戏资源结构体
extern WindowSize g_window_size;       // 全局窗口尺寸结构体
// 以下是函数声明

/**
 * @brief 初始化窗口尺寸结构体
 * @param size 窗口尺寸结构体指针
 * @return 成功返回0
 */
int InitWindowSize(struct window_size* size);

/**
 * @brief 初始化中文字体
 * @param fontPath 字体文件路径
 * @param fontSize 字体大小
 * @param allText 需要显示的所有中文字符
 * @param g_chineseFont 输出：加载完成的字体结构体指针
 */
void InitChineseFont(const char* fontPath, int fontSize, const char* allText,
                     Font* g_chineseFont);

/**
 * @brief 初始化游戏所有资源
 * @param res 游戏资源结构体指针
 * @param winSize 窗口尺寸结构体指针
 * @return 成功返回0，失败返回-1
 */
int InitGameResources(
    GameResources* res,
    struct window_size* winSize);  // 初始化所有资源，成功返回0，失败返回-1

/**
 * @brief 每帧更新窗口大小，重新计算元素位置
 * @param winSize 窗口尺寸结构体指针
 */
void UpdateWindowSize(struct window_size* winSize);

/**
 * @brief 卸载游戏所有资源
 * @param res 游戏资源结构体指针
 */
void UnloadGameResources(GameResources* res);  // 卸载所有资源

void RenderingBackground(void);  // 从全局变量中获取当前页面信息，绘制背景

// 全局变量声明（使用extern）
extern Color color_white;
extern Color color_black;

#endif  // RENDERING_H