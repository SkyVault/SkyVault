#ifndef SKYVAULT_ENTITY_H
#define SKYVAULT_ENTITY_H

#include "component.h"
#include <memory>
#include <map>
#include <type_traits>
#include <iostream>
#include <initializer_list>
#include <typeindex>
#include <vector>

struct EntityWorld;

struct Entity {
    friend EntityWorld;

    Entity(){ 
        static int UUID{0};
        this->uuid = UUID++; 
    }

    template<typename... Args>
    void AddTags(Args&&... args) {
        (tags.push_back(args), ...);
    } 

    auto GetTags() { return tags; }

    template<typename T, typename... Args>
    void Add(Args&&... args) {
        components[typeid(T)] = std::make_unique<T>(std::forward<Args>(args)...);
        matchlist.push_back(typeid(T));

        component_names.push_back(std::string{typeid(T).name()});
    }

    template<typename T>
    T* Get() {
        if (components.find(typeid(T)) == components.end()) {
            std::cout << "Entity does not have the component: " << typeid(T).name() << std::endl;
            return nullptr;
        }
        auto* c = components[typeid(T)].get();
        return (T*)(c);
    }

    template<typename T>
    bool Has() {
        return (components.find(typeid(T)) != components.end());
    }

    std::vector<std::type_index>& GetMatchlist() { return matchlist; }

    void Kill();
    void Revive();

    inline int GetUUID() { return uuid; }
    inline bool Dead() { return remove; }

    inline std::map<std::type_index, std::unique_ptr<Component>>& GetComponents() { return components; }
    inline std::vector<std::string>& GetComponentNames() {return component_names;}

private:
    int uuid{0};
    std::map<std::type_index, std::unique_ptr<Component>> components;
    std::vector<std::type_index> matchlist;
    std::vector<std::string> component_names;
    std::vector<std::string> tags;

    bool loaded{false};
    bool remove{false};
};

#endif//SKYVAULT_ENTITY_H
