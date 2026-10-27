#ifndef EVT_RUN_CONTEXT_API_H
#define EVT_RUN_CONTEXT_API_H

#include "scr_procedure_index.h"

/* Run one FLW0 procedure with the supplied event context arguments. */
s32 evtRunContext(ScrProcedureIndex procedureIndex, s32 firstArgument,
    s32 secondArgument, s32 commandIndex, u16 options);

#endif /* EVT_RUN_CONTEXT_API_H */
