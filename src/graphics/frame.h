#ifndef SKYVAULT_FRAME_H
#define SKYVAULT_FRAME_H
#include <SFML/Graphics.hpp>

struct Frame : sf::IntRect {
    float duration{100.0f};
};

#endif//SKYVAULT_FRAME_H
