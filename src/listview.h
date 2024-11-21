#ifndef LISTVIEW_H
#define LISTVIEW_H

#include "raylib.h"

int ListViewEx(Rectangle bounds, const char **textItems, int itemCount, int *selected, int *scrollIndex);

#endif // LISTVIEW_H