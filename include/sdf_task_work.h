#ifndef SDF_TASK_WORK_H
#define SDF_TASK_WORK_H

#include "common.h"

struct SdfMemBlock;
struct SdfList;
struct SdfListNode;

/* Caller-supplied description copied into one scheduled task entry (0x14). */
typedef struct SdfTaskItemDesc {
    s32 key;                       /* 0x00 */
    s32 (*init)(void);             /* 0x04 */
    void (*destroy)(s32, s32);     /* 0x08 */
    s32 (*update)(s32, s32);       /* 0x0C */
    void (*callback)(s32, s32);    /* 0x10 */
} SdfTaskItemDesc;

typedef char SdfTaskItemDesc_size_must_be_0x14[
    (sizeof(SdfTaskItemDesc) == 0x14 &&
     (u32)&((SdfTaskItemDesc *)0)->key == 0x00 &&
     (u32)&((SdfTaskItemDesc *)0)->init == 0x04 &&
     (u32)&((SdfTaskItemDesc *)0)->destroy == 0x08 &&
     (u32)&((SdfTaskItemDesc *)0)->update == 0x0C &&
     (u32)&((SdfTaskItemDesc *)0)->callback == 0x10)
        ? 1 : -1];

/* Allocated entry stored as a task worker list-node value (0x1C). */
typedef struct SdfTaskEntry {
    u32 flags;                     /* 0x00 */
    s32 key;                       /* 0x04 */
    s32 (*init)(void);             /* 0x08 */
    void (*destroy)(s32, s32);     /* 0x0C */
    s32 (*update)(s32, s32);       /* 0x10 */
    void (*callback)(s32, s32);    /* 0x14 */
    s32 initResult;                /* 0x18 */
} SdfTaskEntry;

typedef char SdfTaskEntry_size_must_be_0x1C[
    (sizeof(SdfTaskEntry) == 0x1C &&
     (u32)&((SdfTaskEntry *)0)->flags == 0x00 &&
     (u32)&((SdfTaskEntry *)0)->key == 0x04 &&
     (u32)&((SdfTaskEntry *)0)->init == 0x08 &&
     (u32)&((SdfTaskEntry *)0)->destroy == 0x0C &&
     (u32)&((SdfTaskEntry *)0)->update == 0x10 &&
     (u32)&((SdfTaskEntry *)0)->callback == 0x14 &&
     (u32)&((SdfTaskEntry *)0)->initResult == 0x18)
        ? 1 : -1];

/* Backing owner passed through the Kwln task's user-value word (0x14). */
typedef struct TaskWork {
    struct SdfMemBlock *allocation; /* 0x00 */
    char *primaryTaskName;          /* 0x04 */
    char *secondaryTaskName;        /* 0x08 */
    struct SdfList *list;           /* 0x0C */
    struct SdfListNode *currentNode;/* 0x10: next entry visited by the active pass. */
} TaskWork;

typedef char TaskWork_layout_must_match_native_owner[
    (sizeof(TaskWork) == 0x14 &&
     (u32)&((TaskWork *)0)->allocation == 0x00 &&
     (u32)&((TaskWork *)0)->primaryTaskName == 0x04 &&
     (u32)&((TaskWork *)0)->secondaryTaskName == 0x08 &&
     (u32)&((TaskWork *)0)->list == 0x0C &&
     (u32)&((TaskWork *)0)->currentNode == 0x10)
        ? 1 : -1];

/* This legacy factory deliberately accepts an opaque callback function type. */
TaskWork *sdfCreateTaskWorker(char *, s32, s32, SdfTaskItemDesc *, void (*)(), void *);
TaskWork *sdfCreateNamedTaskWork(char *, void (*)(), void *);
void sdfDestroyTaskResourceWork(TaskWork *);
void sdfDestroyTaskWorkerTasks(TaskWork *);
void sdfAttachTaskItem(TaskWork *, SdfTaskItemDesc *);
void sdfRemoveTaskItem(TaskWork *, s32);
SdfTaskEntry *sdfFindTaskItemValueByKey(TaskWork *, s32);
void sdfSetTaskItemMode(TaskWork *, s32, u32);

#endif /* SDF_TASK_WORK_H */
