#ifndef SKYVAULT_AST_H
#define SKYVAULT_AST_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <tuple>
#include <mutex>
#include <algorithm>

class Art {
	Art(const Art&) = delete;
	Art& operator=(const Art&) = delete;

	static std::unique_ptr<Art> instance;
	static std::once_flag onceFlag;

    Art() { }

public:
    static auto* It() {
        std::call_once(Art::onceFlag, [] () {
            instance.reset(new Art());     
        });

        return instance.get();
    }

    void Draw(sf::Sprite& d, float layer=0.5f);
    void Flush(std::unique_ptr<sf::RenderWindow>& window);

private:
    std::vector<std::tuple<sf::Sprite, float>> sprites;
};

#endif//SKYVAULT_AST_H 
