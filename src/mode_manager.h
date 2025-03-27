#ifndef MODES_H
#define MODES_H

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include <stdbool.h>
#include "uthash.h"

#define MAX_MODES 64
#define MAX_NAME_LENGTH 64
#define MAX_PREFIX_LENGTH 16
#define MAX_CALLBACK_NAME_LENGTH 32
#define MAX_CALLBACKS_PER_EVENT 8
#define MODE_MANAGER_KEY "MODE_MANAGER"

typedef struct Callback {
    char name[MAX_CALLBACK_NAME_LENGTH];
    int refCount;
    int luaRefs[MAX_CALLBACKS_PER_EVENT];
    UT_hash_handle hh;
} Callback;

typedef struct {
    int id;
    char prefix[MAX_PREFIX_LENGTH];
    char name[MAX_NAME_LENGTH];
    Callback* callbacks;
    bool isTemporary;
} Mode;

typedef struct {
    Mode* currentMode;
    Mode modes[MAX_MODES];
    int modeCount;
} ModeManager;

ModeManager NewModeManager(void);
void FreeModeManager(lua_State* L, ModeManager* modeManager);
ModeManager* GetModeManager(lua_State* L);

Mode* GetCurrentMode(char* input, ModeManager* modeManager);
Mode* GetModeFromName(char* name, ModeManager* modeManager);
void InitMode(Mode* mode);

int AddCallback(lua_State* L, Mode* mode, const char* eventName, int funcRef);
void FreeCallbacks(lua_State* L, Mode* mode);
Callback* GetCallback(Mode* mode, const char* eventName);

#endif // MODES_H
