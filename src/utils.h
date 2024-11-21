#ifndef UTILS_H
#define UTILS_H

void AddToListViewExList(char ***listViewExList, int *listViewExCount, const char *newItem);
void ClearListViewExList(char ***listViewExList, int *listViewExCount);
void Search(const char *query, char ***listViewExList, int *listViewExCount);

#endif // UTILS_H
