#include "Rendering.h"

#include "main.h"

int InitWindowSize(struct window_size* size, int width, int height) {
  size->width = width;
  size->height = height;
  size->width_half = width / 2;
  size->height_half = height / 2;
  return 0;
}