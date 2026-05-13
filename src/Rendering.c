#include "Rendering.h"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

// 函数实现
int InitWindowSize(struct window_size* size, int width, int height) {
  size->width = width;
  size->height = height;
  size->width_half = width / 2;
  size->height_half = height / 2;
  return 0;
}

void InitChineseFont(const char* fontPath, int fontSize, const char* allText,
                     Font* g_chineseFont) {
  int count = 0;
  int* codepoints = LoadCodepoints(allText, &count);
  *g_chineseFont = LoadFontEx(fontPath, fontSize, codepoints, count);
  free(codepoints);
}

Font g_chineseFont = {0};       // 初始化为空
Texture2D ChessBoard_13 = {0};  // 初始化为空

int InitRendering() {
  struct window_size window_size;
  SetTargetFPS(60);  // 设置帧率为60帧每秒
  InitWindowSize(&window_size, 800, 600);
  // 初始化窗口,加载材质
  InitWindow(window_size.width, window_size.height, "五子棋游戏test");
  // 加载字体图片音乐音效资源
  Font g_chineseFont;
  InitChineseFont("../res/Font/微软雅黑.ttf", 20, "开始游戏重新退出悔棋",
                  &g_chineseFont);
  Texture ChessBoard_13 = LoadTexture("../res/Picture/chess_board_13.jpg");
  InitAllGameButtons();  // 初始化所有按钮
  return 0;
}

int InitGameResources(GameResources* res, int windowWidth, int windowHeight,
                      const char* title) {
  // 先初始化窗口
  SetTargetFPS(60);
  InitWindow(windowWidth, windowHeight, title);

  // 初始化所有资源
  InitChineseFont("../res/Font/微软雅黑.ttf", 20, "开始游戏重新退出悔棋",
                  &res->chineseFont);

  res->chessBoard13 = LoadTexture("../res/Picture/chess_board_13.jpg");
  if (res->chessBoard13.id == 0) {
    TraceLog(LOG_ERROR, "棋盘纹理加载失败");
    return -1;
  }

  // 初始化其他资源
  // res->blackChess = LoadTexture(...);
  // res->whiteChess = LoadTexture(...);

  InitAllGameButtons();  // 按钮初始化

  return 0;
}

void UnloadGameResources(GameResources* res) {
  if (res->chessBoard13.id != 0) UnloadTexture(res->chessBoard13);
  // 卸载其他资源
  CloseWindow();
}

// 全局变量定义（只在这里定义一次）
Color color_white = {255, 255, 255, 255};
Color color_black = {0, 0, 0, 255};