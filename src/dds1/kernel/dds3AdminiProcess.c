#include "common.h"
#include "dds3Admin.h"

extern AdminWork* dds3GetAdminTaskWork(void);
extern void dds3AdminSubmitModeRequest(s32 a0, s32 a1, s32 a2, s32 a3);
extern void* kwlnTaskGetUserValue(void* task);
extern void kwlnTaskSetUserValue(void* task, u32 value);
extern void* func_002CFEB8(s32 a0);
extern void sdfReleaseChipBlock(void* ptr);
extern void* memcpy(void* dst, void* src, s32 n);
/* One dispatch row per mode: three function pointers, 12 bytes each. The three
 * columns are consecutive symbols, ddsAdminModeCallbacks / D_003297D4 / D_003297D8. */
typedef struct AdminDispatch {
    void (*entry)(void);
    s32 (*destroy)(void);
    void (*cleanup)(void);
} AdminDispatch;

extern AdminDispatch ddsAdminModeCallbacks[];
extern u8 dds3AdminTaskName[];

/* Configure administrative state from three caller-supplied parameters. */
void dds3AdminSubmitMarkedRequest(s32 value, void *data, u32 size)
{
    AdminWork* work;

    dds3AdminSubmitModeRequest(value, data, size, 0);
    work = dds3GetAdminTaskWork();
    work->flags |= 8;
}

/* Mark the admin state with its second independent control flag. */
void dds3AdminSetControlFlag(void)
{
    AdminWork* work;

    work = dds3GetAdminTaskWork();
    work->flags |= 2;
}

s8 dds3AdminGetActiveMode(void)
{
    return dds3GetAdminTaskWork()->unk08;
}

s8 dds3AdminGetRequestedMode(void)
{
    return dds3GetAdminTaskWork()->unk09;
}

/* Read the signed sample immediately before the ring buffer's write index. */
s8 dds3AdminReadPreviousSignedSample(void)
{
    AdminWork* work;

    work = dds3GetAdminTaskWork();
    return work->signedHistory[(work->historyIndex + 7) & 7];
}

/* Read the corresponding unsigned sample from the previous ring slot. */
u8 dds3AdminReadPreviousUnsignedSample(void)
{
    AdminWork* work;

    work = dds3GetAdminTaskWork();
    return work->unsignedHistory[(work->historyIndex + 7) & 7];
}

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", func_00102AE0);

typedef void (*AdminStep)(void);

extern void func_00102AE0(void);

/* Run the mode's destroy callback; a non-negative result is stored (+1) in unk21 and the mode
   cleared. Returns the next step function, or NULL if the callback failed. */
AdminStep dds3AdminPollModeDestruction(void *task) {
    AdminWork *work = kwlnTaskGetUserValue(task);
    s32 mode = work->unk08;
    s32 (*destroy)(void);
    s32 result;

    if (mode >= 0) {
        destroy = ddsAdminModeCallbacks[mode].destroy;
        if (destroy != NULL) {
            result = destroy();
            if (result < 0) {
                return NULL;
            }
            work->unk21 = result + 1;
            work->unk08 = -1;
        }
    }
    return func_00102AE0;
}

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", func_00102CD8);

/* Run the mode's destroy callback (the row's second pointer), then free the
 * attached data block and the task itself. */
void dds3AdminReleaseTaskWork(void* task) {
    AdminWork* work = kwlnTaskGetUserValue(task);
    s32 mode = work->unk08;

    if (mode >= 0) {
        void (*destroy)(void) = ddsAdminModeCallbacks[mode].destroy;

        if (destroy != NULL) {
            destroy();
        }
    }
    if (work->unk1C != NULL) {
        sdfReleaseChipBlock(work->unk1C);
    }
    sdfReleaseChipBlock(kwlnTaskGetUserValue(task));
}
