#include "Button.h"

#include <stddef.h>

// ------------------------------
// 外部回调函数声明（告诉编译器这些函数在其他文件里）
// ------------------------------
void OnStartGame(void);
void OnRestartGame(void);
void OnExitGame(void);
void OnUndoMove(void);

// ------------------------------
// 私有静态变量（只在本文件可见，不会污染全局）
// ------------------------------
static Button btnStart;
static Button btnRestart;
static Button btnExit;
static Button btnUndo;

static Button* menuButtons[2];
static int menuButtonCount = 2;

static Button* gameButtons[2];
static int gameButtonCount = 2;

// ------------------------------
// 核心函数实现
// ------------------------------
Button CreateButton(float x, float y, float width, float height,
                    const char* text, int fontSize, void (*onClick)(void),
                    Color colorNormal, Color colorHover, Color colorPressed,
                    Color textColor) {
  Button btn = {0};
  btn.bounds = (Rectangle){x, y, width, height};
  btn.text = text;
  btn.fontSize = fontSize;
  btn.colorNormal = colorNormal;
  btn.colorHover = colorHover;
  btn.colorPressed = colorPressed;
  btn.textColor = textColor;
  btn.borderRadius = 0.2f;  // 默认圆角
  btn.borderThick = 2.0f;   // 默认边框粗细
  btn.borderColor = GRAY;   // 默认边框颜色
  btn.state = BUTTON_NORMAL;
  btn.onClick = onClick;
  return btn;
}

void UpdateButton(Button* button) {
  if (button == NULL) return;

  Vector2 mousePos = GetMousePosition();
  bool isMouseOver = CheckCollisionPointRec(mousePos, button->bounds);

  if (isMouseOver) {
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      button->state = BUTTON_PRESSED;
    } else {
      if (button->state == BUTTON_PRESSED && button->onClick != NULL) {
        button->onClick();
      }
      button->state = BUTTON_HOVER;
    }
  } else {
    button->state = BUTTON_NORMAL;
  }
}

void DrawButton(Button* button, Font g_chineseFont) {
  if (button == NULL) return;

  Color currentColor;
  switch (button->state) {
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

  // 绘制背景
  DrawRectangleRounded(button->bounds, button->borderRadius, 8, currentColor);
  // 绘制边框(使用圆角矩形)
  DrawRectangleRoundedLinesEx(button->bounds, button->borderRadius, 8,
                              button->borderThick, button->borderColor);
  // 绘制居中文字
  Vector2 textSize = MeasureTextEx(g_chineseFont,     // 你加载的中文字体
                                   button->text,      // 按钮文字
                                   button->fontSize,  // 字体大小
                                   1.0f  // 字符间距（和 DrawTextEx 保持一致）
  );
  float textX = button->bounds.x + (button->bounds.width - textSize.x) / 2.0f;
  float textY = button->bounds.y + (button->bounds.height - textSize.y) / 2.0f;
  button->bounds.y + (button->bounds.height - button->fontSize) / 2;
  DrawTextEx(
      g_chineseFont,  // 字体对象
      button->text,   // 文字内容
      (Vector2){textX,
                textY},  // 位置：必须是 Vector2 类型，不能传两个单独的 float
      button->fontSize,  // 字体大小
      1.0f,              // 字符间距（推荐 1.0f）
      button->textColor  // 文字颜色
  );
}

void UpdateAllButtons(Button* buttons[], int count) {
  for (int i = 0; i < count; i++) {
    UpdateButton(buttons[i]);
  }
}

void DrawAllButtons(Button* buttons[], int count, Font g_chineseFont) {
  for (int i = 0; i < count; i++) {
    DrawButton(buttons[i], g_chineseFont);
  }
}

// ------------------------------
// 项目专属按钮初始化
// ------------------------------
void InitAllGameButtons(void) {
  // 主菜单按钮
  btnStart = CreateButton(300, 200, 200, 50, "开始游戏", 24, OnStartGame,
                          (Color){76, 175, 80, 255}, (Color){56, 142, 60, 255},
                          (Color){27, 94, 32, 255}, WHITE);

  btnExit = CreateButton(300, 270, 200, 50, "退出游戏", 24, OnExitGame,
                         (Color){244, 67, 54, 255}, (Color){211, 47, 47, 255},
                         (Color){183, 28, 28, 255}, WHITE);

  // 游戏内按钮
  btnRestart =
      CreateButton(650, 100, 120, 40, "重新开始", 20, OnRestartGame,
                   (Color){33, 150, 243, 255}, (Color){25, 118, 210, 255},
                   (Color){13, 71, 161, 255}, WHITE);

  btnUndo = CreateButton(650, 160, 120, 40, "悔棋", 20, OnUndoMove,
                         (Color){255, 152, 0, 255}, (Color){245, 124, 0, 255},
                         (Color){230, 81, 0, 255}, WHITE);

  // 组装数组
  menuButtons[0] = &btnStart;
  menuButtons[1] = &btnExit;

  gameButtons[0] = &btnRestart;
  gameButtons[1] = &btnUndo;
}

Button** GetMenuButtons(int* outCount) {
  if (outCount != NULL) *outCount = menuButtonCount;
  return menuButtons;
}

Button** GetGameButtons(int* outCount) {
  if (outCount != NULL) *outCount = gameButtonCount;
  return gameButtons;
}

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
