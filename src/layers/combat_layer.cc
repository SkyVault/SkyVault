#include "combat_layer.h"

void CombatLayer::Load() { 
    GameState::It()->CurrentState = GameState::States::COMBAT_STATE;

    camera->View.setCenter(sf::Vector2f(0, 0));

    // Create a simple combat senario
    const auto [ww, wh] = GameState::It()->GetWindowSize();
    auto scale_x = camera->View.getSize().x/static_cast<float>(ww);
    auto scale_y = camera->View.getSize().y/static_cast<float>(wh);

    const float cw = scale_x * ww * 0.5f;
    const float ch = scale_y * wh * 0.5f;

    constexpr float HALF_X_DIST = 200.0f;

    // Player 
    player = std::make_unique<CombatPlayer>();

    player->Position = sf::Vector2f(
            cw,
            ch
            ) - ((player->Size * 0.5f) + sf::Vector2f(HALF_X_DIST, 0));
    
    // Enemies
    
    constexpr float ENEMY_MARGIN{16.f};

    int total_height{0};
    for (int i = 0; i < 3; i++) {
        auto enemy = std::make_unique<CombatBasicEnemy>(); 
        //enemy->Position = sf::Vector2f((camera->View.getSize().x) - 200, 100 + (i * (enemy->Size.y * 1.1f))); 
        total_height += enemy->Size.y + ENEMY_MARGIN;
        this->enemies.emplace_back(std::move(enemy));
    }

    const int half_height = total_height * 0.5f;

    float cursor_y{ch - half_height};
    for (auto& enemy : enemies) { 
        enemy->Position = sf::Vector2f(
                cw + HALF_X_DIST,
                cursor_y
                ); 
        cursor_y += enemy->Size.y + ENEMY_MARGIN;
    }
}

void CombatLayer::Update(const SkyTime& time) {
    auto [x, y] = GameState::It()->GetWindowSize();
    sky->Update(x, y, time); 

    camera->View.setCenter(sf::Vector2f(0, 0));
}

void CombatLayer::Render(std::unique_ptr<sf::RenderWindow>& window) {
    // Unproject the sky and user interface
    window->setView(window->getDefaultView());
        window->draw(*sky); 

        auto font = Assets::It()->Get<sf::Font>("dialog");
        // User interface

        { // Draw Turn Marker
            const auto [ww, wh] = GameState::It()->GetWindowSize(); 
            auto x = ww * 0.5f;
            auto y = 64.0f;
            
            const std::string turn_text = 
                (players_turn 
                    ? "Your Turn"
                    : "Their Turn");

            constexpr auto size = 0.0f;
            auto text = sf::Text();
            text.setFont(*font);
            text.setString(turn_text); 

            const auto text_width = text.getLocalBounds().width;
            const auto text_height = text.getLocalBounds().height;

            sf::RectangleShape bg;
            bg.setSize(sf::Vector2f(text_width * 1.2f, text_height * 2.4f));
            bg.setPosition(x - bg.getSize().x * 0.5f, y - (bg.getSize().y * 0.5f));
            bg.setFillColor(sf::Color(0x83, 0x76, 0x9C));

            text.setPosition(
                    (bg.getPosition() + bg.getSize() * 0.5f) - 
                    sf::Vector2f(text_width * 0.5f, text_height));

            window->draw(bg);
            window->draw(text);
        } 

    window->setView(camera->View);

    auto [ww, wh] = window->getSize();

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
