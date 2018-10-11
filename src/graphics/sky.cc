#include "sky.h"
#include "../skyvault.h"

Sky::Sky() {
    noiseTexture.loadFromFile("assets/images/noise.png");
    cloudTexture.loadFromFile("assets/images/clouds.png");
    shader.loadFromFile("assets/shaders/sky.vert", "assets/shaders/sky.frag");
    cloud.setTexture(cloudTexture);
}

void Sky::Load(int width, int height) {
    for (int i = 0; i < 30; i++) {
        const auto scale = rand_float() * 2.0f; 
        const auto flip = rand_int(0, 1) == 0 ? -1.0f : 1.0f;
        cloudStates.push_back(CloudState{
            sf::Vector2f(
                -400.0f + rand_float() * (float)width + 100.0f, 
                (height * 0.3) + (rand_float() * (float)height) * 0.7f
            ),
            sf::Vector2f((1 + scale) * flip, 1 + scale),
            1.0f + rand_float() * 1.5f
        });
    }
}

void Sky::Update(int width, int height, const SkyTime& time) {
    background.setSize(sf::Vector2f(width, height));
    //background.setFillColor(sf::Color(0.0, 0.2*255, 0.5*255, 255));
    //noise.setScale(1280/255.0, 720/255.0);
    shader.setUniform("iTime", time.timer);
    shader.setUniform("iChannel0", noiseTexture);
    shader.setUniform("iMouse", sf::Mouse::getPosition());

    for(auto& cloud : cloudStates) {
        cloud.Position.x += time.dt * 10.0f * cloud.WindScale;
        if (cloud.Position.x > width) {
            cloud.Position.x = -(cloudTexture.getSize().x * cloud.Scale.x);
        }
    }
}

void Sky::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(background, &shader);

    sf::Sprite cloud;
    cloud.setTexture(cloudTexture);
    for(const auto& c : cloudStates) {
        cloud.setPosition(c.Position);
        cloud.setScale(c.Scale);
        target.draw(cloud);
    }
}
