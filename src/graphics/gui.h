// This handles all top level screen drawing, dialog boxes and buttons, and hearts and stuff
#ifndef SKYVAULT_GUI_H
#define SKYVAULT_GUI_H 

#include <SFML/Graphics.hpp>
#include <sol.hpp>
#include "dialog.h"
#include "../utilities/input.h"
#include "../utilities/quest_engine.h"
#include "../game_state.h"
#include "../skytime.h"
#include "../graphics/assets.h"
//#include <
//
//NOTE(Dustin): I'm going to make the dialog box code behandled here, 
// though ill make it easy to move if i feel like it should

struct GUI {
    void DoDialog(const sol::table& data);

    void Update(const SkyTime& time);
    void Render(const std::unique_ptr<sf::RenderWindow>& window);

private:
    std::vector<std::unique_ptr<Dialog>> dialog_stack;

    GameState::States PreviousState;

    const sf::Color DialogBoxColor{sf::Color(0, 100, 100, 200)};
};

#endif//SKYVAULT_GUI_H
