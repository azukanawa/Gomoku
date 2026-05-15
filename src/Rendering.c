#include "Rendering.h"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

#include "main.h"

// 函数实现
int InitWindowSize(WindowSize* size) {
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

int InitGameResources(GameResources* res, WindowSize* winSize) {
  // 先初始化窗口
  SetTargetFPS(60);
  InitWindow(winSize->width, winSize->height, "五子棋游戏test");
  SetWindowState(FLAG_WINDOW_RESIZABLE);  // 设置窗口可变分辨率
  SetWindowMinSize(800, 600);

  // 开始初始化所有资源

  // 1. 初始化字体
  InitChineseFont("../res/Font/微软雅黑.ttf", 20,
                  "开始游戏重新退出悔棋放弃人机对战双人对战游戏结束，黑棋获胜！"
                  "游戏结束，白棋获胜！游戏结束，平局！",
                  &res->chineseFont);
  // 1. 初始化字体结束

  // 2. 初始化纹理
  res->wood1 = LoadTexture("../res/Picture/wood1.jpg");
  res->title = LoadTexture("../res/Picture/title.png");
  res->Background = LoadTexture("../res/Picture/background.jpeg");
  res->Table = LoadTexture("../res/Picture/table.jpg");
  // 2. 初始化纹理结束

  // 3. 初始化音频设备
  InitAudioDevice();
  res->DownSound = LoadSound("../res/Sound/落子.mp3");
  SetSoundVolume(res->DownSound, 1.0f);  // 设置落子音效音量为200%
  res->MenuBGM = LoadMusicStream("../res/Music/周志华-紫禁城冬雪.mp3");
  res->MenuBGM.looping = true;
  SetMusicVolume(res->MenuBGM, 0.25f);  // 设置BGM音量为25%
  res->GameBGM = LoadMusicStream("../res/Music/InGame.mp3");
  res->GameBGM.looping = true;
  SetMusicVolume(res->GameBGM, 0.25f);  // 设置BGM音量为25%
  res->currentBGM = &res->MenuBGM;      // 默认当前BGM为菜单音乐

  // 3. 初始化音频设备结束

  // 4. 初始化按钮
  InitAllGameButtons(winSize);
  // 4. 初始化按钮结束

  // 5.最后检查初始化是否成功
  // 5.最后检查结束

  return 0;
}

void UnloadGameResources(GameResources* res) {
  TraceLog(LOG_INFO, "=== 开始卸载游戏资源 ===");
  if (res->wood1.id != 0) UnloadTexture(res->wood1);
  if (res->MenuBGM.stream.buffer != 0) UnloadMusicStream(res->MenuBGM);
  // 卸载其他资源
  TraceLog(LOG_INFO, "=== 所有资源卸载完成，程序正常退出 ===");
  CloseWindow();
}

void UpdateWindowSize(
    WindowSize* winSize) {  // 每帧更新窗口大小,重新计算元素位置
  winSize->width = GetScreenWidth();
  winSize->height = GetScreenHeight();
  winSize->width_half = winSize->width / 2;
  winSize->height_half = winSize->height / 2;
}

void RenderingBackground(void) {
  switch (ButtonPage) {
    case 0:  // 主菜单页面
      DrawTexturePro(
          g_gameResources.Background,
          (Rectangle){0, 0, g_gameResources.Background.width,
                      g_gameResources.Background.height},
          (Rectangle){0, 0, g_window_size.width, g_window_size.height},
          (Vector2){0, 0}, 0.0f, WHITE);
      DrawTexture(g_gameResources.title,
                  g_window_size.width_half - g_gameResources.title.width / 2,
                  g_window_size.height_half - 200, WHITE);
      break;
    case 1:  // 游戏内页面
    case 2:
      DrawTexturePro(
          g_gameResources.Table,
          (Rectangle){0, 0, g_gameResources.Table.width,
                      g_gameResources.Table.height - 80},
          (Rectangle){0, 0, g_window_size.width, g_window_size.height},
          (Vector2){0, 0}, 0.0f, WHITE);
      break;
    default:
      ClearBackground(WHITE);
      break;
  }
}

void DrawMessage(char* resultText) {
  // ========== 提示框样式参数（可直接调整） ==========
  const float boxWidth = 450.0f;
  const float boxHeight = 100.0f;
  const float borderRadius = 12.0f;
  const float borderThick = 4.0f;
  const int fontSize = 36;
  const Color fillColor = WHITE;
  const Color borderColor = DARKGRAY;
  const Color textColor = BLACK;

  // 计算屏幕中心位置
  const float boxX = (GetScreenWidth() - boxWidth) / 2.0f;
  const float boxY = (GetScreenHeight() - boxHeight) / 2.0f;
  const Rectangle boxBounds = {boxX, boxY, boxWidth, boxHeight};

  // ========== 绘制提示框 ==========
  // 绘制填充圆角矩形
  DrawRectangleRounded(boxBounds, borderRadius, 8, fillColor);

  // 绘制圆角边框
  DrawRectangleRoundedLinesEx(boxBounds, borderRadius, 8, borderThick,
                              borderColor);

  // 计算文字大小并居中
  Vector2 textSize =
      MeasureTextEx(g_gameResources.chineseFont, resultText, fontSize, 1.0f);
  float textX = boxBounds.x + (boxBounds.width - textSize.x) / 2.0f;
  float textY = boxBounds.y + (boxBounds.height - textSize.y) / 2.0f;

  // 绘制居中文字
  DrawTextEx(g_gameResources.chineseFont, resultText, (Vector2){textX, textY},
             fontSize, 1.0f, textColor);
}

// 全局变量定义（只在这里定义一次）
Color color_white = {255, 255, 255, 255};
Color color_black = {0, 0, 0, 255};