#ifndef SKYVAULT_PHYSICS_BODY_H
#define SKYVAULT_PHYSICS_BODY_H
#include "../component.h"

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cmath>

#ifndef MACROSTR
#define MACROSTR(k) #k
#endif

#define X_PHYSICS_TYPES\
	X(PHYSICS_STATIC)\
	X(PHYSICS_LADDER)\
	X(PHYSICS_DYNAMIC)

enum PhysicsTypes {
#define X(Enum) Enum,
     X_PHYSICS_TYPES
#undef X
};

const static std::vector<std::string> PhysicsTypesNames = {
#define X(String) MACROSTR(String),
    X_PHYSICS_TYPES
#undef X
};


struct PhysicsBody : public Component {
    PhysicsBody(PhysicsTypes type=PhysicsTypes::PHYSICS_STATIC);

    sf::Vector2f Velocity;
    float Friction{0.5f};

    void Decelerate(float dt);

private:
    PhysicsTypes type{PHYSICS_STATIC};
};

#endif//SKYVAULT_PHYSICS_BODY_H
