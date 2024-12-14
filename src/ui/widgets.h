#ifndef UI_WIDGETS_H
#define UI_WIDGETS_H

#include "raylib.h"

typedef struct Widget {
    // Widget-specific data
    // in a perfect word not no be used
    void* data;

    // Rendering callback
    void (*draw)(struct Widget* widget, Rectangle rect, State* state);

    // Input handling callbacks (array to support multiple)
    InputCallback inputCallbacks[MAX_INPUT_CALLBACKS];
    int inputCallbackCount;

    // State update callback
    void (*update)(struct Widget* widget, State* state);

    // Focus-related function pointers
    bool (*canReceiveFocus)(struct Widget* widget);
    bool (*moveFocus)(struct Widget* widget, FocusDirection direction);
    void (*onFocus)(struct Widget* widget);
    void (*onBlur)(struct Widget* widget);

    // Hierarchy and navigation properties
    struct Widget* parent;
    struct Widget** children;
    int childCount;

    // Widget state flags
    bool focusable;
    bool visible;

    // Widget type identifier (for container-specific logic)
    // WidgetType type;
} Widget;

#endif // UI_WIDGETS_H
