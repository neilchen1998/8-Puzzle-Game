#include <utility> // std::to_underlying

#include "fmt/core.h"
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "gui/colourlib.hpp"
#include "gui/settingslib.hpp"

namespace
{
constexpr int btnFontSize = 35;
constexpr int btnHeight = 60;
constexpr int btnFont = 40;
constexpr float btnY = 350;
constexpr float btnPadding = 10;
constexpr float volumeSliderLen = 140;
const std::string_view exitBtnTxt{"Back to MENU"};
const Rectangle backgroundCheckboxTxtRec { 50, btnY, btnFont, btnFont };
const char *checkboxText = "Background music: OFF";
const char *sliderTxt = "Main volume: ";
} // namespace

Settings::Settings()
    : screenWidth_(GetScreenWidth()),
      screenHeight_(GetScreenHeight()),
      volume_(25.0f),
      exit_(false),
      exitBtnState_(gui::ButtonState::Unselected),
      exitBtnWidth_(MeasureText(exitBtnTxt.data(), btnFont)),
      exitBtnRec_(
          {0.5f * (screenWidth_ - exitBtnWidth_), 600, exitBtnWidth_ + 2 * btnPadding, btnHeight}),
      btnColours_({JADE_GREEN, DARK_GREEN, TEAL}),
      volumeLabelRec_(
          {0.5f * (screenWidth_) - 150, 0.5f * screenHeight_ - 100, 60, 24}),
      volumeSliderBarRec_({volumeLabelRec_.x, 0.5f * screenHeight_ - 100 + 24, volumeSliderLen, 16}),
      fxBackgroundEnabled_(true)
{
    // Load sound effects
    fxMove_ = LoadSound("resources/switch-menu.mp3");
    fxSelect_ = LoadSound("resources/click-menu.mp3");

    // Measure the length of the texts
    backgroundMusicTxtLen_ = MeasureText(checkboxText, btnFont);
    mainVolumeTxtLen_ = MeasureText(sliderTxt, btnFont);
}

Settings::~Settings()
{
    // Unload sound effects
    UnloadSound(fxMove_);
    UnloadSound(fxSelect_);
}

void Settings::Update()
{
    static bool exitAct = false;
    static gui::ButtonState prevState = gui::ButtonState::Unselected;

    // Check if the restart button is hovered or pressed
    const Vector2 mousePos = GetMousePosition();
    if (CheckCollisionPointRec(mousePos, exitBtnRec_))
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            exitBtnState_ = gui::ButtonState::Selected;
        }
        else
        {
            exitBtnState_ = gui::ButtonState::Hovered;
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            exitAct = true;
        }
    }
    else
    {
        exitBtnState_ = gui::ButtonState::Unselected;
    }

    if (exitBtnState_ != prevState)
    {
        if (fxBackgroundEnabled_)
        {
            PlaySound(fxSelect_);
        }
        prevState = exitBtnState_;
    }

    if (fxBackgroundEnabled_ && CheckCollisionPointRec(mousePos, volumeSliderBarRec_) &&
        (IsMouseButtonDown(MOUSE_BUTTON_LEFT) || IsMouseButtonReleased(MOUSE_BUTTON_LEFT)))
    {
        PlaySound(fxMove_);
    }

    // Check if action is valid
    if (exitAct)
    {
        if (fxBackgroundEnabled_)
        {
            PlaySound(fxSelect_);
        }
        exit_ = true;
        exitAct = false;
    }

    // Update the master volume
    SetMasterVolume(volume_);
}

void Settings::Draw()
{
    float anchorY = volumeLabelRec_.y;

    // Draw the volume slider
    GuiLabel({volumeLabelRec_.x + mainVolumeTxtLen_, volumeLabelRec_.y, volumeLabelRec_.width, volumeLabelRec_.height}, TextFormat("Volume: %i %", (int)volume_));
    GuiSliderBar({volumeSliderBarRec_.x + mainVolumeTxtLen_, volumeSliderBarRec_.y, volumeSliderBarRec_.width, volumeSliderBarRec_.height}, NULL, NULL, &volume_, 0.0f, 100.0f);
    DrawText("Main volume: ", volumeLabelRec_.x, anchorY, btnFont, BLACK);

    anchorY += 50.0f;

    // Draw the checkbox and its text description
    GuiCheckBox(Rectangle{volumeLabelRec_.x + backgroundMusicTxtLen_ + 10, anchorY, backgroundCheckboxTxtRec.width, backgroundCheckboxTxtRec.height}, NULL, &fxBackgroundEnabled_);
    DrawText(TextFormat("Background music: %s", (fxBackgroundEnabled_) ? "ON" : "OFF"), volumeLabelRec_.x, anchorY, btnFont, BLACK);

    // Draw the exit button
    DrawRectangle(exitBtnRec_.x, exitBtnRec_.y, exitBtnRec_.width, exitBtnRec_.height,
                  btnColours_[static_cast<int>(exitBtnState_)]);
    DrawText(exitBtnTxt.data(), exitBtnRec_.x + btnPadding, exitBtnRec_.y + btnPadding, btnFont,
             WHITE);
}

bool Settings::Exit()
{
    // Return the old value of exit_ and set it to false regardless
    return std::exchange(exit_, false);
}
