#ifndef CONTAINERS_LIST_H
#define CONTAINERS_LIST_H

typedef enum {
    LIST_ORIENTATION_VERTICAL,
    LIST_ORIENTATION_HORIZONTAL
} ListOrientation;

typedef struct {
    ListOrientation orientation;
    Dimension* childSizes;    // Sizing for each child widget
} ListContainer;

#endif // CONTAINERS_LIST_H