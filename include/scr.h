#ifndef SCR_H
#define SCR_H

#include "common.h"
#include "kwln.h"

/* One 4-byte script operand slot; DDS1/2 script/scrTraceCode.c via scrProcessWork_s. */
typedef union ScrStackValue {
    s32 i;
    f32 f;
    char *s;
} ScrStackValue;

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
typedef struct scrProcessWork_s {
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
    dds3ProcessID_t *task;       /* 0xE4: scheduler object returned by dds3StartProcess */
    struct scrProcessWork_s *previous; /* 0xE8: named-process list links */
    struct scrProcessWork_s *next;     /* 0xEC */
    void *actor;          /* 0xF0: caller-owned command work */
} scrProcessWork_s;

typedef char ScrData_size_must_be_0xF4[
    (sizeof(scrProcessWork_s) == 0xF4) ? 1 : -1];

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
s32 scrTraceCode(scrProcessWork_s *context);
void scrProcDestroyTask(scrProcessWork_s *process);
scrProcessWork_s *bfContextCreate(void *header, ScrSection *sections, ScrLabel *procedures,
                        ScrLabel *labels, ScrInstr *instructions, void *auxiliaryData,
                        char *strings, s32 procedureIndex);
scrProcessWork_s *scrCreateTaskWithDefaultOption(void *header);
void PushInt(scrProcessWork_s *script, s32 value);
void PushFloat(scrProcessWork_s *script, f32 value);
void PushStr(scrProcessWork_s *script, char *value);
void PushRet(scrProcessWork_s *script, s32 value);
s32 PopInt(scrProcessWork_s *script);
f32 PopFloat(scrProcessWork_s *script);
dds3ProcessID_t *scrRunScriptEx(u32 priority, void *header,
    ScrSection *sections, ScrLabel *procedures, ScrLabel *labels,
    ScrInstr *instructions, void *auxiliaryData, char *strings, s32 procedureIndex);


/* Command callback plus operand count (0x8); DDS1 script/scrTraceCode.c. */
typedef struct ScrCommand {
    u32 (*func)(void);
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

/* Script VM stack value types. */
#define SCR_STACK_TYPE_STRING 5
#define SCR_STACK_RET 27

#endif /* SCR_H */
