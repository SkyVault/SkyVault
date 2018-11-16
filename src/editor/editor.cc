#include "editor.h"
#include "../game_state.h"
#include <string.h>
#include <sstream>

void Editor::initUI(std::unique_ptr<sf::RenderWindow> &window, std::shared_ptr<sol::state>& lua) {
    ImGui::SFML::Init(*window);
    this->lua = lua;
    this->lua->set_function("print", [](const char *  y){ std::cout << y << std::endl; });
    this->lua->set_function("noclip", [](){
        GameState::It()->ToggleNoClip(); 
        std::cout << "Toggled No clip to (" << (GameState::It()->IsNoClip() ? "true" : "false") << ")\n";
    });
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
    ) {
    ImGui::SFML::Update(*window, editorClock.restart());
    ImGui::Begin("Sky Vault"); // begin window
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
    ImGui::End();

    if (GameState::It()->FullEditor()){ 
        doColors(sky);
        doInGameTerminal();
        doEntityInspector(world, window, tiledMap);
    }

    // Render the editor
    ImGui::SFML::Render(*window);
}

void Editor::doEntityInspector
    ( std::shared_ptr<EntityWorld>& world
    , std::unique_ptr<sf::RenderWindow> &window
    , std::shared_ptr<TiledMap> &tiledMap
    ) {

    ImGui::Begin("Entity Editor");
    
    if (ImGui::TreeNode("Inspector")) {
        ImGui::BeginGroup();

        const auto& entities = world->GetEntities();

        for(auto& entity : entities) {
            ImGui::Spacing();

            auto str = "entity id: " + std::to_string(entity->GetUUID());
            if (ImGui::CollapsingHeader(str.c_str())){
                auto& components = entity->GetComponentNames();

                if (entity->Has<Body>()) {
            
                    auto body = entity->Get<Body>();
                    const auto& btn_label = std::string{"Goto"};
                    if (ImGui::Button(btn_label.c_str())) {
                        auto player = world->GetFirstWith<Player>();
                        if (player) {
                            player->Get<Body>()->Position = body->Position;
                        }
                    }

                    float p[] = {body->Position.x, body->Position.y};
                    ImGui::DragFloat2(std::string{"Position##"+entity->GetUUID()}.c_str(), p);
                    body->Position.x = p[0];
                    body->Position.y = p[1];
                }

                for (auto& m : components) {
                    auto name = std::string{m};
                    auto it = name.begin();
                    while(*it >= '0' && *it <= '9' && it != name.end())
                        it++;
                    ImGui::BulletText(std::string(it, name.end()).c_str());
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
            ImGui::Text("[%-18s]", name.c_str());
            ImGui::SameLine();

            // NOTE(Dustin): This is just a test
            if (ImGui::Button(("spawn##"+std::to_string(i++)).c_str())) {
                auto e = world->Create(Assets::It()->GetPrefab(name));

                if (e->Has<Body>()) {
                    sf::Vector2f worldPos = window->mapPixelToCoords(sf::Vector2i(cursor.x, cursor.y));
                    e->Get<Body>()->Position = worldPos;
                }
            }
        }

        ImGui::EndGroup();
    }

    if (ImGui::TreeNode("Billboards")) {
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
                sprite.setTexture(*Assets::It()->Get<sf::Texture>("tiles"));
                sprite.setTextureRect(region);
                sprite.setScale(scale, scale);

                if (ImGui::ImageButton(sprite)) {
                    std::cout << "REEEEEEEEEEE" << std::endl;
                    
                    HoldingBillboardToBePlaced = true;
                    BillboardRect = region;

                }

                if (i % 3 != 0) ImGui::SameLine();

                ImGui::PopID(); 
            });
        }

        ImGui::EndGroup();
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

void Editor::Draw(std::unique_ptr<sf::RenderWindow> &window, std::shared_ptr<TiledMap> &tiledMap) {
    // get the current mouse position in the window
    sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);

    // convert it to world coordinates
    sf::Vector2f worldPos = window->mapPixelToCoords(pixelPos);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        if (HoldingBillboardToBePlaced){
            HoldingBillboardToBePlaced = !HoldingBillboardToBePlaced;
        } else 
            cursor = worldPos;

    // TODO(Dustin): use isMousePressed
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (HoldingBillboardToBePlaced) {
            tiledMap->AddBillboard(BillboardRect, worldPos - sf::Vector2f((float)BillboardRect.width, (float)BillboardRect.height) * 0.5f);
            HoldingBillboardToBePlaced = false;
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

    if (HoldingBillboardToBePlaced) {
        const auto [x, y] = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
        sf::Sprite sprite;
        sprite.setTexture(*Assets::It()->Get<sf::Texture>("tiles"));
        sprite.setTextureRect(BillboardRect);
        sprite.setColor(sf::Color(255, 255, 255, 100));
        sprite.setPosition(sf::Vector2f(x, y) - sf::Vector2f((float)BillboardRect.width, (float)BillboardRect.height) * 0.5f);
        window->draw(sprite);
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
