#include "main.h"

#include <limits.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    InitWindow(800, 600, "raylib [core] example - basic window");
    while (!WindowShouldClose())
    {
        BeginDrawing();
        // ClearBackground();
        /* code */
        EndDrawing();
    }
    CloseWindow(); // 窗口被关闭,卸载OpenGL上下文
    return 0;
}
