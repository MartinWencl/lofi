#ifndef UI_WINDOW_H
#define UI_WINDOW_H

#define DEFAULT_WINDOW_PERCENT_WIDTH 0.8
#define DEFAULT_WINDOW_PERCENT_HEIGHT 0.7

#define FALLBACK_WINDOW_WIDTH 600
#define FALLBACK_WINDOW_HEIGHT 800

#include <stdbool.h>

typedef struct Dimensions {
    int width;
    int height;
} Dimensions;

typedef Dimensions MonitorDimensions;

typedef struct {
    int refreshRate;             // Override refresh rate, 0 to use monitor's default
    float windowPercentWidth;    // Window width as a percentage of monitor width (0.0 - 1.0)
    float windowPercentHeight;   // Window height as a percentage of monitor height (0.0 - 1.0)
    float opacity;               // Window opacity (0.0 - 1.0)
    bool decoration;             // Window decoration (title bar and borders)
} WindowConfig;

extern const WindowConfig DEFAULT_WINDOW_CONFIG;

Dimensions InitWindowFromConfig(const WindowConfig* config, char* appName); 

#endif // UI_WINDOW_H
