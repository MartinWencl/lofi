#include "lua_core.h"
#include "events.h"
#include "font.h"
#include "mode_manager.h"
#include "utils.h"

#include "lua.h"
#include "lualib.h"

#include "lua/macros.h"
#include "lua/window.h"
#include "lua/theme.h"
#include "lua/font.h"
#include "lua/modes.h"

#include <stdlib.h>
#include <string.h>

static void PushStateToLua(lua_State* L, State* state, Mode* mode) {
    bool removeModePrefix = mode->isTemporary;

    lua_newtable(L);

    // Handle input text with optional mode prefix removal
    const char* input_to_push = state->input;
    if (removeModePrefix && state->input[0] != '\0') {
        // Find first space
        const char* space = strchr(state->input, ' ');
        if (space != NULL) {
            // Skip the space character itself
            input_to_push = space + 1;
        }
    }
    
    lua_pushstring(L, input_to_push);
    lua_setfield(L, -2, "input_text");
    
    // Push list items
    lua_newtable(L);
    for (int i = 0; i < state->listCount; i++) {
        lua_pushstring(L, state->list[i]);
        lua_rawseti(L, -2, i + 1);
    }
    lua_setfield(L, -2, "list");
    
    lua_pushinteger(L, state->focus.index);
    lua_setfield(L, -2, "focus");
    TraceLog(LOG_DEBUG, "STATE: Pushed the state to lua.");
}

static void UpdateStateFromLua(lua_State* L, State* state, Mode* mode) {
    bool addModePrefix = mode->isTemporary;
    // Ensure we're working with a table
    luaL_checktype(L, -1, LUA_TTABLE);
    
    // Update input text
    lua_getfield(L, -1, "input_text");
    if (!lua_isnil(L, -1)) {
        const char* input = lua_tostring(L, -1);
        if (addModePrefix && mode->prefix[0] != '\0') {
            // Create temporary buffer for the combined string
            char combined[INPUT_TEXT_MAX_SIZE];
            snprintf(combined, sizeof(combined), "%s %s", mode->prefix, input);
            strncpy(state->input, combined, INPUT_TEXT_MAX_SIZE - 1);
        } else {
            strncpy(state->input, input, INPUT_TEXT_MAX_SIZE - 1);
        }
        state->input[INPUT_TEXT_MAX_SIZE - 1] = '\0'; // Ensure null termination
    }
    lua_pop(L, 1);
    
    ClearListViewExList(&(state->list), &(state->listCount));

    // Update list items
    lua_getfield(L, -1, "list");
    if (!lua_isnil(L, -1)) {
        int listCount = lua_rawlen(L, -1);
        
        for (int i = 1; i <= listCount; i++) {
            lua_rawgeti(L, -1, i);
            const char* item = lua_tostring(L, -1);
            AddToListViewExList(&(state->list), &(state->listCount), item);
            lua_pop(L, 1);
        }
    }
    lua_pop(L, 1);
    
    // Update focus
    lua_getfield(L, -1, "focus");
    if (!lua_isnil(L, -1)) {
        state->focus.index = lua_tointeger(L, -1);
    }
    lua_pop(L, 1);
    TraceLog(LOG_DEBUG, "STATE: Updated the state from lua.");
}

int DispatchLuaEvent(lua_State* L, State* state, Mode* mode, EventType EventType) {
    if (!state || !mode) {
        TraceLog(LOG_ERROR, "LUA EVENT: Invalid state or no current mode!");
        return 0;
    }

    Mode* currentMode = mode;
    TraceLog(LOG_DEBUG, "LUA EVENT: Dispatching %s event for mode '%s'", 
             GetEventAsStr(EventType), currentMode->name);

    const char* callbackName = GetEventAsStr(EventType);

    Callback* callback = GetCallback(currentMode, callbackName);
    if (!callback || callback->refCount == 0) {
        TraceLog(LOG_DEBUG, "LUA EVENT: No callbacks registered for %s", callbackName);
        return 1; // Not an error, just no callbacks
    }

    if (!L) {
        TraceLog(LOG_ERROR, "LUA EVENT: No Lua state available!");
        return 0;
    }

    int success = 1;
    // Execute all callbacks registered for this event
    for (int i = 0; i < callback->refCount; i++) {
        int ref = callback->luaRefs[i];
        
        // Get the callback function from registry
        lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
        if (!lua_isfunction(L, -1)) {
            TraceLog(LOG_ERROR, "LUA EVENT: Invalid callback reference for %s", callbackName);
            lua_pop(L, 1);
            success = 0;
            continue;
        }

        // Push current app state as second argument
        PushStateToLua(L, state, mode);

        // Call the function with event table and state table as arguments
        if (lua_pcall(L, 1, 1, 0) != LUA_OK) {
            const char* errorMessage = lua_tostring(L, -1);
            TraceLog(LOG_ERROR, "LUA EVENT: Error in callback execution: %s", errorMessage);
            lua_pop(L, 1);
            success = 0;
            continue;
        }

        // Handle return value if it's a table (updated state)
        if (lua_istable(L, -1)) {
            UpdateStateFromLua(L, state, mode);
        }
        lua_pop(L, 1); // Remove return value
    }

    return success;
}

int lofi_Log(lua_State *L) {
    if (lua_gettop(L) != 2) {
        lua_pushstring(L, "Requires two arguments: log_level and log_message");
        lua_error(L);
        return 0; // This line is not actually reached because lua_error does a longjmp.
    }

    const char *logMessage = luaL_checkstring(L, 1);
    int logLevel = luaL_checkinteger(L, 2); 
    
    TraceLog(logLevel, "LUA LOG: %s", logMessage);
    
    return 0;
}

void LoadOptionsFromLua(State* state, lua_State* L) {
    LoadWindowConfigFromLua(L, &state->ui.config.window);
    LoadFontConfigFromLua(L, &state->ui);
    LoadThemeFromLua(L, &state->ui);
}

int InitLuaModule(lua_State* L) {
    luaopen_table(L);
        LUA_SET_FUNCTION(lofi_Log, "log");
        LUA_SET_FUNCTION(lofi_RegisterMode, "register_mode");

        LUA_SET_TABLE("log_level",
            LUA_SET_INT(LOG_INFO, "INFO");
            LUA_SET_INT(LOG_DEBUG, "DEBUG");
            LUA_SET_INT(LOG_WARNING, "WARNING");
            LUA_SET_INT(LOG_ERROR, "ERROR");
        );

    lua_setglobal(L, "lofi");

    TraceLog(LOG_DEBUG, "LUA: Initialized the to-be exported lua module.");
    return 1;
}

void RegisterModeManager(ModeManager* modeManager, lua_State* L)
{
    lua_pushlightuserdata(L, modeManager);
    lua_setfield(L, LUA_REGISTRYINDEX, MODE_MANAGER_KEY);

    TraceLog(LOG_DEBUG, "LUA MODES: Pushed the mode manager into user data.");
}

void InitLua(const char* initPath, ModeManager* modeManager, lua_State* L) {
    luaL_openlibs(L);
    InitLuaModule(L);
    RegisterModeManager(modeManager, L);

    // this is more of a hack, but it makes sure the
    // values exist before the file is run
    // The tables are otherwise not present in lua -- 
    // either this or create a separate function to generate the 
    // empty table structure (lofi.opt ...)
    ExportWindowConfigToLua(L, DEFAULT_WINDOW_CONFIG);
    ExportFontConfigToLua(L, (FontConfig) {"default", 20});

    if (luaL_dofile(L, initPath) != LUA_OK) {
        TraceLog(LOG_ERROR, "LUA: Error loading init.lua: %s\n", lua_tostring(L, -1));
        lua_close(L);
    }

    TraceLog(LOG_DEBUG, "Lua initialized, init.lua OK.");
}
