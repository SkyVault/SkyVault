#include "game.h"
#include "graphics/assets.h"
#include "entities/entity.h"
#include "entities/components/body.h"
#include "entities/components/renderable.h"
#include "entities/components/player.h"
#include "entities/filters/physics_filter.h"
#include "entities/filters/render_filter.h"
#include "entities/filters/player_filter.h"
#include "entities/filters/ai_filter.h"
#include "entities/filters/once_off_filter.h"
#include "utilities/input.h"
#include "utilities/quest_engine.h"
#include "graphics/animation.h"
#include "graphics/frame.h"
#include "layers/menu_layer.h"
#include "skyvault.h"

#include <iostream>
#include <sstream>
#include <map>

Game::Game() {
    world = std::make_shared<EntityWorld>();
    gui = std::make_shared<GUI>();
}

void Game::LoadContent() {
    window->setKeyRepeatEnabled(false);

    auto* texture = new sf::Texture();

    lua         = std::make_shared<sol::state>();
    sky         = std::make_shared<Sky>();
    tiledMap    = std::make_shared<TiledMap>();

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
    Assets::It()->LoadDialogs();
    Assets::It()->LoadAnimations();
    //Assets::It()->LoadTextures();

    lua->script("print(\"Initialized \" .. _VERSION)");

    lua->script(R"(
        function getTableAddress(t)
            return tostring(t)
        end
    )");

    lua->script(R"(
        function serialize(t)
            assert(type(t) == "table")
            return serpent.block(t, {comment = false})
        end
    )");

    lua->script(R"(
        function removeIfMatchingAddress(source, test)
            for i = 1, #source do
                if test == tostring(source[i]) then
                    table.remove(source, i)
                    return true
                end
            end
            return false
        end
    )");

    // Set up the quest engine in lua
    // TODO(Dustin): Move this somewhere else
    (*lua)["QuestEngine_StartQuest"] = [](std::string str){
        QuestEngine::It()->StartQuest(str);
    };

    (*lua)["QuestEngine_WorkingOnQuest"] = [](std::string str) {
        return QuestEngine::It()->WorkingOnQuest(str);
    };

    (*lua)["QuestEngine_IsCompleted"] = [](std::string str) {
        return QuestEngine::It()->IsCompleted(str);
    };

    (*lua)["Gui_DoDialog"] = [&](std::string which) {
        gui->DoDialog(Assets::It()->GetDialog(which));
    };

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
    {
        auto* font = new sf::Font();
        if (!font->loadFromFile("assets/fonts/arcade.ttf")){
            std::cout << "Failed to load the arcade font\n";
        }
        const_cast<sf::Texture&>(font->getTexture(42)).setSmooth(false);
        Assets::It()->Add("arcade", font);
    }

    {
        auto* font = new sf::Font();
        if (!font->loadFromFile("assets/fonts/DejaVuSans.ttf")){
            std::cout << "Failed to load the DejaVuSans font\n";
        }
        //const_cast<sf::Texture&>(font->getTexture(42)).setSmooth(false);
        Assets::It()->Add("dialog", font);
    }

    camera = std::make_shared<Camera>(window->getSize().x, window->getSize().y);

    Assets::It()->Add("tiles", texture);

    world->Register<PhysicsFilter>(world);
    world->Register<RenderFilter>();
    world->Register<RenderAnimatedSpriteFilter>();
    world->Register<PlayerFilter>(camera, world, lua, sky);
    world->Register<AIFilter>(world);
    world->Register<OnceOffFilter>(world);

    GameState::It()->PushLayer(new MenuLayer(world, camera, lua, gui, sky, tiledMap));

    camera->View.zoom(0.5f);

    editor = std::make_unique<Editor>();
    editor->initUI(window, lua);
}

void Game::Update(const SkyTime& time) {
    // Update the whole game
    world->Update(time);
    gui->Update(time);
    Tween::It()->Update(time);
    GameState::It()->Update(time);
    QuestEngine::It()->Update(time, world);
    tiledMap->Update(time);

    auto p = world->GetFirstWith<Player>();
    if (p == nullptr) return;

    //TODO(Dustin): Get this working on linux and windows
    if (sf::Joystick::isConnected(0)) {
    }

    if (sf::Joystick::isButtonPressed(0, 0)) {
    }

    if (Input::It()->IsKeyDown(sf::Keyboard::LControl) &&
        Input::It()->IsKeyPressed(sf::Keyboard::E)) {

        GameState::It()->ToggleFullEditor();
    }

    if (GameState::It()->FullEditor() == false){
        sf::IntRect rect;
        rect.left = 0;
        rect.top = 0;
        rect.width = tiledMap->WidthInPixels();
        rect.height = tiledMap->HeightInPixels();
        camera->Clamp(rect);
    }
}

void Game::Render() {
    window->setView(camera->View);
    {
        GameState::It()->Render(window);
        world->Render(window);
        Art::It()->Flush(window);

        if (GameState::It()->FullEditor())
            editor->Draw(window, tiledMap, world);
    }
    window->setView(window->getDefaultView());

#if 0
    for (auto* c : lerp_circles) {
        c->circle.setPosition(c->x, c->circle.getPosition().y);
        window->draw(c->circle);
    }
#endif//

    gui->Render(window);
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
        Render();

        editor->doUI
            ( window
            , time
            , world
            , sky
            , tiledMap
            , camera
            );

        window->display();

        ticks++;
        timer += dt;
    }
}

void Game::DestroyContent() {
    editor->Destroy();
}

void Game::Run() {
    // Create and initialize the window
    window = std::make_unique<sf::RenderWindow>(
        sf::VideoMode(1280, 720),
        "DevWindow"
    );

    window->setFramerateLimit(60);
    window->setPosition(sf::Vector2i(30, 30));
    GameState::It()->SetWindowSize(window->getSize());

    LoadContent();

    RunLoop();

    DestroyContent();

    //TODO(Dustin): @Refactor
    // Lame that we have to call this here, but the destructor
    // does not get called at the end of the program if
    // force killed
    tiledMap->Destroy();

}
