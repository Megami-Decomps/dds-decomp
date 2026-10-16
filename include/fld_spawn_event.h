#ifndef FLD_SPAWN_EVENT_H
#define FLD_SPAWN_EVENT_H

#include "common.h"

/* FLD type-6 event payload: tools/fld.py writes four words and relocates the
 * label pointer. Runtime spawning reads the label; preserve the other words
 * without assigning them unverified meaning. */
typedef struct FldSpawnEvent {
    u32 flags;          /* 0x00 */
    const char *label;  /* 0x04 */
    u32 opaque08[2];    /* 0x08 */
} FldSpawnEvent;

typedef char FldSpawnEventSizeCheck[(sizeof(FldSpawnEvent) == 0x10) ? 1 : -1];
typedef char FldSpawnEventLabelOffsetCheck[
    ((u32)&((FldSpawnEvent *)0)->label == 0x04) ? 1 : -1];
typedef char FldSpawnEventOpaqueOffsetCheck[
    ((u32)&((FldSpawnEvent *)0)->opaque08 == 0x08) ? 1 : -1];

#endif
