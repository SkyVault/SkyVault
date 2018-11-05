#ifndef SKYVAULT_QUEST_ENGINE_H
#define SKYVAULT_QUEST_ENGINE_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <tuple>
#include <mutex>
#include <algorithm>
#include <functional>
#include <map>
#include <string>

#include "../entities/entity.h"
#include "../entities/entity_world.h"
#include "../entities/components/player.h"

typedef std::function<bool(std::shared_ptr<EntityWorld>&)> QuestTest; 

struct QuestEngine;
struct Quest {
    friend QuestEngine;

    inline Quest(const std::string& name, QuestTest _test): 
        test(_test), 
        name(name) 
    {};

    inline std::string GetName() const { return name; }
    inline bool IsCompleted() { return completed; }

private: 
    QuestTest test; 
    const std::string name;
    bool completed{false};
};

class QuestEngine {
	QuestEngine(const QuestEngine&) = delete;
	QuestEngine& operator=(const QuestEngine&) = delete;

	static std::unique_ptr<QuestEngine> instance;
	static std::once_flag onceFlag;

    QuestEngine();

public:
    static auto* It() {
        std::call_once(QuestEngine::onceFlag, [] () {
            instance.reset(new QuestEngine());     
        });

        return instance.get();
    }

    int testValue {13};

    void StartQuest(const std::string& which);
    void Update(const SkyTime& time, std::shared_ptr<EntityWorld>& world);

    bool WorkingOnQuest(const std::string& which);
    bool IsCompleted(const std::string& which);

private:

    std::map<std::string, Quest*> active_quests;
    std::map<std::string, Quest*> quests;
    std::map<std::string, Quest*> completed_quests;
};

#endif//SKYVAULT_QUEST_ENGINE_H
