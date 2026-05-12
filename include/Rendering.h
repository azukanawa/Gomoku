#ifndef RENDERING_H
#define RENDERING_H

#include "raylib.h"

// 结构体定义
struct window_size {
  int width;
  int height;
  int width_half;
  int height_half;
};

// 按钮状态枚举
typedef enum {
  BUTTON_NORMAL,  // 正常状态
  BUTTON_HOVER,   // 鼠标悬停
  BUTTON_PRESSED  // 鼠标按下
} ButtonState;

typedef struct {          // 按钮的结构体内容
  Rectangle bounds;       // 按钮位置和大小(x,y,width,height)
  const char* text;       // 按钮文字
  int fontSize;           // 文字大小
  Color colorNormal;      // 正常颜色
  Color colorHover;       // 悬停颜色
  Color colorPressed;     // 按下颜色
  Color textColor;        // 文字颜色
  ButtonState state;      // 当前状态
  void (*onClick)(void);  // 点击回调函数（点击时自动调用）
} Button;

typedef struct {
  Rectangle bounds;
  Texture2D textureNormal;   // 正常状态图片
  Texture2D textureHover;    // 悬停状态图片
  Texture2D texturePressed;  // 按下状态图片
  ButtonState state;
  void (*onClick)(void);
} ImageButton;  // 图片按钮结构体

// 函数声明
int InitWindowSize(struct window_size* size, int width, int height);

// 按钮系统函数声明
Button CreateButton(float x, float y, float width, float height,
                    const char* text, int fontSize, void (*onClick)(void),
                    Color colorNormal, Color colorHover, Color colorPressed,
                    Color textColor);       // 输入所有参数,封装按钮结构体
void UpdateButton(Button* button);          // 更新按钮状态（逻辑环节调用）
void DrawButton(Button* button);            // 绘制按钮（渲染环节调用）
void DrawImageButton(ImageButton* button);  // 绘制图片按钮
void UpdateAllButtons(Button* buttons[], int count);  // 自动更新所有按钮
void DrawAllButtons(Button* buttons[], int count);    // 自动绘制所有按钮

// 全局变量声明（使用extern）
extern Color color_white;
extern Color color_black;

#endif  // RENDERING_H