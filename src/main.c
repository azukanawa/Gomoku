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
  GameResources gameResources = {0};
  InitGameResources(&gameResources, 800, 600, "五子棋游戏test");
  Button** buttons;
  int buttonCount = 0;

  while (!WindowShouldClose()) {  // 主循环从此处开始

    /* 在这里添加逻辑代码 */
    buttons =
        GetPageButtons(ButtonPage, &buttonCount);  // 获取当前按钮页按钮数组
    UpdateAllButtons(buttons, buttonCount);        // 更新所有按钮状态

    BeginDrawing();  // 窗口绘制开始

    ClearBackground(WHITE);
    /* 在这里添加渲染代码 */
    DrawTexture(gameResources.chessBoard13, 0, 0, WHITE);
    DrawAllButtons(buttons, buttonCount,
                   gameResources.chineseFont);  // 把所有按钮画到屏幕上

    EndDrawing();  // 窗口绘制结束
  }  // 检测到用户关闭窗口才会退出主循环
  UnloadGameResources(&gameResources);
  CloseWindow();  // 窗口被关闭,卸载OpenGL上下文
  return 0;
}