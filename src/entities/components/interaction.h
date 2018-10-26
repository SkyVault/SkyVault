#ifndef SKYVAULT_INTERACTION_H
#define SKYVAULT_INTERACTION_H

#include "../component.h"
#include <functional>

struct Interaction : public Component {
    Interaction(std::function<void()> onInteractionWithPlayer) : onInteractionWithPlayer(onInteractionWithPlayer) {}
    std::function<void()> onInteractionWithPlayer;
};

#endif//SKYVAULT_INTERACTION_H 
