#include "gui/celebrationlib.hpp"

#include <algorithm> // std::generate

#include "slidr/math/mathlib.hpp" // GetNormalFloatDist, GetUniformIntDist

Celebration::Celebration()
    : confetti_(MAX_NUM_CONFETTI)
{
    // Set all confetti to not active
    std::generate(confetti_.begin(), confetti_.end(), [this]() { return GenerateConfetti(); });

    applause_ = LoadSound("resources/applause.wav");
}

Celebration::~Celebration() { UnloadSound(applause_); }

void Celebration::PlayApplauseSound()
{
    // Checks if the sound is NOT playing
    if (!IsSoundPlaying(applause_))
    {
        PlaySound(applause_);
    }
}

void Celebration::StopApplauseSound()
{
    // Checks if the sound is playing
    if (IsSoundPlaying(applause_))
    {
        StopSound(applause_);
    }
}

void Celebration::Update()
{
    // Generate 5 confetti at a time
    for (size_t i = 0; i < 5; i++)
    {
        SpawnConfetti();
    }

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
    return Confetti{// Use designated initializer
                    .position = {(float)GetRandomValue(0, GetScreenWidth()),
                                 (float)GetRandomValue(-GetScreenHeight() * 0.25, 0)},
                    .velocity = {GetNormalFloatDist(0, 100), GetNormalFloatDist(0, 100)},
                    .size = (float)GetRandomValue(5, 12),
                    (float)GetRandomValue(8, 20),
                    .orientation = (float)GetRandomValue(0, 360),
                    .omega = GetNormalFloatDist(-150, 150),
                    .colour = CONFETTI_COLOURS[GetUniformIntDist(0, CONFETTI_COLOURS.size() - 1)],
                    .active = true};
}

void Celebration::SpawnConfetti()
{
    auto itr = confetti_.begin();
    while (itr != confetti_.end())
    {
        if (!itr->active)
        {
            *itr = GenerateConfetti();
            break;
        }

        ++itr;
    }
}
