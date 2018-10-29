return {
    ['BlueDiamond'] = {
        tags = {},
        components = {
            Body = {width = 16, height = 16},
            Sprite = {
                texture = "items",
                region = { 163, 37, 16, 16 }
            },
            Item = {}, -- Item adds a physics component, this might not be good design...
        }
    },

    ['Heart'] = {
        tags = {},
        components = {
            Body = {width = 16, height = 16},
            Sprite = {
                texture = "items",
                region = { 236, 2, 15, 12 }
            },
            Item = {},
        }

    },

    ['Potion'] = {
        tags = {},
        components = {
            Body = {width = 16, height = 16},
            Sprite = {
                texture = "items",
                region = { 218, 2, 14, 14 }
            },
            Item = {},
        }
    },

    ['FloppyDisk'] = {
        tags = {},
        components = {
            Body = {width = 16, height = 16},
            Sprite = {
                texture = "items",
                region = { 199, 73, 16, 16 }
            },
            Item = {},
        }
    }
}
