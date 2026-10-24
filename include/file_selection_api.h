#ifndef FILE_SELECTION_API_H
#define FILE_SELECTION_API_H

#include "common.h"

typedef s32 (*FileSelectionCallback)(void);

/* Returns the pending selected-file-flow flag. */
u32 fileGetSelectionPendingFlag(void);

s32 slQueryYesNo(FileSelectionCallback start, FileSelectionCallback finish, s32 mode);

#endif
