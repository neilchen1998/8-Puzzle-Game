#include <chrono>     // std::chrono::high_resolution_clock, std::chrono::duration_cast
#include <stdlib.h>   // EXIT_SUCCESS, EXIT_FAILURE
#include <vector>     // std::vector

#include "fmt/core.h"
#include "raylib.h" // InitWindow, SetTargetFPS,

#include "gui/screenlib.hpp" // ScreenManager

#define TARGET_FPS 60

int main(void)
{
    const int screenWidth = 1200;
    const int screenHeight = 1200;
    bool shouldClose = false;

    InitWindow(screenWidth, screenHeight, "8 Puzzle Game");

    // Initialize audo device
    InitAudioDevice();

    // Initialize all required variables and load all required data here!
    ScreenManager manager{};

    // Set desired framerate (frames-per-second)
    SetTargetFPS(TARGET_FPS);

    while (!WindowShouldClose() && !shouldClose) // Detect window close button, ESC key, or user's selection
    {
        // Update
        manager.Update();

        shouldClose = manager.GetWindowShouldBeClosed();

        // Draw
        BeginDrawing();

        ClearBackground(RAYWHITE);
        manager.Draw();

        EndDrawing();
    }

    // Unload all loaded data (textures, fonts, audio) here!
    manager.~ScreenManager();

    // Close audio device
    CloseAudioDevice();

    // Close window and OpenGL context
    CloseWindow();

    return EXIT_SUCCESS;
}
