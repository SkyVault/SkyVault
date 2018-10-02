#ifndef SKYVAULT_GAME_H
#define SKYVAULT_GAME_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <sol.hpp>
#include <string>
#include "skyvault.h"
#include "entities/entity_world.h"
#include "graphics/tiled_map.h"
#include "graphics/camera.h"
#include "graphics/sky.h"
#include "skytime.h"

#include "editor/editor.h"

struct Game {
    Game();

    void LoadContent();
    void DestroyContent();

    void Update(const SkyTime& time);
    void Render();

    void RunLoop();

    void Run();

    inline bool Running() { return running; }

private:
    TiledMap map;
    std::unique_ptr<EntityWorld> world;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<sol::state> lua;

    Sky sky;

    bool running{false};

    sf::Clock clock;
    std::unique_ptr<Editor> editor;

    std::unique_ptr<sf::RenderWindow> window;

    float timer{0.0};
    long ticks{0};
};

#endif//SKYVAULT_GAME_H
