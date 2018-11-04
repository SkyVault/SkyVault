#include "ai.h"

void AI::Wait(float time){ 
    CurrentState = WAIT;
    wait_timer = time;
}

void AI::MoveRelative(const sf::Vector2f& to) {
    targetLocation = bodyPosition + to;
    CurrentState = MOVE_RELATIVE;
}

void AI::MoveRelativeRandom(const float min_dist, const float max_dist) {
    const float dist = min_dist + (rand_float() * (max_dist - min_dist));
    const float direction = rand_float() * (2.0f*3.1415926f);

    targetLocation = bodyPosition + sf::Vector2f(cosf(direction) * dist, sinf(direction) * dist);
    CurrentState = MOVE_TO_RANDOM_LOCATION_RELATIVE;
}

bool AI::ReachedTarget() {
    constexpr float margin{0.1f};
    const auto a = bodyPosition;
    const auto b = targetLocation; 
    const auto res = sqrtf(powf((b.x - a.x),2) + powf((b.y - a.y), 2));
    return res <= margin;
}
