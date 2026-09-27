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
    char name[24]; // 0x00
    s32 addr;      // 0x18
    s32 unk1C;     // 0x1C
} ScrLabel; // 0x20 bytes

typedef struct ScrData {
    char name[24];                 // 0x00
    s32 pc;                        // 0x18
    s32 sp;                        // 0x1C
    u8 stackTypes[28];             // 0x20
    ScrStackValue stackValues[28]; // 0x3C
    void *unkAC;                   // 0xAC
    void *unkB0;                   // 0xB0
    ScrLabel *procedures;          // 0xB4
    ScrLabel *labels;              // 0xB8
    ScrInstr *instructions;        // 0xBC
    void *unkC0;                   // 0xC0
    char *strings;                 // 0xC4
    void *unkC8;                   // 0xC8
    void *unkCC;                   // 0xCC
    s32 timer;                     // 0xD0
    s32 cmdTimer;                  // 0xD4
    void *unkD8;                   // 0xD8
    s32 *localInt;                 // 0xDC
    f32 *localFloat;               // 0xE0
} ScrData;

#define SCR_STACK_TYPE_STRING 5

extern ScrData *D_00438E8C;

#define SCR_STACK_RET 27

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010C618);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010C660);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010C6B0);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010C708);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010C760);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010C7B8);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010C810);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010C860);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010C8A8);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010C900);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010C958);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010C9B0);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010CA08);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010CA20);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010CAC0);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010CB00);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010CB30);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010CB88);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010CBA0);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010CBD0);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D0B8);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D0F0);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D128);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D160);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D198);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D280);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D2C0);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D2E0);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D328);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D360);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D398);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D3D0);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D408);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D440);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D478);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D4B0);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D4E8);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D528);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D538);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D5A8);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D650);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D718);

char *func_0010D7D0(s32 paramIdx)
{
    ScrData *scr = D_00438E8C;
    s32 paramSP = scr->sp - paramIdx - 1;
    s32 type = (s8)scr->stackTypes[paramSP];

    if (type >= 0) {
        if (type >= SCR_STACK_TYPE_STRING) {
            if (type == SCR_STACK_TYPE_STRING) {
                return scr->stackValues[paramSP].s;
            }
        }
    }
    return NULL;
}

void func_0010D818(s32 retVal)
{
    D_00438E8C->stackTypes[SCR_STACK_RET] = 0;
    D_00438E8C->stackValues[SCR_STACK_RET].i = retVal;
}

void func_0010D830(f32 retVal)
{
    D_00438E8C->stackTypes[SCR_STACK_RET] = 1;
    D_00438E8C->stackValues[SCR_STACK_RET].f = retVal;
}
