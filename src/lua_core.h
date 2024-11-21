#ifndef LUA_CORE_H
#define LUA_CORE_H

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "events.h"
#include "core.h"

#define MAX_MODES 64
#define MAX_NAME_LENGTH 64
#define MAX_PREFIX_LENGTH 16
#define INPUT_TEXT_MAX_SIZE 256
#define MAX_LIST_ITEMS 1024

typedef struct {
    char prefix[MAX_PREFIX_LENGTH];
    char name[MAX_NAME_LENGTH];
    int callback_refs[NUM_EVENTS];
} Mode;

typedef struct {
    Mode* currentMode;
    Mode modes[MAX_MODES];
    int modeCount;
    lua_State* luaState;
} ModeManager;

int DispatchLuaEvent(AppState state, ModeManager modeManager, AppEventType event);
lua_State* InitLua(const char* initPath);
void InitModeManager(ModeManager *modeManager);

#endif // LUA_CORE_H