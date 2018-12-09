#include "quest_engine.h"

QuestEngine::QuestEngine() {
    quests["FloppyDiskQuest"] = new Quest("FloppyDiskQuest", [](std::shared_ptr<EntityWorld>& world) {
        auto player_opt = world->GetFirstWith<Player>();

        if (!player_opt) return false;
        auto player = player_opt.value();

        for (const auto& item : player->Get<Player>()->Invatory) {
            if (item.Name == "FloppyDisk")
                return true;
        }
        return false;
    });
}

bool QuestEngine::WorkingOnQuest(const std::string& which) {
    return HAS_KEY(active_quests, which);
}

bool QuestEngine::IsCompleted(const std::string& which) {
    return HAS_KEY(completed_quests, which);
}

void QuestEngine::Update(const SkyTime& time, std::shared_ptr<EntityWorld>& world) {
    for (auto [key, quest] : active_quests) {
        if (quest->test(world)) {
            quest->completed = true;

            // Move to completed quests
            active_quests.erase(active_quests.find(key));
            completed_quests[key] = quest; 

            // TODO(Dustin): @Fix this return statement prevents the rest of the potentially completed quests this frame from
            // being completed till the next. This might not be an issue but better fix it just in case
            return;
        }
    }
}

void QuestEngine::StartQuest(const std::string& which) {
    if (!HAS_KEY(quests, which)){
        std::cout << "QuestEngine::StartQuest:: Failed to start quest: " << which << ", doesn't exist" << std::endl;
        return;
    } else {
        std::cout << "QuestEngine::StartQuest:: Starting quest: " << which << std::endl;
    }

    auto* quest = quests[which];
    quests.erase(quests.find(which));
    active_quests[which] = quest; 
}
