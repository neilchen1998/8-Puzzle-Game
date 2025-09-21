#include "raylib.h"

#include "gui/screenlib.hpp"
#include "gui/animationlib.hpp"
#include "gui/colourlib.hpp"
#include "gui/buttonlib.hpp"

namespace
{
    constexpr int buttonFontSize = 40;

    constexpr std::string_view titleText = "Welcome to 8 Puzzle";
    constexpr std::string_view restartTxt("Restart");
    constexpr std::string_view newGameTxt("New Game");
}

ScreenManager::ScreenManager()
        : curState_(GameScreenState::LOGO),
        screenWidth_(GetScreenWidth()),
        screenHeight_(GetScreenHeight()),
        raylibAnimationPtr_(std::make_unique<RaylibAnimation>()),
        boardPtr_(std::make_unique<Board>()),
        celebrationPtr_(std::make_unique<Celebration>())
{
    restartTxtWidth_ = MeasureText(restartTxt.data(), buttonFontSize);
    newGameTxtWidth_ = MeasureText(newGameTxt.data(), buttonFontSize);

    float buttonWidth = std::max(restartTxtWidth_, newGameTxtWidth_) + 20;

    restartBox_ = { screenWidth_ / 2 - buttonWidth - 20, static_cast<float>(screenHeight_ / 2 + 40), buttonWidth, buttonWidth * 2};
    newGameBox_ = { static_cast<float>(screenWidth_ / 2 + 20), restartBox_.y, buttonWidth, buttonWidth * 2};
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
                curState_ = GameScreenState::GAMEPLAY;
            }
            break;
        }
        case GameScreenState::GAMEPLAY:
        {
            boardPtr_->Update();

            if (boardPtr_->IsFinished())
            {
                curState_ = GameScreenState::CELEBRATION;
            }
            else if (boardPtr_->RequestedHelp())
            {
                curState_ = GameScreenState::HELP;
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
            celebrationPtr_->Update();

            // Press enter or left click to change to GAMEPLAY screen
            if (IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                curState_ = GameScreenState::ENDING;
            }
            break;
        }
        case GameScreenState::SAD:
        {
            // Press enter or left click to change to GAMEPLAY screen
            if (IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                curState_ = GameScreenState::ENDING;
            }
            break;
        }
        case GameScreenState::ENDING:
        {
            restartBtnAction_ = false;
            newGameBtnAction_ = false;

            const Vector2 mousePoint = GetMousePosition();

            if (CheckCollisionPointRec(mousePoint, restartBox_))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                {
                    restartBtnState_ = bd::ButtonState::Selected;
                }
                else
                {
                    restartBtnState_ = bd::ButtonState::Hovered;
                }

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    restartBtnAction_ = true;
                }
            }
            else
            {
                restartBtnState_ = bd::ButtonState::Unselected;
            }

            if (CheckCollisionPointRec(mousePoint, newGameBox_))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                {
                    newGameBtnState_ = bd::ButtonState::Selected;
                }
                else
                {
                    newGameBtnState_ = bd::ButtonState::Hovered;
                }

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    newGameBtnAction_ = true;
                }
            }
            else
            {
                newGameBtnState_ = bd::ButtonState::Unselected;
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
        default: break;
    }
}

void ScreenManager::Draw() const
{
    switch(curState_)
    {
        case GameScreenState::LOGO:
        {
            raylibAnimationPtr_->Draw();

            break;
        }
        case GameScreenState::TITLE:
        {
            DrawRectangle(0, 0, screenWidth_, screenHeight_, JADE_GREEN);

            int titleTextWidth = MeasureText(titleText.data(), 60);
            DrawText(titleText.data(), (GetScreenWidth() - titleTextWidth) / 2, GetScreenHeight() / 3, 60, BLACK);

            const int subTxtWidth = MeasureText("PRESS ENTER to start", 20);
            DrawText("PRESS ENTER to start", (GetScreenWidth() - subTxtWidth) / 2, 220, 20, DARKBLUE);

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

            const int subTxtWidth = MeasureText("PRESS ENTER to RETURN to TITLE SCREEN", 20);
            DrawText("PRESS ENTER to RETURN to TITLE SCREEN", (GetScreenWidth() - subTxtWidth) / 2, 220 + 50, 20, DARKBLUE);

            boardPtr_->DrawResult();
            celebrationPtr_->Draw();

            break;
        }
        case GameScreenState::SAD:
        {
            DrawRectangle(0, 0, screenWidth_, screenHeight_, RED);

            const int subTxtWidth = MeasureText("PRESS ENTER to RETURN to END SCREEN", 20);
            DrawText("PRESS ENTER to RETURN to TITLE SCREEN", (GetScreenWidth() - subTxtWidth) / 2, 220, 20, DARKBLUE);

            const int mainTxtWidth = MeasureText("PU can do it next time!", 50);
            DrawText("U can do it next time!", (GetScreenWidth() - mainTxtWidth) / 2, GetScreenHeight() / 2, 50, DARKBLUE);

            break;
        }
        case GameScreenState::ENDING:
        {
            DrawRectangle(0, 0, screenWidth_, screenHeight_, BLUE);

            const int subTxtWidth = MeasureText("PRESS ENTER to RETURN to TITLE SCREEN", 20);
            DrawText("PRESS ENTER to RETURN to TITLE SCREEN", (GetScreenWidth() - subTxtWidth) / 2, 220, 20, DARKBLUE);

            // The restart button
            DrawRectangleRounded(restartBox_, gui::cornerRadius, gui::segments, (restartBtnState_ == bd::ButtonState::Selected) ? CRIMSON : (restartBtnState_ == bd::ButtonState::Hovered) ? FIREBRICK : MAROON);
            DrawText(restartTxt.data(), restartBox_.x + (restartBox_.width - restartTxtWidth_) / 2, restartBox_.y + (restartBox_.height - buttonFontSize) / 2, buttonFontSize, WHITE);

            // The new game button
            DrawRectangleRounded(newGameBox_, gui::cornerRadius, gui::segments, (newGameBtnState_ == bd::ButtonState::Selected) ? DEEP_SKY_BLUE : (newGameBtnState_ == bd::ButtonState::Hovered) ? STEEL_BLUE : CAROLINE_BLUE);
            DrawText(newGameTxt.data(), newGameBox_.x + (newGameBox_.width - newGameTxtWidth_) / 2, newGameBox_.y + (newGameBox_.height - buttonFontSize) / 2, buttonFontSize, WHITE);

            break;
        }
        default: break;
    }
}
