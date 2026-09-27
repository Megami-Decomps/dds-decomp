#include "common.h"

typedef struct VTab {
    void (*fn)(void);
} VTab;

typedef struct VObj {
    VTab *unk0;
} VObj;

typedef struct {
    void *unk0;
    void *unk4;
} Pair;

void func_003340D0(Pair *a0, void *a1, void *a2);

typedef struct {
    u8 pad[0x30];
    u8 unk30;
    u8 unk31;
} StateByte;

typedef struct KeyOut {
    f32 *p0;
    f32 *p4;
    f32 f8;
} KeyOut;

f32 func_00334788(KeyOut *a0);

extern s32 (*D_0040B368[])(void *a0, s32 a1);

extern s32 (*D_0040B3F8[])(void *a0, s32 a1);

typedef struct {
    s32 u0;
    s32 u4;
    s32 u8;
    s32 *arr;
} ArrHolder;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    ArrHolder *unkC;
} MidPtr;

typedef struct {
    s32 unk0;
    MidPtr *unk4;
} Src360;

typedef struct {
    Pair pair;
    s32 unk8;
    s32 unkC;
} Dst360;

void *func_00328D68(s32 size);

void func_00335210(Dst360 *a0, Src360 *a1, void *a2, s32 a3);

extern void *D_0040B420[];

extern void *D_0040B438[];

extern void *D_0040B450[];

extern void *D_0040B468[];

extern void *D_0040B480[];

extern void *D_0040B498[];

extern void *D_0040B4B0[];

extern void *D_0040B4C8[];

extern void *D_0040B4E0[];

extern void *D_0040B4F8[];

void func_003340A8(VObj *a0) {
    a0->unk0->fn();
}

void func_003340D0(Pair *a0, void *a1, void *a2) {
    a0->unk0 = a2;
    a0->unk4 = a1;
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_003340E0);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_003341B8);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00334280);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_003343C8);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_003343E8);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00334510);

void func_00334618(StateByte *a0) {
    u8 t;

    t = a0->unk30;
    if (t != 6) {
        a0->unk31 = t;
        a0->unk30 = 6;
    }
}

void func_00334638(StateByte *a0) {
    if (a0->unk30 == 6) {
        a0->unk30 = a0->unk31;
    }
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00334658);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00334670);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00334678);

f32 func_00334788(KeyOut *a0) {
    f32 t;
    f32 a;

    t = a0->f8;
    a = *a0->p0;
    return (a + (*a0->p4 * t)) - (a * t);
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_003347B0);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00334808);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00334888);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_003348D8);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00334900);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00334930);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00334980);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_003349E0);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00334A60);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00334B10);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00334B70);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00334C30);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00334D10);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00334D70);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00334DF0);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00334EA0);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00334F00);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00334F98);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00335000);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00335050);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_003350B0);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00335108);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00335160);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00335180);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_003351A0);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_003351C0);

s32 func_003351E0(void *a0, s32 a1) {
    return D_0040B3F8[(u16)a1](a0, a1);
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00335210);

void *func_00335268(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_00328D68(0x14);
    func_00335210(r, a0, D_0040B420, a2);
    return r;
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_003352C8);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00335308);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_003353B8);

void *func_003353C8(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_00328D68(0x14);
    func_00335210(r, a0, D_0040B438, a2);
    return r;
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00335428);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00335468);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00335518);

void *func_00335528(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_00328D68(0x14);
    func_00335210(r, a0, D_0040B450, a2);
    return r;
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00335588);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_003355C8);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00335678);

void *func_00335688(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_00328D68(0x14);
    func_00335210(r, a0, D_0040B468, a2);
    return r;
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_003356E8);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00335728);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_003357D8);

void *func_003357E8(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_00328D68(0x14);
    func_00335210(r, a0, D_0040B480, a2);
    return r;
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00335848);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00335888);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_003358E0);

void *func_003358F0(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_00328D68(0x24);
    func_00335210(r, a0, D_0040B498, a2);
    return r;
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00335950);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_003359A0);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00335A18);

void *func_00335A68(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_00328D68(0x24);
    func_00335210(r, a0, D_0040B4B0, a2);
    return r;
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00335AC8);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00335B18);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00335B90);

void *func_00335BE0(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_00328D68(0x14);
    func_00335210(r, a0, D_0040B4C8, a2);
    return r;
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00335C40);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00335C80);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00335D30);

void *func_00335D40(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_00328D68(0x24);
    func_00335210(r, a0, D_0040B4E0, a2);
    return r;
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00335DA0);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00335DD8);

void func_00335E10(void) {
}

void *func_00335E18(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_00328D68(0x24);
    func_00335210(r, a0, D_0040B4F8, a2);
    return r;
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00335E78);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00335EB0);
