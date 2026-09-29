#ifndef SCR_H
#define SCR_H

#include "common.h"

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

/* Interpreter state, operands and stack (0xE4); DDS1/2 script/scrTraceCode.c. */
typedef struct ScrData {
    char name[24];
    s32 pc;
    s32 sp;
    s8 stackTypes[28];
    ScrStackValue stackValues[28];
    void *unkAC;
    void *unkB0;
    ScrLabel *procedures;
    ScrLabel *labels;
    ScrInstr *instructions;
    void *unkC0;
    char *strings;
    void *unkC8;
    void *unkCC;
    s32 timer;
    s32 cmdTimer;
    void *unkD8;
    s32 *localInt;
    f32 *localFloat;
} ScrData;

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

/* Script process handle and scheduler task ID (0xE8); DDS1/2 script/scrScriptProcess.c.
 * The task name is a 32-byte entry indexed from nameTableBase. */
typedef struct {
    u8 pad0[0xB4];
    s32 nameTableBase;      /* 0xB4: address of process names */
    u8 padB8[0x10];
    s32 nameIndex;          /* 0xC8: 32-byte name entry */
    s32 resourceIndex;      /* 0xCC: released unless negative */
    u8 padD0[8];
    void *scriptHandle;     /* 0xD8 */
    u32 workBuffer;         /* 0xDC */
    u32 auxBuffer;          /* 0xE0 */
    s32 taskId;             /* 0xE4 */
} ScrProcTask;

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

#endif /* SCR_H */
