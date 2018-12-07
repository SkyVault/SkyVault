#include "ai.h"
#include "body.h"

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

float AI::GetDistToPlayer() {
    return dist_to_player;
}

void ColoredBlockAI(const SkyTime& time, std::unique_ptr<Entity>& self, AI* ai) {
    auto sprite = self->Get<Renderable>();

    auto tags = self->GetTags();
    for (auto tag : tags) {
        if (tag == "Red") {
            sprite->Color = sf::Color::Red;
        } else if (tag == "Blue") {
            sprite->Color = sf::Color::Blue; 
        } else if (tag == "Green") {
            sprite->Color = sf::Color::Green; 
        } 
    } 
}

// AI's
void BasicEnemyAI(const SkyTime& time, std::unique_ptr<Entity>& self, AI* ai) {
    if (ai->DoFirst()) { ai->CurrentState = AI::States::IDLE; return; }

    switch(ai->CurrentState) {
        case AI::States::IDLE:
        case AI::States::MOVE_TO_RANDOM_LOCATION_RELATIVE: 
            if (ai->GetDistToPlayer() < 100.0f)
                ai->CurrentState = AI::States::ATTACK_PLAYER;
            break;
        default: break;
    }

    if (ai->CurrentState == AI::States::ATTACK_PLAYER)
        if (ai->GetDistToPlayer() >= 100.0f)
            ai->CurrentState = AI::States::IDLE;

    switch (ai->CurrentState) {
        case AI::States::IDLE: {

            // Move to a random location and scan for player
            ai->MoveRelativeRandom(40, 120);

            break;
        } 
        case AI::States::MOVE_TO_RANDOM_LOCATION_RELATIVE: {
            if (ai->ReachedTarget()) {
                ai->Wait(1.0f);
            }
            break;
        }
        case AI::States::WAIT: {
            if (ai->WaitIsDone()){
                ai->CurrentState = AI::States::IDLE;
            }

            break;
        }

        default: break;
    }
}
