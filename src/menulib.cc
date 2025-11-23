#include <utility> // std::pair

#include "fmt/core.h"
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
      action_(false)
{
    // Initialize the colours and the texts
    btns_[0] = {{TEAL, DARK_GREEN}, "New Game"};
    btns_[1] = {{CORAL, BURNT_SIENNA}, "Settings"};
    btns_[2] = {{LIGHT_GOLD, DARK_SEPIACLITERAL}, "Quit"};

    const float btnX = 0.5 * (screenWidth_ - btnWidth);
    float btnY = 0.5 * (screenWidth_ - btnWidth);

    // Calculate and construct the rectangles
    for (size_t i = 0; i < btns_.size(); ++i, btnY += (btnHeight + btnPadding * 2))
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
    static unsigned int curSelection = selectedOption_;
    static bool leftClickPressedInState = false;
    static Vector2 prevMousePos = GetMousePosition();

    const Vector2 mousePos = GetMousePosition();

    // Detect key actions first then check if the mouse moves to a new position
    if (IsKeyPressed(KEY_UP))
    {
        curSelection = (selectedOption_ - 1 + N) % N;
    }
    else if (IsKeyPressed(KEY_DOWN))
    {
        curSelection = (selectedOption_ + 1 + N) % N;
    }
    else if ((mousePos.x != prevMousePos.x) || (mousePos.y != prevMousePos.y))
    {
        // Check if the cursor is over a button
        for (size_t i = 0; i < N; i++)
        {
            if (CheckCollisionPointRec(mousePos, btns_[i].rec))
            {
                curSelection = i;

                break;
            }
        }

        // Update the mouse position
        prevMousePos = mousePos;
    }

    // Check if another button is selected, if so then play the music
    if (curSelection != selectedOption_)
    {
        selectedOption_ = curSelection;

        PlaySound(fxMenuMove_);
    }

    // Check if the user click a button
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
        CheckCollisionPointRec(mousePos, btns_[selectedOption_].rec))
    {
        leftClickPressedInState = true;
    }

    // Check for user's input
    // (i) the user presses ENTER
    // (ii) the user clicks on the button
    if (IsKeyPressed(KEY_ENTER) ||
        (leftClickPressedInState && CheckCollisionPointRec(mousePos, btns_[selectedOption_].rec) &&
         IsMouseButtonReleased(MOUSE_BUTTON_LEFT)))
    {
        PlaySound(fxMenuSelect_);

        leftClickPressedInState = false;
        action_ = true;
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

int Menu::GetSelection()
{
    if (!action_)
    {
        return INT_MAX;
    }

    action_ = false;
    return selectedOption_;
}
