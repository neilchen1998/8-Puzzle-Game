#include <stdlib.h> // EXIT_SUCCESS, EXIT_FAILURE
#include <vector>    // std::vector
#include <chrono>   // std::chrono::high_resolution_clock, std::chrono::duration_cast
#include <fmt/core.h>   // fmt::print

#include "raylib.h" // InitWindow, SetTargetFPS, 
#include "fmt/core.h"

#include "gui/screenlib.hpp"    // ScreenManager

#define TARGET_FPS 60

int main(void)
{
    const int screenWidth = 1200;
    const int screenHeight = 1200;

    InitWindow(screenWidth, screenHeight, "8 Puzzle Game");

    // Initialize all required variables and load all required data here!
    ScreenManager manager {};

    // Set desired framerate (frames-per-second)
    SetTargetFPS(TARGET_FPS);

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
       // Update
       manager.Update();

        // Draw
        BeginDrawing();

            ClearBackground(RAYWHITE);
            manager.Draw();

        EndDrawing();
    }

    // Unload all loaded data (textures, fonts, audio) here!
    manager.~ScreenManager();

    // Close window and OpenGL context
    CloseWindow();

    return EXIT_SUCCESS;
}
