#include "combat_layer.h"

void CombatLayer::Load() { 
    GameState::It()->CurrentState = GameState::States::COMBAT_STATE;

    camera->View.setCenter(sf::Vector2f(0, 0));

    // Create a simple combat senario

    // Player 
    player = std::make_unique<CombatPlayer>();
    player->Position = sf::Vector2f(100, 100);
    
    // Enemies

    for (int i = 0; i < 3; i++) {
        auto enemy = std::make_unique<CombatBasicEnemy>();
        const auto [ww, wh] = GameState::It()->GetWindowSize();

        enemy->Position = sf::Vector2f(600, 100 + (i * (enemy->Size.y * 1.1f)));

        this->enemies.emplace_back(std::move(enemy));
    }
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

    player->Draw(window);

    for (auto& enemy : enemies) {
        enemy->Draw(window);
    }
}

void CombatLayer::Destroy() { 
    std::cout << "Destroying combat layer" << std::endl;
    GameState::It()->CurrentState = GameState::States::PLAYING_STATE;

    enemies.clear();
}
