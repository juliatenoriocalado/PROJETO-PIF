#include "raylib.h"

int main(void) {
    InitWindow(800, 600, "Carnival Story");

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);
        DrawText("Raylib funcionando!", 220, 280, 30, WHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}