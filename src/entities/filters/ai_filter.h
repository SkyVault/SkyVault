#ifndef SKYVAULT_AI_FILTER_H
#define SKYVAULT_AI_FILTER_H

#include "../filter.h"
#include "../components/ai.h"
#include "../components/body.h"
#include "../components/physics_body.h"
#include <SFML/Graphics.hpp>
#include <cmath>

struct AIFilter : public Filter {
    AIFilter() : Filter({
            typeid(AI),
            typeid(PhysicsBody),
            typeid(Body)
            }) {}

    void Update(const SkyTime& time, std::unique_ptr<Entity>& entity) override;
    void Render(std::unique_ptr<sf::RenderWindow>& window, std::unique_ptr<Entity>& entity) override;
};

#endif//SKYVAULT_AI_FILTER_H
