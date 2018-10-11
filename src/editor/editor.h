#ifndef SKYVAULT_EDITOR_H
#define SKYVAULT_EDITOR_H

#include "../imgui.h"
#include "../imgui-SFML.h"
#include "../skytime.h"
#include "../graphics/assets.h"
#include "../entities/entity_world.h"
#include "../entities/components/body.h"
#include "../entities/components/player.h"
#include "../skyvault.h"

#include <memory>
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <sol.hpp>
#include <typeinfo>

struct Editor {
    void initUI(std::unique_ptr<sf::RenderWindow> &window, std::shared_ptr<sol::state>& lua);
    void processEvent(sf::Event& event);
    void doUI(std::unique_ptr<sf::RenderWindow> &window, const SkyTime& time, std::shared_ptr<EntityWorld>& world);
    void doEntityInspector(std::shared_ptr<EntityWorld>& world);
private:
    std::shared_ptr<sol::state> lua;

    void doInGameTerminal();
    std::vector<std::string> command_history;
    sf::Clock editorClock;
};

#endif//SKYVAULT_EDITOR_H
