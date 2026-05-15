#include "Rendering.h"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

#include "main.h"

// 函数实现
int InitWindowSize(struct window_size* size) {
  int boardTotalSize = (g_boardSize - 1) * CELL_SIZE + 2 * BOARD_MARGIN;
  size->width = boardTotalSize + 400;
  size->height = boardTotalSize + 200;
  size->width_half = (boardTotalSize + 400) / 2;
  size->height_half = (boardTotalSize + 200) / 2;
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

int InitGameResources(GameResources* res, struct window_size* winSize) {
  // 先初始化窗口
  SetTargetFPS(60);
  InitWindow(winSize->width, winSize->height, "五子棋游戏test");
  SetWindowState(FLAG_WINDOW_RESIZABLE);  // 设置窗口可变分辨率
  SetWindowMinSize(800, 600);

  // 初始化所有资源
  InitChineseFont("../res/Font/微软雅黑.ttf", 20, "开始游戏重新退出悔棋",
                  &res->chineseFont);

  res->chessBoard13 = LoadTexture("../res/Picture/chess_board_13.jpg");
  res->wood1 = LoadTexture("../res/Picture/wood1.jpg");
  res->DownSound = LoadSound("../res/Sound/落子.mp3");
  if (res->chessBoard13.id == 0) {
    TraceLog(LOG_ERROR, "棋盘纹理加载失败");
    return -1;
  }

  InitAudioDevice();  // 初始化音频设备
  res->MenuBGM = LoadMusicStream("../res/Music/周志华-紫禁城冬雪.mp3");
  res->MenuBGM.looping = true;
  InitAllGameButtons(winSize);  // 首次按钮初始化

  return 0;
}

void UnloadGameResources(GameResources* res) {
  TraceLog(LOG_INFO, "=== 开始卸载游戏资源 ===");
  if (res->chessBoard13.id != 0) UnloadTexture(res->chessBoard13);
  if (res->wood1.id != 0) UnloadTexture(res->wood1);
  if (res->MenuBGM.stream.buffer != 0) UnloadMusicStream(res->MenuBGM);
  // 卸载其他资源
  TraceLog(LOG_INFO, "=== 所有资源卸载完成，程序正常退出 ===");
  CloseWindow();
}

void UpdateWindowSize(
    struct window_size* winSize) {  // 每帧更新窗口大小,重新计算元素位置
  winSize->width = GetScreenWidth();
  winSize->height = GetScreenHeight();
  winSize->width_half = winSize->width / 2;
  winSize->height_half = winSize->height / 2;
}

// 全局变量定义（只在这里定义一次）
Color color_white = {255, 255, 255, 255};
Color color_black = {0, 0, 0, 255};