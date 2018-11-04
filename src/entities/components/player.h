#ifndef SKYVAULT_PLAYER_H
#define SKYVAULT_PLAYER_H

#include "../component.h"
#include "../../utilities/state_machine.h"
#include "item.h"

#include <vector>

struct Player : public Component {
    enum State {
        ATTACKING,
        DASHING,
        MOVING,
    };

    float DashTimer{0.0f};
    const float MaxDashTime{1.0f};
    float DashScaler{4.0f};
    float PreFriction{0.0f};
    SM State{SM(MOVING)};

    std::vector<Item> Invatory;
};

#endif//SKYVAULT_PLAYER_H
