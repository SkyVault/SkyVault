#ifndef SKYVAULT_GAME_H
#define SKYVAULT_GAME_H

#include <SFML/Graphics.hpp>
#include <memory>

struct Game {
    Game();

    void LoadContent();
    void DestroyContent();

    void Update();
    void Render();

    void RunLoop();

    void Run();

    inline bool Running() { return running; }

private:
    bool running{false};

    std::shared_ptr<sf::RenderWindow> window;
};

#endif//SKYVAULT_GAME_H
