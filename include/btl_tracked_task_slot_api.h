#ifndef BTL_TRACKED_TASK_SLOT_API_H
#define BTL_TRACKED_TASK_SLOT_API_H

#include "common.h"

struct dds3ProcessID_t;

typedef s32 BtlTrackedProcessSlot;
typedef s32 BtlTrackedResourceFlagSlot;

enum {
    BTL_TRACKED_PROCESS_APH_NAME_PANEL = 0,
    BTL_TRACKED_PROCESS_SKILL_NAME_PANEL = 1,
    BTL_TRACKED_RESOURCE_SELECTION_BUFFER = 2,
    BTL_TRACKED_PROCESS_ACTOR_PANEL = 3,
    BTL_TRACKED_RESOURCE_COMMAND_HINT_WINDOW = 4,
    BTL_TRACKED_PROCESS_SELECTION_TRAIL = 5,
    BTL_TRACKED_PROCESS_PHASE_CHANGE_PANEL = 6,
    BTL_TRACKED_PROCESS_REGISTERED_ACTOR_PANEL_CHILD = 7,
    BTL_TRACKED_PROCESS_RESERVE_ACTOR_PANEL = 8,
    BTL_TRACKED_PROCESS_GUIDE_PANEL = 9,
    BTL_TRACKED_PROCESS_MAHEN_PANEL = 10,
    BTL_TRACKED_PROCESS_ANALYSIS_PANEL = 11,
    BTL_TRACKED_PROCESS_LINKAGE_NOTICE = 12,
    BTL_TRACKED_PROCESS_TUTORIAL_COMMAND_NOTICE = 13
};

/* These fixed-address functions read and write raw words in the mixed slots. */
s32 btlGetTrackedTaskHandle(s32 slot);
void btlSetTrackedTaskHandle(s32 slot, s32 value);

/* Process and flag helpers preserve the raw accessor ABI without leaking casts. */
static inline struct dds3ProcessID_t *btlGetTrackedProcess(BtlTrackedProcessSlot slot) {
    return (struct dds3ProcessID_t *)(u32)btlGetTrackedTaskHandle(slot);
}

static inline void btlSetTrackedProcess(BtlTrackedProcessSlot slot, struct dds3ProcessID_t *process) {
    btlSetTrackedTaskHandle(slot, (s32)(u32)process);
}

/* The legacy indexed accessors store these flags in the same word slots. */
static inline s32 btlGetTrackedResourceFlag(BtlTrackedResourceFlagSlot slot) {
    return btlGetTrackedTaskHandle(slot);
}

static inline void btlSetTrackedResourceFlag(BtlTrackedResourceFlagSlot slot, s32 active) {
    btlSetTrackedTaskHandle(slot, active);
}

#endif /* BTL_TRACKED_TASK_SLOT_API_H */
