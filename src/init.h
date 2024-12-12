#ifndef INIT_H
#define INIT_H

#define APP_NAME "lofi"
#define INIT_WIDTH 400
#define INIT_HEIGHT 300
#define INIT_FPS 60
#define INIT_OPACITY 0.9

#include "state.h"
#include "lua.h"

bool TryInitialize(State* state, lua_State* L);

#endif // INIT_H
