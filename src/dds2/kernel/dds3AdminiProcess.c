#include "sdk_ps2.h"
#include "common.h"
#include "dds3_admin_control_api.h"
#include "sdf_chip.h"
#include "dds3_task_runner.h"
#include "dds3_admin_dispatch.h"
#include "dds3_process_registration.h"
#include "dds3_named_slot.h"
#include "dds3_task_create.h"
#include "dds3_task_lifecycle.h"
#include "btl.h"

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
        flags |= DDS3_ADMIN_CONTROL_REQUEST_PENDING;
        flags &= ~DDS3_ADMIN_CONTROL_KEEP_CURRENT_HISTORY_SLOT;
        flags &= ~DDS3_ADMIN_CONTROL_RESTORE_PREVIOUS_MODE;
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
            work->Stat |= DDS3_ADMIN_CONTROL_MARK_HISTORY_SLOT_FOR_RESTORE;
        } else {
            work->Stat &= ~DDS3_ADMIN_CONTROL_MARK_HISTORY_SLOT_FOR_RESTORE;
        }
    }
}

extern dds3TaskUpdateResult_t dds3AdminiNeoProcess(dds3ProcessID_t *task);

/* Replace the pending mode request without advancing the history slot. */
void dds3AdminiOverwriteSequence(s32 requestedMode, const void *requestData, u32 dataBytes)
{
    dds3AdminiData_t* work;

    dds3AdminiCallSequence(requestedMode, requestData, dataBytes, 0);
    work = GetAdminiProcessData();
    work->Stat |= DDS3_ADMIN_CONTROL_KEEP_CURRENT_HISTORY_SLOT;
}

/* Request exit of the active administration mode. */
void dds3AdminiExitSequence(void)
{
    dds3AdminiData_t* work;

    work = GetAdminiProcessData();
    work->Stat |= DDS3_ADMIN_CONTROL_EXIT_REQUESTED;
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
dds3TaskUpdateResult_t dds3AdminiStartSequence(dds3ProcessID_t *task) {
    dds3AdminiData_t *work = (dds3AdminiData_t *)dds3GetProcessData(task);
    u32 flags = work->Stat;
    s32 isRestoringHistory;
    void (*modeEntryCallback)(s32, void *);

    if ((flags & DDS3_ADMIN_CONTROL_REQUEST_PENDING) != 0 && work->NextSequence >= 0) {
        if (work->CallCount != 0) {
            work->CallCount--;
            return DDS3_TASK_UPDATE_RETAIN_CALLBACK;
        }
        work->NowSequence = work->NextSequence;
        work->NextSequence = -1;
        work->Stat &= ~DDS3_ADMIN_CONTROL_REQUEST_PENDING;
        work->Stat &= ~DDS3_ADMIN_CONTROL_EXIT_REQUESTED;
        work->RunTimer = 0;
        if (flags & DDS3_ADMIN_CONTROL_MARK_HISTORY_SLOT_FOR_RESTORE) {
            work->FlagHistory[work->HistoryPointer] |= DDS3_ADMIN_HISTORY_FLAG_RESTORE_PREVIOUS_MODE;
            work->Stat &= ~DDS3_ADMIN_CONTROL_MARK_HISTORY_SLOT_FOR_RESTORE;
        }
        if (work->Stat & DDS3_ADMIN_CONTROL_RESTORE_PREVIOUS_MODE) {
            work->Stat &= ~DDS3_ADMIN_CONTROL_RESTORE_PREVIOUS_MODE;
            work->HistoryPointer = (work->HistoryPointer + 7) & 7;
            isRestoringHistory = 1;
        } else {
            isRestoringHistory = 0;
            if (work->Stat & DDS3_ADMIN_CONTROL_KEEP_CURRENT_HISTORY_SLOT) {
                work->Stat &= ~DDS3_ADMIN_CONTROL_KEEP_CURRENT_HISTORY_SLOT;
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
    return (dds3TaskUpdateResult_t)dds3AdminiNeoProcess;
}

/* Run the mode's destroy callback; a non-negative result is stored (+1) as
   the transition delay and the mode cleared. Return the next update address,
   or retain this callback when destruction reports failure. */
dds3TaskUpdateResult_t dds3AdminiEndCheck(dds3ProcessID_t *task) {
    dds3AdminiData_t *work = (dds3AdminiData_t *)dds3GetProcessData(task);
    s32 activeMode = work->NowSequence;
    s32 (*destroy)(void);
    s32 destroyResult;

    if (activeMode >= 0) {
        destroy = ddsAdminModeCallbacks[activeMode].end;
        if (destroy != NULL) {
            destroyResult = destroy();
            if (destroyResult < 0) {
                return DDS3_TASK_UPDATE_RETAIN_CALLBACK;
            }
            work->CallCount = destroyResult + 1;
            work->NowSequence = -1;
        }
    }
    return (dds3TaskUpdateResult_t)dds3AdminiStartSequence;
}

dds3TaskUpdateResult_t dds3AdminiNeoProcess(dds3ProcessID_t *task) {
    dds3AdminiData_t *work = (dds3AdminiData_t *)dds3GetProcessData(task);
    u32 flags = work->Stat;
    Dds3AdminModeCleanupResult (*cleanup)(void);
    s32 previousHistoryIndex;

    if ((flags & DDS3_ADMIN_CONTROL_EXIT_REQUESTED) == 0) {
        if (work->NowSequence >= 0) {
            cleanup = ddsAdminModeCallbacks[work->NowSequence].check;
            if (cleanup != NULL && cleanup() != DDS3_ADMIN_MODE_CLEANUP_CONTINUE) {
                work->Stat |= DDS3_ADMIN_CONTROL_EXIT_REQUESTED;
            }
            work->RunTimer++;
        }
        flags = work->Stat;
    }
    if ((flags & DDS3_ADMIN_CONTROL_EXIT_REQUESTED) && work->NowSequence >= 0) {
        previousHistoryIndex = (work->HistoryPointer + 7) & 7;
        if (work->SequenceHistory[previousHistoryIndex] >= 0 &&
            (work->FlagHistory[previousHistoryIndex] & DDS3_ADMIN_HISTORY_FLAG_RESTORE_PREVIOUS_MODE)) {
            void *requestData = work->CallData;

            work->Stat |= DDS3_ADMIN_CONTROL_REQUEST_PENDING |
                                  DDS3_ADMIN_CONTROL_RESTORE_PREVIOUS_MODE;
            work->NextSequence = work->SequenceHistory[previousHistoryIndex];
            work->CallCount = 2;
            work->Stat &= ~DDS3_ADMIN_CONTROL_MARK_HISTORY_SLOT_FOR_RESTORE;
            if (requestData != NULL) {
                sdfChipFreeDirect(requestData);
                work->CallData = NULL;
                work->CallDataSize = 0;
            }
        }
        if ((work->Stat & DDS3_ADMIN_CONTROL_REQUEST_PENDING) == 0) {
            dds3AdminiCallSequence(0, 0, 0, 0);
        }
        work->Stat &= ~DDS3_ADMIN_CONTROL_EXIT_REQUESTED;
    }
    if ((work->Stat & DDS3_ADMIN_CONTROL_REQUEST_PENDING) != 0 && work->NextSequence >= 0) {
        return (dds3TaskUpdateResult_t)dds3AdminiEndCheck;
    }
    return DDS3_TASK_UPDATE_RETAIN_CALLBACK;
}

/* Run the mode's destroy callback (the row's second pointer), then release the
 * attached data block and the task's AdminWork block. */
void dds3AdminiNeoShutdown(dds3ProcessID_t *task) {
    dds3AdminiData_t *work = (dds3AdminiData_t *)dds3GetProcessData(task);
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


extern dds3TaskUpdateResult_t dds3AdminiNeoProcess(dds3ProcessID_t *task);
extern void dds3AdminiNeoShutdown(dds3ProcessID_t *task);
extern char dds3AdminTaskName[];

void dds3InitAdmini(void) {
    dds3AdminiData_t *work;
    s32 i;

    if (dds3SearchProcessName(dds3AdminTaskName) == NULL) {
        work = sdfChipAllocDirect(0x24);
        work->Stat = 0;
        work->RunTimer = 0;
        work->NowSequence = -1;
        work->NextSequence = -1;
        work->HistoryPointer = 0;
        for (i = 0; i < 8; i++) {
            work->SequenceHistory[i] = -1;
            work->FlagHistory[i] = 0;
        }
        work->CallData = 0;
        work->CallDataSize = 0;
        dds3StartProcess(dds3AdminTaskName, 2, 0, 1, dds3AdminiNeoProcess, dds3AdminiNeoShutdown, (u32)work);
    }
}

extern fclNamedSlot_t D_0043D410[];
extern char D_00411198[];
extern s32 D_00435C20;
extern s32 D_00435C24;
extern s32 D_00435C28;
extern s32 D_00435C2C;

s32 fclEncycFreeEntry(const char *name);

s32 fclEncycFindFreeSlot(const char *name) {
    s32 slotIndex;
    s32 characterIndex;
    s32 previous;

    if (dds3SearchProcessName(D_00411198) == NULL) {
        return -1;
    }
    if (fclEncycFreeEntry(name) >= 0) {
        return -1;
    }
    for (slotIndex = 0; slotIndex < 32; slotIndex++) {
        if (D_0043D410[slotIndex].kind == 0) {
            break;
        }
    }
    if (slotIndex == 32) {
        return -1;
    }
    D_0043D410[slotIndex].kind = 0;
    characterIndex = 0;
    while ((D_0043D410[slotIndex].name[characterIndex] = name[characterIndex]) != '\0') {
        if (characterIndex == 31) {
            D_0043D410[slotIndex].name[characterIndex] = '\0';
            break;
        }
        characterIndex++;
    }
    D_0043D410[slotIndex].task = NULL;
    D_0043D410[slotIndex].update = NULL;
    D_0043D410[slotIndex].destroy = NULL;
    D_0043D410[slotIndex].setState = NULL;
    D_0043D410[slotIndex].enabled = 0;
    D_0043D410[slotIndex].userValue = NULL;
    D_0043D410[slotIndex].previous = -1;
    D_0043D410[slotIndex].next = -1;
    previous = D_00435C24;
    if (previous < 0) {
        D_00435C20 = slotIndex;
        D_00435C24 = slotIndex;
        D_00435C2C = slotIndex;
    } else {
        D_00435C24 = slotIndex;
        D_0043D410[previous].next = slotIndex;
        D_0043D410[slotIndex].previous = previous;
    }
    D_00435C28++;
    return slotIndex;
}

/* One shared initializer for the three configure modes and unregister. */
static inline void dds3SetNamedSlotState(s32 index, s32 kind, dds3ProcessFunc_t update, dds3ProcessShutdownFunc_t destroy,
                                         void (*setState)(s32), void *userValue) {
    D_0043D410[index].kind = kind;
    D_0043D410[index].task = NULL;
    D_0043D410[index].update = update;
    D_0043D410[index].destroy = destroy;
    D_0043D410[index].setState = setState;
    D_0043D410[index].enabled = 0;
    D_0043D410[index].userValue = userValue;
}

/* Find an exact NUL-terminated name along the linked slot indices, or return -1. */
s32 fclEncycFreeEntry(const char *name) {
    s32 slotIndex = D_00435C20;
    s32 characterIndex;

    while (slotIndex >= 0) {
        characterIndex = 0;
        while (D_0043D410[slotIndex].name[characterIndex] != '\0') {
            if (name[characterIndex] != D_0043D410[slotIndex].name[characterIndex]) {
                break;
            }
            characterIndex++;
        }
        if (D_0043D410[slotIndex].name[characterIndex] == '\0' && name[characterIndex] == '\0') {
            return slotIndex;
        }
        slotIndex = D_0043D410[slotIndex].next;
    }
    return -1;
}

/* Unregister a named slot: stop its task, clear it and unlink it from the slot chain. */
s32 fclEncycFreeEntries(const char *name) {
    s32 slotIndex;

    if (dds3SearchProcessName(D_00411198) == NULL) {
        return 0;
    }
    slotIndex = fclEncycFreeEntry(name);
    if (slotIndex < 0) {
        return 0;
    }
    if (D_0043D410[slotIndex].kind > 0) {
        if (D_0043D410[slotIndex].kind < 3) {
            if (dds3ExistProcess(D_0043D410[slotIndex].task) == DDS3_PROCESS_REGISTERED) {
                dds3KillProcess(D_0043D410[D_00435C2C].task, 1);
            }
        }
    }
    D_0043D410[slotIndex].name[0] = 0;
    dds3SetNamedSlotState(slotIndex, 0, NULL, NULL, NULL, 0);
    if (D_0043D410[slotIndex].previous < 0) {
        D_00435C20 = D_0043D410[slotIndex].next;
    } else {
        D_0043D410[D_0043D410[slotIndex].previous].next = D_0043D410[slotIndex].next;
    }
    if (D_0043D410[slotIndex].next < 0) {
        D_00435C24 = D_0043D410[slotIndex].previous;
    } else {
        D_0043D410[D_0043D410[slotIndex].next].previous = D_0043D410[slotIndex].previous;
    }
    D_0043D410[slotIndex].previous = -1;
    D_0043D410[slotIndex].next = -1;
    if (D_00435C2C == slotIndex) {
        D_00435C2C = D_00435C20;
    }
    D_00435C28--;
    return 1;
}

s32 fclEncycConfigureMode3(const char *name, void (*setState)(s32)) {
    s32 index = fclEncycFindFreeSlot(name);
    if (index < 0) {
        return 0;
    }
    dds3SetNamedSlotState(index, 3, NULL, NULL, setState, 0);
    return 1;
}

s32 fclEncycAllocEntry(const char *name, dds3ProcessFunc_t update, dds3ProcessShutdownFunc_t destroy, void *userValue) {
    s32 index = fclEncycFindFreeSlot(name);
    if (index < 0) {
        return 0;
    }
    dds3SetNamedSlotState(index, 1, update, destroy, NULL, userValue);
    return 1;
}

s32 fclEncycConfigureMode2(const char *name, dds3ProcessFunc_t update, dds3ProcessShutdownFunc_t destroy, void *userValue) {
    s32 index = fclEncycFindFreeSlot(name);
    if (index < 0) {
        return 0;
    }
    dds3SetNamedSlotState(index, 2, update, destroy, NULL, userValue);
    return 1;
}

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", dds3AdminTaskName);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", D_00435C20);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", D_00435C24);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", D_00435C28);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", D_00435C2C);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", D_00435C2F);

