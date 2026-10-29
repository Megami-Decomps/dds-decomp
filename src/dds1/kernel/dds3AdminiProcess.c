#include "sdk_ps2.h"
#include "common.h"
#include "dds3_admin_control_api.h"
#include "sdf_chip.h"
#include "dds3Admin.h"
#include "kwln.h"
#include "kwln_task_runner.h"
#include "dds3_admin_dispatch.h"

extern char dds3AdminTaskName[];

/* Return the named administration task's user state; the task must exist. */
dds3AdminiData_t *GetAdminiProcessData(void) {
    return (dds3AdminiData_t *)dds3GetProcessData(dds3SearchProcessName(dds3AdminTaskName));
}

/* Read the administration state's shared value word, without modifying it. */
u32 dds3GetAdminTaskValue(void) {
    dds3AdminiData_t *work;

    work = GetAdminiProcessData();
    return work->RunTimer;
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
void dds3AdminiCallSequence(s32 requestedMode, const void *requestData, u32 dataBytes, s32 markHistory) {
    dds3AdminiData_t *work;
    void *previousRequestData;
    u32 flags;

    if (requestData == NULL || dataBytes <= DDS3_ADMIN_REQUEST_DATA_MAX_BYTES) {
        work = GetAdminiProcessData();
        previousRequestData = work->CallData;
        work->NextSequence = requestedMode;
        flags = work->Stat;
        flags |= DDS3_ADMIN_REQUEST_PENDING_BIT;
        flags &= ~DDS3_ADMIN_KEEP_HISTORY_SLOT_BIT;
        flags &= ~DDS3_ADMIN_RESTORE_HISTORY_BIT;
        work->Stat = flags;
        work->CallCount = DDS3_ADMIN_REQUEST_DELAY;
        if (previousRequestData != NULL) {
            sdfChipFreeDirect(previousRequestData);
            work->CallData = NULL;
            work->CallDataSize = 0;
        }
        if (requestData != NULL) {
            work->CallData = sdfChipAllocDirect(dataBytes);
            memcpy(work->CallData, requestData, dataBytes);
            work->CallDataSize = dataBytes;
        } else {
            work->CallData = NULL;
            work->CallDataSize = 0;
        }
        if (markHistory != 0) {
            work->Stat |= DDS3_ADMIN_MARK_HISTORY_BIT;
        } else {
            work->Stat &= ~DDS3_ADMIN_MARK_HISTORY_BIT;
        }
    }
}

extern KwlnTaskUpdateResult dds3AdminiNeoProcess(dds3ProcessID_t *task);

/* Configure administrative state from three caller-supplied parameters. */
void dds3AdminiOverwriteSequence(s32 requestedMode, const void *requestData, u32 dataBytes)
{
    dds3AdminiData_t* work;

    dds3AdminiCallSequence(requestedMode, requestData, dataBytes, 0);
    work = GetAdminiProcessData();
    work->Stat |= 8;
}

/* Mark the admin state with its second independent control flag. */
void dds3AdminiExitSequence(void)
{
    dds3AdminiData_t* work;

    work = GetAdminiProcessData();
    work->Stat |= 2;
}

s8 dds3AdminiGetActiveSequence(void)
{
    return GetAdminiProcessData()->NowSequence;
}

s8 dds3AdminiGetNextSequence(void)
{
    return GetAdminiProcessData()->NextSequence;
}

/* Read the signed sample immediately before the ring buffer's write index. */
s8 dds3AdminiGetPrevSequence(void)
{
    dds3AdminiData_t* work;

    work = GetAdminiProcessData();
    return work->SequenceHistory[(work->HistoryPointer + 7) & 7];
}

/* Read the flags associated with the previous mode-history slot. */
Dds3AdminHistoryFlags dds3AdminiGetPrevHistoryFlag(void)
{
    dds3AdminiData_t* work;

    work = GetAdminiProcessData();
    return work->FlagHistory[(work->HistoryPointer + 7) & 7];
}

/* Activate a pending mode request and continue through the mode dispatcher. */
KwlnTaskUpdateResult dds3AdminiStartSequence(dds3ProcessID_t *task) {
    dds3AdminiData_t *work = (dds3AdminiData_t *)dds3GetProcessData(task);
    u32 flags = work->Stat;
    s32 isRestoringHistory;
    void (*modeEntryCallback)(s32, void *);

    if ((flags & 1) != 0 && work->NextSequence >= 0) {
        if (work->CallCount != 0) {
            work->CallCount--;
            return KWLN_TASK_UPDATE_RETAIN_CALLBACK;
        }
        work->NowSequence = work->NextSequence;
        work->NextSequence = -1;
        work->Stat &= ~1;
        work->Stat &= ~2;
        work->RunTimer = 0;
        if (flags & 4) {
            work->FlagHistory[work->HistoryPointer] |= DDS3_ADMIN_HISTORY_FLAG_RESTORE_PREVIOUS_MODE;
            work->Stat &= ~4;
        }
        if (work->Stat & 0x10000) {
            work->Stat &= ~0x10000;
            work->HistoryPointer = (work->HistoryPointer + 7) & 7;
            isRestoringHistory = 1;
        } else {
            isRestoringHistory = 0;
            if (work->Stat & 8) {
                work->Stat &= ~8;
            } else {
                work->HistoryPointer = (work->HistoryPointer + 1) & 7;
            }
        }
        work->SequenceHistory[work->HistoryPointer] = work->NowSequence;
        work->FlagHistory[work->HistoryPointer] = 0;
        modeEntryCallback = ddsAdminModeCallbacks[work->NowSequence].start;
        if (modeEntryCallback != NULL) {
            modeEntryCallback(isRestoringHistory, work->CallData);
        }
    }
    return (KwlnTaskUpdateResult)dds3AdminiNeoProcess;
}

/* Run the mode's destroy callback; a non-negative result is stored (+1) as
   the transition delay and the mode cleared. Return the next update address,
   or retain this callback when destruction reports failure. */
KwlnTaskUpdateResult dds3AdminiEndCheck(dds3ProcessID_t *task) {
    dds3AdminiData_t *work = (dds3AdminiData_t *)dds3GetProcessData(task);
    s32 activeMode = work->NowSequence;
    s32 (*destroy)(void);
    s32 destroyResult;

    if (activeMode >= 0) {
        destroy = ddsAdminModeCallbacks[activeMode].end;
        if (destroy != NULL) {
            destroyResult = destroy();
            if (destroyResult < 0) {
                return KWLN_TASK_UPDATE_RETAIN_CALLBACK;
            }
            work->CallCount = destroyResult + 1;
            work->NowSequence = -1;
        }
    }
    return (KwlnTaskUpdateResult)dds3AdminiStartSequence;
}

KwlnTaskUpdateResult dds3AdminiNeoProcess(dds3ProcessID_t *task) {
    dds3AdminiData_t *work = (dds3AdminiData_t *)dds3GetProcessData(task);
    u32 flags = work->Stat;
    Dds3AdminModeCleanupResult (*cleanup)(void);
    s32 previousHistoryIndex;

    if ((flags & 2) == 0) {
        if (work->NowSequence >= 0) {
            cleanup = ddsAdminModeCallbacks[work->NowSequence].check;
            if (cleanup != NULL && cleanup() != DDS3_ADMIN_MODE_CLEANUP_CONTINUE) {
                work->Stat |= 2;
            }
            work->RunTimer++;
        }
        flags = work->Stat;
    }
    if ((flags & 2) && work->NowSequence >= 0) {
        previousHistoryIndex = (work->HistoryPointer + 7) & 7;
        if (work->SequenceHistory[previousHistoryIndex] >= 0 &&
            (work->FlagHistory[previousHistoryIndex] & DDS3_ADMIN_HISTORY_FLAG_RESTORE_PREVIOUS_MODE)) {
            void *requestData = work->CallData;

            work->Stat |= 0x10001;
            work->NextSequence = work->SequenceHistory[previousHistoryIndex];
            work->CallCount = 2;
            work->Stat &= ~4;
            if (requestData != NULL) {
                sdfChipFreeDirect(requestData);
                work->CallData = NULL;
                work->CallDataSize = 0;
            }
        }
        if ((work->Stat & 1) == 0) {
            dds3AdminiCallSequence(0, 0, 0, 0);
        }
        work->Stat &= ~2;
    }
    if ((work->Stat & 1) != 0 && work->NextSequence >= 0) {
        return (KwlnTaskUpdateResult)dds3AdminiEndCheck;
    }
    return KWLN_TASK_UPDATE_RETAIN_CALLBACK;
}

/* Run the mode's destroy callback (the row's second pointer), then release the
 * attached data block and the task's AdminWork block. */
void dds3AdminiNeoShutdown(dds3ProcessID_t *task) {
    dds3AdminiData_t* work = (dds3AdminiData_t*)dds3GetProcessData(task);
    s32 activeMode = work->NowSequence;

    if (activeMode >= 0) {
        s32 (*destroy)(void) = ddsAdminModeCallbacks[activeMode].end;

        if (destroy != NULL) {
            destroy();
        }
    }
    if (work->CallData != NULL) {
        sdfChipFreeDirect(work->CallData);
    }
    sdfChipFreeDirect((void *)dds3GetProcessData(task));
}

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", dds3AdminTaskName);
