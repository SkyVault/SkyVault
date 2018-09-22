#include <iostream>
#include "skyvault.h"
#include "game.h"

int main() {
    std::cout << "Sky Vault Engine Version: " << SKYVAULT_VERSION << std::endl;

    let myGame = new Game();
    myGame->Run();
    
    return 0;
}
