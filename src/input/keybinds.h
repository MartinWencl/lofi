#ifndef INPUT_KEYBINDS_H
#define INPUT_KEYBINDS_H

#include "lua.h"
#include "stdint.h"
#include "stdbool.h"

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
    bool modifiers[MAX_ACTIVE_MODIFIERS];
} Keybind;

typedef struct
{
    Keybind keybinds[MAX_KEYBINDS];
    uint32_t count;
} KeybindStore;

bool CheckKeybind(Keybind keybind);
bool ExecuteKeybind(lua_State* L, Keybind keybind);
bool CheckAndExecuteKeybind(lua_State* L, Keybind keybind);

Keybind NewKeybind(void);
Keybind ParseKeysFromString(Keybind k, char** str, uint32_t numberOfChars);
Keybind SetCallback(Keybind k, lua_State *L);

KeybindStore NewKeybindStore(void);
uint32_t AddToKeybindStore(KeybindStore* store, Keybind keybind);
void ClearKeybindAt(KeybindStore* store, uint32_t index);

// @LUA-EXPORTED-FUNCTION
int lofi_keybind(lua_State *L); 

#endif // INPUT_KEYBINDS_H
