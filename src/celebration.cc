#include "gui/celebrationlib.hpp"

Celebration::Celebration()
{
    // Resize the confetti vector
    confetti_.resize(MAX_NUM_CONFETTI);

    // Set all confetti to not active
    auto itr = confetti_.begin();
    while (itr != confetti_.end())
    {
        itr->active = false;

        ++itr;
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
            Rectangle rec = { itr->position.x, itr->position.y, itr->size.x, itr->size.y };
            Vector2 origin = { (itr->position.x / 2), (itr->position.y / 2) };

            DrawRectanglePro(rec, origin, itr->orientation, itr->color);
        }
        ++itr;
    }
}

void Celebration::SpawnConfetti()
{
    auto itr = confetti_.begin();
    while (itr != confetti_.end())
    {
        if (!itr->active)
        {
            itr->active = true;
            itr->position = (Vector2){ (float)GetRandomValue(0, GetScreenWidth()), (float)GetRandomValue(-GetScreenHeight() * 0.25, 0) };
            itr->velocity = (Vector2){ (float)GetNormalFloatDist(-100, 100), (float)GetNormalFloatDist(-100, 100) };
            itr->size = (Vector2){ (float)GetRandomValue(5, 12), (float)GetRandomValue(8, 20) };
            itr->orientation = (float)GetRandomValue(0, 360);
            itr->omega = (float)GetNormalFloatDist(-150, 150);
            itr->color = CONFETTI_COLOURS[GetUniformIntDist(0, CONFETTI_COLOURS.size() - 1)];
            break;
        }

        ++itr;
    }
}
