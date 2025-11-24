#include "fmt/core.h"
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "gui/raygui.h"

#include "gui/colourlib.hpp"
#include "gui/settingslib.hpp"

namespace
{
constexpr int btnFontSize = 35;
constexpr int btnWidth = 250;
constexpr int btnHeight = 60;
constexpr float btnY = 350;
constexpr float btnPadding = 10;
} // namespace

Settings::Settings()
    : screenWidth_(GetScreenWidth()),
      screenHeight_(GetScreenHeight()),
      volume_(25.0f)
{
    // Load sound effects
    // fxMenuMove_ = LoadSound("resources/switch-menu.mp3");
    // fxMenuSelect_ = LoadSound("resources/click-menu.mp3");
}

Settings::~Settings()
{
    // Unload sound effects
    // UnloadSound(fxMenuMove_);
    // UnloadSound(fxMenuSelect_);
}

void Settings::Update()
{

}

void Settings::Draw()
{
    GuiLabel((Rectangle){ 12, 62, 140, 24 }, TextFormat("Volume: %i %", (int)volume_));
    GuiSliderBar((Rectangle){ 12, 60 + 24, 140, 16 }, NULL, NULL, &volume_, 0.0f, 100.0f);
    SetMasterVolume(volume_);
}
