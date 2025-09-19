#ifndef INCLUDE_PROMPT_PROMPTLIB_H_
#define INCLUDE_PROMPT_PROMPTLIB_H_

#include <vector>   // std::vector
#include <optional> // std::optional
#include <string_view>  // std::string_view
#include <span>  // std::span
#include <concepts> // std::integral
#include <bitset> // std::bitset

#include "constants/constantslib.hpp"   // constants::EMPTY, constants::EIGHT_PUZZLE_NUM, etc.

namespace prompt
{
    std::optional<std::vector<int>> parse_string_to_layout(const std::string_view str);

    template <std::integral T>
    bool validate_puzzle(std::span<T> layout)
    {
        std::bitset<constants::EIGHT_PUZZLE_NUM> s;
        s.reset();

        for (auto ele : layout)
        {
            if (ele != constants::EMPTY)
            {
                s.set(ele - 1);
            }
            else
            {
                s.set(constants::EIGHT_PUZZLE_NUM - 1);
            }
        }

        return s.all();
    }
};

#endif // INCLUDE_PROMPT_PROMPTLIB_H_