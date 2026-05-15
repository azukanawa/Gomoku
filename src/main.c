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
GameResources g_gameResources = {0};
WindowSize g_window_size;
int main() {  // 主函数开始
  InitBoard(&g_chessBoard);
  InitWindowSize(&g_window_size);
  InitGameResources(&g_gameResources, &g_window_size);
  Music* currentBGM = &g_gameResources.MenuBGM;

  PlayMusicStream(g_gameResources.MenuBGM);
  while (!WindowShouldClose() && ButtonPage != -1) {
    // 主循环从此处开始
    Button** buttons;
    int buttonCount = 0;

    // 帧逻辑更新开始

    // 1.更新音乐流（保持音乐播放）
    UpdateMusicStream(g_gameResources.MenuBGM);

    // 2.更新窗口大小（如果用户调整了窗口大小）
    if (IsWindowResized()) {
      UpdateWindowSize(&g_window_size);
      InitAllGameButtons(&g_window_size);
      TraceLog(LOG_INFO, "窗口大小改变：%dx%d", g_window_size.width,
               g_window_size.height);
    }

    // 3.处理落子逻辑（如果在游戏内页面）
    if (ButtonPage == 1) {
      HandleChessPlacement(g_boardSize, &g_window_size);
    }
    // 4. 更新当前按钮状态

    buttons = GetPageButtons(ButtonPage, &buttonCount);
    UpdateAllButtons(buttons, buttonCount);
    // 更新所有按钮状态结束

    // 帧逻辑绘制结束
    // 窗口绘制开始
    BeginDrawing();
    // 1.清理显存，设置背景色为白色

    switch (ButtonPage) {
      case 0:
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
      case 1:
        DrawTexturePro(
            g_gameResources.Table,
            (Rectangle){0, 0, g_gameResources.Table.width,
                        g_gameResources.Table.height - 80},
            (Rectangle){0, 0, g_window_size.width, g_window_size.height},
            (Vector2){0, 0}, 0.0f, WHITE);
        DrawChessBoard(&g_gameResources, g_boardSize, &g_window_size);
        DrawChessPreview(g_boardSize, &g_window_size);
        DrawAllChessPieces(&g_gameResources, g_boardSize, &g_window_size);
        break;

      default:
        break;
    }

    // 5.绘制按钮
    DrawAllButtons(buttons, buttonCount, g_gameResources.chineseFont);
    // 窗口绘制结束
    EndDrawing();
  }  // 检测到用户关闭窗口才会退出主循环
  FreeBoard(&g_chessBoard);
  UnloadGameResources(&g_gameResources);
  CloseWindow();  // 窗口被关闭,卸载OpenGL上下文
  return 0;
}