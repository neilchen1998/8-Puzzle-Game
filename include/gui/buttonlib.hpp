#ifndef INCLUDE_GUI_BUTTONLIB_H_
#define INCLUDE_GUI_BUTTONLIB_H_

namespace gui
{
constexpr int counterWidth = 250;
constexpr int counterHeight = 60;
constexpr float cornerRadius = 0.3f;
constexpr int segments = 10;
constexpr int moveCounterFontSize = 25;

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
    Settings,

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

} // namespace gui

#endif // INCLUDE_GUI_BUTTONLIB_H_
