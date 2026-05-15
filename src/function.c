#include "function.h"

#include <limits.h>
#include <stdlib.h>

#include "Rendering.h"

// 五子棋四个有效连线方向：横、竖、右下、左下
static int directions[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};

int MaxInt(int a, int b) { return a > b ? a : b; }

int MinInt(int a, int b) { return a < b ? a : b; }

void InitBoard(ChessBoard* board) {
  board->board = (Piece**)malloc(sizeof(Piece*) * g_boardSize);
  for (int i = 0; i < g_boardSize; i++) {
    board->board[i] = (Piece*)malloc(sizeof(Piece) * g_boardSize);
  }
  for (int i = 0; i < g_boardSize; i++) {
    for (int j = 0; j < g_boardSize; j++) {
      board->board[i][j] = EMPTY;
    }
  }

  board->emptyCeils = g_boardSize * g_boardSize;

  return;
}

void FreeBoard(ChessBoard* board) {
  for (int i = 0; i < g_boardSize; i++) {
    free(board->board[i]);
  }
  free(board->board);
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
    while (next_row >= 0 && next_row < g_boardSize && next_col >= 0 &&
           next_col < g_boardSize &&
           board->board[next_row][next_col] == current_piece) {
      consecutive_count++;
      next_row += dir_row;
      next_col += dir_col;
    }

    // 反向延伸扫描
    next_row = current_row - dir_row;
    next_col = current_col - dir_col;
    while (next_row >= 0 && next_row < g_boardSize && next_col >= 0 &&
           next_col < g_boardSize &&
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

Bool IsStartOfSegment(const ChessBoard* board, int row, int col, int dr, int dc,
                      Piece piece) {
  // 沿着反方向向前探测一格
  int prev_row = row - dr;
  int prev_col = col - dc;

  // 如果前一个位置是己方棋子，那当前点位一定不是起点
  if (prev_row >= 0 && prev_row < g_boardSize && prev_col >= 0 &&
      prev_col < g_boardSize) {
    if (board->board[prev_row][prev_col] == piece) {
      return FALSE;  // 不是起点
    }
  }

  // 如果前一个位置是空格，需要看空格前面是否还有棋子
  if (prev_row >= 0 && prev_row < g_boardSize && prev_col >= 0 &&
      prev_col < g_boardSize) {
    if (board->board[prev_row][prev_col] == EMPTY) {
      // 再往前看一步
      int prev2_row = prev_row - dr;
      int prev2_col = prev_col - dc;
      if (prev2_row >= 0 && prev2_row < g_boardSize && prev2_col >= 0 &&
          prev2_col < g_boardSize) {
        if (board->board[prev2_row][prev2_col] == piece) {
          return FALSE;  // 不是跳空棋型的起点
        }
      }
    }
  }

  // 前一个位置是空位、敌方或边界，那当前点就是标准的起点
  return TRUE;
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
    if (nr < 0 || nr >= g_boardSize || nc < 0 || nc >= g_boardSize) {
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
    if (nr < 0 || nr >= g_boardSize || nc < 0 || nc >= g_boardSize) {
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
    for (int row = 0; row < g_boardSize; row++) {
      for (int col = 0; col < g_boardSize; col++) {
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

int QuickScore(const ChessBoard* board, int row, int col, Piece player) {
  int total = 0;
  for (int d = 0; d < 4; d++) {
    int dr = directions[d][0];
    int dc = directions[d][1];
    total += ScanSegment(board, row, col, dr, dc, player);
  }
  return total;
}

int GenerateMoves(const ChessBoard* board, int* rows, int* cols) {
  int count = 0;
  for (int r = 0; r < g_boardSize; r++) {
    for (int c = 0; c < g_boardSize; c++) {
      if (board->board[r][c] != EMPTY) continue;
      Bool has_neighbor = FALSE;
      for (int dr = -1; dr <= 1 && !has_neighbor; dr++) {
        for (int dc = -1; dc <= 1 && !has_neighbor; dc++) {
          if (dr == 0 && dc == 0) continue;
          int nr = r + dr, nc = c + dc;
          if (nr >= 0 && nr < g_boardSize && nc >= 0 && nc < g_boardSize)
            if (board->board[nr][nc] != EMPTY) has_neighbor = TRUE;
        }
      }
      if (has_neighbor) {
        rows[count] = r;
        cols[count] = c;
        count++;
      }
    }
  }
  return count;
}

void SortMoves(ChessBoard* board, int* rows, int* cols, int count,
               Piece player) {
  for (int i = 0; i < count; i++) {
    int best_idx = i;
    board->board[rows[i]][cols[i]] = player;
    int best_val = QuickScore(board, rows[i], cols[i], player);
    board->board[rows[i]][cols[i]] = EMPTY;

    for (int j = i + 1; j < count; j++) {
      board->board[rows[j]][cols[j]] = player;
      int val = QuickScore(board, rows[j], cols[j], player);
      board->board[rows[j]][cols[j]] = EMPTY;
      if (val > best_val) {
        best_val = val;
        best_idx = j;
      }
    }
    if (best_idx != i) {
      int tr = rows[i], tc = cols[i];
      rows[i] = rows[best_idx];
      cols[i] = cols[best_idx];
      rows[best_idx] = tr;
      cols[best_idx] = tc;
    }
  }
}

int AlphaBeta(ChessBoard* board, int depth, int alpha, int beta,
              Piece current_player, Piece maximizing_player) {
  // 终局截断
  int cur_eval = Evaluate(board, maximizing_player);
  if (cur_eval >= SCORE_WIN) return SCORE_WIN;
  if (cur_eval <= -SCORE_WIN) return -SCORE_WIN;

  if (depth == 0 || IsBoardFull(board)) {
    return cur_eval;
  }

  // 深度1直接评估所有走法，不再递归
  if (depth == 1) {
    int best = (current_player == maximizing_player) ? INT_MIN : INT_MAX;
    int moves_rows[225], moves_cols[225];
    int n = GenerateMoves(board, moves_rows, moves_cols);
    for (int i = 0; i < n; i++) {
      int r = moves_rows[i], c = moves_cols[i];
      board->board[r][c] = current_player;
      board->emptyCeils--;
      if (CheckWin(board, r, c) == current_player) {
        board->board[r][c] = EMPTY;
        board->emptyCeils++;
        return (current_player == maximizing_player) ? SCORE_WIN : -SCORE_WIN;
      }
      int val = Evaluate(board, maximizing_player);
      board->board[r][c] = EMPTY;
      board->emptyCeils++;
      if (current_player == maximizing_player) {
        if (val > best) best = val;
      } else {
        if (val < best) best = val;
      }
    }
    return best;
  }

  // 深度 > 1：生成候选走法并排序
  int moves_rows[225], moves_cols[225];
  int move_count = GenerateMoves(board, moves_rows, moves_cols);
  if (move_count == 0) return cur_eval;

  SortMoves(board, moves_rows, moves_cols, move_count, current_player);

  Piece opponent = (current_player == PLAYER_1) ? PLAYER_2 : PLAYER_1;

  if (current_player == maximizing_player) {
    int max_score = INT_MIN;
    for (int i = 0; i < move_count; i++) {
      int r = moves_rows[i], c = moves_cols[i];
      board->board[r][c] = current_player;
      board->emptyCeils--;
      if (CheckWin(board, r, c) == current_player) {
        board->board[r][c] = EMPTY;
        board->emptyCeils++;
        return SCORE_WIN;
      }
      int score =
          AlphaBeta(board, depth - 1, alpha, beta, opponent, maximizing_player);
      max_score = MaxInt(max_score, score);
      alpha = MaxInt(alpha, score);
      board->board[r][c] = EMPTY;
      board->emptyCeils++;
      if (alpha >= beta) break;
    }
    return max_score;
  } else {
    int min_score = INT_MAX;
    for (int i = 0; i < move_count; i++) {
      int r = moves_rows[i], c = moves_cols[i];
      board->board[r][c] = current_player;
      board->emptyCeils--;
      if (CheckWin(board, r, c) == current_player) {
        board->board[r][c] = EMPTY;
        board->emptyCeils++;
        return -SCORE_WIN;
      }
      int score =
          AlphaBeta(board, depth - 1, alpha, beta, opponent, maximizing_player);
      min_score = MinInt(min_score, score);
      beta = MinInt(beta, score);
      board->board[r][c] = EMPTY;
      board->emptyCeils++;
      if (beta <= alpha) break;
    }
    return min_score;
  }
}

void GetBestMove(ChessBoard* board, Piece player, int* best_row,
                 int* best_col) {
  Piece opponent = (player == PLAYER_1) ? PLAYER_2 : PLAYER_1;

  // 第一优先级：自己一步必胜
  for (int row = 0; row < g_boardSize; row++) {
    for (int col = 0; col < g_boardSize; col++) {
      if (board->board[row][col] != EMPTY) continue;
      board->board[row][col] = player;
      if (CheckWin(board, row, col) == player) {
        board->board[row][col] = EMPTY;
        *best_row = row;
        *best_col = col;
        return;
      }
      board->board[row][col] = EMPTY;
    }
  }

  // 第二优先级：堵对手一步必胜
  for (int row = 0; row < g_boardSize; row++) {
    for (int col = 0; col < g_boardSize; col++) {
      if (board->board[row][col] != EMPTY) continue;
      board->board[row][col] = opponent;
      if (CheckWin(board, row, col) == opponent) {
        board->board[row][col] = EMPTY;
        *best_row = row;
        *best_col = col;
        return;
      }
      board->board[row][col] = EMPTY;
    }
  }

  //  第三优先级：正常搜索
  int best_score = INT_MIN;
  *best_row = g_boardSize / 2;
  *best_col = g_boardSize / 2;

  Move move_list[225];
  int move_count = 0;

  // 收集有邻居的空位
  for (int row = 0; row < g_boardSize; row++) {
    for (int col = 0; col < g_boardSize; col++) {
      if (board->board[row][col] != EMPTY) continue;

      Bool has_neighbor = FALSE;
      for (int dr = -1; dr <= 1 && !has_neighbor; dr++) {
        for (int dc = -1; dc <= 1 && !has_neighbor; dc++) {
          if (dr == 0 && dc == 0) continue;
          int r = row + dr, c = col + dc;
          if (r >= 0 && r < g_boardSize && c >= 0 && c < g_boardSize)
            if (board->board[r][c] != EMPTY) has_neighbor = TRUE;
        }
      }
      if (!has_neighbor) continue;

      board->board[row][col] = player;
      int score = Evaluate(board, player);
      board->board[row][col] = EMPTY;

      move_list[move_count].row = row;
      move_list[move_count].col = col;
      move_list[move_count].score = score;
      move_count++;
    }
  }

  // 按预评分降序排序
  for (int i = 0; i < move_count; i++) {
    for (int j = i + 1; j < move_count; j++) {
      if (move_list[j].score > move_list[i].score) {
        Move temp = move_list[i];
        move_list[i] = move_list[j];
        move_list[j] = temp;
      }
    }
  }

  // Alpha-Beta 搜索
  int max_search = move_count < 30 ? move_count : 30;
  for (int i = 0; i < max_search; i++) {
    int row = move_list[i].row;
    int col = move_list[i].col;

    board->board[row][col] = player;
    board->emptyCeils--;

    if (CheckWin(board, row, col) == player) {
      board->board[row][col] = EMPTY;
      board->emptyCeils++;
      *best_row = row;
      *best_col = col;
      return;
    }

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

void InitAll(void) {
  InitBoard(&g_chessBoard);
  InitWindowSize(&g_window_size);
  InitGameResources(&g_gameResources, &g_window_size);
}