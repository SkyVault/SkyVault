#ifndef SKYVAULT_PARTICLE_EMITTER
#define SKYVAULT_PARTICLE_EMITTER

#include "../component.h"
#include "../../skytime.h"

#include <SFML/Graphics.hpp>

struct Particle {
    sf::Vector2f Position{sf::Vector2f()};
    sf::Vector2f Velocity{sf::Vector2f()};

    sf::Vector2f Size{sf::Vector2f(8, 8)};

    sf::Color Color{sf::Color::White};
    sf::Color TargetColor{sf::Color::White};

    float Life{0.0f};
    float Rotation{0.0f};
};

struct ParticleEmitterFilter;

struct ParticleEmitter : public Component {
    friend ParticleEmitterFilter;

    sf::Vector2f Offset{sf::Vector2f(0, 0)};
    sf::FloatRect Region{sf::FloatRect(-10, -10, 10, 10)};

    float Radius{64.0f};

    sf::FloatRect VelocityBounds{sf::FloatRect(-10, 10, -10, 10)};

    sf::Vector2f SizeBounds{sf::Vector2f(4, 32)};
    sf::Vector2f LifeRange{sf::Vector2f(0.25f, 2.0f)};

    sf::Color Color{sf::Color::White};

    sf::Vector2i ParticleNumber{sf::Vector2i(1, 1)};

    float MaxTime{0.2f};

private:
    float timer{0.0f};
};

#endif//SKYVAULT_PARTICLE_EMITTER
