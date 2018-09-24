#include "game.h"
#include "graphics/assets.h"
#include "entities/entity.h"
#include "entities/components/body.h"
#include "entities/components/renderable.h"
#include "entities/filters/physics_filter.h"
#include "entities/filters/render_filter.h"

#include <iostream>
#include <sstream>

Game::Game() {
    world = std::make_unique<EntityWorld>();
}

void Game::LoadContent() {
    auto* texture = new sf::Texture();

    if (!texture->loadFromFile("assets/images/dungeon_tiles.png")){
        std::cout << "Could not load texture" << std::endl;
    } else {
        std::cout << "loaded texture" << std::endl;
    }

    Assets::It()->Add("tiles", texture);

    world->Register<PhysicsFilter>();
    world->Register<RenderFilter>();

    var entity = world->Create();
    entity->Add<Body>(sf::Vector2f(10, 10), sf::Vector2f(100, 100));
    entity->Add<PhysicsBody>();
    entity->Add<Renderable>(texture, sf::IntRect(0, 0, 8, 8));

    //map.loadFromFile("assets/maps/Dungeon_Room_2.tmx");
}

void Game::Update() {
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

    // Update the whole game
    world->Update(time);
    // 

    ticks++;
    timer += dt;
}

void Game::Render() {
    world->Render(window);
    //let texture = Assets::It()->Get<sf::Texture>("tiles");
    //sprite.setTexture(texture);
    //sprite.setPosition(100, 100);
    //window->draw(sprite);
    //window->draw(map);
}

void Game::RunLoop() {
    running = true;

    while (running) {
        running = window->isOpen();
        
        sf::Event event;
        while (window->pollEvent(event)) {
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

        Update();

        window->clear();
        Render();
        window->display();
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

    LoadContent();

    RunLoop();

    DestroyContent();
}
