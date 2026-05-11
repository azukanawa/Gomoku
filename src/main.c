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
    return 0;
}