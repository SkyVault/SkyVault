#include "entity.h"

void Entity::Kill() {
   remove = true;
}

void Entity::Revive() {
   remove = false;
}
