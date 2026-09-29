#include "common.h"

extern u64 sdfFindThreadNode(u64);

typedef struct SdfCursorNode {
    struct SdfCursorNode *next;
} SdfCursorNode;

typedef struct SdfNodeCursor {
    SdfCursorNode *current;
    SdfCursorNode *next;
} SdfNodeCursor;

void func_00328AC8(void) {
    u64 node;

    node = sdfFindThreadNode(0xffffffffffffffff);
    func_003289C8(node);
}

INCLUDE_ASM(const s32, "game/code_00328AC8", func_00328AE8);

INCLUDE_ASM(const s32, "game/code_00328AC8", func_00328B20);

INCLUDE_ASM(const s32, "game/code_00328AC8", func_00328BA0);

INCLUDE_ASM(const s32, "game/code_00328AC8", sdfThreadSleepSelf);

void sdfAdvanceNodeCursor(SdfNodeCursor *cursor) {
    SdfCursorNode *node;

    node = cursor->next;
    if (node != NULL) {
        cursor->next = node->next;
    }
    cursor->current = node;
}

INCLUDE_ASM(const s32, "game/code_00328AC8", sdfAdvanceCursorWalk);

INCLUDE_ASM(const s32, "game/code_00328AC8", func_00328CA0);

INCLUDE_ASM(const s32, "game/code_00328AC8", sdfCursorSlotAlloc);

INCLUDE_ASM(const s32, "game/code_00328AC8", func_00328D68);

INCLUDE_SDATA(const s32, "game/code_00328AC8", D_004389C8);

