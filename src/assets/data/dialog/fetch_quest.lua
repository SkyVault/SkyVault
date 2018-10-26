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
            "Okay go get that carrot!",
            -1,
            startQuest = "FetchQuestTest"
        },

        [4] = {
            "Well thats to bad :(",
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
