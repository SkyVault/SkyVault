#include "editor.h"
#include <string.h>
#include <sstream>

void Editor::initUI(std::unique_ptr<sf::RenderWindow> &window, std::shared_ptr<sol::state>& lua) {
    ImGui::SFML::Init(*window);
    this->lua = lua;
    this->lua->set_function("print", [](const char *  y){ std::cout << y << std::endl; });
}

void Editor::processEvent(sf::Event& event) {
    ImGui::SFML::ProcessEvent(event);
}

void Editor::doUI(std::unique_ptr<sf::RenderWindow> &window, const SkyTime& time) {
    ImGui::SFML::Update(*window, editorClock.restart());
    ImGui::Begin("Sample window"); // begin window
    ImGui::LabelText("Timing", "FPS: %f DT: %f", time.fps, time.dt);
    ImGui::End();

    // Entity prefab window
    ImGui::Begin("Entity Prefabs"); // begin window

    const auto prefabs = Assets::It()->GetPrefabs();
    for (auto& [name, table] : prefabs) {
        ImGui::Text("[%-18s]", name.c_str());
        ImGui::SameLine();
        ImGui::Button("spawn");
    }

    ImGui::End();

    doInGameTerminal();

    // Render the editor
    ImGui::SFML::Render(*window);
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
    if (ImGui::InputText("Input", buff, IM_ARRAYSIZE(buff),  ImGuiInputTextFlags_EnterReturnsTrue|ImGuiInputTextFlags_CallbackCompletion|ImGuiInputTextFlags_CallbackHistory)) {
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

        std::string sentence{output.str()};
        std::stringstream ss(sentence);        
        std::string to;
        while (std::getline(ss, to, '\n')) {
            command_history.push_back(to); 
        }

        std::cout.rdbuf(pre);

        command_history.push_back(command);

        memset(buff, 0, sizeof(buff));
        
        reclaim_focus = true;
    }

    ImGui::SetItemDefaultFocus();
    if (reclaim_focus) ImGui::SetKeyboardFocusHere(-1);

    ImGui::End();
}
