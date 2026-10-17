#ifndef FILE_JOB_TYPE_H
#define FILE_JOB_TYPE_H

#include "common.h"

/* A file job type's operations (0x28), fileJobTypeOperations[type]. Each
 * receives the job's data; a NULL slot is skipped where the caller checks. */
typedef struct FileTypeCallbacks {
    void *(*create)(void *request);                     /* 0x00 */
    void (*invoke)(void *data);                         /* 0x04 */
    void (*destroy)(void *data);                        /* 0x08 */
    void *(*createChild)(void *data, u16 type);         /* 0x0C */
    void (*notifyPair)(void *left, void *right);        /* 0x10 */
    void (*reset)(void *data);                          /* 0x14 */
    void (*setPosition)(void *data, f32 position[4]);   /* 0x18 */
    void (*setRotation)(void *data, f32 rotation[4]);   /* 0x1C */
    void (*setScale)(void *data, f32 scale);            /* 0x20 */
    void (*setColor)(void *data, u32 color);            /* 0x24 */
} FileTypeCallbacks;

typedef char FileTypeCallbacksSizeAssert[sizeof(FileTypeCallbacks) == 0x28 ? 1 : -1];
typedef char FileTypeCallbacksResetOffsetAssert[
    ((u32)&((FileTypeCallbacks *)0)->reset == 0x14) ? 1 : -1];

extern FileTypeCallbacks fileJobTypeOperations[];

#endif
