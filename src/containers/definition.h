typedef struct Container {
    // Base Widget Properties
    Widget base;

    // Child Management
    Widget** children;
    int childCount;
    int maxChildren;

    // Layout Properties
    Dimension* childSizes;     // Sizing for each child
    LayoutType layoutType;     // Grid, List, Flex, etc.
    
    // Layout Configuration
    struct {
        bool expandChildren;   // Allow children to expand
        bool shrinkChildren;   // Allow children to shrink
        Alignment childAlignment;
        Spacing childSpacing;
    } layoutOptions;

    // Callback Function Pointers
    // Layout Callbacks
    void (*calculateLayout)(struct Container* container, Rectangle parentBounds);
    void (*arrangeChildren)(struct Container* container);
    void (*resizeChildren)(struct Container* container);

    // Rendering Callbacks
    void (*preRender)(struct Container* container);
    void (*postRender)(struct Container* container);
    void (*renderBackground)(struct Container* container);
    void (*renderOverlay)(struct Container* container);

    // Focus Management Callbacks
    Widget* (*findNextFocusableChild)(
        struct Container* container, 
        Widget* currentWidget, 
        FocusDirection direction
    );
    void (*onChildFocus)(struct Container* container, Widget* childWidget);
    void (*onChildBlur)(struct Container* container, Widget* childWidget);

    // Event Handling Callbacks
    bool (*routeEvent)(struct Container* container, Event* event);
    void (*onChildEvent)(
        struct Container* container, 
        Widget* source, 
        Event* event
    );

    // Lifecycle Callbacks
    void (*onChildAdded)(struct Container* container, Widget* child);
    void (*onChildRemoved)(struct Container* container, Widget* child);
    void (*beforeDestroy)(struct Container* container);

    // Container-specific data
    void* containerData;
} Container;