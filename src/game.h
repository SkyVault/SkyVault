#ifndef SKYVAULT_GAME_H
#define SKYVAULT_GAME_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <sol.hpp>
#include <string>
#include <mutex>
#include <vector>
#include <map>
#include <tuple>

#include "skyvault.h"
#include "entities/entity_world.h"
#include "graphics/tiled_map.h"
#include "graphics/camera.h"
#include "graphics/tween.h"
#include "graphics/art.h"
#include "graphics/sky.h"
#include "graphics/gui.h"
#include "skytime.h"

#include "editor/editor.h"
#include "game_state.h"

struct LerpCircle {
    sf::CircleShape circle;
    float x;
};

struct Game {
    Game();
    inline ~Game() { this->DestroyContent(); }

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
    std::shared_ptr<GUI> gui;

    bool running{false};
    bool debugging{false};

    sf::Clock clock;
    std::unique_ptr<Editor> editor;
    std::unique_ptr<sf::RenderWindow> window;

    std::shared_ptr<Sky> sky;
    std::shared_ptr<TiledMap> tiledMap;

    float timer{0.0};
    long ticks{0};

    float TweenTestFloat = 3.0f;

    std::vector<LerpCircle*> lerp_circles;
};

#endif//SKYVAULT_GAME_H
