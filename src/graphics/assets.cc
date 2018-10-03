#include "assets.h"
#include <crefile.hpp>

void Assets::LoadPrefabs() {

    constexpr auto PrefabPath{"assets/prefabs/"};

    for (auto file : crefile::iter_dir(PrefabPath)) {
        std::cout << file.name() << "\n";

        const sol::table table = lua->script_file(crefile::join(PrefabPath, file.name()));

        table.for_each([&](sol::object const& key, sol::object const& value) {
            const auto skey = key.as<std::string>();
            std::cout << skey << std::endl;

            const auto entPrefab = value.as<sol::table>();
            entity_prefabs[skey] = entPrefab;

            //entPrefab.for_each([&](sol::object const& _key, sol::object const& _value){
                //const auto _skey = _key.as<std::string>();
                //std::cout << "  " << _skey << std::endl;
            //});
        });
    }

    //for(auto& p: fs::directory_iterator(PrefabPath)) {
        //std::cout << p << std::endl; 
    //}

}

void Assets::LoadAnimations() {
    constexpr auto PrefabPath{"assets/data/animations/"};

    for (auto file : crefile::iter_dir(PrefabPath)) {
        
    }

}
