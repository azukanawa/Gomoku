#include "main.h"

#include <limits.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>

#include "Rendering.h"
#include "function.h"

int main() {
  // 设置渲染的基本参数
  struct window_size window_size;
  SetTargetFPS(60);  // 设置帧率为60帧每秒
  InitWindowSize(&window_size, 800, 600);
  // 初始化窗口,加载材质
  InitWindow(window_size.width, window_size.height, "五子棋游戏test");
  Texture ChessBoard_13 = LoadTexture("../res/Picture/chess_board_13.jpg");

  while (!WindowShouldClose()) {  // 主循环从此处开始

    /* 在这里添加逻辑代码 */

    BeginDrawing();  // 窗口绘制开始

    ClearBackground(WHITE);
    /* 在这里添加渲染代码 */
    DrawTexture(ChessBoard_13, 0, 0, WHITE);

    EndDrawing();  // 窗口绘制结束
  }  // 检测到用户关闭窗口才会退出主循环
  CloseWindow();  // 窗口被关闭,卸载OpenGL上下文
  return 0;
}