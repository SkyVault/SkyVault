#include "entity_world.h"

#include <tuple>
#include <algorithm>
#include <type_traits>

#include "components/player.h"
#include "components/body.h"
#include "components/renderable.h"
#include "components/item.h"
#include "components/physics_body.h"
#include "components/interaction.h"
#include "components/ai.h"

#include "filters/physics_filter.h"

#include "../game_state.h"
#include "../skyvault.h"
 
/*
    NOTE(Dustin): 
    Now I'm thinking that all over the entity system we want to see which entities 
    intersect each other, so we probably should just do that here in the entity world
    though there are some interesting issues with that, one of which is that this code
    doesnt really allow for modularity. So its harder to move this code to a new project 
    because it assumes that collisions need to be checked.
 */

// TODO(Dustin): Implement animated sprites
EntityWorld::EntityWorld() {
    entity_list = new Entity* [ENTITY_LIST_LENGTH];
    for (int i = 0; i < ENTITY_LIST_LENGTH; i++)
        entity_list[i] = nullptr; 
}

EntityWorld::~EntityWorld() {
    for (int i = 0; i < entity_list_length; i++)
        if (entity_list[i] != nullptr){
            delete entity_list[i];
        }
    delete entity_list;
}

Entity* EntityWorld::Create() {
    var entity = new Entity(); 
    int index = add_entity(entity); 
    entity->entity_id = index;
    return entity;
}

int EntityWorld::add_entity(Entity* entity) {

    if (GameState::It()->CurrentState == GameState::States::COMBAT_STATE) { 
        combat_entities.push_back(entity);
        return -1;
    } 

    for (int i = 0; i < entity_list_length; i++) {
        if (entity_list[i] == nullptr) {
            entity_list[i] = entity;
            return i;
        }
    }

    printf("ERROR:: Ran out of entity slots\n");

    return -1;
}

void EntityWorld::OnRoomChange(std::function<void(std::string)> fn) {
    this->on_room_change = fn; 
}

Entity* EntityWorld::Create(const sol::table& prefab) {
    var entity = new Entity();

    if (prefab.get<sol::table>("tags")) {
        auto t = prefab.get<sol::table>("tags");
        t.for_each([&](const sol::object& key, const sol::object value) {
            entity->AddTags(value.as<std::string>());
        });
    }

    if (sol::table components = prefab.get<sol::table>("components")) {
        components.for_each([&](const sol::object& key, const sol::object value) {
            const std::string& which_component = key.as<std::string>();
            const sol::table& component = value.as<sol::table>();

            // Match the component type
            if (which_component == "Body") {
                const float x = component.get<float>("x");
                const float y = component.get<float>("y");
                const float width = component.get<float>("width");
                const float height = component.get<float>("height");
                entity->Add<Body>(sf::Vector2f(x, y), sf::Vector2f(width, height));
            } 
            else if (which_component == "Sprite") {
                const std::string which_texture = component.get<std::string>("texture");
                const auto texture = Assets::It()->Get<sf::Texture>(which_texture);

                unsigned int rx{0}, ry{0}, rw{texture->getSize().x}, rh{texture->getSize().y};
                float ox{0.0f}, oy{0.0f};

                if (const sol::table region = component.get<sol::table>("region")) {
                    rx = (unsigned int)(int)region[1];
                    ry = (unsigned int)(int)region[2];
                    rw = (unsigned int)(int)region[3];
                    rh = (unsigned int)(int)region[4];
                }

                if (const sol::table offset = component.get<sol::table>("offset")) {
                    ox = (float)offset[1];
                    oy = (float)offset[2];
                }

                entity->Add<Renderable>(texture, sf::IntRect(rx, ry, rw, rh), sf::Vector2f(ox, oy));
            }
            else if (which_component == "Item") {
                const std::string which_texture = component.get<std::string>("texture");
                const auto texture = Assets::It()->Get<sf::Texture>(which_texture);

                unsigned int rx{0}, ry{0}, rw{texture->getSize().x}, rh{texture->getSize().y};

                if (const sol::table region = component.get<sol::table>("region")) {
                    rx = (unsigned int)(int)region[1];
                    ry = (unsigned int)(int)region[2];
                    rw = (unsigned int)(int)region[3];
                    rh = (unsigned int)(int)region[4];
                }

                const std::string name = prefab.get<std::string>("__x_prefab_name");
                entity->Add<Renderable>(texture, sf::IntRect(rx, ry, rw, rh), sf::Vector2f(0, 0));
                entity->Add<Item>(name, entity->Get<Renderable>()->GetSprite());
                entity->Add<PhysicsBody>(PhysicsTypes::PHYSICS_ITEM);
            }
            else if (which_component == "PhysicsBody") {
                entity->Add<PhysicsBody>(); 
            }

            else if (which_component == "Interaction") {
                entity->Add<Interaction>(component.get<sol::function>("fn"));
            } 

            else if (which_component == "Ai") {
                auto which = component.get<std::string>(1);
                if (which == "ColoredBlockAI") entity->Add<AI>(ColoredBlockAI);
            }

            else {
                std::cout << "(WARNING)::EntityWorld::Create(const sol::table& prefab):: Unknown component type: " << which_component << std::endl;
            }

        });
    }

    //TODO(Dustin): Return index instead of pointer
    int index = add_entity(entity);
    entity->entity_id = index;

    return entity;
}

void EntityWorld::Update(const SkyTime& time) {
    // Clear the interaction grid
    //std::fill(grid.begin(), grid.end(), nullptr);
 
    if (GameState::It()->CurrentState == GameState::States::COMBAT_STATE) {
        // Update combat entities
        return;
    }

    for (auto& [key, filter] : filters) {
        filter->PreUpdate(time);
    }

    constexpr auto margin{10.0f};

    std::vector<std::tuple<float, Interaction*>> potential_interactions;

    //for (auto& e : entities) {
    for (int i = 0; i < entity_list_length; i++) {
        if (entity_list[i] == nullptr) continue;

        auto e = entity_list[i];

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

        if (e->Has<Player>()){
            // Test for intersection with the door
            const auto body = e->Get<Body>();
            for (auto& door : doors) {
                const auto curr = door.Contains(*body);
                if (curr && !door.last) {
                    try {
                        std::invoke(on_room_change, door.To);
                    } catch (std::bad_function_call& e) {}
                }
                door.last = curr; 
            }
        }
        
        Entity* player = GetFirstWith<Player>().value_or(nullptr);
        if (GameState::It()->CurrentState == GameState::States::PLAYING_STATE) {
            
            // We need to make this better
            if (player && e->Has<Interaction>() && e->Has<Body>()){
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
                        this->grid[xx+yy*MAP_SIZE] = e->GetID(); // Use an int identifier
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

            if (player && Input::It()->IsKeyPressed(sf::Keyboard::Z, true)) {
                // Set the players velocity to zero, avoids sliding past entities
                player->Get<PhysicsBody>()->Velocity = sf::Vector2f(0, 0);

                winner->Interact();
            }
        }

        if (e->remove) {
            delete entity_list[i];
            entity_list[i] = nullptr;
        }
    } 
}

void EntityWorld::Render(std::unique_ptr<sf::RenderWindow>& window) {
    if (GameState::It()->CurrentState == GameState::States::COMBAT_STATE) {
        // Draw combat entities
        return;
    }

    for (int i = 0; i < entity_list_length; i++) {
        if (entity_list[i] == nullptr) continue;

        auto* e = entity_list[i];
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
            if (grid[x + y * MAP_SIZE] != -1) {
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

void EntityWorld::AddDoor(const std::string& To, float x, float y, float width, float height) {
    doors.push_back(Door
            ( To
            , x
            , y
            , width
            , height
            ));
}

void EntityWorld::ClearAll() {
    for (int i = 0; i < entity_list_length; i++) {
        if (entity_list[i]) {
            //NOTE(Dustin): Might want to pass this through the filters...
            delete entity_list[i];
        }
    }
    doors.clear();
}
