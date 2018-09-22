#ifndef SKYVAULT_PLATFORM_H
#define SKYVAULT_PLATFORM_H

#include <tuple>
#include <string>

struct Platform {
    Platform(const std::tuple<int, int> &size, const std::string& title);

};

#endif//SKYVAULT_PLATFORM_H
