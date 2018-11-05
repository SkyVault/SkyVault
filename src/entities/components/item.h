#ifndef SKYVAULT_ITEM_H
#define SKYVAULT_ITEM_H

#include "../component.h"

#include <SFML/Graphics.hpp>
#include <string>

struct Item : public Component {
    inline Item(const std::string& Name, sf::Sprite Sprite) : Name(Name), Sprite(Sprite) {}
    inline Item(const Item& copy) {
        Name = copy.Name;
        Sprite = copy.Sprite;
    }
    std::string Name;
    sf::Sprite Sprite; 
};

#endif//SKYVAULT_ITEM_H
