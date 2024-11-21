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
            char* search_query;
        } search;
        
        struct {
            int index;
            int object_type;  // From existing Focus object type
        } focus;
        
        struct {
            int selected_index;
        } selection;
        
        struct {
            int scroll_amount;
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
void event_queue_init(EventQueue* queue);
int event_queue_push(EventQueue* queue, AppEvent event);
AppEvent event_queue_pop(EventQueue* queue);
int event_queue_is_empty(EventQueue* queue);

char *getEventAsStr(AppEventType event_type);

#endif // EVENTS_H