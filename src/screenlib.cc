#include <memory> // std::make_unique

#include "fmt/core.h"
#include "raylib.h"

#include "gui/animationlib.hpp"
#include "gui/buttonlib.hpp"
#include "gui/colourlib.hpp"
#include "gui/menulib.hpp" // Menu
#include "gui/screenlib.hpp"
#include "gui/settingslib.hpp"

namespace
{
constexpr int buttonFontSize = 40;
constexpr int buttonPadding = 20;
constexpr std::string_view greetingTitle{"Welcome to 8 Puzzle"};
constexpr std::string_view pepTalkTxt{"U can do it next time!"};
constexpr std::string_view celebrationInstrTxt{"Press ENTER or CLICK to skip"};
constexpr std::string_view titleInstrTxt{"Press ENTER to start"};
constexpr std::string_view menuInstrTxt{"Press ARROW UP or ARROW DOWN to select"};
constexpr std::string_view settingsInstrTxt{"Click and change the settings"};
constexpr std::string_view endingInstrTxt{"Select RESTART or NEW GAME"};
constexpr std::string_view sadInstrTxt{"Press ENTER to skip"};
constexpr std::string_view restartTxt{"RESTART"};
constexpr std::string_view newGameTxt{"NEW GAME"};
} // namespace

ScreenManager::ScreenManager()
    : curState_(GameScreenState::LOGO),
      screenWidth_(GetScreenWidth()),
      screenHeight_(GetScreenHeight()),
      raylibAnimationPtr_(std::make_unique<RaylibAnimation>()),
      menuPtr_(std::make_unique<Menu>()),
      settingsPtr_(std::make_unique<Settings>()),
      boardPtr_(std::make_unique<Board>()),
      celebrationPtr_(std::make_unique<Celebration>()),
      close_(false)
{
    restartTxtWidth_ = MeasureText(restartTxt.data(), buttonFontSize);
    newGameTxtWidth_ = MeasureText(newGameTxt.data(), buttonFontSize);

    const float buttonWidth = std::max(restartTxtWidth_, newGameTxtWidth_) + buttonPadding;

    const float buttonHeight = 1.6 * buttonWidth;

    restartBox_ = {screenWidth_ / 2 - buttonWidth - buttonPadding,
                   0.5f * (screenHeight_ - buttonHeight), buttonWidth, buttonHeight};
    newGameBox_ = {0.5f * screenWidth_ + buttonPadding, restartBox_.y, buttonWidth, buttonHeight};
}

ScreenManager::~ScreenManager()
{
}

void ScreenManager::Update()
{
    switch (curState_)
    {
    case GameScreenState::LOGO:
    {
        raylibAnimationPtr_->Update();

        // Wait for the intro before jumping to TITLE screen
        if (raylibAnimationPtr_->IsDone())
        {
            curState_ = GameScreenState::TITLE;
        }
        break;
    }
    case GameScreenState::TITLE:
    {
        // Press enter or left click to change to GAMEPLAY screen
        if (IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            curState_ = GameScreenState::MENU;
        }
        break;
    }
    case GameScreenState::MENU:
    {
        menuPtr_->Update();

        int selection = menuPtr_->GetSelection();

        if (selection == 0)
        {
            curState_ = GameScreenState::GAMEPLAY;
        }
        else if (selection == 1)
        {
            curState_ = GameScreenState::SETTINGS;
        }
        else if (selection == 2)
        {
            close_ = true;
        }

        break;
    }
    case GameScreenState::SETTINGS:
    {
        settingsPtr_->Update();

        bool exitToMenu = settingsPtr_->Exit();

        if (exitToMenu)
        {
            curState_ = GameScreenState::MENU;
        }

        break;
    }
    case GameScreenState::GAMEPLAY:
    {
        SetBackgroundMusic();
        boardPtr_->Update();

        if (boardPtr_->IsFinished())
        {
            curState_ = GameScreenState::CELEBRATION;
        }
        else if (boardPtr_->RequestedHelp())
        {
            curState_ = GameScreenState::HELP;
        }
        else if (boardPtr_->GoSettings())
        {
            curState_ = GameScreenState::SETTINGS;
        }
        break;
    }
    case GameScreenState::HELP:
    {
        boardPtr_->UpdateSolution();

        if (boardPtr_->IsFinished())
        {
            curState_ = GameScreenState::SAD;
        }
        break;
    }
    case GameScreenState::CELEBRATION:
    {
        celebrationPtr_->PlayApplauseSound();
        celebrationPtr_->Update();

        static bool leftClickPressedInState = false;

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            leftClickPressedInState = true;
        }

        // Press ENTER or left click to change to ENDING screen
        if (IsKeyPressed(KEY_ENTER) ||
            (leftClickPressedInState && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)))
        {
            curState_ = GameScreenState::ENDING;

            celebrationPtr_->StopApplauseSound();

            leftClickPressedInState = false;
        }
        break;
    }
    case GameScreenState::SAD:
    {
        static bool leftClickPressedInState = false;

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            leftClickPressedInState = true;
        }

        // Press ENTER or left click to change to ENDING screen
        if (IsKeyPressed(KEY_ENTER) ||
            (leftClickPressedInState && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)))
        {
            curState_ = GameScreenState::ENDING;

            leftClickPressedInState = false;
        }
        break;
    }
    case GameScreenState::ENDING:
    {
        restartBtnAction_ = false;
        newGameBtnAction_ = false;

        const Vector2 mousePos = GetMousePosition();

        if (CheckCollisionPointRec(mousePos, restartBox_))
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                restartBtnState_ = gui::ButtonState::Selected;
            }
            else
            {
                restartBtnState_ = gui::ButtonState::Hovered;
            }

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                restartBtnAction_ = true;
            }
        }
        else
        {
            restartBtnState_ = gui::ButtonState::Unselected;
        }

        if (CheckCollisionPointRec(mousePos, newGameBox_))
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                newGameBtnState_ = gui::ButtonState::Selected;
            }
            else
            {
                newGameBtnState_ = gui::ButtonState::Hovered;
            }

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                newGameBtnAction_ = true;
            }
        }
        else
        {
            newGameBtnState_ = gui::ButtonState::Unselected;
        }

        // Check if the undo button needs to take action
        if (restartBtnAction_)
        {
            curState_ = GameScreenState::GAMEPLAY;
            boardPtr_->Restart();
        }

        // Check if the help button needs to take action
        if (newGameBtnAction_)
        {
            curState_ = GameScreenState::GAMEPLAY;
            boardPtr_->Reset();
        }

        break;
    }
    default:
    {
        break;
    }
    }
}

void ScreenManager::Draw() const
{
    switch (curState_)
    {
    case GameScreenState::LOGO:
    {
        raylibAnimationPtr_->Draw();

        break;
    }
    case GameScreenState::TITLE:
    {
        DrawRectangle(0, 0, screenWidth_, screenHeight_, JADE_GREEN);

        int titleTextWidth = MeasureText(greetingTitle.data(), 60);
        DrawText(greetingTitle.data(), (GetScreenWidth() - titleTextWidth) / 2,
                 GetScreenHeight() / 3, 60, BLACK);

        const int subTxtWidth = MeasureText(titleInstrTxt.data(), 20);
        DrawText(titleInstrTxt.data(), (GetScreenWidth() - subTxtWidth) / 2, 220, 20, DARKBLUE);

        break;
    }
    case GameScreenState::MENU:
    {
        menuPtr_->Draw();

        const int subTxtWidth = MeasureText(menuInstrTxt.data(), 20);
        DrawText(menuInstrTxt.data(), (GetScreenWidth() - subTxtWidth) / 2, 220, 20, DARKBLUE);

        break;
    }
    case GameScreenState::SETTINGS:
    {
        settingsPtr_->Draw();

        const int subTxtWidth = MeasureText(settingsInstrTxt.data(), 20);
        DrawText(settingsInstrTxt.data(), (GetScreenWidth() - subTxtWidth) / 2, 220, 20, DARKBLUE);

        break;
    }
    case GameScreenState::GAMEPLAY:
    {
        DrawRectangle(0, 0, screenWidth_, screenHeight_, BEIGE);

        boardPtr_->Draw();

        break;
    }
    case GameScreenState::HELP:
    {
        DrawRectangle(0, 0, screenWidth_, screenHeight_, RED);

        boardPtr_->DrawSolution();

        break;
    }
    case GameScreenState::CELEBRATION:
    {
        DrawRectangle(0, 0, screenWidth_, screenHeight_, BEIGE);

        const int subTxtWidth = MeasureText(celebrationInstrTxt.data(), 20);
        DrawText(celebrationInstrTxt.data(), (GetScreenWidth() - subTxtWidth) / 2, 220 + 50, 20,
                 DARKBLUE);

        boardPtr_->DrawResult();
        celebrationPtr_->Draw();

        break;
    }
    case GameScreenState::SAD:
    {
        DrawRectangle(0, 0, screenWidth_, screenHeight_, RED);

        const int subTxtWidth = MeasureText(sadInstrTxt.data(), 20);
        DrawText(sadInstrTxt.data(), (GetScreenWidth() - subTxtWidth) / 2, 220, 20, DARKBLUE);

        const int mainTxtWidth = MeasureText(pepTalkTxt.data(), 50);
        DrawText(pepTalkTxt.data(), (GetScreenWidth() - mainTxtWidth) / 2, GetScreenHeight() / 2,
                 50, DARKBLUE);

        break;
    }
    case GameScreenState::ENDING:
    {
        DrawRectangle(0, 0, screenWidth_, screenHeight_, BLUE);

        const int subTxtWidth = MeasureText(endingInstrTxt.data(), 20);
        DrawText(endingInstrTxt.data(), (GetScreenWidth() - subTxtWidth) / 2, 220, 20, DARKBLUE);

        // The restart button
        DrawRectangleRounded(restartBox_, gui::cornerRadius, gui::segments,
                             (restartBtnState_ == gui::ButtonState::Selected)  ? CRIMSON
                             : (restartBtnState_ == gui::ButtonState::Hovered) ? FIREBRICK
                                                                               : MAROON);
        DrawText(restartTxt.data(), restartBox_.x + (restartBox_.width - restartTxtWidth_) / 2,
                 restartBox_.y + (restartBox_.height - buttonFontSize) / 2, buttonFontSize, WHITE);

        // The new game button
        DrawRectangleRounded(newGameBox_, gui::cornerRadius, gui::segments,
                             (newGameBtnState_ == gui::ButtonState::Selected)  ? DEEP_SKY_BLUE
                             : (newGameBtnState_ == gui::ButtonState::Hovered) ? STEEL_BLUE
                                                                               : CAROLINE_BLUE);
        DrawText(newGameTxt.data(), newGameBox_.x + (newGameBox_.width - newGameTxtWidth_) / 2,
                 newGameBox_.y + (newGameBox_.height - buttonFontSize) / 2, buttonFontSize, WHITE);

        break;
    }
    default:
    {
        break;
    }
    }
}

void ScreenManager::SetBackgroundMusic()
{
    bool enabled = settingsPtr_->GetBackgroundMusic();
    if (enabled)
    {
        boardPtr_->EnableBackgroundMusic();
    }
    else
    {
        boardPtr_->DisableBackgroundMusic();
    }
}
