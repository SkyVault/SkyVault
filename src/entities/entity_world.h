#ifndef SKYVAULT_ENTITY_WORLD_H
#define SKYVAULT_ENTITY_WORLD_H

#include "../utilities/input.h"

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include <typeindex>
#include <sol.hpp>

#include "entity.h"
#include "filter.h"
#include "../skytime.h"
#include "../skyvault.h"
#include "../graphics/assets.h"

struct EntityWorld {

    Entity* Create();
    Entity* Create(const sol::table& prefab);

    template <typename T, typename... Args>
    void Register(Args&&... args) {
        filters[typeid(T)] = (std::make_unique<T>(std::forward<Args>(args)...)); 
        filters[typeid(T)]->World = this;
    }

    template <typename T>
    T* GetFilter() {
        // This returns a raw pointer that shouldn't be stored anywhere, or freed
        return (T*)filters[typeid(T)].get();
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

    template<typename T>
    inline std::vector<Entity*> GetAllWith(){
        std::vector<Entity*> result;
        for (auto& entity : entities) {
            if (entity->Has<T>()) result.push_back(entity.get());
        }
        return result;
    }

    inline std::vector<std::unique_ptr<Entity>>& GetEntities() { return entities; }

private:
    std::map<std::type_index, std::unique_ptr<Filter>> filters;
    std::vector<std::unique_ptr<Entity>> entities;

    // Interaction grid
    std::array<Entity*, MAP_SIZE*MAP_SIZE> grid;
};

#endif//SKYVAULT_ENTITY_WORLD_H
