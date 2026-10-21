#ifndef SDF_COUNTER_DISPLAY_H
#define SDF_COUNTER_DISPLAY_H

#include "common.h"

/* A separately allocated label/scene record owned by MenuListNode.counterDisplay. */
typedef struct SdfCounterDisplay {
    const char *label; /* 0x00: fixed-stride local-map name table */
    u8 *info;         /* 0x04 */
    s16 value;        /* 0x08 */
    u8 pad0A[4];
    s16 flag;         /* 0x0E */
} SdfCounterDisplay;

const char *sdfCounterGetDisplayWordPointer(void);

typedef char SdfCounterDisplaySizeCheck[(sizeof(SdfCounterDisplay) == 0x10) ? 1 : -1];
typedef char SdfCounterDisplayValueOffset[
    ((unsigned int)&((SdfCounterDisplay *)0)->value == 0x08) ? 1 : -1
];
typedef char SdfCounterDisplayFlagOffset[
    ((unsigned int)&((SdfCounterDisplay *)0)->flag == 0x0E) ? 1 : -1
];

#endif /* SDF_COUNTER_DISPLAY_H */
