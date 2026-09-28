#include "common.h"

typedef struct AdminWork AdminWork;
struct AdminWork {
    u32 flags;
    u32 unk04;
    s8 unk08;
    s8 unk09;
    u8 historyIndex;
    u8 pad0B;
    s8 unk0C[8];
    u8 unk14[8];
    void* unk1C;
    u8 unk20;
    u8 unk21;
    u8 pad22[2];
};

extern AdminWork* func_001028A0(void);
extern void func_001028E8(s32 a0, s32 a1, s32 a2, s32 a3);
extern void* func_00101A70(void* task);
extern void func_00101A68(void* task, u32 value);
extern void* func_002CFEB8(s32 a0);
extern void func_002CFF98(void* ptr);
extern void* memcpy(void* dst, void* src, s32 n);
extern void* D_003297D0[];
extern void* D_003297D4[];
extern void* D_003297D8[];
extern u8 D_003BA848[];

void func_001029E8(s32 a0, s32 a1, s32 a2)
{
    AdminWork* work;

    func_001028E8(a0, a1, a2, 0);
    work = func_001028A0();
    work->flags |= 8;
}

void func_00102A18(void)
{
    AdminWork* work;

    work = func_001028A0();
    work->flags |= 2;
}

s8 func_00102A40(void)
{
    return func_001028A0()->unk08;
}

s8 func_00102A60(void)
{
    return func_001028A0()->unk09;
}

s8 func_00102A80(void)
{
    AdminWork* work;

    work = func_001028A0();
    return work->unk0C[(work->historyIndex + 7) & 7];
}

u8 func_00102AB0(void)
{
    AdminWork* work;

    work = func_001028A0();
    return work->unk14[(work->historyIndex + 7) & 7];
}

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", func_00102AE0);

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", func_00102C58);

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", func_00102CD8);

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", func_00102E58);
