#ifndef SDF_COUNTER_CHANNEL_H
#define SDF_COUNTER_CHANNEL_H

#include "common.h"

typedef struct SdfCounterDisplay {
    u32 *word;   /* 0x00 */
    u8 *info;    /* 0x04 */
    s16 value;   /* 0x08 */
    u8 pad0A[4];
    s16 flag;    /* 0x0E */
} SdfCounterDisplay;

typedef struct SdfCounterChannel {
    s32 index;                       /* 0x00 */
    u8 pad04[0x54];
    struct SdfCounterChannel *next;   /* 0x58 */
    struct SdfCounterChannel *prev;   /* 0x5C */
    u8 pad60[0x10];
    SdfCounterDisplay *display;       /* 0x70 */
} SdfCounterChannel;

u32 *sdfCounterGetDisplayWordPointer(void);

typedef char SdfCounterDisplaySizeCheck[(sizeof(SdfCounterDisplay) == 0x10) ? 1 : -1];
typedef char SdfCounterDisplayValueOffset[
    ((unsigned int)&((SdfCounterDisplay *)0)->value == 0x08) ? 1 : -1
];
typedef char SdfCounterDisplayFlagOffset[
    ((unsigned int)&((SdfCounterDisplay *)0)->flag == 0x0E) ? 1 : -1
];
typedef char SdfCounterChannelSizeCheck[(sizeof(SdfCounterChannel) == 0x74) ? 1 : -1];
typedef char SdfCounterChannelNextOffset[
    ((unsigned int)&((SdfCounterChannel *)0)->next == 0x58) ? 1 : -1
];
typedef char SdfCounterChannelPrevOffset[
    ((unsigned int)&((SdfCounterChannel *)0)->prev == 0x5C) ? 1 : -1
];
typedef char SdfCounterChannelDisplayOffset[
    ((unsigned int)&((SdfCounterChannel *)0)->display == 0x70) ? 1 : -1
];

#endif /* SDF_COUNTER_CHANNEL_H */
