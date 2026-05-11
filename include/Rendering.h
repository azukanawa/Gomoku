#ifndef RENDING_H
#define RENDING_H
#include "main.h"
#include "raylib.h"

struct window_size {
  int width;
  int height;
  int width_half;
  int height_half;
};

int InitWindowSize(struct window_size* size, int width, int height);

Color color_white = {255, 255, 255, 255};
Color color_black = {0, 0, 0, 255};
#endif  // RENDING_H