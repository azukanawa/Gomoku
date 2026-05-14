#include "function.h"

#include <limits.h>

#include "main.h"

// 五子棋四个有效连线方向：横、竖、右下、左下
static int directions[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};

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

int GetPatternScore(int piece_count, int has_skip, int left_open,
                    int right_open) {
  // 五子或以上直接赢
  if (piece_count >= 5) return SCORE_WIN;

  // ---------- 长度为 4 的棋型 ----------
  // 活四：连续4子，无跳空，两端都是空位
  if (piece_count == 4 && !has_skip && left_open && right_open)
    return SCORE_LIVE4;

  // 冲四（连四被堵一端）或 有跳空的四（例如 X X _ X X 这种跳冲四）
  if (piece_count == 4 && ((!left_open || !right_open) || has_skip))
    return SCORE_BLOCK4;

  // 冲四特例：3子 + 一个跳空，且两端开放（例如 X _ X X）
  if (piece_count == 3 && has_skip && left_open && right_open)
    return SCORE_BLOCK4;

  // ---------- 长度为 3 的棋型 ----------
  // 活三（包括跳活三）：两端都开放即可，不管有无跳空，价值相同
  if (piece_count == 3 && left_open && right_open) return SCORE_LIVE3;

  // 眠三：只有一端开放（另一端被敌方或边界堵死）
  if (piece_count == 3 && (left_open != right_open)) return SCORE_BLOCK3;

  // ---------- 长度为 2 的棋型 ----------
  // 活二（包括跳活二）：两端都开放
  if (piece_count == 2 && left_open && right_open) return SCORE_LIVE2;

  // 眠二：恰好一端开放
  if (piece_count == 2 && (left_open != right_open)) return SCORE_BLOCK2;

  // 其他情况得 0 分
  return 0;
}

int IsStartOfSegment(const ChessBoard* board, int row, int col, int dr, int dc,
                     Piece piece) {
  // 沿着反方向向前探测一格
  int prev_row = row - dr;
  int prev_col = col - dc;

  // 如果前一个位置是己方棋子，那当前点位一定不是起点
  if (prev_row >= 0 && prev_row < BOARD_SIZE && prev_col >= 0 &&
      prev_col < BOARD_SIZE) {
    if (board->board[prev_row][prev_col] == piece) {
      return 0;  // 不是起点
    }
  }

  // 如果前一个位置是空格，需要看空格前面是否还有棋子
  if (prev_row >= 0 && prev_row < BOARD_SIZE && prev_col >= 0 &&
      prev_col < BOARD_SIZE) {
    if (board->board[prev_row][prev_col] == EMPTY) {
      // 再往前看一步
      int prev2_row = prev_row - dr;
      int prev2_col = prev_col - dc;
      if (prev2_row >= 0 && prev2_row < BOARD_SIZE && prev2_col >= 0 &&
          prev2_col < BOARD_SIZE) {
        if (board->board[prev2_row][prev2_col] == piece) {
          return 0;  // 不是跳空棋型的起点
        }
      }
    }
  }

  // 前一个位置是空位、敌方或边界，那当前点就是标准的起点
  return 1;
}

int ScanSegment(const ChessBoard* board, int start_row, int start_col, int dr,
                int dc, Piece piece) {
  int piece_count = 1;  // 己方棋子总数（起点算一个）
  int has_skip = 0;     // 是否出现过跳空（一个空格后还有子）
  int right_open = 0;   // 正向端点（沿 dr,dc 方向）是否为空位
  int step = 1;         // 当前步长

  // 连续遇到的空格数（只允许1个作为跳空，2个则认为端点开放）
  int consecutive_spaces = 0;

  // 正向扫描（沿 dr, dc 方向）
  while (1) {
    int nr = start_row + dr * step;  // 下一个扫描位置的行
    int nc = start_col + dc * step;  // 下一个扫描位置的列
    // 出界：端点被边界阻挡，right_open = 0，停止
    if (nr < 0 || nr >= BOARD_SIZE || nc < 0 || nc >= BOARD_SIZE) {
      right_open = 0;
      break;
    }
    Piece p = board->board[nr][nc];
    if (p == piece) {               // 遇到己方棋子
      if (consecutive_spaces == 1)  // 之前刚好有一个空格，说明出现了跳空
        has_skip = 1;
      consecutive_spaces = 0;  // 重置连续空格计数
      piece_count++;
      step++;
    } else if (p == EMPTY) {          // 遇到空格
      if (consecutive_spaces == 0) {  // 第一个空格
        consecutive_spaces = 1;
        step++;
      } else {  // 不是第一个空格，停止，并认为右端是空位
        right_open = 1;
        break;
      }
    } else {  // 遇到敌方棋子：右端被阻挡
      right_open = 0;
      break;
    }
  }

  // 反向扫描（沿 -dr, -dc 方向）
  int left_open = 0;  // 反向端点是否为空位
  step = 1;
  consecutive_spaces = 0;
  while (1) {
    int nr = start_row - dr * step;
    int nc = start_col - dc * step;
    if (nr < 0 || nr >= BOARD_SIZE || nc < 0 || nc >= BOARD_SIZE) {
      left_open = 0;  // 边界阻挡
      break;
    }
    Piece p = board->board[nr][nc];
    if (p == piece) {
      if (consecutive_spaces == 1) has_skip = 1;
      consecutive_spaces = 0;
      piece_count++;
      step++;
    } else if (p == EMPTY) {
      if (consecutive_spaces == 0) {
        consecutive_spaces = 1;
        step++;
      } else {
        left_open = 1;  // 连续两个空格，左端开放
        break;
      }
    } else {
      left_open = 0;  // 敌方棋子阻挡
      break;
    }
  }

  return GetPatternScore(piece_count, has_skip, left_open, right_open);
}

int Evaluate(const ChessBoard* board, Piece view_player) {
  int p1_score = 0, p2_score = 0;

  for (int d = 0; d < 4; d++) {
    int dr = directions[d][0];
    int dc = directions[d][1];
    for (int row = 0; row < BOARD_SIZE; row++) {
      for (int col = 0; col < BOARD_SIZE; col++) {
        Piece piece = board->board[row][col];
        if (piece == EMPTY) continue;

        // 只对每个连续段的“起点”调用 ScanSegment
        if (IsStartOfSegment(board, row, col, dr, dc, piece)) {
          int score = ScanSegment(board, row, col, dr, dc, piece);
          if (piece == PLAYER_1)
            p1_score += score;
          else
            p2_score += score;
        }
      }
    }
  }
  return (view_player == PLAYER_1) ? p1_score - p2_score : p2_score - p1_score;
}

int AlphaBeta(ChessBoard* board, int depth, int alpha, int beta,
              Piece current_player, Piece maximizing_player) {
  if (depth == 0 || IsBoardFull(board)) {
    return Evaluate(board, maximizing_player);
  }

  Piece opponent = (current_player == PLAYER_1) ? PLAYER_2 : PLAYER_1;

  // 极大层：当前走棋方 == 最大化方
  if (current_player == maximizing_player) {
    int max_score = INT_MIN;
    for (int row = 0; row < BOARD_SIZE; row++) {
      for (int col = 0; col < BOARD_SIZE; col++) {
        if (board->board[row][col] == EMPTY) {
          board->board[row][col] = current_player;
          board->emptyCeils--;

          int score = AlphaBeta(board, depth - 1, alpha, beta, opponent,
                                maximizing_player);
          max_score = MaxInt(max_score, score);
          alpha = MaxInt(alpha, score);

          board->board[row][col] = EMPTY;
          board->emptyCeils++;

          if (alpha >= beta) goto prune_max;
        }
      }
    }
  prune_max:
    return max_score;
  }
  // 极小层：当前走棋方是对手
  else {
    int min_score = INT_MAX;
    for (int row = 0; row < BOARD_SIZE; row++) {
      for (int col = 0; col < BOARD_SIZE; col++) {
        if (board->board[row][col] == EMPTY) {
          board->board[row][col] = current_player;
          board->emptyCeils--;

          int score = AlphaBeta(board, depth - 1, alpha, beta, opponent,
                                maximizing_player);
          min_score = MinInt(min_score, score);
          beta = MinInt(beta, score);

          board->board[row][col] = EMPTY;
          board->emptyCeils++;

          if (beta <= alpha) goto prune_min;
        }
      }
    }
  prune_min:
    return min_score;
  }
}

void GetBestMove(ChessBoard* board, Piece player, int* best_row,
                 int* best_col) {
  int best_score = INT_MIN;
  *best_row = BOARD_SIZE / 2;
  *best_col = BOARD_SIZE / 2;

  Piece opponent = (player == PLAYER_1) ? PLAYER_2 : PLAYER_1;

  for (int row = 0; row < BOARD_SIZE; row++) {
    for (int col = 0; col < BOARD_SIZE; col++) {
      if (board->board[row][col] == EMPTY) {
        // 模拟当前玩家落子
        board->board[row][col] = player;
        board->emptyCeils--;

        // 下一步轮到对手，最大化方仍是 player
        int score = AlphaBeta(board, 3, INT_MIN, INT_MAX, opponent, player);

        board->board[row][col] = EMPTY;
        board->emptyCeils++;

        if (score > best_score) {
          best_score = score;
          *best_row = row;
          *best_col = col;
        }
      }
    }
  }
}