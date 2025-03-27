#include "keybinds.h"
#include "callbacks/events.h"
#include "lua.h"
#include "lua_core.h"
#include "state.h"
#include "lualib.h"
#include "lauxlib.h"
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <uchar.h>
#include <time.h> 
#include "mode_manager.h"
#include "raylib.h"

#define LOFI_KEYBIND_STORE "LOFI_KEYBIND_STORE"
#define DEFAULT_KEYBIND_COOLDOWN 0.2

KeybindStore* GetKeybindStore(lua_State* L) {
    lua_getfield(L, LUA_REGISTRYINDEX, LOFI_KEYBIND_STORE);
    KeybindStore* store = (KeybindStore*)lua_touserdata(L, -1);
    lua_pop(L, 1);
    return store;
}

void RegisterKeybindStore(KeybindStore* store, lua_State* L) {
    lua_pushlightuserdata(L, store);
    lua_setfield(L, LUA_REGISTRYINDEX, LOFI_KEYBIND_STORE);
    TraceLog(LOG_DEBUG, "LUA KEYBINDS: Pushed the keybind store into user data.");
}

bool CheckKeybind(Keybind keybind) {
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

bool ExecuteKeybind(State* state, lua_State* L, Keybind keybind) {
    lua_rawgeti(L, LUA_REGISTRYINDEX, keybind.callbackRef);

    Mode* mode = GetCurrentMode((char*)&state->input, &state->modes);
    if (mode == nullptr) {
        TraceLog(LOG_ERROR, "Can't execute keybind, no mode is set!");
        return false;
    }

    PushStateToLua(L, state, mode);

    if (lua_pcall(L, 1, 1, 0) != LUA_OK) {
        // TODO: Handle error
        const char* errorMsg = lua_tostring(L, -1);
        lua_pop(L, 1);  // Remove error message from stack
        TraceLog(LOG_ERROR, "LUA ERROR: %s", errorMsg);
        return false;
    }

    if (lua_istable(L, -1)) {
        UpdateStateFromLua(L, state, mode);
    }
    lua_pop(L, 1); // Remove return value

    return true;
}

Keybind NewKeybind(void) {
    Keybind k = {0};
    clock_gettime(CLOCK_MONOTONIC, &k.lastTriggeredTime);
    return k;
}

// HACK: this is a very crude implementation
Keybind ParseKeyFromString(Keybind k, char **str, uint32_t numberOfChars) {
    if (str == NULL || *str == NULL || numberOfChars == 0)
        return k;
    
    Keybind result = k;
    char *keyName = *str;
    
    // Handle modifier keys
    if (strncmp(keyName, "alt", numberOfChars) == 0) {
        result.modifiers[MOD_ALT] = true;
        if (result.modifierCount < MAX_ACTIVE_MODIFIERS)
            result.modifierCount++;
        return result;
    }
    else if (strncmp(keyName, "ctrl", numberOfChars) == 0) {
        result.modifiers[MOD_CTRL] = true;
        if (result.modifierCount < MAX_ACTIVE_MODIFIERS)
            result.modifierCount++;
        return result;
    }
    else if (strncmp(keyName, "super", numberOfChars) == 0) {
        result.modifiers[MOD_SUPER] = true;
        if (result.modifierCount < MAX_ACTIVE_MODIFIERS)
            result.modifierCount++;
        return result;
    }
    
    // Handle single character keys
    if (numberOfChars == 1) {
        char c = keyName[0];
        
        // A-Z keys
        if (c >= 'a' && c <= 'z')
            result.key = KEY_A + (c - 'a');
        else if (c >= 'A' && c <= 'Z')
            result.key = KEY_A + (c - 'A');
        // 0-9 keys
        else if (c >= '0' && c <= '9')
            result.key = KEY_ZERO + (c - '0');
    }
    // Handle longer key names
    else if (strncmp(keyName, "space", numberOfChars) == 0)
        result.key = KEY_SPACE;
    else if (strncmp(keyName, "escape", numberOfChars) == 0)
        result.key = KEY_ESCAPE;
    else if (strncmp(keyName, "enter", numberOfChars) == 0)
        result.key = KEY_ENTER;
    else if (strncmp(keyName, "tab", numberOfChars) == 0)
        result.key = KEY_TAB;
    else if (strncmp(keyName, "backspace", numberOfChars) == 0)
        result.key = KEY_BACKSPACE;
    else if (strncmp(keyName, "insert", numberOfChars) == 0)
        result.key = KEY_INSERT;
    else if (strncmp(keyName, "delete", numberOfChars) == 0)
        result.key = KEY_DELETE;
    else if (strncmp(keyName, "right", numberOfChars) == 0)
        result.key = KEY_RIGHT;
    else if (strncmp(keyName, "left", numberOfChars) == 0)
        result.key = KEY_LEFT;
    else if (strncmp(keyName, "down", numberOfChars) == 0)
        result.key = KEY_DOWN;
    else if (strncmp(keyName, "up", numberOfChars) == 0)
        result.key = KEY_UP;
    else if (strncmp(keyName, "f1", numberOfChars) == 0)
        result.key = KEY_F1;
    else if (strncmp(keyName, "f2", numberOfChars) == 0)
        result.key = KEY_F2;
    else if (strncmp(keyName, "f3", numberOfChars) == 0)
        result.key = KEY_F3;
    else if (strncmp(keyName, "f4", numberOfChars) == 0)
        result.key = KEY_F4;
    else if (strncmp(keyName, "f5", numberOfChars) == 0)
        result.key = KEY_F5;
    else if (strncmp(keyName, "f6", numberOfChars) == 0)
        result.key = KEY_F6;
    else if (strncmp(keyName, "f7", numberOfChars) == 0)
        result.key = KEY_F7;
    else if (strncmp(keyName, "f8", numberOfChars) == 0)
        result.key = KEY_F8;
    else if (strncmp(keyName, "f9", numberOfChars) == 0)
        result.key = KEY_F9;
    else if (strncmp(keyName, "f10", numberOfChars) == 0)
        result.key = KEY_F10;
    else if (strncmp(keyName, "f11", numberOfChars) == 0)
        result.key = KEY_F11;
    else if (strncmp(keyName, "f12", numberOfChars) == 0)
        result.key = KEY_F12;
    
    return result;
}

KeybindStore NewKeybindStore(void) {
    return (KeybindStore) {0};
}

uint32_t AddToKeybindStore(KeybindStore *store, Keybind keybind) {
    if (store->count > MAX_KEYBINDS) {
        return -1;
    }
    store->keybinds[store->count++] = keybind;
    return store->count;
}
 
int lofi_keybind(lua_State *L) {
    KeybindStore* store = GetKeybindStore(L);
    if (!store) {
        luaL_error(L, "Keybind store not initialized");
        return 0;
    }
    
    if (!lua_istable(L, 1) || !lua_isfunction(L, 2)) {
        luaL_error(L, "Usage: keybind({keys}, callback)");
        return 0;
    }
    
    Keybind k = NewKeybind();
    
    size_t tableLen = lua_objlen(L, 1);
    if (tableLen > 0) {
        for (size_t i = 1; i <= tableLen; i++) {
            lua_rawgeti(L, 1, i);
            if (lua_isstring(L, -1)) {
                const char* keyString = lua_tostring(L, -1);
                uint32_t stringLen = strlen(keyString);
                char* strPtr = (char*)keyString;

                k = ParseKeyFromString(k, &strPtr, stringLen);
                TraceLog(LOG_DEBUG, "LUA: Parsed key - %s", keyString);
            }
            lua_pop(L, 1);
        }
    }
    
    lua_pushvalue(L, 2);
    k.callbackRef = luaL_ref(L, LUA_REGISTRYINDEX);
    
    int32_t index = AddToKeybindStore(store, k);
    if (index == -1) {
        luaL_error(L, "Failed to add keybind: store full");
        return 0;
    }
    TraceLog(LOG_DEBUG, "LUA: Loaded keybind - %d", k.key);
    
    lua_pushinteger(L, index);
    return 1;
}

// HACK: this is also pretty crude
double diff_timespec(const struct timespec *time1, const struct timespec *time0) {
  return (time1->tv_sec - time0->tv_sec)
      + (time1->tv_nsec - time0->tv_nsec) / 1000000000.0;
}

void ProcessKeybinds(State* state, KeybindStore* store, lua_State* L) {
    for (size_t i = 0; i < store->count; i++) {
        Keybind keybind = store->keybinds[i];
        if (CheckKeybind(keybind)) {
            struct timespec now;
            clock_gettime(CLOCK_MONOTONIC, &now);

            if (diff_timespec(&now, &keybind.lastTriggeredTime) > DEFAULT_KEYBIND_COOLDOWN) {
                clock_gettime(CLOCK_MONOTONIC, &store->keybinds[i].lastTriggeredTime);
                
                TraceLog(LOG_DEBUG, "Calling keybind %d", (char)keybind.key);
                ExecuteKeybind(state, L, keybind);
            }
        }
    }
}
