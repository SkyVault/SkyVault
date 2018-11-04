#ifndef SKYVAULT_ITEM_H
#define SKYVAULT_ITEM_H

#include "../component.h"

#include <SFML/Graphics.hpp>
#include <string>

struct Item : public Component {
    std::string Name;
    sf::Sprite Sprite; 
};

#endif//SKYVAULT_ITEM_H
