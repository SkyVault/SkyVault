#include <iostream>
#include <sol.hpp>
#include "skyvault.h"
#include "game.h"
#include "graphics/art.h"
#include "graphics/assets.h"
#include "utilities/input.h"

std::unique_ptr<Art> Art::instance;
std::once_flag Art::onceFlag;

std::unique_ptr<Assets> Assets::instance;
std::once_flag Assets::onceFlag;

std::unique_ptr<Input> Input::instance;
std::once_flag Input::onceFlag;

#include "game.h"

int main() {
    std::cout << "Sky Vault Engine Version: " << SKYVAULT_VERSION << std::endl;

    let myGame = new Game();
    myGame->Run();
    
    return 0;
}
