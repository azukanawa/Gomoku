#include "Rendering.h"

#include <stdio.h>

// 函数实现
int InitWindowSize(struct window_size* size, int width, int height) {
  size->width = width;
  size->height = height;
  size->width_half = width / 2;
  size->height_half = height / 2;
  return 0;
}

Button CreateButton(float x, float y, float width, float height,
                    const char* text, int fontSize, void (*onClick)(void),
                    Color colorNormal, Color colorHover, Color colorPressed,
                    Color textColor) {  // 输入所有参数,封装Button结构体
  Button btn = {0};
  btn.bounds = (Rectangle){x, y, width, height};
  btn.text = text;
  btn.fontSize = fontSize;
  btn.colorNormal = colorNormal;
  btn.colorHover = colorHover;
  btn.colorPressed = colorPressed;
  btn.textColor = textColor;
  btn.state = BUTTON_NORMAL;
  btn.onClick = onClick;
  return btn;
}

// 更新单个按钮状态（核心逻辑）
void UpdateButton(Button* button) {
  Vector2 mousePos = GetMousePosition();
  bool isMouseOver = CheckCollisionPointRec(mousePos, button->bounds);

  // 1. 更新按钮状态
  if (isMouseOver) {
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      button->state = BUTTON_PRESSED;
    } else {
      // 只有从按下状态松开时，才触发点击事件
      if (button->state == BUTTON_PRESSED) {
        if (button->onClick != NULL) {
          button->onClick();  // 自动调用绑定的函数
        }
      }
      button->state = BUTTON_HOVER;
    }
  } else {
    button->state = BUTTON_NORMAL;
  }
}

// 绘制单个按钮
void DrawButton(Button* button) {
  Color currentColor;
  switch (button->state) {  // 根据状态绘制不同的颜色
    case BUTTON_HOVER:
      currentColor = button->colorHover;
      break;
    case BUTTON_PRESSED:
      currentColor = button->colorPressed;
      break;
    default:
      currentColor = button->colorNormal;
      break;
  }

  // 绘制按钮背景(圆角矩形)
  DrawRectangleRounded(button->bounds, 0.2f, 8, currentColor);
  // 绘制按钮边框
  DrawRectangleRoundedLinesEx(button->bounds, 0.2f, 8, 2, GRAY);

  // 绘制居中文字
  int textWidth = MeasureText(button->text, button->fontSize);
  float textX = button->bounds.x + (button->bounds.width - textWidth) / 2;
  float textY =
      button->bounds.y + (button->bounds.height - button->fontSize) / 2;
  DrawText(button->text, textX, textY, button->fontSize, button->textColor);
}

// 自动更新所有按钮（一行代码搞定）
void UpdateAllButtons(Button* buttons[], int count) {
  for (int i = 0; i < count; i++) {
    UpdateButton(buttons[i]);
  }
}

// 全局变量定义（只在这里定义一次）
Color color_white = {255, 255, 255, 255};
Color color_black = {0, 0, 0, 255};