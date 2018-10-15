#ifndef SKYVAULT_PLAYER_H
#define SKYVAULT_PLAYER_H
#include "../component.h"
#include "../../utilities/state_machine.h"

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
};

#endif//SKYVAULT_PLAYER_H
