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
    
    // Create combat action buttons
    combat_action_buttons.push_back(
            std::make_unique<CombatActionButton>(
                player->Position + sf::Vector2f(32 + 8, 0),
                [&](){ 
                    ToggleTurn();
                }) 
            );

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

void CombatLayer::OnPlayerTurn() {
    
}

void CombatLayer::OnEnemiesTurn() {
    turn_of_current_enemy = 0;
} 

void CombatLayer::HandleEnemiesTurn(const SkyTime& time) {
    if (turn_of_current_enemy >= enemies.size()) {
        ToggleTurn();
        return;
    }

    auto& e = enemies[turn_of_current_enemy];

    // AI


    turn_of_current_enemy++;
}

void CombatLayer::Update(const SkyTime& time) {
    auto [x, y] = GameState::It()->GetWindowSize();
    sky->Update(x, y, time); 

    camera->View.setCenter(sf::Vector2f(0, 0));

    if (Input::It()->IsKeyPressed(sf::Keyboard::Enter)) {
        ToggleTurn();
    }

    if (!players_turn) HandleEnemiesTurn(time);

    // Set the positions of the combat buttons
    const auto [px, py] = player->Position + player->Size * 0.5f;

    constexpr float SPEED{200.0f};

    auto lerp_cab = [&](std::unique_ptr<CombatActionButton>& cab){
        auto [cx, cy] = cab->shape.getPosition(); 
        cx = Interpolation::Lerp(cx, cab->target.x, (time.dt * time.dt) * SPEED);
        cy = Interpolation::Lerp(cy, cab->target.y, (time.dt * time.dt) * SPEED);

        auto target_alpha = players_turn ? 1.0f : 0.0f;
        cab->alpha = Interpolation::Lerp(cab->alpha, target_alpha, (time.dt * time.dt) * SPEED);

        cab->shape.setPosition(cx, cy); 
    };

    if (players_turn) {
        constexpr float COMBAT_BUTTON_MARGIN {16.0f};
        const float total_size = 
            (combat_action_buttons.size() * (COMBAT_ACTION_BUTTON_SIZE + COMBAT_BUTTON_MARGIN)); 

        int index = 0; 
        for (auto& cab : combat_action_buttons) {
            cab->target = (sf::Vector2f(
                         px + COMBAT_ACTION_BUTTON_SIZE + 8 + (sinf(index) * 32.f),
                         py - (total_size * 0.5f) + ((COMBAT_BUTTON_MARGIN + COMBAT_BUTTON_MARGIN) * index)
                        ));

            lerp_cab(cab);

            ++index;
        }
    } else {
        for (auto& cab : combat_action_buttons) {
            cab->target = sf::Vector2f(px, py);

            lerp_cab(cab);
        } 
    }
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

    sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);

    // convert it to world coordinates
    sf::Vector2f worldPos = window->mapPixelToCoords(pixelPos);

    if (this->players_turn) { 
        auto [mx, my] = worldPos;

        if (Input::It()->IsMouseLeftPressed(0)) {
            for (const auto& cab : combat_action_buttons) {
                const auto [x, y] = cab->shape.getPosition();
                const auto [w, h] = cab->shape.getSize();
                    
                if (mx > x && mx < x + w &&
                    my > y && my < y + h) {

                    std::invoke(cab->onClick);
                }
            }
        }
    }

    player->Draw(window);

    for (auto& enemy : enemies) {
        enemy->Draw(window);
    }

    // Draw ui buttons
    for (const auto& cab : combat_action_buttons) {
        auto color = cab->shape.getFillColor();
        cab->shape.setFillColor(sf::Color(color.r, color.g, color.b, (unsigned int)(255.0f * cab->alpha)));
        window->draw(cab->shape);
    }
}

void CombatLayer::Destroy() { 
    std::cout << "Destroying combat layer" << std::endl;
    GameState::It()->CurrentState = GameState::States::PLAYING_STATE;

    enemies.clear();
}
