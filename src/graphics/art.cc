#include "art.h"

void Art::Draw(sf::Sprite& d, float layer) {
    sprites.push_back({d, layer}); 
}

void Art::Flush(std::unique_ptr<sf::RenderWindow>& window) {
    std::sort(sprites.begin(), sprites.end(), [](auto a, auto b){
        return std::get<1>(a) < std::get<1>(b);
    });

    for (const auto& sprite : sprites) {
        window->draw(std::get<0>(sprite));
    }

    sprites.clear();
}
