#include "physics_filter.h"
#include "../../skyvault.h"

void PhysicsFilter::Update(const SkyTime& time, std::unique_ptr<Entity>& self) {
    var physics = self->Get<PhysicsBody>();
    var body = self->Get<Body>();

    var xbody = Body(
        sf::Vector2f(
            body->Left() + physics->Velocity.x * time.dt,
            body->Top()
        ),
        body->Size
    );

    var ybody = Body(
        sf::Vector2f(
            body->Left(),
            body->Top() + physics->Velocity.y * time.dt
        ),
        body->Size
    );

    physics->Decelerate(time.dt);

    body->Position = sf::Vector2f(xbody.Left(), ybody.Top());
}

void PhysicsFilter::Render(std::unique_ptr<sf::RenderWindow>& window, std::unique_ptr<Entity>& self) {

}
