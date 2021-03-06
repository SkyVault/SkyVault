#include "editor.h"
#include "../game_state.h"
#include <string.h>
#include <sstream>

void Editor::Destroy() {
    editor_save_data["is_open"] = GameState::It()->FullEditor();
    std::cout << "Serializing out the editors save data" << std::endl;

    auto result = (*lua)["serialize"](editor_save_data);

    if (result.valid()) {
        std::string out = result.get<std::string>(0);
        std::ofstream ostream("assets/data/editor_save_data.lua");
        ostream << "return " << out << std::endl;
        ostream.close();
    }
}

void Editor::initUI(std::unique_ptr<sf::RenderWindow> &window, std::shared_ptr<sol::state>& lua) {
    ImGui::SFML::Init(*window);
    this->lua = lua;
    this->lua->set_function("print", [](const char *  y){ std::cout << y << std::endl; });
    this->lua->set_function("noclip", [](){
        GameState::It()->ToggleNoClip();
        std::cout << "Toggled No clip to (" << (GameState::It()->IsNoClip() ? "true" : "false") << ")\n";
    });

    editor_save_data = lua->script_file("assets/data/editor_save_data.lua");
    if (editor_save_data.get<bool>("is_open")) {
        GameState::It()->ToggleFullEditor();
    }

    // Load tilemap names

    ImGui::StyleColorsLight();
}

void Editor::processEvent(sf::Event& event) {
    ImGui::SFML::ProcessEvent(event);
}

void Editor::doUI
    ( std::unique_ptr<sf::RenderWindow> &window
    , const SkyTime& time
    , std::shared_ptr<EntityWorld>& world
    , std::shared_ptr<Sky>& sky
    , std::shared_ptr<TiledMap>& tiledMap
    , std::shared_ptr<Camera>& camera
    ) {

    ImGui::SFML::Update(*window, editorClock.restart());

    ImGui::Begin("Sky Vault"); // begin window
        // Menu bar

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Edit")) {
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }
        ImGui::Spacing();

        ImGui::LabelText("Timing", "FPS: %f DT: %f", time.fps, time.dt);
        if (ImGui::Button("Toggle Debug View")) {
            std::cout << "Toggling Debug" << std::endl;
            GameState::It()->ToggleDebug();
        }
        ImGui::SameLine();
        if (ImGui::Button("Toggle Full Editor")) {
            GameState::It()->ToggleFullEditor();
        }
        ImGui::SameLine();
        if (ImGui::Button("Toggle No Clip")) {
            GameState::It()->ToggleNoClip();
        }

        if (ImGui::Button("Pop Layer")) {
            GameState::It()->PopLayer(true);
        }
    ImGui::End();

    if (GameState::It()->FullEditor()){
        doColors(sky);
        doInGameTerminal();
        doEntityInspector(world, window, tiledMap, camera);
    }

    // Render the editor
    ImGui::SFML::Render(*window);

    // Camera control @drag
    if (GameState::It()->FullEditor() && moving == -1) {
        static sf::Vector2f last_mouse{sf::Vector2f(0, 0)};
        if (Input::It()->IsKeyDown(sf::Keyboard::LControl) &&

            sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            auto curr = sf::Mouse::getPosition();
            auto delta = sf::Vector2f(curr.x, curr.y) - last_mouse;
            delta *= 0.7f;
            auto newPos = camera->View.getCenter() + delta * -1.0f;
            last_mouse = sf::Vector2f(curr.x, curr.y);
            camera->View.setCenter(newPos);

        } else {
            auto curr = sf::Mouse::getPosition();
            last_mouse = sf::Vector2f(curr.x, curr.y);
        }
    }

    // Camera zoom!
    auto delta = Input::It()->GetMouseWheelDelta();

    if (delta != 0.0f) {
        if (delta < 0.0f) {
            camera->View.zoom(1.1f);
        } else {
            camera->View.zoom(0.9f);
        }
    }
}

void Editor::doEntityInspector
    ( std::shared_ptr<EntityWorld>& world
    , std::unique_ptr<sf::RenderWindow> &window
    , std::shared_ptr<TiledMap> &tiledMap
    , std::shared_ptr<Camera>& camera
    ) {

    auto [wx, wy] = window->getSize();
    float ww = static_cast<float>(wx);
    float wh = static_cast<float>(wy);

    auto windowPos = sf::Vector2
        ( ww - (ww * 0.20f)
        , 0.0f
        );

    ImGui::Begin("World Editor");

    ImGui::SetWindowPos
        ( ImVec2(windowPos.x, windowPos.y)
        , true
        );

    ImGui::SetWindowSize
        ( ImVec2(ww * 0.20f, wh)
        , true
        );

    if (ImGui::CollapsingHeader("Level Inspector")) {
        const auto& maps = Assets::It()->GetTiledMapNames();

        int counter = 0;
        std::string id{""};
        for (const auto& name : maps) {
            id = "maps:" + std::to_string(counter++);
            if (ImGui::Button(name.c_str())) {
                tiledMap->Destroy();
                world->ClearDoors();

                CurrMapName = name;

                auto* physics = world->GetFilter<PhysicsFilter>();
                assert(physics);
                tiledMap->loadFromFile(
                        "assets/maps/" + name,
                        physics,
                        world,
                        lua);
            }
        }
    }

    if (ImGui::CollapsingHeader("Entity Inspector")) {
        if (ImGui::TreeNode("Inspector")) {
            ImGui::BeginGroup();

            const auto& entities = world->GetEntities();

            int id = 1000;
            for(auto& entity : entities) {
                ImGui::Spacing();

                auto str = "entity id: " + std::to_string(entity->GetUUID());

                if (entity->GetUUID() == CurrentlySelectedEntity) {
                    str += " <--";
                }

                if (ImGui::CollapsingHeader(str.c_str())){
                    if (entity->Has<Body>()) {

                        auto body = entity->Get<Body>();
                        const auto btn_label = "Goto##"+std::to_string(id++);
                        if (ImGui::Button(btn_label.c_str())) {
                            //Move camera to location

                            camera->View.setCenter(body->Center());
                        }

                        float p[] = {body->Position.x, body->Position.y};
                        ImGui::DragFloat2(std::string{"Position##"+std::to_string(entity->GetUUID())}.c_str(), p);
                        body->Position.x = p[0];
                        body->Position.y = p[1];
                    }

                    auto& components = entity->GetComponents();

                    for (const auto& [type, _]: components) {
                        auto name = std::string{type.name()};
                        auto it = name.begin();
                        while(*it >= '0' && *it <= '9' && it != name.end())
                            it++;

                        auto final_name = std::string(it, name.end());

                        if (ImGui::TreeNode(final_name.c_str())) {
                            if (final_name == "Laser") {
                                static int color = 0;

#if 0
                                ImGui::RadioButton(("North##"+std::string{id++}).c_str(), &direction, 0); ImGui::SameLine();
                                ImGui::RadioButton(("South##"+std::string{id++}).c_str(), &direction, 1); ImGui::SameLine();
                                ImGui::RadioButton(("East##"+std::string{id++}).c_str(), &direction, 2); ImGui::SameLine();
                                ImGui::RadioButton(("West##"+std::string{id++}).c_str(), &direction, 3);
#endif//
                                if (ImGui::Button("North")) { entity->Get<Laser>()->Facing = North; }
                                ImGui::SameLine();
                                if (ImGui::Button("South")) { entity->Get<Laser>()->Facing = South; }
                                if (ImGui::Button("West")) { entity->Get<Laser>()->Facing = West; }
                                ImGui::SameLine();
                                if (ImGui::Button("East")) { entity->Get<Laser>()->Facing = East; }

                                ImGui::RadioButton("Red", &color, 0); ImGui::SameLine();
                                ImGui::RadioButton("Blue", &color, 1); ImGui::SameLine();
                                ImGui::RadioButton("Green", &color, 2);

                                entity->Get<Laser>()->Color = (LaserColor)color;
                            }

                            ImGui::TreePop();
                        }
                    }
                }
            }

            ImGui::EndGroup();

        }

        if (ImGui::TreeNode("Prefabs")) {
            ImGui::BeginGroup();

            const auto prefabs = Assets::It()->GetPrefabs();
            int i = 0;
            for (auto& [name, table] : prefabs) {
                ImGui::Text("[%-14s]", name.c_str());
                ImGui::SameLine();

                if (ImGui::Button(("spawn##"+std::to_string(i++)).c_str())) {
                    entity_prefab = Assets::It()->GetPrefab(name);
                    HoldingState = HoldingState::Entity;
                }

                ImGui::SameLine();

                if (ImGui::Button(("place##"+std::to_string(i++)).c_str())) {
                    tiledMap->AddEntitySpawn(name, cursor.x, cursor.y);
                }
            }

            ImGui::EndGroup();
        }

        if (ImGui::TreeNode("Billboards")) {
            ImGui::Checkbox("Foreground", &PlaceAsForeground);
            ImGui::BeginGroup();

            const auto meta = tiledMap->GetMetaData();

            if (meta["billboard_regions"].valid()) {
                const auto bt = meta.get<sol::table>("billboard_regions");

                int i = 99;
                bt.for_each([&](sol::object const& key, sol::object const& value) {
                    ImGui::PushID(i++);

                    const auto tab = value.as<sol::table>();
                    const auto x = (int)tab[1];
                    const auto y = (int)tab[2];
                    const auto w = (int)tab[3];
                    const auto h = (int)tab[4];

                    const auto required_width = 64;
                    const auto scale = ((float)required_width) / ((float)w);

                    const auto tileset = tiledMap->GetFirstTileset();
                    const auto region = sf::IntRect(x, y, w, h);

                    sf::Sprite sprite;

                    auto ts = tiledMap->GetFirstTileset();

                    sprite.setTexture(*Assets::It()->Get<sf::Texture>(ts.name));
                    sprite.setTextureRect(region);
                    sprite.setScale(scale, scale);

                    if (ImGui::ImageButton(sprite)) {
                        HoldingState = HoldingState::Billboard;
                        BillboardRect = region;
                    }

                    if (i % 3 != 0) ImGui::SameLine();

                    ImGui::PopID();
                });
            }

            ImGui::EndGroup();
        }

        if (ImGui::TreeNode("Doors")) {
            if (ImGui::Button("New Door")) {
                ImGui::OpenPopup("NewDoor?");
                //HoldingState = HoldingState::Door;
            }

            ImGui::BeginGroup();

            ImGui::EndGroup();
        }

        if (ImGui::BeginPopupModal("NewDoor?", NULL, ImGuiWindowFlags_AlwaysAutoResize)){
            static char buf[100];

            const auto& tiled_map_names = Assets::It()->GetTiledMapNames();
            const char* items[tiled_map_names.size()];
            static int item = -1;

            for (int i = 0; i < tiled_map_names.size(); i++) {
                items[i] = tiled_map_names[i].c_str();
            }

            ImGui::Combo("To", &item, items, IM_ARRAYSIZE(items));

            ImGui::InputText("Unique ID", buf, IM_ARRAYSIZE(buf));

            if (ImGui::Button("OK", ImVec2(120, 0))) {

                HoldingState = HoldingState::Door;
                ToString = std::string{items[item]};
                UuidString = std::string{buf};

                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        if (ImGui::TreeNode("Solids")) {
            if (ImGui::Button("New Solid")) {
                HoldingState = HoldingState::Solid;
            }

        }

    }

    ImGui::End();
}

void Editor::doInGameTerminal() {
    ImGui::SetNextWindowSize(ImVec2(520, 256));
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    ImGui::Begin("Console");

    static char buff[256];
    bool reclaim_focus{false};

    const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing(); // 1 separator, 1 input text
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar); // Leave room for 1 separator + 1 InputText

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

    for (const auto& cmd : command_history) {
        ImVec4 col = ImVec4(1.0, 1.0, 1.0, 1.0);
        if (cmd.find("[ERROR]") != std::string::npos) {
            col = ImVec4(1.0, 1.0, 0.0, 1.0);
        }
        ImGui::PushStyleColor(ImGuiCol_Text, col);
        ImGui::TextUnformatted(cmd.c_str());
        ImGui::PopStyleColor();
    }

    ImGui::PopStyleVar();
    ImGui::EndChild();

    ImGui::Separator();

    if (ImGui::InputText("Input", buff, IM_ARRAYSIZE(buff),  ImGuiInputTextFlags_EnterReturnsTrue|ImGuiInputTextFlags_CallbackCompletion|ImGuiInputTextFlags_CallbackHistory, [](auto a){return 0;})) {
        std::string command{buff};

        const auto pre = std::cout.rdbuf();

        std::ostringstream output;
        std::cout.rdbuf(output.rdbuf());

        try {
            lua->script(command, sol::script_default_on_error);
        } catch (const sol::error& err) {
            command_history.push_back(
                "[ERROR]:: " + std::string{err.what()});
        }

        command_history.push_back(command);

        std::string sentence{output.str()};
        std::stringstream ss(sentence);
        std::string to;
        while (std::getline(ss, to, '\n')) {
            command_history.push_back(to);
        }

        std::cout.rdbuf(pre);

        memset(buff, 0, sizeof(buff));

        reclaim_focus = true;
    }

    ImGui::SetItemDefaultFocus();
    if (reclaim_focus) ImGui::SetKeyboardFocusHere(-1);

    ImGui::End();
}

void Editor::Draw
    ( std::unique_ptr<sf::RenderWindow> &window
    , std::shared_ptr<TiledMap> &tiledMap
    , std::shared_ptr<EntityWorld> &world
    ) {

    const auto& entities = world->GetEntities();

    // get the current mouse position in the window
    sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);

    // convert it to world coordinates
    sf::Vector2f worldPos = window->mapPixelToCoords(pixelPos);

    // Draw the editor controls over each entity
    // delete / move
    for (const auto& entity : entities) {
        if (entity->Has<Body>()) {
            auto body = entity->Get<Body>();

            sf::RectangleShape rect;
            rect.setFillColor(sf::Color(0, 0, 0, 0));
            rect.setOutlineColor(sf::Color(255, 0, 0, 100));
            rect.setOutlineThickness(1);
            rect.setPosition(body->Position + sf::Vector2f(0, body->Size.y) + sf::Vector2f(0, 4));
            rect.setSize(sf::Vector2f(4, 4));

            if (Input::It()->IsMouseLeftPressed(0)) {
                const auto [mx, my] = worldPos;
                if (mx > rect.getPosition().x && mx < rect.getPosition().x + rect.getSize().x &&
                    my > rect.getPosition().y && my < rect.getPosition().y + rect.getSize().y) {
                    entity->Kill();
                }
            }

            window->draw(rect);
        }
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)){
        if (HoldingState == HoldingState::Billboard
                || HoldingState == HoldingState::Entity
                || HoldingState == HoldingState::Door
                || HoldingState == HoldingState::Solid){
            HoldingState = HoldingState::None;
        } else {
            cursor = worldPos;
        }
    }

    const auto [x, y] = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

    // TODO(Dustin): use isMousePressed
    if (Input::It()->IsMouseLeftPressed(0) &&
        !Input::It()->IsKeyDown(sf::Keyboard::LControl)) {

        switch (HoldingState) {
            case HoldingState::Billboard: {
                tiledMap->AddBillboard(
                        BillboardRect,
                        worldPos - sf::Vector2f((float)BillboardRect.width,
                        (float)BillboardRect.height) * 0.5f,
                        PlaceAsForeground);

                HoldingState = HoldingState::None;
                break;
            }

            case HoldingState::Entity: {
                if (HoldingState == HoldingState::Entity) {
                    auto e = world->Create(entity_prefab);
                    if (e && e->Has<Body>()) {
                        e->Get<Body>()->Position = worldPos - (e->Get<Body>()->Size / 2.0f);
                    }

                    //Spawn the entity using the prefab -- worldPos

                    HoldingState = HoldingState::None;
                }

                break;
            }

            case HoldingState::Door: {
                HoldingState = HoldingState::DoorDragging;
                DragStart = sf::Vector2f(x, y);
                break;
            }

            case HoldingState::Solid: {
                HoldingState = HoldingState::SolidDragging;
                DragStart = sf::Vector2f(x, y);
                break;
            }

            default:
                break;
        }
    }


    // Handling door dragging release

    // Set the door end values
    if (HoldingState == HoldingState::DoorDragging
            || HoldingState == HoldingState::SolidDragging) {
        DragEnd = sf::Vector2f(x, y);

        if (Input::It()->IsMouseLeftReleased(0)){
            const auto [dx, dy] = DragStart;
            const auto dw = DragEnd.x - dx;
            const auto dh = DragEnd.y - dy;

            if (HoldingState == HoldingState::SolidDragging) {
                // TODO(Dustin): Add solid
                auto* physics_filter = world->GetFilter<PhysicsFilter>();
                if (physics_filter) {
                    physics_filter->AddSolid(
                            dx, dy,
                            dw, dh
                            );
                }
            }
            else {
                tiledMap->AddDoor(
                        world,
                        ToString,
                        UuidString,
                        dx,
                        dy,
                        dw,
                        dh);
            }
            HoldingState = HoldingState::None;
        }
    }

    constexpr float radius{4.0f};
    sf::CircleShape circle;
    circle.setPosition(cursor - sf::Vector2f(radius, radius));
    circle.setRadius(radius);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(sf::Color::White);
    circle.setOutlineThickness(1);

    window->draw(circle);

    // Draw billboard before being placed
    switch (HoldingState) {
        case HoldingState::Billboard: {
            sf::Sprite sprite;

            auto ts = tiledMap->GetFirstTileset();
            sprite.setTexture(*Assets::It()->Get<sf::Texture>(ts.name));
            sprite.setTextureRect(BillboardRect);
            sprite.setColor(sf::Color(255, 255, 255, 100));
            sprite.setPosition(sf::Vector2f(x, y) - sf::Vector2f((float)BillboardRect.width, (float)BillboardRect.height) * 0.5f);
            window->draw(sprite);
            break;
        }

        case HoldingState::Entity: {
            sf::Sprite sprite;

            // TODO(Dustin): Handle animated sprites
            const sol::table components = entity_prefab.get<sol::table>("components");
            if (const sol::table SpriteData = components.get<sol::table>("Sprite")) {
                auto texture = SpriteData.get<std::string>("texture");
                if (const sol::table region = SpriteData.get<sol::table>("region")) {
                    EntityRect.left = (unsigned int)(int)region[1];
                    EntityRect.top = (unsigned int)(int)region[2];
                    EntityRect.width = (unsigned int)(int)region[3];
                    EntityRect.height = (unsigned int)(int)region[4];
                    sprite.setTextureRect(EntityRect);
                }
                sprite.setTexture(*Assets::It()->Get<sf::Texture>(texture));
                sprite.setColor(sf::Color(255, 255, 255, 100));
                sprite.setPosition(sf::Vector2f(x, y) - sf::Vector2f((float)EntityRect.width, (float)EntityRect.height) * 0.5f);
                window->draw(sprite);
            } else if (const sol::table SpriteData = components.get<sol::table>("Item")) {
                auto texture = SpriteData.get<std::string>("texture");
                if (const sol::table region = SpriteData.get<sol::table>("region")) {
                    EntityRect.left = (unsigned int)(int)region[1];
                    EntityRect.top = (unsigned int)(int)region[2];
                    EntityRect.width = (unsigned int)(int)region[3];
                    EntityRect.height = (unsigned int)(int)region[4];
                    sprite.setTextureRect(EntityRect);
                }
                sprite.setTexture(*Assets::It()->Get<sf::Texture>(texture));
                sprite.setColor(sf::Color(255, 255, 255, 100));
                sprite.setPosition(sf::Vector2f(x, y) - sf::Vector2f((float)EntityRect.width, (float)EntityRect.height) * 0.5f);
                window->draw(sprite);
            }

            break;
        }

        case HoldingState::Door:
        case HoldingState::DoorDragging:
        case HoldingState::Solid:
        case HoldingState::SolidDragging: {
            constexpr float size{12};

            float px = x;
            float py = y;

            if (HoldingState == HoldingState::DoorDragging
                    || HoldingState == HoldingState::SolidDragging) {

                px = DragStart.x;
                py = DragStart.y;

                sf::RectangleShape doorRect;
                doorRect.setPosition(sf::Vector2f(px, py));
                doorRect.setSize(sf::Vector2f(
                    DragEnd.x - px,
                    DragEnd.y - py
                        ));
                doorRect.setFillColor(sf::Color(255, 0, 0, 100));
                window->draw(doorRect);
            }

            sf::RectangleShape cursor_hor;
            sf::RectangleShape cursor_ver;

            cursor_hor.setPosition(sf::Vector2f(
                    px - size / 2,
                    py - 1
                    ));

            cursor_ver.setPosition(sf::Vector2f(
                    px - 1,
                    py - size / 2
                    ));

            cursor_hor.setSize(sf::Vector2f(size, 2));
            cursor_ver.setSize(sf::Vector2f(2, size));

            window->draw(cursor_hor);
            window->draw(cursor_ver);

            break;
        }

        default:
            break;
    }

    // Draw solids
    {
        auto* physics_filter = world->GetFilter<PhysicsFilter>();
        if (physics_filter) {
            const auto& solids = physics_filter->GetSolids();
            sf::RectangleShape solid_rect;
            solid_rect.setOutlineColor(sf::Color(0,0,255,100));
            solid_rect.setFillColor(sf::Color(0,0,0,0));
            solid_rect.setOutlineThickness(1);

            for (const auto& solid : solids) {
                solid_rect.setPosition(solid->Position);
                solid_rect.setSize(solid->Size);

                window->draw(solid_rect);
            }
        }
    }

    const auto& billboards = tiledMap->GetBillboards();
    const auto& entity_spawns = tiledMap->GetEntitySpawns();

    const auto [mx, my] = worldPos; // Mouse in world coords

    sf::RectangleShape rect;
    rect.setOutlineColor(sf::Color(0, 255, 255, 70));
    rect.setOutlineThickness(1);

    constexpr auto BTN_SIZE{4u};

    auto font = Assets::It()->Get<sf::Font>("dialog");
    rect.setOutlineColor(sf::Color(0, 255, 255, 100));
    sf::Text text;
    text.setFont(*font);
    text.setFillColor(sf::Color(0, 255, 255, 100));
    text.setScale(0.2f, 0.2f);

    // Draw Doors
    {
        sf::RectangleShape doorShape;
        doorShape.setFillColor(sf::Color(0, 255, 255, 30));
        auto& doors = world->GetDoors();

        for (auto& door : doors) {
            doorShape.setPosition(door.Position);
            doorShape.setSize(door.Size);


            window->draw(doorShape);

            text.setString(door.To);
            text.setPosition(door.Position + sf::Vector2f(BTN_SIZE + 4, 0));
            window->draw(text);

            text.setString(door.Uuid);
            text.setPosition(door.Position + sf::Vector2f(BTN_SIZE + 4, 14));
            window->draw(text);

            // Delete button
            rect.setFillColor(sf::Color(0, 0, 0, 0));

            auto [bx, by] = door.Position;

            if (mx > bx && mx < bx + BTN_SIZE &&
                my > by && my < by + BTN_SIZE) {

                rect.setFillColor(sf::Color(255, 0, 0));

                if (Input::It()->IsMouseLeftPressed(sf::Mouse::Left)) {
                    // Remove the door
                    std::cout << "Removing Door" << std::endl;

                    door.ShouldRemove = true;
                    tiledMap->RemDoor(door.TableAddress);
                }
            }

            rect.setSize(sf::Vector2f(BTN_SIZE, BTN_SIZE));
            rect.setOutlineColor(sf::Color::White);

            rect.setPosition(door.Position);

            window->draw(rect);
        }
    }

    for (const auto& billboard : billboards) {
        rect.setFillColor(sf::Color(0, 0, 0, 0));

        const auto region = billboard->Sprite.getTextureRect();
        auto [bx, by] = billboard->Sprite.getPosition();

        // Only draw the billboards box if mouse is within it
        if (mx > bx && mx < bx + region.width &&
                my > by && my < by + region.height) {
            rect.setSize(sf::Vector2f(region.width, region.height));
            rect.setPosition(billboard->Sprite.getPosition());

            window->draw(rect);

            rect.setSize(sf::Vector2f(BTN_SIZE, BTN_SIZE));
            rect.setPosition(billboard->Sprite.getPosition());

            // Delete button
            if (mx > bx && mx < bx + BTN_SIZE &&
                my > by && my < by + BTN_SIZE) {

                rect.setFillColor(sf::Color(255, 111, 255));

                if (Input::It()->IsMouseLeftPressed(sf::Mouse::Left)) {
                    billboard->ShouldRemove = true;
                }
            }

            window->draw(rect);

            rect.setFillColor(sf::Color(0, 0, 0, 0));

            rect.setPosition
                ( billboard->Sprite.getPosition()
                + sf::Vector2f(BTN_SIZE + 4, 0)
                );

            bx += BTN_SIZE + 4;

            // Move button
            if (mx > bx && mx < bx + BTN_SIZE &&
                my > by && my < by + BTN_SIZE) {

                rect.setFillColor(sf::Color(111, 222, 255));

                if (Input::It()->IsMouseLeftPressed(sf::Mouse::Left)) {
                    const auto [bx, by] = rect.getPosition();

                    billboard->ShouldRemove = true;
                    HoldingState = HoldingState::None;
                    BillboardRect = billboard->Sprite.getTextureRect();
                    Input::It()->ResetLeftMousePressed();

                }
            }

            window->draw(rect);
            rect.setFillColor(sf::Color(0, 0, 0, 0));
        }
    }

    //NOTE(Dustin): The use of a pointer is awful, what would be better
    // is an integer that we can use to query the entity, or maybe a shared
    // pointer
    if (moving != -1) {
        auto e_opt = world->GetEntity(moving);
        if (e_opt) {
            auto e = e_opt.value();
            if (e->Has<Body>()) {
                auto body = e->Get<Body>();
                body->Position = worldPos + placement_offset;
            }

            if (Input::It()->IsMouseLeftReleased(1)) {
                moving = -1;
            }
        }
    }

    //@MovingEntities
    for (const auto& entity : world->GetEntities()) {
        if (!entity->Has<Body>()) { continue; }
        if (Input::It()->IsMouseLeftPressed(1)) {
            auto body = entity->Get<Body>();
            const auto [mx, my] = worldPos;

            if (mx > body->Position.x && mx < body->Position.x + body->Size.x &&
                my > body->Position.y && my < body->Position.y + body->Size.y) {

                placement_offset = sf::Vector2f
                    ( body->Position.x - mx
                    , body->Position.y - my
                    );

                moving = entity->GetID();
                CurrentlySelectedEntity = entity->GetUUID();
            }
        }
    }

    for (const auto& entity_spawn : entity_spawns) {
        rect.setSize(sf::Vector2f(32, 32));
        rect.setPosition(entity_spawn->Position);
        window->draw(rect);

        constexpr auto SIZE{4u};

        text.setString(entity_spawn->EntityName);
        text.setPosition(entity_spawn->Position + sf::Vector2f(2, 2));

        rect.setSize(sf::Vector2f(SIZE, SIZE));
        rect.setPosition
            ( entity_spawn->Position
            - sf::Vector2f(0, SIZE + 1));

        if (Input::It()->IsMouseLeftPressed(sf::Mouse::Left)) {
            const auto [mx, my] = worldPos;
            const auto [bx, by] = rect.getPosition();

            if (mx > bx && mx < bx + SIZE &&
                my > by && my < by + SIZE) {

                entity_spawn->ShouldRemove = true;
            }
        }

        window->draw(text);
        window->draw(rect);
    }

    constexpr auto offset = radius * 0.5;

    // Top
    sf::RectangleShape line;
    line.setPosition(cursor - sf::Vector2f(0.5, radius + (offset)) - sf::Vector2f(0, radius));
    line.setSize(sf::Vector2f(1, radius*2));
    window->draw(line);

    // Bottom
    line.setPosition(cursor - sf::Vector2f(0.5, -1.0 * radius - (offset)) - sf::Vector2f(0, radius));
    line.setSize(sf::Vector2f(1, radius*2));
    window->draw(line);

    // Right
    line.setPosition(cursor - sf::Vector2f(-1.0 * radius - (offset), 0.5) - sf::Vector2f(radius, 0));
    line.setSize(sf::Vector2f(radius*2, 1));
    window->draw(line);

    // Left
    line.setPosition(cursor - sf::Vector2f(radius + (offset), 0.5) - sf::Vector2f(radius, 0));
    line.setSize(sf::Vector2f(radius*2, 1));
    window->draw(line);
}

void Editor::doColors(std::shared_ptr<Sky>& sky) {
    ImGui::Begin("Color Editor");

    //constexpr int misc_flags = 0;

    //if (ImGui::BeginMenu("Sky")) {
        ImGui::ColorEdit3("Sun Color##1", (float*)&sky->Suncolor);
        ImGui::ColorEdit3("High Color##2", (float*)&sky->Highsky);
        ImGui::ColorEdit3("Low Color##3", (float*)&sky->Lowsky);
    //}

    ImGui::End();
}
