#ifndef SKYVAULT_BODY_H
#define SKYVAULT_BODY_H

#include "../component.h"
#include <SFML/Graphics.hpp>
#include <iostream>

struct Body : public Component {
    Body(){}
    Body(const sf::Vector2f& position, const sf::Vector2f& size): 
        Position(position), 
        Size(size){}

    sf::Vector2f Position{sf::Vector2(0, 0)};
    sf::Vector2f Size{sf::Vector2(0, 0)};
};

#endif//SKYVAULT_BODY_H
