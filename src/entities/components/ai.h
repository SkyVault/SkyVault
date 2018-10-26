#ifndef SKYVAULT_AI_H
#define SKYVAULT_AI_H
#include "../component.h"
#include "../filter.h"
#include "../../skytime.h"
#include <functional>
#include <map>
#include <string>
#include <cmath>

typedef std::function<void(const SkyTime&, std::unique_ptr<Entity>&)> AiAction;

struct AIFilter;
struct AI : public Component {
    friend AIFilter;

    AI(AiAction action): DoAI(action) {}

    enum States {
        WAIT, 
        MOVE_TO_LOCATION,
        MOVE_RELATIVE,
        TRACK_PLAYER
    };

    inline bool WaitIsDone() {return wait_timer <= 0.0f;}
    void Wait(float time);
    void MoveRelative(const sf::Vector2f& to);

    AiAction DoAI;

    States CurrentState{WAIT}; 

    inline bool ReachedTarget() {
        constexpr float margin{0.1f};
        const auto a = bodyPosition;
        const auto b = targetLocation; 
        const auto res = sqrtf(powf((b.x - a.x),2) + powf((b.y - a.y), 2));
        return res <= margin;
    }

    inline bool DoFirst() {
        if (isFirst) {
            isFirst = false;
            return true;
        }
        return false;
    }

    std::map<std::string, bool> Flags;

private:
    bool isFirst{true};
    float wait_timer{0.0f};

    sf::Vector2f bodyPosition{sf::Vector2f(0, 0)};
    sf::Vector2f targetLocation{sf::Vector2f(0, 0)};

};

#endif//SKYVAULT_AI_H
