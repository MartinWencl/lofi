lofi.register_mode({
    name = "example_mode",
    on_select = function(state)
        lofi.log("on_select: Selecting index" .. state.select.index, lofi.log_level.DEBUG) 
        return state
    end,
    on_search = function(state)
        lofi.log("on_search: Preforming mock search.", lofi.log_level.DEBUG) 

        state.input_text = "Input Text"
        
        -- Initialize or update listItems and listCount
        state.listItems = state.listItems or {}
        for i = 1, 10 do
            state.listItems[i] = "result" .. i
        end
        state.list_count = 10 -- Update list count to match the number of items
        
        return state
    end
})