#include "renderable.h"

Renderable::Renderable() {

}

Renderable::Renderable(const sf::Texture* texture) {
    sprite.setTexture(*texture);
}

Renderable::Renderable(const sf::Texture* texture, const sf::IntRect& region) {
    sprite.setTexture(*texture);
    sprite.setTextureRect(region);
}

AnimatedSprite::AnimatedSprite(const sf::Texture* texture, const std::map<std::string, Animation>& animations):
    animations(animations)
{
    sprite.setTexture(*texture);

    const auto first = animations.begin()->first;
    current_animation = first;
}
