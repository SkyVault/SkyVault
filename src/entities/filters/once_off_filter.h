#ifndef SKYVAULT_ONCE_OFF_FILTER_H
#define SKYVAULT_ONCE_OFF_FILTER_H

#include "../filter.h"
#include "../entity_world.h"
#include "../components/laser.h"
#include "../components/body.h"
#include <SFML/Graphics.hpp>

struct OnceOffFilter : public Filter { 
    inline OnceOffFilter(std::shared_ptr<EntityWorld>& world)
    : Filter({typeid(Laser)}), world(world) {}

    void Update(const SkyTime& time, std::unique_ptr<Entity>& entity) override;
    void Render(std::unique_ptr<sf::RenderWindow>& window, std::unique_ptr<Entity>& entity) override;

private:
    std::shared_ptr<EntityWorld> world;
};

#endif//SKYVAULT_ONCE_OFF_FILTER_H
