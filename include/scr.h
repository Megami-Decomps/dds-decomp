#ifndef SCR_H
#define SCR_H

#include "common.h"

typedef union ScrStackValue {
    s32 i;
    f32 f;
    char *s;
} ScrStackValue;

typedef union ScrInstr {
    struct {
        s16 opCode;
        s16 sOperand;
    } parts;
    s32 iOperand;
    f32 fOperand;
} ScrInstr;

typedef struct ScrLabel {
    char name[24];
    s32 addr;              /* 0x18 */
    s32 unk1C;
} ScrLabel;

/* Per-script interpreter state and operand stack. */
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

typedef struct ScrVM {
    u8 unk00[0x40];
    s32 ints[256];
    f32 floats[256];
} ScrVM;

typedef struct ScrCommand {
    u32 (*func)(void);
    s32 paramCount;
} ScrCommand;

typedef struct {
    u8 pad0[0x40];
    s32 integers[256];
    s32 floatBits[256];
} ScrProcGlobals;

typedef struct {
    u8 pad0[0xB4];
    s32 unkB4;
    u8 padB8[0x10];
    s32 unkC8;
    u8 padCC[0xC];
    void *scriptHandle;    /* 0xD8 */
    u8 padDC[8];
    s32 taskId;            /* 0xE4 */
} ScrProcTask;

typedef struct {
    u8 pad[0x388];
    s32 unk388;
} ScrComGlobals;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    s32 w;
} ScrVecW;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} ScrVec4;

/* ScrVmOperand is intentionally local: DDS1's +0x04 is u16, DDS2's is u8. */

#endif /* SCR_H */
