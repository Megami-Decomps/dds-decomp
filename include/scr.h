#ifndef SCR_H
#define SCR_H

#include "common.h"
#include "kwln.h"

/* One 4-byte script operand slot; DDS1/2 script/scrTraceCode.c via ScrData. */
typedef union ScrStackValue {
    s32 i;
    f32 f;
    char *s;
} ScrStackValue;

/* Four-byte decoded VM instruction; DDS1/2 script/scrTraceCode.c via ScrData. */
typedef union ScrInstr {
    struct {
        s16 opCode;
        s16 sOperand;
    } parts;
    s32 iOperand;
    f32 fOperand;
} ScrInstr;

/* Named script label/procedure and address (0x20); DDS1/2 script/scrTraceCode.c. */
typedef struct ScrLabel {
    char name[24];
    s32 addr;              /* 0x18 */
    s32 unk1C;
} ScrLabel;

/* FLW0 section descriptor (0x10), including the file-relative data offset. */
typedef struct ScrSection {
    s32 type;
    s32 unk04;
    s32 count;
    s32 offset;
} ScrSection;

typedef char ScrSection_size_must_be_0x10[
    (sizeof(ScrSection) == 0x10) ? 1 : -1];

/* Native 0xF4-byte process allocated by bfContextCreate in both games.
 * The interpreter, scheduler attachment and named-process list share this owner. */
typedef struct ScrData {
    char name[24];
    s32 pc;
    s32 sp;
    s8 stackTypes[28];
    ScrStackValue stackValues[28];
    void *scriptHeader;
    ScrSection *sections;
    ScrLabel *procedures;
    ScrLabel *labels;
    ScrInstr *instructions;
    void *auxiliaryData;
    char *strings;
    s32 procedureIndex;
    s32 resourceIndex;
    s32 timer;
    s32 cmdTimer;
    void *scriptHandle;
    s32 *localInt;
    f32 *localFloat;
    KwlnTask *task;       /* 0xE4: scheduler object returned by kwlnTaskCreate */
    struct ScrData *previous; /* 0xE8: named-process list links */
    struct ScrData *next;     /* 0xEC */
    void *actor;          /* 0xF0: caller-owned command work */
} ScrData;

typedef char ScrData_size_must_be_0xF4[
    (sizeof(ScrData) == 0xF4) ? 1 : -1];

extern ScrData *scrCurrentContext;
extern ScrData *scrNamedProcessHead;
extern ScrData *scrNamedProcessTail;

ScrData *scrGetCurrentContext(void);
void *scrGetCurrentCommandWork(void);
void *scrGetCurrentActor(KwlnTask *task);
void scrSetCurrentActor(KwlnTask *task, void *actor);
void scrReplaceCurrentTask(KwlnTask *task);
void bfStepContext(ScrData *context);
s32 bfTaskUpdate(KwlnTask *task);
s32 bfContextStep(ScrData *context);
void scrProcDestroyTask(ScrData *process);
ScrData *bfContextCreate(void *header, ScrSection *sections, ScrLabel *procedures,
                        ScrLabel *labels, ScrInstr *instructions, void *auxiliaryData,
                        char *strings, s32 procedureIndex);
ScrData *scrCreateTaskWithDefaultOption(void *header);
void scrPushInteger(ScrData *script, s32 value);
void bfStackPushFloat(ScrData *script, f32 value);
void scrPushString(ScrData *script, char *value);
void scrPushTypeFourValue(ScrData *script, s32 value);
s32 bfStackPopInt(ScrData *script);
f32 bfStackPopFloat(ScrData *script);
KwlnTask *scrCreateTaskFromContextParameters(u32 priority, void *header,
    ScrSection *sections, ScrLabel *procedures, ScrLabel *labels,
    ScrInstr *instructions, void *auxiliaryData, char *strings, s32 procedureIndex);


/* Integer/float VM registers (0x840); DDS1/2 script/scrTraceCode.c. */
typedef struct ScrVM {
    u8 unk00[0x40];
    s32 ints[256];
    f32 floats[256];
} ScrVM;

/* Command callback plus operand count (0x8); DDS1 script/scrTraceCode.c. */
typedef struct ScrCommand {
    u32 (*func)(void);
    s32 paramCount;
} ScrCommand;

/* Shared script integer/float globals (0x840); DDS1/2 script/scrScriptProcess.c. */
typedef struct {
    u8 pad0[0x40];
    s32 integers[256];
    s32 floatBits[256];
} ScrProcGlobals;


/* Common-command global state (0x38C); DDS1/2 script/scrCommonCommand.c. */
typedef struct {
    u8 pad[0x388];
    s32 unk388;
} ScrComGlobals;

/* XYZ float plus integer W operand (0x10); DDS1/2 script/scrCommonCommand.c. */
typedef struct {
    f32 x;
    f32 y;
    f32 z;
    s32 w;
} ScrVecW;

/* Four-float vector operand (0x10); DDS1/2 script/scrCommonCommand.c. */
typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} ScrVec4;

/* ScrVmOperand is intentionally local: DDS1's +0x04 is u16, DDS2's is u8. */

/* Script VM stack value types. */
#define SCR_STACK_TYPE_STRING 5
#define SCR_STACK_RET 27

#endif /* SCR_H */
