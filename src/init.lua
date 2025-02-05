-- local inputbox = lofi.widgets.inputbox
--
-- inputbox.on_select = function(data)
--
-- end
--
-- inputbox.on_text_update = function(data)
--
-- end
--
-- local listview = lofi.widgets.listview
--
-- listview.on_select = function(data)
--
-- end

local inputbox = {
    type = "inputbox",           -- Unique identifier (e.g., "inputbox", "listview")
    id = "optional_unique_id", -- Optional unique identifier
    config = {},           -- Widget-specific configuration
    state = {},            -- Mutable widget state
    events = {                 -- Standardized event handlers
        on_init = function(widget) end,
        -- on_update = function(widget) end,
        -- Widget-specific events can be added later
    }
}

lofi.log("From init.lua", lofi.log_level.DEBUG)

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

-- lofi.register_mode({
--     name = "example_mode",
--     prefix = "e",
--     -- layout = lofi.containers.verticallist {
--     --     inputbox,
--     --     listview,
--     -- }
--     layout = {
--         type = "vertical_list",
--         spacing = 0,
--         widgets = {
--             inputbox,
--             -- listview
--         }
--     }
-- })
