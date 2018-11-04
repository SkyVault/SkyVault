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
	X(PHYSICS_DYNAMIC)\
	X(PHYSICS_ITEM)

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


struct PhysicsFilter;
struct PhysicsBody : public Component {
    friend PhysicsFilter;

    PhysicsBody(PhysicsTypes type=PhysicsTypes::PHYSICS_DYNAMIC);

    sf::Vector2f Velocity;
    float Friction{0.02f};

    void Decelerate(float dt);
    inline PhysicsTypes GetType() {
        return type;
    }

    inline bool CollidingWithSolid(){ return colliding_with_solid; }

private:
    PhysicsTypes type{PHYSICS_STATIC};
    bool colliding_with_solid{false}; 
};

#endif//SKYVAULT_PHYSICS_BODY_H
