#ifndef RENDERING_H
#define RENDERING_H

#include "Button.h"
#include "raylib.h"

// 结构体定义
struct window_size {
  int width;
  int height;
  int width_half;
  int height_half;
};

typedef struct {
  Font chineseFont;
  Texture2D chessBoard13;
  // Texture2D blackChess;
  // Texture2D whiteChess;
  // Sound clickSound;
  //  后续新增资源都在这里添加
} GameResources;

// 渲染相关函数声明(按钮渲染在Button)
int InitWindowSize(struct window_size* size, int width, int height);
void InitChineseFont(const char* fontPath, int fontSize, const char* allText,
                     Font* g_chineseFont);

int InitGameResources(
    GameResources* res, int windowWidth, int windowHeight,
    const char* title);  // 初始化所有资源，成功返回0，失败返回-1

void UnloadGameResources(GameResources* res);  // 卸载所有资源

// 全局变量声明（使用extern）
extern Color color_white;
extern Color color_black;

#endif  // RENDERING_H