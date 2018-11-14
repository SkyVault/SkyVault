#ifndef SKYVAULT_LAYER_H
#define SKYVAULT_LAYER_H
#include "../skytime.h"
#include <memory>
#include <SFML/Graphics.hpp>

struct Layer {
    virtual ~Layer() {}
    virtual void Load() {}
    virtual void Update(const SkyTime& time) {}
    virtual void Render(std::unique_ptr<sf::RenderWindow>& window) {}
    virtual void Destroy() {}
};

#endif//SKYVAULT_LAYER_H
