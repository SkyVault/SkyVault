#ifndef SKYVAULT_EDITOR_H
#define SKYVAULT_EDITOR_H

#include "../imgui.h"
#include "../imgui-SFML.h"
#include "../skytime.h"
#include "../graphics/assets.h"

#include <memory>
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <sol.hpp>

struct Editor {
    void initUI(std::unique_ptr<sf::RenderWindow> &window, std::shared_ptr<sol::state>& lua);
    void processEvent(sf::Event& event);
    void doUI(std::unique_ptr<sf::RenderWindow> &window, const SkyTime& time);
private:
    std::shared_ptr<sol::state> lua;

    void doInGameTerminal();
    std::vector<std::string> command_history;
    sf::Clock editorClock;
};

#endif//SKYVAULT_EDITOR_H
