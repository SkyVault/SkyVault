// This handles all top level screen drawing, dialog boxes and buttons, and hearts and stuff
#ifndef SKYVAULT_GUI_H
#define SKYVAULT_GUI_H 

#include <SFML/Graphics.hpp>
#include "../game_state.h"
#include "../skytime.h"
#include "dialog.h"
//#include <

struct GUI {
    void DoDialog(const sol::table& data);
    void Update(const SkyTime& time);
    void Render(const std::unique_ptr<sf::RenderWindow>& window);

private:
    std::vector<std::unique_ptr<Dialog>> dialog_stack;
};

#endif//SKYVAULT_GUI_H
