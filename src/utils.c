#include "raylib.h"
#include "string.h"
#include "utils.h"
#include <stdlib.h>

#define RESULT_MAX_SIZE 128
#define MAX_RESULTS 64

void AddToListViewExList(char ***listViewExList, int *listViewExCount, const char *newItem)
{ 
    *listViewExList = realloc(*listViewExList, (*listViewExCount + 1) * sizeof(char *));
    (*listViewExList)[*listViewExCount] = malloc((strlen(newItem) + 1) * sizeof(char));
    strcpy((*listViewExList)[*listViewExCount], newItem);
    (*listViewExCount)++;
}
 
void ClearListViewExList(char ***listViewExList, int *listViewExCount) 
{ 
    for (int i = 0; i < *listViewExCount; i++)
    { 
        free((*listViewExList)[i]);
    } 
    free(*listViewExList);
    *listViewExList = NULL;
    *listViewExCount = 0;
}

void Search(const char *query, char ***listViewExList, int *listViewExCount) {
    FilePathList files = LoadDirectoryFiles(GetWorkingDirectory()); // Load all files in working directory and subdirectories
    TraceLog(LOG_INFO, "SEARCH: Searching for query: %s", query);
    TraceLog(LOG_INFO, "SEARCH: Number of files to search: %i", files.count);
    
    // Clear the previous results
    ClearListViewExList(listViewExList, listViewExCount);
    
    for (unsigned int i = 0; i < files.count; i++) {
        const char *fileName = GetFileName(files.paths[i]);
        if (strstr(fileName, query) != NULL) {
            AddToListViewExList(listViewExList, listViewExCount, fileName);
        }
    }
    
    UnloadDirectoryFiles(files); // Clean up file paths
                
    TraceLog(LOG_INFO, "SEARCH: Search finished, number of matches: %i", *listViewExCount);
}
