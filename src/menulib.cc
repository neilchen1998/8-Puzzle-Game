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
    : screenWidth_(GetScreenWidth()), screenHeight_(GetScreenHeight()), selectedOption_(0)
{
    btns_[0] = {std::pair<Color, Color>{TEAL, DARK_GREEN}, "New Game"};
    btns_[1] = {std::pair<Color, Color>{CORAL, BURNT_SIENNA}, "Settings"};
    btns_[2] = {std::pair<Color, Color>{LIGHT_GOLD, DARK_SEPIACLITERAL}, "Quit"};

    const unsigned int N = btns_.size();

    const float btnX = 0.5 * (screenWidth_ - btnWidth);
    float btnY = 0.5 * (screenWidth_ - btnWidth);

    // Calculate and construct the rectangles
    for (size_t i = 0; i < N; ++i, btnY += (btnHeight + btnPadding * 2))
    {
        // Construct a button
        btns_[i].rec = Rectangle{btnX, btnY, btnWidth, btnHeight};

        btns_[i].txtLen = MeasureText(btns_[i].txt.data(), btnFontSize);
    }

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
    const unsigned int N = btns_.size();

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
    const unsigned int N = btns_.size();

    const float btnX = 0.5 * (screenWidth_ - btnWidth);
    float btnY = 0.5 * (screenWidth_ - btnWidth);

    // Draw each option and its button
    for (size_t i = 0; i < N; ++i, btnY += (btnHeight + btnPadding * 2))
    {
        // Pick the colour depends on if the button is selcted or not
        Color btnColour = (i == selectedOption_) ? btns_[i].colours.first : btns_[i].colours.second;

        // Draw the button and the text
        DrawRectangle(btns_[i].rec.x, btns_[i].rec.y, btns_[i].rec.width, btns_[i].rec.height,
                      btnColour);
        DrawText(TextFormat(btns_[i].txt.data()),
                 0.5 * (screenWidth_ - btns_[i].txtLen) - btnPadding, btnY + btnPadding,
                 btnFontSize, WHITE);
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
