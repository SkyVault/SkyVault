#ifndef SKYVAULT_FRAME_H
#define SKYVAULT_FRAME_H
#include <SFML/Graphics.hpp>

struct Frame {
    Frame(int x, int y, int w, int h, float duration=100.0f) : 
    duration(duration),
    x(x), y(y), width(w), height(h)
    {

    }

    Frame(){}

    float duration{100.0f};

    int x{0}, y{0}, width{0}, height{0};
};

#endif//SKYVAULT_FRAME_H
