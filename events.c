#include "events.h"
#include "raylib.h"
#include <string.h>
#include <stdlib.h>

const char* eventStrs[] = {"EVENT_NONE", "EVENT_SEARCH_TRIGGERED", "EVENT_FOCUS_CHANGED", "EVENT_ITEM_SELECTED", "EVENT_SCROLL", "EVENT_EXIT"};

char *getEventAsStr(AppEventType event_type) {
    return eventStrs[event_type];
}

void event_queue_init(EventQueue* queue) {
    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;
}

int event_queue_push(EventQueue* queue, AppEvent event) {
    if (queue->size >= MAX_EVENT_QUEUE) {
        return 0;  // Queue is full
    }
    
    queue->rear = (queue->rear + 1) % MAX_EVENT_QUEUE;
    queue->events[queue->rear] = event;
    queue->size++;
    
    TraceLog(LOG_DEBUG, "EVENT: Event pushed: %s", getEventAsStr(event.type));
    return 1;
}

AppEvent event_queue_pop(EventQueue* queue) {
    AppEvent event = {EVENT_NONE};
    
    if (event_queue_is_empty(queue)) {
        return event;
    }
    
    event = queue->events[queue->front];
    queue->front = (queue->front + 1) % MAX_EVENT_QUEUE;
    queue->size--;
    
    return event;
}

int event_queue_is_empty(EventQueue* queue) {
    return queue->size == 0;
}