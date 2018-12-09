#ifndef SKYVAULT_FILTER_H
#define SKYVAULT_FILTER_H

#include <SFML/Graphics.hpp>
#include <typeindex>
#include <vector>
#include <algorithm>
#include <initializer_list>
#include <memory>
#include "entity.h"
#include "../skytime.h"

struct EntityWorld; 

struct Filter {
    friend EntityWorld;

    Filter(const std::initializer_list<std::type_index> ids) {
        for (const auto t : ids) {
            filter.push_back(t);
        }
    }

    // this could be very much faster 
    auto Matches(std::vector<std::type_index>& ids) {
        for (const auto& t : filter)
            if (std::find(ids.begin(), ids.end(), t) == ids.end())
                return false;
        return true;
    }

    virtual void Load(Entity* entity) {}

    virtual void PreUpdate(const SkyTime& time) {}
    virtual void Update(const SkyTime& time, Entity* entity) {}

    // Doesnt get effected by pausing
    virtual void ConstantUpdate(const SkyTime& time, Entity* entity) {}

    virtual void Render(std::unique_ptr<sf::RenderWindow>& window, Entity* entity) {}
    virtual void Destroy(Entity* entity) {}

    virtual void PostRender(std::unique_ptr<sf::RenderWindow>& window) {}

protected:
    EntityWorld* World{nullptr};

private:
    std::vector<std::type_index> filter;

};

#endif//SKYVAULT_FILTER_H
