#ifndef INCLUDE_GUI_SETTINGSLIB_H_
#define INCLUDE_GUI_SETTINGSLIB_H_

// #include <array>       // std::array
// #include <string_view> // std::string_view

// #include "raylib.h"

class Settings
{
public:
    Settings();

    ~Settings();

    /// @brief Updates the state
    void Update();

    /// @brief Draws the animation on the screen according to the current state
    void Draw();

private:
    /// @brief The width of the main screen
    int screenWidth_;

    /// @brief The height of the main screen
    int screenHeight_;

    /// @brief The volume
    float volume_;
};

#endif // INCLUDE_GUI_SETTINGSLIB_H_
