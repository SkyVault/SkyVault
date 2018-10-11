#include "tiled_map.h"
#include "../skyvault.h"
#include <iostream>
#include <sstream>

bool TiledMap::loadFromFile(const std::string& path, PhysicsFilter* physics) {
    using namespace tinyxml2;

    if (physics == nullptr) {
        std::cout << "TiledMap::loadFromFile:: PhysicsFilter is null" << std::endl;
    }

    XMLDocument doc;
    if (doc.LoadFile(path.c_str()) != XML_SUCCESS){
        std::cout << "Failed to load tiled map " << path << std::endl;
    }

    let map_xml = doc.FirstChildElement("map");
    //let map_properties_xml = map_xml->FirstChildElement("properties");
    //std::cout << map_properties_xml->FirstChildElement()->Attribute("name") << std::endl;
    //[>map_properties_xml->Ea<]
    map_xml->QueryIntAttribute("width",         &width);
    map_xml->QueryIntAttribute("height",        &height);
    map_xml->QueryIntAttribute("tilewidth",     &tilewidth);
    map_xml->QueryIntAttribute("tileheight",    &tileheight);

    auto* child = map_xml->FirstChildElement();
    while(child != nullptr){
        let name = child->Value();
        
        if (strcmp(name, "properties") == 0) {
            // Handle properties of the map
        } else if (strcmp(name, "tileset") == 0) {
            // Load the tileset
            if (child->Attribute("source") != nullptr) {
                std::cout << "Load tileset file" << std::endl;
            } else {

                Tileset tileset;
                tileset.name = std::string{child->Attribute("name")};
                child->QueryIntAttribute("tilewidth", &tileset.tilewidth);
                child->QueryIntAttribute("tileheight", &tileset.tileheight);

                let image = child->FirstChildElement("image");
                if (image != nullptr) {
                    let source = "assets/maps/" + std::string{image->Attribute("source")};
                    if (!tileset.image.loadFromFile(source)) {
                        std::cout << "Tiled map failed to load tileset image: " << source << std::endl;
                    }
                }
            
                tilesets.push_back(tileset);
            }
        } else if (strcmp(name, "layer") == 0) {
            // Push a new tile layer
            
            let id = std::string{child->Attribute("id")};
            let name = std::string{child->Attribute("name")};
            
            let data = child->FirstChildElement("data");
            if (std::string{data->Attribute("encoding")} != "csv") {
                std::cout << "Error we only support csv encoding for now..." << std::endl;
            }
            
            let text = std::string{data->GetText()};
            //var it = text.begin();

            var* layer = new TiledLayer();
            layer->name = name;
            layer->data.resize(width * height);
            int ptr{0};

            layer->vertices.setPrimitiveType(sf::Quads);
            layer->vertices.resize(width * height * 4);

            var stream = std::stringstream(text);

            int i = 0;
            while (stream >> i) {
                layer->data[ptr++] = i;
                if (stream.peek() == ',' || stream.peek() == '\n')
                    stream.ignore();
            }

            for (auto i = 0; i < width; i++) {
                for (auto j = 0; j < height; j++) {
                    int tileNumber = layer->data[i + j * width];

                    if (tileNumber == 0) continue;
                    tileNumber--;

                    if (tilesets.size() == 0) {
                        std::cout << "Missing tileset" << std::endl;
                        return false;
                    }

                    let m_tileset = tilesets[0].image;

                    // find its position in the tileset texture
                    int tu = tileNumber % (m_tileset.getSize().x / tilewidth);
                    int tv = tileNumber / (m_tileset.getSize().x / tilewidth);

                    // get a pointer to the current tile's quad
                    sf::Vertex* quad = &layer->vertices[(i + j * width) * 4];

                    // define its 4 corners
                    quad[0].position = sf::Vector2f(i * tilewidth, j * tileheight);
                    quad[1].position = sf::Vector2f((i + 1) * tilewidth, j * tileheight);
                    quad[2].position = sf::Vector2f((i + 1) * tilewidth, (j + 1) * tileheight);
                    quad[3].position = sf::Vector2f(i * tilewidth, (j + 1) * tileheight);

                    // define its 4 texture coordinates
                    quad[0].texCoords = sf::Vector2f(tu * tilewidth, tv * tileheight);
                    quad[1].texCoords = sf::Vector2f((tu + 1) * tilewidth, tv * tileheight);
                    quad[2].texCoords = sf::Vector2f((tu + 1) * tilewidth, (tv + 1) * tileheight);
                    quad[3].texCoords = sf::Vector2f(tu * tilewidth, (tv + 1) * tileheight);
                }
            }

            layers.push_back(layer);

        } else if (strcmp(name, "objectgroup") == 0) {
            // Handle tiled objects
            auto* objectXml = child->FirstChildElement();
            while (objectXml != nullptr) {
                float ox{0.0f}, oy{0.0f}, owidth{0.0f}, oheight{0.0f};

                objectXml->QueryFloatAttribute("x",         &ox);
                objectXml->QueryFloatAttribute("y",         &oy);
                objectXml->QueryFloatAttribute("width",     &owidth);
                objectXml->QueryFloatAttribute("height",    &oheight);

                if (objectXml->Attribute("type") == nullptr) {
                    physics->AddSolid(ox, oy, owidth, oheight);
                } else {
                    const auto type = std::string{objectXml->Attribute("type")};

                    //if (type == "") {
                    //}

                }
                objectXml = objectXml->NextSiblingElement();
            }
        }
        
        child = child->NextSiblingElement();
    }

    //setScale(2, 2);

    //std::cout << map_xml->Attribute("version") << std::endl;
    return true;
}

void TiledMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (tilesets.size() == 0) {
        std::cout << "Missing tileset" << std::endl;
        return;
    }

    // apply the transform
    states.transform *= getTransform();

    // apply the tileset texture
    states.texture = &tilesets[0].image;

    // draw the vertex array
    for (auto* layer : layers){
        target.draw(layer->vertices, states);
    }
}

