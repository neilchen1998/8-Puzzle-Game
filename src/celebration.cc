#include "gui/celebrationlib.hpp"

#include <algorithm> // std::generate
#include <array>     // std::array

#include "slidr/math/mathlib.hpp" // GetNormalFloatDist, GetUniformIntDist, GetUniFloatDist

namespace
{
constexpr float GRAVITY = 200.0;
constexpr std::array<Color, 6> CONFETTI_COLOURS{LIGHT_CORAL, APRICOT,  LEMON,
                                                MINT,        SKY_BLUE, LAVENDER};
} // namespace

Celebration::Celebration()
{
    // Generate confetti
    std::generate(confetti_.begin(), confetti_.end(), [this]() { return GenerateConfetti(); });

    fxApplause_ = LoadSound("resources/applause.wav");
}

Celebration::~Celebration() { UnloadSound(fxApplause_); }

void Celebration::PlayApplauseSound()
{
    // Checks if the sound is NOT playing
    if (!IsSoundPlaying(fxApplause_))
    {
        PlaySound(fxApplause_);
    }
}

void Celebration::StopApplauseSound()
{
    // Checks if the sound is playing
    if (IsSoundPlaying(fxApplause_))
    {
        StopSound(fxApplause_);
    }
}

void Celebration::Update()
{
    Spawn5Confetti();

    // Get the delta time
    float deltaT = GetFrameTime();

    auto itr = confetti_.begin();
    while (itr != confetti_.end())
    {
        // Only update the confetti that is active
        if (itr->active)
        {
            // Apply gravity
            itr->velocity.y += GRAVITY * deltaT;

            // Calculate the new position & angular position
            itr->position.x += itr->velocity.x * deltaT;
            itr->position.y += itr->velocity.y * deltaT;
            itr->orientation += itr->omega * deltaT;

            // Check if the confetti is still on the screen
            if (itr->position.y > GetScreenHeight() + 10)
            {
                itr->active = false;
            }
        }

        ++itr;
    }
}

void Celebration::Draw() const
{
    auto itr = confetti_.cbegin();
    while (itr != confetti_.cend())
    {
        if (itr->active)
        {
            Rectangle rec = {itr->position.x, itr->position.y, itr->size.x, itr->size.y};
            Vector2 origin = {(itr->position.x / 2), (itr->position.y / 2)};

            DrawRectanglePro(rec, origin, itr->orientation, itr->colour);
        }
        ++itr;
    }
}

Celebration::Confetti Celebration::GenerateConfetti()
{
    // Use designated initializer
    const float quarterScreen = 0.2 * GetScreenWidth();
    return Confetti{.position = {GetUniFloatDist(0, GetScreenWidth()),
                                 GetUniFloatDist(-quarterScreen, quarterScreen)},
                    .velocity = {GetNormalFloatDist(10, 50), GetNormalFloatDist(10, 50)},
                    .size = GetUniFloatDist(5, 12),
                    GetUniFloatDist(8, 20),
                    .orientation = GetUniFloatDist(0, 360),
                    .omega = GetNormalFloatDist(10, 50),
                    .colour = CONFETTI_COLOURS[GetUniformIntDist(0, CONFETTI_COLOURS.size() - 1)],
                    .active = true};
}

void Celebration::Spawn5Confetti()
{
    int cnt = 0;
    for (auto &c : confetti_)
    {
        // Break if 5 items have been activated
        if (cnt >= 5)
        {
            break;
        }

        // Check if the current item can be activated
        if (!c.active)
        {
            c = GenerateConfetti();
            ++cnt;
        }
    }
}
