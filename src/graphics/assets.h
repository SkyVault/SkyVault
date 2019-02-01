#ifndef SKYVAULT_ASSETS_H
#define SKYVAULT_ASSETS_H
#include <map>
#include <SFML/Graphics.hpp>
#include <string>
#include <type_traits>
#include <sol.hpp>
#include <iostream>
#include <mutex>

#include "animation.h"

struct Assets {
private:
	Assets(const Assets&) = delete;
	Assets& operator=(const Assets&) = delete;

	static std::unique_ptr<Assets> instance;
	static std::once_flag onceFlag;

    Assets() {

    }

public:
    static auto* It() {
        std::call_once(Assets::onceFlag, [] () {
            instance.reset(new Assets());
        });

        return instance.get();
    }

    inline void GiveLua(std::shared_ptr<sol::state>& lua) {
        this->lua = lua;
    }

    void LoadPrefabs();
    void LoadAnimations();
    void LoadDialogs();

    template <typename T>
    void Add(const std::string& id, T& t) {
        //TODO(Dustin): Handle tables and other types
        if constexpr (std::is_same<T, sf::Texture*>()) {
            images[id] = t;
        } else if constexpr (std::is_same<T, sf::Font*>()) {
            fonts[id] = t;
        } else {
            std::cout << "Assets::Add:: Added unsupported type of asset" << std::endl;
        }
    }

    template <typename T>
    T* Get(const std::string& id) {
        if constexpr (std::is_same<T, sf::Texture>()) {
            if (images.find(id) == images.end()) {
                std::cout << "Cannot find texture: " << id << std::endl;
                return nullptr;
            }
            return images[id];
        } else if constexpr (std::is_same<T, sf::Font>()) {
            return fonts[id];
        }
    }

    bool HasImage(const std::string& id) {
        return images.find(id) != images.end();
    }

    sol::table GetDialog(const std::string& name) {
        if (dialogs.find(name) == dialogs.end()) {
            std::cout << "Cannot find dialog: " << name << std::endl;
            return sol::table();
        }
        return dialogs[name];
    }

    Animation* GetAnimation(const std::string& name) {
        if (animations.find(name) == animations.end()) {
            std::cout << "Cannot find animation: " << name << std::endl;
            return nullptr;
        }
        return animations[name];
    }

    sol::table GetPrefab(const std::string& name) {
        if (entity_prefabs.find(name) == entity_prefabs.end()) {
            std::cout << "Cannot find prefab: " << name << std::endl;
            return sol::table{};
        }
        return entity_prefabs[name];
    }

    inline std::map<std::string, sol::table>& GetPrefabs() {return entity_prefabs;}

private:
    std::map<std::string, sf::Texture*> images;
    std::map<std::string, sf::Font*> fonts;

    std::map<std::string, sol::table> entity_prefabs,
                                      dialogs;

    std::map<std::string, Animation*> animations;

    std::shared_ptr<sol::state> lua;
};
#endif//SKYVAULT_ASSETS_H
