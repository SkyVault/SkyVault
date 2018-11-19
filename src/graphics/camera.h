#ifndef SKYVAULT_CAMERA_H
#define SKYVAULT_CAMERA_H
#include <SFML/Graphics.hpp>

struct Camera {
    Camera(int width, int height);
    
    void Resize(int width, int height);
    void Clamp(const sf::IntRect& bounds);
    
    sf::View View;
};

#endif//SKYVAULT_CAMERA_H
