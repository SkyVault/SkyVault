#include "ai.h"

void AI::Wait(float time){ 
    CurrentState = WAIT;
    wait_timer = time;
}

void AI::MoveRelative(const sf::Vector2f& to) {
    targetLocation = bodyPosition + to;
    CurrentState = MOVE_RELATIVE;
}
