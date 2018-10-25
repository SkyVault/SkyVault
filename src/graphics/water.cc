#include "water.h"

Water::Water() {

}

void Water::Load(int width, int height) {

}

void Water::Update(int width, int height, const SkyTime& time) {

}

void Water::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

}
