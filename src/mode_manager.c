#include "mode_manager.h"
#include <stdlib.h>
#include <string.h>
#include "raylib.h"

ModeManager NewModeManager(void) {
 return (ModeManager) {0};
}

void InitModeManager(ModeManager *modeManager) {
    memset(modeManager->modes, 0, sizeof(modeManager->modes));
    modeManager->currentMode = NULL;
    modeManager->modeCount = 0;

    TraceLog(LOG_DEBUG, "STATE: Initialized the mode manager.");   
}

void InitMode(Mode* mode) {
    mode->id = -1;
    memset(mode->prefix, 0, MAX_PREFIX_LENGTH);
    memset(mode->name, 0, MAX_NAME_LENGTH);
    mode->callbacks = NULL;  // Initialize the hash to NULL
}

Callback* GetCallback(Mode* mode, const char* eventName) {
    Callback* callback = NULL;
    HASH_FIND_STR(mode->callbacks, eventName, callback);
    return callback;
}

int AddCallback(lua_State* L, Mode* mode, const char* eventName, int funcRef) {
    if (!mode || !eventName) {
        return 0;
    }

    TraceLog(LOG_DEBUG, "Adding callback for event: %s, with ref: %d", eventName, funcRef);

    Callback* callback = GetCallback(mode, eventName);
    
    // If callback doesn't exist for this event, create it
    if (!callback) {
        callback = (Callback*)malloc(sizeof(Callback));
        if (!callback) {
            return 0;
        }
        
        strncpy(callback->name, eventName, MAX_CALLBACK_NAME_LENGTH - 1);
        callback->name[MAX_CALLBACK_NAME_LENGTH - 1] = '\0';
        callback->refCount = 0;
        HASH_ADD_STR(mode->callbacks, name, callback);
    }
    
    // Check if we've reached the maximum number of callbacks for this event
    if (callback->refCount >= MAX_CALLBACKS_PER_EVENT) {
        return 0;
    }
    
    // Store the Lua function reference
    TraceLog(LOG_DEBUG, "Storing ref %d at index %d", funcRef, callback->refCount);
    callback->luaRefs[callback->refCount++] = funcRef;
    return 1;
}

void CleanupCallbacks(lua_State* L, Mode* mode) {
    if (!L || !mode) {
        TraceLog(LOG_WARNING, "CleanupCallbacks: Invalid L or mode pointer");
        return;
    }
    
    Callback *current, *tmp;
    
    HASH_ITER(hh, mode->callbacks, current, tmp) {
        if (!current) {
            TraceLog(LOG_WARNING, "CleanupCallbacks: Null callback in hash iteration");
            continue;
        }

        TraceLog(LOG_DEBUG, "Cleaning up callback: %s", current->name);
        TraceLog(LOG_DEBUG, "Number of callback references: %d", current->refCount);
        
        for (int i = 0; i < current->refCount; i++) {
            int ref = current->luaRefs[i];
            TraceLog(LOG_DEBUG, "Unreferencing Lua function ref: %d", ref);
            
            // Validate reference before unreferencing
            if (ref <= 0) {
                TraceLog(LOG_WARNING, "Invalid Lua reference: %d", ref);
                continue;
            }
            
            // Try to verify the reference is valid by checking if we can get it
            lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
            if (lua_isnil(L, -1)) {
                TraceLog(LOG_WARNING, "Reference %d points to nil", ref);
                lua_pop(L, 1);
                continue;
            }
            lua_pop(L, 1);
            
            luaL_unref(L, LUA_REGISTRYINDEX, ref);
        }
        
        HASH_DEL(mode->callbacks, current);
        free(current);
    }
    
    mode->callbacks = NULL;
}

void CleanupModeManager(lua_State* L, ModeManager* manager) {
    if (!L) {
        TraceLog(LOG_WARNING, "CleanupModeManager: Lua state is NULL");
        return;
    }
    if (!manager) {
        TraceLog(LOG_WARNING, "CleanupModeManager: Manager is NULL");
        return;
    }

    TraceLog(LOG_DEBUG, "Starting cleanup of ModeManager with %d modes", manager->modeCount);
    
    for (int i = 0; i < manager->modeCount; i++) {
        TraceLog(LOG_DEBUG, "Cleaning up mode %d: %s", i, manager->modes[i].name);
        CleanupCallbacks(L, &manager->modes[i]);
    }
    
    manager->modeCount = 0;
    manager->currentMode = NULL;
}

char* GetInputPrefix(char *input) {
    if (input == NULL || strlen(input) == 0) return "";

    int pos = strcspn(input, " \t");
    
    char *prefix = malloc(pos + 1);
    strncpy(prefix, input, pos);
    prefix[pos] = '\0';
    
    return prefix;
}

Mode* GetCurrentMode(char* input, ModeManager* modeManager) {
    if (modeManager->currentMode) {
        return modeManager->currentMode;
    } 

    // if no mode is set, look at the first word in input
    // if it matches any prefixes, we use that mode
    char* inputPrefix = GetInputPrefix(input);

    for (int i = 0; i < modeManager->modeCount; i++) {
        const Mode *mode = &modeManager->modes[i];
        
        if (strcmp(mode->prefix, inputPrefix) == 0) {
            return mode;
        }
    }

    return NULL;
}
