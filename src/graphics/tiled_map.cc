#include "tiled_map.h"
#include "../skyvault.h"
#include <iostream>
#include <sstream>

TiledMap::~TiledMap() {
    this->Destroy();
}

std::string trim(const std::string& str,
                 const std::string& whitespace = " \t\n")
{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

bool TiledMap::loadFromFile(const std::string& path, PhysicsFilter* physics, std::shared_ptr<EntityWorld>& world, std::shared_ptr<sol::state>& lua) {
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

                    //TODO(Dustin): @Important check to see if the image already
                    // exists in the assets cache

                    if (Assets::It()->HasImage(tileset.name)) {
                        tileset.image = Assets::It()->Get<sf::Texture>(tileset.name);
                    } else {
                        tileset.image = new sf::Texture();
                        if (!tileset.image->loadFromFile(source)) {
                            std::cout << "Tiled map failed to load tileset image: " << source << std::endl;
                        }
                        Assets::It()->Add(tileset.name, tileset.image);
                    }

                }

                tilesets.push_back(tileset);
            }
        } else if (strcmp(name, "layer") == 0) {
            // Push a new tile layer

            let id = std::string{child->Attribute("id")};
            let name = std::string{child->Attribute("name")};

            let data = child->FirstChildElement("data");
            auto text = std::string{data->GetText()};
            std::vector<unsigned char> decompressed;
            decompressed.resize(width*height*4);

            const auto encoding = std::string{data->Attribute("encoding")};
            bool isCompressed{false};

            if (encoding == "base64") {
                if (data->Attribute("compression") != nullptr) {
                    if (std::string{data->Attribute("compression")} != "gzip") {
                        std::cout << "(Error)::TiledMap::loadFromFile:: We only support csv and gzip encoding, the map: " << path << " uses: " << encoding << ".\n";
                        return false;
                    }
                } else {
                    std::cout << "Currently we only support gzip compressed base64, not raw base64 (ABORTING)\n";
                    assert(0);
                }

                text = trim(text);
                text = base64_decode(text);

                isCompressed = gzip::is_compressed(text.data(), text.size());

                auto s = gzip::decompress(text.data(), text.size());
                int i = 0;
                for (unsigned char c : s)
                    decompressed[i++] = c;
            }

            var* layer = new TiledLayer();
            layer->name = name;
            layer->data.resize(width * height);
            int ptr{0};

            layer->vertices.setPrimitiveType(sf::Quads);
            layer->vertices.resize(width * height * 4);

            if (!isCompressed) {
                var stream = std::stringstream(text);
                int i = 0;
                while (stream >> i) {
                    layer->data[ptr++] = i;
                    if (stream.peek() == ',' || stream.peek() == '\n')
                        stream.ignore();
                }
            } else {
                int ptr = 0;
                for (auto i = 0u; i < (decompressed.size()); i+=4u) {
                    layer->data[ptr++] =
                          decompressed[i + 0]
                        | decompressed[i + 1] << 8
                        | decompressed[i + 2] << 16
                        | decompressed[i + 3] << 24;
                }
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
                    int tu = tileNumber % (m_tileset->getSize().x / tilewidth);
                    int tv = tileNumber / (m_tileset->getSize().x / tilewidth);

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

                    if (type == "Door") {
                        std::cout
                            << "We don't load doors using the tiled map anymore, please use the in game editor."
                            << "\n";
                    }

                }
                objectXml = objectXml->NextSiblingElement();
            }
        }
        child = child->NextSiblingElement();
    }

    // TODO(Dustin): We are going to need to rewrite the majority of this.
    // It's very dangerous and needs to be rewritten to be safer.

    // Load billboards and entities
    std::string pcopy = path;
    auto beg = path.begin();
    auto end = path.end();
    while (end != beg) {
        if (*end == '.'){ break; }
        --end;
    }

    std::string file_name = std::string(beg, end);
    const std::string data_file_path = file_name + ".data.lua";

    meta_data_file_name = data_file_path;

        std::string code = R"(
return {
    billboard_regions = {},
    billboards = {},
    solids = {},
    doors = {},
    entities = {},
}
        )";

    auto set_meta_data_to_code = [&]() -> bool{
        const auto result = lua->script(code, &sol::script_default_on_error);
        if (result.valid()) {
            meta_data = result.get<sol::table>(0);
        } else {
            std::cout << "Failed to retrieve the table from the data file for map: " << file_name << std::endl;
            return false;
        }
        return true;
    };

    if (!file_exists(data_file_path)) {
        std::ofstream file(data_file_path);
        if (!file) {
            std::cout << "Failed to create the data file for the map: " << file_name << std::endl;
            return false;
        }

        file << code;
        file.close();

        if (!set_meta_data_to_code()) { return false; }

    } else {
        // Load the script and the data associated with it
        meta_data = lua->script_file(data_file_path);
        if (meta_data["billboard_regions"].valid()){
        }else{
            if (!set_meta_data_to_code()) { return false; }
        }

        // Load the entities
        if (meta_data["entities"].valid()) {
            auto entities_table = meta_data.get<sol::table>("entities");

            entities_table.for_each([&](const sol::object& key, const sol::object& value){
                const auto entity_table = value.as<sol::table>();
                const auto which = entity_table.get<std::string>(1);
                const auto x = entity_table.get<float>(2);
                const auto y = entity_table.get<float>(3);

                const std::string uuid = (*lua)["getTableAddress"](entity_table);
                auto sh = std::make_shared<EntitySpawn>();
                sh->Position = sf::Vector2f(x, y);
                sh->EntityName = which;
                sh->Uuid = uuid;

                entity_spawns.push_back(sh);

                const auto prefab = Assets::It()->GetPrefab(which);
                auto entity = world->Create(prefab);
                if (entity->Has<Body>()) {
                    entity->Get<Body>()->Position = sf::Vector2f(x, y);
                }
            });
        } else {
            std::cout << "Error::TiledMap::loadTiledMap:: Failed to load the entities from the data script, entities table entry is invalid, map: " << path << std::endl;
        }

        // Load the doors
        if (meta_data["doors"].valid()) {
            auto doors_table = meta_data.get<sol::table>("doors");
            doors_table.for_each([&](const sol::object& key, const sol::object& value) {

                const auto door_table = value.as<sol::table>();
                const auto dx = door_table.get<float>(1);
                const auto dy = door_table.get<float>(2);
                const auto dw = door_table.get<float>(3);
                const auto dh = door_table.get<float>(4);
                const auto to = door_table.get<std::string>(5);

                const auto uuid = (*lua)["getTableAddress"](door_table);

                // Create the doors entity
                world->AddDoor(uuid, to, dx, dy, dw, dh);
            });
        }

        // Load the billboards
        if (meta_data["billboards"].valid()){
            auto billboards_table = meta_data.get<sol::table>("billboards");
            billboards_table.for_each([&](const sol::object& key, const sol::object& value){
                const auto billboard_table = value.as<sol::table>();

                const auto rx = billboard_table.get<int>(1);
                const auto ry = billboard_table.get<int>(2);
                const auto rw = billboard_table.get<int>(3);
                const auto rh = billboard_table.get<int>(4);
                const auto x = billboard_table.get<float>(5);
                const auto y = billboard_table.get<float>(6);
                const auto fore = billboard_table.get<bool>(7);

                sf::Sprite sprite;
                sprite.setTexture(*tilesets[0].image);
                sprite.setTextureRect(sf::IntRect(rx, ry, rw, rh));
                sprite.setPosition(sf::Vector2f(x, y));

                const std::string uuid = (*lua)["getTableAddress"](billboard_table);
                auto sh = std::make_shared<Billboard>(sprite);
                sh->Uuid = uuid;

                if (!fore)
                    billboards.push_back(sh);
                else
                    foreground_billboards.push_back(sh);
            });
        } else {
            std::cout << "Error::TiledMap::loadTiledMap:: Failed to load the billboards from the data script, billboards table entry is invalid, map: " << path << std::endl;
        }
    }

    this->lua = lua;
    return true;
}

void TiledMap::AddBillboard(const sf::IntRect& region, sf::Vector2f position, bool foreground){
    if (meta_data["billboards"].valid()) {
        auto t  = meta_data.get<sol::table>("billboards");
        sol::table b = lua->create_table();
        b[1 + 0] = region.left;
        b[1 + 1] = region.top;
        b[1 + 2] = region.width;
        b[1 + 3] = region.height;
        b[1 + 4] = position.x;
        b[1 + 5] = position.y;
        b[1 + 6] = foreground;
        t.add(b);

        sf::Sprite sprite;
        sprite.setTexture(*tilesets[0].image);
        sprite.setTextureRect(region);
        sprite.setPosition(position);

        const std::string uuid = (*lua)["getTableAddress"](b);
        auto sh = std::make_shared<Billboard>(sprite);
        sh->Uuid = uuid;

        if (!foreground)
            billboards.push_back(sh);
        else
            foreground_billboards.push_back(sh);
    }
}

void TiledMap::AddDoor(std::shared_ptr<EntityWorld>& world, std::string to, float x, float y, float w, float h) {
    auto t  = meta_data.get<sol::table>("doors");
    sol::table d = lua->create_table();
    d[1 + 0] = x;
    d[1 + 1] = y;
    d[1 + 2] = w;
    d[1 + 3] = h;
    d[1 + 4] = to;
    t.add(d);

    const std::string uuid = (*lua)["getTableAddress"](d);
    world->AddDoor(uuid, to, x, y, w, h);
}

void TiledMap::AddEntitySpawn(const std::string& which, float x, float y) {
    auto es = std::make_shared<EntitySpawn>();
    es->EntityName = which;
    es->Position = sf::Vector2f(x, y);

    auto t  = meta_data.get<sol::table>("entities");
    sol::table e = lua->create_table();
    e[1 + 0] = which;
    e[1 + 1] = x;
    e[1 + 2] = y;
    t.add(e);

    es->Uuid = (*lua)["getTableAddress"](e);

    entity_spawns.push_back(es);
}

void TiledMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (tilesets.size() == 0) {
        std::cout << "Missing tileset" << std::endl;
        return;
    }

    // apply the transform
    states.transform *= getTransform();

    // apply the tileset texture
    states.texture = tilesets[0].image;

    // draw the vertex array
    for (auto* layer : layers){
        target.draw(layer->vertices, states);
    }

    for (const auto& billboard : billboards) {
        auto position = billboard->Sprite.getPosition();
        Art::It()->Draw(billboard->Sprite, position.y);
    }

    for (const auto& billboard : foreground_billboards) {
        auto position = billboard->Sprite.getPosition();
        Art::It()->Draw(billboard->Sprite, 1000 + position.y);
    }
}

void TiledMap::Update(const SkyTime& time) {
    auto RemoveBillboardFromMetaData = [&](sol::table& table, const auto& item) {
        auto result = (*lua)["removeIfMatchingAddress"](table, item->Uuid);
        if (result.valid()){
            if ((bool)result) {
                std::cout << "Successfully removed table" << std::endl;
            } else {
                std::cout << "Failed to removed table" << std::endl;
            }
        }
    };

    if (entity_spawns.size() == 1)  {
        if (entity_spawns[0]->ShouldRemove){
            auto t = meta_data.get<sol::table>("entities");
            RemoveBillboardFromMetaData(t,entity_spawns[0]);
            entity_spawns.clear();
        }
    } else {
        // TODO(Dustin): Find out why we need to return
        auto it = entity_spawns.begin();
        while (it != entity_spawns.end()){
            if ((*it)->ShouldRemove){
                auto t = meta_data.get<sol::table>("entities");
                RemoveBillboardFromMetaData(t,*it);
                it = entity_spawns.erase(it);
                return;
            }
            ++it;
        }
    }

    // TODO(Dustin): Refactor
    if (billboards.size() == 1)  {
        if (billboards[0]->ShouldRemove){
            auto t = meta_data.get<sol::table>("billboards");
            RemoveBillboardFromMetaData(t,billboards[0]);
            billboards.clear();
        }
    } else {
        // TODO(Dustin): Find out why we need to return
        auto it = billboards.begin();
        while (it != billboards.end()){
            if ((*it)->ShouldRemove){
                auto t = meta_data.get<sol::table>("billboards");
                RemoveBillboardFromMetaData(t,*it);
                it = billboards.erase(it);
                continue;
            }
            ++it;
        }
    }

    if (foreground_billboards.size() == 1)  {
        if (foreground_billboards[0]->ShouldRemove){
            auto t = meta_data.get<sol::table>("billboards");
            RemoveBillboardFromMetaData(t,foreground_billboards[0]);
            foreground_billboards.clear();
        }
    } else {
        // TODO(Dustin): Find out why we need to return
        auto it = foreground_billboards.begin();
        while (it != foreground_billboards.end()){
            if ((*it)->ShouldRemove){
                auto t = meta_data.get<sol::table>("billboards");
                RemoveBillboardFromMetaData(t,*it);
                it = foreground_billboards.erase(it);
                return;
            }
            ++it;
        }
    }
}

std::string TiledMap::base64_decode(std::string const& encoded_string) {
    const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

    auto is_base64 =
        [](unsigned char c)->bool {
        return (static_cast<bool>(isalnum(c)) || (c == '+') || (c == '/'));
    };

    auto in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;

    while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i == 4) {
            for (i = 0; i < 4; i++) {
                char_array_4[i] = static_cast<unsigned char>(base64_chars.find(char_array_4[i]));
            }
            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++) {
                ret += char_array_3[i];
            }
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 4; j++) {
            char_array_4[j] = 0;
        }

        for (j = 0; j < 4; j++) {
            char_array_4[j] = static_cast<unsigned char>(base64_chars.find(char_array_4[j]));
        }

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) {
            ret += char_array_3[j];
        }
    }

    return ret;
}

void TiledMap::Destroy() {
    // TODO(Dustin): We are going to need to rewrite the majority of this.
    // It's very dangerous and needs to be rewritten to be safer.

    // Write the meta_data back to the file
    if (!lua) return;
    // TODO(Dustin): Explicitly handle errors and
    // make sure that result.get actually returns a string

    auto result = (*lua)["serialize"](meta_data);

    std::ofstream outFile(meta_data_file_name);
    if (!outFile) {
        std::cout << "Failed to open the meta_data_file for map: " << meta_data_file_name << std::endl;
        return;
    }

    if (result.valid() == false) {
        std::cout << "Result is not valid" << std::endl;
        outFile.close();
        return;
    }

    outFile << "return " << result.get<std::string>(0);
    outFile.close();

    if (layers.size() == 0) return;
    for (auto* layer : layers)
        delete layer;

    layers.clear();
    tilesets.clear();

    billboards.clear();
    foreground_billboards.clear();

    entity_spawns.clear();
}

std::vector<std::shared_ptr<Billboard>> TiledMap::GetBillboards(){
    std::vector<std::shared_ptr<Billboard>> all_billboards;

    for (auto& b : billboards)
        all_billboards.push_back(b);

    for (auto& b : foreground_billboards)
        all_billboards.push_back(b);

    return all_billboards;
}

std::vector<std::shared_ptr<EntitySpawn>> TiledMap::GetEntitySpawns() {
    return entity_spawns;
}
