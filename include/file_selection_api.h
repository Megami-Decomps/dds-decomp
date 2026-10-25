#ifndef FILE_SELECTION_API_H
#define FILE_SELECTION_API_H

#include "common.h"

typedef s32 (*FileSelectionCallback)(void);

/* The selected-file flow is idle until entering it sets the handoff pending. */
typedef u32 FileSelectionFlowState;
enum {
    FILE_SELECTION_FLOW_IDLE = 0,
    FILE_SELECTION_FLOW_PENDING = 1
};

FileSelectionFlowState fileGetSelectionPendingFlag(void);

s32 slQueryYesNo(FileSelectionCallback start, FileSelectionCallback finish, s32 mode);

#endif
