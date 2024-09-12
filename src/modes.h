typedef struct {
    const char* name;
    const char* description;
    lua_CFunction on_init;
    lua_CFunction on_input_change;
    lua_CFunction on_item_render;
    lua_CFunction on_select;
} Mode;