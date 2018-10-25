#ifndef SKYVAULT_WATER_H
#define SKYVAULT_WATER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "../skytime.h"

struct Water : public sf::Drawable, public sf::Transformable {
    Water();

    void Load(int width, int height);
    void Update(int width, int height, const SkyTime& time);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::Shader shader;
    sf::RectangleShape background;
};

#endif//SKYVAULT_WATER_H
