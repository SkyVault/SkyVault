#include "entity_world.h"
#include "../skyvault.h"

Entity* EntityWorld::Create() {
    var entity = new Entity();
    entities.push_back(std::unique_ptr<Entity>(entity));
    return entity;
}

void EntityWorld::Update(const SkyTime& time) {
    for (auto& e : entities) {
        for (auto& [key, f] : filters) {
            if (f->Matches(e->GetMatchlist())) {
                if (!e->loaded) 
                    f->Load(e);
                if (e->remove)
                    f->Destroy(e);
                else
                    f->Update(time, e);
            }
        }

        e->loaded = true;
    }
}

void EntityWorld::Render(std::unique_ptr<sf::RenderWindow>& window) {
    for (auto& e : entities) {
        for (auto& [key, f] : filters) {
            if (f->Matches(e->GetMatchlist())) {
                f->Render(window, e);
            }
        }
    }

    for (auto& [key, f] : filters) {
        f->PostRender(window);
    }
}
