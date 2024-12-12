#include "state.h"

#include "raylib.h"
#include <string.h>
#include <stdlib.h>

// void InitStateStruct(State* state) {
//     memset(state->input, 0, sizeof(state->input));
//     state->list = NULL;
//     state->listCount = 0;
//     state->focus.index = -1;
//     state->focus.object = F_TEXTBOX;
//     state->ui = {
//         .loaded {
//             .font = GetDefaultFont();
//             .theme = DEFAULT_THEME ;
//             .window {0, 0};
//         };
//         .config {
//             .font {
//                 .name = "default";
//                 .size = 12;
//             };
//             .
// }
//     };
// }

void CleanupState(State* state) {
    ClearListViewExList(&state->list, &state->listCount);
}
