#include "lua_core.h"
#include <string.h>
#include <stdlib.h>

// hate this
static ModeManager *g_modeManager;

void InitModeManager(ModeManager *modeManager) {
    memset(modeManager->modes, 0, sizeof(modeManager->modes));
    modeManager->currentMode = NULL;
    modeManager->modeCount = 0;
    modeManager->luaState = NULL; 

    g_modeManager = modeManager;  
}

// Helper function to push AppState to Lua
static void push_app_state_to_lua(lua_State* L, AppState* state) {
    lua_newtable(L);
    
    // Push input text
    lua_pushstring(L, state->input_text);
    lua_setfield(L, -2, "input_text");
    
    // Push list items
    lua_newtable(L);
    for (int i = 0; i < state->list_count; i++) {
        lua_pushstring(L, state->list_items[i]);
        lua_rawseti(L, -2, i + 1);
    }
    lua_setfield(L, -2, "list_items");
    
    // Push other state fields
    lua_pushinteger(L, state->list_count);
    lua_setfield(L, -2, "list_count");
    
    lua_pushinteger(L, state->list_scroll_index);
    lua_setfield(L, -2, "list_scroll_index");
    
    lua_pushinteger(L, state->focus.index);
    lua_setfield(L, -2, "focus");
}

static void update_app_state_from_lua(lua_State* L, AppState* state) {
    // Ensure we're working with a table
    luaL_checktype(L, -1, LUA_TTABLE);
    
    // Update input text
    lua_getfield(L, -1, "input_text");
    if (!lua_isnil(L, -1)) {
        const char* input = lua_tostring(L, -1);
        strncpy(state->input_text, input, INPUT_TEXT_MAX_SIZE - 1);
    }
    lua_pop(L, 1);
    
    // // Update list items
    // lua_getfield(L, -1, "list_items");
    // if (!lua_isnil(L, -1)) {
        // // Free existing list items
        // if (state->list_items) {
            // for (int i = 0; i < state->list_count; i++) {
                // free(state->list_items[i]);
            // }
            // free(state->list_items);
        // }
        
        // // Count and allocate new list items
        // state->list_count = lua_rawlen(L, -1);
        // state->list_items = malloc(state->list_count * sizeof(char*));
        
        // for (int i = 1; i <= state->list_count; i++) {
            // lua_rawgeti(L, -1, i);
            // const char* item = lua_tostring(L, -1);
            // state->list_items[i-1] = strdup(item);
            // lua_pop(L, 1);
        // }
    // }
    // lua_pop(L, 1);
    
    // Update list scroll index
    lua_getfield(L, -1, "list_scroll_index");
    if (!lua_isnil(L, -1)) {
        state->list_scroll_index = lua_tointeger(L, -1);
    }
    lua_pop(L, 1);
    
    // Update focus
    lua_getfield(L, -1, "focus");
    if (!lua_isnil(L, -1)) {
        state->focus.index = lua_tointeger(L, -1);
    }
    lua_pop(L, 1);
}

int DispatchLuaEvent(AppState state, ModeManager modeManager, AppEventType event) {
    int ref = modeManager.currentMode->callback_refs[event];

    if (ref == LUA_NOREF || !modeManager.luaState) return 0;

    lua_State* L = modeManager.luaState;
    
    lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
    
    // Push current app state
    push_app_state_to_lua(L, &state);
    
    if (lua_pcall(L, 1, 1, 0) != LUA_OK) {
        // Error handling
        return 0;
    }
    
    // Update app state from returned table
    update_app_state_from_lua(L, &state);
    
    // Remove the returned state table
    lua_pop(L, 1);
    
    return 1;
}

static int lofi_register_mode(lua_State* L) {
    luaL_checktype(L, 1, LUA_TTABLE);

    // Check for name
    lua_getfield(L, 1, "name");
    if (lua_isnil(L, -1)) {
        return luaL_error(L, "Mode must have a name");
    }
    const char* mode_name = lua_tostring(L, -1);
    lua_pop(L, 1);

    // Check if we have space
    if (g_modeManager->modeCount >= MAX_MODES) {
        return luaL_error(L, "Maximum number of modes reached");
    }

    Mode* mode = &g_modeManager->modes[g_modeManager->modeCount++];
    strncpy(mode->name, mode_name, MAX_NAME_LENGTH - 1);

    // Store references for all possible events
    // #define STORE_CALLBACK(event_name) \
        // lua_getfield(L, 1, "on_" #event_name); \
        // mode->ref_##event_name = lua_isfunction(L, -1) ? \
            // luaL_ref(L, LUA_REGISTRYINDEX) : LUA_NOREF;
            
    lua_getfield(L, 1, "on_select");
    mode->callback_refs[EVENT_ITEM_SELECTED] = lua_isfunction(L, -1) ? luaL_ref(L, LUA_REGISTRYINDEX) : LUA_NOREF;
    lua_getfield(L, 1, "on_search");
    mode->callback_refs[EVENT_SEARCH_TRIGGERED] = lua_isfunction(L, -1) ? luaL_ref(L, LUA_REGISTRYINDEX) : LUA_NOREF;

    return 0;
}

int luaopen_lofi(lua_State* L) {
    lua_newtable(L);

    // Add event constants
    lua_pushinteger(L, EVENT_NONE);
    lua_setfield(L, -2, "EVENT_NONE");
    lua_pushinteger(L, EVENT_SEARCH_TRIGGERED);
    lua_setfield(L, -2, "EVENT_SEARCH_TRIGGERED");
    lua_pushinteger(L, EVENT_FOCUS_CHANGED);
    lua_setfield(L, -2, "EVENT_FOCUS_CHANGED");
    lua_pushinteger(L, EVENT_ITEM_SELECTED);
    lua_setfield(L, -2, "EVENT_ITEM_SELECTED");
    lua_pushinteger(L, EVENT_SCROLL);
    lua_setfield(L, -2, "EVENT_SCROLL");
    lua_pushinteger(L, EVENT_EXIT);
    lua_setfield(L, -2, "EVENT_EXIT");

    // Add mode management functions
    lua_pushcfunction(L, lofi_register_mode);
    lua_setfield(L, -2, "register_mode");
    // lua_pushcfunction(L, lofi_switch_mode);
    // lua_setfield(L, -2, "switch_mode");

    return 1;
}

lua_State* InitLua(const char* init_path) {
    // Create Lua state
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    // Open our custom lofi module
    luaopen_lofi(L);
    lua_setglobal(L, "lofi");

    // Load and run the init.lua file
    if (luaL_dofile(L, init_path) != LUA_OK) {
        fprintf(stderr, "Error loading init.lua: %s\n", lua_tostring(L, -1));
        lua_close(L);
        return NULL;
    }

    return L;
}