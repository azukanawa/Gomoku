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
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(WHITE);
    /* code */
    EndDrawing();
  }
  CloseWindow();  // 窗口被关闭,卸载OpenGL上下文
  return 0;
}