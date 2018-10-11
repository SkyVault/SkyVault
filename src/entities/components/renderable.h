#ifndef SKYVAULT_RENDERABLE_H
#define SKYVAULT_RENDERABLE_H

#include "../component.h"
#include "../../graphics/animation.h"
#include "../../graphics/frame.h"
#include <map>
#include <string>
#include <SFML/Graphics.hpp>

struct RenderFilter;
struct RenderAnimatedSpriteFilter;

struct Renderable : public Component {
    friend RenderFilter;

    Renderable();
    Renderable(const sf::Texture* texture);
    Renderable(const sf::Texture* texture, const sf::IntRect& region);
    Renderable(const sf::Texture* texture, const sf::IntRect& region, sf::Vector2f offset);
    
    sf::Vector2f Offset{sf::Vector2f(0, 0)};
    sf::Color Color{sf::Color::White};

    inline sf::Sprite& GetSprite() { return sprite; }
protected:
    sf::Sprite sprite;
};

struct AnimatedSprite : public Renderable {
    friend RenderAnimatedSpriteFilter;

    AnimatedSprite(const sf::Texture* texture, const std::map<std::string, Animation>& animations);

    inline Animation& GetCurrentAnimation() {
        return animations[current_animation];
    }
   
private:
    float timer{0.0f};

    std::string current_animation{""};
    std::map<std::string, Animation> animations;
};

#endif//SKYVAULT_RENDERABLE_H
