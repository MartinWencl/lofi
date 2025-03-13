#ifndef CALLBACK_EVENTS_H
#define CALLBACK_EVENTS_H

#define NUM_EVENTS EVENT_EXIT + 1
#define MAX_EVENT_QUEUE 64

typedef enum {
    EVENT_NONE = 0,
    EVENT_SEARCH_TRIGGERED,
    EVENT_ITEM_SELECTED,
    EVENT_FOCUS_CHANGED,
    
    // HACK: keep exit last, it's used to calc the number of elems
    // don't explicitly number events
    EVENT_EXIT
} EventType;

typedef struct {
    EventType type;
    struct {
        struct {
            int index;
        } focus;
    } data;
} Event;

typedef struct {
    Event events[MAX_EVENT_QUEUE];
    int front;
    int rear;
    int size;
} EventQueue;

EventQueue NewEventQueue(void);
int EventQueuePush(EventQueue* queue, Event event);
Event EventQueuePop(EventQueue* queue);
int IsEventQueueEmpty(EventQueue* queue);

char *GetEventAsStr(EventType event_type);

#endif // CALLBACK_EVENTS_H
