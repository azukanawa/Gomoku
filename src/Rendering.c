#include "Rendering.h"

// 函数实现
int InitWindowSize(struct window_size* size, int width, int height) {
  size->width = width;
  size->height = height;
  size->width_half = width / 2;
  size->height_half = height / 2;
  return 0;
}

// 全局变量定义（只在这里定义一次）
Color color_white = {255, 255, 255, 255};
Color color_black = {0, 0, 0, 255};