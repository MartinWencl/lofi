#include "raylib.h"
#include "lua.h"

#include <GLFW/glfw3.h>

#include "init.h"
#include "cleanup.h"
#include "state.h"
#include "callbacks/events.h"
#include "callbacks/handler.h"
#include "input/definitions.h"
#include "ui.h"

int main(void) {    
    SetTraceLogLevel(LOG_DEBUG);

    State state;
    lua_State *L = luaL_newstate();
    
    if (!TryInitialize(&state, L)) {
        TraceLog(LOG_ERROR, "Failed to initialize!");
        return 1;
    }
    
    while (!WindowShouldClose()) {
        ProcessInput(&state, &state.eventQueue);

        while (!IsEventQueueEmpty(&state.eventQueue)) {
            Event event = EventQueuePop(&state.eventQueue);
            HandleEvent(L, &state, &state.modes, event);
        }

        BeginDrawing();
        DrawUI(&state, state.ui.loaded.window);
        EndDrawing();
    }

    Cleanup(L, &state);
    return 0;
}
