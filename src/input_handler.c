#include <stdlib.h>
#include <string.h>
#include "input.h"

// Existing implementation from previous input handler remains the same

// Fluent Interface Implementation
InputMappingBuilder* InputHandlerBegin(InputHandler* handler) {
    if (!handler) return NULL;
    
    // Allocate builder
    InputMappingBuilder* builder = malloc(sizeof(InputMappingBuilder));
    if (!builder) return NULL;
    
    // Initialize builder with default values
    builder->handler = handler;
    builder->condition = NULL;
    builder->eventGenerator = NULL;
    builder->stateModifier = NULL;
    builder->priority = 10; // Default medium priority
    
    return builder;
}

InputMappingBuilder* When(InputMappingBuilder* builder, InputConditionFunc condition) {
    if (!builder) return NULL;
    builder->condition = condition;
    return builder;
}

InputMappingBuilder* Generate(InputMappingBuilder* builder, InputEventGeneratorFunc eventGenerator) {
    if (!builder) return NULL;
    builder->eventGenerator = eventGenerator;
    return builder;
}

InputMappingBuilder* Modify(InputMappingBuilder* builder, InputStateModifierFunc stateModifier) {
    if (!builder) return NULL;
    builder->stateModifier = stateModifier;
    return builder;
}

InputMappingBuilder* Priority(InputMappingBuilder* builder, int priority) {
    if (!builder) return NULL;
    builder->priority = priority;
    return builder;
}

bool End(InputMappingBuilder* builder) {
    if (!builder || !builder->handler) {
        if (builder) free(builder);
        return false;
    }
    
    // Register the mapping
    bool result = RegisterInputMapping(
        builder->handler,
        builder->condition,
        builder->eventGenerator,
        builder->stateModifier,
        builder->priority
    );
    
    // Free the builder
    free(builder);
    
    return result;
}
