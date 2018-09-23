#ifndef SKYVAULT_PHYSICS_FILTER
#define SKYVAULT_PHYSICS_FILTER
#include "../filter.h"
#include "../components/body.h"
#include "../components/physics_body.h"

struct PhysicsFilter : public Filter {
    PhysicsFilter() : Filter({typeid(Body), typeid(PhysicsBody)}) {}
    void Update(const SkyTime& time, std::unique_ptr<Entity>& entity) override;
    void Render(std::unique_ptr<sf::RenderWindow>& window, std::unique_ptr<Entity>& entity) override;
};

#endif//SKYVAULT_PHYSICS_FILTER
