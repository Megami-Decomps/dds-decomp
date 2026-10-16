#ifndef EVT_SCRIPT_CONTEXT_H
#define EVT_SCRIPT_CONTEXT_H

#include "common.h"

typedef struct EvtScriptContext {
    u16 stateFlags;
    u16 opaque02;
    s32 commandIndex;
    s32 firstArgument;
    s32 secondArgument;
    s32 returnValue;
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
