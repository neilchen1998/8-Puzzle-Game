#include "raylib.h"
#include "fmt/core.h"

#include "gui/animationlib.hpp"

void RaylibAnimation::Update()
{
    switch (curState_)
    {
        case LoadingState::SMALL_BOX_BLINKING:
        {
            ++framesCounter_;
            if (framesCounter_ == 90)
            {
                curState_ = LoadingState::LEFT_BOX_GROWING;
                framesCounter_ = 0;
            }
            break;
        }
        case LoadingState::LEFT_BOX_GROWING:
        {
            topSideRecWidth_ += 4;
            leftSideRecHeight_ += 4;

            // Check the upper left box has completed
            if (topSideRecWidth_ == 256)
            {
                curState_ = LoadingState::RIGHT_BOX_GROWING;
            }
            break;
        }
        case LoadingState::RIGHT_BOX_GROWING:
        {
            bottomSideRecWidth_ += 4;
            rightSideRecHeight_ += 4;
            
            // Check the bottom right box has completed
            if (bottomSideRecWidth_ == 256)
            {
                curState_ = LoadingState::LETTER_APPEARING;
            }
            break;
        }
        case LoadingState::LETTER_APPEARING:
        {
            ++framesCounter_;

            // Increase a letter by one for every 6 frames
            if (framesCounter_ / 6)
            {
                ++lettersCount_;
                framesCounter_ = 0;
            }

            // Check if all letters have appeared
            if (lettersCount_ >= 10)
            {
                alpha_ -= 0.015f;

                // Check if the logo has faded out by looking at alpha value
                if (alpha_ <= 0.0f)
                {
                    curState_ = LoadingState::ENDING;
                }
            }
            break;
        }
        case LoadingState::ENDING:
        {
            curState_ = LoadingState::ENDING;      
            break;
        }
        default: break;
    }
}

void RaylibAnimation::Draw() const
{
    switch (curState_)
    {
        case LoadingState::SMALL_BOX_BLINKING:
        {
            if ((framesCounter_/15)%2) DrawRectangle(logoPositionX_, logoPositionY_, 16, 16, BLACK);

            break;
        }
        case LoadingState::LEFT_BOX_GROWING:
        {
            DrawRectangle(logoPositionX_, logoPositionY_, topSideRecWidth_, 16, BLACK);
            DrawRectangle(logoPositionX_, logoPositionY_, 16, leftSideRecHeight_, BLACK);
            break;
        }
        case LoadingState::RIGHT_BOX_GROWING:
        {
            DrawRectangle(logoPositionX_, logoPositionY_, topSideRecWidth_, 16, BLACK);
            DrawRectangle(logoPositionX_, logoPositionY_, 16, leftSideRecHeight_, BLACK);

            DrawRectangle(logoPositionX_ + 240, logoPositionY_, 16, rightSideRecHeight_, BLACK);
            DrawRectangle(logoPositionX_, logoPositionY_ + 240, bottomSideRecWidth_, 16, BLACK);
            break;
        }
        case LoadingState::LETTER_APPEARING:
        {
            DrawRectangle(logoPositionX_, logoPositionY_, topSideRecWidth_, 16, Fade(BLACK, alpha_));
            DrawRectangle(logoPositionX_, logoPositionY_ + 16, 16, leftSideRecHeight_ - 32, Fade(BLACK, alpha_));

            DrawRectangle(logoPositionX_ + 240, logoPositionY_ + 16, 16, rightSideRecHeight_ - 32, Fade(BLACK, alpha_));
            DrawRectangle(logoPositionX_, logoPositionY_ + 240, bottomSideRecWidth_, 16, Fade(BLACK, alpha_));

            DrawRectangle(GetScreenWidth()/2 - 112, GetScreenHeight()/2 - 112, 224, 224, Fade(RAYWHITE, alpha_));

            DrawText(TextSubtext("raylib", 0, lettersCount_), GetScreenWidth()/2 - 44, GetScreenHeight()/2 + 48, 50, Fade(BLACK, alpha_));

            // Only show the subtext when the first letter of raylib comes out
            std::string_view subTitle("made with blood, sweat, and tears");
            if (lettersCount_)
            {
                const int subTxtWidth = MeasureText(subTitle.data(), 30);
                DrawText(subTitle.data(), (GetScreenWidth() - subTxtWidth) / 2, GetScreenHeight()/2 + 150, 30, Fade(LIME, alpha_));
            }
            break;
        }
        default: break;
    }
}
