#include "physics_filter.h"
#include "../../skyvault.h"

void PhysicsFilter::AddSolid(float x, float y, float width, float height) {
    solids.push_back(std::make_unique<Solid>(x, y, width, height));
}

void PhysicsFilter::ClearSolids(){
    solids.clear();
}

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
    sf::RectangleShape shape;
    shape.setOutlineThickness(1);
    for (auto& solid: solids) {
        shape.setPosition(solid->x, solid->y);
        shape.setSize(sf::Vector2f(solid->width, solid->height));
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineColor(sf::Color::Red);
        window->draw(shape);
    }
}
