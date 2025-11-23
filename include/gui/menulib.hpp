#ifndef INCLUDE_GUI_MENULIB_H_
#define INCLUDE_GUI_MENULIB_H_

#include <array>       // std::array
#include <string_view> // std::string_view
#include <utility>     // std::pair

#include "raylib.h"

#include "gui/buttonlib.hpp"

class Menu
{
    /// @brief the colours of the button {selected, unselected}
    using BtnColours = std::pair<Color, Color>;

public:
    Menu();

    ~Menu();

    /// @brief Updates the state
    void Update();

    /// @brief Draws the animation on the screen according to the current state
    void Draw() const;

    /// @brief
    /// @return The selected option, -1 if the user has not pressed ENTER
    int GetSelection() const;

private:
    /// @brief Calculates the width of buttons based on the longest text
    void CalculateBtnWidth();

private:
    /// @brief The width of the main screen
    int screenWidth_;

    /// @brief The height of the main screen
    int screenHeight_;

    /// @brief The options in the menu
    std::array<std::string_view, 3> options_;

    /// @brief The lengths of the options
    std::array<float, 3> optionLens_;

    // @brief The colours of the buttons
    std::array<BtnColours, 3> btnColours_;

    /// @brief The current selected option index
    unsigned int selectedOption_;

    /// @brief The sound effect for moving between buttons
    Sound fxMenuMove_;

    /// @brief The sound effect for selecting a button
    Sound fxMenuSelect_;
};

#endif // INCLUDE_GUI_MENULIB_H_
