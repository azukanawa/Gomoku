#include "function.h"

#include "main.h"
int MaxInt(int a, int b) { return a > b ? a : b; }

int MinInt(int a, int b) { return a < b ? a : b; }

void InitBoard(ChessBoard* board) {
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      board->board[i][j] = EMPTY;
    }
  }
  board->emptyCeils = BOARD_SIZE * BOARD_SIZE;

  return;
}

Bool IsBoardFull(const ChessBoard* board) {
  if (board->emptyCeils == 0) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
 * @brief 判断指定位置落子后是否获胜
 * @param cb 棋盘指针
 * @param x 落子的行坐标
 * @param y 落子的列坐标
 * @return Piece 获胜玩家，EMPTY 为未获胜
 */
Piece CheckWin(const ChessBoard* cb, int x, int y) {
  // 获取当前落子的玩家
  Piece current = cb->board[x][y];
  // 当前位置为空，直接返回未获胜
  if (current == EMPTY) {
    return EMPTY;
  }

  // 五子棋 4 个连线方向：横向、纵向、右斜、左斜
  int directions[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};

  // 遍历 4 个方向，逐个检查
  for (int d = 0; d < 4; d++) {
    int dx = directions[d][0];
    int dy = directions[d][1];
    // 计数器：包含当前落子，初始为 1
    int count = 1;

    // 向正方向延伸，统计同色棋子
    int nx = x + dx;
    int ny = y + dy;
    while (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE &&
           cb->board[nx][ny] == current) {
      count++;
      nx += dx;
      ny += dy;
    }

    // 向反方向延伸，统计同色棋子
    nx = x - dx;
    ny = y - dy;
    while (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE &&
           cb->board[nx][ny] == current) {
      count++;
      nx -= dx;
      ny -= dy;
    }

    // 任意方向达到 5 子连珠，判定获胜
    if (count >= 5) {
      return current;
    }
  }

  // 所有方向均未满足五子连珠
  return EMPTY;
}