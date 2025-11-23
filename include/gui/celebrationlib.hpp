#ifndef INCLUDE_GUI_CELEBRATIONLIB_H_
#define INCLUDE_GUI_CELEBRATIONLIB_H_

#include <array>  // std::array
#include <vector> // std::vector

#include "gui/colourlib.hpp" // LIGHT_CORAL, APRICOT, LEMON, etc.
#include "raylib.h"          // Vector2

namespace
{
constexpr std::size_t MAX_NUM_CONFETTI = 400;
}

class Celebration
{
public:
    Celebration();

    ~Celebration();

    /// @brief Updates the confetti
    void Update();

    /// @brief Draws the confetti
    void Draw() const;

    /// @brief Plays applause sound
    void PlayApplauseSound();

    /// @brief Stops playing applause sound
    void StopApplauseSound();

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
    void Spawn5Confetti();

    /// @brief Generates a piece of confetti
    Confetti GenerateConfetti();

private:
    /// @brief the confetti container
    std::array<Confetti, MAX_NUM_CONFETTI> confetti_;

    /// @brief The applause sound effect
    Sound fxApplause_;
};

#endif // INCLUDE_GUI_CELEBRATIONLIB_H_
