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

/* Zero means no successful save/load result has been recorded. */
typedef u32 FileMenuOperationResult;
enum {
    FILE_MENU_OPERATION_NO_RESULT = 0,
    FILE_MENU_OPERATION_SAVE_SUCCEEDED = 1,
    FILE_MENU_OPERATION_LOAD_SUCCEEDED = 2
};

FileMenuOperationResult fileGetLoadSelectionState(void);

s32 slQueryYesNo(FileSelectionCallback start, FileSelectionCallback finish, s32 mode);

#endif
