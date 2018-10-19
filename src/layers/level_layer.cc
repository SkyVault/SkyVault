#include "level_layer.h"
#include <iostream>

void LevelLayer::Load(){
    std::cout << "Level Loaded" << std::endl; 
    
    // Load the sky
    auto [x, y] = GameState::It()->GetWindowSize();
    sky.Load(x, y);

    auto* physics_filter = world->GetFilter<PhysicsFilter>();
    map.loadFromFile("assets/maps/Dungeon_Room_2.tmx", physics_filter);
    map.setScale(1.0f, 1.0f);

    {
        var player = world->Create();
        player->Add<Body>(sf::Vector2f(500, 400+16), sf::Vector2f(16, 16));
        player->Add<PhysicsBody>();
        player->Add<Player>();
        player->Add<Renderable>(Assets::It()->Get<sf::Texture>("tes"), sf::IntRect(0, 0, 8*2, 16*2), sf::Vector2f(0, -8*2));
    }

    {
        var other = world->Create();
        other->Add<Body>(sf::Vector2f(500, 400), sf::Vector2f(32,16));
        other->Add<PhysicsBody>();
        other->Add<Renderable>(Assets::It()->Get<sf::Texture>("enemies"), sf::IntRect(0, 16, 32, 32), sf::Vector2f(0, -16));
    }

    {
        var other = world->Create();
        other->Add<Body>(sf::Vector2f(500+32, 400-64), sf::Vector2f(32,16));
        other->Add<PhysicsBody>();
        other->Add<Renderable>(Assets::It()->Get<sf::Texture>("enemies"), sf::IntRect(64, 16, 32, 32), sf::Vector2f(0, -16));
    }
}
void LevelLayer::Update(const SkyTime& time){
    auto [x, y] = GameState::It()->GetWindowSize();
    sky.Update(x, y, time);
}

void LevelLayer::Render(std::unique_ptr<sf::RenderWindow>& window){
    window->draw(sky);
    window->draw(map);
}

void LevelLayer::Destroy(){

}
