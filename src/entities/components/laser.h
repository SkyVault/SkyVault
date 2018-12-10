#ifndef SKYVAULT_LASER_H
#define SKYVAULT_LASER_H

#include "../component.h"
#include "../../skyvault.h"

enum LaserColor {
    Red,
    Blue,
    Green
};

struct Laser : public Component {
    inline Laser(LaserColor Color) : Color(Color) {}
    inline Laser() {}
    LaserColor Color{Red}; 
    Cardinal Facing{West};
};

#endif//SKYVAULT_LASER_H
