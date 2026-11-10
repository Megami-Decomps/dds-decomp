#ifndef SCR_H
#define SCR_H

/* The FLW0 script interpreter: the loaded file's tables, and the per-procedure
 * work block scrTraceCode steps through.
 * See include/README.md for how the layouts were verified. */

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

typedef struct scrHeader_t scrHeader_t;
typedef struct scrTypeTable_t scrTypeTable_t;
typedef struct scrProcTable_t scrProcTable_t;
typedef struct scrLabelTable_t scrLabelTable_t;
typedef union scrCode_u scrCode_u;
typedef union scrStackData_u scrStackData_u;
typedef struct scrCommandTable_t scrCommandTable_t;
typedef struct scrProcessWork_s scrProcessWork_s;

dds3ProcessID_t *scrStartScriptProcess(s32 priority, scrProcessWork_s *work);
/* DDS returns the process: code_00196478 and code_0019E138 use it. */
dds3ProcessID_t *scrStartScriptByName(s32 priority, char *name, s32 messageHandle);
dds3ProcessID_t *scrStartScript2(s32 priority, void *data, s32 startproc);

/* A command handler pops its own arguments with scrGetIntPara and friends and
 * returns non-zero to advance past the instruction. */
typedef s32 (*scrCommandFunc_t)(void);

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

/* One bytecode word: s0 is the opcode, s1 its operand, and the other members
 * read the same four bytes as an immediate. */
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

/* One stack slot, tagged by the matching scrProcessWork_s.StackType byte. */
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

/* CodeFunc_Comm dispatches on the opcode's operand and then drops paranum
 * slots off the stack. */
struct scrCommandTable_t
{
    scrCommandFunc_t comm;
    s32 paranum;
};

/* One running procedure. Slot 27 of the two stack arrays is the return value:
 * scrSetIntReturnValue writes StackData[27] and tags StackType[27]. */
struct scrProcessWork_s
{
    u8 Name[24];
    s32 CP;
    s32 SP;
    u8 StackType[28]; /* enum scr_stack_type per slot */
    scrStackData_u StackData[28];
    scrHeader_t *Header;
    scrTypeTable_t *TypeTable;
    scrProcTable_t *ProcTable;
    scrLabelTable_t *LabelTable;
    scrCode_u *Code;
    u8 *MessageData;
    u8 *StringData;
    s32 StartProc;
    s32 MesHandle;
    s32 ScriptTimer;
    s32 CommandTimer;
    struct sdfMemHandle_t *MemHandle; /* sdfMemHandle_t: libsdf.h conflicts with DDS sdf.h */
    u32 *LocalInt;
    f32 *LocalFloat;
    dds3ProcessID_t *ProcessID;
    scrProcessWork_s *prev;
    scrProcessWork_s *next;
    void *data;
};

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

/* What a codefunc returns to scrTraceCode. */
enum code_ret
{
    CODE_RET_END = 0,
    CODE_RET_NEXT = 1,
    CODE_RET_SYNC = 2
};

enum scr_result
{
    SCR_RESULT_ERROR = 0,
    SCR_RESULT_CONTINUE = 1,
    SCR_RESULT_END = 2
};

/* DDS1/2 additions with no SMT3 counterpart in this header. */
struct ItfMesSub;

#define SCR_STACK_RETURN_VALUE_SLOT 27

extern scrProcessWork_s *scrHandleFirst;
extern scrProcessWork_s *scrHandleLast;

scrProcessWork_s *scrGetScriptHandle(void);
scrProcessWork_s *scrSearchScriptName(char *scriptname);
void scrSetUserData(dds3ProcessID_t *id, void *p);
s32 scrRunScript(scrProcessWork_s *h);
s32 scrScriptProcess(dds3ProcessID_t *id);
s32 scrTraceCode(scrProcessWork_s *pwork);
void scrPrintProcedureEnd(scrProcessWork_s *pw);
scrProcessWork_s *scrCreateTaskWithDefaultOption(void *data);
/* Return the procedure-table index for an exact name match, or NOT_FOUND. */
s32 scrSearchProcedureName(void *scrdata, char *procname);

#endif /* SCR_H */
