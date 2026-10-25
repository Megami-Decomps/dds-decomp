#ifndef FLD_ARCHIVE_LOAD_STATE_H
#define FLD_ARCHIVE_LOAD_STATE_H

#define FLD_ARCHIVE_LOAD_PHASE_CHECK 0
#define FLD_ARCHIVE_LOAD_PHASE_WAIT 1
#define FLD_ARCHIVE_LOAD_PHASE_REPORTED -1

typedef enum FldArchiveLoadResult {
    FLD_ARCHIVE_LOAD_PENDING = 0,
    FLD_ARCHIVE_LOAD_COMPLETE = 1
} FldArchiveLoadResult;

typedef enum FldArchivePendingState {
    FLD_ARCHIVE_PENDING_NONE = 0,
    FLD_ARCHIVE_PENDING_ACTIVE = 1
} FldArchivePendingState;

#endif
