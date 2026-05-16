#include <limits.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>

#include "Button.h"
#include "chessBoard.h"
#include "function.h"
#include "game.h"

// 全局变量定义（只在这里写一次，分配实际内存）
int g_boardSize = 13;
ChessBoard g_chessBoard;
Piece g_currentPlayer = PLAYER_1;  // 黑棋先行
GameResources g_gameResources = {0};
WindowSize g_window_size;
PositionStack g_positionStack;
int main() {                   // 主函数开始
  GameResult whoWin = NOTEND;  // 当前游戏结果，初始为 NOTEND（未结束）
  InitAll(&g_positionStack);

  PlayMusicStream(g_gameResources.MenuBGM);
  while (!WindowShouldClose() && ButtonPage != -1) {
    // 主循环从此处开始
    Button** buttons;
    int buttonCount = 0;

    // 帧逻辑更新开始

    // 1.更新音乐流（保持音乐播放）
    UpdateMusicStream(*g_gameResources.currentBGM);

    // 2.更新窗口大小（如果用户调整了窗口大小）
    if (IsWindowResized()) {
      UpdateWindowSize(&g_window_size);
      InitAllGameButtons(&g_window_size);
      TraceLog(LOG_INFO, "窗口大小改变：%dx%d", g_window_size.width,
               g_window_size.height);
    }

    // 3.处理落子逻辑（如果在游戏内页面）
    if (ButtonPage == 1 && whoWin == NOTEND) {
      whoWin = LocalTwoPlayerMode();
    } else if (ButtonPage == 2 && whoWin == NOTEND) {
      whoWin = AiMode();
    }

    // 4. 更新当前按钮状态

    buttons = GetPageButtons(ButtonPage, &buttonCount);
    UpdateAllButtons(buttons, buttonCount);
    // 更新所有按钮状态结束

    // 帧逻辑绘制结束
    // 窗口绘制开始
    BeginDrawing();
    // 1.清理显存，设置背景色为白色

    RenderingBackground();
    // 2.根据当前页面绘制背景（菜单背景或游戏内背景）
    switch (ButtonPage) {
      case 0:  // 主菜单页面
        whoWin = NOTEND;
        g_currentPlayer = PLAYER_1;
        break;
      case 1:  // 游戏内页面
      case 2:
        DrawChessBoard(&g_gameResources, g_boardSize, &g_window_size);
        if (whoWin == NOTEND) {
          DrawChessPreview(g_boardSize, &g_window_size);
        }
        DrawAllChessPieces(&g_gameResources, g_boardSize, &g_window_size);
        switch (whoWin) {
          case NOTEND:
            break;
          case PLAYER_1_WIN:
            DrawMessage("游戏结束，黑棋获胜！");
            break;
          case PLAYER_2_WIN:
            DrawMessage("游戏结束，白棋获胜！");
            break;
          case DRAW:
            DrawMessage("游戏结束，平局！");
            break;
          default:
            break;
        }
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
  DestroyPositionStack(&g_positionStack);
  UnloadGameResources(&g_gameResources);
  CloseWindow();  // 窗口被关闭,卸载OpenGL上下文
  return 0;
}