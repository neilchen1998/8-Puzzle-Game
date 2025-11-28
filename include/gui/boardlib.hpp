#ifndef INCLUDE_GUI_BOARDLIB_H_
#define INCLUDE_GUI_BOARDLIB_H_

#include <memory> // std::shared_ptr
#include <stack>  // std::stack

#include "raylib.h"
#include "slidr/constants/constantslib.hpp" // constants::EMPTY
#include "slidr/node/nodelib.hpp"           // Node, GetState()

#include "gui/buttonlib.hpp"

namespace gui
{
constexpr int boardWidth = 500;
constexpr int boardHeight = 500;
constexpr int borderThickness = 10;
} // namespace gui

class Board
{
public:
    Board();

    ~Board();

    /// @brief Updates the state
    void Update();

    /// @brief Draws the animation on the screen according to the current state
    void Draw() const;

    /// @brief Draws the animation on the result
    void DrawResult() const;

    /// @brief Update the solution
    void UpdateSolution();

    /// @brief Draws the animation on the screen showing the solution
    void DrawSolution() const;

    /// @brief Checks if the game is finished
    /// @return True if the game is finished
    inline bool IsFinished() noexcept { return std::exchange(isSolved_, false); }

    /// @brief Checks if the user requests for help
    /// @return True if the user requests for help
    inline bool RequestedHelp() noexcept { return std::exchange(requestedHelp_, false); }

    /// @brief Checks if the user wants to go to settings page
    /// @return True if the user wants to go to settings page
    inline bool GoSettings() noexcept { return std::exchange(goSettings_, false); }

    /// @brief Resets the board
    void Reset();

    /// @brief Restarts the board
    void Restart();

    /// @brief Enable the background music
    void EnableBackgroundMusic() const;

    /// @brief Disable the background music
    void DisableBackgroundMusic() const;

private:
    /// @brief Check which button is pressed
    /// @param mousePos The vector of the mouse cursor
    /// @return The button that is pressed
    gui::Button CheckWhichButtonIsPressed(const Vector2 &mousePos);

    /// @brief Draw the board
    void DrawBoard() const;

private:
    /// @brief The width of the main screen
    int screenWidth_;

    /// @brief The height of the main screen
    int screenHeight_;

    /// @brief the texture of the board pieces
    Texture2D numbers_;

    /// @brief the width of the board
    int boardWidth__;

    /// @brief the height of the board
    int boardHeight_;

    /// @brief the thickness of the boarder of the board
    int borderThickness_;

    /// @brief the x position of the board
    float boxX_;

    /// @brief the y position of the board
    float boxY_;

    /// @brief the width of the board“
    float buttonWidth_;

    /// @brief the height of the board
    float buttonHeight_;

    /// @brief the x position of the undo button
    float undoBtnX_;

    /// @brief the y position of the undo button
    float undoBtnY_;

    /// @brief the x position of the restart button
    float restartBtnX_;

    /// @brief the y position of the restart button
    float restartBtnY_;

    /// @brief the x position of the help button
    float helpBtnX_;

    /// @brief the y position of the help button
    float helpBtnY_;

    /// @brief the x position of the settings button
    float settingsBtnX_;

    /// @brief the y position of the settings button
    float settingsBtnY_;

    /// @brief the number of grids in the board
    int N_;

    /// @brief the width of each grid in the board
    float cellWidth_;

    /// @brief the height of each grid in the board
    float cellHeight_;

    /// @brief the width
    float w;

    /// @brief the height
    float h;

    /// @brief the width offset for the puzzle piece image
    float offsetW_;

    /// @brief the height offset for the puzzle piece image
    float offsetH_;

    std::vector<Rectangle> buttonPositions_;

    std::stack<std::shared_ptr<Node>> history_;

    /// @brief The state of restart button
    gui::ButtonState restartBtnState_;

    /// @brief The state of undo button
    gui::ButtonState undoBtnState_;

    /// @brief The state of help button
    gui::ButtonState helpBtnState_;

    /// @brief The state of settings button
    gui::ButtonState settingsBtnState_;

    /// @brief The action of the restart button
    bool restartBtnAction_;

    /// @brief The action of the undo button
    bool undoBtnAction_;

    /// @brief The action of the help button
    bool helpBtnAction_;

    /// @brief The action of the settings button
    bool settingsBtnAction_;

    /// @brief True if the puzzle is solved
    bool isSolved_;

    /// @brief True if the user requests for help
    bool requestedHelp_;

    /// @brief True if the user wants to go to settings page
    bool goSettings_;

    /// @brief The iterator that points to the solution
    std::vector<short>::const_iterator itr_;

    /// @brief The solution
    std::vector<short> solutionDir_;

    /// @brief The number of moves that the user makes to reach the end
    unsigned moves_;

    /// @brief The optimal moves for the puzzle
    unsigned optimalMoves_;

    /// @brief The sound effect for buttons
    Sound fxButton_;

    /// @brief The background music
    Music backgroundMusic_;
};

#endif // INCLUDE_GUI_BOARDLIB_H_
