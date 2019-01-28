#ifndef SKYVAULT_EDITOR_H
#define SKYVAULT_EDITOR_H

#include "../imgui.h"
#include "../imgui-SFML.h"
#include "../skytime.h"
#include "../graphics/assets.h"
#include "../graphics/camera.h"
#include "../graphics/animation.h"
#include "../graphics/sky.h"
#include "../graphics/tiled_map.h"
#include "../graphics/camera.h"
#include "../entities/entity_world.h"
#include "../entities/components/body.h"
#include "../entities/components/player.h"
#include "../entities/components/laser.h"
#include "../entities/components/physics_body.h"
#include "../entities/components/renderable.h"
#include "../skyvault.h"
#include "../utilities/input.h"

#include <memory>
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <sol.hpp>
#include <typeinfo>
#include <iostream>
#include <string>

/*
    TODO(Dustin):
        - [X] Place entities by dragging and dropping
        - [ ] Place entity spawns by dragging and dropping
        - [ ] Allow for deletion of entities
 */

struct Editor {
    void Destroy(); // serializes out the table

    void initUI(std::unique_ptr<sf::RenderWindow> &window, std::shared_ptr<sol::state>& lua);
    void processEvent(sf::Event& event);

    void doUI
        ( std::unique_ptr<sf::RenderWindow> &window
        , const SkyTime& time
        , std::shared_ptr<EntityWorld>& world
        , std::shared_ptr<Sky>& sky
        , std::shared_ptr<TiledMap>& tiledMap
        , std::shared_ptr<Camera>& camera
        );

    void doEntityInspector
        ( std::shared_ptr<EntityWorld>& world
        , std::unique_ptr<sf::RenderWindow> &window
        , std::shared_ptr<TiledMap> &tiledMap
        , std::shared_ptr<Camera>& camera
        );

    void Draw
        ( std::unique_ptr<sf::RenderWindow> &window
        , std::shared_ptr<TiledMap> &tiledMap
        , std::shared_ptr<EntityWorld>& world
        );

    enum HoldingState {
        None,
        Entity,
        Billboard,
        Door
    };

private:
    std::shared_ptr<sol::state> lua;

    EntityID moving{-1};

    void doInGameTerminal();
    void doColors(std::shared_ptr<Sky>& sky);

    std::vector<std::string> command_history;
    sf::Clock editorClock;

    sf::Vector2f cursor;
    sf::Vector2f placement_offset{sf::Vector2f(0, 0)};

    sf::Vector2f cam_size;

    bool HoldingBillboardToBePlaced;
    bool HoldingEntityToBePlaced;

    bool PlaceAsForeground{false};

    EntityID CurrentlySelectedEntity{-1};

    sf::IntRect BillboardRect{sf::IntRect(0, 0, 0, 0)};
    sf::IntRect EntityRect{sf::IntRect(0, 0, 0, 0)};

    sol::table editor_save_data;
    sol::table entity_prefab;

    sf::Vector2f EntityInspectorSize{0, 0};
};

#endif//SKYVAULT_EDITOR_H
