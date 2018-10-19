#include "game.h"
#include "graphics/assets.h"
#include "entities/entity.h"
#include "entities/components/body.h"
#include "entities/components/renderable.h"
#include "entities/components/player.h"
#include "entities/filters/physics_filter.h"
#include "entities/filters/render_filter.h"
#include "entities/filters/player_filter.h"
#include "entities/filters/interaction_filter.h"
#include "utilities/input.h"
#include "graphics/animation.h"
#include "graphics/frame.h"
#include "layers/menu_layer.h"
#include "skyvault.h"

#include <iostream>
#include <sstream>
#include <map>

Game::Game() {
    world = std::make_shared<EntityWorld>();
}

void Game::LoadContent() {
    Atlas = { "Water", "Water", "Water", "Water", "Water"
            , "Water", "Water", "Water", "Water", "Water"
            , "Water", "Water", "Map",   "Water", "Water"
            , "Water", "Water", "Water", "Water", "Water"
            , "Water", "Water", "Water", "Water", "Water"
            };

    window->setKeyRepeatEnabled(false);

    auto* texture = new sf::Texture();

    lua = std::make_shared<sol::state>();

    lua->open_libraries
        ( sol::lib::base
        , sol::lib::math
        , sol::lib::string
        , sol::lib::table
        , sol::lib::package // require
        );

    //Load serpent -- (table serializer)
    sol::table serpent = lua->script_file("assets/scripts/serpent.lua");
    lua->set("serpent", serpent);

    Assets::It()->GiveLua(lua);
    Assets::It()->LoadPrefabs();
    Assets::It()->LoadAnimations();
    //Assets::It()->LoadTextures();

    lua->script("print(\"Initialized \" .. _VERSION)");

    sol::table asset_data = lua->script_file("assets/data/asset_data.lua");
    if (!asset_data) {
        std::cout << "Failed to load the asset data script!\n";
        exit(0);
    }

    //TODO(Dustin): Refactor into the assets singleton
    sol::table textures_data = asset_data["textures"];
    textures_data.for_each([&](sol::object const& key, sol::object const& value){
        const auto skey = key.as<std::string>(); 
        const auto spath = value.as<std::string>(); 

        auto* texture = new sf::Texture();
        if (!texture->loadFromFile(spath)) {
            std::cout << "Failed to load texture: " << spath << std::endl;
            delete texture;
            return;
        }
        Assets::It()->Add(skey, texture);
    });

    if (!texture->loadFromFile("assets/images/dungeon_tiles.png")){
        std::cout << "Could not load texture" << std::endl;
    } else {
        std::cout << "loaded texture" << std::endl;
    }

    //TODO(Dustin): Refactor into the assets singleton
    auto* font = new sf::Font();
    if (!font->loadFromFile("assets/fonts/arcade.ttf")){
        std::cout << "Failed to load the arcade font\n";
    }
    const_cast<sf::Texture&>(font->getTexture(42)).setSmooth(false);

    Assets::It()->Add("arcade", font);

    camera = std::make_shared<Camera>(window->getSize().x, window->getSize().y);

    Assets::It()->Add("tiles", texture);

    world->Register<PhysicsFilter>();
    world->Register<RenderFilter>();
    world->Register<RenderAnimatedSpriteFilter>();
    world->Register<PlayerFilter>();
    world->Register<InteractionFilter>();

    GameState::It()->PushLayer(new MenuLayer(world, camera, lua));

    camera->View.zoom(0.5f);

    editor = std::make_unique<Editor>();
    editor->initUI(window, lua);
}

void Game::Update(const SkyTime& time) {
    // Update the whole game
    world->Update(time);
    GameState::It()->Update(time);

    auto p = world->GetFirstWith<Player>();
    if (p == nullptr) return;

    if (sf::Joystick::isConnected(0)) {
    }

    if (sf::Joystick::isButtonPressed(0, 0)) {
        std::cout << "Here" << std::endl;
    }

    //TODO(Dustin): Move this to the player filter
    //TODO(Dustin): Camera smoothing
    camera->View.setCenter(p->Get<Body>()->Center());
}

void Game::Render() {
    window->setView(window->getDefaultView());
    window->setView(camera->View);

    world->Render(window);
    GameState::It()->Render(window);
    Art::It()->Flush(window);
}

void Game::RunLoop() {
    running = true;

    while (running) {
        running = window->isOpen();
        
        Input::It()->Update();

        sf::Event event;
        while (window->pollEvent(event)) {

            editor->processEvent(event);
            Input::It()->HandleEvent(event);

            switch (event.type) {
                case sf::Event::Closed: {
                    running = false; 
                    window->close();
                    break;                             
                }
                default:
                    break;
            }
        }

        let dt = clock.restart().asSeconds();
        var fps = 0.f;

        if (dt != 0.0) 
            fps = 1.f / dt;

        // This is what we pass by constant reference to each loop that requires it
        SkyTime time = {
            dt,
            fps, 
            timer,
            ticks
        };

        Update(time);

        window->clear();
        window->setView(camera->View);
        Render();

        editor->doUI(window, time, world);

        window->display();

        ticks++;
        timer += dt;
    } 
}

void Game::DestroyContent() {

}

void Game::Run() {
    // Create and initialize the window
    window = std::make_unique<sf::RenderWindow>(
        sf::VideoMode(1280, 720),
        "DevWindow"
    );

    window->setPosition(sf::Vector2i(400, 80));

    LoadContent();

    RunLoop();

    DestroyContent();
}
