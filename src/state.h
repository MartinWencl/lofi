#ifndef APP_STATE_H
#define APP_STATE_H

#include "theme.h"

// Declare the global application state
extern AppState app_state;

typedef struct
{
    char *input;
    char *selected_item;
    char **items; // dynamically allocated array of strings
    int num_items;
} AppState;

typedef struct
{
        Theme *theme;
        int font_size;
        bool show_icons;
} UIState;

#endif // APP_STATE_H