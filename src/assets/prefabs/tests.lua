return {
    ["Bird"] = {
        tags = {"Hello", "World"},
        components = {
            Body = {width = 100, height = 100},
            AnimatedSprite = {
                texture = "bird",
                animation = "Bird"
            }
        }
    },

    ["Skelly"] = { 
        components = {
            Body = {x = 500 + 32, y = 400-64, width = 32, height = 16},
            PhysicsBody = {},
            Sprite = {
                texture = "enemies",
                region = {64, 16, 32, 32},
                offset = {0, -16},
            },

            Interaction = {
                fn = function()
                    if (not QuestEngine_IsCompleted ("FloppyDiskQuest")) and 
                       (not QuestEngine_WorkingOnQuest ("FloppyDiskQuest")) then
                        Gui_DoDialog "preQuest"
                   else
                        if QuestEngine_WorkingOnQuest "FloppyDiskQuest" then
                            Gui_DoDialog "duringQuest"
                        elseif QuestEngine_IsCompleted "FloppyDiskQuest" then
                            Gui_DoDialog "postQuest"
                        end
                   end
                end
            }
        }
    }
}
