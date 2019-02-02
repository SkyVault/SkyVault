#include "level_layer.h"
#include <iostream>

void LevelLayer::Load(){
    std::cout << "Level Loaded" << std::endl;

    Atlas = { "Water", "Water", "Water", "Water", "Water"
            , "Water", "Water", "Water", "Water", "Water"
            , "Water", "Water", "Map", "Water", "Water"
            , "Water", "Water", "Water", "Water", "Water"
            , "Water", "Water", "Water", "Water", "Water"
            };

    // Load the sky
    auto [x, y] = GameState::It()->GetWindowSize();
    sky->Load(x, y);

    auto* physics_filter = world->GetFilter<PhysicsFilter>();

    tiledMap->Destroy();
    tiledMap->loadFromFile("assets/maps/VarianTown.tmx", physics_filter, world, lua);
    tiledMap->setScale(1.0f, 1.0f);

    {
        var player = world->Create();
        player->Add<Body>(sf::Vector2f(500, 400), sf::Vector2f(16, 16));
        player->Add<PhysicsBody>();
        player->Add<Player>();
        player->Add<Renderable>(Assets::It()->Get<sf::Texture>("tes"), sf::IntRect(0, 0, 8*2, 16*2), sf::Vector2f(0, -8*2));
    }

    {
        var test_emitter = world->Create();
        test_emitter->Add<Body>(sf::Vector2f(500 + 64, 400), sf::Vector2f(16, 16));
        test_emitter->Add<ParticleEmitter>();
    }

    {
        var other = world->Create();
        other->AddTags("Enemy");
        other->Add<Body>(sf::Vector2f(500 + 32, 400 - 32), sf::Vector2f(32,16));
        other->Add<PhysicsBody>();
        other->Add<Renderable>(Assets::It()->Get<sf::Texture>("enemies"), sf::IntRect(0, 16, 32, 32), sf::Vector2f(0, -16));
        other->Add<AI>(BasicEnemyAI);
    }

#if 0
    {
        var laser = world->Create();
        laser->Add<Body>(sf::Vector2f(500 + 128 + 64, 400 + 128), sf::Vector2f(32 * 0.8f, 56 * 0.8f));
        laser->Add<Laser>(LaserColor::Green);
        laser->Add<PhysicsBody>();
        laser->Add<Renderable>(Assets::It()->Get<sf::Texture>("moveable_block"), sf::IntRect(0, 0, 32, 56), sf::Vector2f(0, 0));
        laser->Get<Renderable>()->GetSprite().setScale(0.8f, 0.8f);
    }
#endif//

    {
        var box = world->Create();
        box->Add<Body>(sf::Vector2f(500 + 128 + 64, 400 + 128), sf::Vector2f(48, 48));
        box->Add<AnimatedSprite>(
                Assets::It()->Get<sf::Texture>("blender_test"),
                std::map<std::string, Animation>{{"rotate",
                    Animation(std::vector<Frame>{
                        Frame(48 * 0, 0, 48, 48),
                        Frame(48 * 1, 0, 48, 48),
                        Frame(48 * 2, 0, 48, 48),
                        Frame(48 * 3, 0, 48, 48),
                    })
                }});
        box->Add<PhysicsBody>();
    }

    {
        var craigory = world->Create();
        craigory->Add<Body>(sf::Vector2f(470, 485), sf::Vector2f(43, 37));
        craigory->Add<ShopKeeper>();
    }

    auto cursor = 0.0f;
    for (auto tag : {"BlueDiamond", "Heart", "Potion", "FloppyDisk"}) {
        const auto e = world->Create(Assets::It()->GetPrefab(tag));
        e->Get<Body>()->Position = sf::Vector2f(500+128 + (cursor++*16), 400);
    }

    world->OnRoomChange([&](const std::string& to, const std::string& uuid) {
        std::cout << "Changing rooms to... " << to << std::endl;
        // NOTE(Dustin): Were passing in nullptrs because the smart pointers go out of scope
        // and are no longer accessable, to fix we must store the physics_filter world lua objects

        auto* _physics_filter = world->GetFilter<PhysicsFilter>();

        tiledMap->Destroy();
        world->ClearDoors();

        tiledMap->loadFromFile("assets/maps/" + to, _physics_filter, world, lua);

        // Move the player to the door that matches the uuid
        auto& doors = world->GetDoors();

        for (auto& door : doors) {
            if (door.Uuid == uuid) {

                // Ensures that when we spawn inside the door
                // it doesn't send us back to the prevous room

                door.LastToTrue();

                const auto player_opt = world->GetFirstWith<Player>();

                if (!player_opt) { return; }

                auto player = player_opt.value();
                auto body = player->Get<Body>();
                body->Position = door.Position + door.Size / 2.0f - body->Size / 2.0f;
            }
        }
    });
}
void LevelLayer::Update(const SkyTime& time){
    auto [x, y] = GameState::It()->GetWindowSize();
    sky->Update(x, y, time);

    auto player_opt = world->GetFirstWith<Player>();
    if (!player_opt) {
        return;
    }

    auto player = player_opt.value();

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
            // Unproject the sky
            window->setView(window->getDefaultView());
                window->draw(*sky);
            window->setView(camera->View);

            window->draw(*tiledMap);
        }
        //std::cout << map_tile << std::endl;

    }
}

void LevelLayer::Destroy(){

}
