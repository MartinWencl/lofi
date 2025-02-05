#ifndef UI_STATE_H
#define UI_STATE_H

#include "raylib.h"
#include "ui/window.h"
#include "ui/font.h"
#include "ui/theme.h"

typedef struct UIState {
    struct {
        FontConfig font;
        WindowConfig window;
    } config;
    struct {
        Font font;
        Dimensions window;
        Theme theme;
    } loaded;
} UIState;

#endif // UI_STATE_H