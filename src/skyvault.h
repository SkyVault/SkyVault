#ifndef SKYVAULT_SKYVAULT_H
#define SKYVAULT_SKYVAULT_H

#define VERSION_MAJOR "0"
#define VERSION_REVISION "0"
#define VERSION_MINOR "1"
#define SKYVAULT_VERSION VERSION_MAJOR "." VERSION_REVISION "." VERSION_MINOR

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

#endif//SKY_VAULT_H
