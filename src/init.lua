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
