#include "events.h"
#include "raylib.h"
#include <string.h>
#include <stdlib.h>

//TODO: do it better
const char* eventStrs[] = {"EVENT_NONE", "on_search", "on_select", "on_focus_change", "EVENT_EXIT"};

char *GetEventAsStr(EventType event_type) {
    return eventStrs[event_type];
}

void InitEventQueue(EventQueue* queue) {
    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;
    TraceLog(LOG_DEBUG, "STATE: Initialized the event queue.");
}

int EventQueuePush(EventQueue* queue, Event event) {
    if (queue->size >= MAX_EVENT_QUEUE) {
        return 0;  // Queue is full
    }
    
    queue->rear = (queue->rear + 1) % MAX_EVENT_QUEUE;
    queue->events[queue->rear] = event;
    queue->size++;
    
    TraceLog(LOG_DEBUG, "EVENT: Event pushed: %s", GetEventAsStr(event.type));
    return 1;
}

Event EventQueuePop(EventQueue* queue) {
    Event event = {EVENT_NONE};
    
    if (IsEventQueueEmpty(queue)) {
        return event;
    }
    
    event = queue->events[queue->front];
    queue->front = (queue->front + 1) % MAX_EVENT_QUEUE;
    queue->size--;
    
    return event;
}

int IsEventQueueEmpty(EventQueue* queue) {
    return queue->size == 0;
}
