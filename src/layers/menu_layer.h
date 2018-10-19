#ifndef SKYVAULT_MENU_LAYER_H
#define SKYVAULT_MENU_LAYER_H

#include <SFML/Graphics.hpp>
#include "../utilities/input.h"
#include "layer.h"
#include "level_layer.h"
#include "../game_state.h"

struct MenuLayer : public Layer {
    inline MenuLayer( 
        const std::shared_ptr<EntityWorld>& world,
        const std::shared_ptr<Camera>& camera,
        const std::shared_ptr<sol::state>& lua):

    world(std::move(world)),
    camera(std::move(camera)),
    lua(std::move(lua))

    {

    }

    void Load() override;
    void Update(const SkyTime& time) override;
    void Render(std::unique_ptr<sf::RenderWindow>& window) override;
    void Destroy() override;

private:
    std::shared_ptr<EntityWorld> world;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<sol::state> lua;
};

#endif//SKYVAULT_MENU_LAYER_H
