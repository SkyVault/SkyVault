#include "camera.h"

Camera::Camera(int width, int height) {
    View = sf::View(sf::FloatRect(0, 0, width, height));
}

void Camera::Resize(int width, int height) {
}

void Camera::Clamp(const sf::IntRect& bounds) {
    auto [cx, cy] = View.getCenter();
    auto [w, h] = View.getSize();

    auto vx = cx - (w * 0.5f);
    auto vy = cy - (h * 0.5f);
    auto vw = w;
    auto vh = h; 

    if (vx < bounds.left) {
        vx = bounds.left;
    }

    if (vx + vw > bounds.left + bounds.width) {
        vx = bounds.left + bounds.width - vw;
    }

    if (vy < bounds.top) {
        vy = bounds.top;
    }

    if (vy + vh > bounds.top + bounds.height) {
        vy = bounds.top + bounds.height - vh;
    }

    View.setCenter(sf::Vector2f
            ( vx + w * 0.5f
            , vy + h * 0.5f
            ));
}
