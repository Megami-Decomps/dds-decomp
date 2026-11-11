#ifndef KERNEL_H
#define KERNEL_H

#include "common.h"

struct sdfDrawTag_t;

/* Scheduler task links, callbacks and hierarchy (0x50); DDS1/2 kernel/dds3KernelCore.c and game task units. */
typedef struct dds3ProcessID_t dds3ProcessID_t;
/* Callback result: 0 retains the callback, -1 finishes the task, and
 * other words select a replacement callback. */
typedef s32 dds3TaskUpdateResult_t;
typedef dds3TaskUpdateResult_t (*dds3ProcessFunc_t)(dds3ProcessID_t *task);
/* Called after linked children receive immediate kill requests and the task
 * leaves scheduler queues, while its allocation is still valid; the scheduler
 * frees it after the callback returns. */
typedef void (*dds3ProcessShutdownFunc_t)(dds3ProcessID_t *task);

struct dds3ProcessID_t {
    char name[0x18];          /* 0x00: NUL-padded task name, compared byte-wise */
    u32 namehash;             /* 0x18: sum of name's bytes */
    u32 flag;               /* 0x1C */
    u32 priority;            /* 0x20: ascending task queue priority. */
    u32 starttime;
    u32 runtime;      /* 0x28 */
    s16 starttimer;     /* 0x2C: signed; positive values count down, zero activates, negatives remain queued. */
    s16 shutdowntimer;   /* 0x2E: signed; positive values count down, zero destroys, negatives stay pending. */
    dds3ProcessFunc_t processfunc; /* 0x30 */
    dds3ProcessShutdownFunc_t shutdownfunc; /* 0x34 */
    void *workdata;         /* 0x38: caller-supplied value exposed by accessors. */
    dds3ProcessID_t *next;      /* 0x3C */
    dds3ProcessID_t *prev;      /* 0x40 */
    dds3ProcessID_t *parent;        /* 0x44 */
    dds3ProcessID_t *child;     /* 0x48 */
    dds3ProcessID_t *sibling;   /* 0x4C: next task in the parent's child list. */
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

/* Native administration state and dispatch row; HD owner: kernel.h. */
typedef struct dds3AdminiData_t
{
    u32 Stat;
    u32 RunTimer;
    s8 NowSequence;
    s8 NextSequence;
    u8 HistoryPointer;
    u8 unk0B;
    s8 SequenceHistory[8];
    u8 FlagHistory[8];
    void *CallData;
    u8 CallDataSize;
    u8 CallCount;
} dds3AdminiData_t;

#endif /* KERNEL_H */
