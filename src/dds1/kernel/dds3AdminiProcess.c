#include "common.h"
#include "dds3Admin.h"
#include "kwln.h"

extern char dds3AdminTaskName[];
extern void *kwlnTaskGetTaskByName(char *);
extern u32 kwlnTaskGetUserValue(KwlnTask *);
extern void sdfReleaseChipBlock(void *);
extern void *sdfAllocSizeClassBlock(s32);
extern void *memcpy(void *, void *, s32);

/* Return the named administration task's user state; the task must exist. */
AdminWork *dds3GetAdminTaskWork(void) {
    return (AdminWork *)kwlnTaskGetUserValue(kwlnTaskGetTaskByName(dds3AdminTaskName));
}

/* Read the administration state's shared value word, without modifying it. */
u32 dds3GetAdminTaskValue(void) {
    AdminWork *work;

    work = dds3GetAdminTaskWork();
    return work->value;
}

#define DDS3_ADMIN_REQUEST_PENDING_BIT 1
#define DDS3_ADMIN_KEEP_HISTORY_SLOT_BIT 8
#define DDS3_ADMIN_RESTORE_HISTORY_BIT 0x10000
#define DDS3_ADMIN_MARK_HISTORY_BIT 4
#define DDS3_ADMIN_REQUEST_DATA_MAX_BYTES 0x100
#define DDS3_ADMIN_REQUEST_DELAY 2

/* Request a mode and replace its attached data. NULL data is accepted regardless
 * of dataBytes; only oversized non-NULL data rejects the entire request.
 * Mode and stored byte count retain their byte truncation (256 bytes records zero).
 * Previous data is released before copying; the history flag marks the old slot
 * when the requested mode is activated. */
void dds3AdminSubmitModeRequest(s32 requestedMode, void *requestData, u32 dataBytes, s32 markHistory) {
    AdminWork *work;
    void *previousData;
    u32 flags;

    if (requestData == NULL || dataBytes <= DDS3_ADMIN_REQUEST_DATA_MAX_BYTES) {
        work = dds3GetAdminTaskWork();
        previousData = work->unk1C;
        work->unk09 = requestedMode;
        flags = work->flags;
        flags |= DDS3_ADMIN_REQUEST_PENDING_BIT;
        flags &= ~DDS3_ADMIN_KEEP_HISTORY_SLOT_BIT;
        flags &= ~DDS3_ADMIN_RESTORE_HISTORY_BIT;
        work->flags = flags;
        work->unk21 = DDS3_ADMIN_REQUEST_DELAY;
        if (previousData != NULL) {
            sdfReleaseChipBlock(previousData);
            work->unk1C = NULL;
            work->unk20 = 0;
        }
        if (requestData != NULL) {
            work->unk1C = sdfAllocSizeClassBlock(dataBytes);
            memcpy(work->unk1C, requestData, dataBytes);
            work->unk20 = dataBytes;
        } else {
            work->unk1C = NULL;
            work->unk20 = 0;
        }
        if (markHistory != 0) {
            work->flags |= DDS3_ADMIN_MARK_HISTORY_BIT;
        } else {
            work->flags &= ~DDS3_ADMIN_MARK_HISTORY_BIT;
        }
    }
}

/* One dispatch row per mode: three function pointers, 12 bytes each. The three
 * columns are consecutive symbols, ddsAdminModeCallbacks / D_003297D4 / D_003297D8. */
typedef struct AdminDispatch {
    void (*entry)(s32 restoring, void *data);
    s32 (*destroy)(void);
    s32 (*cleanup)(void);
} AdminDispatch;

extern AdminDispatch ddsAdminModeCallbacks[];
extern void *dds3AdminPollModeCompletion(void *task);

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

/* Activate a pending mode request and continue through the mode dispatcher. */
void *dds3AdminActivateRequestedMode(void *task) {
    AdminWork *work = (AdminWork *)kwlnTaskGetUserValue(task);
    u32 flags = work->flags;
    s32 restoring;
    void (*entry)(s32, void *);

    if ((flags & 1) != 0 && work->unk09 >= 0) {
        if (work->unk21 != 0) {
            work->unk21--;
            return NULL;
        }
        work->unk08 = work->unk09;
        work->unk09 = -1;
        work->flags &= ~1;
        work->flags &= ~2;
        work->value = 0;
        if (flags & 4) {
            work->unsignedHistory[work->historyIndex] |= 1;
            work->flags &= ~4;
        }
        if (work->flags & 0x10000) {
            work->flags &= ~0x10000;
            work->historyIndex = (work->historyIndex + 7) & 7;
            restoring = 1;
        } else {
            restoring = 0;
            if (work->flags & 8) {
                work->flags &= ~8;
            } else {
                work->historyIndex = (work->historyIndex + 1) & 7;
            }
        }
        work->signedHistory[work->historyIndex] = work->unk08;
        work->unsignedHistory[work->historyIndex] = 0;
        entry = ddsAdminModeCallbacks[work->unk08].entry;
        if (entry != NULL) {
            entry(restoring, work->unk1C);
        }
    }
    return dds3AdminPollModeCompletion;
}

/* Run the mode's destroy callback; a non-negative result is stored (+1) in unk21 and the mode
   cleared. Returns the next step function, or NULL if the callback failed. */
void *dds3AdminPollModeDestruction(void *task) {
    AdminWork *work = (AdminWork *)kwlnTaskGetUserValue(task);
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
    return dds3AdminActivateRequestedMode;
}

void *dds3AdminPollModeCompletion(void *task) {
    AdminWork *work = (AdminWork *)kwlnTaskGetUserValue(task);
    u32 flags = work->flags;
    s32 (*cleanup)(void);
    s32 previous;

    if ((flags & 2) == 0) {
        if (work->unk08 >= 0) {
            cleanup = ddsAdminModeCallbacks[work->unk08].cleanup;
            if (cleanup != NULL && cleanup() != 0) {
                work->flags |= 2;
            }
            work->value++;
        }
        flags = work->flags;
    }
    if ((flags & 2) && work->unk08 >= 0) {
        previous = (work->historyIndex + 7) & 7;
        if (work->signedHistory[previous] >= 0 && (work->unsignedHistory[previous] & 1)) {
            void *data = work->unk1C;

            work->flags |= 0x10001;
            work->unk09 = work->signedHistory[previous];
            work->unk21 = 2;
            work->flags &= ~4;
            if (data != NULL) {
                sdfReleaseChipBlock(data);
                work->unk1C = NULL;
                work->unk20 = 0;
            }
        }
        if ((work->flags & 1) == 0) {
            dds3AdminSubmitModeRequest(0, 0, 0, 0);
        }
        work->flags &= ~2;
    }
    if ((work->flags & 1) != 0 && work->unk09 >= 0) {
        return dds3AdminPollModeDestruction;
    }
    return NULL;
}

/* Run the mode's destroy callback (the row's second pointer), then free the
 * attached data block and the task itself. */
void dds3AdminReleaseTaskWork(void* task) {
    AdminWork* work = (AdminWork*)kwlnTaskGetUserValue(task);
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
    sdfReleaseChipBlock((void *)kwlnTaskGetUserValue(task));
}

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", dds3AdminTaskName);

