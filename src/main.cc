#include <iostream>
#include "skyvault.h"
#include "graphics/assets.h"

std::unique_ptr<Assets> Assets::instance;
std::once_flag Assets::onceFlag;

#include "game.h"

int main() {
    std::cout << "Sky Vault Engine Version: " << SKYVAULT_VERSION << std::endl;

    let myGame = new Game();
    myGame->Run();
    
    return 0;
}
