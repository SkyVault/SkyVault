#include "physics_body.h"

PhysicsBody::PhysicsBody(PhysicsTypes type):
type(type) 
{
    
}

void PhysicsBody::Decelerate(float dt) {
    float v = powf(Friction, dt);
    Velocity = Velocity * v;
}
