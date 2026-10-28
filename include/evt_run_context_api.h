#ifndef EVT_RUN_CONTEXT_API_H
#define EVT_RUN_CONTEXT_API_H

#include "evt_script_context.h"
#include "scr_procedure_index.h"

/* Run one FLW0 procedure with the supplied event context words. */
ScrVmInteger evtRunContext(ScrProcedureIndex procedureIndex, s32 firstArgument,
    s32 secondArgument, s32 commandIndex, u16 options);

static inline EvtScriptArgument evtScriptArgumentFromBits(s32 bits) {
    EvtScriptArgument argument;
    argument.bits = bits;
    return argument;
}

static inline EvtScriptArgument evtScriptArgumentFromPointer(void *pointer) {
    EvtScriptArgument argument;
    argument.pointer = pointer;
    return argument;
}

/* Keep the retail entry point's two integer arguments at this typed boundary. */
static inline ScrVmInteger evtRunContextWithArguments(ScrProcedureIndex procedureIndex,
    EvtScriptArgument firstArgument, EvtScriptArgument secondArgument,
    s32 commandIndex, u16 options) {
    return evtRunContext(procedureIndex, firstArgument.bits, secondArgument.bits,
        commandIndex, options);
}

#endif /* EVT_RUN_CONTEXT_API_H */
