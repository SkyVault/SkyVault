#ifndef SKYVAULT_CAMERA_H
#define SKYVAULT_CAMERA_H
#include <SFML/Graphics.hpp>

struct Camera {
    Camera(int width, int height);
    
    void Resize(int width, int height);
    void Clamp(const sf::IntRect& bounds);

    void Track(const sf::Vector2f& track_point, const float smoothing=0.08f);
    
    sf::View View;
};

#endif//SKYVAULT_CAMERA_H
