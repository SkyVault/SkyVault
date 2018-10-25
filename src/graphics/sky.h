#ifndef SKYVAULT_SKY_H
#define SKYVAULT_SKY_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "../skytime.h"

struct CloudState {
    sf::Vector2f Position;
    sf::Vector2f Scale;

    float y{0.0f};
    float WindScale{0.0f};
};

struct Sky : public sf::Drawable, public sf::Transformable {
    Sky();

    void Load(int width, int height);
    void Update(int width, int height, const SkyTime& time);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::Vector3f Suncolor{sf::Vector3f(1.0, 0.7 ,0.3)};
    sf::Vector3f Highsky{sf::Vector3f(0.1,0.2,0.8)};
    sf::Vector3f Lowsky{sf::Vector3f(0.78,0.78,0.7)};

private:
    sf::Texture clouds;

    sf::Shader shader;
    sf::RectangleShape background;
    sf::Texture noiseTexture;
    sf::Texture cloudTexture;

    sf::Sprite cloud;

    std::vector<CloudState> cloudStates;
};


#endif//SKYVAULT_SKY_H
