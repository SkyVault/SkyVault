#ifndef SKYVAULT_COMBAT_H
#define SKYVAULT_COMBAT_H

#include <functional>
#include <vector>
#include <memory>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "../entities/combat_entity.h"
#include "../utilities/input.h"
#include "../graphics/gui.h"
#include "layer.h"
#include "../graphics/tween.h"
#include "level_layer.h"
#include "../game_state.h"
#include "../skyvault.h"

constexpr float ENEMY_MARGIN{16.f};
constexpr float COMBAT_ACTION_BUTTON_SIZE {16};
constexpr float HALF_X_DIST = 200.0f; 

struct CombatActionButton {
    inline CombatActionButton(const sf::Vector2f& _position, std::function<void()> _onClick) : onClick(_onClick) {
        shape.setPosition(_position);
        shape.setSize(sf::Vector2f(COMBAT_ACTION_BUTTON_SIZE, COMBAT_ACTION_BUTTON_SIZE));
        
        target = _position;

        // Temparary color
        shape.setFillColor(sf::Color(
            rand_int(0, 255),
            rand_int(0, 255),
            rand_int(0, 255)
                    ));
    }

    sf::RectangleShape shape;

    sf::Vector2f target;

    float alpha{1.0f};

    std::function<void()> onClick;
};

struct CombatLayer : public Layer {
    inline CombatLayer( 
        const std::shared_ptr<EntityWorld>& world,
        const std::shared_ptr<Camera>& camera,
        const std::shared_ptr<sol::state>& lua,
        const std::shared_ptr<Sky>& sky
        ): 
    world(std::move(world)),
    camera(std::move(camera)),
    lua(std::move(lua)),
    sky(std::move(sky)) {}

    void Load() override;
    void Update(const SkyTime& time) override;
    void Render(std::unique_ptr<sf::RenderWindow>& window) override;
    void Destroy() override;

    inline void ToggleTurn() {
        players_turn = !players_turn;
        if (players_turn) 
            OnPlayerTurn();
        else
            OnEnemiesTurn();
    }

    void HandleEnemiesTurn(const SkyTime& time);
    void HandlePlayersTurn(const SkyTime& time);
    void OnPlayerTurn();
    void OnEnemiesTurn();

private:
    std::shared_ptr<EntityWorld> world;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<sol::state> lua;
    std::shared_ptr<Sky> sky;

    std::unique_ptr<CombatPlayer> player;
    std::vector<std::unique_ptr<CombatEntity>> enemies;

    std::vector<std::unique_ptr<CombatActionButton>> combat_action_buttons;

    int attack_indicator{0};

    bool players_turn{true};

    unsigned int turn_of_current_enemy{0};
};

#endif//SKYVAULT_COMBAT_H
