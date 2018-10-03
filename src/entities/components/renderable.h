#ifndef SKYVAULT_RENDERABLE_H
#define SKYVAULT_RENDERABLE_H

#include "../component.h"
#include "../../graphics/animation.h"
#include "../../graphics/frame.h"
#include <map>
#include <string>
#include <SFML/Graphics.hpp>

struct RenderFilter;

struct Renderable : public Component {
    friend RenderFilter;

    Renderable();
    Renderable(const sf::Texture* texture);
    Renderable(const sf::Texture* texture, const sf::IntRect& region);
    
    sf::Color Color{sf::Color::White};

    inline sf::Sprite& GetSprite() { return sprite; }
private:
    sf::Sprite sprite;
};

struct AnimatedSprite : public Renderable {
    friend RenderFilter;

    AnimatedSprite(const sf::Texture* texture, std::map<std::string, Animation>& animations);

    inline Animation& GetCurrentAnimation() {
        return animations[current_animation];
    }
   
private:
    std::string current_animation{""};
    std::map<std::string, Animation> animations;
};

#endif//SKYVAULT_RENDERABLE_H
