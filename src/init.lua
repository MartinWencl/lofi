local inputbox = {
    type = "inputbox",           -- Unique identifier (e.g., "inputbox", "listview")
    id = "optional_unique_id", -- Optional unique identifier
    -- config = {},           -- Widget-specific configuration
    -- state = {},            -- Mutable widget state
    events = {                 -- Standardized event handlers
        on_init = function(config, state) end,
    }
}

lofi.opt.window = {
    refresh_rate = 60,              -- Integer value for refresh rate
    window_percent_width = 0.3,     -- Number between 0 and 1 for window width as screen percentage
    window_percent_height = 0.2,    -- Number between 0 and 1 for window height as screen percentage
    opacity = 0.2,                  -- Number between 0 and 1 for window opacity
    decoration = true               -- Boolean for window decoration
}

lofi.opt.font = {
    name = "JetBrains Mono Nerd Font",
    size = 40,
}

lofi.register_mode({
    name = "find (fd) files",
    prefix = "fd",
    callbacks = {
        on_search = function (state)
            local input = state.input_text
            local cmd = string.format("fd --max-depth=1 %s .", input)
            local handle = io.popen(cmd)
            if not handle then
                state.list = { "Error: find command failed" }
                return state
            end
            local result = handle:read("*a")
            print(result)
            handle:close()
            if not result or result == "" then
                state.list = { "No files found" }
            else
                state.list = {}
                for filename in string.gmatch(result, "[^\n]+") do
                    table.insert(state.list, filename)
                end
            end
            return state
        end,
        on_select = function (state)
            local handle = io.popen("xdg-open " .. state.list[state.focus + 1])
            if not handle then
                state.list = { "Error: failed to open file " }
            end
            return state
        end
    }
})

lofi.register_mode({
    name = "ripgrep files",
    prefix = "rg",
    callbacks = {
        on_search = function (state)
            local input = state.input_text
            local cmd = string.format("rg --files %s .", input)
            local handle = io.popen(cmd)
            if not handle then
                state.list = { "Error: ripgrep command failed" }
                return state
            end
            local result = handle:read("*a")
            print(result)
            handle:close()
            if not result or result == "" then
                state.list = { "No files found" }
            else
                state.list = {}
                for filename in string.gmatch(result, "[^\n]+") do
                    table.insert(state.list, filename)
                end
            end
            return state
        end,
        on_select = function (state)
            if #state.list > 0 then
                local handle = io.popen("xdg-open " .. state.list[state.focus + 1])
                if not handle then
                    state.list = { "Error: failed to open file " }
                end
                return state
            else
                state.list = { "No files found" }
                return state
            end
        end,
        on_view = function (state)
            if #state.list > 0 then
                local file = state.list[state.focus + 1]
                local handle = io.popen("rg --column -n -E '" .. input .. "' " .. file)
                if not handle then
                    state.list = { "Error: failed to open ripgrep" }
                end
                return state
            else
                state.list = { "No files found" }
                return state
            end
        end
    }
})

lofi.register_mode({
    name = "example_mode2",
    prefix = "e",
    callbacks = {
        on_search = function ()
            lofi.log("SEARCHING FROM LUA", lofi.log_level.DEBUG)
        end,
    }
})
