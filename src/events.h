#ifndef EVENTS_H
#define EVENTS_H

#define NUM_EVENTS EVENT_EXIT + 1

typedef enum {
    EVENT_NONE = 0,
    EVENT_SEARCH_TRIGGERED,
    EVENT_FOCUS_CHANGED,
    EVENT_ITEM_SELECTED,
    EVENT_SCROLL,
    
    // HACK: keep exit last, it's used to calc the number of elems
    // don't explicitly number events
    EVENT_EXIT
} AppEventType;

// Struct to represent an application event
typedef struct {
    AppEventType type;
    union {
        struct {
            char* searchQuery;
        } search;
        
        struct {
            int index;
            int objectType;  // From existing Focus object type
        } focus;
        
        struct {
            int selectedIndex;
        } selection;
        
        struct {
            int scrollAmount;
        } scroll;
    } data;
} AppEvent;

// Event queue structure
#define MAX_EVENT_QUEUE 64

typedef struct {
    AppEvent events[MAX_EVENT_QUEUE];
    int front;
    int rear;
    int size;
} EventQueue;

// Event queue management functions
void InitEventQueue(EventQueue* queue);
int EventQueuePush(EventQueue* queue, AppEvent event);
AppEvent EventQueuePop(EventQueue* queue);
int IsEventQueueEmpty(EventQueue* queue);

char *GetEventAsStr(AppEventType event_type);

#endif // EVENTS_H