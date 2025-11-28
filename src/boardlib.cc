#include <memory>  // std::make_unique
#include <span>    // std::span
#include <utility> // std::to_underlying
#include <vector>  // std::vector

#include "fmt/core.h"                 // fmt::format
#include "raylib.h"                   // LoadTexture, Vector2, Rectangle
#include "slidr/solver/solverlib.hpp" // slidr::Solver

#include "creator/creatorlib.hpp"
#include "gui/boardlib.hpp"
#include "gui/buttonlib.hpp"
#include "gui/colourlib.hpp"

Board::Board()
    : screenWidth_(GetScreenWidth()),
      screenHeight_(GetScreenHeight()),
      numbers_(LoadTexture("resources/numbers.png")),
      boardWidth__(gui::boardWidth),
      boardHeight_(gui::boardHeight),
      borderThickness_(gui::borderThickness),
      boxX_((screenWidth_ - boardWidth__) / 2),
      boxY_((screenHeight_ - boardHeight_) / 2),
      buttonWidth_(200),
      buttonHeight_(80),
      undoBtnX_((screenHeight_ + boardHeight_) / 2 + borderThickness_),
      undoBtnY_((screenHeight_ - boardHeight_) / 2),
      restartBtnX_((screenHeight_ + boardHeight_) / 2 + borderThickness_),
      restartBtnY_(undoBtnY_ + buttonHeight_ + borderThickness_),
      helpBtnX_((screenHeight_ + boardHeight_) / 2 + borderThickness_),
      helpBtnY_(restartBtnY_ + buttonHeight_ + borderThickness_),
      settingsBtnX_((screenHeight_ + boardHeight_) / 2 + borderThickness_),
      settingsBtnY_(helpBtnY_ + buttonHeight_ + borderThickness_),
      N_(constants::EIGHT_PUZZLE_SIZE),
      cellWidth_(boardWidth__ / N_),
      cellHeight_(boardHeight_ / N_),
      w(numbers_.width / 5.0f),
      h(numbers_.height / 2.0f),
      offsetW_(cellWidth_ / 5),
      offsetH_(cellHeight_ / 8),
      restartBtnState_(gui::ButtonState::Unselected),
      undoBtnState_(gui::ButtonState::Unselected),
      helpBtnState_(gui::ButtonState::Unselected),
      isSolved_(false),
      requestedHelp_(false),
      goSettings_(false),
      moves_(INT_MAX)
{
    buttonPositions_.resize(std::to_underlying(gui::Button::ButtonN));

    // Calculate the position of each piece of the puzzle
    for (size_t i = 0; i < constants::EIGHT_PUZZLE_NUM; i++)
    {
        float posX = boxX_ + ((i % 3) * cellWidth_);
        float posY = boxY_ + ((i / 3) * cellHeight_);
        buttonPositions_[i] = Rectangle{posX, posY, cellWidth_, cellHeight_};
    }

    buttonPositions_[std::to_underlying(gui::Button::Undo)] =
        Rectangle{undoBtnX_, undoBtnY_, buttonWidth_, buttonHeight_};
    buttonPositions_[std::to_underlying(gui::Button::Restart)] =
        Rectangle{restartBtnX_, restartBtnY_, buttonWidth_, buttonHeight_};
    buttonPositions_[std::to_underlying(gui::Button::Help)] =
        Rectangle{helpBtnX_, helpBtnY_, buttonWidth_, buttonHeight_};
    buttonPositions_[std::to_underlying(gui::Button::Settings)] =
        Rectangle{settingsBtnX_, settingsBtnY_, buttonWidth_, buttonHeight_};

    std::vector<int> initalLayout = creator::GetRandomLayout();
    std::shared_ptr<Node> startNode = std::make_shared<Node>(initalLayout);

    history_.push(startNode);

    slidr::Solver s{*history_.top()};

    s.SolvePuzzle();

    solutionDir_ = s.GetSolutionDirection();

    itr_ = solutionDir_.cbegin();

    optimalMoves_ = (solutionDir_.size() - 1);

    fxButton_ = LoadSound("resources/buttonfx.wav");

    // Initialize the background music
    backgroundMusic_ = LoadMusicStream("resources/piano-background.mp3");
    SetMusicVolume(backgroundMusic_, 0.5f);
    PlayMusicStream(backgroundMusic_);
}

Board::~Board()
{
    // Unload resources to prevent memory leaks
    UnloadTexture(numbers_);
    UnloadSound(fxButton_);
    UnloadMusicStream(backgroundMusic_);
}

void Board::Update()
{
    const Vector2 mousePos = GetMousePosition();
    gui::Button btn = CheckWhichButtonIsPressed(mousePos);

    restartBtnAction_ = false;
    undoBtnAction_ = false;
    helpBtnAction_ = false;
    settingsBtnAction_ = false;

    // Check if the restart button is hovered or pressed
    if (CheckCollisionPointRec(mousePos,
                               buttonPositions_[std::to_underlying(gui::Button::Restart)]))
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            restartBtnState_ = gui::ButtonState::Selected;
        }
        else
        {
            restartBtnState_ = gui::ButtonState::Hovered;
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            restartBtnAction_ = true;
        }
    }
    else
    {
        restartBtnState_ = gui::ButtonState::Unselected;
    }

    // Check if the undo button is hovered or pressed
    if (CheckCollisionPointRec(mousePos, buttonPositions_[std::to_underlying(gui::Button::Undo)]))
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            undoBtnState_ = gui::ButtonState::Selected;
        }
        else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            undoBtnAction_ = true;
        }
        else
        {
            undoBtnState_ = gui::ButtonState::Hovered;
        }
    }
    else
    {
        undoBtnState_ = gui::ButtonState::Unselected;
    }

    // Check if the help butoon is hovered or pressed
    if (CheckCollisionPointRec(mousePos, buttonPositions_[std::to_underlying(gui::Button::Help)]))
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            helpBtnState_ = gui::ButtonState::Selected;
        }
        else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            helpBtnAction_ = true;
        }
        else
        {
            helpBtnState_ = gui::ButtonState::Hovered;
        }
    }
    else
    {
        helpBtnState_ = gui::ButtonState::Unselected;
    }

    // Check if the settings butoon is hovered or pressed
    if (CheckCollisionPointRec(mousePos, buttonPositions_[std::to_underlying(gui::Button::Settings)]))
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            settingsBtnState_ = gui::ButtonState::Selected;
        }
        else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            settingsBtnAction_ = true;
        }
        else
        {
            settingsBtnState_ = gui::ButtonState::Hovered;
        }
    }
    else
    {
        settingsBtnState_ = gui::ButtonState::Unselected;
    }

    // Check if the button is clicked
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        switch (btn)
        {
        case gui::Button::FirstPiece:
        case gui::Button::SecondPiece:
        case gui::Button::ThirdPiece:
        case gui::Button::FourthPiece:
        case gui::Button::FifthPiece:
        case gui::Button::SixthPiece:
        case gui::Button::SeventhPiece:
        case gui::Button::EighthPiece:
        case gui::Button::NinthPiece:
        {
            const std::shared_ptr<Node> top = history_.top();

            // Get the position of the empty piece
            int posX = top->GetPosX();
            int xRow = posX / constants::EIGHT_PUZZLE_SIZE;
            int xCol = posX % constants::EIGHT_PUZZLE_SIZE;

            // Get the position of the piece that is clicked
            int btnRow = std::to_underlying(btn) / constants::EIGHT_PUZZLE_SIZE;
            int btnCol = std::to_underlying(btn) % constants::EIGHT_PUZZLE_SIZE;

            // Check if the condition for moving to the direction is satisfied
            if (((xCol + 1) == btnCol) && (xRow == btnRow))
            {
                auto [childLayout, childPosX] = top->GetNextLayout(constants::RIGHT);
                history_.push(std::make_shared<Node>(childLayout, childPosX, top->GetDepth() + 1,
                                                     top, constants::RIGHT));
            }
            else if (((xCol - 1) == btnCol) && (xRow == btnRow))
            {
                auto [childLayout, childPosX] = top->GetNextLayout(constants::LEFT);
                history_.push(std::make_shared<Node>(childLayout, childPosX, top->GetDepth() + 1,
                                                     top, constants::LEFT));
            }
            else if (((xRow + 1) == btnRow) && (xCol == btnCol))
            {
                auto [childLayout, childPosX] = top->GetNextLayout(constants::DOWN);
                history_.push(std::make_shared<Node>(childLayout, childPosX, top->GetDepth() + 1,
                                                     top, constants::DOWN));
            }
            else if (((xRow - 1) == btnRow) && (xCol == btnCol))
            {
                auto [childLayout, childPosX] = top->GetNextLayout(constants::UP);
                history_.push(std::make_shared<Node>(childLayout, childPosX, top->GetDepth() + 1,
                                                     top, constants::UP));
            }
            break;
        }
        default:
        {
            break;
        }
        }
    }

    // Check if the restart button needs to take action
    if (restartBtnAction_)
    {
        // Pop the stack until there is only one element in the stack
        while (history_.size() > 1)
        {
            history_.pop();
        }

        PlaySound(fxButton_);
    }

    // Check if the undo button needs to take action
    if (undoBtnAction_)
    {
        // Pop the stack (history) iff there are more than one element in the
        // stack
        if (history_.size() > 1)
        {
            history_.pop();
        }

        PlaySound(fxButton_);
    }

    // Check if the help button needs to take action
    if (helpBtnAction_)
    {
        requestedHelp_ = true;

        // Clear the history since the solution will take over
        while (history_.size() > 1)
        {
            history_.pop();
        }

        PlaySound(fxButton_);
    }

    // Check if the settings button needs to take action
    if (settingsBtnAction_)
    {
        goSettings_ = true;

        PlaySound(fxButton_);
    }

    // Check if the puzzle is completed
    if (history_.top()->IsSolved())
    {
        isSolved_ = true;

        // Update the stats
        moves_ = (history_.size() - 1);
    }

    // Update the background music
    UpdateMusicStream(backgroundMusic_);
}

void Board::Draw() const
{
    DrawBoard();

    // Undo button
    if (undoBtnState_ == gui::ButtonState::Selected)
    {
        DrawRectangle(undoBtnX_, undoBtnY_, buttonWidth_, buttonHeight_, TANGERINE);
    }
    else if (undoBtnState_ == gui::ButtonState::Hovered)
    {
        DrawRectangle(undoBtnX_, undoBtnY_, buttonWidth_, buttonHeight_, TIGER);
    }
    else
    {
        DrawRectangle(undoBtnX_, undoBtnY_, buttonWidth_, buttonHeight_, APRICOT);
    }
    DrawText(TextFormat("Undo"), undoBtnX_ + 15, undoBtnY_ + 15, 40, WHITE);

    // Restart button
    if (restartBtnState_ == gui::ButtonState::Selected)
    {
        DrawRectangle(restartBtnX_, restartBtnY_, buttonWidth_, buttonHeight_, CRIMSON);
    }
    else if (restartBtnState_ == gui::ButtonState::Hovered)
    {
        DrawRectangle(restartBtnX_, restartBtnY_, buttonWidth_, buttonHeight_, RED_WINE);

    }
    else
    {
        DrawRectangle(restartBtnX_, restartBtnY_, buttonWidth_, buttonHeight_, MAROON);
    }
    DrawText(TextFormat("Restart"), restartBtnX_ + 15, restartBtnY_ + 15, 40, WHITE);

    // Help button
    if (helpBtnState_ == gui::ButtonState::Selected)
    {
        DrawRectangle(helpBtnX_, helpBtnY_, buttonWidth_, buttonHeight_, DEEP_SKY_BLUE);
    }
    else if (helpBtnState_ == gui::ButtonState::Hovered)
    {
        DrawRectangle(helpBtnX_, helpBtnY_, buttonWidth_, buttonHeight_, STEEL_BLUE);
    }
    else
    {
        DrawRectangle(helpBtnX_, helpBtnY_, buttonWidth_, buttonHeight_, CAROLINE_BLUE);
    }
    DrawText(TextFormat("Help"), helpBtnX_ + 15, helpBtnY_ + 15, 40, WHITE);

    Rectangle settingsBox{settingsBtnX_, settingsBtnY_, buttonWidth_, buttonHeight_};
    if (settingsBtnState_ == gui::ButtonState::Selected)
    {
        DrawRectangle(settingsBox.x, settingsBox.y, settingsBox.width, settingsBox.height, DEEP_SKY_BLUE);
    }
    else if (settingsBtnState_ == gui::ButtonState::Hovered)
    {
        DrawRectangle(settingsBox.x, settingsBox.y, settingsBox.width, settingsBox.height, STEEL_BLUE);
    }
    else
    {
        DrawRectangle(settingsBox.x, settingsBox.y, settingsBox.width, settingsBox.height, CAROLINE_BLUE);
    }
    DrawText(TextFormat("Settings"), settingsBtnX_ + 15, settingsBtnY_ + 15, 40, WHITE);

    // Draw the number of steps (depth) on the top
    const int depth = history_.top()->GetDepth();
    if (depth < 100)
    {
        DrawText(TextFormat("Moves: %02i", depth), (screenWidth_ - boardWidth__) / 2,
                 (screenHeight_ - boardHeight_) / 2 - 40, 40, BLUE);
    }
    else
    {
        DrawText(TextFormat("Moves: %03i", depth), (screenWidth_ - boardWidth__) / 2,
                 (screenHeight_ - boardHeight_) / 2 - 40, 40, BLUE);
    }
}

void Board::DrawResult() const
{
    // Calculate the positions
    const float rectX = (screenWidth_ - gui::counterWidth) / 2;
    const float optimalMovesRectY = screenHeight_ / 2 - gui::counterHeight - 10;
    const float userMovesRectY = screenHeight_ / 2 + 10;

    // Construct the text
    std::string optimalMovesText = fmt::format("Optimal Moves: {}", optimalMoves_);
    std::string userMovesText = fmt::format("User Moves: {}", moves_);

    // Calculate the width of the text
    int textWidth = std::max(MeasureText(optimalMovesText.c_str(), gui::moveCounterFontSize),
                             MeasureText(userMovesText.c_str(), gui::moveCounterFontSize));

    // Construct and draw the rectangles
    Rectangle optimalMovesRect = {rectX, optimalMovesRectY, gui::counterWidth, gui::counterHeight};
    Rectangle userMovesRect = {rectX, userMovesRectY, gui::counterWidth, gui::counterHeight};
    DrawRectangleRounded(optimalMovesRect, gui::cornerRadius, gui::segments, LIGHTGRAY);
    DrawRectangleRounded(userMovesRect, gui::cornerRadius, gui::segments, LIGHTGRAY);

    // Draw the text
    DrawText(optimalMovesText.c_str(), rectX + (gui::counterWidth - textWidth) / 2,
             optimalMovesRectY + (gui::counterHeight - gui::moveCounterFontSize) / 2,
             gui::moveCounterFontSize, DARKBLUE);
    DrawText(userMovesText.c_str(), rectX + (gui::counterWidth - textWidth) / 2,
             userMovesRectY + (gui::counterHeight - gui::moveCounterFontSize) / 2,
             gui::moveCounterFontSize, MAROON);
}

void Board::UpdateSolution()
{
    static double prevTime = GetTime();
    double curTime = GetTime();
    if (((curTime - prevTime) > 0.8) && (itr_ != solutionDir_.cend()))
    {
        const std::shared_ptr<Node> top = history_.top();

        auto [childLayout, childPosX] = top->GetNextLayout(*itr_);
        history_.push(
            std::make_shared<Node>(childLayout, childPosX, top->GetDepth() + 1, top, *itr_));

        prevTime = curTime;

        ++itr_;
    }

    if (((curTime - prevTime) > 1.0) && (itr_ == solutionDir_.cend()))
    {
        isSolved_ = true;
    }
}

void Board::DrawSolution() const
{
    DrawBoard();

    // Draw text on the top
    // NOTE: the number of moves is always 2 digits only
    DrawText(TextFormat("Moves: %02i", history_.top()->GetDepth()),
             (screenWidth_ - boardWidth__) / 2, (screenHeight_ - boardHeight_) / 2 - 40, 40, BLUE);
}

void Board::Reset()
{
    restartBtnState_ = gui::ButtonState::Unselected;
    undoBtnState_ = gui::ButtonState::Unselected;
    helpBtnState_ = gui::ButtonState::Unselected;
    isSolved_ = false;
    requestedHelp_ = false;
    moves_ = INT_MAX;
    itr_ = solutionDir_.cbegin();

    std::vector<int> initalLayout = creator::GetRandomLayout();
    std::shared_ptr<Node> startNode = std::make_shared<Node>(initalLayout);

    std::stack<std::shared_ptr<Node>> newHistory;
    newHistory.push(startNode);
    std::swap(history_, newHistory);

    slidr::Solver s{*history_.top()};

    s.SolvePuzzle();

    optimalMoves_ = (s.GetSolutionDirection().size() - 1);
}

void Board::Restart()
{
    // Reset all members
    restartBtnState_ = gui::ButtonState::Unselected;
    undoBtnState_ = gui::ButtonState::Unselected;
    helpBtnState_ = gui::ButtonState::Unselected;
    isSolved_ = false;
    requestedHelp_ = false;
    moves_ = INT_MAX;
    itr_ = solutionDir_.cbegin();

    // Pop the stack until there is only one left
    while (history_.size() > 1)
    {
        history_.pop();
    }
}

void Board::EnableBackgroundMusic() const
{
    SetMusicVolume(backgroundMusic_, 0.5f);
}

void Board::DisableBackgroundMusic() const
{
    SetMusicVolume(backgroundMusic_, 0.0f);
}

gui::Button Board::CheckWhichButtonIsPressed(const Vector2 &mousePos)
{
    // Loop through all pieces on the board
    for (size_t i = 0; i <= std::to_underlying(gui::Button::NinthPiece); i++)
    {
        if (CheckCollisionPointRec(mousePos, buttonPositions_[i]))
        {
            return (gui::Button)i;
        }
    }

    // Not button is pressed
    return gui::Button::Invalid;
}

void Board::DrawBoard() const
{
    // Draw the board
    Rectangle box{boxX_, boxY_, (float)boardWidth__, (float)boardHeight_};
    DrawRectangleLinesEx(box, borderThickness_, DARKBLUE);

    // Draw the lines
    for (int i = 1; i < N_; i++)
    {
        // Draw horizontal lines
        float y = boxY_ + (i * cellHeight_);
        Vector2 startPos = {boxX_, y};
        Vector2 endPos = {boxX_ + boardWidth__, y};
        DrawLineEx(startPos, endPos, borderThickness_, DARKBLUE);

        // Draw vertical lines
        float x = boxX_ + (i * cellWidth_);
        startPos = {x, boxY_};
        endPos = {x, boxY_ + boardHeight_};
        DrawLineEx(startPos, endPos, borderThickness_, DARKBLUE);
    }

    // Loop through all the elements in the node and draw all the pieces
    std::span<const int> curState = history_.top()->GetState();
    for (size_t i = 0; i < curState.size(); i++)
    {
        // Only draw the number if the current piece is non-empty
        if (int num = curState[i]; num != constants::EMPTY)
        {
            // Calculate the position of the number located on the texture
            // (sprite sheet technique)
            int recX = (num - 1) % 5;
            int recY = (num - 1) / 5;
            Rectangle sourceRec = {recX * w, recY * h, w, h};

            // Calculate the position of the texture
            float posX = boxX_ + ((i % 3) * cellWidth_) + offsetW_;
            float posY = boxY_ + ((i / 3) * cellHeight_) + offsetH_;
            Vector2 position = {posX, posY};

            // Draw a fraction of the texture
            DrawTextureRec(numbers_, sourceRec, position, WHITE);
        }
    }
}
