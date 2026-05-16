#include "Button.h"

#include <stddef.h>

#include "Rendering.h"
#include "function.h"
#include "main.h"
#include "raylib.h"

// button回调函数声明
void OnStartGame(void);
void OnRestartGame(void);
void OnExitGame(void);
void OnUndoMove(void);
void OnStartGame_AI(void);

// ------------------------------
// 核心函数实现
// ------------------------------

// 把参数填入button结构体并返回
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

void UpdateButton(Button* button)  // 按钮状态更新函数
{
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

void DrawButton(Button* button, Font g_chineseFont)  // 绘制单个Button
{
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

void UpdateAllButtons(Button* buttons[],
                      int count) {  // 传入Button的指针数组和数量，批量更新状态
  for (int i = 0; i < count; i++) {
    UpdateButton(buttons[i]);
  }
}

void DrawAllButtons(
    Button* buttons[], int count,
    Font g_chineseFont) {  // 传入Button的指针数组和数量，批量绘制
  for (int i = 0; i < count; i++) {
    DrawButton(buttons[i], g_chineseFont);
  }
}

// 私有静态按钮结构体
static Button btnStart;
static Button btnStart_AI;
static Button btnRestart;
static Button btnExit;
static Button btnUndo;

// 指向按钮的指针数组们和按钮数量
static Button* menuButtons[3];
static int menuButtonCount = 3;
static Button* gameButtons[2];
static int gameButtonCount = 2;

int ButtonPage = 0;  // 0表示主菜单，1表示游戏内

#define BASE_WIDTH 800
#define BASE_HEIGHT 600

// ✅ 和头文件声明完全一模一样
void InitAllGameButtons(const struct window_size* winSize) {
  // 按钮绝对尺寸（保持不变）
  const float MENU_BTN_WIDTH = 200.0f;
  const float MENU_BTN_HEIGHT = 50.0f;
  const float GAME_BTN_WIDTH = 120.0f;
  const float GAME_BTN_HEIGHT = 40.0f;

  // 主菜单按钮（水平居中）
  const float menuBtnX = winSize->width_half - MENU_BTN_WIDTH / 2.0f;
  const float startBtn_AI_Y = winSize->height * (130.0f / BASE_HEIGHT);
  const float startBtnY = winSize->height * (200.0f / BASE_HEIGHT);
  const float exitBtnY = winSize->height * (270.0f / BASE_HEIGHT);

  btnStart_AI =
      CreateButton(menuBtnX, startBtn_AI_Y, MENU_BTN_WIDTH, MENU_BTN_HEIGHT,
                   "人机对战", 24, OnStartGame_AI, (Color){76, 175, 80, 255},
                   (Color){56, 142, 60, 255}, (Color){27, 94, 32, 255}, WHITE);

  btnStart =
      CreateButton(menuBtnX, startBtnY, MENU_BTN_WIDTH, MENU_BTN_HEIGHT,
                   "双人对战", 24, OnStartGame, (Color){76, 175, 80, 255},
                   (Color){56, 142, 60, 255}, (Color){27, 94, 32, 255}, WHITE);

  btnExit =
      CreateButton(menuBtnX, exitBtnY, MENU_BTN_WIDTH, MENU_BTN_HEIGHT,
                   "退出游戏", 24, OnExitGame, (Color){244, 67, 54, 255},
                   (Color){211, 47, 47, 255}, (Color){183, 28, 28, 255}, WHITE);

  // 游戏内按钮（右上角对齐）
  const float rightMargin = 30.0f;
  const float gameBtnX = winSize->width - rightMargin - GAME_BTN_WIDTH;
  const float restartBtnY = winSize->height * (100.0f / BASE_HEIGHT);
  const float undoBtnY = winSize->height * (160.0f / BASE_HEIGHT);

  btnRestart = CreateButton(
      gameBtnX, restartBtnY, GAME_BTN_WIDTH, GAME_BTN_HEIGHT, "放弃游戏", 20,
      OnRestartGame, (Color){33, 150, 243, 255}, (Color){25, 118, 210, 255},
      (Color){13, 71, 161, 255}, WHITE);

  btnUndo =
      CreateButton(gameBtnX, undoBtnY, GAME_BTN_WIDTH, GAME_BTN_HEIGHT, "悔棋",
                   20, OnUndoMove, (Color){255, 152, 0, 255},
                   (Color){245, 124, 0, 255}, (Color){230, 81, 0, 255}, WHITE);

  // 组装数组
  menuButtons[0] = &btnStart_AI;
  menuButtons[1] = &btnStart;
  menuButtons[2] = &btnExit;
  gameButtons[0] = &btnRestart;
  gameButtons[1] = &btnUndo;
}
Button** GetPageButtons(
    int ButtonPage, int* outCount) {  // 根据当前页面返回对应的按钮数组和数量
  if (ButtonPage == 0) {              // 0是标题页
    FreeBoard(&g_chessBoard);
    InitBoard(&g_chessBoard);
    DestroyPositionStack(&g_positionStack);
    InitPositionStack(&g_positionStack);
    *outCount = menuButtonCount;
    return menuButtons;
  } else if (ButtonPage == 1 || ButtonPage == 2) {
    *outCount = gameButtonCount;  // 1或2是游戏内
    return gameButtons;
  } else {
    return NULL;  // 无效页面
  }
}

void OnStartGame_AI(void) {
  ButtonPage = 2;                                         // 2代表人机对战
  StopMusicStream(*g_gameResources.currentBGM);           // 停止当前BGM
  PlayMusicStream(g_gameResources.GameBGM);               // 播放游戏内BGM
  g_gameResources.currentBGM = &g_gameResources.GameBGM;  // 切换到游戏内BGM
  TraceLog(LOG_INFO, "点击了【人机对战】按钮");
}

void OnStartGame(void) {
  ButtonPage = 1;                                         // 1代表双人对战
  StopMusicStream(*g_gameResources.currentBGM);           // 停止当前BGM
  PlayMusicStream(g_gameResources.GameBGM);               // 播放游戏内BGM
  g_gameResources.currentBGM = &g_gameResources.GameBGM;  // 切换到游戏内BGM
  TraceLog(LOG_INFO, "点击了【双人对战】按钮");
}

void OnRestartGame(void) {
  ButtonPage = 0;
  StopMusicStream(*g_gameResources.currentBGM);           // 停止当前BGM
  PlayMusicStream(g_gameResources.MenuBGM);               // 播放菜单BGM
  g_gameResources.currentBGM = &g_gameResources.MenuBGM;  // 切换到菜单BGM
  TraceLog(LOG_INFO, "点击了【重新开始】按钮");
}

void OnExitGame(void) {
  ButtonPage = -1;  // 设置为-1表示退出游戏
  TraceLog(LOG_INFO, "点击了【退出游戏】按钮");
}

void OnUndoMove(void) {
  OutPositionStack(&g_positionStack);
  if (ButtonPage == 2) {
    OutPositionStack(&g_positionStack);
  }

  TraceLog(LOG_INFO, "点击了【悔棋】按钮");
}
