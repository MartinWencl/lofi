#include "keybinds.h"
#include "stdbool.h"
#include "stdint.h"
#include "stddef.h"
#include "lua.h"

#include "raylib.h"

bool CheckKeybind(Keybind keybind)
{
    if (!IsKeyDown(keybind.key)) {
        return false;
    } 
    
    for (size_t i = 0; i < keybind.modifierCount; i++)
    {
        if (keybind.modifiers[i]  || !IsKeyDown((Modifier)(i)))
        {
            return false;
        }
    }
    
    return true;
}

bool ExecuteKeybind(lua_State* L, Keybind keybind)
{
    lua_rawgeti(L, LUA_REGISTRYINDEX, keybind.callbackRef);
    
    if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
        // TODO: Handle error
        const char* errorMsg = lua_tostring(L, -1);
        lua_pop(L, 1);  // Remove error message from stack
        return false;
    }

    return true;
}

bool CheckAndExecuteKeybind(lua_State* L, Keybind keybind)
{
    if(CheckKeybind(keybind)) {
        ExecuteKeybind(L, keybind);
        return true;
    }
    return false;
}

Keybind NewKeybind(void)
{
    return (Keybind) {0};
}

Keybind ParseKeysFromString(Keybind k, char **str, uint32_t numberOfChars)
{
    return (Keybind) {0};
}

Keybind SetCallback(Keybind k, lua_State *L)
{
    return (Keybind) {0};
}

KeybindStore NewKeybindStore(void)
{
    return (KeybindStore) {0};
}

uint32_t AddToKeybindStore(KeybindStore *store, Keybind keybind)
{
    return 0;
}

void ClearKeybindAt(KeybindStore *store, uint32_t index)
{
    return;
}

int lofi_keybind(lua_State *L)
{
    return 0;
}
