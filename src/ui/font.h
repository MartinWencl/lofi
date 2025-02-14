#ifndef UI_FONT_H
#define UI_FONT_H

// forward declaration as ui/state.h needs FontConfig
// but i want to keep the loading of the font here
struct UIState;

typedef struct {
    char* name;
    int size;
} FontConfig;

void LoadFontFromState(struct UIState* ui);
void FreeFont(struct UIState* ui);

#endif // UI_FONT_H
