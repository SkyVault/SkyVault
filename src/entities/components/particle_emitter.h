#ifndef SKYVAULT_PARTICLE_EMITTER
#define SKYVAULT_PARTICLE_EMITTER
#endif//SKYVAULT_PARTICLE_EMITTER

#include "../component.h"
#include "../../skytime.h"

#include <SFML/Graphics.hpp>

struct Particle {
    sf::Vector2f Position{sf::Vector2f()};
    sf::Vector2f Velocity{sf::Vector2f()};

    sf::Vector2f Size{sf::Vector2f(8, 8)};

    sf::Color Color{sf::Color::White};
    sf::Color TargetColor{sf::Color::White};

    float Rotation{0.0f};
};

struct ParticleEmitterFilter;

struct ParticleEmitter {
    friend ParticleEmitterFilter;

    sf::Vector2f Offset;
    sf::FloatRect Region;

    float Radius{-1.0f};

    // X Y -> Min; Width height -> Max
    sf::FloatRect VelocityBounds;
    sf::FloatRect SizeBounds;

    sf::Color Color;

    sf::Vector2i ParticleNumber{sf::Vector2i(1, 1)};

    float MaxTime{0.2f};

private:
    float timer{0.0f};
};
