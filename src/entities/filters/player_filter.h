#ifndef SKYVAULT_PLAYER_FILTER_H
#define SKYVAULT_PLAYER_FILTER_H

#include "../filter.h"
#include "../../graphics/camera.h"
#include "../components/body.h"
#include "../components/renderable.h"
#include "../components/player.h"
#include "../../utilities/input.h"
#include "../../game_state.h"

struct PlayerFilter : public Filter {
    PlayerFilter(const std::shared_ptr<Camera>& camera) 
        : Filter(
            { typeid(Body)
            , typeid(Renderable)
            , typeid(Player)
            })

        , camera(std::move(camera)) 
    {}


    void Load(std::unique_ptr<Entity>& entity) override;
    void Update(const SkyTime& time, std::unique_ptr<Entity>& entity) override;
    void Render(std::unique_ptr<sf::RenderWindow>& window, std::unique_ptr<Entity>& entity) override;

    std::shared_ptr<Camera> camera;
};

#endif//SKYVAULT_PLAYER_FILTER_H
