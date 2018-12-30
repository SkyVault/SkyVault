#ifndef SKYVAULT_TWEEN_H
#define SKYVAULT_TWEEN_H

#include <map>
#include <SFML/Graphics.hpp>
#include <type_traits>
#include <sol.hpp>
#include <iostream>
#include <mutex>
#include <functional>
#include <cmath>
#include "../skytime.h"

// Math formulas
/*
quad    = "p * p",
cubic   = "p * p * p",
quart   = "p * p * p * p",
quint   = "p * p * p * p * p",
expo    = "2 ^ (10 * (p - 1))",
sine    = "-math.cos(p * (math.pi * .5)) + 1",
circ    = "-(math.sqrt(1 - (p * p)) - 1)",
back    = "p * p * (2.7 * p - 1.7)",
elastic = "-(2^(10 * (p - 1)) * math.sin((p - 1.075) * (math.pi * 2) / .3))"
*/

constexpr float ERROR_MARGIN {0.001f};

enum InterpolationTypes { 
    LINEAR,
    QUAD,
    CUBIC,
    QUART,
    QUINT,
    EXPO,
    SINE,
    CIRC,
    BACK,
    ELASTIC,
};

// Interpolation functions
namespace Interpolation {
    template <typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
    static T LerpPercent(T a, T b, float t) {
        return a * (1 - t) + b * t;
    } 

    template <typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
    static T Lerp(T a, T b, float t) { return a + (b - a) * t; } 
}
struct TweenTypeFloat {

    float value; 
    float goal;
    float start; 
    float percent{0.0f};
    float timeSeconds{4.0f}; // Seconds
    float currSeconds{0.0f};

    InterpolationTypes type_in;
    InterpolationTypes type_out;

    std::function<void()> oncomplete;

    bool isDone();

    inline TweenTypeFloat
        ( float value
        , float goal
        , float timeSeconds
        , InterpolationTypes type_in=LINEAR
        , InterpolationTypes type_out=LINEAR
        )
    {
        this->value = value;
        this->goal = goal;
        this->start = value;
        this->type_in = type_in;
        this->type_out = type_out;
        this->timeSeconds = timeSeconds;
    }
};

struct Tween {
private:
    Tween(const Tween&) = delete; 
    Tween& operator=(const Tween&) = delete;

	static std::unique_ptr<Tween> instance;
	static std::once_flag onceFlag;

    Tween() {}

public: 
    static auto* It() {
        std::call_once(Tween::onceFlag, [] () {
            instance.reset(new Tween());     
        });

        return instance.get();
    }

    inline auto AddFloatInterp(TweenTypeFloat* interp) {
        tweens.push_back(interp);
        return [interp](std::function<void()> fn) {
            interp->oncomplete = fn;
        };
    }

    void Update(const SkyTime& time);
    

private:
    std::vector<TweenTypeFloat*> tweens;
}; 

#endif//SKYVAULT_TWEEN_H
