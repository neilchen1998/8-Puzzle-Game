#ifndef INCLUDE_GUI_CELEBRATIONLIB_H_
#define INCLUDE_GUI_CELEBRATIONLIB_H_

#include <vector>   // std::vector
#include <array>   // std::array

#include "raylib.h" // Vector2
#include "gui/colourlib.hpp"    // LIGHT_CORAL, APRICOT, LEMON, etc.

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

private:
    /// @brief Spawns a piece of confetti
    void SpawnConfetti();

    /// @brief Generates a piece of confetti
    Confetti GenerateConfetti();

private:
    /// @brief the confetti container
    std::vector<Confetti> confetti_;

    /// @brief The gravity
    static constexpr float GRAVITY = 200.0;

    /// @brief the maximum pieces of confetti
    static constexpr std::size_t MAX_NUM_CONFETTI = 400;

    /// @brief the colours of the confetti
    static constexpr std::array<Color, 6> CONFETTI_COLOURS { LIGHT_CORAL, APRICOT, LEMON, MINT, SKY_BLUE, LAVENDER };
};

#endif // INCLUDE_GUI_CELEBRATIONLIB_H_
