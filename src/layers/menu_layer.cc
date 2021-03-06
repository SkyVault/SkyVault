#include "menu_layer.h"
#include <iostream>

void MenuLayer::Load(){
    std::cout << "Menu Loaded" << std::endl; 
   //if (Input::It()->IsKeyPressed(sf::Keyboard::Enter)) {  
   //}
}

void MenuLayer::Update(const SkyTime& time){
   GameState::It()->PushLayer(new LevelLayer(
       world,
       camera,
       lua,
       gui,
       tiledMap,
       sky
       ));
}

void MenuLayer::Render(std::unique_ptr<sf::RenderWindow>& window){
    auto [w, h] = window->getSize();

    auto* font = Assets::It()->Get<sf::Font>("arcade");
    if (font == nullptr) return;

    sf::Text text;    
    text.setFont(*font);
    text.setString("Press Enter");
    text.setCharacterSize(42);

    auto fw = text.getLocalBounds().width;
    auto fh = text.getLocalBounds().height;

    text.setPosition(sf::Vector2f(w * 0.5 - fw * 0.5, h * 0.5 - fh));

    window->draw(text);
}

void MenuLayer::Destroy(){}
