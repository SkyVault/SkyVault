#ifndef SKYVAULT_COMBAT_ENTITY_H
#define SKYVAULT_COMBAT_ENTITY_H 

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "../graphics/animation.h"

#include "../skytime.h"

struct CombatEntity {
    sf::Vector2f Size{sf::Vector2f(32, 64)};
    sf::Vector2f Position{sf::Vector2f(0, 0)};

    virtual void Update(const SkyTime& time) {}
    virtual void Draw(std::unique_ptr<sf::RenderWindow>& window) {
        sf::RectangleShape shape;
        shape.setPosition(Position);
        shape.setSize(Size);
        shape.setFillColor(sf::Color::Red);
        window->draw(shape);
    }

private:
    sf::Sprite sprite;
    Animation animation;
};

struct CombatBasicEnemy : public CombatEntity {
    
};

struct CombatPlayer : public CombatEntity {
    void Draw(std::unique_ptr<sf::RenderWindow>& window) override {
        sf::RectangleShape shape;
        shape.setPosition(Position);
        shape.setSize(Size);
        shape.setFillColor(sf::Color::Blue);
        window->draw(shape);
    }
};

#endif//SKYVAULT_COMBAT_ENTITY_H
