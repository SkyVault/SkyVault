#include "game.h"
#include "graphics/assets.h"
#include "entities/entity.h"
#include "entities/components/body.h"
#include "entities/components/renderable.h"
#include "entities/components/player.h"
#include "entities/filters/physics_filter.h"
#include "entities/filters/render_filter.h"
#include "entities/filters/player_filter.h"
#include "utilities/input.h"
#include "graphics/animation.h"
#include "graphics/frame.h"
#include "skyvault.h"

#include <iostream>
#include <sstream>
#include <map>

Game::Game() {
    world = std::make_unique<EntityWorld>();
}

void Game::LoadContent() {
    window->setKeyRepeatEnabled(false);

    auto* texture = new sf::Texture();

    lua = std::make_shared<sol::state>();
    lua->open_libraries(sol::lib::base);
    Assets::It()->GiveLua(lua);
    Assets::It()->LoadPrefabs();
    Assets::It()->LoadAnimations();

    lua->script("print(\"Initialized \" .. _VERSION)");

    sol::table asset_data = lua->script_file("assets/data/asset_data.lua");
    if (!asset_data) {
        std::cout << "Failed to load the asset data script!\n";
        exit(0);
    }

    sol::table textures_data = asset_data["textures"];
    textures_data.for_each([&](sol::object const& key, sol::object const& value){
        const auto skey = key.as<std::string>(); 
        const auto spath = value.as<std::string>(); 

        std::cout << "---> " << skey << std::endl;

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

    camera = std::make_shared<Camera>(window->getSize().x, window->getSize().y);

    Assets::It()->Add("tiles", texture);

    world->Register<PhysicsFilter>();
    world->Register<RenderFilter>();
    world->Register<RenderAnimatedSpriteFilter>();
    world->Register<PlayerFilter>();

    {
        var player = world->Create();
        player->Add<Body>(sf::Vector2f(400, 800), sf::Vector2f(20 * 2, 48 * 2));
        player->Add<PhysicsBody>();
        player->Add<Player>();
        player->Add<Renderable>(texture, sf::IntRect(0, 0, 8, 8));
    }

    {
        Animation flight_anim(std::vector<Frame>{
                Frame(0, 0, 8, 8), 
                Frame(8, 0, 8, 8), 
                Frame(16, 0, 8, 8), 
                Frame(24, 0, 8, 8), 
                Frame(32, 0, 8, 8)
                });

        var bird = world->Create();
        var t = Assets::It()->Get<sf::Texture>("bird");

        bird->Add<Body>(sf::Vector2f(400 + 64, 800 + 64), sf::Vector2f(8*4, 8*4));
        bird->Add<PhysicsBody>();
        
        //bird->Add<Renderable>(t, sf::IntRect(0, 0, 8, 8));
        bird->Add<AnimatedSprite>(t, std::map<std::string, Animation>{
                {"flight", flight_anim} 
                });
    }

    camera->View.zoom(0.8f);

    map.loadFromFile("assets/maps/Dungeon_Room_2.tmx");
    map.setScale(2.0f, 2.0f);

    editor = std::make_unique<Editor>();
    editor->initUI(window, lua);
}

void Game::Update(const SkyTime& time) {
    // Update the whole game
    sky.Update(window->getSize().x, window->getSize().y, time);
    world->Update(time);

    auto p = world->GetFirstWith<Player>();
    if (p == nullptr) return;

    if (Input::It()->IsKeyPressed(sf::Keyboard::Enter)) {
        std::cout << "Enter!" << std::endl;
    }

    if (Input::It()->IsKeyReleased(sf::Keyboard::Enter)) {
        std::cout << "Released!" << std::endl;
    }

    //TODO(Dustin): Move this to the player filter
    //TODO(Dustin): Camera smoothing
    camera->View.setCenter(p->Get<Body>()->Center());
}

void Game::Render() {
    window->setView(window->getDefaultView());
    window->draw(sky);
    window->setView(camera->View);

    window->draw(map);
    world->Render(window);
    //let texture = Assets::It()->Get<sf::Texture>("tiles");
    //sprite.setTexture(texture);
    //sprite.setPosition(100, 100);
    //window->draw(sprite);
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

        editor->doUI(window, time);

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

    window->setPosition(sf::Vector2i(40, 80));

    LoadContent();

    RunLoop();

    DestroyContent();
}
