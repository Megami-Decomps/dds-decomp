#ifndef FLD_ARCHIVE_LOAD_STATE_H
#define FLD_ARCHIVE_LOAD_STATE_H

#include "common.h"

typedef s32 FldArchiveLoadPhase;
enum {
    FLD_ARCHIVE_LOAD_PHASE_REPORTED = -1,
    FLD_ARCHIVE_LOAD_PHASE_CHECK = 0,
    FLD_ARCHIVE_LOAD_PHASE_WAIT = 1,
    FLD_ARCHIVE_LOAD_PHASE_RESIDENT = 2
};

typedef enum FldArchiveLoadResult {
    FLD_ARCHIVE_LOAD_PENDING = 0,
    FLD_ARCHIVE_LOAD_COMPLETE = 1
} FldArchiveLoadResult;

typedef enum FldArchivePendingState {
    FLD_ARCHIVE_PENDING_NONE = 0,
    FLD_ARCHIVE_PENDING_ACTIVE = 1
} FldArchivePendingState;

/* Advance the scene-BGM archive request until its packed track is resident. */
FldArchiveLoadResult fldStepSceneBgmArchive(void);
FldArchiveLoadResult fldPollArchiveLoad(s32 archiveId);

#endif
