#ifndef SKYVAULT_INTERACTION_H
#define SKYVAULT_INTERACTION_H

#include "../component.h"
#include <sol.hpp>
#include <functional>

struct Interaction : public Component {
    inline Interaction(std::function<void()> onInteractionWithPlayer) : onInteractionWithPlayer(onInteractionWithPlayer) {}
    inline Interaction(sol::function onInteractionWithPlayerLua) : onInteractionWithPlayerLua(onInteractionWithPlayerLua) {
        LuaFunction = true; 
    }

    bool Hot{false};
    bool LuaFunction{false};

    inline void Interact() {
        if (LuaFunction) {
            onInteractionWithPlayerLua();
        } else {
            onInteractionWithPlayer();
        }
    }

private:
    std::function<void()> onInteractionWithPlayer;
    sol::function onInteractionWithPlayerLua;

};

#endif//SKYVAULT_INTERACTION_H 
