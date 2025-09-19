#ifndef INCLUDE_GUI_BOARDLIB_H_
#define INCLUDE_GUI_BOARDLIB_H_

#include <memory>    // std::unique_ptr
#include <stack>    // std::stack

#include "raylib.h"

#include "constants/constantslib.hpp"   // constants::EMPTY
#include "node/nodelib.hpp" // Node, GetState()

namespace bd
{
    enum class Button
    {
        // Puzzle pieces
        FirstPiece = 0,
        SecondPiece,
        ThirdPiece,
        FourthPiece,
        FifthPiece,
        SixthPiece,
        SeventhPiece,
        EighthPiece,
        NinthPiece,

        // Buttons
        NewGame,
        Restart,
        Undo,
        Help,

        Invalid,

        // The size of the enum
        ButtonN
    };

    enum class ButtonState
    {
        Unselected,
        Hovered,
        Selected
    };
}   // namespace bd

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
    inline bool IsFinished() noexcept
    {
        return isSolved_;
    }

    /// @brief Checks if the game is finished
    /// @return True if the game is finished
    inline bool RequestedHelp() noexcept
    {
        return requestedHelp_;
    }

    /// @brief Resets the board
    void Reset();

    /// @brief Restarts the board
    void Restart();

private:
    /// @brief Check which button is pressed
    /// @param mousePoint The vector of the mouse cursor
    /// @return The button that is pressed
    bd::Button CheckWhichButtonIsPressed(const Vector2& mousePoint);

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
    bd::ButtonState restartBtnState_;

    /// @brief The state of undo button
    bd::ButtonState undoBtnState_;

    /// @brief The state of help button
    bd::ButtonState helpBtnState_;

    /// @brief The action of the restart button
    bool restartBtnAction_;

    /// @brief The action of the undo button
    bool undoBtnAction_;

    /// @brief The action of the help button
    bool helpBtnAction_;

    /// @brief True if the puzzle is solved
    bool isSolved_;

    /// @brief True if the user requests for help
    bool requestedHelp_;

    /// @brief The iterator that points to the solution
    std::vector<short>::const_iterator itr_;

    /// @brief The solution
    std::vector<short> solutionDir_;

    /// @brief The number of moves that the user makes to reach the end
    unsigned moves_;

    /// @brief The optimal moves for the puzzle
    unsigned optimalMoves_;

    /// @brief The sound effect for buttons
    Sound fxButton;
};

#endif // INCLUDE_GUI_BOARDLIB_H_
