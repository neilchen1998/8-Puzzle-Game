#ifndef INCLUDE_GUI_ANIMATIONLIB_H_
#define INCLUDE_GUI_ANIMATIONLIB_H_

#include <string> // std::string

class RaylibAnimation
{
public:
    RaylibAnimation();

    ~RaylibAnimation() = default;

    /// @brief Updates the state
    void Update();

    /// @brief Draws the animation on the screen according to the current state
    void Draw() const;

    /// @brief Checks if the animation is finished
    /// @return TRUE if the animation is done
    inline bool IsDone() const noexcept { return (curState_ == LoadingState::ENDING); }

public:
    /// @brief The states of the game
    enum struct LoadingState : unsigned short
    {
        SMALL_BOX_BLINKING = 0,
        LEFT_BOX_GROWING,
        RIGHT_BOX_GROWING,
        LETTER_APPEARING,
        ENDING
    };

private:
    /// @brief The current state of the game
    LoadingState curState_;

    /// @brief The width of the main screen
    int screenWidth_;

    /// @brief The height of the main screen
    int screenHeight_;

    /// @brief The hight of the left side of the rectangle
    int leftSideRecHeight_;

    int topSideRecWidth_;

    /// @brief The hight of the top side of the rectangle
    int topSideRecHeight_;

    /// @brief The counter that counts the number of letters has appeared so far
    int lettersCount_;

    /// @brief The alpha (saturation) that controls the fading
    float alpha_;

    /// @brief The position x of the logo
    const int logoPositionX_;

    /// @brief The position y of the logo
    const int logoPositionY_;

    /// @brief The bottom side rectangle width
    int bottomSideRecWidth_;

    /// @brief The top right side rectangle width
    int rightSideRecHeight_;

    /// @brief The counter that counts the number of frames since animation begins
    unsigned long framesCounter_;

    /// @brief the width of Raylib
    int raylibTxtWidth_;

    /// @brief The width of the subtitle
    int subTxtWidth_;

    /// @brief The subtitle
    std::string subTitle_;
};

#endif // INCLUDE_GUI_ANIMATIONLIB_H_
