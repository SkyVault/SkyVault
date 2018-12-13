local
function block(color) 
    return {
        tags = {color, "Block"},
        components = {
            Body = {width=32, height=56},
            Ai = {"ColoredBlockAI"},
            PhysicsBody = {},
            Sprite = {
                texture = "moveable_block",
                region = {0, 0, 32, 56},
                offset = {0, 0},
            },
        }
    } 
end

return {
    ["RedBlock"] = block "Red",
    ["GreenBlock"] = block "Green",
    ["BlueBlock"] = block "Blue",

    ["Laser"] = {
        tags = {},
        components = {
        
        }
    },
}
