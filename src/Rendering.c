#include "Rendering.h"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

#define CELL_SIZE 40      // 每个格子的像素大小（标准围棋格子约40x40）
#define BOARD_MARGIN 30   // 棋盘边缘留白（防止棋子超出边界）
#define LINE_WIDTH 2.0f   // 网格线宽度
#define LINE_COLOR BLACK  // 网格线颜色

// 函数实现
int InitWindowSize(struct window_size* size, int width, int height) {
  size->width = width;
  size->height = height;
  size->width_half = width / 2;
  size->height_half = height / 2;
  return 0;
}

void InitChineseFont(const char* fontPath, int fontSize, const char* allText,
                     Font* g_chineseFont) {
  int count = 0;
  int* codepoints = LoadCodepoints(allText, &count);
  *g_chineseFont = LoadFontEx(fontPath, fontSize, codepoints, count);
  free(codepoints);
}

Font g_chineseFont = {0};       // 初始化为空
Texture2D ChessBoard_13 = {0};  // 初始化为空

int InitRendering() {
  struct window_size window_size;
  SetTargetFPS(60);  // 设置帧率为60帧每秒
  InitWindowSize(&window_size, 800, 600);
  // 初始化窗口,加载材质
  InitWindow(window_size.width, window_size.height, "五子棋游戏test");
  // 加载字体图片音乐音效资源
  Font g_chineseFont;
  InitChineseFont("../res/Font/微软雅黑.ttf", 20, "开始游戏重新退出悔棋",
                  &g_chineseFont);
  Texture ChessBoard_13 = LoadTexture("../res/Picture/chess_board_13.jpg");
  InitAllGameButtons();  // 初始化所有按钮
  return 0;
}

int InitGameResources(GameResources* res, int windowWidth, int windowHeight,
                      const char* title) {
  // 先初始化窗口
  SetTargetFPS(60);
  InitWindow(windowWidth, windowHeight, title);

  // 初始化所有资源
  InitChineseFont("../res/Font/微软雅黑.ttf", 20, "开始游戏重新退出悔棋",
                  &res->chineseFont);

  res->chessBoard13 = LoadTexture("../res/Picture/chess_board_13.jpg");
  res->wood1 = LoadTexture("../res/Picture/wood1.jpg");
  if (res->chessBoard13.id == 0) {
    TraceLog(LOG_ERROR, "棋盘纹理加载失败");
    return -1;
  }

  // 初始化其他资源
  // res->blackChess = LoadTexture(...);
  // res->whiteChess = LoadTexture(...);

  InitAllGameButtons();  // 按钮初始化

  return 0;
}

void UnloadGameResources(GameResources* res) {
  if (res->chessBoard13.id != 0) UnloadTexture(res->chessBoard13);
  // 卸载其他资源
  CloseWindow();
}

/**
 * @brief 绘制居中的围棋棋盘
 * @param res 游戏资源指针（包含木纹纹理）
 * @param boardSize 棋盘路数（如13表示13x13棋盘）
 * @param winSize 窗口大小结构体
 * @return 成功返回0，失败返回-1
 */
int DrawChessBoard(const GameResources* res, int boardSize,
                   const struct window_size* winSize) {
  // 参数合法性检查
  if (res == NULL || winSize == NULL) {
    TraceLog(LOG_ERROR, "DrawChessBoard: 空指针参数");
    return -1;
  }
  if (boardSize < 9 || boardSize > 19) {
    TraceLog(LOG_ERROR, "DrawChessBoard: 棋盘路数必须在9-19之间");
    return -1;
  }
  if (res->wood1.id == 0) {
    TraceLog(LOG_ERROR, "DrawChessBoard: 木纹纹理未加载");
    return -1;
  }

  // ==========================
  // 1. 计算棋盘尺寸和居中坐标
  // ==========================
  // 棋盘总尺寸 = (路数-1)*格子大小 + 2*边缘留白
  const float boardTotalSize = (boardSize - 1) * CELL_SIZE + 2 * BOARD_MARGIN;

  // 计算棋盘左上角坐标（实现居中）
  const float boardX = winSize->width_half - boardTotalSize / 2.0f;
  const float boardY = winSize->height_half - boardTotalSize / 2.0f;

  // ==========================
  // 2. 从木纹纹理中截取背景（高级纹理绘制）
  // ==========================
  // 定义要截取的纹理区域（从木纹纹理左上角开始，截取boardTotalSize大小的正方形）
  Rectangle sourceRec = {
      0.0f,            // 纹理X起点
      0.0f,            // 纹理Y起点
      boardTotalSize,  // 截取宽度
      boardTotalSize   // 截取高度
  };

  // 定义绘制到屏幕的目标区域
  Rectangle destRec = {boardX, boardY, boardTotalSize, boardTotalSize};

  // 使用DrawTexturePro绘制（支持截取、缩放、旋转，比DrawTextureRec更强大）
  DrawTexturePro(res->wood1,       // 源纹理
                 sourceRec,        // 要截取的纹理区域
                 destRec,          // 屏幕上的目标区域
                 (Vector2){0, 0},  // 旋转原点（左上角）
                 0.0f,             // 旋转角度（0度不旋转）
                 WHITE             // 色调（白色不改变纹理颜色）
  );

  // 可选：绘制棋盘边框（增强立体感）
  DrawRectangleLinesEx(destRec, 3.0f, (Color){80, 50, 20, 255});

  // ==========================
  // 3. 绘制棋盘网格线
  // ==========================
  // 网格线的起始坐标（扣除边缘留白）
  const float gridStartX = boardX + BOARD_MARGIN;
  const float gridStartY = boardY + BOARD_MARGIN;
  // 网格线的结束坐标
  const float gridEndX = boardX + boardTotalSize - BOARD_MARGIN;
  const float gridEndY = boardY + boardTotalSize - BOARD_MARGIN;

  // 绘制横线（共boardSize条）
  for (int i = 0; i < boardSize; i++) {
    float y = gridStartY + i * CELL_SIZE;
    DrawLineEx((Vector2){gridStartX, y}, (Vector2){gridEndX, y}, LINE_WIDTH,
               LINE_COLOR);
  }

  // 绘制竖线（共boardSize条）
  for (int i = 0; i < boardSize; i++) {
    float x = gridStartX + i * CELL_SIZE;
    DrawLineEx((Vector2){x, gridStartY}, (Vector2){x, gridEndY}, LINE_WIDTH,
               LINE_COLOR);
  }

  // ==========================
  // 4. 可选：绘制星位（标准围棋棋盘标记）
  // ==========================
  const float starRadius = 3.0f;
  // 13路棋盘星位坐标（相对于网格起点的格子索引）
  int starPositions[][2] = {{3, 3}, {3, 9}, {6, 6}, {9, 3}, {9, 9}};
  int starCount = sizeof(starPositions) / sizeof(starPositions[0]);

  // 只在13路棋盘绘制星位（可扩展到19路）
  if (boardSize == 13) {
    for (int i = 0; i < starCount; i++) {
      float x = gridStartX + starPositions[i][0] * CELL_SIZE;
      float y = gridStartY + starPositions[i][1] * CELL_SIZE;
      DrawCircleV((Vector2){x, y}, starRadius, LINE_COLOR);
    }
  }

  return 0;
}
// 全局变量定义（只在这里定义一次）
Color color_white = {255, 255, 255, 255};
Color color_black = {0, 0, 0, 255};