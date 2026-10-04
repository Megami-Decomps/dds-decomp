#ifndef EFF_QUEUE_H
#define EFF_QUEUE_H

#include "common.h"

/* Native 0x70-byte output of the effect resource-name chooser in DDS1/2.
 * The producer fills both 50-byte strings: the prefixed name is formatted
 * into a file path, while the plain name is retained for later requests.
 * DDS1 reads completion unsigned and DDS2 also compares its signed view. */
typedef struct EffQueueRecord {
    char nameWithPrefix[0x32];
    char name[0x32];
    union {
        s32 signedState; /* +0x64 */
        u32 state;
    } completion;
    u8 pad68[8];
} EffQueueRecord;

extern void effUpdateResourceQueue(u32 *, void *, EffQueueRecord *);

#endif /* EFF_QUEUE_H */
