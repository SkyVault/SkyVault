#ifndef SKYVAULT_RENDER_FILTER_H
#define SKYVAULT_RENDER_FILTER_H

#include <iostream>
#include "../filter.h"
#include "../../graphics/art.h"
#include "../components/body.h"
#include "../components/renderable.h"

struct RenderFilter : public Filter {
    RenderFilter() : Filter({
            typeid(Body),
            typeid(Renderable)
            }) {}

    void Update(const SkyTime& time, Entity* entity) override;
    void Render(std::unique_ptr<sf::RenderWindow>& window, Entity* entity) override;
};

struct RenderAnimatedSpriteFilter : public Filter {
    RenderAnimatedSpriteFilter() : Filter({
            typeid(Body),
            typeid(AnimatedSprite)
            }) {}

    void Update(const SkyTime& time, Entity* entity) override;
    void Render(std::unique_ptr<sf::RenderWindow>& window, Entity* entity) override;
};

#endif//SKYVAULT_RENDER_FILTER_H
