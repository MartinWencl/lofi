#include "raylib.h"
#include "lua.h"

#include <GLFW/glfw3.h>
#include <assert.h>

#include "state.h"
#include "lua_core.h"
#include "callbacks/events.h"
#include "callbacks/handler.h"
#include "input/definitions.h"
#include "ui.h"
#include "utils.h"
#include "window.h"

int main(void) {    
    SetTraceLogLevel(LOG_DEBUG);

    State* lofi = NewState();
    assert(lofi != nullptr); 

    lua_State* lua = NewLuaState(lofi);
    assert(lua != nullptr); 

    LoadOptionsFromLua(lofi, lua);

    // change to newwidowfromconfig later and keep it in a variable here
    Dimensions windowDimenstions = InitWindowFromConfig(&lofi->ui.config.window, "lofi");
    lofi->ui.loaded.window = windowDimenstions;

    TraceLog(LOG_INFO, "Setup complete!");
    
    while (!WindowShouldClose()) {
        ProcessInput(lofi, &lofi->eventQueue, lua);

        while (!IsEventQueueEmpty(&lofi->eventQueue)) {
            Event event = EventQueuePop(&lofi->eventQueue);
            HandleEvent(lua, lofi, &lofi->modes, event);
        }

        BeginDrawing();
        DrawUI(lofi, lofi->ui.loaded.window);
        EndDrawing();
    }

    assert(&lofi->list != nullptr); 
    ClearListViewExList(&lofi->list, &lofi->listCount);

    assert(lofi != nullptr); 
    FreeState(lofi);

    assert(lua != nullptr); 
    lua_close(lua);

    CloseWindow();
    return 0;
}
