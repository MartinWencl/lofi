#ifndef INPUT_KEYBINDS_H
#define INPUT_KEYBINDS_H

// NOTE: Keybinds are currently only posix compatible
// as they use clock_gettime()
#define _POSIX_C_SOURCE 199309L

#include "state_declaration.h"
#include "events.h"
#include "lua.h"
#include "stdint.h"
#include "stdbool.h"
#include <math.h>
#include <time.h>

#define MAX_ACTIVE_MODIFIERS 3
#define MAX_KEYBINDS 256

typedef enum {
    MOD_ALT,
    MOD_CTRL,
    MOD_SUPER,
} Modifier;

typedef struct {
    uint32_t key;
    uint32_t callbackRef;
    uint32_t modifierCount;
    struct timespec lastTriggeredTime;
    bool modifiers[MAX_ACTIVE_MODIFIERS];
} Keybind;

typedef struct {
    Keybind keybinds[MAX_KEYBINDS];
    uint32_t count;
} KeybindStore;

bool CheckKeybind(Keybind keybind);
bool ExecuteKeybind(State* state, lua_State* L, Keybind keybind);

Keybind NewKeybind(void);
Keybind ParseKeyFromString(Keybind k, char** str, uint32_t numberOfChars);

KeybindStore NewKeybindStore(void);
void RegisterKeybindStore(KeybindStore* store, lua_State* L);
uint32_t AddToKeybindStore(KeybindStore* store, Keybind keybind);
void ClearKeybindAt(KeybindStore* store, uint32_t index);

void ProcessKeybinds(State* state, KeybindStore* store, EventQueue* queue, lua_State* L);

// @LUA-EXPORTED-FUNCTION
int lofi_keybind(lua_State *L); 

#endif // INPUT_KEYBINDS_H
