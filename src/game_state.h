#ifndef SKYVAULT_GAMESTATE_H
#define SKYVAULT_GAMESTATE_H

#include <memory>
#include <mutex>

class GameState {
	GameState(const GameState&) = delete;
	GameState& operator=(const GameState&) = delete;

	static std::unique_ptr<GameState> instance;
	static std::once_flag onceFlag;

    GameState() { }

public:
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

private:
    bool debugging{false};
    bool fullEditor{false};
    bool noClip{false};
};
#endif//SKYVAULT_GAMESTATE_H
