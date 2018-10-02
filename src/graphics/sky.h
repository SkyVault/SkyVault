#ifndef SKYVAULT_SKY_H
#define SKYVAULT_SKY_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "../skytime.h"

struct Sky : public sf::Drawable, public sf::Transformable {
    Sky();

    void Update(int width, int height, const SkyTime& time);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::Texture clouds;

    std::vector<sf::Color> day_shades = {
        sf::Color(0.0f * 255, 0.4f * 255, 1.0f * 255, 255), // Morning
        sf::Color(0.0f * 255, 0.6f * 255, 1.0f * 255, 255), // Day
        sf::Color(0.0f * 255, 0.6f * 255, 1.0f * 255, 255), // Day
        sf::Color(0.0f * 255, 0.6f * 255, 1.0f * 255, 255), // Day
        sf::Color(0.0f * 255, 0.6f * 255, 1.0f * 255, 255), // Day
        sf::Color(0.0f * 255, 0.6f * 255, 1.0f * 255, 255), // Day
        sf::Color(0.0f * 255, 0.6f * 255, 1.0f * 255, 255), // Day
        sf::Color(0.0f * 255, 0.6f * 255, 1.0f * 255, 255), // Day
        sf::Color(1.0f * 255, 0.5f * 255, 0.0f * 255, 255), // Dusk
        sf::Color(0.0f * 255, 0.0f * 255, 0.2f * 255, 255), // Dusk
        sf::Color(0.0f * 255, 0.0f * 255, 0.2f * 255, 255), // Dusk
        sf::Color(0.0f * 255, 0.0f * 255, 0.2f * 255, 255), // Dusk
    };

    int current_color{0};

    sf::RectangleShape background;
};


#endif//SKYVAULT_SKY_H
