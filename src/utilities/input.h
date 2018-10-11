#ifndef SKYVAULT_INPUT_H
#define SKYVAULT_INPUT_H

#include <map>
#include <SFML/Graphics.hpp>
#include <string>
#include <type_traits>
#include <sol.hpp>
#include <iostream>

#include <mutex>

struct KeyState { int state, last; };

struct Input {
private:
	Input(const Input&) = delete;
	Input& operator=(const Input&) = delete;

	static std::unique_ptr<Input> instance;
	static std::once_flag onceFlag;

    Input() {

    }

public:
    enum Button {
        DPAD_UP         = 0x0001,
        DPAD_DOWN       = 0x0002,
        DPAD_LEFT       = 0x0004,
        DPAD_RIGHT      = 0x0008,
        START           = 0x0010,
        BACK            = 0x0020,
        LEFT_THUMB      = 0x0040,
        RIGHT_THUMB     = 0x0080,
        LB              = 0x0100,
        RB              = 0x0200,
        A               = 0x1000,
        B               = 0x2000,
        X               = 0x4000,
        Y               = 0x8000,
    };

    static auto* It() {
        std::call_once(Input::onceFlag, [] () {
            instance.reset(new Input());     
        });

        return instance.get();
    }

    void Update();
    void HandleEvent(sf::Event& event);

    bool IsKeyPressed(int key);
    bool IsKeyReleased(int key);
    bool IsKeyDown(int key);
    bool IsKeyUp(int key);

private:

    std::map<int, KeyState> key_map;
};

#endif//SKYVAULT_INPUT_H
