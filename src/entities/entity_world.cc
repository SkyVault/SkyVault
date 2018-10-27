#include "entity_world.h"

#include <tuple>
#include <algorithm>

#include "components/player.h"
#include "components/body.h"
#include "components/physics_body.h"
#include "components/interaction.h"

#include "../game_state.h"
#include "../skyvault.h"

Entity* EntityWorld::Create() {
    var entity = new Entity();
    entities.push_back(std::unique_ptr<Entity>(entity));
    return entity;
}

void EntityWorld::Update(const SkyTime& time) {
    // Clear the interaction grid
    std::fill(grid.begin(), grid.end(), nullptr);

    for (auto& [key, filter] : filters) {
        filter->PreLoad();
    }

    const auto player = GetFirstWith<Player>();
    constexpr auto margin{10.0f};

    std::vector<std::tuple<float, Interaction*>> potential_interactions;

    for (auto& e : entities) {
        for (auto& [key, f] : filters) {
            if (f->Matches(e->GetMatchlist())) {
                if (!e->loaded) f->Load(e);
                if (e->remove) f->Destroy(e);
                else {
                    if (GameState::It()->CurrentState == GameState::States::PLAYING_STATE)
                        f->Update(time, e);   
                    f->ConstantUpdate(time, e);   
                }
            }
        }

        e->loaded = true;

        if (player && e->Has<Interaction>() && e->Has<Body>()) {
            e->Get<Interaction>()->Hot = false;
            const auto player_body = player->Get<Body>();
            const auto obody = e->Get<Body>();
            const auto distance = obody->Distance(*player_body);
            if (distance < (obody->Size.x + margin)){
                potential_interactions.push_back({distance, e->Get<Interaction>()});
            }
        }

        // If the entity has a body, place him in the interaction grid
        if (e->Has<Body>()) {
            const auto& body = e->Get<Body>();
            
            const auto gx = static_cast<int>((body->Position.x * 8 * 4) / MAP_SIZE);
            const auto gy = static_cast<int>((body->Position.y * 8 * 4) / MAP_SIZE);

            const auto gx2 = static_cast<int>((32 * (body->Position.x + body->Size.x)) / MAP_SIZE);
            const auto gy2 = static_cast<int>((32 * (body->Position.y + body->Size.y)) / MAP_SIZE);

            for (int yy = gy; yy <= gy2; yy++)
                for (int xx = gx; xx <= gx2; xx++)
                    this->grid[xx+yy*MAP_SIZE] = e.get(); // Use an int identifier
        }
    }

    if (potential_interactions.size() > 0) {
        if (potential_interactions.size() > 1) {
            std::sort(potential_interactions.begin(), potential_interactions.end(), [](auto a, auto b) {
                return std::get<0>(a) < std::get<0>(b);
            });
        }
        
        const auto winner = std::get<1>(potential_interactions[0]);
        winner->Hot = true;

        if (Input::It()->IsKeyPressed(sf::Keyboard::Z)) {
            // Set the players velocity to zero, avoids sliding past entity

            player->Get<PhysicsBody>()->Velocity = sf::Vector2f(0, 0);
            winner->onInteractionWithPlayer();
        }
    }
}

void EntityWorld::Render(std::unique_ptr<sf::RenderWindow>& window) {
    for (auto& e : entities) {
        for (auto& [key, f] : filters) {
            if (f->Matches(e->GetMatchlist())) {
                f->Render(window, e);
            }
        }

        if (e->Has<Interaction>() && e->Has<Body>()) {
            // Draw the interaction shape
            if (e->Get<Interaction>()->Hot){
                const auto body = e->Get<Body>();
                sf::RectangleShape shape(sf::Vector2f(16, 16));
                shape.setPosition(e->Get<Body>()->Position - sf::Vector2f(16, body->Size.y));
                shape.setFillColor(sf::Color::Red);
                window->draw(shape);
            }
        }
    }

    for (auto& [key, f] : filters) {
        f->PostRender(window);
    }


    if (GameState::It()->IsDebug() == false) return;

    constexpr float grid_square{8.0f};

    sf::RectangleShape shape;

    for (int y = 0; y < MAP_SIZE; y++) {
        for (int x = 0; x < MAP_SIZE; x++) {
            if (grid[x + y * MAP_SIZE] != nullptr) {
                shape.setSize(sf::Vector2f(grid_square, grid_square)); 
                shape.setFillColor(sf::Color(255, 0, 0, 25));
                shape.setPosition(sf::Vector2f(x * grid_square, y * grid_square));
                window->draw(shape);
            }
        }
    }

    shape.setFillColor(sf::Color(255, 255, 255, 25));
    for (int x = 0; x < MAP_SIZE; x++) {
        shape.setPosition(sf::Vector2f(x * grid_square, 0));
        shape.setSize(sf::Vector2f(1, MAP_SIZE * grid_square));
        window->draw(shape);
    }

    for (int y = 0; y < MAP_SIZE; y++) {
        shape.setPosition(sf::Vector2f(0, y * grid_square));
        shape.setSize(sf::Vector2f(MAP_SIZE * grid_square, 1));
        window->draw(shape);
    }
}
