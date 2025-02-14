#ifndef INPUT_KEYBINDS_H
#define INPUT_KEYBINDS_H

#define MAX_ACTIVE_MODIFIERS 3

typedef enum {
    MOD_ALT,
    MOD_CTRL,
    MOD_SUPER,
} Modifier;

typedef struct {
    int key;
    Modifier modifiers[MAX_ACTIVE_MODIFIERS];
    int modifierCount;
    int callbackRef;
} Keybind;

#endif // INPUT_KEYBINDS_H
