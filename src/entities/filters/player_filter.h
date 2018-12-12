#ifndef SKYVAULT_PLAYER_FILTER_H
#define SKYVAULT_PLAYER_FILTER_H

#include <sol.hpp>

#include "../filter.h"
#include "../entity_world.h"
#include "../../graphics/camera.h"
#include "../components/body.h"
#include "../components/renderable.h"
#include "../components/player.h"
#include "../../utilities/input.h"
#include "../../game_state.h"
#include "../../layers/combat_layer.h"

struct PlayerFilter : public Filter {
    PlayerFilter
        ( const std::shared_ptr<Camera>& camera
        , const std::shared_ptr<EntityWorld>& world
        , const std::shared_ptr<sol::state>& lua
        , const std::shared_ptr<Sky>& sky
        ): 
        Filter(
            { typeid(Body)
            , typeid(Renderable)
            , typeid(Player)
            })
        , camera(std::move(camera)) 
        , world(std::move(world))
        , lua(std::move(lua)) 
        , sky(std::move(sky))
    {}

    void Load(Entity* entity) override;
    void Update(const SkyTime& time, Entity* entity) override;
    void Render(std::unique_ptr<sf::RenderWindow>& window, Entity* entity) override;

    std::shared_ptr<Camera> camera;

private:
    std::shared_ptr<EntityWorld> world;
    std::shared_ptr<sol::state> lua;
    std::shared_ptr<Sky> sky; 
};

#endif//SKYVAULT_PLAYER_FILTER_H
