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

Piece CheckWin(const ChessBoard* board, int current_row, int current_col) {
  // 当前落子的玩家棋子类型
  Piece current_piece = board->board[current_row][current_col];

  // 五子棋四个有效连线方向：横、竖、右下、左下
  int directions[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};
  int direction_index;  // 方向循环索引

  // 遍历所有连线方向
  for (direction_index = 0; direction_index < 4; direction_index++) {
    int dir_row = directions[direction_index][0];  // 行方向增量
    int dir_col = directions[direction_index][1];  // 列方向增量
    int consecutive_count = 1;                     // 连续同色棋子数量

    // 正向延伸扫描
    int next_row = current_row + dir_row;
    int next_col = current_col + dir_col;
    while (next_row >= 0 && next_row < BOARD_SIZE && next_col >= 0 &&
           next_col < BOARD_SIZE &&
           board->board[next_row][next_col] == current_piece) {
      consecutive_count++;
      next_row += dir_row;
      next_col += dir_col;
    }

    // 反向延伸扫描
    next_row = current_row - dir_row;
    next_col = current_col - dir_col;
    while (next_row >= 0 && next_row < BOARD_SIZE && next_col >= 0 &&
           next_col < BOARD_SIZE &&
           board->board[next_row][next_col] == current_piece) {
      consecutive_count++;
      next_row -= dir_row;
      next_col -= dir_col;
    }

    // 达到五子连珠，判定胜利
    if (consecutive_count >= 5) {
      return current_piece;
    }
  }

  // 所有方向无五子连珠
  return EMPTY;
}
