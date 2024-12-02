#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "raylib.h"
#include "core.h"
#include "events.h"

// Forward declarations
typedef struct InputHandler InputHandler;
typedef struct InputMappingBuilder InputMappingBuilder;

// Type definitions (same as previous implementation)
typedef bool (*InputConditionFunc)(void);
typedef AppEvent (*InputEventGeneratorFunc)(AppState* state);
typedef void (*InputStateModifierFunc)(AppState* state);

// Fluent Builder Structure
struct InputMappingBuilder {
    InputHandler* handler;
    InputConditionFunc condition;
    InputEventGeneratorFunc eventGenerator;
    InputStateModifierFunc stateModifier;
    int priority;
};

// Fluent method to start building an input mapping
InputMappingBuilder* InputHandlerBegin(InputHandler* handler);

// Fluent methods to configure the mapping
InputMappingBuilder* When(InputMappingBuilder* builder, InputConditionFunc condition);
InputMappingBuilder* Generate(InputMappingBuilder* builder, InputEventGeneratorFunc eventGenerator);
InputMappingBuilder* Modify(InputMappingBuilder* builder, InputStateModifierFunc stateModifier);
InputMappingBuilder* Priority(InputMappingBuilder* builder, int priority);

// Finalize and register the mapping
bool End(InputMappingBuilder* builder);

#endif // INPUT_HANDLER_H
