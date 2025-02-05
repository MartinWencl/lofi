#ifndef UI_THEME_H
#define UI_THEME_H

#include "raylib.h"

typedef struct {
    Color background;
    Color foreground;
    Color border;
    Color selected;
    Color text;
    Color accent;
    Color hover;
    Color error;
    Color success;
} Theme;

extern const Theme DEFAULT_THEME;

#endif // UI_THEME_H
