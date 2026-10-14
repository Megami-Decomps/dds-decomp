#ifndef PAR_HISTORY_H
#define PAR_HISTORY_H

#include "common.h"

/* One particle emitter's circular history of three-component points. */
typedef struct ParHistory {
    s32 nextIndex;
    s32 count;
    f32 (*points)[3];
} ParHistory;

/* Indexed history rows share a caller-provided ring capacity. */
typedef struct ParHistoryTable {
    ParHistory *records;
    s32 capacity;
} ParHistoryTable;

typedef char ParHistory_size_must_be_0x0C[
    (sizeof(ParHistory) == 0x0C) ? 1 : -1];
typedef char ParHistory_points_at_0x08[
    ((u32)&((ParHistory *)0)->points == 0x08) ? 1 : -1];
typedef char ParHistoryTable_size_must_be_0x08[
    (sizeof(ParHistoryTable) == 0x08) ? 1 : -1];

#endif /* PAR_HISTORY_H */
