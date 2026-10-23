#ifndef FLD_RUNTIME_RESET_API_H
#define FLD_RUNTIME_RESET_API_H

/* The reset always clears area state; this selects whether map/save state also resets. */
typedef enum FldRuntimeResetMode {
    FLD_RUNTIME_RESET_CLEAR_MAP_STATE = 0,
    FLD_RUNTIME_RESET_PRESERVE_MAP_STATE = 1
} FldRuntimeResetMode;

void fldResetFieldRuntimeState(FldRuntimeResetMode mode);

#endif
