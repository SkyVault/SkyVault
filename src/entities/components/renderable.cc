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
