#include <vector>   // std::vector
#include <optional> // std::optional
#include <string_view>  // std::string_view

#include "prompt/promptlib.hpp" // prompt::parse_string_to_layout
#include "constants/constantslib.hpp"   // constants::RIGHT, constants::LEFT, etc.

auto prompt::parse_string_to_layout(const std::string_view str) -> std::optional<std::vector<int>>
{
    // Check if the input size is equal to the size of the puzzle
    std::size_t N = str.length();
    if (N != constants::EIGHT_PUZZLE_NUM)
    {
        return std::nullopt;
    }

    // Construct the return vector of size 9
    std::vector<int> ret(constants::EIGHT_PUZZLE_NUM);

    // Loop through each element
    for (size_t i = 0; i < N; i++)
    {
        // Check if the value is between 1 to 8,
        // if not then it should be either one of the followings:
        // 'x', 'X', or '0'
        if (str[i] >= '0'  && str[i] < '9')
        {
            ret[i] = static_cast<int>(str[i] - '0');
        }
        else if ((str[i] == 'x') || (str[i] == 'X'))
        {
            ret[i] = constants::EMPTY;
        }
        else
        {
            return std::nullopt;
        }
    }

    return ret;
}
