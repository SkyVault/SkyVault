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

struct Filter {
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

    virtual void Load(std::unique_ptr<Entity>& entity) {}
    virtual void Update(const SkyTime& time, std::unique_ptr<Entity>& entity) {}
    virtual void Render(std::unique_ptr<sf::RenderWindow>& window, std::unique_ptr<Entity>& entity) {}
    virtual void Destroy(std::unique_ptr<Entity>& entity) {}

private:
    std::vector<std::type_index> filter;
};

#endif//SKYVAULT_FILTER_H
