#include "lua/theme.h"
#include "ui/theme.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LOAD_THEME_COLOR(fieldName) \
    lua_getfield(L, -1, #fieldName); \
    if (lua_isstring(L, -1)) { \
        const char* hexStr = lua_tostring(L, -1); \
        char colorStr[9]; \
        if (strlen(hexStr) == 6) { \
            snprintf(colorStr, sizeof(colorStr), "%sff", hexStr); \
            theme.fieldName = GetColor((int)strtol(colorStr, NULL, 16)); \
        } else { \
            theme.fieldName = GetColor((int)strtol(hexStr, NULL, 16)); \
        } \
    } else { \
        theme.fieldName = DEFAULT_THEME.fieldName; \
    } \
    lua_pop(L, 1);

void LoadThemeFromLua(lua_State* L, UIState* state) {
    lua_getglobal(L, "lofi");
    if (!lua_istable(L, -1)) {
        state->loaded.theme = DEFAULT_THEME;
        return;
    }

    lua_getfield(L, -1, "opt");
    if (!lua_istable(L, -1)) {
        state->loaded.theme = DEFAULT_THEME;
        lua_pop(L, 1);
        return;
    }

    lua_getfield(L, -1, "theme");
    if (!lua_istable(L, -1)) {
        state->loaded.theme = DEFAULT_THEME;
        lua_pop(L, 2);
        return;
    }

    Theme theme;

    LOAD_THEME_COLOR(background);
    LOAD_THEME_COLOR(foreground);
    LOAD_THEME_COLOR(border);
    LOAD_THEME_COLOR(selected);
    LOAD_THEME_COLOR(text);
    LOAD_THEME_COLOR(accent);
    LOAD_THEME_COLOR(hover);
    LOAD_THEME_COLOR(error);
    LOAD_THEME_COLOR(success);

    state->loaded.theme = theme;

    lua_pop(L, 3);
}
