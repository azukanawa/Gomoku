#include "main.h"

#include <limits.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>

#include "Button.h"
#include "Rendering.h"
#include "function.h"

// ------------------------------
// 空回调函数占位符（先通过编译，看渲染效果）
// 后续想加功能，直接在对应函数里写代码即可
// ------------------------------

void OnStartGame(void) {
  // 暂时什么都不做
  TraceLog(LOG_INFO, "点击了【开始游戏】按钮");
}

void OnRestartGame(void) {
  // 暂时什么都不做
  TraceLog(LOG_INFO, "点击了【重新开始】按钮");
}

void OnExitGame(void) {
  // 暂时什么都不做
  TraceLog(LOG_INFO, "点击了【退出游戏】按钮");
}

void OnUndoMove(void) {
  // 暂时什么都不做
  TraceLog(LOG_INFO, "点击了【悔棋】按钮");
}

int main() {  // 主函数开始
  // 设置渲染的基本参数
  struct window_size window_size;
  SetTargetFPS(60);  // 设置帧率为60帧每秒
  InitWindowSize(&window_size, 800, 600);
  // 初始化窗口,加载材质
  InitWindow(window_size.width, window_size.height, "五子棋游戏test");
  Texture ChessBoard_13 = LoadTexture("../res/Picture/chess_board_13.jpg");
  InitAllGameButtons();  // 初始化所有按钮

  while (!WindowShouldClose()) {  // 主循环从此处开始

    int buttonCount;
    Button** buttons;
    /* 在这里添加逻辑代码 */
    buttons = GetMenuButtons(&buttonCount);  // 获取主菜单按钮数组
    UpdateAllButtons(buttons, buttonCount);  // 更新所有按钮状态

    BeginDrawing();  // 窗口绘制开始

    ClearBackground(WHITE);
    /* 在这里添加渲染代码 */
    DrawTexture(ChessBoard_13, 0, 0, WHITE);
    buttons = GetMenuButtons(&buttonCount);
    DrawAllButtons(buttons, buttonCount);  // 把所有按钮画到屏幕上

    EndDrawing();  // 窗口绘制结束
  }  // 检测到用户关闭窗口才会退出主循环
  UnloadTexture(ChessBoard_13);
  CloseWindow();  // 窗口被关闭,卸载OpenGL上下文
  return 0;
}