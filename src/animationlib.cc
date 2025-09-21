#include "raylib.h" // GetScreenHeight, MeasureText, DrawRectangle, etc.

#include "gui/animationlib.hpp"

namespace
{
    constexpr int padding = 10;
    constexpr int logoWidth = 112;
    constexpr int authorTxtFont = 50;
    constexpr int subtxtFont = 30;
    constexpr int recHeight = 16;
    constexpr int recWidth = 256;
}

RaylibAnimation::RaylibAnimation()
    : curState_(LoadingState::SMALL_BOX_BLINKING),
    screenWidth_(GetScreenHeight()),
    screenHeight_(GetScreenHeight()),
    logoPositionX_(screenWidth_/2 - recWidth / 2),
    logoPositionY_(screenHeight_/2 - recWidth / 2),
    leftSideRecHeight_(recHeight),
    topSideRecWidth_(recHeight),
    topSideRecHeight_(recHeight),
    bottomSideRecWidth_(recHeight),
    rightSideRecHeight_(recHeight),
    lettersCount_(0),
    framesCounter_(0),
    alpha_(1.0f),
    subTitle_ ("made by Neil with blood, sweat, and tears")
{
    subTxtWidth_ = MeasureText(subTitle_.data(), subtxtFont);
    raylibTxtWidth_ = MeasureText("raylib", authorTxtFont);
}

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
            if (topSideRecWidth_ == recWidth)
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
            if (bottomSideRecWidth_ == recWidth)
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
            if ((framesCounter_/15)%2)
            {
                DrawRectangle(logoPositionX_, logoPositionY_, topSideRecHeight_, topSideRecHeight_, BLACK);
            }

            break;
        }
        case LoadingState::LEFT_BOX_GROWING:
        {
            DrawRectangle(logoPositionX_, logoPositionY_, topSideRecWidth_, topSideRecHeight_, BLACK);
            DrawRectangle(logoPositionX_, logoPositionY_, topSideRecHeight_, leftSideRecHeight_, BLACK);
            break;
        }
        case LoadingState::RIGHT_BOX_GROWING:
        {
            DrawRectangle(logoPositionX_, logoPositionY_, topSideRecWidth_, 16, BLACK);
            DrawRectangle(logoPositionX_, logoPositionY_, topSideRecHeight_, leftSideRecHeight_, BLACK);

            DrawRectangle(logoPositionX_ + 240, logoPositionY_, topSideRecHeight_, rightSideRecHeight_, BLACK);
            DrawRectangle(logoPositionX_, logoPositionY_ + 240, bottomSideRecWidth_, 16, BLACK);
            break;
        }
        case LoadingState::LETTER_APPEARING:
        {
            DrawRectangle(logoPositionX_, logoPositionY_, topSideRecWidth_, topSideRecHeight_, Fade(BLACK, alpha_));
            DrawRectangle(logoPositionX_, logoPositionY_ + 16, topSideRecHeight_, leftSideRecHeight_ - 32, Fade(BLACK, alpha_));

            DrawRectangle(logoPositionX_ + 240, logoPositionY_ + 16, topSideRecHeight_, rightSideRecHeight_ - topSideRecHeight_ * 2, Fade(BLACK, alpha_));
            DrawRectangle(logoPositionX_, logoPositionY_ + 240, bottomSideRecWidth_, 16, Fade(BLACK, alpha_));

            DrawRectangle(GetScreenWidth()/2 - logoWidth, GetScreenHeight()/2 - logoWidth, 224, 224, Fade(RAYWHITE, alpha_));

            DrawText(TextSubtext("raylib", 0, lettersCount_), GetScreenWidth()/2 + logoWidth - raylibTxtWidth_ - padding, GetScreenHeight()/2 + 48, authorTxtFont, Fade(BLACK, alpha_));

            // Only show the subtext when the first letter of raylib comes out
            if (lettersCount_)
            {
                DrawText(subTitle_.data(), (GetScreenWidth() - subTxtWidth_) / 2, GetScreenHeight()/2 + 150, subtxtFont, Fade(GRAY, alpha_));
            }

            break;
        }
        default: break;
    }
}
