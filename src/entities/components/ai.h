#ifndef SKYVAULT_AI_H
#define SKYVAULT_AI_H
#include "../component.h"
#include "../components/body.h"
#include "../filter.h"
#include "../entity_world.h"
#include "../entity.h"
#include "renderable.h"
#include "../../skytime.h"
#include "../../skyvault.h"
#include <functional>
#include <algorithm>
#include <map>
#include <string>
#include <cmath>

struct AI;
typedef std::function<void(const SkyTime&, Entity*, AI* ai, std::shared_ptr<EntityWorld>& world)> AiAction;

#ifndef MACROSTR
#define MACROSTR(k) #k
#endif

#define X_AI_STATES\
	X(WAIT)\
	X(IDLE)\
    X(SCAN_FOR_PLAYER)\
    X(MOVE_TO_RANDOM_LOCATION_RELATIVE)\
	X(MOVE_TO_LOCATION)\
	X(MOVE_RELATIVE)\
    X(ATTACK_PLAYER)\
    X(TRACK_PLAYER)

struct AIFilter;
struct EntityWorld;
struct AI : public Component {
    friend AIFilter;
    friend EntityWorld;

    AI(AiAction action): DoAI(action) {}

    enum States {
#define X(Enum) Enum,
    X_AI_STATES
#undef X
    };

    const std::vector<std::string> StatesNames = {
#define X(String) MACROSTR(String),
        X_AI_STATES
#undef X
    };

    inline bool WaitIsDone() {return wait_timer <= 0.0f;}
    void Wait(float time);
    void MoveRelative(const sf::Vector2f& to);
    void MoveRelativeRandom(const float min_dist, const float max_dist);
    bool ReachedTarget();

    float GetDistToPlayer();

    AiAction DoAI;

    States CurrentState {WAIT}; 

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

    float dist_to_player{0.0f};
};

void ColoredBlockAI(const SkyTime& time, Entity* self, AI* ai, std::shared_ptr<EntityWorld>& world);
void BasicEnemyAI(const SkyTime& time, Entity* self, AI* ai, std::shared_ptr<EntityWorld>& world);

#endif//SKYVAULT_AI_H
