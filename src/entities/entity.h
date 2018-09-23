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

    template<typename T, typename... Args>
    void Add(Args&&... args) {
        components[typeid(T)] = std::make_unique<T>(std::forward<Args>(args)...);
        matchlist.push_back(typeid(T));
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
    
    std::vector<std::type_index>& GetMatchlist() { return matchlist; }

    void Kill();
    void Revive();
    inline bool Dead() { return remove; }

private:
    std::map<std::type_index, std::unique_ptr<Component>> components;
    std::vector<std::type_index> matchlist;

    bool loaded{false};
    bool remove{false};
};

#endif//SKYVAULT_ENTITY_H
