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
#include "../entities/entity_world.h"
#include "../entities/components/body.h"
#include "../entities/components/player.h"
#include "../entities/components/physics_body.h"
#include "../entities/components/renderable.h"
#include "../skyvault.h"
#include "../utilities/input.h"

#include <memory>
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <sol.hpp>
#include <typeinfo>

struct Editor {
    void initUI(std::unique_ptr<sf::RenderWindow> &window, std::shared_ptr<sol::state>& lua);
    void processEvent(sf::Event& event);

    void doUI
        ( std::unique_ptr<sf::RenderWindow> &window
        , const SkyTime& time
        , std::shared_ptr<EntityWorld>& world
        , std::shared_ptr<Sky>& sky
        , std::shared_ptr<TiledMap>& tiledMap
        );

    void doEntityInspector
        ( std::shared_ptr<EntityWorld>& world
        , std::unique_ptr<sf::RenderWindow> &window
        , std::shared_ptr<TiledMap> &tiledMap
        );
    void Draw(std::unique_ptr<sf::RenderWindow> &window, std::shared_ptr<TiledMap> &tiledMap);

private:
    std::shared_ptr<sol::state> lua;

    void doInGameTerminal();
    void doColors(std::shared_ptr<Sky>& sky);

    std::vector<std::string> command_history;
    sf::Clock editorClock;

    sf::Vector2f cursor;

    bool HoldingBillboardToBePlaced;
    sf::IntRect BillboardRect{sf::IntRect(0, 0, 0, 0)};
};

#endif//SKYVAULT_EDITOR_H
