#ifndef SKYVAULT_INPUT_H
#define SKYVAULT_INPUT_H

#include <map>
#include <SFML/Graphics.hpp>
#include <string>
#include <type_traits>
#include <sol.hpp>
#include <iostream>

#include <mutex>

/*
TODO(Dustin):
- [ ] Mouse support
    * [ ] Is mouse left/right down
    * [ ] Is mouse left/right pressed
    * [ ] Is mouse left/right released
    * [ ] Is mouse left/right not down

- [ ] Controller / Gamepad support
*/

#ifndef MACROSTR
#define MACROSTR(k) #k
#endif

#define X_ACTION_TYPES\
	X(MOVEMENT)\
	X(ATTACK)\
	X(INTERACT)

enum  ActionTypes {
#define X(Enum) Enum,
     X_ACTION_TYPES
#undef X
};

const static std::vector<std::string> ActionTypesNames = {
#define X(String) MACROSTR(String),
    X_ACTION_TYPES
#undef X
};

struct KeyState { int state, last; };
struct MouseState { int state, last; };

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

    enum Mouse {
        MOUSE_LEFT,
        MOUSE_RIGHT,
        MOUSE_MIDDLE
    };

    static auto* It() {
        std::call_once(Input::onceFlag, [] () {
            instance.reset(new Input());     
        });

        return instance.get();
    }

    void Update();
    void HandleEvent(sf::Event& event);

    bool IsKeyPressed(int key, bool reset=false);
    bool IsKeyReleased(int key);
    bool IsKeyDown(int key);
    bool IsKeyUp(int key);

    bool IsMouseLeftDown(int mouse_button);
    bool IsMouseLeftPressed(int mouse_button);

    bool IsMouseLeftUp(int mouse_button);
    bool IsMouseLeftReleased(int mouse_button); 

    sf::Vector2f GetMovementAxis() {
        // NOTE(Dustin): This can be done much more efficiently
        // but I dont know if its worth it at the moment.
        
        auto left = IsKeyDown(sf::Keyboard::Left);
        auto right = IsKeyDown(sf::Keyboard::Right);
        auto up = IsKeyDown(sf::Keyboard::Up);
        auto down = IsKeyDown(sf::Keyboard::Down);
        // Check joystick
        
        sf::Vector2f joyStickDir(0, 0);

        sf::Vector2f keyDir(0, 0); 
        if (left)    keyDir.x = -1;
        if (right)   keyDir.x =  1;
        if (up)      keyDir.y = -1;
        if (down)    keyDir.y =  1;

        auto add = keyDir + joyStickDir;
        auto len = sqrtf(add.x*add.x + add.y*add.y);

        if (len == 0) return sf::Vector2f(0, 0);

        return add / len; // + joyStickDir;
    }

private:

    std::map<int, KeyState> key_map;

    MouseState left_mouse_button;
    MouseState right_mouse_button;
    MouseState middle_mouse_button;

    //std::map<ActionTypes, int> keyboard_mappings; // Used for remapping of keys and buttons
    //std::map<ActionTypes, int> button_mappings;
};

#endif//SKYVAULT_INPUT_H
