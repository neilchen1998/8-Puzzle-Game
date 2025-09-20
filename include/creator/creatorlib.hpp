#ifndef INCLUDE_CREATOR_CREATORLIB_H_
#define INCLUDE_CREATOR_CREATORLIB_H_

#include <vector>   // std::vector
#include <span>     // std::span
#include <algorithm>    // std::shuffle
#include <random>    // std::mt19937
#include <chrono>   // std::chrono

#include "slidr/constants/constantslib.hpp"   // constants::EMPTY

namespace creator
{
    bool Solvable(std::span<const int> layout)
    {
        int cnt = 0;

        for (size_t i = 0; i < constants::EIGHT_PUZZLE_NUM - 1; i++)
        {
            for (size_t j = i + 1; j < constants::EIGHT_PUZZLE_NUM; j++)
            {
                int left = layout[i];
                int right = layout[j];
                if ((left != constants::EMPTY) && (right != constants::EMPTY) && (left > right))
                {
                    ++cnt;
                }
            }
        }

        return ((cnt % 2) == 0) ? true : false;
    }

    std::vector<int> GetRandomLayout()
    {
        std::vector<int> layout {1, 2, 3, 4, 5, 6, 7, 8, constants::EMPTY};
        unsigned seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        std::mt19937 g(seed);

        for (size_t i = 0; i < 1'000'000; i++)
        {
            std::shuffle(layout.begin(), layout.end(), g);

            if (Solvable(std::span(layout)))
            {
                return layout;
            }
        }

        // Return a default one if we could not find a solvable puzzle
        return {1, 2, 7, 4, constants::EMPTY, 5, 8, 3, 6};
    }
}   // namespace creator

#endif // INCLUDE_CREATOR_CREATORLIB_H_
