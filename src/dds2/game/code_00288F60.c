#include "common.h"

extern void mnuCloseCurrentProfilePanel(s32 effect);
extern void mnuEnsureProfilePanelEffect(s32 unused, s32 effect);

typedef struct MenuRenderListNode {
    u8 pad00[0x70];
    u8 *items; /* 0x70 */
} MenuRenderListNode;

typedef struct MenuRenderListContext {
    u8 pad00[0x1C];
    MenuRenderListNode *node; /* 0x1C */
} MenuRenderListContext;

typedef struct MenuRenderState {
    u8 pad00[4];
    MenuRenderListContext *context; /* 0x04 */
    u8 pad08[0x40];
    s32 effect; /* 0x48 */
    u8 pad4C[8];
    u8 snapshot[0x1C4]; /* 0x54 */
} MenuRenderState;

/* Tear the panel's effect down, snapshot the live list, then bring the panel
 * back with the snapshot in front of it. */
void mnuRebuildProfilePanelFromRenderSnapshot(MenuRenderState *state) {
    mnuCloseCurrentProfilePanel(state->effect);
    memcpy(state->snapshot, state->context->node->items, 0x1C4);
    mnuEnsureProfilePanelEffect((s32)state->snapshot, state->effect);
}
