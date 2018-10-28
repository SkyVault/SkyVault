#include "sky.h"
#include "../skyvault.h"
#include <iostream>

Sky::Sky() {
    noiseTexture.loadFromFile("assets/images/noise.png");
    cloudTexture.loadFromFile("assets/images/clouds.png");
    shader.loadFromFile("assets/shaders/sky.vert", "assets/shaders/sky.frag");
    cloud.setTexture(cloudTexture);
}

void Sky::Load(int width, int height) {
    std::cout << width << " " << height << std::endl;
    for (int i = 0; i < 30; i++) {
        const auto scale = (rand_float() * 2.0f) * 0.5; 
        cloudStates.push_back(CloudState{
            sf::Vector2f(
                rand_float() * (float)width, 
                rand_float() * (float)height
            ),
            sf::Vector2f((1 + scale) + (rand_float() * 0.2f), 1 + scale),

            (rand_float() * (float)height),
            1.0f + rand_float() * 1.5f
        });
    }
}

void Sky::Update(int width, int height, const SkyTime& time) {
    background.setSize(sf::Vector2f(width, height));
    //background.setFillColor(sf::Color(0.0, 0.2*255, 0.5*255, 255));
    //noise.setScale(1280/255.0, 720/255.0);
    //shader.setUniform("iTime", time.timer);
    //shader.setUniform("iChannel0", noiseTexture);
    //shader.setUniform("iMouse", sf::Mouse::getPosition());

    shader.setUniform("Suncolor", Suncolor);
    shader.setUniform("Highsky", Highsky);
    shader.setUniform("Lowsky", Lowsky);

    int i = 0;
    for(auto& cloud : cloudStates) {
        cloud.Position.x += time.dt * 10.0f * cloud.WindScale;
        cloud.Position.y = cos(time.timer + (i * 100.0f)) * cloud.WindScale * 2.0f;
        if (cloud.Position.x > width) {
            cloud.Position.x = -(cloudTexture.getSize().x * cloud.Scale.x);
        }
        ++i;
    }
}

void Sky::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(background, &shader);

    sf::Sprite cloud;
    cloud.setTexture(cloudTexture);
    for(const auto& c : cloudStates) {
        cloud.setPosition(sf::Vector2f(c.Position.x, c.Position.y + c.y));
        cloud.setScale(c.Scale);
        target.draw(cloud);
    }
}
