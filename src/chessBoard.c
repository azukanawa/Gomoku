#include "chessBoard.h"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

#include "Rendering.h"
#include "main.h"
#include "math.h"

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

  // 1. 计算棋盘尺寸和居中坐标
  // 棋盘总尺寸 = (路数-1)*格子大小 + 2*边缘留白
  const float boardTotalSize = (boardSize - 1) * CELL_SIZE + 2 * BOARD_MARGIN;

  // 计算棋盘左上角坐标（实现居中）
  const float boardX = winSize->width_half - boardTotalSize / 2.0f;
  const float boardY = winSize->height_half - boardTotalSize / 2.0f;

  // 2. 从木纹纹理中截取背景（高级纹理绘制）
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

  // 绘制棋盘边框（增强立体感）
  DrawRectangleLinesEx(destRec, 3.0f, (Color){80, 50, 20, 255});

  // 3. 绘制棋盘网格线
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

  // 4.绘制星位（标准围棋棋盘标记）
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

/**
 * @brief 获取鼠标在棋盘上对应的落子点坐标（行和列）
 * @param boardSize 棋盘路数
 * @param winSize 窗口大小
 * @param row 输出：对应的行（0~boardSize-1）
 * @param col 输出：对应的列（0~boardSize-1）
 * @return 鼠标在棋盘上返回TRUE，否则返回FALSE
 */
Bool GetMouseChessPosition(int boardSize, const struct window_size* winSize,
                           int* row, int* col) {
  // 计算棋盘关键参数（和DrawChessBoard保持完全一致）
  const float boardTotalSize = (boardSize - 1) * CELL_SIZE + 2 * BOARD_MARGIN;
  const float boardX = winSize->width_half - boardTotalSize / 2.0f;
  const float boardY = winSize->height_half - boardTotalSize / 2.0f;
  const float gridStartX = boardX + BOARD_MARGIN;
  const float gridStartY = boardY + BOARD_MARGIN;
  const float gridEndX = boardX + boardTotalSize - BOARD_MARGIN;
  const float gridEndY = boardY + boardTotalSize - BOARD_MARGIN;

  // 获取鼠标位置
  Vector2 mousePos = GetMousePosition();

  // 检测鼠标是否在棋盘网格区域内（不包含边缘留白）
  if (mousePos.x < gridStartX || mousePos.x > gridEndX ||
      mousePos.y < gridStartY || mousePos.y > gridEndY) {
    return FALSE;
  }

  // 计算鼠标对应的落子点（四舍五入实现自动吸附到最近的交点）
  *col = roundf((mousePos.x - gridStartX) / CELL_SIZE);
  *row = roundf((mousePos.y - gridStartY) / CELL_SIZE);

  // 边界检查，防止数组越界
  if (*row < 0 || *row >= boardSize || *col < 0 || *col >= boardSize) {
    return FALSE;
  }

  return TRUE;
}

/**
 * @brief 处理鼠标落子逻辑
 * @param boardSize 棋盘路数
 * @param winSize 窗口大小
 * @return 落子成功返回TRUE，否则返回FALSE
 */
Bool HandleChessPlacement(int boardSize, const struct window_size* winSize) {
  int row, col;
  if (!GetMouseChessPosition(boardSize, winSize, &row, &col)) {
    return FALSE;
  }

  // 只有当鼠标左键松开时才落子（防止长按重复落子）
  if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
    // 检查该位置是否为空
    if (g_chessBoard.board[row][col] == EMPTY) {
      // 落子
      g_chessBoard.board[row][col] = g_currentPlayer;
      g_chessBoard.emptyCeils--;

      // 切换玩家
      g_currentPlayer = (g_currentPlayer == PLAYER_1) ? PLAYER_2 : PLAYER_1;

      TraceLog(LOG_INFO, "落子成功：行%d，列%d，玩家%d", row, col,
               g_currentPlayer == PLAYER_1 ? 2 : 1);
      PlaySound(gameResources.DownSound);  // 播放落子音效
      return TRUE;
    } else {
      TraceLog(LOG_INFO, "该位置已有棋子");
      return FALSE;
    }
  }

  return FALSE;
}

/**
 * @brief 绘制落子预览（半透明圆形）
 * @param boardSize 棋盘路数
 * @param winSize 窗口大小
 */
void DrawChessPreview(int boardSize, const struct window_size* winSize) {
  int row, col;
  if (!GetMouseChessPosition(boardSize, winSize, &row, &col)) {
    return;
  }

  // 计算棋盘关键参数
  const float boardTotalSize = (boardSize - 1) * CELL_SIZE + 2 * BOARD_MARGIN;
  const float boardX = winSize->width_half - boardTotalSize / 2.0f;
  const float boardY = winSize->height_half - boardTotalSize / 2.0f;
  const float gridStartX = boardX + BOARD_MARGIN;
  const float gridStartY = boardY + BOARD_MARGIN;

  // 计算预览圆的屏幕坐标
  float x = gridStartX + col * CELL_SIZE;
  float y = gridStartY + row * CELL_SIZE;

  // 预览圆大小（比实际棋子小一点）
  float previewRadius = (CELL_SIZE - 8) / 2.0f;

  // 预览圆透明度：鼠标按下时更不透明
  float alpha = IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? 0.8f : 0.5f;

  // 根据当前玩家选择颜色
  Color previewColor;
  if (g_currentPlayer == PLAYER_1) {
    previewColor = (Color){0, 0, 0, (unsigned char)(alpha * 255)};  // 半透明黑
  } else {
    previewColor =
        (Color){255, 255, 255, (unsigned char)(alpha * 255)};  // 半透明白
  }

  // 绘制预览圆
  DrawCircleV((Vector2){x, y}, previewRadius, previewColor);
}

/**
 * @brief 绘制棋盘上所有的棋子
 * @param res 游戏资源（如果使用纹理棋子的话）
 * @param boardSize 棋盘路数
 * @param winSize 窗口大小
 */
void DrawAllChessPieces(const GameResources* res, int boardSize,
                        const struct window_size* winSize) {
  // 计算棋盘关键参数
  const float boardTotalSize = (boardSize - 1) * CELL_SIZE + 2 * BOARD_MARGIN;
  const float boardX = winSize->width_half - boardTotalSize / 2.0f;
  const float boardY = winSize->height_half - boardTotalSize / 2.0f;
  const float gridStartX = boardX + BOARD_MARGIN;
  const float gridStartY = boardY + BOARD_MARGIN;

  // 棋子大小（比格子小4像素，两边各留2像素间隙）
  float pieceRadius = (CELL_SIZE - 4) / 2.0f;

  // 遍历棋盘，绘制所有非空棋子
  for (int row = 0; row < boardSize; row++) {
    for (int col = 0; col < boardSize; col++) {
      Piece piece = g_chessBoard.board[row][col];
      if (piece == EMPTY) {
        continue;
      }

      // 计算棋子的屏幕坐标（完美落在网格交点上）
      float x = gridStartX + col * CELL_SIZE;
      float y = gridStartY + row * CELL_SIZE;

      // 绘制棋子
      if (piece == PLAYER_1) {
        // 黑棋（带一点阴影增强立体感）
        DrawCircleV((Vector2){x + 1, y + 1}, pieceRadius,
                    (Color){30, 30, 30, 255});
        DrawCircleV((Vector2){x, y}, pieceRadius, BLACK);
      } else {
        // 白棋（带一点阴影增强立体感）
        DrawCircleV((Vector2){x + 1, y + 1}, pieceRadius,
                    (Color){200, 200, 200, 255});
        DrawCircleV((Vector2){x, y}, pieceRadius, WHITE);
      }
    }
  }
}
