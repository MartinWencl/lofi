#ifndef MODES_H
#define MODES_H

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "events.h"

#define MAX_MODES 64
#define MAX_NAME_LENGTH 64
#define MAX_PREFIX_LENGTH 16

typedef struct {
    int id;
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

void InitModeManager(ModeManager *modeManager);

#endif // MODES_H