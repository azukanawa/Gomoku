#include <limits.h>
#include <stdlib.h>
#define BOARD_SIZE 15

typedef enum {
  EMPTY,
  PLAYER_1,
  PLAYER_2,
} Piece;

typedef struct {
  Piece board[BOARD_SIZE][BOARD_SIZE];
  int emptyCeils;
} ChessBoard;

int main() { return 0; }