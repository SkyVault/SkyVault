#include "assets.h"
#include <crefile.hpp>

void Assets::LoadPrefabs() {

    constexpr auto PrefabPath{"assets/prefabs/"};

    for (auto file : crefile::iter_dir(PrefabPath)) {
        const sol::table table = lua->script_file(crefile::join(PrefabPath, file.name()));

        table.for_each([&](sol::object const& key, sol::object const& value) {
            const auto skey = key.as<std::string>();
            std::cout << skey << std::endl;

            const auto entPrefab = value.as<sol::table>();
            entity_prefabs[skey] = entPrefab;
        });
    }

    //for(auto& p: fs::directory_iterator(PrefabPath)) {
        //std::cout << p << std::endl; 
    //}

}

void Assets::LoadDialogs() {
    constexpr auto DialogPath{"assets/data/dialog/"};

    for (auto file : crefile::iter_dir(DialogPath)) {
        const sol::table table = lua->script_file(crefile::join(DialogPath, file.name()));

        table.for_each([&](sol::object const& key, sol::object const& value) {
            const auto skey = key.as<std::string>();
            std::cout << skey << std::endl;

            const auto entPrefab = value.as<sol::table>();
            dialogs[skey] = entPrefab;
        });
    }
}

void Assets::LoadAnimations() {
    constexpr auto PrefabPath{"assets/data/animations/"};

    for (auto file : crefile::iter_dir(PrefabPath)) {
        const sol::table table = lua->script_file(crefile::join(PrefabPath, file.name()));
        
        table.for_each([&](sol::object const& key, sol::object const& value) {
            auto anim_name = key.as<std::string>();
            auto anim_table = value.as<sol::table>();

            Animation::Playback playback = Animation::Playback::Forward;
            bool repeated = true;
            std::vector<Frame> frames;

            anim_table.for_each([&](sol::object const& key, sol::object const& value) {
                auto property = key.as<std::string>();

                if (property == "playback") {
                    auto v = value.as<std::string>();
                    if (v == "forward"){}
                    else if (v == "backward")
                        playback = Animation::Playback::Backward;
                    else if (v == "bounce")
                        playback = Animation::Playback::Bounce;
                    else{
                        std::cout << "Unknown playback type (" << v << ") for animation (" << anim_name << ")\n";
                    }
                } else if (property == "repeated") {
                    repeated = value.as<bool>();
                } else if (property == "frames") { 
                    auto frames_table = value.as<sol::table>();
                    frames_table.for_each([&](sol::object const& key, sol::table const& frame_table) {

                        sol::table region_table = frame_table[1];
                        float duration = frame_table["duration"];

                        int rx = region_table[0 + 1];
                        int ry = region_table[1 + 1];
                        int rw = region_table[2 + 1];
                        int rh = region_table[3 + 1];

                        frames.push_back(Frame(rx, ry, rw, rh, duration));
                    });
                } else {
                    std::cout << "Unknown property (" << property << ") for animation (" << anim_name << ")\n";
                    return;
                }
            });

            auto animation = new Animation(frames);
            animations.insert({anim_name, animation});
        });
    }

}
