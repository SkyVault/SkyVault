#ifndef SKYVAULT_ENTITY_WORLD_H
#define SKYVAULT_ENTITY_WORLD_H

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>

#include "entity.h"
#include "filter.h"
#include "../skytime.h"

struct EntityWorld {

    Entity* Create();

    template <typename T, typename... Args>
    void Register(Args&&... args) {
        filters.push_back(std::make_unique<T>(std::forward<Args>(args)...)); 
    }

    void Update(const SkyTime& time);
    void Render(std::unique_ptr<sf::RenderWindow>& window);
    
    template<typename T>
    Entity* GetFirstWith() {
        for (auto& entity : entities) {
            if (entity->Has<T>()) {
                return entity.get();
            }
        }
        return nullptr;
    }

private:
    std::vector<std::unique_ptr<Filter>> filters;
    std::vector<std::unique_ptr<Entity>> entities;
};

#endif//SKYVAULT_ENTITY_WORLD_H
