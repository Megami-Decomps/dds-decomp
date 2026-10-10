#ifndef KWLN_H
#define KWLN_H

#include "common.h"

/* Shared fog/depth state consumed by sdfConsBuildFrustumPacket (0x14).
 * The viewer config serializes fogNear, depthNear, fogFar, depthFar, fogColor;
 * timeline payloads store fogFar before fogNear instead. */
typedef struct KwlnDrawVectorParams {
    f32 fogNear;    /* 0x00: minimum fog range. */
    f32 fogFar;     /* 0x04: maximum fog range. */
    f32 depthNear;  /* 0x08: near depth used in the projection coefficients. */
    f32 depthFar;   /* 0x0C: far depth used in the projection coefficients. */
    u32 fogColor;   /* 0x10: packed GS FOGCOL value. */
} KwlnDrawVectorParams;

typedef char KwlnDrawVectorParams_size[(sizeof(KwlnDrawVectorParams) == 0x14) ? 1 : -1];

/* Scheduler task links, callbacks and hierarchy (0x50); DDS1/2 kernel/dds3KernelCore.c and game task units. */
typedef struct KwlnTask KwlnTask;
typedef s32 (*TaskUpdate)(KwlnTask *task);
typedef void (*TaskDestroy)(KwlnTask *task);

struct KwlnTask {
    char name[0x18];          /* 0x00: NUL-padded task name, compared byte-wise */
    s32 nameSum;             /* 0x18 */
    u32 flags;               /* 0x1C */
    u32 priority;            /* 0x20: ascending task queue priority. */
    u32 unk24;
    u32 timer;               /* 0x28 */
    s16 startDelayTicks;     /* 0x2C: signed; positive values count down, zero activates, negatives remain queued. */
    s16 destroyDelayTicks;   /* 0x2E: signed; positive values count down, zero destroys, negatives stay pending. */
    TaskUpdate update;       /* 0x30 */
    TaskDestroy destroy;     /* 0x34 */
    u32 userValue;           /* 0x38: caller-supplied value exposed by accessors. */
    KwlnTask *listNext;      /* 0x3C */
    KwlnTask *listPrev;      /* 0x40 */
    KwlnTask *parent;        /* 0x44 */
    KwlnTask *childList;     /* 0x48 */
    KwlnTask *next;          /* 0x4C */
};

u32 kwlnTaskGetTimer(KwlnTask *task);
/* Return the task's low scheduler state when registered, otherwise DETACHED. */
s32 kwlnTaskGetRegisteredState(KwlnTask *task);
void kwlnTaskSetUserValue(KwlnTask *task, u32 value);
u32 kwlnTaskGetUserValue(KwlnTask *task);
KwlnTask *kwlnTaskGetTaskByName(const char *name);
s32 kwlnTaskIsRegistered(KwlnTask *task);
s32 kwlnTaskExists(const char *name);

#endif /* KWLN_H */
