#ifndef SKYVAULT_PLAYER_FILTER_H
#define SKYVAULT_PLAYER_FILTER_H

#include "../filter.h"
#include "../components/body.h"
#include "../components/renderable.h"
#include "../components/player.h"

struct PlayerFilter : public Filter {
    PlayerFilter() : Filter({
            typeid(Body),
            typeid(Renderable),
            typeid(Player)
            }){}


    void Load(std::unique_ptr<Entity>& entity) override;
    void Update(const SkyTime& time, std::unique_ptr<Entity>& entity) override;
    void Render(std::unique_ptr<sf::RenderWindow>& window, std::unique_ptr<Entity>& entity) override;
};

#endif//SKYVAULT_PLAYER_FILTER_H
