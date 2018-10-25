#include "level_layer.h"
#include <iostream>

void LevelLayer::Load(){
    std::cout << "Level Loaded" << std::endl; 

    Atlas = { "Water", "Water", "Water", "Water", "Water"
            , "Water", "Water", "Water", "Water", "Water"
            , "Water", "Water", "Water",   "Water", "Water"
            , "Water", "Water", "Water", "Water", "Water"
            , "Water", "Water", "Water", "Water", "Water"
            };
    
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

    auto player = world->GetFirstWith<Player>();
    if (player == nullptr) {
        return;
    }

    auto body = player->Get<Body>();
    const auto xx = static_cast<int>(((body->Position.x + (MAP_SIZE_PIXELS * 2)) / static_cast<float>(MAP_SIZE_PIXELS)));
    const auto yy = static_cast<int>(((body->Position.y + (MAP_SIZE_PIXELS * 2)) / static_cast<float>(MAP_SIZE_PIXELS))); 

    globalPosition = sf::Vector2i(xx, yy);

}

void LevelLayer::Render(std::unique_ptr<sf::RenderWindow>& window){
    const auto [xx, yy] = globalPosition;

    if (xx >= 0 && yy >= 0){
        const auto map_tile = Atlas[xx + yy * AtlasSize];

        if (map_tile == "Water") {
            //
        } else {
            window->draw(sky);
            window->draw(map); 
        }
        //std::cout << map_tile << std::endl;

    }
}

void LevelLayer::Destroy(){

}