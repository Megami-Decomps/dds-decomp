#ifndef KWLN_H
#define KWLN_H

#include "common.h"

/* Scheduler task links and callbacks; identical in DDS1 and DDS2. */
typedef struct KwlnTask KwlnTask;
typedef s32 (*TaskUpdate)(KwlnTask *task);
typedef void (*TaskDestroy)(KwlnTask *task);

struct KwlnTask {
    u8 unk00[0x18];
    s32 nameSum;             /* 0x18 */
    u32 flags;               /* 0x1C */
    u32 unk20;
    u32 unk24;
    u32 timer;               /* 0x28 */
    s16 unk2C;
    s16 unk2E;
    TaskUpdate update;       /* 0x30 */
    TaskDestroy destroy;     /* 0x34 */
    u32 unk38;
    KwlnTask *listNext;      /* 0x3C */
    KwlnTask *listPrev;      /* 0x40 */
    KwlnTask *parent;        /* 0x44 */
    KwlnTask *childList;     /* 0x48 */
    KwlnTask *next;          /* 0x4C */
};

#endif /* KWLN_H */
