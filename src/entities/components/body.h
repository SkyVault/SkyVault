#ifndef SKYVAULT_BODY_H
#define SKYVAULT_BODY_H

#include "../component.h"

#include <cmath>
#include <SFML/Graphics.hpp>
#include <iostream>

struct Body : public Component {
    Body(){}
    Body(const sf::Vector2f& position, const sf::Vector2f& size): 
        Position(position), 
        Size(size){}

    inline bool Contains(const Body& other) {
        return 
            Position.x + Size.x > other.Position.x &&
            Position.x < other.Position.x + other.Size.x &&
            Position.y + Size.y > other.Position.y &&
            Position.y < other.Position.y + other.Size.y;
    }

    inline float Left() { return Position.x; }
    inline float Right() { return Position.x + Size.x; }

    inline float Top() { return Position.y; }
    inline float Bottom() { return Position.y + Size.y; }

    inline sf::Vector2f Center() {
        return sf::Vector2f(
            Position.x + Size.x / 2.f,
            Position.y + Size.y / 2.f
        );
    }

    inline float Distance(const Body& other) {
        const auto a = Position;
        const auto b = other.Position;
        return sqrtf(powf((b.x - a.x), 2) + powf((b.y - a.y), 2));
    }

    sf::Vector2f Position{sf::Vector2(0, 0)};
    sf::Vector2f Size{sf::Vector2(0, 0)};
};

#endif//SKYVAULT_BODY_H
