#ifndef DDS_SCR_PROCEDURE_START_API_H
#define DDS_SCR_PROCEDURE_START_API_H

#include "scr.h"

/* A procedure start succeeds only when its table index is in range and the
 * process VM state has been reset to that procedure's entry point. */
typedef s32 ScrProcedureStartResult;
enum {
    SCR_PROCEDURE_START_INVALID_INDEX = 0,
    SCR_PROCEDURE_START_INITIALIZED = 1
};

ScrProcedureStartResult scrStartProcessProcedure(scrProcessWork_s *process,
    ScrProcedureIndex procedureIndex);

#endif /* DDS_SCR_PROCEDURE_START_API_H */
