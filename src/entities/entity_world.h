#ifndef SKYVAULT_ENTITY_WORLD_H
#define SKYVAULT_ENTITY_WORLD_H

#include "components/body.h"
#include "../utilities/input.h"

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include <typeindex>
#include <sol.hpp>
#include <optional>

#include "entity.h"
#include "filter.h"
#include "../entities/components/renderable.h"
#include "../entities/components/ai.h"
#include "../skytime.h"
#include "../skyvault.h"
#include "../graphics/assets.h"

#define ENTITY_LIST_LENGTH (2048)

using EntityID = int;

struct EntityWorld;
struct Door : Body{
    friend EntityWorld;

    inline Door(std::string _uuid, std::string _to, float x, float y, float w, float h):
        Body(x, y, w, h),
        To(_to),
        Uuid(_uuid)
    {}
    std::string To{""};

    // This is the lua table address used for removing
    std::string Uuid{""};

private:
    bool last{false};
};

struct EntityWorld {
    EntityWorld();
    ~EntityWorld();

    Entity* Create();
    Entity* Create(const sol::table& prefab);

    template <typename T, typename... Args>
    void Register(Args&&... args) {
        filters[typeid(T)] = (std::make_unique<T>(std::forward<Args>(args)...));
        filters[typeid(T)]->World = this;
    }

    template <typename T>
    T* GetFilter() {
        // This returns a raw pointer that shouldn't be stored anywhere, or freed
        return (T*)filters[typeid(T)].get();
    }

    void Update(const SkyTime& time);
    void Render(std::unique_ptr<sf::RenderWindow>& window);

    template<typename T>
    std::optional<Entity*> GetFirstWith() {
        for (int i = 0; i < entity_list_length; i++) {
            if (entity_list[i] != nullptr) {
                if (entity_list[i]->Has<T>()) {
                    return std::optional<Entity*>{entity_list[i]};
                }
            }
        }
        return std::nullopt;
    }

    template<typename T>
    inline std::vector<Entity*> GetAllWith(){
        std::vector<Entity*> result;
        for (int i = 0; i < entity_list_length; i++) {
            if (entity_list[i] != nullptr) {
                if (entity_list[i]->Has<T>())
                    result.push_back(entity_list[i]);
            }
        }

        return result;
    }

    template<typename T>
    inline std::vector<int> GetAllIdsWith(){
        std::vector<int> result;
        for (int i = 0; i < entity_list_length; i++) {
            if (entity_list[i] != nullptr) {
                if (entity_list[i]->Has<T>())
                    result.push_back(entity_list[i]->entity_id);
            }
        }
        return result;
    }

    inline std::vector<Entity*> GetAllWithTag(const std::string& tag){
        std::vector<Entity*> result;
        for (int i = 0; i < entity_list_length; i++) {
            if (entity_list[i] != nullptr) {
                if (entity_list[i]->HasTag(tag))
                    result.push_back(entity_list[i]);
            }
        }
        return result;
    }

    inline std::optional<Entity*> GetEntity(const int index) {
        if (index == -1) return std::nullopt;
        if (index > entity_list_length) return std::nullopt;
        if (entity_list[index] == nullptr) return std::nullopt;
        return std::optional<Entity*>{entity_list[index]};
    }

    // This shouldn't be a function
    inline std::vector<Entity*> GetEntities() {
        auto result = std::vector<Entity*>();
        for (int i = 0; i < entity_list_length; i++) {
            if (entity_list[i] != nullptr)
                result.push_back(entity_list[i]);
        }
        return result;
    }

    void AddDoor(const std::string& Uuid, const std::string& To, float x, float y, float width, float height);
    inline auto& GetDoors() {
        return doors;
    }

    void ClearAll();

    void OnRoomChange(std::function<void(std::string)> fn);

private:
    int add_entity(Entity* entity); // Returns the index of the entity

    std::map<std::type_index, std::unique_ptr<Filter>> filters;

    Entity** entity_list;
    int entity_list_length{ENTITY_LIST_LENGTH};
    int last_freed_place{0};

    std::vector<Entity*> combat_entities;

    std::vector<Door> doors;
    std::function<void(std::string)> on_room_change;

    // Interaction grid
    std::array<EntityID, MAP_SIZE*MAP_SIZE> grid;
};

#endif//SKYVAULT_ENTITY_WORLD_H
