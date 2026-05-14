#include "main.h"

#include <limits.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>

#include "Button.h"
#include "Rendering.h"
#include "function.h"

int main() {  // 主函数开始
  int boardTotalSize = (BOARD_SIZE - 1) * CELL_SIZE + 2 * BOARD_MARGIN;
  GameResources gameResources = {0};
  InitGameResources(&gameResources, boardTotalSize + 300, boardTotalSize + 100,
                    "五子棋游戏test");
  struct window_size window_size;
  InitWindowSize(&window_size, boardTotalSize + 300, boardTotalSize + 100);
  Button** buttons;
  int buttonCount = 0;

  PlayMusicStream(gameResources.MenuBGM);
  while (!WindowShouldClose()) {  // 主循环从此处开始

    /* 在这里添加逻辑代码 */
    UpdateMusicStream(
        gameResources
            .MenuBGM);  // 更新音乐流（必须在主循环中调用以保持音乐播放）
    buttons =
        GetPageButtons(ButtonPage, &buttonCount);  // 获取当前按钮页按钮数组
    UpdateAllButtons(buttons, buttonCount);        // 更新所有按钮状态

    BeginDrawing();  // 窗口绘制开始

    ClearBackground(WHITE);  // 1.清理显存，设置背景色为白色
    DrawChessBoard(&gameResources, BOARD_SIZE,
                   &window_size);  // 2.绘制棋盘（使用木纹纹理）
    DrawAllButtons(buttons, buttonCount,
                   gameResources.chineseFont);  // 把所有按钮画到屏幕上
    EndDrawing();                               // 窗口绘制结束
  }  // 检测到用户关闭窗口才会退出主循环
  UnloadGameResources(&gameResources);
  CloseWindow();  // 窗口被关闭,卸载OpenGL上下文
  return 0;
}