#include <utility> // std::pair

#include "raylib.h"

#include "gui/colourlib.hpp"
#include "gui/menulib.hpp"

namespace
{
constexpr int btnFontSize = 35;
constexpr int btnWidth = 250;
constexpr int btnHeight = 60;
constexpr float btnY = 350;
constexpr float btnPadding = 10;
} // namespace

Menu::Menu()
    : screenWidth_(GetScreenWidth()),
      screenHeight_(GetScreenHeight()),
      selectedOption_(0),
      btnColours_{std::make_pair(TEAL, DARK_GREEN), std::make_pair(CORAL, BURNT_SIENNA),
                     std::make_pair(LIGHT_GOLD, DARK_SEPIACLITERAL)},
      options_{"New Game", "Settings", "Quit"}
{
    CalculateBtnWidth();

    // Load sound effects
    fxMenuMove_ = LoadSound("resources/switch-menu.mp3");
    fxMenuSelect_ = LoadSound("resources/click-menu.mp3");
}

Menu::~Menu()
{
    // Unload sound effects
    UnloadSound(fxMenuMove_);
    UnloadSound(fxMenuSelect_);
}

void Menu::Update()
{
    const unsigned int N = options_.size();

    if (IsKeyPressed(KEY_UP))
    {
        selectedOption_ = (--selectedOption_ + N) % N;

        PlaySound(fxMenuMove_);
    }
    else if (IsKeyPressed(KEY_DOWN))
    {
        selectedOption_ = (++selectedOption_ + N) % N;

        PlaySound(fxMenuMove_);
    }
}

void Menu::Draw() const
{
    const unsigned int N = options_.size();

    const float btnX = 0.5 * (screenWidth_ - btnWidth);
    float btnY = 0.5 * (screenWidth_ - btnWidth);

    // Draw each option and its button
    for (size_t i = 0; i < N; ++i, btnY += (btnHeight + btnPadding * 2))
    {
        // Pick the colour depends on if the button is selcted or not
        Color btnColour =
            (i == selectedOption_) ? btnColours_[i].first : btnColours_[i].second;

        // Construct a button
        Rectangle btn{btnX, btnY, btnWidth, btnHeight};

        // Draw the button and the text
        DrawRectangle(btn.x, btn.y, btn.width, btn.height, btnColour);
        DrawText(TextFormat(options_[i].data()), 0.5 * (screenWidth_ - optionLens_[i]) - btnPadding,
                 btnY + btnPadding, btnFontSize, WHITE);
    }
}

void Menu::CalculateBtnWidth()
{
    // Calculate the width of buttons based on the longest text
    for (size_t i = 0; i < options_.size(); ++i)
    {
        optionLens_[i] = MeasureText(options_[i].data(), btnFontSize);
    }
}

int Menu::GetSelection() const
{
    if (IsKeyPressed(KEY_ENTER))
    {
        PlaySound(fxMenuSelect_);

        return selectedOption_;
    }

    return -1;
}
