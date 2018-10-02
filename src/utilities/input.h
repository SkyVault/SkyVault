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
