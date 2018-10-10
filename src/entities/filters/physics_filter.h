#ifndef SKYVAULT_PHYSICS_FILTER
#define SKYVAULT_PHYSICS_FILTER
#include "../filter.h"
#include "../components/body.h"
#include "../components/physics_body.h"
#include <vector>
#include <memory>

struct Solid : public Body{
    Solid(float x, float y, float width, float height):
        Body(sf::Vector2f(x, y), sf::Vector2f(width, height)){}
};

struct PhysicsFilter : public Filter {
    PhysicsFilter() : Filter({typeid(Body), typeid(PhysicsBody)}) {}
    void Update(const SkyTime& time, std::unique_ptr<Entity>& entity) override;
    void PostRender(std::unique_ptr<sf::RenderWindow>& window) override;

    void AddSolid(float x, float y, float width, float height);
    void ClearSolids();

private:
    std::vector<std::unique_ptr<Solid>> solids;
};

#endif//SKYVAULT_PHYSICS_FILTER
