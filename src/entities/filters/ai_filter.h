#ifndef SKYVAULT_AI_FILTER_H
#define SKYVAULT_AI_FILTER_H

#include "../filter.h"
#include "../entity_world.h"
#include "../components/ai.h"
#include "../components/body.h"
#include "../components/physics_body.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <memory>

struct AIFilter : public Filter {
    AIFilter(std::shared_ptr<EntityWorld>& world) : Filter({
            typeid(AI),
            typeid(PhysicsBody),
            typeid(Body)
            }), world(world) {}

    void Update(const SkyTime& time, std::unique_ptr<Entity>& entity) override;
    void Render(std::unique_ptr<sf::RenderWindow>& window, std::unique_ptr<Entity>& entity) override;

private:
    std::shared_ptr<EntityWorld> world;
};

#endif//SKYVAULT_AI_FILTER_H
