#include "raylib.h"
#include "lua.h"

#include "init.h"
#include "state.h"
#include "callbacks/events.h"
#include "callbacks/handler.h"
#include "input/definitions.h"
#include "ui.h"

#define RAYGUI_IMPLEMENTATION 
#include "raygui.h"

int main(void) {    
    SetTraceLogLevel(LOG_DEBUG);

    State state;
    lua_State *L = luaL_newstate();
    
    TryInitialize(&state, L);
    
    while (!WindowShouldClose()) {
        // Process input and generate events
        ProcessInput(&state, &state.eventQueue);

        // Process all pending events
        while (!IsEventQueueEmpty(&state.eventQueue)) {
            Event event = EventQueuePop(&state.eventQueue);
            HandleEvent(&state, &state.modes, event);
        }

        // Drawing
        BeginDrawing();

        DrawUI(&state, state.ui.loaded.window);

        EndDrawing();
    }

    // Cleanup
    CleanupState(&state);
    CloseWindow();

    return 0;
}
