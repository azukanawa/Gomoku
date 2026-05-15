#include "main.h"

#include <limits.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>

#include "Button.h"
#include "Rendering.h"
#include "chessBoard.h"
#include "function.h"

// 全局变量定义（只在这里写一次，分配实际内存）
int g_boardSize = 13;
ChessBoard g_chessBoard;
Piece g_currentPlayer = PLAYER_1;  // 黑棋先行
GameResources gameResources = {0};
WindowSize window_size;
int main() {  // 主函数开始
  InitBoard(&g_chessBoard);
  InitWindowSize(&window_size);
  InitGameResources(&gameResources, &window_size);
  Music* currentBGM = &gameResources.MenuBGM;

  Button** buttons;
  int buttonCount = 0;

  PlayMusicStream(gameResources.MenuBGM);
  while (!WindowShouldClose() && ButtonPage != -1) {
    // 主循环从此处开始

    // 帧逻辑更新开始

    // 1.更新音乐流（保持音乐播放）
    UpdateMusicStream(gameResources.MenuBGM);

    // 2.更新窗口大小（如果用户调整了窗口大小）
    if (IsWindowResized()) {
      UpdateWindowSize(&window_size);
      InitAllGameButtons(&window_size);
      TraceLog(LOG_INFO, "窗口大小改变：%dx%d", window_size.width,
               window_size.height);
    }

    // 3.处理落子逻辑（如果在游戏内页面）
    if (ButtonPage == 1) {
      HandleChessPlacement(g_boardSize, &window_size);
    }
    // 4. 更新当前按钮状态

    buttons = GetPageButtons(ButtonPage, &buttonCount);
    UpdateAllButtons(buttons, buttonCount);
    // 更新所有按钮状态结束

    // 帧逻辑绘制结束
    // 窗口绘制开始
    BeginDrawing();
    // 1.清理显存，设置背景色为白色
    ClearBackground(WHITE);

    switch (ButtonPage) {
      case 0:
        DrawTexture(gameResources.title,
                    window_size.width_half - gameResources.title.width / 2,
                    window_size.height_half - 200, WHITE);
        break;
      case 1:
        DrawChessBoard(&gameResources, g_boardSize, &window_size);
        DrawChessPreview(g_boardSize, &window_size);
        DrawAllChessPieces(&gameResources, g_boardSize, &window_size);
        break;

      default:
        break;
    }

    // 5.绘制按钮
    DrawAllButtons(buttons, buttonCount, gameResources.chineseFont);
    // 窗口绘制结束
    EndDrawing();
  }  // 检测到用户关闭窗口才会退出主循环
  FreeBoard(&g_chessBoard);
  UnloadGameResources(&gameResources);
  CloseWindow();  // 窗口被关闭,卸载OpenGL上下文
  return 0;
}