#include <iostream>
#include <sol.hpp>
#include "skyvault.h"
#include "game.h"
#include "graphics/art.h"
#include "graphics/assets.h"
#include "utilities/input.h"
#include "game_state.h"
#include "utilities/quest_engine.h"

/*
TODO(Dustin): Global todos that we need to get done before release
    
- [O] Camera
    * [X] Clamp camera to room
    * [X] Smooth camera follow
    * [X] Pan camera from editor 
- [ ] Graphics
    * [ ] Simple phong shading
    * [ ] Water distortion
    * [ ] GUI
        + [ ] Button
        + [ ] Layout
        + [ ] List
        + [ ] Grid
- [ ] Gameplay
    * [ ] Draggable blocks
    * [ ] Trigger zones (for the blocks and player)
    * [ ] Combat
        + [ ] Figure out what type of combat were going to use...
 */

std::unique_ptr<GameState> GameState::instance;
std::once_flag GameState::onceFlag;

std::unique_ptr<Art> Art::instance;
std::once_flag Art::onceFlag;

std::unique_ptr<Assets> Assets::instance;
std::once_flag Assets::onceFlag;

std::unique_ptr<Input> Input::instance;
std::once_flag Input::onceFlag;

std::unique_ptr<QuestEngine> QuestEngine::instance;
std::once_flag QuestEngine::onceFlag;

#include "game.h"

int main() {
    std::cout << "Sky Vault Engine Version: " << SKYVAULT_VERSION << std::endl;

    let myGame = new Game();
    myGame->Run();

    return 0;
}
