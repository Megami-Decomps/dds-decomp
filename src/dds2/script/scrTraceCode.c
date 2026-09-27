#include "common.h"

extern u32 func_0010C408(void);

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

#define SCR_STACK_TYPE_STRING 5

extern ScrData *D_00438E8C;

#define SCR_STACK_RET 27

typedef struct ScrVM {
    u8 unk00[0x40];  // 0x00
    s32 ints[256];   // 0x40
    f32 floats[256]; // 0x440
} ScrVM;

extern ScrVM *D_00435DD0;

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010C618);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010C660);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010C6B0);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010C708);

u32 func_0010C760(u32 arg0) {
    s32 temp_v0;

    temp_v0 = (s32)arg0;
    func_0010C348(arg0, *(u32 *)
                                                  (*(s16 *)(*(s32 *)(temp_v0 + 0x18) * 4 + *(s32 *)(temp_v0 + 0xbc) + 2) * 4 +
                                                  *(s32 *)(temp_v0 + 0xdc)));
    *(s32 *)(temp_v0 + 0x18) = *(s32 *)(temp_v0 + 0x18) + 1;
    return 1;
}

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010C7B8);

u32 func_0010C810(u32 arg0) {
    s32 temp_v0;

    temp_v0 = (s32)arg0;
    func_0010C3A8(arg0, *(s32 *)(temp_v0 + 0xc4) +
                                                (s32)*(s16 *)(*(s32 *)(temp_v0 + 0x18) * 4 + *(s32 *)(temp_v0 + 0xbc) + 2));
    *(s32 *)(temp_v0 + 0x18) = *(s32 *)(temp_v0 + 0x18) + 1;
    return 1;
}

u32 func_0010C860(s32 arg0) {
    *(u8 *)(*(s32 *)(arg0 + 0x1c) + arg0 + 0x20) = *(u8 *)(arg0 + 0x3b);
    *(u32 *)(*(s32 *)(arg0 + 0x1c) * 4 + arg0 + 0x3c) = *(u32 *)(arg0 + 0xa8);
    *(s32 *)(arg0 + 0x1c) = *(s32 *)(arg0 + 0x1c) + 1;
    *(s32 *)(arg0 + 0x18) = *(s32 *)(arg0 + 0x18) + 1;
    return 1;
}

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010C8A8);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010C900);

u32 func_0010C958(s32 arg0) {
    u32 temp_v0;

    temp_v0 = func_0010C408();
    *(u32 *)
      (*(s16 *)(*(s32 *)(arg0 + 0x18) * 4 + *(s32 *)(arg0 + 0xbc) + 2) * 4 +
      *(s32 *)(arg0 + 0xdc)) = temp_v0;
    *(s32 *)(arg0 + 0x18) = *(s32 *)(arg0 + 0x18) + 1;
    return 1;
}

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010C9B0);

u32 func_0010CA08(s32 arg0) {
    *(s32 *)(arg0 + 0x18) = *(s32 *)(arg0 + 0x18) + 1;
    return 1;
}

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010CA20);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010CAC0);

u32 func_0010CB00(s32 arg0) {
    *(u32 *)(arg0 + 0x18) =
              *(u32 *)
                (*(s16 *)(*(s32 *)(arg0 + 0x18) * 4 + *(s32 *)(arg0 + 0xbc) + 2) * 0x20 +
                  *(s32 *)(arg0 + 0xb4) + 0x18);
    return 1;
}

u32 func_0010CB30(u32 arg0) {
    s32 temp_v0;

    temp_v0 = (s32)arg0;
    func_0010C3D8(arg0, *(u32 *)(temp_v0 + 0x18));
    *(u32 *)(temp_v0 + 0x18) =
              *(u32 *)
                (*(s16 *)(*(s32 *)(temp_v0 + 0x18) * 4 + *(s32 *)(temp_v0 + 0xbc) + 2) * 0x20 +
                  *(s32 *)(temp_v0 + 0xb4) + 0x18);
    return 1;
}

u32 func_0010CB88(s32 arg0) {
    *(s32 *)(arg0 + 0x18) = *(s32 *)(arg0 + 0x18) + 1;
    return 1;
}

u32 func_0010CBA0(s32 arg0) {
    *(u32 *)(arg0 + 0x18) =
              *(u32 *)
                (*(s16 *)(*(s32 *)(arg0 + 0x18) * 4 + *(s32 *)(arg0 + 0xbc) + 2) * 0x20 +
                  *(s32 *)(arg0 + 0xb8) + 0x18);
    return 1;
}

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010CBD0);

u32 func_0010D0B8(u32 arg0) {
    func_0010CBD0(arg0, 0);
    *(s32 *)((s32)arg0 + 0x18) = *(s32 *)((s32)arg0 + 0x18) + 1;
    return 1;
}

u32 func_0010D0F0(u32 arg0) {
    func_0010CBD0(arg0, 1);
    *(s32 *)((s32)arg0 + 0x18) = *(s32 *)((s32)arg0 + 0x18) + 1;
    return 1;
}

u32 func_0010D128(u32 arg0) {
    func_0010CBD0(arg0, 2);
    *(s32 *)((s32)arg0 + 0x18) = *(s32 *)((s32)arg0 + 0x18) + 1;
    return 1;
}

u32 func_0010D160(u32 arg0) {
    func_0010CBD0(arg0, 3);
    *(s32 *)((s32)arg0 + 0x18) = *(s32 *)((s32)arg0 + 0x18) + 1;
    return 1;
}

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D198);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D280);

u32 func_0010D328(u32 arg0) {
    func_0010CBD0(arg0, 4);
    *(s32 *)((s32)arg0 + 0x18) = *(s32 *)((s32)arg0 + 0x18) + 1;
    return 1;
}

u32 func_0010D360(u32 arg0) {
    func_0010CBD0(arg0, 5);
    *(s32 *)((s32)arg0 + 0x18) = *(s32 *)((s32)arg0 + 0x18) + 1;
    return 1;
}

u32 func_0010D398(u32 arg0) {
    func_0010CBD0(arg0, 6);
    *(s32 *)((s32)arg0 + 0x18) = *(s32 *)((s32)arg0 + 0x18) + 1;
    return 1;
}

u32 func_0010D3D0(u32 arg0) {
    func_0010CBD0(arg0, 7);
    *(s32 *)((s32)arg0 + 0x18) = *(s32 *)((s32)arg0 + 0x18) + 1;
    return 1;
}

u32 func_0010D408(u32 arg0) {
    func_0010CBD0(arg0, 8);
    *(s32 *)((s32)arg0 + 0x18) = *(s32 *)((s32)arg0 + 0x18) + 1;
    return 1;
}

u32 func_0010D440(u32 arg0) {
    func_0010CBD0(arg0, 9);
    *(s32 *)((s32)arg0 + 0x18) = *(s32 *)((s32)arg0 + 0x18) + 1;
    return 1;
}

u32 func_0010D478(u32 arg0) {
    func_0010CBD0(arg0, 10);
    *(s32 *)((s32)arg0 + 0x18) = *(s32 *)((s32)arg0 + 0x18) + 1;
    return 1;
}

u32 func_0010D4B0(u32 arg0) {
    func_0010CBD0(arg0, 0xb);
    *(s32 *)((s32)arg0 + 0x18) = *(s32 *)((s32)arg0 + 0x18) + 1;
    return 1;
}

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D4E8);

INCLUDE_ASM(const s32, "script/scrTraceCode", func_0010D5A8);

/* Script command parameter `idx` (0 = first) as an int, converting floats and
 * dereferencing global variable references. */
s32 func_0010D650(s32 idx) {
    ScrData *scr = D_00438E8C;
    s32 i = scr->sp - idx - 1;

    switch (scr->stackTypes[i]) {
    case 0:
    case 4:
        return scr->stackValues[i].i;
    case 1:
        return scr->stackValues[i].f;
    case 2:
        return D_00435DD0->ints[scr->stackValues[i].i];
    case 3:
        return D_00435DD0->floats[scr->stackValues[i].i];
    }
    return 0;
}

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
