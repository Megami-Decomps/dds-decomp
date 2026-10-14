#include "common.h"
#include "dds3_admin_control_api.h"
#include "sdf_chip.h"
#include "dds3Admin.h"
#include "kwln.h"
#include "dds3_admin_dispatch.h"

extern char dds3AdminTaskName[];
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
        previousData = work->requestData;
        work->requestedMode = requestedMode;
        flags = work->flags;
        flags |= DDS3_ADMIN_REQUEST_PENDING_BIT;
        flags &= ~DDS3_ADMIN_KEEP_HISTORY_SLOT_BIT;
        flags &= ~DDS3_ADMIN_RESTORE_HISTORY_BIT;
        work->flags = flags;
        work->modeTransitionDelay = DDS3_ADMIN_REQUEST_DELAY;
        if (previousData != NULL) {
            sdfReleaseChipBlock(previousData);
            work->requestData = NULL;
            work->requestSizeLowByte = 0;
        }
        if (requestData != NULL) {
            work->requestData = sdfAllocSizeClassBlock(dataBytes);
            memcpy(work->requestData, requestData, dataBytes);
            work->requestSizeLowByte = dataBytes;
        } else {
            work->requestData = NULL;
            work->requestSizeLowByte = 0;
        }
        if (markHistory != 0) {
            work->flags |= DDS3_ADMIN_MARK_HISTORY_BIT;
        } else {
            work->flags &= ~DDS3_ADMIN_MARK_HISTORY_BIT;
        }
    }
}

extern s32 dds3AdminPollModeCompletion(KwlnTask *task);

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
    return dds3GetAdminTaskWork()->activeMode;
}

s8 dds3AdminGetRequestedMode(void)
{
    return dds3GetAdminTaskWork()->requestedMode;
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
s32 dds3AdminActivateRequestedMode(KwlnTask *task) {
    AdminWork *work = (AdminWork *)kwlnTaskGetUserValue(task);
    u32 flags = work->flags;
    s32 restoring;
    void (*entry)(s32, void *);

    if ((flags & 1) != 0 && work->requestedMode >= 0) {
        if (work->modeTransitionDelay != 0) {
            work->modeTransitionDelay--;
            return 0;
        }
        work->activeMode = work->requestedMode;
        work->requestedMode = -1;
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
        work->signedHistory[work->historyIndex] = work->activeMode;
        work->unsignedHistory[work->historyIndex] = 0;
        entry = ddsAdminModeCallbacks[work->activeMode].entry;
        if (entry != NULL) {
            entry(restoring, work->requestData);
        }
    }
    return (s32)dds3AdminPollModeCompletion;
}

/* Run the mode's destroy callback; a non-negative result is stored (+1) as
   the transition delay and the mode cleared. Return the next update address
   as a signed scheduler control word, or zero on failure. */
s32 dds3AdminPollModeDestruction(KwlnTask *task) {
    AdminWork *work = (AdminWork *)kwlnTaskGetUserValue(task);
    s32 mode = work->activeMode;
    s32 (*destroy)(void);
    s32 result;

    if (mode >= 0) {
        destroy = ddsAdminModeCallbacks[mode].destroy;
        if (destroy != NULL) {
            result = destroy();
            if (result < 0) {
                return 0;
            }
            work->modeTransitionDelay = result + 1;
            work->activeMode = -1;
        }
    }
    return (s32)dds3AdminActivateRequestedMode;
}

s32 dds3AdminPollModeCompletion(KwlnTask *task) {
    AdminWork *work = (AdminWork *)kwlnTaskGetUserValue(task);
    u32 flags = work->flags;
    s32 (*cleanup)(void);
    s32 previous;

    if ((flags & 2) == 0) {
        if (work->activeMode >= 0) {
            cleanup = ddsAdminModeCallbacks[work->activeMode].cleanup;
            if (cleanup != NULL && cleanup() != 0) {
                work->flags |= 2;
            }
            work->value++;
        }
        flags = work->flags;
    }
    if ((flags & 2) && work->activeMode >= 0) {
        previous = (work->historyIndex + 7) & 7;
        if (work->signedHistory[previous] >= 0 && (work->unsignedHistory[previous] & 1)) {
            void *data = work->requestData;

            work->flags |= 0x10001;
            work->requestedMode = work->signedHistory[previous];
            work->modeTransitionDelay = 2;
            work->flags &= ~4;
            if (data != NULL) {
                sdfReleaseChipBlock(data);
                work->requestData = NULL;
                work->requestSizeLowByte = 0;
            }
        }
        if ((work->flags & 1) == 0) {
            dds3AdminSubmitModeRequest(0, 0, 0, 0);
        }
        work->flags &= ~2;
    }
    if ((work->flags & 1) != 0 && work->requestedMode >= 0) {
        return (s32)dds3AdminPollModeDestruction;
    }
    return 0;
}

/* Run the mode's destroy callback (the row's second pointer), then release the
 * attached data block and the task's AdminWork block. */
void dds3AdminReleaseTaskWork(KwlnTask *task) {
    AdminWork* work = (AdminWork*)kwlnTaskGetUserValue(task);
    s32 mode = work->activeMode;

    if (mode >= 0) {
        s32 (*destroy)(void) = ddsAdminModeCallbacks[mode].destroy;

        if (destroy != NULL) {
            destroy();
        }
    }
    if (work->requestData != NULL) {
        sdfReleaseChipBlock(work->requestData);
    }
    sdfReleaseChipBlock((void *)kwlnTaskGetUserValue(task));
}

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", dds3AdminTaskName);

