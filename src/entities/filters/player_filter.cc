#include "player_filter.h"
#include "../components/physics_body.h"
#include "../../skyvault.h"

void PlayerFilter::Load(Entity* entity) {

}

void PlayerFilter::Update(const SkyTime& time, Entity* self) {
    var body = self->Get<Body>();
    var player = self->Get<Player>();
    var physics = self->Get<PhysicsBody>();

    constexpr float speed{900.0f};
    
    switch(player->State.CurrentState()) {
    case Player::State::DASHING: {
        physics->Friction = 0.002f;
        if (player->DashTimer <= 0.0f)  {
            player->State.SetState(Player::State::MOVING);
            physics->Friction = player->PreFriction;
        }
        player->DashTimer -= time.dt;
        break;
    }

    case Player::State::MOVING: {
        auto axis = Input::It()->GetMovementAxis();

        physics->Velocity.x += speed * time.dt * axis.x;
        physics->Velocity.y += speed * time.dt * axis.y;

        // TODO(Dustin): Check for collision with movable object
        if (physics->entity_collisions.size() > 0) {
            for (auto id : physics->entity_collisions) {
                auto e_opt = world->GetEntity(id);

                if (!e_opt) continue;

                auto other = e_opt.value();

                if (other->HasTag("Enemy")) {

                    // Enter combat with enemy
                    // Temp: 
                    other->Kill();

                    GameState::It()->PushLayer(
                            new CombatLayer(
                                world,
                                camera,
                                lua,
                                sky
                            )); 

                } else if (other->HasTag("Block")) {
                    // TODO(Dustin): Get the side that the player is on, then lock the block to only
                    // move in that direction, or maybe clamp the other directions but make them move
                    // slightly

                    other->Get<PhysicsBody>()->Velocity = physics->Velocity * 0.2f; 
                }
            }
        }

        if (Input::It()->IsKeyPressed(sf::Keyboard::LShift)) {
            if (player->DashTimer <= 0.0f) {
                auto v = physics->Velocity;
                float len = sqrtf((v.x*v.x) + (v.y*v.y));
                if (len == 0.0f) len = 1.f;

                sf::Vector2f norm = v / len;

                player->DashTimer = player->MaxDashTime;
                physics->Velocity = norm * 400.0f;
                player->PreFriction = physics->Friction;
                player->State.SetState(Player::State::DASHING);
            }
        }

        break;
    }
    default:
        break;
    }
    player->State.Tick();

    // Update camera
    if (GameState::It()->FullEditor() == false &&
        GameState::It()->NormalMode()) {
        const auto track_point = 
            body->Center() 
            + physics->Velocity * 0.50f;

        camera->Track(track_point);
    } 
}

void PlayerFilter::Render(std::unique_ptr<sf::RenderWindow>& window, Entity* entity) {

}
