#ifndef BUTTON_H
#define BUTTON_H

#include "raylib.h"

// 按钮状态枚举
typedef enum { BUTTON_NORMAL, BUTTON_HOVER, BUTTON_PRESSED } ButtonState;

// 通用按钮结构体
typedef struct {
  Rectangle bounds;
  const char* text;
  int fontSize;
  Color colorNormal;
  Color colorHover;
  Color colorPressed;
  Color textColor;
  float borderRadius;  // 圆角大小(0-1)
  float borderThick;   // 边框粗细(像素)
  Color borderColor;   // 边框颜色
  ButtonState state;
  void (*onClick)(void);  // 点击回调
} Button;

// ------------------------------
// 按钮核心API
// ------------------------------
// 创建按钮
Button CreateButton(float x, float y, float width, float height,
                    const char* text, int fontSize, void (*onClick)(void),
                    Color colorNormal, Color colorHover, Color colorPressed,
                    Color textColor);

// 更新单个按钮状态
void UpdateButton(Button* button);

// 绘制单个按钮
void DrawButton(Button* button, Font g_chineseFont);

// 批量更新/绘制所有按钮
void UpdateAllButtons(Button* buttons[], int count);
void DrawAllButtons(Button* buttons[], int count, Font g_chineseFont);

// ------------------------------
// 你的项目专属按钮管理API
// ------------------------------
// 初始化所有游戏按钮（只在main开头调用一次）
void InitAllGameButtons(void);

// 获取主菜单按钮数组
Button** GetMenuButtons(int* outCount);

// 获取游戏内按钮数组
Button** GetGameButtons(int* outCount);

#endif  // BUTTON_H