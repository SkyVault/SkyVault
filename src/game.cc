#include "game.h"

Game::Game() {}

void Game::LoadContent() {

}

void Game::Update() {

}

void Game::Render() {

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
