#ifndef SCR_TRACE_RESULT_H
#define SCR_TRACE_RESULT_H

#include "common.h"

typedef u32 ScrOpcodeHandlerResult;

#define SCR_TRACE_RESULT_YIELDED 1
#define SCR_TRACE_RESULT_FINISHED 2

/* Opcode callback results consumed by scrTraceCode; meanings differ from the
 * outer trace results above. */
#define SCR_OPCODE_HANDLER_RESULT_FINISHED 0
#define SCR_OPCODE_HANDLER_RESULT_CONTINUE 1
#define SCR_OPCODE_HANDLER_RESULT_YIELDED 2

#endif
