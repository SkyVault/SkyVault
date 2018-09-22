#ifndef SKYVAULT_GAME_H
#define SKYVAULT_GAME_H

#include <SFML/Graphics.hpp>
#include <memory>
#include "skyvault.h"
#include "graphics/tiled_map.h"

struct Time {
    float dt, fps, timer;
    long ticks;
};

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
    TiledMap map;

    bool running{false};

    sf::Clock clock;
    std::shared_ptr<sf::RenderWindow> window;

    float timer{0.0};
    long ticks{0};
};

#endif//SKYVAULT_GAME_H
