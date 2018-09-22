#include "game.h"
#include "graphics/assets.h"
#include <iostream>
#include <sstream>

Game::Game() {}

void Game::LoadContent() {
    sf::Texture texture;

    if (!texture.loadFromFile("assets/images/dungeon_tiles.png")){
        std::cout << "Could not load texture" << std::endl;
    } else {
        std::cout << "loaded texture" << std::endl;
    }

    Assets::It()->Add("tiles", texture);
}

void Game::Update() {
    let dt = clock.restart().asSeconds();
    var fps = 0;

    if (dt != 0.0) 
        fps = 1 / dt;

    // This is what we pass by constant reference to each loop that requires it
    Time time = {
        dt,
        fps, 
        timer,
        ticks
    };

    // Update the whole game

    // 

    ticks++;
    timer += dt;
}

void Game::Render() {
    var sprite = sf::Sprite();
    let texture = Assets::It()->Get<sf::Texture>("tiles");
    sprite.setTexture(texture);
    sprite.setPosition(100, 100);
    window->draw(sprite);
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
    window = std::make_shared<sf::RenderWindow>(
        sf::VideoMode(1280, 720),
        "DevWindow"
    );

    LoadContent();

    RunLoop();

    DestroyContent();
}
