package main

import (
	"fmt"
	"log"

	"fyne.io/fyne/v2"
	"github.com/aarzilli/golua/lua"
)

type LuaMode struct {
	Name         string
	Description  string
	OnActivate   int
	OnDeactivate int
	OnSubmitted  int
	OnSelected   int
}

func callLuaFunc(L *lua.State, ref int, nargs int, nresults int) error {
	log.Printf("Calling Lua function with ref: %d, nargs: %d, nresults: %d\n", ref, nargs, nresults)

	L.RawGeti(lua.LUA_REGISTRYINDEX, ref)
	if !L.IsFunction(-1) {
		L.Pop(1)
		return fmt.Errorf("Invalid Lua function reference: %d", ref)
	}

	if err := L.Call(nargs, nresults); err != nil {
		return fmt.Errorf("Error calling Lua function: %s", err)
	}

	log.Println("Lua function called successfully")
	return nil
}

func getLuaStringField(L *lua.State, fieldName string) string {
	L.GetField(-1, fieldName)
	defer L.Pop(1)
	if L.IsString(-1) {
		return L.ToString(-1)
	}
	log.Printf("Failed to retrieve field '%s'", fieldName)
	return ""
}

func getLuaFunctionRef(L *lua.State, fieldName string) int {
	L.GetField(-1, fieldName)
	if L.IsFunction(-1) {
		return L.Ref(lua.LUA_REGISTRYINDEX)
	}
	L.Pop(1)
	return lua.LUA_REFNIL
}

func parseLuaMode(L *lua.State) *LuaMode {
	mode := &LuaMode{
		Name:         getLuaStringField(L, "name"),
		Description:  getLuaStringField(L, "description"),
		OnActivate:   getLuaFunctionRef(L, "on_activate"),
		OnDeactivate: getLuaFunctionRef(L, "on_deactivate"),
		OnSubmitted:  getLuaFunctionRef(L, "on_submitted"),
		OnSelected:   getLuaFunctionRef(L, "on_selected"),
	}

	if ok := L.CheckStack(0); !ok {
		log.Printf("Lua stack error!")
	}

	return mode
}

func luaRegisterLuaMode(L *lua.State) int {
	if !L.IsTable(1) {
		L.ArgError(1, "expecting table as argument")
	}

	L.PushValue(1)
	mode := parseLuaMode(L)
	L.Pop(1)

	if mode != nil {
		modes[mode.Name] = convertLuaModeToMode(L, mode)
		log.Printf("Registered LuaMode: %+v\n", mode)
	}

	return 0 // number of return values
}

func luaSetMode(L *lua.State) int {
	if !L.IsString(1) {
		L.ArgError(1, "expecting string as first argument")
	}

	modeName := L.ToString(1)
	L.Pop(1)
	var w = fyne.CurrentApp().Driver().AllWindows()[0]
	SetCurrentMode(modeName, w)

	return 0 // number of return values
}

func convertLuaModeToMode(L *lua.State, luaMode *LuaMode) *Mode {
	onActivate := func(w fyne.Window) {
		if luaMode.OnActivate != lua.LUA_REFNIL {
			L.RawGeti(lua.LUA_REGISTRYINDEX, luaMode.OnActivate)
			if err := L.Call(0, 0); err != nil {
				log.Printf("Error calling on_activate: %s\n", err)
			}
		}
	}

	onDeactivate := func(w fyne.Window) {
		if luaMode.OnDeactivate != lua.LUA_REFNIL {
			L.RawGeti(lua.LUA_REGISTRYINDEX, luaMode.OnDeactivate)
			if err := L.Call(0, 0); err != nil {
				log.Printf("Error calling on_deactivate: %s\n", err)
			}
		}
	}

	onSubmitted := func(text string) {
		if luaMode.OnSubmitted != lua.LUA_REFNIL {
			log.Printf("onSubmit function ref: %d\n", luaMode.OnSubmitted)
			L.RawGeti(lua.LUA_REGISTRYINDEX, luaMode.OnSubmitted)
			if !L.IsFunction(-1) {
				log.Printf("Invalid Lua function reference for onSubmitted: %d\n", luaMode.OnSubmitted)
				L.Pop(1)
				return
			}
			L.PushString(text)
			if err := L.Call(1, 0); err != nil {
				log.Printf("Error calling on_submitted: %s\n", err)
			}
			L.Pop(1) // Clean up the stack
		}
	}

	onSelected := func(item Capture) {
		if luaMode.OnSelected != lua.LUA_REFNIL {
			log.Printf("onSelect function ref: %d\n", luaMode.OnSelected)
			L.RawGeti(lua.LUA_REGISTRYINDEX, luaMode.OnSelected)
			if !L.IsFunction(-1) {
				log.Printf("Invalid Lua function reference for onSelected: %d\n", luaMode.OnSelected)
				L.Pop(1)
				return
			}
			// L.PushString(item)
			if err := L.Call(1, 0); err != nil {
				log.Printf("Error calling on_selected: %s\n", err)
			}
			L.Pop(1) // Clean up the stack
		}
	}

	return &Mode{
		Name:         luaMode.Name,
		Description:  luaMode.Description,
		OnActivate:   onActivate,
		OnDeactivate: onDeactivate,
		OnSubmitted:  onSubmitted,
		OnSelected:   onSelected,
	}
}

func test() {
	L := lua.NewState()
	defer L.Close()
	L.OpenLibs()

	// Register the functions to Lua
	L.Register("registerLuaMode", luaRegisterLuaMode)
	L.Register("setMode", luaSetMode)

	// Example Lua script that registers a new Lua mode and sets it
	luaScript := `
        local mode = {
            name = "exampleLuaMode",
            description = "This is an example Lua mode.",
			on_submitted = function(s) print("Submitted:", s) end,
            on_activate = function() print("Activated from lua") end,
            on_deactivate = function() print("Deactivated") end,
            
            on_selected = function() print("Selected") end
        }
        registerLuaMode(mode)
        setMode("exampleLuaMode")
		-- setMode("filePicker")
    `

	// Run the Lua script
	if err := L.DoString(luaScript); err != nil {
		log.Printf("Error running Lua script: %s\n", err.Error())
	}

	captures.Append(Capture{id: 1, showName: "test"})
}
