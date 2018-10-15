#include "player_filter.h"
#include "../components/physics_body.h"
#include "../../skyvault.h"

void PlayerFilter::Load(std::unique_ptr<Entity>& entity) {

}

void PlayerFilter::Update(const SkyTime& time, std::unique_ptr<Entity>& self) {
    //var body = self->Get<Body>();
    var player = self->Get<Player>();
    var physics = self->Get<PhysicsBody>();

    constexpr float speed{400.0f};
    
    switch(player->CurrentState) {
    case Player::State::DASHING: {
        physics->Friction = 0.002f;
        if (player->DashTimer <= 0.0f)  {
            player->CurrentState = Player::State::MOVING;
            physics->Friction = player->PreFriction;
        }
        player->DashTimer -= time.dt;
        break;
    }
    case Player::State::MOVING: {
            if (Input::It()->IsKeyDown(sf::Keyboard::Left)) {
                physics->Velocity.x -= speed * time.dt;
            }

            if (Input::It()->IsKeyDown(sf::Keyboard::Right)) {
                physics->Velocity.x += speed * time.dt;
            }

            if (Input::It()->IsKeyDown(sf::Keyboard::Up)) {
                physics->Velocity.y -= speed * time.dt;
            }

            if (Input::It()->IsKeyDown(sf::Keyboard::Down)) {
                physics->Velocity.y += speed * time.dt;
            } 

            if (Input::It()->IsKeyPressed(sf::Keyboard::LShift)) {
                if (player->DashTimer <= 0.0f) {
                    float sign = 1;
                    if (physics->Velocity.x < 0.0f || physics->Velocity.y < 0.0f)
                        sign = -1;

                    auto v = physics->Velocity;
                    float len = sqrtf((v.x*v.x) + (v.y*v.y));
                    if (len == 0.0f) len = 1.f;

                    sf::Vector2f norm = v / len;

                    player->DashTimer = player->MaxDashTime;
                    physics->Velocity = norm * 400.0f;
                    player->PreFriction = physics->Friction;
                    player->CurrentState = Player::State::DASHING;
                }
                break;
            }

            break;
        }
    default:
        break;
    }
}

void PlayerFilter::Render(std::unique_ptr<sf::RenderWindow>& window, std::unique_ptr<Entity>& entity) {

}
