#ifndef INCLUDE_CONSTANTS_CONSTANTSLIB_H_
#define INCLUDE_CONSTANTS_CONSTANTSLIB_H_

// source: https://www.learncpp.com/cpp-tutorial/sharing-global-constants-across-multiple-files-using-inline-variables/

namespace constants
{
    inline constexpr int EMPTY { 0xF };
    inline constexpr int EIGHT_PUZZLE_SIZE { 3 };
    inline constexpr int EIGHT_PUZZLE_NUM { (EIGHT_PUZZLE_SIZE * EIGHT_PUZZLE_SIZE) };
    inline constexpr int RIGHT { 0 };
    inline constexpr int UP   { 1 };
    inline constexpr int LEFT  { 2 };
    inline constexpr int DOWN  { 3 };
} // namespace constants

#endif // INCLUDE_CONSTANTS_CONSTANTSLIB_H_
