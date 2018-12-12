#include "combat_layer.h"

void CombatLayer::Load() { 
    GameState::It()->CurrentState = GameState::States::COMBAT_STATE;
}

void CombatLayer::Update(const SkyTime& time) {
    auto [x, y] = GameState::It()->GetWindowSize();
    sky->Update(x, y, time); 
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

}
