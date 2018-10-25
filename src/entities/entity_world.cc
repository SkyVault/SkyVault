#include "entity_world.h"
#include "../skyvault.h"
#include "components/body.h"
#include "../game_state.h"

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

    for (auto& e : entities) {
        for (auto& [key, f] : filters) {
            if (f->Matches(e->GetMatchlist())) {
                if (!e->loaded) f->Load(e);
                if (e->remove) f->Destroy(e);
                else f->Update(time, e);
            }
        }

        e->loaded = true;

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
}

void EntityWorld::Render(std::unique_ptr<sf::RenderWindow>& window) {
    for (auto& e : entities) {
        for (auto& [key, f] : filters) {
            if (f->Matches(e->GetMatchlist())) {
                f->Render(window, e);
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
