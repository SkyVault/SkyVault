#include "input.h"

void Input::Update() {
    for (auto& [key, state] : key_map) {
        state.last = state.state;
    }

    mouse_wheel_delta = 0.0f;
    left_mouse_button.last = left_mouse_button.state;
}

bool Input::IsMouseLeftDown(int mouse_button) {
    left_mouse_button.state = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    return left_mouse_button.state;
}

bool Input::IsMouseLeftPressed(int mouse_button) {
    left_mouse_button.state = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    return left_mouse_button.state == 1 &&
        left_mouse_button.last == 0;
}

bool Input::IsMouseLeftUp(int mouse_button) {
    left_mouse_button.state = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    return !left_mouse_button.state;
}

bool Input::IsMouseLeftReleased(int mouse_button) {
    left_mouse_button.state = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    return left_mouse_button.state == 0 &&
        left_mouse_button.last == 1;
}

bool Input::IsKeyPressed(int key, bool reset) {
    const auto state = key_map[key];
    const auto res =
        state.state == 1 &&
        state.last == 0;

    if (reset) {
        key_map[key].last = key_map[key].state;
    }

    return res;
}

bool Input::IsKeyReleased(int key) {
    const auto state = key_map[key];
    return
        state.state == 0 &&
        state.last == 1;
}

bool Input::IsKeyDown(int key) {
    const auto state = key_map[key];
    return state.state;
}

bool Input::IsKeyUp(int key) {
    return !IsKeyDown(key);
}

void Input::HandleEvent(sf::Event& event) {
    switch(event.type) {
        case sf::Event::KeyPressed: {
            const auto key = event.key.code;
            key_map[key] = KeyState{1, 0};
            break;
        }
        case sf::Event::KeyReleased: {
            const auto key = event.key.code;
            if (key_map.find(key) == key_map.end()) {
                key_map[key] = KeyState{1, 0};
            } else {
                key_map[key] = KeyState{0, 1};
            }
            break;
        }
        case sf::Event::MouseWheelMoved:
        {
            mouse_wheel_delta = event.mouseWheel.delta;
            break;
        }

        default: break;
    }
}
