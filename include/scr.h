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

/* One stack slot, tagged by the matching scrProcessWork_s.StackType byte. */
typedef union scrStackData_u scrStackData_u;
union scrStackData_u
{
    s32 i;
    f32 f;
    struct
    {
        s16 s0;
        s16 s1;
    } s;
    struct
    {
        u8 c0;
        u8 c1;
        u8 c2;
        u8 c3;
    } c;
};

typedef s8 ScrStackValueType;

/* One bytecode word: s0 is the opcode, s1 its operand, and the other members
 * read the same four bytes as an immediate. */
typedef union scrCode_u scrCode_u;
union scrCode_u
{
    s32 i;
    f32 f;
    struct
    {
        s16 s0;
        s16 s1;
    } s;
    struct
    {
        u8 c0;
        u8 c1;
        u8 c2;
        u8 c3;
    } c;
};

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
    scrStackData_u stackValues[SCR_STACK_STORAGE_SLOT_COUNT];
    /* Borrowed FLW0 source views; the VM reads them but does not edit them. */
    const struct BfFlw0Header *scriptHeader;
    const ScrSection *procedureSection; /* FLW0 descriptor; count bounds the procedure table */
    const ScrProcedure *procedures; /* FLW0 PROCEDURES section */
    const ScrLabel *labels;          /* FLW0 LABELS section */
    const scrCode_u *instructions;
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

enum scr_stack_type
{
    SCR_STACK_TYPE_INT = 0,
    SCR_STACK_TYPE_FLOAT = 1,
    SCR_STACK_TYPE_INTVAR = 2,
    SCR_STACK_TYPE_FLOATVAR = 3,
    SCR_STACK_TYPE_RET = 4,
    SCR_STACK_TYPE_STRING = 5,
    SCR_STACK_TYPE_EMPTY = 6,
    SCR_STACK_TYPE_MAX = 7
};

/* Bytecode opcodes, the index into scrTraceCode's codefunc table. */
enum code_type
{
    CODE_TYPE_PUSHI = 0,
    CODE_TYPE_PUSHF = 1,
    CODE_TYPE_PUSHIX = 2,
    CODE_TYPE_PUSHIY = 3,
    CODE_TYPE_PUSHREG = 4,
    CODE_TYPE_POPIX = 5,
    CODE_TYPE_POPIY = 6,
    CODE_TYPE_PROC = 7,
    CODE_TYPE_COMM = 8,
    CODE_TYPE_END = 9,
    CODE_TYPE_JUMP = 10,
    CODE_TYPE_CALL = 11,
    CODE_TYPE_RUN = 12,
    CODE_TYPE_GOTO = 13,
    CODE_TYPE_ADD = 14,
    CODE_TYPE_SUB = 15,
    CODE_TYPE_MUL = 16,
    CODE_TYPE_DIV = 17,
    CODE_TYPE_MINUS = 18,
    CODE_TYPE_NOT = 19,
    CODE_TYPE_OR = 20,
    CODE_TYPE_AND = 21,
    CODE_TYPE_EQ = 22,
    CODE_TYPE_NEQ = 23,
    CODE_TYPE_S = 24,
    CODE_TYPE_L = 25,
    CODE_TYPE_SE = 26,
    CODE_TYPE_LE = 27,
    CODE_TYPE_IF = 28,
    CODE_TYPE_PUSHIS = 29,
    CODE_TYPE_PUSHLIX = 30,
    CODE_TYPE_PUSHLFX = 31,
    CODE_TYPE_POPLIX = 32,
    CODE_TYPE_POPLFX = 33,
    CODE_TYPE_PUSHSTR = 34,
    CODE_TYPE_MAX = 35
};
#endif /* SCR_H */
