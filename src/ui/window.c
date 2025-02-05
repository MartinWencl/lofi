#include "ui/window.h"
#include "raylib.h"

const WindowConfig DEFAULT_WINDOW_CONFIG = {
    .refreshRate = 0,
    .windowPercentWidth = 0.3f,
    .windowPercentHeight = 0.2f,
    .opacity = 0.9f,
    .decoration = false
};

Dimensions GetSafeMonitorDimensions() {
    int monitor = GetCurrentMonitor();

    Dimensions dimensions = {0, 0};

    if (GetMonitorCount() == 0) {
        TraceLog(LOG_WARNING, "No monitors detected. Using default dimensions.");
        dimensions.width = FALLBACK_WINDOW_WIDTH;
        dimensions.height = FALLBACK_WINDOW_HEIGHT;
    } else {
        dimensions.width = GetMonitorWidth(monitor);
        dimensions.height = GetMonitorHeight(monitor);
    }

    return dimensions;
}

void CenterWindow() {
    int monitor = GetCurrentMonitor();
    int monitorWidth = GetMonitorWidth(monitor);
    int monitorHeight = GetMonitorHeight(monitor);

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    int posX = (monitorWidth - screenWidth) / 2;
    int posY = (monitorHeight - screenHeight) / 2;

    SetWindowPosition(posX, posY);
}

Dimensions SetWindowFromConfig(const WindowConfig* config) {
    float windowPercentWidth = config->windowPercentWidth;
    float windowPercentHeight = config->windowPercentHeight;

    Dimensions dimensions = GetSafeMonitorDimensions();

    TraceLog(LOG_DEBUG, "Monitor width: %d", dimensions.width);
    TraceLog(LOG_DEBUG, "Monitor height: %d", dimensions.height);

    TraceLog(LOG_DEBUG, "Configured window settings:");
    TraceLog(LOG_DEBUG, "Refresh Rate: %s", config->refreshRate > 0 ? TextFormat("%d", config->refreshRate) : "Monitor Default");
    TraceLog(LOG_DEBUG, "Width Percent: %f", windowPercentWidth);
    TraceLog(LOG_DEBUG, "Height Percent: %f", windowPercentHeight);
    TraceLog(LOG_DEBUG, "Opacity: %f", config->opacity);
    TraceLog(LOG_DEBUG, "Decoration: %s", config->decoration ? "Enabled" : "Disabled");

    if (windowPercentWidth <= 0.0f || windowPercentWidth > 1.0f) {
        windowPercentWidth = DEFAULT_WINDOW_PERCENT_WIDTH;
        TraceLog(LOG_ERROR, "Window width percentage out of bounds. Using default value.");
    }

    if (windowPercentHeight <= 0.0f || windowPercentHeight > 1.0f) {
        windowPercentHeight = DEFAULT_WINDOW_PERCENT_HEIGHT;
        TraceLog(LOG_ERROR, "Window height percentage out of bounds. Using default value.");
    }

    Dimensions windowDimensions;
    windowDimensions.width = (int)(dimensions.width * windowPercentWidth);
    windowDimensions.height = (int)(dimensions.height * windowPercentHeight);

    SetWindowSize(windowDimensions.width, windowDimensions.height);
    CenterWindow();

    TraceLog(LOG_DEBUG, "Window width: %i", windowDimensions.width);
    TraceLog(LOG_DEBUG, "Window height: %i", windowDimensions.height);

    SetWindowOpacity(config->opacity);

    if (!config->decoration) {
        SetWindowState(FLAG_WINDOW_UNDECORATED); // Set window as undecorated
    }

    if (config->refreshRate > 0) {
        SetTargetFPS(config->refreshRate);
    } else {
        // Use monitor refresh rate by default if overridden value is not provided
        SetTargetFPS(GetMonitorRefreshRate(0));
    }

    return windowDimensions;
}

Dimensions InitWindowFromConfig(const WindowConfig* config, char* appName) {
    InitWindow(0, 0, appName);
    SetWindowFromConfig(config);
}
