#include "common.h"

extern char D_003B39C8[]; /* "/tool/effect/ep/" */
extern char D_003B39E0[]; /* "/tool/effect/" */
extern char D_003B3B88[]; /* "/tool/effect/mat/" */
extern char D_003B3BA0[]; /* "/tool/effect/hlp/" */
extern char D_003B3CA0[]; /* "LmapMain" */

extern s32 CancelWakeupThread(u64);

extern u64 GetThreadId(void);

extern u64 func_002CF530(u64);

extern u32 D_003BD2C8;

extern u32 func_002CD2A8(u16);

extern u32 func_002CD730(u32, u16);

extern s32 D_003BAA00;

extern s32 kwlnTaskGetTaskByName(u32);

extern u64 func_00197760(s32, s32, u64, u64, u64, u64);

extern u32 D_003BD978;

extern s32 D_003BD97C;

extern u32 D_003BD980;

extern s32 func_002C4A10(void);

extern s32 D_003BD274;

extern u32 D_003BD268;

extern u32 D_003BD25C;

extern u32 D_003BD260;

extern u32 D_003BD970;

extern u32 D_003BD974;

extern s32 func_0021F600(u32);

extern s32 func_002C2540(u32, u32);

extern s32 func_002C1B30(u32);

extern u32 func_002BD1F8(u64);

extern u64 func_002D0A48(u64);

extern u64 func_00288B88(void);

extern u32 func_002BD9C0(u64, u64);

extern u32 D_003BD11C;

extern u32 D_003BD120;

extern u32 D_003BD128;

extern u32 D_003BD158;

extern u32 D_003BD15C;

extern u32 D_003BD160;

extern u32 D_003BD124;

extern s32 D_003BD118;

extern s32 D_003BD10C;

extern s32 D_003BD110;

extern s32 D_003BD098;

extern s32 D_003BD09C;

extern u8 D_003BD955;

extern s32 D_003BD06C;

extern s32 D_003BD05C;

extern s32 func_001A17F0(void);

extern u64 func_002B3390(u32, u32, u32);

extern u64 func_002B0988(void);

extern u32 D_003BC998;

extern s32 D_003BC99C;

extern u32 D_003BC9A0;

extern u32 func_0029BF88(u32, u32);

extern u32 D_003BC988;

extern s32 D_003BC98C;

extern u32 D_003BC990;

extern u32 func_0029C230(u32);

extern u32 D_003BC968;

extern u32 func_00151E60(u32);

extern u64 func_00293400(void);

extern u32 D_003BC950;

extern u32 D_003BC954;

extern s32 *D_003BC948;

extern s64 func_001A1438(void);

extern s64 func_001A1448(void);

extern u64 func_0029A958(void);
/* 4x4 float matrix with 128-bit row access for VU0/DMA transfers. */
typedef struct Matrix4 {
    union {
        float m[4][4];
        s128 rows[4];
    } u; // 0x00
} Matrix4; // 0x40

/* Operand block used by the script VM helpers near func_002CD730 (layout inferred from field accesses). */
typedef struct ScrVmOperand {
    u8 pad_0x00[0x04]; // 0x00
    u8 b04;            // 0x04
    u8 pad_0x05[0x0F]; // 0x05
    u16 h14;           // 0x14
    u8 pad_0x16[0x3A]; // 0x16
    float f50;         // 0x50
    u8 unk54;          // 0x54
    s8 s55;            // 0x55
} ScrVmOperand; // 0x56

typedef struct IntPair {
    s32 x; // 0x00
    s32 y; // 0x04
} IntPair; // 0x08

typedef struct Vec3 {
    float x; // 0x00
    float y; // 0x04
    float z; // 0x08
} Vec3; // 0x0C

typedef struct QuadU32 {
    u32 x; // 0x00
    u32 y; // 0x04
    u32 z; // 0x08
    u32 w; // 0x0C
} QuadU32; // 0x10

typedef struct ShortPair2C {
    u8 pad_0x00[0x2C]; // 0x00
    s16 h2C;           // 0x2C
    s16 h2E;           // 0x2E
} ShortPair2C; // 0x30

extern u32 D_003BD058;

extern u32 D_0038F2FC[];

extern u8 D_00394680[];

extern s8 D_003BD270;
/* Reference-counted object header (layout inferred from field accesses). */
typedef struct RefObj {
    u8 pad_0x00[0x14]; // 0x00
    s32 cnt14;         // 0x14
    s32 unk18;         // 0x18
    s32 cnt1C;         // 0x1C
} RefObj; // 0x20

/* Battle/display work object (layout inferred from field accesses). */
typedef struct BdWork {
    s32 x0;            // 0x00
    s32 x4;            // 0x04
    u8 pad_0x08[0x58]; // 0x08
    void *x60;         // 0x60
    s32 x64;           // 0x64
} BdWork; // 0x68

/* 24-byte table entries (full layout unknown; stride inferred from index math). */
typedef struct Entry24B {
    u8 v0;            // 0x00
    u8 pad_0x01[0x17]; // 0x01
} Entry24B; // 0x18

typedef struct Entry24W {
    u32 v0;            // 0x00
    u8 pad_0x04[0x14]; // 0x04
} Entry24W; // 0x18

/* 0xAARRGGBB color split into RGB and alpha fields. */
typedef struct RgbAlpha {
    u8 pad_0x00[0x18]; // 0x00
    u32 rgb18;         // 0x18
    u8 pad_0x1C[0x1C]; // 0x1C
    u32 alpha38;       // 0x38
    u32 x3C;           // 0x3C
    u8 pad_0x40[0x10]; // 0x40
    float f50;         // 0x50
} RgbAlpha; // 0x54

extern void func_002DB538(void *, float);

extern s32 func_002C2568(s32, void *);

extern float func_002C8588(void);

extern float func_002FA1C0(float);

extern Entry24B D_00393220[];

extern Entry24W D_00393234[];
extern u32 D_003BC94C;
extern void func_002BD3D8(void *, s32, void *);

typedef struct FnTbl20 {
    void (*fn)(void *);
    u8 pad_0x04[0x10]; // 0x04
} FnTbl20; // 0x14

extern FnTbl20 D_0037E778[];

extern FnTbl20 D_0037E7F0[];

/* Function-pointer tables indexed by object fields (entry size inferred). */
typedef struct FnTbl28 {
    void (*fn)(void *);
    u8 pad_0x04[0x18]; // 0x04
} FnTbl28; // 0x1C

typedef struct FnTbl24 {
    void (*fn)(void *);
    u8 pad_0x04[0x14]; // 0x04
} FnTbl24; // 0x18

extern FnTbl28 D_0037E8B0[];

extern FnTbl24 D_0037EADC[];

extern FnTbl24 D_0037EC5C[];

extern FnTbl28 D_0037ED18[];

extern FnTbl28 D_0037ED98[];

extern FnTbl24 D_0037EE40[];

extern FnTbl24 D_0037EE44[];

extern void func_0029B168(s32 *);

extern s32 D_003BD26C;

extern void func_002C3C48(void);

extern FnTbl28 D_0037E8B4[];

extern FnTbl24 D_0037EAE0[];

extern FnTbl24 D_0037EC60[];

extern FnTbl28 D_0037ED1C[];

extern FnTbl28 D_0037EDA4[];

extern FnTbl24 D_0037EE48[];

extern void func_002CAF78(void *, void *);

extern u32 D_003DFA30[];

extern u32 D_003BD984;

extern u32 D_003DFED0[];

extern u32 D_003DFEE0[];

/* Shared work object for the func_002BA538 helpers (layout inferred). */
typedef struct BaObj {
    u8 pad_0x00[0x0C]; // 0x00
    u8 *x0C;           // 0x0C
} BaObj; // 0x10

/* 84-byte table entries (full layout unknown; stride inferred from index math). */
typedef struct Entry84W {
    u32 v0;             // 0x00
    u8 pad_0x04[0x50]; // 0x04
} Entry84W; // 0x54

extern s32 func_002BA538(void *, s32, void *, void *);

extern u8 D_0038F9D0[];

extern u8 D_0038FAD0[];

extern u8 D_0038F8E8[];

extern u8 D_0038FA20[];

extern u8 D_0038F938[];

extern void func_002B9050(void);

extern void func_002B9080(void *, void *);

extern u8 D_003BD088[];

extern u8 D_003BD090[];

extern u8 D_003DF8D0[];

extern void func_002CD630(void *, s32);

extern Entry84W D_00391230[];

extern void func_00218028(void *);

extern void kwlnTaskDestroyWithHierarchyByName(char *, s32);
/* 28-byte table entries (full layout unknown; stride inferred from index math). */
typedef struct Entry28W {
    u32 v0;             // 0x00
    u8 pad_0x04[0x18]; // 0x04
} Entry28W; // 0x1C

typedef struct Entry28B {
    u8 v0;             // 0x00
    u8 pad_0x01[0x1B]; // 0x01
} Entry28B; // 0x1C

typedef struct Entry28H {
    u16 v0;            // 0x00
    u8 pad_0x02[0x1A]; // 0x02
} Entry28H; // 0x1C

/* Copy source for func_002CF3F8 (layout inferred from field accesses). */
typedef struct CfSrc {
    u8 pad_0x00[0x04]; // 0x00
    u32 x04;           // 0x04
    u8 pad_0x08[0x1C]; // 0x08
    u32 x24;           // 0x24
    u32 x28;           // 0x28
    u8 pad_0x2C[0x10]; // 0x2C
    float f3C;         // 0x3C
} CfSrc; // 0x40

extern void *func_0029BD90(void *);

extern void func_002A6440(s32);

extern void func_002AB290(s32);

extern s32 func_002B7388(s32, void *, u32);

extern u8 D_003B3968[];

extern u8 D_003B3958[];

extern u8 D_003BCA40[];

extern u8 D_003BD050[];

extern u8 D_003B38C8[];

extern u8 D_003B3938[];

extern u8 D_003B3928[];

extern u8 D_003B3918[];

extern u8 D_003B3908[];

extern u8 D_003B38F8[];

extern u8 D_003B38E8[];

extern u8 D_003B38D8[];

extern u8 D_003B3888[];

extern u8 D_003BD000[];

extern u8 D_0038DE70[];

extern u8 D_0038DF50[];

extern u8 D_0038E0F0[];

extern u8 D_0038E1A0[];

extern u8 D_0038E280[];

extern u8 D_0038E240[];

extern u8 D_0038E310[];

extern u8 D_0038E450[];

extern u8 D_0038E500[];

extern u8 D_0038E620[];

extern u8 D_0038E6F0[];

extern u8 D_0038E7C0[];

extern u8 D_0038E800[];

extern u8 D_0038E9A0[];

extern u8 D_003DF840[];

extern u8 D_003BD108;

extern u8 D_003BD109;

extern u8 D_003BD10A;

extern s32 func_002B9320(void *, void *, s32);

extern u8 D_003DE148[];

extern Entry28W D_003907B8[];

extern Entry28B D_003907B4[];

extern Entry28H D_003907B6[];

extern Entry28B D_003907B5[];

extern Entry28W D_003907B0[];

/* Value holder with a float field and a u16 data pointer (layout inferred). */
typedef struct ValPtr44 {
    u8 pad_0x00[0x08]; // 0x00
    float f08;         // 0x08
    u8 pad_0x0C[0x38]; // 0x0C
    u16 *p44;          // 0x44
} ValPtr44; // 0x48

typedef struct ValPtr34 {
    u8 pad_0x00[0x08]; // 0x00
    float f08;         // 0x08
    u8 pad_0x0C[0x28]; // 0x0C
    u16 *p34;          // 0x34
} ValPtr34; // 0x38

extern s32 func_002BB880(char *, s32);

extern void func_0029A810(u16 *);

extern void func_00217F88(void *);

extern void func_00217FB8(void *);

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029A840);

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029A8D8);

void func_0029A938(s32 arg0) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x80) = 0;
    func_002177D0();
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029A958);

void func_0029A9F0(void *p) {
    void *q = *(void **)((s32)p + 8);
    if (q != NULL) {
        func_002CFF98(q);
    }
    if (*(s32 *)((s32)p + 4) != 0) {
        func_0029A938(*(s32 *)((s32)p + 4));
    }
    func_002CFF98(p);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029AA40);

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029AA90);

void func_0029AB28(s32 arg0) {
    func_002DB538(*(void **)(*(s32 *)(arg0 + 4) + 0x1c), 0.0f);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029AB48);

/* Pass a vector to the VU0 model helpers via vf10 (gcc cannot do this from plain C). */
void func_0029ABA0(s32 arg0, void *vec) {
    __asm__ volatile ("lqc2 $vf10, 0(%0)" :: "r" (vec));
    func_00217F88(*(void **)(arg0 + 4));
}

void func_0029ABC0(s32 arg0, void *vec) {
    __asm__ volatile ("lqc2 $vf10, 0(%0)" :: "r" (vec));
    func_00217FB8(*(void **)(arg0 + 4));
}

void func_0029ABE0(s32 arg0) {
    func_002180A8(*(u32 *)(arg0 + 4));
}

void func_0029ABF8(s32 arg0, float scale) {
    float v[3];
    float t;

    t = *(float *)arg0 * scale;
    v[0] = v[1] = v[2] = t;
    __asm__ volatile ("lqc2 $vf10, 0(%0)" :: "r" (v));
    func_00218028(*(void **)(arg0 + 4));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029AC30);

u64 func_0029AD68(void) {
    u64 temp_v0;
    s64 temp_v1;

    temp_v0 = func_0029A958();
    func_0029AC30(temp_v0);
    temp_v1 = func_001A1438();
    if ((temp_v1 != 0) && (temp_v1 = func_001A1448(), temp_v1 == 0)) {
        func_0029B108(temp_v0);
    }
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029ADC8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029AE08);

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029AE88);

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029B0B0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029B108);

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029B168);

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029B1D8);

void func_0029B270(void) {
    s32 *node = D_003BC948;
    s32 obj;
    s32 v;

    if (node != NULL) {
        do {
            obj = *node;
            v = *(s32 *)(obj + 0xC);
            if ((v & 2) != 0) {
                *(s32 *)(obj + 0xC) = v | 8;
            }
            node = *(s32 **)((s32)node + 8);
        } while (node != NULL);
    }
}

void func_0029B2B0(void) {
    s32 temp_v0;
    s32 *piVar2;

    piVar2 = D_003BC948;
    while (piVar2 != (s32 *)0x0) {
        temp_v0 = *piVar2;
        piVar2 = (s32 *)piVar2[2];
        *(u32 *)(temp_v0 + 0xc) = *(u32 *)(temp_v0 + 0xc) & 0xffffffef;
    }
}

void func_0029B2E8(void) {
    s32 temp_v0;
    s32 *piVar2;

    piVar2 = D_003BC948;
    while (piVar2 != (s32 *)0x0) {
        temp_v0 = *piVar2;
        piVar2 = (s32 *)piVar2[2];
        *(u32 *)(temp_v0 + 0xc) = *(u32 *)(temp_v0 + 0xc) | 0x10;
    }
}

void func_0029B320(void) {
    s32 *node = D_003BC948;
    s32 obj;
    s32 *next;
    s32 v;

    if (node == NULL) {
        return;
    }
    do {
        obj = *node;
        next = *(s32 **)((s32)node + 8);
        v = *(s32 *)(obj + 0xC) | 0xA;
        *(s32 *)(obj + 0xC) = v;
        func_0029B168(node);
        node = next;
    } while (node != NULL);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029B368);

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029B528);

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029B5B8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029B678);

void func_0029B7E0(s32 arg0) {
    func_002DB538(*(void **)(*(s32 *)(arg0 + 0x4C) + 0x1C), 0.0f);
    *(u32 *)(arg0 + 4) = 0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029B818);

/* Pass a vector to the VU0 model helpers via vf10 (gcc cannot do this from plain C). */
void func_0029BCF8(s32 arg0, void *vec) {
    __asm__ volatile ("lqc2 $vf10, 0(%0)" :: "r" (vec));
    func_00217F88(*(void **)(arg0 + 0x4c));
}

void func_0029BD18(s32 arg0, void *vec) {
    __asm__ volatile ("lqc2 $vf10, 0(%0)" :: "r" (vec));
    func_00217FB8(*(void **)(arg0 + 0x4c));
}

void func_0029BD38(s32 arg0) {
    func_002180A8(*(u32 *)(arg0 + 0x4c));
}

void func_0029BD50(s32 arg0, float f) {
    u32 bits;

    __asm__ volatile (
        ".set noreorder               \n"
        "vaddw.xyz vf10, vf0, vf0w    \n"
        "vmulx.w vf10, vf0, vf0x      \n"
        "mfc1 %0, $f12                \n"
        "qmtc2.ni %0, $vf2            \n"
        "vmulx.xyzw vf10, vf10, $vf2x \n"
        ".set reorder"
        : "+r" (bits) : : "memory");
    func_00218028(*(void **)(arg0 + 0x4C));
}

void func_0029BD80(void) {
    D_003BC954 = 0;
    D_003BC950 = 0xffffffff;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029BD90);

u32 func_0029BF88(u32 arg0, u32 arg1) {
    void *p;

    p = func_0029BD90((void *)arg0);
    *(u32 *)((s32)p + 0x18) = arg1;
    return (u32)p;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029BFB0);

RefObj *func_0029C028(RefObj *obj) {
    obj->cnt14++;
    D_003BC94C++;
    return obj;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029C048);

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029C1F0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029C230);

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029C378);

RefObj *func_0029C408(RefObj *obj) {
    obj->cnt1C++;
    return obj;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029C420);

void func_0029C500(s32 arg0, u32 arg1, s32 arg2) {
    func_0029C048(arg1, *(u32 *)(*(s32 *)(arg2 + 0xc) * 4 + *(s32 *)(arg0 + 0x14)));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029C530);

void func_0029C570(void) {
    u64 temp_v0;

    temp_v0 = func_00293400();
    func_0029C530(temp_v0);
}

void func_0029C590(u32 arg0) {
    func_00105888();
    func_002CFF98(arg0);
}

void func_0029C5B8(s32 arg0) {
    func_0029C530(arg0 + 4);
}

void func_0029C5D0(u32 *arg0) {
    *arg0 = 0;
}

void func_0029C5D8(s32 *arg0) {
    s32 temp_v0;

    temp_v0 = *arg0;
    if (temp_v0 == 0) {
        func_00105828(arg0[1], arg0[2]);
        temp_v0 = *arg0;
    }
    *arg0 = temp_v0 + 1;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029C620);

void func_0029C6F0(void) {
    u64 temp_v0;

    temp_v0 = func_00293400();
    func_0029C620(temp_v0);
}

void func_0029C710(void) {
    func_002CFF98();
}

void func_0029C728(s32 arg0) {
    func_0029C620(arg0 + 0x18);
}

void func_0029C740(s32 arg0) {
    *(u32 *)(arg0 + 0x10) = 0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029C748);

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029CDE0);

void func_0029CDF0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

void func_0029CDF8(s32 arg0) {
    u32 temp_v0;
    u32 *puVar2;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *(u32 *)(arg0 + 0x4c);
    puVar2 = *(u32 **)(arg0 + 8);
    if (temp_v0 != 0) {
        do {
            temp_v1 = temp_v1 + 1;
            *puVar2 = 0xffffffff;
            puVar2 = puVar2 + 2;
        } while (temp_v1 < temp_v0);
    }
    memset(*(u32 *)(arg0 + 0x10), 0, temp_v0 << 3);
}

void func_0029CE50(void *arg0) {
    func_002CEAE8();
}

void func_0029CE68(void) {
    func_002CEC08();
}

void func_0029CE80(void) {
    func_002CEC28();
}

void func_0029CE98(s32 arg0) {
    func_0029CE50((void *)(arg0 + 0x14));
}

void func_0029CEB0(s32 *p) {
    func_0029CDF8((s32)p);
    *p = 0;
}

void func_0029CED8(s32 arg0) {
    func_002CEC40();
}

void func_0029CEF0(s32 arg0) {
    func_002CF248();
}

void func_0029CF08(s32 arg0) {
    func_0029CED8(arg0);
    func_0029CEF0(arg0);
}

void func_0029CF30(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 4) = arg1;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029CF38);

void func_0029CF88(void) {
    u64 temp_v0;

    temp_v0 = func_00293400();
    func_0029CF38(temp_v0);
}

void func_0029CFA8(u32 arg0) {
    func_0010A158();
    func_002CFF98(arg0);
}

void func_0029CFD0(s32 arg0) {
    func_0029CF38(arg0 + 4);
}

void func_0029CFE8(u32 *arg0) {
    *arg0 = 0;
}

void func_0029CFF0(s32 *arg0) {
    s32 temp_v0;

    temp_v0 = *arg0;
    if (temp_v0 == 0) {
        func_00109C10(arg0[1], (s16)arg0[2], arg0[3], arg0[4]);
        temp_v0 = *arg0;
    }
    *arg0 = temp_v0 + 1;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029D040);

void func_0029D1B8(s32 arg0) {
    func_00186C18(arg0 + 0xc0);
}

void func_0029D1D0(void) {
    func_00186CB8();
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029D1E8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029D3F0);

void func_0029D450(s32 arg0) {
    func_00186F90(arg0 + 0xc0);
}

void func_0029D468(void) {
    func_00187080();
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029D480);

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029D650);

void func_0029D6B0(s32 arg0) {
    func_00187460(arg0 + 0xc0);
}

void func_0029D6C8(void) {
    func_00187580();
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029D6E0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029D8B0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029D910);

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029DA70);

void func_0029DB78(s32 arg0) {
    func_00187FC0(arg0 + 0xc0);
}

void func_0029DB90(void) {
    func_00188050();
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029DBA8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029DD70);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2AC0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029DDD0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029DF00);

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029DFE0);

void func_0029E058(s32 arg0) {
    func_0029DDD0(*(u16 *)(arg0 + 0x1c), *(u32 *)(arg0 + 0x28));
}

void func_0029E078(s32 arg0) {
    *(u32 *)(arg0 + 0x20) = 0;
}

void func_0029E080(s32 arg0) {
    D_0037E778[*(s32 *)(arg0 + 0x1C)].fn((void *)arg0);
    *(s32 *)(arg0 + 0x20) = *(s32 *)(arg0 + 0x20) + 1;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029E0D0);

void func_0029E0E0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

void func_0029E0E8(Matrix4 *mat, float value) {
    mat->u.m[1][2] = value;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029E0F0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029E220);

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029E300);

void func_0029E378(s32 arg0) {
    func_0029E0F0(*(u16 *)(arg0 + 0x1c), *(u32 *)(arg0 + 0x28));
}

void func_0029E398(s32 arg0) {
    *(u32 *)(arg0 + 0x20) = 0;
}

void func_0029E3A0(s32 arg0) {
    D_0037E7F0[*(s32 *)(arg0 + 0x1C)].fn((void *)arg0);
    *(s32 *)(arg0 + 0x20) = *(s32 *)(arg0 + 0x20) + 1;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029E3F0);

void func_0029E400(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

void func_0029E408(Matrix4 *mat, float value) {
    mat->u.m[1][2] = value;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029E410);

void func_0029E4C0(u32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)((s32)arg0 + 100);
    if (temp_v0 != 0) {
        func_00151F00(temp_v0);
    }
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029E4F8);

void func_0029E5B0(s32 arg0, s32 arg1) {
    u32 temp_v0;

    if (*(s32 *)(arg0 + 100) != 0) {
        func_00151F00(*(s32 *)(arg0 + 100));
    }
    temp_v0 = func_00151E60(*(u32 *)(arg1 + 100));
    *(u32 *)(arg0 + 100) = temp_v0;
}

void func_0029E600(s32 arg0) {
    func_001522E8(*(u32 *)(arg0 + 100), 0);
    *(u32 *)(arg0 + 0x28) = 0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029E630);

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029E728);

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029E738);

void func_0029E750(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_0029E758(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029E760);

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029E7D8);

void func_0029E868(s32 arg0) {
    func_002A3CB0(*(u32 *)(arg0 + 4));
    func_002D0918(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029E898);

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029EEC8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029F030);

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029F0A8);

void func_0029F138(s32 arg0) {
    func_002A3CB0(*(u32 *)(arg0 + 4));
    func_002D0918(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029F168);

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029F8A0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029FA08);

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029FA80);

void func_0029FB18(s32 arg0) {
    func_002A3CB0(*(u32 *)(arg0 + 4));
    func_002D0918(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029FB48);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A0260);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A03C8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A0440);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A04B8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A0548);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A05A8);

void func_002A0608(s32 arg0) {
    func_002A3CB0(*(u32 *)(arg0 + 4));
    func_002D0918(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A0638);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A0BE0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A0D48);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A0DC0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A0E20);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A0EB0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A0F10);

void func_002A0F70(s32 arg0) {
    func_002A3CB0(*(u32 *)(arg0 + 4));
    func_002D0918(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A0FA0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A1588);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A16F0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A1768);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A17C8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A1858);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A18B8);

void func_002A1918(s32 arg0) {
    func_002A3CB0(*(u32 *)(arg0 + 4));
    func_002D0918(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A1948);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A2008);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A2170);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A21E8);

void func_002A2288(s32 arg0) {
    func_002A3CB0(*(u32 *)(arg0 + 4));
    func_002D0918(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A22B8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A2A60);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A2BC8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A2C40);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A2C98);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A2D28);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A2D88);

void func_002A2DE8(s32 arg0) {
    func_002A3CB0(*(u32 *)(arg0 + 4));
    func_002D0918(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A2E18);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A34D8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A3640);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A36F8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A3798);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A37F8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A3840);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A3910);

void func_002A3958(s32 arg0) {
    D_0037E8B0[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
    *(s32 *)(arg0 + 0x28) = *(s32 *)(arg0 + 0x28) + 1;
}

void func_002A39A8(s32 arg0) {
    D_0037E8B4[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
}

void func_002A39E0(u32 arg0) {
    func_002A3958(arg0);
    func_002A39A8(arg0);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A3A08);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A3A18);

void func_002A3A30(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002A3A38(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2B10);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A3A40);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A3BD8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A3CB0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A3D68);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A3E10);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A4318);

u32 func_002A4378(s32 arg0) {
    return (&D_003BC968)[arg0];
}

void func_002A4390(s32 arg0) {
    *(u32 *)(**(s32 **)(arg0 + 0x30) + 4) = 0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A43A0);

void func_002A4448(u32 arg0) {
    func_002A5610(*(u32 *)arg0);
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A4478);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A46E0);

void func_002A4850(s32 arg0) {
    u32 temp_v0;

    temp_v0 = *(u32 *)(*(s32 *)(arg0 + 0x30) + 4);
    *(u32 *)(*(s32 *)(*(s32 *)(arg0 + 0x30) + 8) + 4) = 0;
    func_002A5410(temp_v0);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A4878);

void func_002A4AB8(s32 arg0) {
    func_002A3CB0(*(u32 *)(arg0 + 8));
    func_002A53A8(*(u32 *)(arg0 + 4));
    func_002D0918(*(u32 *)(arg0 + 0xc));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A4AF0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A4C40);

void func_002A4DE8(s32 arg0) {
    *(u32 *)(**(s32 **)(arg0 + 0x30) + 4) = 0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A4DF8);

void func_002A4EA0(u32 arg0) {
    func_002A5610(*(u32 *)arg0);
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A4ED0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A5118);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A5288);

void func_002A5378(s32 arg0) {
    u64 temp_v0;

    temp_v0 = func_00293400();
    func_002A5288(*(u16 *)(arg0 + 0xc), temp_v0);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A53A8);

void func_002A53F0(s32 arg0) {
    func_002A5288(*(u16 *)(arg0 + 0x2c), *(u32 *)(arg0 + 0x34));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A5410);

void func_002A5458(s32 arg0) {
    D_0037EADC[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
    *(s32 *)(arg0 + 0x28) = *(s32 *)(arg0 + 0x28) + 1;
}

void func_002A54A8(s32 arg0) {
    D_0037EAE0[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
}

void func_002A54E0(u32 arg0) {
    func_002A5458(arg0);
    func_002A54A8(arg0);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A5508);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A5518);

void func_002A5530(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002A5538(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A5540);

void func_002A5610(s32 arg0) {
    func_002DAA68(*(u32 *)(arg0 + 0x18));
    func_002D0918(*(u32 *)(arg0 + 0x1c));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A5640);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A5A00);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A5A58);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A5A90);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A5B08);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A5BD8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A5CF0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A5D60);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A5DE0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A5F80);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A60C0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A6120);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A6198);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A6218);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A6298);

void func_002A6390(s32 arg0, u32 arg1) {
    u32 temp_v0;

    if (*(s32 *)(arg0 + 0x40) != 0) {
        func_0029C378(*(s32 *)(arg0 + 0x40));
    }
    temp_v0 = func_0029C230(arg1);
    *(u32 *)(arg0 + 0x40) = temp_v0;
}

void func_002A63E0(s32 arg0) {
    if (*(s32 *)(arg0 + 0x44) != 0) {
        func_0029A748(*(s32 *)(arg0 + 0x44));
        return;
    }
}

void func_002A6410(s32 arg0) {
    if (*(s32 *)(arg0 + 0x44) != 0) {
        func_0029A750(*(s32 *)(arg0 + 0x44));
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A6440);

void func_002A70B0(s32 arg0) {
    func_002A6410(arg0);
    func_002A6440(arg0);
}

void func_002A70D8(s32 arg0) {
    func_0029A7C8(*(u32 *)(arg0 + 0x44));
}

void func_002A70F0(s32 arg0) {
    func_0029A7F8(*(u32 *)(arg0 + 0x44));
}

void func_002A7108(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 4) = arg1;
}

void func_002A7110(ValPtr44 *p, float v) {
    p->f08 = v;
    func_0029A810(p->p44);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A7130);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A7288);

void func_002A74B0(s32 arg0) {
    func_002DAA68(*(u32 *)(arg0 + 0x2c));
    func_002D0918(*(u32 *)(arg0 + 0x38));
}

void func_002A74E0(s32 arg0) {
    *(u32 *)(arg0 + 0x14) = 0;
    *(u32 *)(arg0 + 0x18) = 3;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A74F0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A7568);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A7610);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A7890);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A78E0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A7B30);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A7B68);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A7DF8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A7E58);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A7F78);

void func_002A8018(s32 arg0) {
    *(u32 *)(arg0 + 0x2c) = 0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A8020);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A85B0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A85C0);

void func_002A85D8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002A85E0(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A85E8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A8670);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A8A20);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A8A90);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A9160);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A9340);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A93A0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A95C0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A9630);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A9690);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A9860);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A9978);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A9D38);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A9DA8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AA748);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AA928);

void func_002AAA18(s32 arg0) {
    u64 temp_v0;

    temp_v0 = func_00293400();
    func_002AA928(*(u16 *)(arg0 + 0xc), temp_v0);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AAA48);

void func_002AAA90(s32 arg0) {
    func_002AA928(*(u16 *)(arg0 + 0x2c), *(u32 *)(arg0 + 0x34));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AAAB0);

void func_002AAAF8(s32 arg0) {
    D_0037EC5C[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
    *(s32 *)(arg0 + 0x28) = *(s32 *)(arg0 + 0x28) + 1;
}

void func_002AAB48(s32 arg0) {
    D_0037EC60[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
}

void func_002AAB80(u32 arg0) {
    func_002AAAF8(arg0);
    func_002AAB48(arg0);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AABA8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AABB8);

void func_002AABD0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002AABD8(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AABE0);

void func_002AACD0(s32 arg0) {
    func_002DAA68(*(u32 *)(arg0 + 0x18));
    func_002D0918(*(u32 *)(arg0 + 0x1c));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AAD00);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AAF70);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AAFF0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AB028);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AB0D0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AB130);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AB1D0);

void func_002AB230(s32 arg0) {
    if (*(s32 *)(arg0 + 0x34) != 0) {
        func_0029A748(*(s32 *)(arg0 + 0x34));
        return;
    }
}

void func_002AB260(s32 arg0) {
    if (*(s32 *)(arg0 + 0x34) != 0) {
        func_0029A750(*(s32 *)(arg0 + 0x34));
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AB290);

void func_002AB968(s32 arg0) {
    func_002AB260(arg0);
    func_002AB290(arg0);
}

void func_002AB990(s32 arg0) {
    func_0029A7C8(*(u32 *)(arg0 + 0x34));
}

void func_002AB9A8(s32 arg0) {
    func_0029A7F8(*(u32 *)(arg0 + 0x34));
}

void func_002AB9C0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 4) = arg1;
}

void func_002AB9C8(ValPtr34 *p, float v) {
    p->f08 = v;
    func_0029A810(p->p34);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AB9E8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002ABA38);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002ABAA8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002ABCF0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002ABD50);

void func_002ABDB0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x30);
    func_002AE3C8(*(u32 *)(temp_v0 + 4));
    func_002D0918(*(u32 *)(temp_v0 + 8));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002ABDE0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AC4E0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AC648);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AC698);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AC708);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AC950);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AC9B0);

void func_002ACA10(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x30);
    func_002AE3C8(*(u32 *)(temp_v0 + 4));
    func_002D0918(*(u32 *)(temp_v0 + 8));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002ACA40);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AD150);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AD2B8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AD308);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AD380);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AD5C8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AD628);

void func_002AD688(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x30);
    func_002AE3C8(*(u32 *)(temp_v0 + 4));
    func_002D0918(*(u32 *)(temp_v0 + 8));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AD6B8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002ADCE0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002ADE48);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002ADF00);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002ADFA0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AE000);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AE048);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AE0D8);

void func_002AE120(s32 arg0) {
    D_0037ED18[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
    *(s32 *)(arg0 + 0x28) = *(s32 *)(arg0 + 0x28) + 1;
}

void func_002AE170(s32 arg0) {
    D_0037ED1C[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
}

void func_002AE1A8(u32 arg0) {
    func_002AE120(arg0);
    func_002AE170(arg0);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AE1D0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AE1E0);

void func_002AE1F8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002AE200(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AE208);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AE350);

void func_002AE3C8(s32 arg0) {
    if (*(s32 *)(arg0 + 0x18) == 0) {
        D_003BC98C = D_003BC98C - 1;
        if (D_003BC98C == 0) {
            func_0029BFB0(D_003BC990);
            D_003BC990 = 0;
        }
    }
    else {
        func_0029BFB0(*(s32 *)(arg0 + 0x18));
    }
    func_002DAA68(*(u32 *)(arg0 + 0x2c));
    func_002D0918(*(u32 *)(arg0 + 0x30));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AE430);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AE498);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AE8A8);

u32 func_002AE8D8(void) {
    return D_003BC988;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AE8E0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AE930);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AE9A0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AEB80);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AEBD0);

void func_002AEC30(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x30);
    func_002B0B08(*(u32 *)(temp_v0 + 4));
    func_002D0918(*(u32 *)(temp_v0 + 8));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AEC60);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AF370);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AF4D8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AF560);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AF588);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AF5E8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AF640);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AF680);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AF6F8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AFD10);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AFD78);

void func_002AFDE0(s32 arg0) {
    *(u32 *)(*(s32 *)(*(s32 *)(arg0 + 0x30) + 4) + 8) = 0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AFDF0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AFE68);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B02B0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B0300);

void func_002B0360(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x30);
    func_002B0B08(*(u32 *)(temp_v0 + 4));
    func_002D0918(*(u32 *)(temp_v0 + 8));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B0390);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B0408);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B0598);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B0650);

void func_002B06E0(s32 arg0) {
    u64 temp_v0;

    temp_v0 = func_00293400();
    func_002B0650(*(u16 *)(arg0 + 0xc), temp_v0);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B0710);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B0758);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B07E8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B0830);

void func_002B0880(s32 arg0) {
    D_0037EDA4[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
}

void func_002B08B8(u32 arg0) {
    func_002B0830(arg0);
    func_002B0880(arg0);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B08E0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B08F0);

void func_002B0908(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002B0910(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

u32 func_002B0918(void) {
    if (D_003BC99C == 0) {
        D_003BC9A0 = func_0029BF88(D_003BC998, 0x200);
    }
    D_003BC99C = D_003BC99C + 1;
    return D_003BC9A0;
}

void func_002B0958(s32 arg0) {
    D_003BC99C = D_003BC99C - 1;
    if (D_003BC99C == 0) {
        func_0029BFB0(D_003BC9A0);
        D_003BC9A0 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B0988);

u64 func_002B0AD8(void) {
    u64 temp_v0;

    temp_v0 = func_002B0988();
    func_002B0918();
    return temp_v0;
}

void func_002B0B08(s32 arg0) {
    func_002B0958(D_003BC9A0);
    func_002DAA68(*(s32 *)(arg0 + 0x2C));
    func_002D0918(*(s32 *)(arg0 + 0x30));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B0B40);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B0B70);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B1170);

u32 func_002B11A0(void) {
    return D_003BC998;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B11A8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B12D8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B14E8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B1560);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B1D68);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B1F58);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B2088);

void func_002B20D0(s32 arg0) {
    D_0037EE40[*(s32 *)(arg0 + 0x2C)].fn((void *)*(s32 *)(arg0 + 0x38));
    func_0029A938(*(s32 *)(arg0 + 0x30));
    func_002CFF98((void *)arg0);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B2120);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B21F0);

void func_002B2238(s32 arg0) {
    D_0037EE44[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
    *(s32 *)(arg0 + 0x28) = *(s32 *)(arg0 + 0x28) + 1;
}

void func_002B2288(s32 arg0) {
    D_0037EE48[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
}

void func_002B22C0(u32 arg0) {
    func_002B2238(arg0);
    func_002B2288(arg0);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B22E8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B22F8);

void func_002B2310(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002B2318(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B2320);

void func_002B2410(s32 arg0) {
    func_002DAA68(*(u32 *)(arg0 + 0x18));
    func_002D0918(*(u32 *)(arg0 + 0x1c));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B2440);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B26B0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B27B8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B2938);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B2A48);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B2E98);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B2F20);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B3090);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B3130);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B3178);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B3350);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B3390);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B3420);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B3588);

u64 func_002B35C8(s32 arg0) {
    u64 temp_v0;

    temp_v0 = func_002B3390(*(u32 *)(arg0 + 0x38), **(u32 **)(arg0 + 0x30),
                                                (*(u32 **)(arg0 + 0x30))[1]);
    func_002B3420(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B3608);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B3698);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B3AC0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B3BB8);

void func_002B3C20(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    if ((*(u32 *)(temp_v0 + 500) & 0x6000000) != 0) {
        func_001EFD58(temp_v0 + 0x50, temp_v0 + 0x60, 0);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B3C68);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B3E80);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B3EC8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B4068);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B40A0);

void func_002B40E0(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    if ((*(u32 *)(temp_v0 + 500) & 0x6000000) != 0) {
        func_002B3E80();
        return;
    }
}

void func_002B4118(s32 arg0) {
    s32 temp_v0;
    u32 temp_v1;
    u32 *puVar3;
    u8 *puVar4;
    u32 temp_v2;

    temp_v2 = 0;
    func_001A17F0();
    temp_v0 = *(s32 *)(arg0 + 0x28);
    puVar4 = (u8 *)(*(s32 *)(arg0 + 0x38) + 0x18);
    puVar3 = (u32 *)(*(s32 *)(arg0 + 0x38) + 0x10);
    do {
        temp_v1 = puVar3[-4];
        if (temp_v1 < *puVar3) {
            return;
        }
        if (temp_v0 == 0) {
            func_002B40A0(temp_v2, *puVar4, puVar3[-2]);
            temp_v1 = puVar3[-4];
        }
        if ((temp_v1 != 0) && (temp_v0 == temp_v1 - *puVar3)) {
            func_002B4068(temp_v2, *puVar4, *puVar3);
        }
        temp_v2 = temp_v2 + 1;
        puVar4 = puVar4 + 1;
        puVar3 = puVar3 + 1;
    } while (temp_v2 < 2);
}

void func_002B41D8(void) {
    func_001EF9D8(0xc);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B41F0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B4270);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B4328);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B43E0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B4498);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B44E0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B4540);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B4618);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B4678);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B46E0);

void func_002B4738(u32 arg0) {
    func_002B4678();
    func_002B46E0(arg0);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B4760);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B4770);

void func_002B4788(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002B4790(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B4798);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B4998);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B49E8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B4B08);

void func_002B4B98(s32 arg0) {
    *(u32 *)(arg0 + 8) = 0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B4BA0);

void func_002B5128(u32 *arg0, u32 arg1) {
    *arg0 = arg1;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B5130);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B5160);

void func_002B51A8(void) {
}

void func_002B51B0(void) {
    func_002B8DF0();
    func_002BC510();
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B51D0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B51E8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B5290);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B5300);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B5390);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B5480);

void func_002B5548(u32 arg0) {
    if (D_003BD05C == 0) {
        func_002B5300();
        func_002936A8(arg0);
        return;
    }
}

void func_002B5590(u32 arg0) {
    if (D_003BD05C != 0) {
        func_002944D8(D_003BD05C);
        D_003BD05C = 0;
    }
    if (D_003BD06C != 0) {
        func_002934C0(D_003BD06C);
        D_003BD06C = 0;
    }
    func_002934C0(arg0);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B55E8);

void func_002B5698(void) {
    func_002B9050();
    func_002B9080(D_003BD088, D_003DF8D0);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B56C8);

void func_002B5788(void) {
    func_002B9050();
    func_002B9080(D_003BD090, D_003DF8D0);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B57B8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B5878);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B5908);

u32 func_002B5990(void) {
    return D_0038F2FC[0] + D_003BD058;
}

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2D20);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2D30);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2D40);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2D50);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2D60);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2D70);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2D80);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2D90);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2DA0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2DB0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2DC0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2DD0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2DE0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2DF0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2E00);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2E10);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2E20);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2E30);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2E40);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2E50);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2E60);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2E70);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2E80);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2E90);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2EB0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2ED0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2EF0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2F10);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2F30);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2F50);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2F70);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2F90);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2FB0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2FD0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2FF0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3010);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3030);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3050);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3070);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3090);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B30B0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B30D0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B30F0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3110);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3130);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3150);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3170);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3190);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B31B0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B31D8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B31F8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3218);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3238);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3258);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3278);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3298);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B32B8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B32D8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B32F8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3318);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3338);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3358);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3378);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3398);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B33B8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B33D8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B33F8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3418);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3438);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3458);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3478);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3498);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B34B8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B34D8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B34F8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3518);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3538);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3558);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3578);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3598);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B35B8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B35D8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B35F8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3618);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3638);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3658);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3678);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3698);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B36B8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B36D8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B36F8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3718);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3738);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3758);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3778);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3798);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B37B8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B37C8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B37D8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B37E8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B37F8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3808);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3818);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3828);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3838);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3848);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3858);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3868);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3878);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3888);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3898);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B38A8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B38B8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B38C8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B38D8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B38E8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B38F8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3908);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3918);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3928);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3938);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3948);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3958);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3968);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3978);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3988);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3998);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B39A8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B39B8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B39C8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B39E0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B39F0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B59A8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B65C0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B6778);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B6AB8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B6B40);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B6B98);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B6C00);

u32 func_002B6FD8(void) {
    D_003BD955 = 1;
    func_002B7000(0);
    return 0;
}

u32 func_002B7000(void) {
    if (*(s32 *)(D_003BD098 + 0x34) != 0) {
        D_003BD09C = *(s32 *)(D_003BD098 + 0x34);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B7020);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B71D0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B7388);

s32 func_002B7718(void) {
    return func_002B7388((s32)D_003B3968, D_0038DE70, 8);
}

s32 func_002B7740(void) {
    return func_002B7388((s32)D_003B3958, D_0038DF50, 6);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B7768);

s32 func_002B7790(void) {
    return func_002B7388((s32)D_003BCA40, D_0038E0F0, 6);
}

s32 func_002B77B8(void) {
    return func_002B7388((s32)D_003BD050, D_0038E1A0, 3);
}

s32 func_002B77E0(void) {
    return func_002B7388((s32)D_003B38C8, D_0038E280, 5);
}

s32 func_002B7808(void) {
    return func_002B7388((s32)D_003B3938, D_0038E240, 2);
}

s32 func_002B7830(void) {
    return func_002B7388((s32)D_003B3928, D_0038E310, 0xB);
}

s32 func_002B7858(void) {
    return func_002B7388((s32)D_003B3918, D_0038E450, 6);
}

s32 func_002B7880(void) {
    return func_002B7388((s32)D_003B3908, D_0038E500, 4);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B78A8);

s32 func_002B78D0(void) {
    return func_002B7388((s32)D_003B38F8, D_0038E620, 7);
}

s32 func_002B78F8(void) {
    return func_002B7388((s32)D_003B38E8, D_0038E6F0, 5);
}

s32 func_002B7920(void) {
    return func_002B7388((s32)D_003B38D8, D_0038E7C0, 2);
}

s32 func_002B7948(void) {
    return func_002B7388((s32)D_003B3888, D_0038E800, 0xA);
}

s32 func_002B7970(void) {
    return func_002B7388((s32)D_003BD000, D_0038E9A0, 2);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B7998);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B7A28);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3A90);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3AA8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B7B78);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B7E28);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B7E60);

void func_002B80E0(void) {
    D_003BD108 = D_003DF840[0x88];
    D_003BD109 = D_003DF840[0x89];
    D_003BD10A = D_003DF840[0x8A];
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B8108);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B8618);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B8648);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B8948);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B8B48);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B8BF0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B8DF0);

void func_002B8E60(void) {
    if (D_003BD110 != 0) {
        func_001FBEE8(D_003BD110);
        D_003BD110 = 0;
    }
    if (D_003BD10C != 0) {
        func_001FB870(D_003BD10C);
        D_003BD10C = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B8EA8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B8F70);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B8FE0);

void func_002B9050(void) {
    if (D_003BD118 != 0) {
        func_001FC2E8(D_003BD118);
        D_003BD118 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B9080);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B90D0);

void func_002B9188(void) {
    if (D_003BD110 != 0) {
        func_001FBEE8(D_003BD110);
        D_003BD110 = 0;
    }
    if (D_003BD10C != 0) {
        func_001FB870(D_003BD10C);
        D_003BD10C = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B91D0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B92F0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B9320);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BA058);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BA0C0);

void func_002BA128(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_002B9320(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0xb8));
}

s32 func_002BA148(void) {
    return func_002B9320(D_003DE148, D_003DE148 + 0x24, *(s32 *)(D_003DE148 + 0x34));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BA170);

void func_002BA1D0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_002B9320(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x34));
}

void func_002BA1F0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_002B9320(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x38));
}

void func_002BA210(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_002B9320(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x34));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BA230);

void func_002BA290(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_002B9320(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x34));
}

void func_002BA2B0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_002B9320(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x34));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BA2D0);

void func_002BA338(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_002B9320(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x34));
}

void func_002BA358(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_002B9320(temp_v0 + 0x3c, temp_v0 + 0x60, *(u32 *)(temp_v0 + 0x80));
}

void func_002BA380(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_002B9320(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x34));
}

void func_002BA3A0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_002B9320(temp_v0 + 0x3c, temp_v0 + 0x60, *(u32 *)(temp_v0 + 0x80));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BA3C8);

void func_002BA430(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_002B9320(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x70));
}

void func_002BA450(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_002B9320(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x34));
}

void func_002BA470(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_002B9320(temp_v0 + 0x50, temp_v0 + 0x74, *(u32 *)(temp_v0 + 0x84));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BA498);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BA4F8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BA538);

s32 func_002BAE18(BaObj *p) {
    return func_002BA538(p->x0C + 0x60, *(s32 *)(p->x0C + 0xB8), D_0038F9D0, D_0038F8E8);
}

s32 func_002BAE48(BaObj *p) {
    return func_002BA538(p->x0C + 0x8C, *(s32 *)(p->x0C + 0xB8), D_0038FAD0, D_0038FA20);
}

s32 func_002BAE78(BaObj *p) {
    return func_002BA538(p->x0C + 0x7C, *(s32 *)(p->x0C + 0xD4), D_0038F9D0, D_0038F8E8);
}

s32 func_002BAEA8(BaObj *p) {
    return func_002BA538(p->x0C + 0xA8, *(s32 *)(p->x0C + 0xD4), D_0038FAD0, D_0038FA20);
}

s32 func_002BAED8(BaObj *p) {
    return func_002BA538(p->x0C + 0x34, *(s32 *)(p->x0C + 0xB8), D_0038F9D0, D_0038F8E8);
}

s32 func_002BAF08(BaObj *p) {
    return func_002BA538(p->x0C + 0x60, *(s32 *)(p->x0C + 0xB8), D_0038F9D0, D_0038F8E8);
}

s32 func_002BAF38(BaObj *p) {
    return func_002BA538(p->x0C + 0x8C, *(s32 *)(p->x0C + 0xB8), D_0038F9D0, D_0038F8E8);
}

s32 func_002BAF68(BaObj *p) {
    return func_002BA538(p->x0C + 0x34, *(s32 *)(p->x0C + 0x8C), D_0038F9D0, D_0038F8E8);
}

s32 func_002BAF98(BaObj *p) {
    return func_002BA538(p->x0C + 0x60, *(s32 *)(p->x0C + 0x8C), D_0038F9D0, D_0038F8E8);
}

s32 func_002BAFC8(BaObj *p) {
    return func_002BA538(p->x0C + 0x80, *(s32 *)(p->x0C + 0xD8), D_0038F9D0, D_0038F8E8);
}

s32 func_002BAFF8(BaObj *p) {
    return func_002BA538(p->x0C + 0xAC, *(s32 *)(p->x0C + 0xD8), D_0038FAD0, D_0038FA20);
}

s32 func_002BB028(BaObj *p) {
    return func_002BA538(p->x0C + 0x90, *(s32 *)(p->x0C + 0x80), D_0038F9D0, D_0038F938);
}

s32 func_002BB058(BaObj *p) {
    return func_002BA538(p->x0C + 0x9C, *(s32 *)(p->x0C + 0xF4), D_0038F9D0, D_0038F8E8);
}

s32 func_002BB088(BaObj *p) {
    return func_002BA538(p->x0C + 0xC8, *(s32 *)(p->x0C + 0xF4), D_0038FAD0, D_0038FA20);
}

s32 func_002BB0B8(BaObj *p) {
    return func_002BA538(p->x0C, *(s32 *)(p->x0C + 0xF4), D_0038F9D0, D_0038F938);
}

s32 func_002BB0E8(BaObj *p) {
    return func_002BA538(p->x0C + 0x34, *(s32 *)(p->x0C + 0x70), D_0038F9D0, D_0038F938);
}

s32 func_002BB118(BaObj *p) {
    return func_002BA538(p->x0C + 0x34, *(s32 *)(p->x0C + 0x8C), D_0038F9D0, D_0038F8E8);
}

s32 func_002BB148(BaObj *p) {
    return func_002BA538(p->x0C + 0x60, *(s32 *)(p->x0C + 0x8C), D_0038F9D0, D_0038F8E8);
}

void func_002BB178(void) {
    D_003BD124 = 1;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BB188);

void func_002BB6B0(s32 arg0) {
    func_002BB188(*(s32 *)(arg0 + 0xc) + 0x3c, *(u32 *)(*(s32 *)(arg0 + 0xc) + 0x4c));
}

void func_002BB6D0(s32 arg0) {
    func_002BB188(*(s32 *)(arg0 + 0xc) + 0x60, *(s32 *)(*(s32 *)(arg0 + 0xc) + 0x3c) + 1);
}

void func_002BB6F8(s32 arg0) {
    func_002BB188(*(s32 *)(arg0 + 0xc) + 0x70, *(s32 *)(*(s32 *)(arg0 + 0xc) + 0x8c) + 1);
}

void func_002BB720(s32 arg0) {
    func_002BB188(*(s32 *)(arg0 + 0xc) + 0x70, *(s32 *)(*(s32 *)(arg0 + 0xc) + 0x8c) + 1);
}

void func_002BB748(s32 arg0) {
    func_002BB188(*(s32 *)(arg0 + 0xc) + 0x60, *(s32 *)(*(s32 *)(arg0 + 0xc) + 0x74) + 1);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BB770);

void func_002BB838(void) {
    func_002BB770(0x4b);
}

void func_002BB850(void) {
    func_002BB770(0x4b);
}

void func_002BB868(void) {
    func_002BB770(0xb);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BB880);

s32 func_002BB930(void) {
    return func_002BB880(D_003B3B88, 0x43);
}

s32 func_002BB950(void) {
    return func_002BB880(D_003B3B88, 0x43);
}

s32 func_002BB970(void) {
    return func_002BB880(D_003B39C8, 0x20);
}

s32 func_002BB990(void) {
    return func_002BB880(D_003B39E0, 0x10);
}

s32 func_002BB9B0(void) {
    return func_002BB880(D_003B39C8, 0x20);
}

s32 func_002BB9D0(void) {
    return func_002BB880(D_003B3B88, 1);
}

s32 func_002BB9F0(void) {
    return func_002BB880(D_003B3B88, 1);
}

s32 func_002BBA10(void) {
    return func_002BB880(D_003B3B88, 1);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BBA30);

s32 func_002BBA68(void) {
    return func_002BB880(D_003B3BA0, 4);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BBA88);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BBC70);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BBF58);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3B18);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3B28);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3B38);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3B48);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3B58);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3B68);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3B78);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3B88);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3BA0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BC140);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BC328);

void func_002BC510(void) {
    D_003BD11C = 0;
    D_003BD124 = 1;
    D_003BD120 = 0;
    D_003BD128 = 0;
    D_003BD158 = 0;
    D_003BD15C = 0;
    D_003BD160 = 0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BC538);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BC5C0);

void func_002BC618(void) {
    func_002CFF98();
}

u32 func_002BC630(u32 *arg0) {
    return *arg0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BC638);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BC6F0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BC748);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BC8F0);

void func_002BC978(u64 arg0, u32 *arg1) {
    u64 temp_v0;
    u32 temp_v1;

    temp_v0 = func_00288B88();
    temp_v1 = func_002BD9C0(temp_v0, 0);
    *arg1 = temp_v1;
    func_002D0918(temp_v0);
    func_002887A0(arg0);
}

void func_002BC9D0(u64 arg0, u32 *arg1) {
    u64 temp_v0;
    u32 temp_v1;

    temp_v0 = func_00288B88();
    temp_v1 = func_002BD9C0(temp_v0, 1);
    *arg1 = temp_v1;
    func_002887A0(arg0);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BCA18);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BCAB0);

void func_002BCB18(u64 arg0, u32 *arg1) {
    u64 temp_v0;
    u64 temp_v1;
    u32 temp_v2;

    temp_v0 = func_00288B88();
    temp_v1 = func_002D0A48(temp_v0);
    temp_v2 = func_002BD1F8(temp_v1);
    *arg1 = temp_v2;
    func_002D0918(temp_v0);
    func_002887A0(arg0);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BCB78);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BCBD0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BCC20);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BCCA8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BCD50);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BCDF0);

u32 func_002BCEA8(u32 arg0) {
    func_002BCD50();
    func_002CFF98(arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BCED8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BCF90);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BD028);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BD1F8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BD258);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BD2F8);

u32 func_002BD378(s32 arg0) {
    func_002D0918(*(u32 *)(arg0 + 0x14));
    return 1;
}

s32 func_002BD398(s32 arg0, s32 arg1) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = arg1 * 0xa0 + *(s32 *)(arg0 + 0x18);
    temp_v0 = *(s32 *)(temp_v1 + 0x9c);
    if (temp_v0 != 0) {
        temp_v1 = temp_v0;
    }
    return temp_v1;
}

void func_002BD3B8(BdWork *p) {
    if (p->x0 & 1) {
        p->x4 = 0;
    } else {
        p->x4 = 0x10000;
    }
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BD3D8);

void func_002BD5A0(s32 arg0, s32 arg1) {
    func_002BD3D8(arg0, arg1, *(s32 *)(arg0 + 0x18) + arg1 * 0xa0);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BD5C8);

void func_002BD620(void *a0, s32 a1, BdWork *p) {
    p->x60 = a0;
    p->x64 = a1;
    func_002BD3D8(a0, a1, p);
}

void func_002BD640(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)((s32)arg0 + 0x18) + (s32)arg1 * 0xa0;
    memset(temp_v0, 0, 0xa0);
    func_002BD620(arg0, arg1, temp_v0);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BD6A8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BD7A0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BD800);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BD870);

u8 func_002BD8F8(s32 arg0) {
    return **(s32 **)(arg0 + 0x24) != 0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BD908);

u32 func_002BD988(u32 arg0) {
    func_002D0918(*(u32 *)arg0);
    func_002CFF98(arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BD9C0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BDBC0);

u32 func_002BDD60(u32 arg0) {
    s32 *piVar1;

    piVar1 = (s32 *)arg0;
    if (*piVar1 != 0) {
        func_002D0918(*piVar1);
    }
    if (piVar1[1] == 0) {
        func_002BD870(arg0);
        func_002D0918(piVar1[8]);
    }
    func_002D0918(piVar1[3]);
    func_002BD378(arg0);
    func_002CFF98(arg0);
    return 1;
}

u32 func_002BDDD0(u32 *arg0, u32 arg1, u32 arg2) {
    *arg0 = arg2;
    arg0[4] = arg1;
    if ((arg2 & 2) != 0) {
        func_002BD3B8((BdWork *)arg0);
    }
    arg0[2] = arg0[2] + 1;
    return 1;
}

u32 func_002BDE18(u32 arg0, s32 arg1, s32 arg2, u32 arg3) {
    func_002BDDD0(arg0, *(s32 *)(arg1 + 8) + arg2 * 0x24, arg3);
    return 1;
}

u32 func_002BDE50(s32 arg0) {
    *(u32 *)(arg0 + 0x10) = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BDE60);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BDEC8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BDF28);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BE0C0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BE128);

u32 func_002BE1C8(s32 arg0, s32 arg1) {
    *(u32 *)(arg1 * 0xa0 + *(s32 *)(arg0 + 0x18) + 0x9c) = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BE1E8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BE258);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BE2D8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BE378);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BE3A0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BE448);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BE4B8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BE728);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BE8A8);

void func_002BED28(u32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4, u32 arg5, u32 arg6, u32 arg7) {
    func_002BE448(arg6, arg7);
    func_002C0F88(arg0, arg1, arg2, arg3, arg4, arg5, arg7);
    func_002C0A48(0x44, arg7);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BEDC8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BEEA0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BF198);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BF400);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BF438);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BF4E0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BF5D8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BF790);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BF828);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BF970);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BF9E0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BFA80);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BFAF8);

void func_002BFB50(s32 arg0, s32 arg1) {
    u32 *puVar1;
    s32 temp_v0;

    temp_v0 = 3;
    puVar1 = (u32 *)(arg1 * 0xa0 + *(s32 *)(arg0 + 0x18) + 0x14);
    do {
        temp_v0 = temp_v0 - 1;
        *puVar1 = puVar1[0x1c];
        puVar1 = puVar1 + 1;
    } while (-1 < temp_v0);
}

void func_002BFB98(IntPair *p, s32 a, s32 b) {
    p->x = a;
    p->y = b;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BFBA8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BFCE0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BFE78);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C0038);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C0178);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C0200);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C0340);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C04C8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C05F0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C0628);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C0778);

void func_002C0858(u8 arg0, u32 arg1) {
    func_002C0778(arg0, 0, arg1);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C0878);

void func_002C0950(u32 arg0, u32 arg1) {
    func_002C0878(arg0, 0, arg1);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C0970);

void func_002C0A48(u32 arg0, u32 arg1) {
    func_002C0970(arg0, 0, arg1);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C0A68);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C0B20);

void func_002C0BD8(QuadU32 *q, u32 value) {
    q->x = value;
    q->y = value;
    q->z = value;
    q->w = value;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C0BF0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C0C20);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C0DA8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C0DD8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C0DF8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C0F88);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C0FA8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C1098);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C10C0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C1228);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C1380);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C1430);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C14E0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C1548);

void func_002C1588(u32 arg0) {
    func_002C0950(0x30000, arg0);
    func_002C0A48(0x44, arg0);
    func_002C0DD8(0, 0, 0, 0x2000, 0xe00, 0, arg0);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C15E0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C1630);

void func_002C16E0(void) {
}

void func_002C16E8(void) {
}

u32 func_002C16F0(void) {
    return 0;
}

s32 func_002C16F8(s32 arg0) {
    if (arg0 == 0) {
        return 0;
    }
    *(s32 *)(arg0 + 0xC) = (*(s32 *)(arg0 + 0xC) & -2) | 2;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C1728);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C1770);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C1888);

u32 func_002C18C8(u32 arg0) {
    s64 temp_v0;

    func_002CFF98(*(u32 *)arg0);
    do {
        temp_v0 = func_002C1B30(arg0);
    } while (temp_v0 != 0);
    func_002CFF98(arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C1910);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C1978);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C19C0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C1A30);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C1B30);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C1CC8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C1D78);

void func_002C1F18(u32 arg0, u32 arg1, s32 arg2) {
    s32 temp_v0;
    s32 temp_v1;
    float *pfVar3;
    float temp_v2;
    float temp_v3;

    temp_v1 = func_002C2540(arg1, arg0);
    temp_v0 = *(s32 *)(temp_v1 + 8);
    pfVar3 = (float *)(temp_v1 + 0xc);
    temp_v2 = *(float *)(temp_v0 + 0xc);
    if (1 < arg2) {
        temp_v2 = temp_v2 * (float)(s32)arg2;
    }
    temp_v3 = *pfVar3;
    *pfVar3 = temp_v3 + temp_v2;
    if (*(float *)(temp_v0 + 8) < temp_v3 + temp_v2) {
        *pfVar3 = *(float *)(temp_v0 + 4);
    }
}

void func_002C1F88(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = (s32)arg0;
    if ((*(u32 *)(temp_v0 + 0xc) & 1) != 0) {
        func_002C1F18(arg0, (u32)*(u16 *)(*(s32 *)(temp_v0 + 0x18) + 6) + *(s32 *)(temp_v0 + 0x3c),
                                    arg1);
        return;
    }
}

void func_002C1FD0(u32 arg0, u32 arg1, s32 arg2) {
    s32 temp_v0;
    s32 temp_v1;
    float *pfVar3;
    float temp_v2;
    float temp_v3;

    temp_v1 = func_002C2540(arg1, arg0);
    temp_v0 = *(s32 *)(temp_v1 + 8);
    pfVar3 = (float *)(temp_v1 + 0xc);
    temp_v2 = *(float *)(temp_v0 + 0xc);
    if (1 < arg2) {
        temp_v2 = temp_v2 * (float)(s32)arg2;
    }
    temp_v3 = *pfVar3;
    *pfVar3 = temp_v3 - temp_v2;
    if (temp_v3 - temp_v2 < *(float *)(temp_v0 + 4)) {
        *pfVar3 = *(float *)(temp_v0 + 8);
    }
}

void func_002C2040(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = (s32)arg0;
    if ((*(u32 *)(temp_v0 + 0xc) & 1) != 0) {
        func_002C1FD0(arg0, (u32)*(u16 *)(*(s32 *)(temp_v0 + 0x18) + 6) + *(s32 *)(temp_v0 + 0x3c),
                                    arg1);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C2088);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3BD0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3BE0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3BF0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3C00);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3C10);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3C20);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3C30);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3C40);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3C50);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C20F8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C22F0);

s32 func_002C2540(u32 key, u32 head) {
    u32 n;

    n = *(u32 *)(head + 0x14);
    while (n != 0 && *(u16 *)(n + 6) != key) {
        n = *(u32 *)(n + 0x1C);
    }
    return n;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C2568);

void func_002C25E0(u32 arg0) {
    func_002C2568(0, arg0);
}

s32 func_002C2600(s32 arg0) {
    return func_002C2568(*(s16 *)(arg0 + 0xa) - 1, (void *)arg0);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C2620);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C2658);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C26C8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C2768);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C27F8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C2870);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C28E0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C2A20);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C2BF8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C2CC0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C2DA0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C2E38);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C2EA0);

s32 func_002C2ED0(void) {
    return kwlnTaskGetTaskByName((u32)D_003B3CA0) != 0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C2EF8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3CA0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C2F40);

void func_002C2FC0(void) {
    s64 temp_v0;

    D_003BD25C = 1;
    D_003BD970 = 0;
    D_003BD974 = 0;
    temp_v0 = func_0021F600(0x413);
    D_003BD260 = (u32)(temp_v0 == 0);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C2FF8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C3060);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C30F0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C3220);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C3420);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C3510);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C35C8);

u32 func_002C3640(void) {
    s64 temp_v0;
    u32 temp_v1;

    temp_v0 = func_0021F600(0x412);
    temp_v1 = 2;
    if (temp_v0 == 0) {
        temp_v0 = func_0021F600(0x411);
        temp_v1 = 1;
        if (temp_v0 == 0) {
            func_0021F600(0x410);
            temp_v1 = 0;
        }
    }
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C3690);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C36E8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C3738);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C3800);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C3868);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C38B0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C3AC8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C3C48);

void func_002C3CD0(u32 arg0) {
    func_002C3C48();
    D_003BD268 = arg0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C3CF8);

void func_002C3D48(void) {
    if (D_003BD268 != 0) {
        func_002C3C48();
        D_003BD268 = D_003BD268 - 1;
    } else {
        func_002C3C48();
        D_003BD268 = D_003BD26C - 1;
    }
}

s8 func_002C3D88(void) {
    return D_003BD270;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C3D90);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C3DB0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C3F78);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C4160);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C42F0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C44D0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C45C8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C4630);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C4650);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C4680);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C4850);

u32 func_002C49F8(void) {
    return **(u32 **)(*(s32 *)(D_003BD274 + 0x1c) + 0x70);
}

s32 func_002C4A10(void) {
    return *(s16 *)(*(s32 *)(*(s32 *)(D_003BD274 + 0x1c) + 0x70) + 8);
}

float func_002C4A28(void) {
    s32 p;

    p = *(s32 *)(D_003BD274 + 0x30);
    return (float)(*(s32 *)p) / 10.0f;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C4A58);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C4C88);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C5338);

void func_002C56C0(void) {
    s32 temp_v0;

    temp_v0 = **(s32 **)(D_003BD274 + 0x30);
    if (temp_v0 < 10) {
        **(s32 **)(D_003BD274 + 0x30) = temp_v0 + 1;
    }
}

void func_002C56E8(void) {
    s32 temp_v0;

    temp_v0 = **(s32 **)(D_003BD274 + 0x30);
    if (temp_v0 != 0) {
        **(s32 **)(D_003BD274 + 0x30) = temp_v0 - 1;
    }
}

void func_002C5708(s32 arg0) {
    s32 ptr;

    ptr = *(s32 *)(D_003BD274 + 0x30);
    *(s16 *)(ptr + 6) = arg0;
    *(s16 *)(ptr + 4) = 8;
}

void func_002C5720(void) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(D_003BD274 + 0x30);
    if (0 < *(s16 *)(temp_v0 + 4)) {
        *(s16 *)(temp_v0 + 4) = *(s16 *)(temp_v0 + 4) - 1;
    }
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C5748);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C5798);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C57F0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C5BF8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C5C40);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C5C70);

void func_002C5D30(void) {
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C5D38);

void func_002C5FB8(u32 arg0) {
    func_00195CD8(arg0, 1, 3);
}

s32 func_002C5FD8(s32 x, s32 n) {
    s32 i = 0;
    s32 cnt = 0;
    s32 ni;

    do {
        ni = i + 1;
        if (n == ni) {
            break;
        }
        cnt += (x >> i) & 1;
        i = ni;
    } while (i < 0x1F);
    return cnt;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C6010);

void func_002C6098(s32 arg0, s32 arg1) {
    if ((arg1 <= *(s32 *)(arg0 + 0x20)) && (arg1 != 0)) {
        *(s32 *)(arg0 + 0xc) = arg1;
    }
}

void func_002C60B8(s32 arg0) {
    if (*(s32 *)(arg0 + 0xc) < 10) {
        *(s32 *)(arg0 + 0xc) = *(s32 *)(arg0 + 0xc) + 1;
    }
}

void func_002C60D8(s32 arg0) {
    if (1 < *(s32 *)(arg0 + 0xc)) {
        *(s32 *)(arg0 + 0xc) = *(s32 *)(arg0 + 0xc) - 1;
    }
}

s32 func_002C60F8(void *p) {
    void *n;
    s32 mask;

    n = *(void **)((s32)p + 0x10);
    mask = 0;
    do {
        void *m = *(void **)((s32)n + 0x70);
        n = *(void **)((s32)n + 0x58);
        mask |= 1 << (*(s16 *)((s32)m + 8) - 1);
    } while (n != NULL);
    return mask;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C6130);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C62D8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C6370);

void func_002C63D8(void) {
    s32 temp_v0;

    D_003BD978 = 0;
    temp_v0 = func_002C4A10();
    D_003BD97C = temp_v0 - 1;
    D_003BD980 = 0x3c;
}

void func_002C6408(void) {
    if ((s32)D_003BD978 < 0x3C) {
        D_003BD978++;
    }
}

void func_002C6428(void) {
    if ((s32)D_003BD978 > 0) {
        D_003BD978 -= 2;
    } else {
        D_003BD978 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C6448);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C6948);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C6EC8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C7000);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C7058);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C7080);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C7180);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C7430);

void func_002C7700(void) {
    D_003DFEE0[0] = D_003DFED0[0];
    D_003DFEE0[1] = D_003DFED0[1];
    D_003DFEE0[2] = D_003DFED0[2];
    D_003BD984 = 1;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C7738);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C7950);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C7A60);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C7B38);

void func_002C7B58(s32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    u32 *puVar1;

    puVar1 = *(u32 **)(arg0 + 8);
    if (*(s16 *)(arg0 + 0x16) == *(s16 *)(arg0 + 0x14)) {
        if (puVar1[3] == 0) {
            *puVar1 = arg1;
            puVar1[1] = arg2;
            puVar1[2] = arg3;
            *(u32 *)(arg0 + 8) = puVar1[4];
            puVar1[3] = 1;
        }
        *(u16 *)(arg0 + 0x16) = 0;
        return;
    }
    *(s16 *)(arg0 + 0x16) = *(s16 *)(arg0 + 0x16) + 1;
}

void func_002C7BA8(s32 arg0, u16 arg1) {
    *(u16 *)(arg0 + 0x14) = arg1;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C7BB0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C7C58);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C7D18);

u32 func_002C7D80(s32 *arg0) {
    if (*arg0 != 0) {
        func_002D2D00(*arg0);
        *arg0 = 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C7DC0);

void func_002C7EC8(s32 arg0, s32 arg1, u64 arg2, u64 arg3) {
    u64 temp_v0;

    temp_v0 = func_00197760(arg0 << 4, arg1 << 3, 0, arg2, arg3, 0);
    func_00195880(temp_v0, 1);
    func_00194920(temp_v0);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C7F18);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C7FB0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C8040);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C80D8);

void func_002C8420(float *arg0, float *arg1) {
    *arg0 = *arg0 + *arg1;
    arg0[1] = arg0[1] + arg1[1];
    arg0[2] = arg0[2] + arg1[2];
}

void func_002C8458(float *arg0, float *arg1) {
    *arg0 = *arg0 - *arg1;
    arg0[1] = arg0[1] - arg1[1];
    arg0[2] = arg0[2] - arg1[2];
}

void func_002C8490(float arg0, float arg1, float arg2, float *arg3) {
    *arg3 = *arg3 + arg0;
    arg3[1] = arg3[1] + arg1;
    arg3[2] = arg3[2] + arg2;
}

void func_002C84B8(Vec3 *v, float x, float y, float z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

void func_002C84C8(float arg0, float *arg1) {
    *arg1 = *arg1 * arg0;
    arg1[1] = arg1[1] * arg0;
    arg1[2] = arg1[2] * arg0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C84F0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C8558);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C8588);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C8628);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C8648);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C8710);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C8970);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C8BD0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C8E30);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C8F40);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C8FE8);

float func_002C9228(float x, float y) {
    float p = 1.0f;
    s32 i = 1;

    if (y >= 1.0f) {
        do {
            i++;
            p *= x;
        } while ((float)i <= y);
    }
    return p;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C9268);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C94A8);

void func_002C95F0(float *arg0, float *arg1, float *arg2) {
    *arg0 = *arg1 + *arg2;
    arg0[1] = arg1[1] + arg2[1];
    arg0[2] = arg1[2] + arg2[2];
    arg0[3] = arg1[3] + arg2[3];
}

void func_002C9638(float *arg0, float *arg1, float *arg2) {
    *arg0 = (arg1[3] * *arg2 + *arg1 * arg2[3] + arg1[1] * arg2[2]) -
                          arg1[2] * arg2[1];
    arg0[1] = (arg1[3] * arg2[1] + arg1[1] * arg2[3] + arg1[2] * *arg2) -
                              *arg1 * arg2[2];
    arg0[2] = (arg1[3] * arg2[2] + arg1[2] * arg2[3] + *arg1 * arg2[1]) -
                              arg1[1] * *arg2;
    arg0[3] = ((arg1[3] * arg2[3] - *arg1 * *arg2) - arg1[1] * arg2[1]) -
                              arg1[2] * arg2[2];
}

float func_002C9740(float *arg0, float *arg1) {
    return *arg0 * *arg1 + arg0[1] * arg1[1] + arg0[2] * arg1[2] +
                  arg0[3] * arg1[3];
}

float func_002C9780(float *arg0, float *arg1) {
    return (arg0[1] * arg1[2] - arg0[2] * arg1[1]) +
                  (arg0[2] * *arg1 - *arg0 * arg1[2]) +
                  (*arg0 * arg1[1] - arg0[1] * *arg1);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C97C8);

float func_002C97E8(float *arg0) {
    return *arg0 * *arg0 + arg0[1] * arg0[1] + arg0[2] * arg0[2] +
                  arg0[3] * arg0[3];
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C9818);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C9838);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C98D0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C9948);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C9A08);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C9AD0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C9BF0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C9D10);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C9D98);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002C9F60);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CA0C0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CA158);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CA210);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CA2F0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CA410);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CA4A8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CA508);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CA598);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CA6B0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CA708);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CA778);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CA858);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CA8F0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CA988);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CAA20);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CAAC8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CAC60);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CACD8);

void func_002CAD20(s32 arg0, s32 arg1) {
    if (arg1 != 0) {
        *(s32 *)(arg0 + 0x18) = (s32)arg1;
    }
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CAD30);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CADD0);

void func_002CAEB8(s32 arg0, s32 arg1) {
    if (arg1 != 0) {
        *(s32 *)(arg0 + 0x14) = (s32)arg1;
    }
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CAEC8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CAF78);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CAFE0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CB058);

void *func_002CB0F8(void *head, s32 key) {
    void *n;

    n = *(void **)((s32)head + 8);
    while (*(s32 *)((s32)n + 4) != key) {
        n = *(void **)((s32)n + 8);
        if (n == NULL) {
            break;
        }
    }
    return n;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CB120);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CB1C8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CB278);

u8 func_002CB2B8(s32 arg0) {
    u8 temp_v0;
    s64 temp_v1;

    temp_v0 = 0;
    if (arg0 != 0) {
        temp_v1 = kwlnTaskGetTaskByName(*(u32 *)((s32)arg0 + 4));
        temp_v0 = temp_v1 != 0;
    }
    return temp_v0;
}

s32 func_002CB2E0(u32 name) {
    return kwlnTaskGetTaskByName(name) != 0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CB300);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CB358);

s32 func_002CB390(void *p, s32 key) {
    void *r;

    r = func_002CB0F8(*(void **)((s32)p + 0xC), key);
    if (r != NULL) {
        return *(s32 *)((s32)r + 0x10);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CB3B8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3DC0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3E00);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3E40);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3EE0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CB3F8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CB4B8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CB5A8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CB5F0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CB6B8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CB6F8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CB718);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CB850);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CB8E0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CB938);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CB990);

void func_002CB9B8(void) {
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CB9C0);

void func_002CBAF0(ShortPair2C *p, s32 a, s32 b) {
    p->h2C = a;
    p->h2E = b;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CBB00);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CBB48);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CBB68);

void func_002CBBA0(void *p, u32 *mod, u32 *div) {
    u32 *t = *(u32 **)((s32)p + 8);
    *mod = *t % *(u32 *)((s32)p + 0x14);
    *div = *t / *(u32 *)((s32)p + 0x14);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CBBD8);

void func_002CBC10(s32 arg0, s32 arg1, s32 arg2, u32 arg3) {
    u32 temp_v0;

    temp_v0 = arg2 * *(s32 *)(arg0 + 0x14) + arg1;
    if (temp_v0 < *(u32 *)(arg0 + 0x10)) {
        *(u32 *)(temp_v0 * 8 + *(s32 *)(arg0 + 4) + 4) = arg3;
    }
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CBC48);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CBC98);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CBCF0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CBD48);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CBDA8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CBE18);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CBF60);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CC0D0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CC238);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CC3A8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CC430);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CC570);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CC5F0);

float func_002CC728(float arg0, float arg1, float arg2) {
    return arg0 + arg1 * arg2;
}

u32 func_002CC738(void) {
    return 0;
}

void func_002CC740(void) {
}

u32 func_002CC748(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CC750);

void func_002CC7D8(void) {
    memset(D_003BAA00 + 0x2ebb0, 0, 0x3000);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CC808);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CC9C0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CCB80);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CCC18);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CCCC0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CCDC8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CCE60);

void func_002CD0C0(u32 arg0) {
    func_002CCE60(arg0, 0);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CD0D8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CD240);

u32 func_002CD2A8(u16 i) {
    return D_003907B8[i].v0;
}

void func_002CD2D0(u32 arg0, u16 arg1) {
    u32 *puVar1;
    u32 temp_v0;

    puVar1 = (u32 *)func_002CD730(arg0, arg1);
    temp_v0 = func_002CD2A8(arg1);
    *puVar1 = temp_v0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CD310);

void func_002CD398(u32 v, u32 *a, u32 *b) {
    u32 lo;

    v &= 0xFFFF;
    lo = v & 0xF;
    v >>= 4;
    *a = v;
    *b = lo << 1;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CD3B8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CD428);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CD548);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CD5B8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CD630);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CD6B0);

s8 func_002CD728(ScrVmOperand *op) {
    return op->s55;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CD730);

u32 func_002CD768(u32 arg0, u16 arg1) {
    u32 *puVar1;

    puVar1 = (u32 *)func_002CD730(arg0, arg1);
    return *puVar1;
}

u32 func_002CD788(ScrVmOperand *p) {
    return func_002CD730((u32)p, func_002CD728(p) & 0xFFFF);
}

s8 func_002CD7B8(ScrVmOperand *op) {
    return op->s55;
}

s8 func_002CD7C0(ScrVmOperand *p, s32 v) {
    p->s55 = v;
    func_002CD630(p, v & 0xFFFF);
    return p->s55;
}

u32 func_002CD7F0(u32 arg0, u32 arg1) {
    return arg1;
}

u32 func_002CD7F8(void) {
    return 0;
}

u32 func_002CD800(void) {
    return 1;
}

void func_002CD808(s32 unused, u32 v, u32 *a, u32 *b) {
    u32 lo;

    v &= 0xFFFF;
    lo = v & 7;
    v >>= 3;
    *a = v;
    *b = lo << 2;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CD828);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CD888);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CD8E8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CD940);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CD998);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CD9F8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CDA48);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CDA98);

s32 func_002CDB00(s32 arg0, s32 arg1) {
    u32 key = arg1 & 0xFFFF;
    u16 *p = (u16 *)(arg0 + 0x22);
    u32 i = 0;

    do {
        if (*p++ == key) {
            return 1;
        }
        i++;
    } while (i < 0x18);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CDB40);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CDC40);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CDC80);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CDCA0);

u16 func_002CDCD8(s32 arg0, s32 arg1, u16 arg2) {
    u16 temp_v0;
    u16 *puVar2;

    puVar2 = (u16 *)(arg1 * 2 + arg0 + 0x22);
    temp_v0 = *puVar2;
    *puVar2 = arg2;
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CDCF0);

u8 func_002CDD38(u16 i) {
    return D_003907B4[i].v0;
}

u16 func_002CDD60(u16 i) {
    return D_003907B6[i].v0;
}

u8 func_002CDD88(u16 i) {
    return D_003907B5[i].v0;
}

u32 func_002CDDB0(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CDDB8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CDE20);

u32 func_002CDE60(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CDE68);

void func_002CDE98(u32 arg0, u32 arg1, u32 arg2) {
    memset(arg2, 0, 8);
}

u8 func_002CDEB8(s32 arg0, u32 arg1) {
    return (s64)*(s8 *)(arg0 + 0x55) == (arg1 & 0xffff);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CDED0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CDFE8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CE170);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CE188);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CE1C8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CE248);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CE2E8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CE380);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CE468);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CE480);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CE498);

void func_002CE698(u32 arg0, u32 arg1, u16 arg2) {
    func_002CE498(arg0, arg1, arg2, 0);
}

u32 func_002CE6B8(u16 i) {
    return D_003907B0[i].v0;
}

u32 func_002CE6E0(u16 i) {
    return D_00391230[i].v0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CE710);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CE800);

u8 func_002CE8F0(s32 i) {
    return D_00393220[i].v0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CE910);

u32 func_002CE968(s32 i) {
    return D_00393234[i].v0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CE988);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CE9E0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CEA10);

u8 *func_002CEA80(void) {
    return D_00394680;
}

void func_002CEA90(s32 arg0) {
    u32 temp_v0;
    u32 *puVar2;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *(u32 *)(arg0 + 0x4c);
    puVar2 = *(u32 **)(arg0 + 8);
    if (temp_v0 != 0) {
        do {
            temp_v1 = temp_v1 + 1;
            *puVar2 = 0xffffffff;
            puVar2 = puVar2 + 2;
        } while (temp_v1 < temp_v0);
    }
    memset(*(u32 *)(arg0 + 0x10), 0, temp_v0 << 3);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CEAE8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CEC08);

void func_002CEC28(s32 arg0) {
    func_002D0918(*(u32 *)(arg0 + 0x54));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CEC40);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CF248);

float func_002CF390(ScrVmOperand *op) {
    return op->f50;
}

void func_002CF398(ScrVmOperand *op, float value) {
    op->f50 = value;
}

void func_002CF3A0(s32 arg0) {
    func_00296F58(arg0 + 0x14, arg0 + 0x38, 0, 0);
}

void func_002CF3C8(RgbAlpha *p, u32 color) {
    p->rgb18 = color & 0xFFFFFF;
    p->alpha38 = color >> 24;
}

u32 func_002CF3E8(s32 arg0) {
    return *(u32 *)(arg0 + 0x3c);
}

void func_002CF3F0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x3c) = arg1;
}

void func_002CF3F8(RgbAlpha *dst, CfSrc *src) {
    dst->rgb18 = src->x04;
    dst->f50 = src->f3C;
    dst->alpha38 = src->x24;
    dst->x3C = src->x28;
}

void func_002CF420(void) {
    D_003BD2C8 = 1;
}

void func_002CF430(void) {
    D_003BD2C8 = 0;
}

void func_002CF438(void) {
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CF440);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CF468);

void func_002CF4E0(u64 arg0, u64 arg1, u64 arg2) {
    u64 temp_v0;

    temp_v0 = func_002CF530(arg1);
    func_002CF468(arg0, temp_v0, arg1, arg2);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CF530);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CF570);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CF5C0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CF618);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CF670);

INCLUDE_ASM(const s32, "game/code_0029A840", sdfAddHandler);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CF7B8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CF898);

void func_002CF8C8(u32 arg0, u32 arg1, u32 arg2) {
    iWakeupThread(arg2);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CF8E0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CF930);

u32 func_002CF940(u32 base) {
    u32 now;

    now = *(volatile u32 *)0x10000000;
    return (now - base) & 0xFFFF;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CF958);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CF9A8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CFA68);

void func_002CFAD8(s32 arg0) {
    u64 temp_v0;
    s32 temp_v1;

    temp_v0 = GetThreadId();
    temp_v1 = CancelWakeupThread(temp_v0);
    arg0 = arg0 - temp_v1;
    do {
        arg0 = arg0 - 1;
        SleepThread();
    } while (0 < arg0);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002CFB18);
