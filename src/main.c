#include "raylib.h"
#include "lua.h"

#include <GLFW/glfw3.h>

#include "init.h"
#include "state.h"
#include "callbacks/events.h"
#include "callbacks/handler.h"
#include "input/definitions.h"
#include "ui.h"

// #define RAYGUI_IMPLEMENTATION 
// #include "raygui.h"

int main(void) {    
    SetTraceLogLevel(LOG_DEBUG);

    State state;
    lua_State *L = luaL_newstate();
    
    TryInitialize(&state, L);
    
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

    CleanupAppState(L, &state);
    CloseWindow();

    return 0;
}
