#ifndef SCRIPT_FLAG_SLOT_H
#define SCRIPT_FLAG_SLOT_H

#include "common.h"

typedef struct ScriptFlagSlot {
    u8 id;             /* 0x00 */
    u8 pad01[3];       /* opaque */
    u32 flag;          /* 0x04 */
} ScriptFlagSlot;

typedef char ScriptFlagSlot_size_must_be_0x08[
    (sizeof(ScriptFlagSlot) == 0x08) ? 1 : -1];

#endif
