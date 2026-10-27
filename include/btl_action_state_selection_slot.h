#ifndef BTL_ACTION_STATE_SELECTION_SLOT_H
#define BTL_ACTION_STATE_SELECTION_SLOT_H

#include "btl_action_state_selection.h"

/* DDS1 stores this hook at +8 in the 0x14-byte callback window at BtlState+0x5F4. */
typedef struct BtlDds1ActionStateSelectionCallbackWindow {
    u8 pad00[8];
    BtlActionStateSelectionHook actionStateSelectionHook;
    u8 pad0C[8];
} BtlDds1ActionStateSelectionCallbackWindow;
typedef char BtlDds1ActionStateSelectionWindowSizeCheck[
    (sizeof(BtlDds1ActionStateSelectionCallbackWindow) == 0x14) ? 1 : -1];
typedef char BtlDds1ActionStateSelectionHookOffsetCheck[
    ((u32)&((BtlDds1ActionStateSelectionCallbackWindow *)0)->actionStateSelectionHook == 8) ? 1 : -1];

#endif /* BTL_ACTION_STATE_SELECTION_SLOT_H */
