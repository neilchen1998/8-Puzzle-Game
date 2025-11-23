#include "raylib.h"

#include "gui/menulib.hpp"
#include "gui/colourlib.hpp"

namespace
{
    constexpr int btnFontSize = 35;
    constexpr int buttonWidth = 250;
    constexpr int buttonHeight = 60;
    constexpr float btnY = 350;
    constexpr float btnPadding = 10;
}

Menu::Menu()
    : screenWidth_(GetScreenWidth()),
    screenHeight_(GetScreenHeight()),
    selectedOption_(0),
    btnWidth_(buttonWidth)
{
    options_ = {"New Game", "Settings", "Quit"};

    CalculateBtnWidth();
}

Menu::~Menu()
{

}

void Menu::Update()
{
    const unsigned int N = options_.size();

    if (IsKeyPressed(KEY_UP))
    {
        selectedOption_ = (--selectedOption_ + N) % N;
    }
    else if (IsKeyPressed(KEY_DOWN))
    {
        selectedOption_ = (++selectedOption_ + N) % N;
    }
}

void Menu::Draw() const
{
    const unsigned int N = options_.size();

    const float btnX = 0.5 * (screenWidth_ - btnWidth_);

    for (size_t i = 0; i < N; ++i, btnY += (buttonHeight + btnPadding * 2))
    {
        Color btnColour = (i == selectedOption_) ? CRIMSON : LIGHT_CORAL;
        Rectangle btn{btnX, btnY, btnWidth_, buttonHeight};

        DrawRectangle(btn.x, btn.y, btn.width, btn.height, btnColour);
        DrawText(TextFormat(options_[i].data()), 0.5 * (screenWidth_ - optionLens_[i]) - btnPadding, btnY + btnPadding, btnFontSize, WHITE);
    }
}

void Menu::CalculateBtnWidth()
{
    // Calculate the width of buttons based on the longest text
    for (size_t i = 0; i < options_.size(); ++i)
    {
        float cur = MeasureText(options_[i].data(), btnFontSize);
        optionLens_[i] = cur;
    }
}
