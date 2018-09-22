#ifndef SKYVAULT_BODY_H
#define SKYVAULT_BODY_H

#include "../component.h"
#include <SFML/Graphics.hpp>

struct Body : public Component {
    sf::Vector2f Position;
    sf::Vector2f Size;
};

#endif//SKYVAULT_BODY_H
