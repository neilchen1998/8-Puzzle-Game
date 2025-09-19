#ifndef INCLUDE_GUI_SCREENLIB_H_
#define INCLUDE_GUI_SCREENLIB_H_

#include <memory>   // std::unique_ptr, std::make_unique

#include "gui/animationlib.hpp"
#include "gui/boardlib.hpp"
#include "gui/celebrationlib.hpp"   // Celebration

/// @brief The states of the game
enum struct GameScreenState : int
{
    LOGO = 0,
    TITLE,
    GAMEPLAY,
    HELP,
    SAD,
    CELEBRATION,
    ENDING
};

class ScreenManager
{
public:
    ScreenManager();

    ~ScreenManager() = default;

    /// @brief Update the state
    void Update();

    /// @brief Draw the the state on the screen
    void Draw() const;

private:
    /// @brief The current state of the game
    GameScreenState curState_;

    /// @brief The width of the main screen
    int screenWidth_;

    /// @brief The height of the main screen
    int screenHeight_;

    /// @brief The pointer that points to the ray animation class
    std::unique_ptr<RaylibAnimation> raylibAnimationPtr_;

    /// @brief The pointer that points to the Board class
    std::unique_ptr<Board> boardPtr_;

    /// @brief The pointer that points to the Celebration class
    std::unique_ptr<Celebration> celebrationPtr_;

    /// @brief The state of the restart button
    bd::ButtonState restartBtnState_;

    /// @brief The state of the new game button
    bd::ButtonState newGameBtnState_;

    /// @brief The restart button box
    Rectangle restartBox_;

    /// @brief The new game button box
    Rectangle newGameBox_;

    /// @brief The action of the restart button
    bool restartBtnAction_;

    /// @brief The action of the new game button
    bool newGameBtnAction_;

    /// @brief The text width of the restart
    float restartTxtWidth_;

    /// @brief The text width of the new game
    float newGameTxtWidth_;
};

#endif // INCLUDE_GUI_SCREENLIB_H_
