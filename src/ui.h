#ifndef UI_H
#define UI_H

#include "core.h"
#include "listview.h"
#include "inputbox.h"

#include "raygui.h"

typedef struct Dimensions {
    int width;
    int height;
} Dimensions;

typedef Dimensions MonitorDimensions;
typedef Dimensions WindowDimensions;

void DrawUI(AppState* state, WindowDimensions windowDimensions);

#endif //UI_H