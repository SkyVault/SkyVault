#ifndef SKYVAULT_SKYVAULT_H
#define SKYVAULT_SKYVAULT_H

#define VERSION_MAJOR "0"
#define VERSION_REVISION "0"
#define VERSION_MINOR "1"
#define SKYVAULT_VERSION VERSION_MAJOR "." VERSION_REVISION "." VERSION_MINOR

// Size of a map in tiles, used mainly for entity interactions
#define MAP_SIZE 256

#include <random>

#define let const auto
#define var auto
#define global static

#ifndef EDITOR
#define EDITOR
#endif//EDITOR

//TODO: Implement a state system
enum GameStates {
    PAUSED,
    RUNNING,
    QUITTING
};

global bool is_debugging {true};

static bool IsDebug() { return is_debugging; }
static void ToggleDebug() { is_debugging = !is_debugging; }

inline int rand_int(int min, int max) {
    static std::mt19937 rng;
    static bool first{true};

    if (first){
        rng.seed(std::random_device()());
        first = false;
    }

    std::uniform_int_distribution<std::mt19937::result_type> dist6(1,6); // distribution in range [1, 6]
    return dist6(rng);
}

inline float rand_float() {
    return rand() / (RAND_MAX + 1.f);
}

#endif//SKY_VAULT_H
