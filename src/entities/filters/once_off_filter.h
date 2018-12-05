#ifndef SKYVAULT_ONCE_OFF_FILTER_H
#define SKYVAULT_ONCE_OFF_FILTER_H

#include "../filter.h"
#include "../components/laser.h"
#include "../components/body.h"
#include <SFML/Graphics.hpp>

struct OnceOffFilter : public Filter { 
    inline OnceOffFilter() 
    : Filter({typeid(Laser)}) {}

    void Update(const SkyTime& time, std::unique_ptr<Entity>& entity) override;
    void Render(std::unique_ptr<sf::RenderWindow>& window, std::unique_ptr<Entity>& entity) override;
};

#endif//SKYVAULT_ONCE_OFF_FILTER_H
