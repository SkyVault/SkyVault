#include "combat_layer.h"

void CombatLayer::Load() { 
    GameState::It()->CurrentState = GameState::States::COMBAT_STATE;

    camera->View.setCenter(sf::Vector2f(0, 0));
}

void CombatLayer::Update(const SkyTime& time) {
    auto [x, y] = GameState::It()->GetWindowSize();
    sky->Update(x, y, time); 

    camera->View.setCenter(sf::Vector2f(0, 0));
}

void CombatLayer::Render(std::unique_ptr<sf::RenderWindow>& window) {
    // Unproject the sky
    window->setView(window->getDefaultView());
        window->draw(*sky);
    window->setView(camera->View);

    sf::CircleShape shape;
    shape.setRadius(100);
    shape.setPosition(100, 100);
    shape.setFillColor(sf::Color::Green);

    window->draw(shape);
    
}

void CombatLayer::Destroy() { 
    std::cout << "Destroying combat layer" << std::endl;
    GameState::It()->CurrentState = GameState::States::PLAYING_STATE;
}
