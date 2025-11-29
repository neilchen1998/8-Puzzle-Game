#ifndef INCLUDE_GUI_SETTINGSLIB_H_
#define INCLUDE_GUI_SETTINGSLIB_H_

#include <array> // std::array

#include "raylib.h" // Rectangle

#include "gui/buttonlib.hpp" // ButtonState

class Settings
{
public:
    Settings();

    ~Settings();

    /// @brief Updates the state
    void Update();

    /// @brief Draws the animation on the screen according to the current state
    void Draw();

    /// @brief Checks if user wants to exit settings page
    /// @return TRUE if the user wants to exit settings page
    bool Exit();

    /// @brief Checks if the user wants the background music
    /// @return TRUE is the background music is enabled
    bool GetBackgroundMusic() const;

    /// @brief Gets the master volume set by the user
    /// @return The master volume in percentage
    float GetMasterVolume() const;

private:
    /// @brief The width of the main screen
    int screenWidth_;

    /// @brief The height of the main screen
    int screenHeight_;

    /// @brief The volume
    float volume_;

    /// @brief TRUE if user wants to exit
    bool exit_;

    /// @brief The state of the exit button
    gui::ButtonState exitBtnState_;

    /// @brief The width of the exit button
    float exitBtnWidth_;

    /// @brief The colours of the exit button
    std::array<Color, 3> btnColours_;

    /// @brief The rectangle of the exit button
    Rectangle exitBtnRec_;

    /// @brief The rectangle of the volume label
    Rectangle volumeLabelRec_;

    /// @brief The rectangle of the volume slider bar
    Rectangle volumeSliderBarRec_;

    /// @brief The rectangle of the checkbox for the background music
    Rectangle backgroundCheckboxRec_;

    /// @brief The sound effect for moving
    Sound fxMove_;

    /// @brief The sound effect for selecting
    Sound fxSelect_;

    /// @brief The background sound effect enabled
    bool fxBackgroundEnabled_;

    /// @brief The text length of the background music description
    float backgroundMusicTxtLen_;

    /// @brief The text length of the main volume description
    float mainVolumeTxtLen_;
};

#endif // INCLUDE_GUI_SETTINGSLIB_H_
