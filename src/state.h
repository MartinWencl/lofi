#include "theme.h"

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