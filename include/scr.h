#ifndef SCR_H
#define SCR_H

#include "common.h"
#include "kwln.h"
#include "scr_trace_result.h"
#include "scr_command_result.h"
#include "scr_command_index.h"
#include "scr_procedure_index.h"
#include "scr_command_parameter_index.h"
#include "scr_command_parameter_count.h"
#include "scr_command_yield_count.h"
#include "scr_vm_integer.h"
#include "itf_mes_window_index.h"

struct sdfMemHandle_t;
struct BfFlw0Header;
struct ItfMesSub;

typedef s32 ScrProgramCounter;
typedef s32 ScrGlobalIntegerIndex;
typedef s32 ScrGlobalFloatIndex;
/* Zero-based signed halfword selectors for the VM's per-process local arrays. */
typedef s16 ScrLocalIntegerIndex;
typedef s16 ScrLocalFloatIndex;
/* Signed halfword slot counts stored in the FLW0 header. */
typedef s16 ScrLocalIntegerCount;
typedef s16 ScrLocalFloatCount;
typedef s32 ScrStackDepth;
typedef s32 ScrStackValueIndex;

#define SCR_STACK_STORAGE_SLOT_COUNT 28
#define SCR_STACK_RETURN_VALUE_SLOT 27

/* Script instruction handler table indices (35 entries in codefunc[]). */
typedef s16 ScrOpcode;
enum {
    SCR_OPCODE_PUSH_INTEGER = 0,
    SCR_OPCODE_PUSH_FLOAT = 1,
    SCR_OPCODE_PUSH_GLOBAL_INTEGER = 2,
    SCR_OPCODE_PUSH_GLOBAL_FLOAT = 3,
    SCR_OPCODE_PUSH_RETURN_VALUE = 4,
    SCR_OPCODE_POP_GLOBAL_INTEGER = 5,
    SCR_OPCODE_POP_GLOBAL_FLOAT = 6,
    SCR_OPCODE_PROCEDURE = 7,
    SCR_OPCODE_COMMAND = 8,
    SCR_OPCODE_END = 9,
    SCR_OPCODE_JUMP_PROCEDURE = 10,
    SCR_OPCODE_CALL_PROCEDURE = 11,
    SCR_OPCODE_RUN = 12,
    SCR_OPCODE_GOTO_LABEL = 13,
    SCR_OPCODE_ADD = 14,
    SCR_OPCODE_SUBTRACT = 15,
    SCR_OPCODE_MULTIPLY = 16,
    SCR_OPCODE_DIVIDE = 17,
    SCR_OPCODE_NEGATE = 18,
    SCR_OPCODE_NOT = 19,
    SCR_OPCODE_LOGICAL_OR = 20,
    SCR_OPCODE_LOGICAL_AND = 21,
    SCR_OPCODE_EQUAL = 22,
    SCR_OPCODE_NOT_EQUAL = 23,
    SCR_OPCODE_LESS_THAN = 24,
    SCR_OPCODE_GREATER_THAN = 25,
    SCR_OPCODE_LESS_OR_EQUAL = 26,
    SCR_OPCODE_GREATER_OR_EQUAL = 27,
    SCR_OPCODE_IF = 28,
    SCR_OPCODE_PUSH_SIGNED_SHORT = 29,
    SCR_OPCODE_PUSH_LOCAL_INTEGER = 30,
    SCR_OPCODE_PUSH_LOCAL_FLOAT = 31,
    SCR_OPCODE_POP_LOCAL_INTEGER = 32,
    SCR_OPCODE_POP_LOCAL_FLOAT = 33,
    SCR_OPCODE_PUSH_STRING = 34,
    SCR_OPCODE_COUNT = 35
};

/* One 4-byte script operand slot; the type tag determines whether a word is
 * a scalar, a global-register index, a return address, or a borrowed string. */
typedef union ScrStackValue {
    u32 bits; /* raw slot copy, preserving the separately stored type tag */
    ScrVmInteger integer;
    f32 floating;
    ScrGlobalIntegerIndex globalIntegerIndex;
    ScrGlobalFloatIndex globalFloatIndex;
    ScrProgramCounter returnAddress;
    const char *string; /* borrowed read-only pointer into source FLW0 string section */
} ScrStackValue;

typedef s8 ScrStackValueType;

/* SCR_OPCODE_COMMAND uses this instruction view's second halfword to select
 * a callback and parameter count from the command table. */
typedef struct ScrCommandInstruction {
    ScrOpcode opCode;
    ScrCommandIndex commandIndex;
} ScrCommandInstruction;

typedef char ScrCommandInstruction_size_must_be_0x04[
    (sizeof(ScrCommandInstruction) == 4) ? 1 : -1];

/* Local-variable opcodes use the second halfword as a typed slot index. */
typedef struct ScrLocalIntegerInstruction {
    ScrOpcode opCode;
    ScrLocalIntegerIndex localIndex;
} ScrLocalIntegerInstruction;

typedef struct ScrLocalFloatInstruction {
    ScrOpcode opCode;
    ScrLocalFloatIndex localIndex;
} ScrLocalFloatInstruction;

/* Control-flow instruction operands are signed halfword indexes into FLW0
 * procedure and label tables, distinct from the VM program counter. */
typedef s16 ScrProcedureTableIndexOperand;
typedef s16 ScrLabelTableIndexOperand;

typedef struct ScrProcedureIndexInstruction {
    ScrOpcode opCode;
    ScrProcedureTableIndexOperand procedureIndex;
} ScrProcedureIndexInstruction;

typedef struct ScrLabelIndexInstruction {
    ScrOpcode opCode;
    ScrLabelTableIndexOperand labelIndex;
} ScrLabelIndexInstruction;

typedef char ScrLocalIntegerInstruction_size_must_be_0x04[
    (sizeof(ScrLocalIntegerInstruction) == 4) ? 1 : -1];
typedef char ScrLocalFloatInstruction_size_must_be_0x04[
    (sizeof(ScrLocalFloatInstruction) == 4) ? 1 : -1];
typedef char ScrProcedureIndexInstruction_size_must_be_0x04[
    (sizeof(ScrProcedureIndexInstruction) == 4) ? 1 : -1];
typedef char ScrLabelIndexInstruction_size_must_be_0x04[
    (sizeof(ScrLabelIndexInstruction) == 4) ? 1 : -1];

/* Four-byte decoded VM instruction with opcode-specific operand overlays. */
typedef union ScrInstr {
    struct {
        ScrOpcode opCode;
        s16 sOperand;
    } parts;
    ScrCommandInstruction command;
    ScrLocalIntegerInstruction localInteger;
    ScrLocalFloatInstruction localFloat;
    ScrProcedureIndexInstruction procedureIndex;
    ScrLabelIndexInstruction labelIndex;
    s32 iOperand;
    f32 fOperand;
} ScrInstr;

/* FLW0 label-table entry (0x20); its instruction address is at 0x18. */
typedef struct ScrLabel {
    char name[24];
    ScrProgramCounter addr; /* 0x18 */
    s32 unk1C;
} ScrLabel;

typedef char ScrLabel_size_must_be_0x20[
    (sizeof(ScrLabel) == 0x20) ? 1 : -1];

/* Procedure-table entry; distinct from a label despite sharing its 0x20 layout. */
typedef struct ScrProcedure {
    char name[24];
    ScrProgramCounter addr; /* 0x18 */
    s32 unk1C;
} ScrProcedure;

typedef char ScrProcedure_size_must_be_0x20[
    (sizeof(ScrProcedure) == 0x20) ? 1 : -1];

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
    ScrStackDepth sp; /* live stack depth; the next push writes at this index */
    ScrStackValueType stackTypes[SCR_STACK_STORAGE_SLOT_COUNT];
    ScrStackValue stackValues[SCR_STACK_STORAGE_SLOT_COUNT];
    /* Borrowed FLW0 source views; the VM reads them but does not edit them. */
    const struct BfFlw0Header *scriptHeader;
    const ScrSection *procedureSection; /* FLW0 descriptor; count bounds the procedure table */
    const ScrProcedure *procedures; /* FLW0 PROCEDURES section */
    const ScrLabel *labels;          /* FLW0 LABELS section */
    const ScrInstr *instructions;
    struct ItfMesSub *auxiliaryData; /* mutable: message initialization relocates it */
    const char *strings;          /* borrowed read-only view; lifetime follows source owner/caller */
    ScrProcedureIndex procedureIndex;
    ItfMesWindowIndex messageWindowIndex;
    u32 timer;
    ScrCommandYieldCount cmdTimer; /* prior yielded traces since last continuation */
    struct sdfMemHandle_t *scriptHandle; /* 0xD8: allocation descriptor, not scriptHeader data */
    ScrVmInteger *localInt; /* process-owned VM integer locals */
    f32 *localFloat;       /* process-owned VM float locals */
    dds3ProcessID_t *task;       /* 0xE4: scheduler object returned by dds3StartProcess */
    struct scrProcessWork_s *previous; /* 0xE8: named-process list links */
    struct scrProcessWork_s *next;     /* 0xEC */
    void *userData;       /* 0xF0: opaque caller-owned script context, never freed here */
} scrProcessWork_s;

typedef char ScrData_size_must_be_0xF4[
    (sizeof(scrProcessWork_s) == 0xF4) ? 1 : -1];
typedef char ScrStack_return_value_slot_must_be_last[
    (SCR_STACK_RETURN_VALUE_SLOT + 1 == SCR_STACK_STORAGE_SLOT_COUNT) ? 1 : -1];
typedef char ScrData_scriptHandle_at_0xD8[
    ((u32)&((scrProcessWork_s *)0)->scriptHandle == 0xD8) ? 1 : -1];
typedef char ScrData_messageWindowIndex_at_0xCC[
    ((u32)&((scrProcessWork_s *)0)->messageWindowIndex == 0xCC) ? 1 : -1];

extern scrProcessWork_s *nowpwork;
extern scrProcessWork_s *scrHandleFirst;
extern scrProcessWork_s *scrHandleLast;

scrProcessWork_s *scrGetScriptHandle(void);
scrProcessWork_s *scrSearchScriptName(const char *name);
void *scrGetUserData(dds3ProcessID_t *task);
void scrSetUserData(dds3ProcessID_t *task, void *actor);
void scrScriptShutdown(dds3ProcessID_t *task);
void scrRunScript(scrProcessWork_s *context);
KwlnTaskUpdateResult scrScriptProcess(dds3ProcessID_t *task);
ScrTraceResult scrTraceCode(scrProcessWork_s *context);
void scrProcDestroyTask(scrProcessWork_s *process);
scrProcessWork_s *scrCreateTaskWithDefaultOption(const struct BfFlw0Header *header);
void PushInt(scrProcessWork_s *script, ScrVmInteger value);
void PushFloat(scrProcessWork_s *script, f32 value);
void PushStr(scrProcessWork_s *script, const char *value);
void PushRet(scrProcessWork_s *script, ScrProgramCounter returnAddress);
ScrVmInteger PopInt(scrProcessWork_s *script);
f32 PopFloat(scrProcessWork_s *script);
/* Command callback plus operand count (0x8); DDS1 script/scrTraceCode.c. */
typedef struct ScrCommand {
    ScrCommandResult (*func)(void);
    ScrCommandParameterCount paramCount;
} ScrCommand;

/* Script integer register 210 (dds3GlobalWork_t +0x388): set/clear by the process-control script commands. */
#define SCR_PROCESS_CONTROL_FLAG_REGISTER ((ScrGlobalIntegerIndex)210)

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
#endif /* SCR_H */
