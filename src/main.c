#include "main.h"

#include <limits.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>

#include "Rendering.h"
#include "function.h"

int main() {
  struct window_size window_size;
  InitWindowSize(&window_size, 800, 600);
  InitWindow(window_size.width, window_size.height, "五子棋游戏test");
  Texture ChessBoard_13 = LoadTexture("../res/Picture/chess_board_13.jpg");
  SetTargetFPS(60);  // 设置帧率为60帧每秒
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(WHITE);
    /* 在这里添加渲染代码 */
    DrawTexture(ChessBoard_13, 0, 0, WHITE);
    EndDrawing();
    // 在这里添加逻辑判断代码
  }
  CloseWindow();  // 窗口被关闭,卸载OpenGL上下文
  return 0;
}