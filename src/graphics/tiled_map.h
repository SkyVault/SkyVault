#ifndef SKYVAULT_TILED_MAP_H
#define SKYVAULT_TILED_MAP_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <algorithm>
#include <gzip/decompress.hpp>
#include <gzip/compress.hpp>
#include <gzip/utils.hpp>
#include "../tinyxml2.h"
#include "../entities/filters/physics_filter.h"
#include "../entities/entity_world.h"
#include "../entities/entity.h"
#include "../entities/components/body.h"
#include "../graphics/art.h"

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
    std::vector<unsigned short> data;
    sf::VertexArray vertices;
    float layer{0.0f};
};

struct TiledObject {

};

struct Billboard {
    inline Billboard(const sf::Sprite& sprite): 
        Sprite(sprite)
	{ }

    sf::Sprite Sprite;
    bool ShouldRemove{false};

    std::string Uuid{""}; // Corrisponds to the unique table id
};

struct TiledMap : public sf::Drawable, public sf::Transformable {
    ~TiledMap();

    bool loadFromFile(const std::string& path, PhysicsFilter* physics, std::shared_ptr<EntityWorld>& world, std::shared_ptr<sol::state>& lua);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    void Update(const SkyTime& time);

    inline int getWidth() const { return width; }
    inline int getHeight() const { return height; }

    inline sol::table GetMetaData() { return meta_data; }
    inline Tileset GetFirstTileset() {
        if (tilesets.size() == 0) {
            std::cout << "Tried to get a non existant tileset from the tiled map" << std::endl;
            return Tileset{};
        }
        return tilesets[0];
    }

    void Destroy();
    void AddBillboard(const sf::IntRect& region, sf::Vector2f position);
    void AddEntity();
    std::vector<std::shared_ptr<Billboard>> GetBillboards();

private:
    std::vector<TiledLayer*> layers;
    std::vector<TiledLayer*> foreground_layers;
    std::vector<Tileset> tilesets;
    std::vector<std::shared_ptr<Billboard>> billboards;

    int width{0}, height{0};
    int tilewidth{0}, tileheight{0};

    std::string base64_decode(std::string const& encoded_string);
    std::string meta_data_file_name{""};
    sol::table meta_data;

    std::shared_ptr<sol::state> lua;
};


#endif//SKYVAULT_TILED_MAP_H
