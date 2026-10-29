#ifndef EVT_SCRIPT_CONTEXT_H
#define EVT_SCRIPT_CONTEXT_H

#include "common.h"
#include "scr_vm_integer.h"

/* The host clears the result-written bit before each script run, preserving
 * every other context flag; handlers set it when they publish a return value. */
typedef u16 EvtScriptContextFlags;
#define EVT_SCRIPT_CONTEXT_RESULT_WRITTEN_FLAG 0x0001
#define EVT_SCRIPT_CONTEXT_RESULT_RESET_MASK 0xFFFE

/* The caller supplies these context words in integer form, while the data
 * calculation handlers consume them as borrowed pointers. */
typedef union EvtScriptArgument {
    s32 bits;
    void *pointer;
} EvtScriptArgument;

typedef char EvtScriptArgumentSizeCheck[(sizeof(EvtScriptArgument) == 4) ? 1 : -1];

typedef struct EvtScriptContext {
    EvtScriptContextFlags stateFlags;
    u16 opaque02;
    s32 commandIndex;
    EvtScriptArgument firstArgument;
    EvtScriptArgument secondArgument;
    ScrVmInteger returnValue;
    u16 options;
    u8 opaque16[2];
} EvtScriptContext;

typedef char EvtScriptContextSizeCheck[(sizeof(EvtScriptContext) == 0x18) ? 1 : -1];
typedef char EvtScriptContextCommandIndexOffsetCheck[
    ((u32)&((EvtScriptContext *)0)->commandIndex == 0x04) ? 1 : -1];
typedef char EvtScriptContextFirstArgumentOffsetCheck[
    ((u32)&((EvtScriptContext *)0)->firstArgument == 0x08) ? 1 : -1];
typedef char EvtScriptContextSecondArgumentOffsetCheck[
    ((u32)&((EvtScriptContext *)0)->secondArgument == 0x0C) ? 1 : -1];
typedef char EvtScriptContextReturnValueOffsetCheck[
    ((u32)&((EvtScriptContext *)0)->returnValue == 0x10) ? 1 : -1];
typedef char EvtScriptContextOptionsOffsetCheck[
    ((u32)&((EvtScriptContext *)0)->options == 0x14) ? 1 : -1];

#endif
