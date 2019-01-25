#ifndef SKYVAULT_LEVEL_LAYER_H
#define SKYVAULT_LEVEL_LAYER_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <sol.hpp>

#include "layer.h"
#include "../skyvault.h"
#include "../utilities/quest_engine.h"
#include "../entities/entity_world.h"
#include "../entities/components/body.h"
#include "../entities/components/renderable.h"
#include "../entities/components/player.h"
#include "../entities/components/renderable.h"
#include "../entities/components/ai.h"
#include "../entities/components/laser.h"
#include "../entities/components/shop_keeper.h"
#include "../entities/components/interaction.h"
#include "../entities/components/particle_emitter.h"
#include "../graphics/tiled_map.h"
#include "../graphics/camera.h"
#include "../graphics/sky.h"
#include "../graphics/art.h"
#include "../graphics/assets.h"
#include "../graphics/gui.h"
#include "../skytime.h"
#include "../game_state.h"

struct LevelLayer: public Layer {
    inline LevelLayer(
        const std::shared_ptr<EntityWorld>& world,
        const std::shared_ptr<Camera>& camera,
        const std::shared_ptr<sol::state>& lua,
        const std::shared_ptr<GUI>& gui,
        const std::shared_ptr<TiledMap>& tiledMap,
        const std::shared_ptr<Sky>& sky
        ):

    world(std::move(world)),
    camera(std::move(camera)),
    lua(std::move(lua)),
    gui(std::move(gui)),
    sky(std::move(sky)),
    tiledMap(std::move(tiledMap))
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
    std::shared_ptr<GUI> gui;
    std::shared_ptr<Sky> sky;
    std::shared_ptr<TiledMap> tiledMap;

    const int AtlasSize{5};
    std::vector<std::string> Atlas;

    sf::Vector2i globalPosition;

};

#endif//SKYVAULT_LEVEL_LAYER_H
