--[[
-- This script converts the outputted aseprite json animation files
-- to Lua tables that the game engine can load.
--
-- It uses hardcoded paths, so that should be refactored at some point
--]]

local json = require "src/assets/scripts/json"
local serpent = require "src/assets/scripts/serpent"

require "lfs"

function readAll(file)
    local f = assert(io.open(file, "rb"))
    local content = f:read("*all")
    f:close()
    return content
end

function string:split(delimiter)
  local result = { }
  local from  = 1
  local delim_from, delim_to = string.find( self, delimiter, from  )
  while delim_from do
    table.insert( result, string.sub( self, from , delim_from-1 ) )
    from  = delim_to + 1
    delim_from, delim_to = string.find( self, delimiter, from  )
  end
  table.insert( result, string.sub( self, from  ) )
  return result
end

function isN(a)
    return tonumber(a) ~= nil
end

for file in lfs.dir "src/assets/images/" do
    local dest = string.find(file, ".", 1, true)
    local ext = string.sub(file, dest+1, #file)
    local name = string.sub(file, 1, dest-1)
    
    if ext == "json" then
        local jdata = readAll("src/assets/images/"..file)
        local ltable = json.decode(jdata)

        local final = {[name] = {
            playback = "forward",
            repeated = true,
            frames = {},
        }}

        for key, i in pairs(ltable.frames) do

            --bird 3.aseprite
            local period = string.find(key, ".", 1, true)
            local index = period - 1
            while index >= 1 and isN(key:sub(index, index)) do
                index = index - 1
            end

            local num = tonumber(string.sub(key, index+1, period-1)) + 1

            final[name].frames[num] = {
                {i.frame.x, i.frame.y, i.frame.w, i.frame.h},
                duration = i.duration
            }
        end

        local outfile = io.open("src/assets/data/animations/" .. name .. ".lua", "w")
        outfile:write("return " .. serpent.block(final, {comment=false, compact=true}))
        outfile:close()
    end
end
