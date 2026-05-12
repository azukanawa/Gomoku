#ifndef RENDERING_H
#define RENDERING_H

#include "Button.h"
#include "raylib.h"

// 结构体定义
struct window_size {
  int width;
  int height;
  int width_half;
  int height_half;
};

// 渲染相关函数声明(按钮渲染在Button)
int InitWindowSize(struct window_size* size, int width, int height);

// 全局变量声明（使用extern）
extern Color color_white;
extern Color color_black;

#endif  // RENDERING_H