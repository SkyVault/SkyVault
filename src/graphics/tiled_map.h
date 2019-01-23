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

struct EntitySpawn {
    sf::Vector2f Position;
    std::string EntityName{""};
    bool ShouldRemove{false};

    std::string Uuid{""}; // Corrisponds to the unique table id

};

struct TiledMap : public sf::Drawable, public sf::Transformable {
    ~TiledMap();

    bool loadFromFile(const std::string& path, PhysicsFilter* physics, std::shared_ptr<EntityWorld>& world, std::shared_ptr<sol::state>& lua);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void DrawForeground(sf::RenderWindow& target) const;

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
    void AddBillboard(const sf::IntRect& region, sf::Vector2f position, bool foreground=false);
    void AddEntitySpawn(const std::string& which, float x, float y);

    std::vector<std::shared_ptr<Billboard>> GetBillboards();
    std::vector<std::shared_ptr<EntitySpawn>> GetEntitySpawns();

    inline int WidthInPixels() const { return width * tilewidth; }
    inline int HeightInPixels() const { return height * tileheight; }

private:
    std::vector<TiledLayer*> layers;
    std::vector<TiledLayer*> foreground_layers;
    std::vector<Tileset> tilesets;

    std::vector<std::shared_ptr<Billboard>> billboards;
    std::vector<std::shared_ptr<Billboard>> foreground_billboards;

    // NOTE(Dustin): We don't have to keep this in the final build of the game
    std::vector<std::shared_ptr<EntitySpawn>> entity_spawns;

    int width{0}, height{0};
    int tilewidth{0}, tileheight{0};

    std::string base64_decode(std::string const& encoded_string);
    std::string meta_data_file_name{""};
    sol::table meta_data;

    std::shared_ptr<sol::state> lua;
};


#endif//SKYVAULT_TILED_MAP_H
