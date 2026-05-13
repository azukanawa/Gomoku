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

Piece CheckWin(const ChessBoard* board, int currentRow, int currentCol) {
  // 当前落子的玩家棋子类型
  Piece currentPiece = board->board[currentRow][currentCol];

  // 五子棋四个有效连线方向：横、竖、右下、左下
  int directions[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};
  int directionIndex;  // 方向循环索引

  // 遍历所有连线方向
  for (directionIndex = 0; directionIndex < 4; directionIndex++) {
    int dirRow = directions[directionIndex][0];  // 行方向增量
    int dirCol = directions[directionIndex][1];  // 列方向增量
    int consecutiveCount = 1;                    // 连续同色棋子数量

    // 正向延伸扫描
    int nextRow = currentRow + dirRow;
    int nextCol = currentCol + dirCol;
    while (nextRow >= 0 && nextRow < BOARD_SIZE && nextCol >= 0 &&
           nextCol < BOARD_SIZE &&
           board->board[nextRow][nextCol] == currentPiece) {
      consecutiveCount++;
      nextRow += dirRow;
      nextCol += dirCol;
    }

    // 反向延伸扫描
    nextRow = currentRow - dirRow;
    nextCol = currentCol - dirCol;
    while (nextRow >= 0 && nextRow < BOARD_SIZE && nextCol >= 0 &&
           nextCol < BOARD_SIZE &&
           board->board[nextRow][nextCol] == currentPiece) {
      consecutiveCount++;
      nextRow -= dirRow;
      nextCol -= dirCol;
    }

    // 达到五子连珠，判定胜利
    if (consecutiveCount >= 5) {
      return currentPiece;
    }
  }

  // 所有方向无五子连珠
  return EMPTY;
}