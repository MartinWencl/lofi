#ifndef UI_FOCUS_H
#define UI_FOCUS_H

#include "ui/widgets.h"
 
typedef enum {
    F_TEXTBOX = -1,
    F_LIST
} Focusable;

typedef struct Focus {
    int index;
    Focusable object;
} Focus;

typedef enum {
    FOCUS_DIRECTION_NEXT,
    FOCUS_DIRECTION_PREV,
    FOCUS_DIRECTION_UP,
    FOCUS_DIRECTION_DOWN,
    FOCUS_DIRECTION_LEFT,
    FOCUS_DIRECTION_RIGHT
} FocusDirection;

typedef struct {
    Widget* currentFocusedWidget;
    Widget* rootWidget;

    struct {
        bool allowWrapAround;    // Wrap focus to start/end of hierarchy
        bool preventCyclicFocus; // Prevent repeated focus on same widget
    } options;
} FocusManager;

#endif // UI_FOCUS_H
