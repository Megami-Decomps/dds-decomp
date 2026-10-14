#ifndef EFF_CLASS_OPS_H
#define EFF_CLASS_OPS_H

#include "common.h"

/* DDS1 0029C530 / DDS2 002DE248 use this callback table for native classes.
 * The two units dispatch each slot directly and read payloadSize for sizing. */
typedef struct EffClassOps {
    void (*initialize)();
    void *(*createResource)();
    void (*destroyResource)();
    void (*update)();
    void (*draw)();
    u32 payloadSize;
} EffClassOps;

typedef char EffClassOpsSizeCheck[(sizeof(EffClassOps) == 0x18) ? 1 : -1];
typedef char EffClassOpsCreateOffsetCheck[
    ((u32)&((EffClassOps *)0)->createResource == 0x04) ? 1 : -1];
typedef char EffClassOpsPayloadSizeOffsetCheck[
    ((u32)&((EffClassOps *)0)->payloadSize == 0x14) ? 1 : -1];

#endif
