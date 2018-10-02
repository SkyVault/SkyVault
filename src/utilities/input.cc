#include "input.h"

void Input::Update() {
    for (auto& [key, state] : key_map) {
        state.last = state.state;
    }

    
}

bool Input::IsKeyPressed(int key) {
    const auto state = key_map[key];
    return
        state.state == 1 &&
        state.last == 0;
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
        default: break;
    }
}
