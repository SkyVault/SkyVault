#ifndef SKYVAULT_RENDER_FILTER_H
#define SKYVAULT_RENDER_FILTER_H

#include "../filter.h"
#include "../components/body.h"
#include "../components/renderable.h"

struct RenderFilter : public Filter {
    RenderFilter() : Filter({
            typeid(Body),
            typeid(Renderable)
            }) {}

    void Update(const SkyTime& time, std::unique_ptr<Entity>& entity) override;
    void Render(std::unique_ptr<sf::RenderWindow>& window, std::unique_ptr<Entity>& entity) override;
};

#endif//SKYVAULT_RENDER_FILTER_H
