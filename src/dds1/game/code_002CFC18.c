#include "common.h"

extern u64 sdfFindThreadById(u64);

void func_002CFC18(void) {
    u64 thread;

    thread = sdfFindThreadById(0xffffffffffffffff);
    func_002CFB18(thread);
}

INCLUDE_ASM(const s32, "game/code_002CFC18", func_002CFC38);

INCLUDE_ASM(const s32, "game/code_002CFC18", func_002CFC70);

INCLUDE_ASM(const s32, "game/code_002CFC18", func_002CFCF0);

INCLUDE_ASM(const s32, "game/code_002CFC18", func_002CFD50);

typedef struct SdfNode {
    struct SdfNode *next;
} SdfNode;

typedef struct {
    SdfNode *current;
    SdfNode *next;
} SdfNodeCursor;

void sdfAdvanceNodeCursor(SdfNodeCursor *cursor) {
    SdfNode *next;

    next = cursor->next;
    if (next != (SdfNode *)0x0) {
        cursor->next = next->next;
    }
    cursor->current = next;
}

INCLUDE_ASM(const s32, "game/code_002CFC18", func_002CFDA0);

INCLUDE_ASM(const s32, "game/code_002CFC18", func_002CFDF0);

INCLUDE_ASM(const s32, "game/code_002CFC18", func_002CFE70);

INCLUDE_ASM(const s32, "game/code_002CFC18", func_002CFEB8);

INCLUDE_SDATA(const s32, "game/code_002CFC18", D_003BD2D8);

