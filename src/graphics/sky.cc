#include "sky.h"
#include "../skyvault.h"

Sky::Sky() {

}

void Sky::Update(int width, int height, const SkyTime& time) {
    let lerp = [](float a, float b, float t)->float {
        return (1 - t) * a + t * b;
    };

    if (time.ticks % 500 == 0) { 
        current_color++;
    }

    current_color %= day_shades.size();
    
    let lastColor = day_shades[(current_color - 1) % day_shades.size()];
    let currentColor = day_shades[current_color];
    let myColor = sf::Color(
        lerp(currentColor.r, lastColor.r, time.dt), 
        lerp(currentColor.g, lastColor.g, time.dt), 
        lerp(currentColor.b, lastColor.b, time.dt), 
        255);

    background.setSize(sf::Vector2f(width, height));
    background.setFillColor(sf::Color(0.0, 0.2*255, 0.5*255, 255));
}

void Sky::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(background);
}
