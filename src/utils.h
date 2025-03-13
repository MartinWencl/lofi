#ifndef UTILS_H
#define UTILS_H

#include "state.h"

void AddToListViewExList(char ***listViewExList, int *listViewExCount, const char *newItem);
void ClearListViewExList(char ***listViewExList, int *listViewExCount);
void Search(const char *query, char ***listViewExList, int *listViewExCount);

void PrintConfiguration(const State* state);

#endif // UTILS_H
