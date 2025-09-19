#ifndef INCLUDE_GUI_CELEBRATIONLIB_H_
#define INCLUDE_GUI_CELEBRATIONLIB_H_

#include <vector>   // std::vector
#include <array>   // std::array

#include "raylib.h"
#include "math/mathlib.hpp"
#include "gui/colourlib.hpp"

namespace
{
    constexpr float GRAVITY = 200.0;
    constexpr size_t MAX_NUM_CONFETTI = 500;

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
        Color color;

        /// @brief True if the confetti is on the screen
        bool active;
    };

    std::array<Color, 6> CONFETTI_COLOURS { LIGHT_CORAL, APRICOT, LEMON, MINT, SKY_BLUE, LAVENDER };
}

class Celebration
{
public:
    Celebration();

    ~Celebration() = default;

    /// @brief Updates the confetti
    void Update();

    /// @brief Draws the confetti
    void Draw() const;

private:
    /// @brief Spawns a single confetti
    void SpawnConfetti();

private:
    /// @brief 
    std::vector<Confetti> confetti_;
};

#endif // INCLUDE_GUI_CELEBRATIONLIB_H_
