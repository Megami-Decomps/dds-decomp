#ifndef KWLN_H
#define KWLN_H

#include "common.h"

struct sdfDrawTag_t;

/* Shared fog/depth state consumed by sdf3DMakeFogPacket (0x14).
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
typedef struct dds3ProcessID_t dds3ProcessID_t;
/* Callback result: 0 retains the callback, -1 finishes the task, and
 * other words select a replacement callback. */
typedef s32 KwlnTaskUpdateResult;
typedef KwlnTaskUpdateResult (*KwlnTaskUpdateCallback)(dds3ProcessID_t *task);
typedef void (*TaskDestroy)(dds3ProcessID_t *task);

struct dds3ProcessID_t {
    char name[0x18];          /* 0x00: NUL-padded task name, compared byte-wise */
    s32 nameSum;             /* 0x18 */
    u32 flags;               /* 0x1C */
    u32 priority;            /* 0x20: ascending task queue priority. */
    u32 unk24;
    u32 runUpdateCount;      /* 0x28 */
    s16 startDelayTicks;     /* 0x2C: signed; positive values count down, zero activates, negatives remain queued. */
    s16 destroyDelayTicks;   /* 0x2E: signed; positive values count down, zero destroys, negatives stay pending. */
    KwlnTaskUpdateCallback update; /* 0x30 */
    TaskDestroy destroy;     /* 0x34 */
    u32 userValue;           /* 0x38: caller-supplied value exposed by accessors. */
    dds3ProcessID_t *listNext;      /* 0x3C */
    dds3ProcessID_t *listPrev;      /* 0x40 */
    dds3ProcessID_t *parent;        /* 0x44 */
    dds3ProcessID_t *childList;     /* 0x48 */
    dds3ProcessID_t *nextSibling;   /* 0x4C: next task in the parent's child list. */
};

void dds3SetProcessData(dds3ProcessID_t *task, u32 userValue);
u32 dds3GetProcessData(const dds3ProcessID_t *task);
/* Return a borrowed scheduler-owned handle from the starting, active, or
 * destroy-pending queue. The handle remains valid through its destroy callback
 * and is freed when finalization completes; return NULL when no name matches. */
dds3ProcessID_t *dds3SearchProcessName(const char *name);
/* Detach task from its parent hierarchy; scheduler queue links are unchanged. */
void dds3UnlinkProcess(dds3ProcessID_t *task);

/* Append the common rectangle used to frame a grid of sprite cells. */
void dds3ConsPanelTagLink(struct sdfDrawTag_t *packetList, s32 column, s32 row,
                        s32 columnCount, s32 rowCount);

#endif /* KWLN_H */
