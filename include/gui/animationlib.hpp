#ifndef INCLUDE_GUI_ANIMATIONLIB_H_
#define INCLUDE_GUI_ANIMATIONLIB_H_

#include <string>
#include "raylib.h"

/// @brief The states of the game
enum struct LoadingState : unsigned short
{
    SMALL_BOX_BLINKING = 0,
    LEFT_BOX_GROWING,
    RIGHT_BOX_GROWING,
    LETTER_APPEARING,
    ENDING
};

class RaylibAnimation
{
public:
    RaylibAnimation()
        : curState_(LoadingState::SMALL_BOX_BLINKING),
        screenWidth_(GetScreenHeight()),
        screenHeight_(GetScreenHeight()),
        logoPositionX_(screenWidth_/2 - 128),
        logoPositionY_(screenHeight_/2 - 128),
        leftSideRecHeight_(16),
        topSideRecWidth_(16),
        bottomSideRecWidth_(16),
        rightSideRecHeight_(16),
        lettersCount_(0),
        framesCounter_(0),
        alpha_(1.0f),
        author_("Neil")
    {
    }

    ~RaylibAnimation() = default;

    /// @brief Updates the state
    void Update();

    /// @brief Draws the animation on the screen according to the current state
    void Draw() const;

    /// @brief Checks if the animation is finished
    /// @return TRUE if the animation is done
    inline bool IsDone() const noexcept
    {
        return (curState_ == LoadingState::ENDING);
    }

private:
    /// @brief The current state of the game
    LoadingState curState_;

    /// @brief The width of the main screen
    int screenWidth_;

    /// @brief The height of the main screen
    int screenHeight_;

    int leftSideRecHeight_;

    int topSideRecWidth_;

    int lettersCount_;

    float alpha_;

    const int logoPositionX_;

    const int logoPositionY_;

    /// @brief 
    int bottomSideRecWidth_;

    int rightSideRecHeight_;

    unsigned long framesCounter_;

    /// @brief the name of the author
    std::string author_;
};

#endif // INCLUDE_GUI_ANIMATIONLIB_H_
