#ifndef INCLUDE_GUI_MENULIB_H_
#define INCLUDE_GUI_MENULIB_H_

#include <array>       // std::array
#include <string_view> // std::string_view
#include <utility>     // std::pair

#include "raylib.h"

class Menu
{
    /// @brief the colours of the button {selected, unselected}
    using BtnColours = std::pair<Color, Color>;

    /// @brief The button struct
    struct Btn
    {
        BtnColours colours;
        std::string_view txt;
        Rectangle rec;
        float txtLen;
    };

public:
    Menu();

    ~Menu();

    /// @brief Updates the state
    void Update();

    /// @brief Draws the animation on the screen according to the current state
    void Draw() const;

    /// @brief Get the selection from the user
    /// @return The selection from the user, INT_MAX if the user has not pressed ENTER
    int GetSelection();

private:
    /// @brief The width of the main screen
    int screenWidth_;

    /// @brief The height of the main screen
    int screenHeight_;

    std::array<Btn, 3> btns_;

    /// @brief The current selected option index
    int selectedOption_;

    /// @brief The sound effect for moving between buttons
    Sound fxMenuMove_;

    /// @brief The sound effect for selecting a button
    Sound fxMenuSelect_;

    /// @brief The action flag
    bool action_;
};

#endif // INCLUDE_GUI_MENULIB_H_
