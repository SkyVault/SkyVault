#ifndef SKYVAULT_GAMESTATE_H
#define SKYVAULT_GAMESTATE_H

#include <vector>
#include <memory>
#include <mutex>
#include <iostream>
#include "layers/layer.h"

// NOTE(Dustin): To keep the number of singletons small
// were also going to use this class as the layer manager.

class GameState {
	GameState(const GameState&) = delete;
	GameState& operator=(const GameState&) = delete;

	static std::unique_ptr<GameState> instance;
	static std::once_flag onceFlag;

    GameState() { }
public:
    ~GameState() {
        for(auto* s : layer_stack) {
            if (s) {
                s->Destroy();
                delete s;
                s = nullptr;
            }
        }
    }


#ifndef MACROSTR
#define MACROSTR(k) #k
#endif

#define X_GAME_STATES\
	X(PLAYING_STATE)\
	X(PAUSED_STATE)\
	X(DIALOG_STATE)\
    X(CUTSCENE_STATE)\
    X(COMBAT_STATE)

    enum States {
#define X(Enum) Enum,
     X_GAME_STATES
#undef X
    };

    const std::vector<std::string> StatesNames {
#define X(String) MACROSTR(String),
        X_GAME_STATES
#undef X
    };

    static auto* It() {
        std::call_once(GameState::onceFlag, [] () {
            instance.reset(new GameState());     
        });

        return instance.get();
    }

    inline void ToggleDebug(){ debugging = !debugging; }
    inline bool IsDebug() { return debugging; }

    inline void ToggleFullEditor() { fullEditor = !fullEditor; }
    inline bool FullEditor(){return fullEditor;}

    inline void ToggleNoClip() { noClip = !noClip; }
    inline bool IsNoClip(){return noClip;}

    // If this returns true, the player and ai are allowed to move and
    // control the camera, else the engine will override that control.
    inline bool NormalMode() { 
        return 
            CurrentState != GameState::States::CUTSCENE_STATE   &&
            CurrentState != GameState::States::COMBAT_STATE;
    }

    inline void PushLayer(Layer* layer, bool dontDestroy=false) {
        if (layer_stack.size() > 0 && !dontDestroy)
            layer_stack[layer_stack.size()-1]->Destroy();
        layer_stack.insert(layer_stack.begin(), layer);
        layer->Load();
    }

    //NOTE(Dustin): We dont destroy or create any memory
    inline Layer* PopLayer() {
        if (layer_stack.size() == 0) {
            std::cout << "GameState::PopLayer:: Layer stack is empty!" << std::endl;
            return nullptr;
        }
        auto* layer = layer_stack[layer_stack.size()-1];
        layer_stack.pop_back();
        return layer;
    }

    inline void Update(const SkyTime& time) {
        if (layer_stack.size() == 0) return;
        layer_stack[0]->Update(time);
    }

    inline void Render(std::unique_ptr<sf::RenderWindow>& window) {
        window_size = window->getSize();
        if (layer_stack.size() == 0) return;

        layer_stack[0]->Render(window);
    }

    inline sf::Vector2u GetWindowSize() { return window_size; }
    inline void SetWindowSize(const sf::Vector2u& size) { window_size = size; }

    States CurrentState {PLAYING_STATE};

private:
    bool debugging{false};
    bool fullEditor{false};
    bool noClip{false};

    sf::Vector2u window_size;

    std::vector<Layer*> layer_stack; // @RawPtr
};
#endif//SKYVAULT_GAMESTATE_H
