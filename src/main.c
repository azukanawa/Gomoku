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

// ==============================================
// ✅ 全局变量定义（只在这里写一次，分配实际内存）
// ==============================================
ChessBoard g_chessBoard = {0};     // 初始化为全零
Piece g_currentPlayer = PLAYER_1;  // 黑棋先行
GameResources gameResources = {0};
struct window_size window_size;
ChessBoard chessBoard = {0};  // 初始化棋盘结构体
int main() {                  // 主函数开始

  InitWindowSize(&window_size);
  InitGameResources(&gameResources, &window_size);
  Music* currentBGM = &gameResources.MenuBGM;

  Button** buttons;
  int buttonCount = 0;

  PlayMusicStream(gameResources.MenuBGM);
  while (!WindowShouldClose() && ButtonPage != -1) {  // 主循环从此处开始

    // 帧逻辑更新开始
    UpdateMusicStream(gameResources.MenuBGM);  // 更新音乐流（保持音乐播放）

    if (IsWindowResized()) {
      // 1. 更新窗口大小结构体
      UpdateWindowSize(&window_size);

      // 2. 重新初始化按钮，自动计算新位置
      InitAllGameButtons(&window_size);

      // 3. 如果你有其他需要自适应的元素（比如文字、面板），在这里更新
      TraceLog(LOG_INFO, "窗口大小改变：%dx%d", window_size.width,
               window_size.height);
    }
    buttons =
        GetPageButtons(ButtonPage, &buttonCount);  // 获取当前按钮页按钮数组
    UpdateAllButtons(buttons, buttonCount);        // 更新所有按钮状态

    HandleChessPlacement(BOARD_SIZE, &window_size);
    // 帧逻辑绘制结束
    // 窗口绘制开始
    BeginDrawing();

    ClearBackground(WHITE);  // 1.清理显存，设置背景色为白色
    DrawChessBoard(&gameResources, BOARD_SIZE,
                   &window_size);  // 2.绘制棋盘（使用木纹纹理）
    DrawAllChessPieces(&gameResources, BOARD_SIZE, &window_size);
    DrawChessPreview(BOARD_SIZE, &window_size);
    DrawAllButtons(buttons, buttonCount,
                   gameResources.chineseFont);  // 把所有按钮画到屏幕上
    EndDrawing();                               // 窗口绘制结束
  }  // 检测到用户关闭窗口才会退出主循环
  UnloadGameResources(&gameResources);
  CloseWindow();  // 窗口被关闭,卸载OpenGL上下文
  return 0;
}