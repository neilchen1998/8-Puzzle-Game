#include <cstddef>
#include <vector>   // std::vector
#include <fstream>  // std::ofstream
#include <algorithm>    // std::generate

#include "nanobench.h"  // ankerl::nanobench::Bench

#include "raylib.h"
#include "gui/colourlib.hpp"
#include "slidr/math/mathlib.hpp"

#include "raylib.h"

struct Confetti
{
    /// @brief The x and y position of the confetti
    Vector2 position;

    /// @brief The x and y component of the velocity of the confetti
    Vector2 velocity;

    /// @brief The physical size in pixel of the confetti
    Vector2 size;

    /// @brief The orientation of the confetti
    float orientation;

    /// @brief The angular velocity of the confetti
    float omega;

    /// @brief The colour of the confetti
    Color colour;

    /// @brief True if the confetti is on the screen
    bool active;
};

static constexpr std::array<Color, 6> CONFETTI_COLOURS { LIGHT_CORAL, APRICOT, LEMON, MINT, SKY_BLUE, LAVENDER };

static constexpr int NUM_OF_CONFETTI = 2'000;

class Dummy
{
public:
    Dummy() : confetti_(NUM_OF_CONFETTI)
    {

    }

    void ConstructorA()
    {
        auto itr = confetti_.begin();
        while (itr != confetti_.end())
        {
            itr->active = true;
            itr->position = (Vector2){ (float)GetRandomValue(0, GetScreenWidth()), (float)GetRandomValue(-GetScreenHeight() * 0.25, 0) };
            itr->velocity = (Vector2){ GetNormalFloatDist(0, 100), GetNormalFloatDist(0, 100) };
            itr->size = (Vector2){ (float)GetRandomValue(5, 12), (float)GetRandomValue(8, 20) };
            itr->orientation = (float)GetRandomValue(0, 360);
            itr->omega = GetNormalFloatDist(-150, 150);
            itr->colour =  CONFETTI_COLOURS[GetUniformIntDist(0, CONFETTI_COLOURS.size() - 1)];

            ++itr;
        }
    }

    void ConstructorB()
    {
        std::generate(confetti_.begin(), confetti_.end(), []()
        {
            return Confetti
            {
                // Use designated initializer
                .position = { (float)GetRandomValue(0, GetScreenWidth()), (float)GetRandomValue(-GetScreenHeight() * 0.25, 0) },
                .velocity = { GetNormalFloatDist(0, 100), GetNormalFloatDist(0, 100) },
                .size = (float)GetRandomValue(5, 12), (float)GetRandomValue(8, 20),
                .orientation = (float)GetRandomValue(0, 360),
                .omega = GetNormalFloatDist(-150, 150),
                .colour = CONFETTI_COLOURS[GetUniformIntDist(0, CONFETTI_COLOURS.size() - 1)],
                .active = true
            };
        });
    }
private:
    std::vector<Confetti> confetti_;
};

int main()
{
    std::ofstream file("./build/benchmarks/celebration-results.csv");
    ankerl::nanobench::Bench bench;

    bench.minEpochIterations(500).title("Constructors")
        .run("For Loop", [&]
        {
            Dummy d {};
            d.ConstructorA();

            ankerl::nanobench::doNotOptimizeAway(d);
        })
        .run("std::generate", [&]
        {
            Dummy d {};
            d.ConstructorB();

            ankerl::nanobench::doNotOptimizeAway(d);
        });

    // Render the results to a csv file
    bench.render(ankerl::nanobench::templates::csv(), file);
}
