#include "camera.h"

Camera::Camera(int width, int height) {
    View = sf::View(sf::FloatRect(0, 0, width, height));
}

void Camera::Resize(int width, int height) {
    //View = sf::View(sf::FloatRect(View.x, View.y, width, height));
}
