#ifndef INCLUDE_GUI_MENULIB_H_
#define INCLUDE_GUI_MENULIB_H_

#include <string_view>  // std::string_view
#include <array>    // std::array

#include "raylib.h"
#include "slidr/constants/constantslib.hpp" // constants::EMPTY
#include "slidr/node/nodelib.hpp"           // Node, GetState()

#include "gui/buttonlib.hpp"

namespace
{
} // namespace

class Menu
{
public:
    Menu();

    ~Menu();

    /// @brief Updates the state
    void Update();

    /// @brief Draws the animation on the screen according to the current state
    void Draw() const;

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

    /// @brief The current selected option index
    unsigned int selectedOption_;

    /// @brief The width of the buttons
    float btnWidth_;
};

#endif // INCLUDE_GUI_MENULIB_H_
