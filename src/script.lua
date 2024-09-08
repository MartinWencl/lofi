local lofi = require("lofi")
-- Define a Lua function
local function my_lua_function()
    print("Hello from the stored Lua function!")
end

lofi.set_function(my_lua_function)
lofi.add_mode(function()
        print("First!")
    end,
    function()
        print("Update!")
    end)
