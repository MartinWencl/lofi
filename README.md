# Lofi: Lua-Powered App Launcher

> NOTE: Lofi is in the early stages of development and WILL be subject to changes. Use at your own discretion.

## ✨ What is Lofi?

Lofi is an **extensible** application launcher, configured and extended with lua!

## 🔧 Compilation
```bash
# Clone the repository
git clone https://github.com/yourusername/lofi
cd lofi

# Configure the build
meson setup build

# Compile the project
meson compile -C build
```

## 🌟 Configuration Example

```lua
-- Customize window appearance
lofi.opt.window = {
    refresh_rate = 60,
    window_percent_width = 0.3,
    window_percent_height = 0.2,
    opacity = 0.2
}

-- Define custom find mode
lofi.register_mode({
    name = "find",
    prefix = "fd",
    callbacks = {
        on_search = function(state)
            local input = state.input_text
            local cmd = string.format("fd --max-depth=1 %s .", input)
            local handle = io.popen(cmd)
            if not handle then
                state.list = { "Error: find command failed" }
                return state
            end
            local result = handle:read("*a")
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
        on_select = function(state)
            local selected_file = state.list[state.focus + 1]
            os.execute("xdg-open " .. selected_file)
            return state
        end
    }
})
```

## 🐛 Troubleshooting

### Common Issues
- Ensure all dependencies are installed
- Check that you're using a compiler with C23 support
- Verify LuaJIT and Raylib are correctly linked
