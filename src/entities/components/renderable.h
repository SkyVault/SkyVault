#ifndef SKYVAULT_RENDERABLE_H
#define SKYVAULT_RENDERABLE_H

#include "../component.h"
#include <SFML/Graphics.hpp>

struct RenderFilter;
struct Renderable : public Component {
    friend RenderFilter;

    Renderable();
    Renderable(const sf::Texture* texture);
    Renderable(const sf::Texture* texture, const sf::IntRect& region);
    
    sf::Color Color{sf::Color::White};
private:
    sf::Sprite sprite;
};

#endif//SKYVAULT_RENDERABLE_H
