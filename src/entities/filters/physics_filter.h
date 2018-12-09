#ifndef SKYVAULT_PHYSICS_FILTER
#define SKYVAULT_PHYSICS_FILTER

#include "../filter.h"
#include "../components/body.h"
#include "../components/physics_body.h"
#include "../components/player.h"
#include "../entity_world.h"
#include <memory>
#include <vector>

struct Solid : public Body{
    Solid(float x, float y, float width, float height):
        Body(sf::Vector2f(x, y), sf::Vector2f(width, height)){}
};

//struct EntityWorld;
struct PhysicsFilter : public Filter {
    //friend EntityWorld;

    PhysicsFilter(std::shared_ptr<EntityWorld>& world) : Filter({
            typeid(Body), 
            typeid(PhysicsBody)
            }), world(world) {}

    void PreUpdate(const SkyTime& time) override;
    void Update(const SkyTime& time, Entity* entity) override;
    void PostRender(std::unique_ptr<sf::RenderWindow>& window) override;

    void AddSolid(float x, float y, float width, float height);
    void ClearSolids();

private:
    std::vector<std::unique_ptr<Solid>> solids;
    std::shared_ptr<EntityWorld> world;
    std::vector<int> physicsBodiesIds;
};

#endif//SKYVAULT_PHYSICS_FILTER
