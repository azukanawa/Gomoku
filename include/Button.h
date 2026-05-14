#ifndef BUTTON_H
#define BUTTON_H

#include "Rendering.h"
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

void UpdateButton(Button* button);                    // 更新单个按钮状态
void DrawButton(Button* button, Font g_chineseFont);  // 绘制单个按钮

// 批量更新/绘制所有按钮
void UpdateAllButtons(Button* buttons[], int count);
void DrawAllButtons(Button* buttons[], int count, Font g_chineseFont);

void InitAllGameButtons(const struct window_size* winSize);

Button** GetPageButtons(int ButtonPage,
                        int* outCount);  // 根据当前页面返回对应的按钮数组和数量

extern int ButtonPage;  // 0表示主菜单，1表示游戏内
#endif                  // BUTTON_H