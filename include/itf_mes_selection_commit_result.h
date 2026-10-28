#ifndef ITF_MES_SELECTION_COMMIT_RESULT_H
#define ITF_MES_SELECTION_COMMIT_RESULT_H

/* A chooser stays interactive until confirm or an enabled option key commits
 * its current row. Cursor movement alone is not a committed selection. */
typedef enum ItfMesSelectionCommitResult {
    ITF_MES_SELECTION_NOT_COMMITTED = 0,
    ITF_MES_SELECTION_COMMITTED = 1,
} ItfMesSelectionCommitResult;

#endif /* ITF_MES_SELECTION_COMMIT_RESULT_H */
