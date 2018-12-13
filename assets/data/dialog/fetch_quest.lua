--[[
-- NOTE(Dustin): Instead of having a startQuest thing, we could have each dialog
-- return a string value, and that value will go into the callback?
--]]
return {
    preQuest = {
        [1] = {
            "Hello player! This is a test fetch quest, oh goodie!",
            2 
        },

        [2] = {
            "Would you like to do the quest?",
            {{"yes", 3}, {"no", 4}}
        },
        
        [3] = {
            "Okay go get that floppy disk!",
            -1,
            startQuest = "FloppyDiskQuest"
        },

        [4] = {
            "Well thats to bad :(",
            -1
        }
    },

    duringQuest = {
        [1] = {
            "Still waiting on that floppy disk...",
            -1
        }
    },

    postQuest = {
        [1] = {
            "You did it! I now can enjoy this carrot",
            -1
        }
    }
}
