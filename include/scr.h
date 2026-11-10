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
struct ItfMesSub;

typedef s32 ScrProgramCounter;
typedef s32 ScrGlobalIntegerIndex;
typedef s32 ScrGlobalFloatIndex;
/* Zero-based signed halfword selectors for the VM's per-process local arrays. */
typedef s16 ScrLocalIntegerIndex;
typedef s16 ScrLocalFloatIndex;
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

typedef struct scrHeader_t scrHeader_t;
typedef struct scrTypeTable_t scrTypeTable_t;
typedef struct scrProcTable_t scrProcTable_t;
typedef struct scrLabelTable_t scrLabelTable_t;

/* File header. MagicCode is "FLW0" compared as a word, so it is four bytes
 * inline, not a pointer. The type table starts right after, at +32. */
struct scrHeader_t
{
    u8 FileType;
    u8 FileFormat;
    u16 UserID;
    s32 FileSize;
    char MagicCode[4];
    s32 ExpandSize;
    s32 TypeTableCount;
    s16 LocalIntCount;
    s16 LocalFloatCount;
    s32 Resrve2;
    s32 Resrve3;
};

/* One entry per section of the file. ItemAddress is a byte offset from the
 * header, so the section pointer is (u8 *)header + ItemAddress. */
struct scrTypeTable_t
{
    s32 Type; /* enum scr_datatype */
    s32 ItemSize;
    s32 ItemCount;
    s32 ItemAddress;
};

struct scrProcTable_t
{
    u8 Name[24];
    s32 CodeAddress;
    s32 Reserve1;
};

struct scrLabelTable_t
{
    u8 Name[24];
    s32 CodeAddress;
    s32 Reserve1;
};

/* Native 0xF4-byte process allocated by bfContextCreate in both games.
 * The interpreter, scheduler attachment and named-process list share this owner. */
typedef struct scrProcessWork_s {
    char name[24];
    ScrProgramCounter pc;
    ScrStackDepth sp; /* live stack depth; the next push writes at this index */
    ScrStackValueType stackTypes[SCR_STACK_STORAGE_SLOT_COUNT];
    scrStackData_u stackValues[SCR_STACK_STORAGE_SLOT_COUNT];
    /* Borrowed FLW0 source views; the VM reads them but does not edit them. */
    const struct scrHeader_t *scriptHeader;
    const scrTypeTable_t *procedureSection; /* FLW0 descriptor; count bounds the procedure table */
    const scrProcTable_t *procedures; /* FLW0 PROCEDURES section */
    const scrLabelTable_t *labels;          /* FLW0 LABELS section */
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
scrProcessWork_s *scrCreateTaskWithDefaultOption(const struct scrHeader_t *header);
void PushInt(scrProcessWork_s *script, ScrVmInteger value);
void PushFloat(scrProcessWork_s *script, f32 value);
void PushStr(scrProcessWork_s *script, const char *value);
void PushRet(scrProcessWork_s *script, ScrProgramCounter returnAddress);
ScrVmInteger PopInt(scrProcessWork_s *script);
f32 PopFloat(scrProcessWork_s *script);
/* Return the procedure-table index for an exact name match, or NOT_FOUND. */
ScrProcedureIndex scrSearchProcedureName(const scrHeader_t *header, const char *name);
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

/* scrTypeTable_t.Type: which section of the file the entry describes. */
enum scr_datatype
{
    SCR_DATATYPE_PROCTABLE = 0,
    SCR_DATATYPE_LABELTABLE = 1,
    SCR_DATATYPE_SCRDATA = 2,
    SCR_DATATYPE_MESSAGE = 3,
    SCR_DATATYPE_STRINGS = 4,
    SCR_DATATYPE_MAX = 5
};

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
