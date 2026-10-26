#ifndef SCR_H
#define SCR_H

#include "common.h"
#include "kwln.h"
#include "scr_trace_result.h"
#include "scr_command_result.h"
#include "itf_mes_window_index.h"

struct sdfMemHandle_t;
struct BfFlw0Header;
struct ItfMesSub;

typedef s32 ScrProcedureIndex;
#define SCR_PROCEDURE_INDEX_NOT_FOUND ((ScrProcedureIndex)-1)

typedef s32 ScrProgramCounter;
typedef s32 ScrGlobalIntegerIndex;
typedef s32 ScrGlobalFloatIndex;

/* One 4-byte script operand slot; the type tag determines whether a word is
 * a scalar, a global-register index, a return address, or a borrowed string. */
typedef union ScrStackValue {
    u32 bits; /* raw slot copy, preserving the separately stored type tag */
    s32 integer;
    f32 floating;
    ScrGlobalIntegerIndex globalIntegerIndex;
    ScrGlobalFloatIndex globalFloatIndex;
    ScrProgramCounter returnAddress;
    const char *string; /* borrowed read-only pointer into source FLW0 string section */
} ScrStackValue;

typedef s8 ScrStackValueType;

/* Four-byte decoded VM instruction; DDS1/2 script/scrTraceCode.c via scrProcessWork_s. */
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
    ScrProgramCounter addr; /* 0x18 */
    s32 unk1C;
} ScrLabel;

/* FLW0 section descriptor (0x10), including the file-relative data offset. */
typedef s32 BfFlw0SectionKind;
enum {
    BF_FLW0_SECTION_PROCEDURES = 0,
    BF_FLW0_SECTION_LABELS = 1,
    BF_FLW0_SECTION_INSTRUCTIONS = 2,
    BF_FLW0_SECTION_AUXILIARY = 3,
    BF_FLW0_SECTION_STRINGS = 4
};

typedef struct ScrSection {
    BfFlw0SectionKind type;
    s32 unk04;
    s32 count;
    s32 offset;
} ScrSection;

typedef char ScrSection_size_must_be_0x10[
    (sizeof(ScrSection) == 0x10) ? 1 : -1];

/* Native 0xF4-byte process allocated by bfContextCreate in both games.
 * The interpreter, scheduler attachment and named-process list share this owner. */
typedef struct scrProcessWork_s {
    char name[24];
    ScrProgramCounter pc;
    s32 sp;
    ScrStackValueType stackTypes[28];
    ScrStackValue stackValues[28];
    /* Borrowed FLW0 source views; the VM reads them but does not edit them. */
    const struct BfFlw0Header *scriptHeader;
    const ScrSection *sections;
    const ScrLabel *procedures;
    const ScrLabel *labels;
    const ScrInstr *instructions;
    struct ItfMesSub *auxiliaryData; /* mutable: message initialization relocates it */
    const char *strings;          /* borrowed read-only view; lifetime follows source owner/caller */
    ScrProcedureIndex procedureIndex;
    ItfMesWindowIndex messageWindowIndex;
    u32 timer;
    u32 cmdTimer;
    struct sdfMemHandle_t *scriptHandle; /* 0xD8: allocation descriptor, not scriptHeader data */
    s32 *localInt;
    f32 *localFloat;
    dds3ProcessID_t *task;       /* 0xE4: scheduler object returned by dds3StartProcess */
    struct scrProcessWork_s *previous; /* 0xE8: named-process list links */
    struct scrProcessWork_s *next;     /* 0xEC */
    void *userData;       /* 0xF0: opaque caller-owned script context, never freed here */
} scrProcessWork_s;

typedef char ScrData_size_must_be_0xF4[
    (sizeof(scrProcessWork_s) == 0xF4) ? 1 : -1];
typedef char ScrData_scriptHandle_at_0xD8[
    ((u32)&((scrProcessWork_s *)0)->scriptHandle == 0xD8) ? 1 : -1];
typedef char ScrData_messageWindowIndex_at_0xCC[
    ((u32)&((scrProcessWork_s *)0)->messageWindowIndex == 0xCC) ? 1 : -1];

extern scrProcessWork_s *nowpwork;
extern scrProcessWork_s *scrHandleFirst;
extern scrProcessWork_s *scrHandleLast;

scrProcessWork_s *scrGetScriptHandle(void);
scrProcessWork_s *scrSearchScriptName(const char *name);
void *scrGetData(void);
void *scrGetUserData(dds3ProcessID_t *task);
void scrSetUserData(dds3ProcessID_t *task, void *actor);
void scrScriptShutdown(dds3ProcessID_t *task);
void scrRunScript(scrProcessWork_s *context);
s32 scrScriptProcess(dds3ProcessID_t *task);
ScrTraceResult scrTraceCode(scrProcessWork_s *context);
void scrProcDestroyTask(scrProcessWork_s *process);
scrProcessWork_s *scrCreateTaskWithDefaultOption(const struct BfFlw0Header *header);
void PushInt(scrProcessWork_s *script, s32 value);
void PushFloat(scrProcessWork_s *script, f32 value);
void PushStr(scrProcessWork_s *script, const char *value);
void PushRet(scrProcessWork_s *script, ScrProgramCounter returnAddress);
s32 PopInt(scrProcessWork_s *script);
f32 PopFloat(scrProcessWork_s *script);
/* Command callback plus operand count (0x8); DDS1 script/scrTraceCode.c. */
typedef struct ScrCommand {
    ScrCommandResult (*func)(void);
    s32 paramCount;
} ScrCommand;

/* Script integer register 210 (DatGameState +0x388): set/clear by the process-control script commands. */
#define SCR_PROCESS_CONTROL_FLAG_REGISTER 210

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

/* Script VM stack value tags stored in ScrData.stackTypes. */
enum {
    SCR_STACK_TYPE_INTEGER = 0,
    SCR_STACK_TYPE_FLOAT = 1,
    SCR_STACK_TYPE_INTEGER_REFERENCE = 2,
    SCR_STACK_TYPE_FLOAT_REFERENCE = 3,
    SCR_STACK_TYPE_RETURN_ADDRESS = 4,
    SCR_STACK_TYPE_STRING = 5
};
#define SCR_STACK_RET 27

#endif /* SCR_H */
