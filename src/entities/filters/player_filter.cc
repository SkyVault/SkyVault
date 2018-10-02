#include "player_filter.h"
#include "../components/physics_body.h"
#include "../../skyvault.h"

void PlayerFilter::Load(std::unique_ptr<Entity>& entity) {

}

void PlayerFilter::Update(const SkyTime& time, std::unique_ptr<Entity>& self) {
    //var body = self->Get<Body>();
    var physics = self->Get<PhysicsBody>();

    constexpr float speed{400.0f};

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

}

void PlayerFilter::Render(std::unique_ptr<sf::RenderWindow>& window, std::unique_ptr<Entity>& entity) {

}
