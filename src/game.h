#ifndef SKYVAULT_GAME_H
#define SKYVAULT_GAME_H

#include <SFML/Graphics.hpp>
#include <memory>
#include "skyvault.h"
#include "entities/entity_world.h"
#include "graphics/tiled_map.h"
#include "skytime.h"

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
    std::unique_ptr<EntityWorld> world;

    bool running{false};

    sf::Clock clock;
    std::unique_ptr<sf::RenderWindow> window;

    float timer{0.0};
    long ticks{0};
};

#endif//SKYVAULT_GAME_H
