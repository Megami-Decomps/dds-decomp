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
    s8 stackTypes[28];             // 0x20
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

typedef struct ScrVM {
    u8 unk00[0x40];  // 0x00
    s32 ints[256];   // 0x40
    f32 floats[256]; // 0x440
} ScrVM;

typedef struct ScrCommand {
    u32 (*func)(void);
    s32 paramCount;
} ScrCommand;

#define SCR_STACK_RET 27
#define SCR_STACK_TYPE_STRING 5

extern ScrVM *D_003BAA00;
extern ScrData *D_003BD78C;
extern ScrCommand D_0039E288[];
extern u32 (*D_00329930[])(ScrData *scr);

void func_0010C120(ScrData *scr, s32 val);
void func_0010C150(ScrData *scr, f32 val);
void func_0010C180(ScrData *scr, char *str);
void func_0010C1B0(ScrData *scr, s32 val);
s32 func_0010C1E0(ScrData *scr);
f32 func_0010C2B8(ScrData *scr);
u32 func_0010C9A8(ScrData *scr, s32 op);

u32 func_0010C3F0(ScrData *scr)
{
    func_0010C120(scr, scr->instructions[scr->pc].parts.sOperand);
    scr->pc++;
    return 1;
}

u32 func_0010C438(ScrData *scr)
{
    scr->pc++;
    func_0010C150(scr, scr->instructions[scr->pc].fOperand);
    scr->pc++;
    return 1;
}

u32 func_0010C488(ScrData *scr)
{
    func_0010C120(scr, D_003BAA00->ints[scr->instructions[scr->pc].parts.sOperand]);
    scr->pc++;
    return 1;
}

u32 func_0010C4E0(ScrData *scr)
{
    func_0010C150(scr, D_003BAA00->floats[scr->instructions[scr->pc].parts.sOperand]);
    scr->pc++;
    return 1;
}

u32 func_0010C538(ScrData *scr)
{
    func_0010C120(scr, scr->localInt[scr->instructions[scr->pc].parts.sOperand]);
    scr->pc++;
    return 1;
}

u32 func_0010C590(ScrData *scr)
{
    func_0010C150(scr, scr->localFloat[scr->instructions[scr->pc].parts.sOperand]);
    scr->pc++;
    return 1;
}

u32 func_0010C5E8(ScrData *scr)
{
    func_0010C180(scr, scr->strings + scr->instructions[scr->pc].parts.sOperand);
    scr->pc++;
    return 1;
}

u32 func_0010C638(ScrData *scr)
{
    scr->stackTypes[scr->sp] = scr->stackTypes[SCR_STACK_RET];
    scr->stackValues[scr->sp].i = scr->stackValues[SCR_STACK_RET].i;
    scr->sp++;
    scr->pc++;
    return 1;
}

u32 func_0010C680(ScrData *scr)
{
    D_003BAA00->ints[scr->instructions[scr->pc].parts.sOperand] = func_0010C1E0(scr);
    scr->pc++;
    return 1;
}

u32 func_0010C6D8(ScrData *scr)
{
    f32 val;

    val = func_0010C2B8(scr);
    D_003BAA00->floats[scr->instructions[scr->pc].parts.sOperand] = val;
    scr->pc++;
    return 1;
}

u32 func_0010C730(ScrData *scr)
{
    scr->localInt[scr->instructions[scr->pc].parts.sOperand] = func_0010C1E0(scr);
    scr->pc++;
    return 1;
}

u32 func_0010C788(ScrData *scr)
{
    scr->localFloat[scr->instructions[scr->pc].parts.sOperand] = func_0010C2B8(scr);
    scr->pc++;
    return 1;
}

u32 func_0010C7E0(ScrData *scr)
{
    scr->pc++;
    return 1;
}

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010C7F8);

u32 func_0010C898(ScrData *scr)
{
    if (scr->sp == 0) {
        return 0;
    }
    scr->pc = func_0010C1E0(scr) + 1;
    return 1;
}

u32 func_0010C8D8(ScrData *scr)
{
    scr->pc = scr->procedures[scr->instructions[scr->pc].parts.sOperand].addr;
    return 1;
}

u32 func_0010C908(ScrData *scr)
{
    func_0010C1B0(scr, scr->pc);
    scr->pc = scr->procedures[scr->instructions[scr->pc].parts.sOperand].addr;
    return 1;
}

u32 func_0010C960(ScrData *scr)
{
    scr->pc++;
    return 1;
}

u32 func_0010C978(ScrData *scr)
{
    scr->pc = scr->labels[scr->instructions[scr->pc].parts.sOperand].addr;
    return 1;
}

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010C9A8);

u32 func_0010CE90(ScrData *scr)
{
    func_0010C9A8(scr, 0);
    scr->pc++;
    return 1;
}

u32 func_0010CEC8(ScrData *scr)
{
    func_0010C9A8(scr, 1);
    scr->pc++;
    return 1;
}

u32 func_0010CF00(ScrData *scr)
{
    func_0010C9A8(scr, 2);
    scr->pc++;
    return 1;
}

u32 func_0010CF38(ScrData *scr)
{
    func_0010C9A8(scr, 3);
    scr->pc++;
    return 1;
}

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010CF70);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D058);

u32 func_0010D100(ScrData *scr)
{
    func_0010C9A8(scr, 4);
    scr->pc++;
    return 1;
}

u32 func_0010D138(ScrData *scr)
{
    func_0010C9A8(scr, 5);
    scr->pc++;
    return 1;
}

u32 func_0010D170(ScrData *scr)
{
    func_0010C9A8(scr, 6);
    scr->pc++;
    return 1;
}

u32 func_0010D1A8(ScrData *scr)
{
    func_0010C9A8(scr, 7);
    scr->pc++;
    return 1;
}

u32 func_0010D1E0(ScrData *scr)
{
    func_0010C9A8(scr, 8);
    scr->pc++;
    return 1;
}

u32 func_0010D218(ScrData *scr)
{
    func_0010C9A8(scr, 9);
    scr->pc++;
    return 1;
}

u32 func_0010D250(ScrData *scr)
{
    func_0010C9A8(scr, 10);
    scr->pc++;
    return 1;
}

u32 func_0010D288(ScrData *scr)
{
    func_0010C9A8(scr, 11);
    scr->pc++;
    return 1;
}

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D2C0);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D380);

/* Script command parameter `idx` (0 = first) as an int, converting floats and
 * dereferencing global variable references. */
s32 func_0010D428(s32 idx) {
    ScrData *scr = D_003BD78C;
    s32 i = scr->sp - idx - 1;

    switch (scr->stackTypes[i]) {
    case 0:
    case 4:
        return scr->stackValues[i].i;
    case 1:
        return scr->stackValues[i].f;
    case 2:
        return D_003BAA00->ints[scr->stackValues[i].i];
    case 3:
        return D_003BAA00->floats[scr->stackValues[i].i];
    }
    return 0;
}

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D4F0);

char *func_0010D5A8(s32 paramIdx)
{
    ScrData *scr = D_003BD78C;
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

void func_0010D5F0(s32 retVal)
{
    D_003BD78C->stackTypes[SCR_STACK_RET] = 0;
    D_003BD78C->stackValues[SCR_STACK_RET].i = retVal;
}

void func_0010D608(f32 retVal)
{
    D_003BD78C->stackTypes[SCR_STACK_RET] = 1;
    D_003BD78C->stackValues[SCR_STACK_RET].f = retVal;
}
