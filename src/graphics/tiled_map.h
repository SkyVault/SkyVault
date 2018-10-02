#ifndef SKYVAULT_TILED_MAP_H
#define SKYVAULT_TILED_MAP_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "../tinyxml2.h"

struct Tileset {
    std::string name{""};
    int tilewidth{8};
    int tileheight{8};

    sf::Texture image;
    //std::string image_path{""};
    //int imagewidth{0};
    //int imageheight{0};
};

struct TiledLayer {
    std::string name{""};
    std::vector<unsigned int> data;
    sf::VertexArray vertices;
};

struct TiledObject {

};

struct TiledMap : public sf::Drawable, public sf::Transformable {
    ~TiledMap() {
        for (auto* layer : layers)
            delete layer;
    }

    bool loadFromFile(const std::string& path);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    inline int getWidth() const { return width; }
    inline int getHeight() const { return height; }

private:
    std::vector<TiledLayer*> layers;
    std::vector<Tileset> tilesets;

    int width{0}, height{0};
    int tilewidth{0}, tileheight{0};
};


#endif//SKYVAULT_TILED_MAP_H
