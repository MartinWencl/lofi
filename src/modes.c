#include "modes.h"
#include <stdlib.h>
#include <string.h>
#include "raylib.h"

void InitModeManager(ModeManager *modeManager) {
    memset(modeManager->modes, 0, sizeof(modeManager->modes));
    modeManager->currentMode = NULL;
    modeManager->modeCount = 0;
    // modeManager->luaState = NULL;

    TraceLog(LOG_DEBUG, "STATE: Initialized the mode manager.");   
}
