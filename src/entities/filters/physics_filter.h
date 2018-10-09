#ifndef SKYVAULT_PHYSICS_FILTER
#define SKYVAULT_PHYSICS_FILTER
#include "../filter.h"
#include "../components/body.h"
#include "../components/physics_body.h"
#include <vector>
#include <memory>

struct Solid {
    float x; 
    float y;
    float width;
    float height;
    Solid(float x, float y, float width, float height):
        x(x), y(y), width(width), height(height) {}
};

struct PhysicsFilter : public Filter {
    PhysicsFilter() : Filter({typeid(Body), typeid(PhysicsBody)}) {}
    void Update(const SkyTime& time, std::unique_ptr<Entity>& entity) override;
    void Render(std::unique_ptr<sf::RenderWindow>& window, std::unique_ptr<Entity>& entity) override;

    void AddSolid(float x, float y, float width, float height);
    void ClearSolids();

private:
    std::vector<std::unique_ptr<Solid>> solids;
};

#endif//SKYVAULT_PHYSICS_FILTER
