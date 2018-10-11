#ifndef SKYVAULT_INTERACTION_FILTER
#define SKYVAULT_INTERACTION_FILTER

#include <SFML/Graphics.hpp>
#include "../../skyvault.h"
#include "../filter.h"
#include "../entity.h"
#include "../components/body.h"
#include "../components/player.h"
#include <array>
#include <algorithm>

struct InteractionFilter : public Filter {
    InteractionFilter() : Filter({
            typeid(Body)
            }){}


    void PreLoad() override;
    void Update(const SkyTime& time, std::unique_ptr<Entity>& entity) override;
    void PostRender(std::unique_ptr<sf::RenderWindow>& window) override;

private:
    std::array<Entity*, MAP_SIZE*MAP_SIZE> grid;
};

#endif//SKYVAULT_INTERACTION_FILTER
