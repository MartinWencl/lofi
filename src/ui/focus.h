#ifndef UI_FOCUS_H
#define UI_FOCUS_H
 
typedef enum {
    F_TEXTBOX = -1,
    F_LIST
} Focusable;

typedef struct Focus {
    int index;
    Focusable object;
} Focus;

#endif // UI_FOCUS_H
