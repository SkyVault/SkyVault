#include "ai_filter.h"

void AIFilter::Update(const SkyTime& time, Entity* entity) {
    auto ai = entity->Get<AI>();

    auto body = entity->Get<Body>();
    auto physics = entity->Get<PhysicsBody>();
    ai->bodyPosition = body->Position;

    const auto player_opt = world->GetFirstWith<Player>();

    if (!player_opt) return;
    const auto player = player_opt.value();

    if (player != nullptr && body != nullptr) {
        const auto player_body = player->Get<Body>();
        ai->dist_to_player = sqrtf(
                powf(player_body->Position.x - body->Position.x, 2) +
                powf(player_body->Position.y - body->Position.y, 2)
                );
    }

    ai->DoAI(time, entity, ai, world);

    switch (ai->CurrentState) {
        case AI::States::WAIT: {
            ai->wait_timer -= time.dt;
            break;
        }
        case AI::States::MOVE_RELATIVE: case AI::States::MOVE_TO_RANDOM_LOCATION_RELATIVE: {
            const auto curr = body->Position;
            const auto tar = ai->targetLocation;

            auto dir = atan2f(
                (tar.y - curr.y),
                (tar.x - curr.x)
            );

            // MOVE
            constexpr float speed{20.0f};

            physics->Velocity = sf::Vector2f(cosf(dir), sinf(dir)) * speed;

            if (physics->CollidingWithSolid()) {
                ai->targetLocation = curr;  
                physics->Velocity = sf::Vector2f(0, 0);
            }

            break;
        }
        default: break;
    }
}

void AIFilter::Render(std::unique_ptr<sf::RenderWindow>& window, Entity* entity) {

}
