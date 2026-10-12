#ifndef BTL_AI_TABLE_H
#define BTL_AI_TABLE_H

#include "common.h"

/* Weighted action entry in an enemy species' AI table. */
typedef struct AiSlot {
    u8 weight; /* 0x00: used by the weighted-slot selector. */
    u8 pad01;
    u16 actionId; /* 0x02 */
    u32 actionArg; /* 0x04 */
} AiSlot;

typedef char AiSlot_size_must_be_8[(sizeof(AiSlot) == 8) ? 1 : -1];
typedef char AiSlot_actionId_offset_must_be_2[
    ((u32)&((AiSlot *)0)->actionId == 2) ? 1 : -1];
typedef char AiSlot_actionArg_offset_must_be_4[
    ((u32)&((AiSlot *)0)->actionArg == 4) ? 1 : -1];

#endif /* BTL_AI_TABLE_H */
