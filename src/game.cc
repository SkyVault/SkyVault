#include "game.h"
#include "graphics/assets.h"
#include "entities/entity.h"
#include "entities/components/body.h"
#include "entities/components/renderable.h"
#include "entities/components/player.h"
#include "entities/filters/physics_filter.h"
#include "entities/filters/render_filter.h"
#include "entities/filters/player_filter.h"
#include "skyvault.h"

#ifdef EDITOR
#include "imgui.h"
#include "imgui-SFML.h"
#endif

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
    world->Register<PlayerFilter>();

    var entity = world->Create();
    entity->Add<Body>(sf::Vector2f(10, 10), sf::Vector2f(100, 100));
    entity->Add<PhysicsBody>();
    entity->Add<Player>();
    entity->Add<Renderable>(texture, sf::IntRect(0, 0, 8, 8));

    map.loadFromFile("assets/maps/Dungeon_Room_2.tmx");
    map.setScale(2.0f, 2.0f);

#ifdef EDITOR
    ImGui::SFML::Init(*window);
#endif
}

void Game::Update(const SkyTime& time) {
    // Update the whole game
    world->Update(time);
    // 
}

void Game::Render() {
    world->Render(window);
    //let texture = Assets::It()->Get<sf::Texture>("tiles");
    //sprite.setTexture(texture);
    //sprite.setPosition(100, 100);
    //window->draw(sprite);
    window->draw(map);
}

void Game::RunLoop() {
    running = true;

    while (running) {
        running = window->isOpen();
        
        sf::Event event;
        while (window->pollEvent(event)) {
#ifdef EDITOR
            ImGui::SFML::ProcessEvent(event);
#endif
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
        Render();

#ifdef EDITOR
        ImGui::SFML::Update(*window, editorClock.restart());
        ImGui::Begin("Sample window"); // begin window

        if (ImGui::Button("Hello World")) {
            printf("Hey!!\n");
        }

        ImGui::LabelText("Timing", "FPS: %f DT: %f", time.fps, time.dt);

        ImGui::End();

        // Render the editor
        ImGui::SFML::Render(*window);
#endif

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

    LoadContent();

    RunLoop();

    DestroyContent();
}
