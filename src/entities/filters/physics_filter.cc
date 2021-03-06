#include "physics_filter.h"
#include "../../game_state.h"
#include "../../skyvault.h"
//#include "../entity_world.h"

void PhysicsFilter::AddSolid(float x, float y, float width, float height) {
    solids.push_back(std::make_unique<Solid>(x, y, width, height));
}

void PhysicsFilter::ClearSolids(){
    solids.clear();
}

void PhysicsFilter::PreUpdate(const SkyTime& time) {
    //if (World == nullptr) return;

    physicsBodiesIds = world->GetAllIdsWith<PhysicsBody>();
}

void PhysicsFilter::Update(const SkyTime& time, Entity* self) {
    var physics = self->Get<PhysicsBody>();
    var body = self->Get<Body>();

    // Clear physics collisions
    // TODO(Dustin): We should move this to the entity world, 
    // because of the grid system, so that we dont loop through each
    // physics body for each physics body in the game
    physics->entity_collisions.clear();

    physics->colliding_with_solid = false;

    switch (physics->GetType()) {
        case PhysicsTypes::PHYSICS_DYNAMIC: {
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

            if (!GameState::It()->IsNoClip()) {
                for (auto& solid : solids) {
                    if (xbody.Contains(*solid)) {
                        xbody = *body;
                        physics->colliding_with_solid = true;
                    }

                    if (ybody.Contains(*solid)) {
                        ybody = *body;
                        physics->colliding_with_solid = true;
                    }
                } 
            }

            for (int other_id : physicsBodiesIds) {
                auto other_opt = world->GetEntity(other_id);

                if (!other_opt) continue;
                auto other = other_opt.value();

                if (other->GetUUID() == self->GetUUID()) {continue;}

                const auto other_body = other->Get<Body>();
                const auto other_physics = other->Get<PhysicsBody>();

                if (other_physics->GetType() == PHYSICS_DYNAMIC){
                    bool collision{false};
                    if (xbody.Contains(*other_body)) {
                        collision = true;
                        xbody = *body; 
                        physics->colliding_with_solid = true;
                    }

                    if (ybody.Contains(*other_body)) {
                        collision = true;
                        ybody = *body;
                        physics->colliding_with_solid = true;
                    } 

                    if (collision)
                        physics->entity_collisions.push_back(other->GetID());
               
                } else if (other_physics->GetType() == PHYSICS_ITEM) {
                    bool contains{false};
                    if (xbody.Contains(*other_body)) {
                        contains = true;
                    }

                    if (ybody.Contains(*other_body)) {
                        contains = true;
                    } 

                    if (contains && self->Has<Player>()) {
                        auto* player = self->Get<Player>();
                        auto item = other->Get<Item>();
                        player->Invatory.emplace_back(Item(*item));
                        other->Kill();
                    }
                }

            }

            physics->Decelerate(time.dt);

            body->Position = sf::Vector2f(xbody.Left(), ybody.Top());
            break;
        }
        default: break;
    }
}

void PhysicsFilter::PostRender(std::unique_ptr<sf::RenderWindow>& window) {
    if (!GameState::It()->IsDebug()) return;

    sf::RectangleShape shape;
    shape.setOutlineThickness(1);
    for (auto& solid: solids) {
        shape.setPosition(solid->Position);
        shape.setSize(sf::Vector2f(solid->Size));
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineColor(sf::Color::Red);
        window->draw(shape);
    }
}
