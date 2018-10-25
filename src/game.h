#ifndef SKYVAULT_GAME_H
#define SKYVAULT_GAME_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <sol.hpp>
#include <string>
#include <mutex>
#include <vector>
#include <map>

#include "skyvault.h"
#include "entities/entity_world.h"
#include "graphics/tiled_map.h"
#include "graphics/camera.h"
#include "graphics/art.h"
#include "skytime.h"

#include "editor/editor.h"
#include "game_state.h"

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
    std::shared_ptr<EntityWorld> world;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<sol::state> lua;

    bool running{false};
    bool debugging{false};

    sf::Clock clock;
    std::unique_ptr<Editor> editor;

    std::unique_ptr<sf::RenderWindow> window;

    float timer{0.0};
    long ticks{0};
};

#endif//SKYVAULT_GAME_H
