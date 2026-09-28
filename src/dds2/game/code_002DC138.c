#include "common.h"

extern u64 func_003283E0(u64);

extern u64 func_0035A828(u64);

extern u32 D_004390D8;

extern u8 *D_004389B0;

extern u8 *D_004389AC;

extern u8 *D_004389A8;

extern u8 *D_004389A4;

extern u8 *D_004389A0;

extern u32 D_004390C8;

extern u32 D_004390CC;

extern u32 D_004390E4;

extern u32 D_004390E8;

extern u32 D_004390DC;

extern u32 D_004390E0;

extern u32 D_004390D0;

extern u32 D_004390D4;

extern u32 D_004390C0;

extern u32 D_004390C4;

extern u8 D_004390B8;

extern u64 func_00325BB0(u64, u32);

extern u64 func_00325AB8(u64, u32);

extern u64 func_00320AE8(u64, u64, u32 *);

extern u64 func_00325790(u64, u32);

extern u64 func_0031F0E8(void);

extern s32 func_0031E550(void);

extern u32 D_0043895C;

extern u32 *D_00438940;

extern s32 D_00438930;

extern u32 D_0043891C;

extern s32 func_00317FE0(u32);

extern u32 D_00438918;

extern s32 D_00435E50;

extern s32 D_00435DD0;

extern u64 func_0019F460(s32, s32, u64, u64, u64, u64);

extern u64 func_0019D958(u64);

extern u64 func_0019F448(u64, u64, u64, u64, u64, u64);

extern s32 func_0030AC10(void);

extern u32 D_004388AC;

extern u32 D_004388B0;

extern u32 D_0043908C;

extern u32 D_00439090;

extern s32 func_0023A170(u32);

extern u32 func_00304938(u64);

extern u64 func_003298F8(u64);

extern u64 func_002C8108(void);

extern u32 func_00305148(u64, u64);

extern s32 func_002DC1D0(u32, u32);

extern u32 func_00331940(u32, u32);

extern u32 func_00328D68(u32);

extern s32 D_00437E74;

extern u32 D_00437E78;

extern u32 D_00437E50;

extern u32 D_00437E08;

extern u64 func_002D3468(void);

extern s64 func_001AA308(void);

extern s64 func_001AA318(void);

extern u64 func_002DC2D8(void);

extern s32 *D_00437E30;

extern u32 D_00437E38;

extern u32 D_00437E3C;

extern u32 func_00159A50(u32);

extern u32 func_002DDF48(u32);

extern u32 D_00437E70;

extern u32 D_00437E80;

extern s32 D_00437E84;

extern u32 D_00437E88;

extern u32 func_002DDCA0(u32, u32);

extern u64 func_002F69F0(u32, u32, u32);

extern s32 func_001AA6F8(void);

extern s32 D_004386C4;

extern s32 D_004386B4;

extern u8 D_00439075;

extern s32 D_004386F0;

extern s32 D_004386F4;

extern s32 D_0043875C;

extern s32 D_00438760;

extern s32 D_00438768;

extern u32 D_0043876C;

extern u32 D_00438770;

extern u32 D_00438778;

extern u32 D_004387A8;

extern u32 D_004387AC;

extern u32 D_004387B0;

extern u32 D_00438774;

extern s32 func_00309638(u32);

extern s32 func_0030A048(u32, u32);

extern u32 D_004388B8;

extern s32 D_004388C4;

extern u32 D_00439098;

extern s32 D_0043909C;

extern u32 D_004390A0;

extern s32 func_0030C9B8(void);

extern s32 func_00101740(u32);

extern u32 func_00314690(u16);

extern u32 func_00314B80(u32, u16);

extern s32 CancelWakeupThread(u64);

extern u64 GetThreadId(void);

extern void func_00232B40(void *);

extern void func_002DCAE8(s32 *);

extern void *func_002DDAA8(void *);

/* Reference-counted object header (layout inferred from field accesses). */
typedef struct RefObj {
    u8 pad_0x00[0x14]; // 0x00
    s32 cnt14;         // 0x14
    s32 unk18;         // 0x18
    s32 cnt1C;         // 0x1C
} RefObj; // 0x20

extern u32 D_00437E34;

/* 4x4 float matrix with 128-bit row access for VU0/DMA transfers. */
typedef struct Matrix4 {
    union {
        float m[4][4];
        s128 rows[4];
    } u; // 0x00
} Matrix4; // 0x40

/* Function-pointer tables indexed by object fields (entry size inferred). */
typedef struct FnTbl28 {
    void (*fn)(void *);
    u8 pad_0x04[0x18]; // 0x04
} FnTbl28; // 0x1C

extern FnTbl28 D_003E9964[];

typedef struct FnTbl24 {
    void (*fn)(void *);
    u8 pad_0x04[0x14]; // 0x04
} FnTbl24; // 0x18

extern FnTbl24 D_003E9B90[];

extern u8 *D_003E9CA8[];

extern u32 D_00437E68;

extern FnTbl24 D_003E9D10[];

extern FnTbl28 D_003E9DEC[];

extern u32 D_00437E6C;

extern u32 func_00343ED0(const char *, u32 *, s32);

extern FnTbl28 D_003E9E74[];

extern u64 func_002F3D88();

extern u32 D_00437E7C;

extern FnTbl24 D_003E9F18[];

extern u32 D_004386B0;

extern u32 D_003FFA84[];

extern u8 D_0045C110[];

extern u8 D_00438758;

extern u8 D_00438759;

extern u8 D_0043875A;

extern u32 D_004386C0;

typedef struct QuadU32 {
    u32 x; // 0x00
    u32 y; // 0x04
    u32 z; // 0x08
    u32 w; // 0x0C
} QuadU32; // 0x10

extern char D_0042D240[]; /* "LmapMain" */

extern s32 D_004388BC;

extern void func_0030BBA8(void);

extern s8 D_004388C0;

extern u32 D_004390A4;

extern u32 D_0045C7A0[];

extern u32 D_0045C7B0[];

extern void kwlnTaskDestroyWithHierarchyByName(char *, s32);

typedef struct ShortPair2C {
    u8 pad_0x00[0x2C]; // 0x00
    s16 h2C;           // 0x2C
    s16 h2E;           // 0x2E
} ShortPair2C; // 0x30

/* 24-byte table entries (full layout unknown; stride inferred from index math). */
typedef struct Entry24B {
    u8 v0;            // 0x00
    u8 pad_0x01[0x17]; // 0x01
} Entry24B; // 0x18

extern Entry24B D_00404A90[];

typedef struct Entry24W {
    u32 v0;            // 0x00
    u8 pad_0x04[0x14]; // 0x04
} Entry24W; // 0x18

extern Entry24W D_00404AA4[];

extern u8 D_00405CA8[];

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

extern s32 CreateSema(void *);

extern u32 D_004389BC;

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DC138);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DC1D0);

void func_002DC260(s32 arg0) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x80) = 0;
    func_002322E8();
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DC280);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DC2D8);

void func_002DC370(void *p) {
    void *q = *(void **)((s32)p + 8);
    if (q != NULL) {
        func_00328E48(q);
    }
    if (*(s32 *)((s32)p + 4) != 0) {
        func_002DC260(*(s32 *)((s32)p + 4));
    }
    func_00328E48(p);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DC3C0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DC410);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DC4A8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DC4C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DC520);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DC540);

void func_002DC560(s32 arg0) {
    func_00232BC0(*(u32 *)(arg0 + 4));
}

void func_002DC578(s32 arg0, float scale) {
    float v[3];
    float t;

    t = *(float *)arg0 * scale;
    v[0] = v[1] = v[2] = t;
    __asm__ volatile ("lqc2 $vf10, 0(%0)" :: "r" (v));
    func_00232B40(*(void **)(arg0 + 4));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DC5B0);

u64 func_002DC6E8(void) {
    u64 temp_v0;
    s64 temp_v1;

    temp_v0 = func_002DC2D8();
    func_002DC5B0(temp_v0);
    temp_v1 = func_001AA308();
    if ((temp_v1 != 0) && (temp_v1 = func_001AA318(), temp_v1 == 0)) {
        func_002DCA88(temp_v0);
    }
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DC748);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DC788);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DC808);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DCA30);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DCA88);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DCAE8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DCB58);

void func_002DCBF0(void) {
    s32 *node = D_00437E30;
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

void func_002DCC30(void) {
    s32 temp_v0;
    s32 *piVar2;

    piVar2 = D_00437E30;
    while (piVar2 != (s32 *)0x0) {
        temp_v0 = *piVar2;
        piVar2 = (s32 *)piVar2[2];
        *(u32 *)(temp_v0 + 0xc) = *(u32 *)(temp_v0 + 0xc) & 0xffffffef;
    }
}

void func_002DCC68(void) {
    s32 temp_v0;
    s32 *piVar2;

    piVar2 = D_00437E30;
    while (piVar2 != (s32 *)0x0) {
        temp_v0 = *piVar2;
        piVar2 = (s32 *)piVar2[2];
        *(u32 *)(temp_v0 + 0xc) = *(u32 *)(temp_v0 + 0xc) | 0x10;
    }
}

void func_002DCCA0(void) {
    s32 *node = D_00437E30;
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
        func_002DCAE8(node);
        node = next;
    } while (node != NULL);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DCCE8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DD038);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DD0C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DD258);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DD3C0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DD448);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DDA10);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DDA30);

void func_002DDA50(s32 arg0) {
    func_00232BC0(*(u32 *)(arg0 + 0xc0));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DDA68);

void func_002DDA98(void) {
    D_00437E3C = 0;
    D_00437E38 = 0xffffffff;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DDAA8);

u32 func_002DDCA0(u32 arg0, u32 arg1) {
    void *p;

    p = func_002DDAA8((void *)arg0);
    *(u32 *)((s32)p + 0x18) = arg1;
    return (u32)p;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DDCC8);

RefObj *func_002DDD40(RefObj *obj) {
    obj->cnt14++;
    D_00437E34++;
    return obj;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DDD60);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DDF08);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DDF48);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DE090);

u32 func_002DE120(u32 arg0) {
    *(s32 *)((s32)arg0 + 0x1c) = *(s32 *)((s32)arg0 + 0x1c) + 1;
    return arg0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DE138);

void func_002DE218(s32 arg0, u32 arg1, s32 arg2) {
    func_002DDD60(arg1, *(u32 *)(*(s32 *)(arg2 + 0xc) * 4 + *(s32 *)(arg0 + 0x14)));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DE248);

void func_002DE288(void) {
    u64 temp_v0;

    temp_v0 = func_002D3468();
    func_002DE248(temp_v0);
}

void func_002DE2A8(u32 arg0) {
    func_001057A8();
    func_00328E48(arg0);
}

void func_002DE2D0(s32 arg0) {
    func_002DE248(arg0 + 4);
}

void func_002DE2E8(u32 *arg0) {
    *arg0 = 0;
}

void func_002DE2F0(s32 *arg0) {
    s32 temp_v0;

    temp_v0 = *arg0;
    if (temp_v0 == 0) {
        func_00105748(arg0[1], arg0[2]);
        temp_v0 = *arg0;
    }
    *arg0 = temp_v0 + 1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DE338);

void func_002DE408(void) {
    u64 temp_v0;

    temp_v0 = func_002D3468();
    func_002DE338(temp_v0);
}

void func_002DE428(void) {
    func_00328E48();
}

void func_002DE440(s32 arg0) {
    func_002DE338(arg0 + 0x18);
}

void func_002DE458(s32 arg0) {
    *(u32 *)(arg0 + 0x10) = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DE460);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DEB10);

void func_002DEB20(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

void func_002DEB28(s32 arg0) {
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

void func_002DEB80(void) {
    func_00316528();
}

void func_002DEB98(void) {
    func_00316648();
}

void func_002DEBB0(void) {
    func_00316668();
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DEBC8);

void func_002DEBE0(s32 *p) {
    func_002DEB28((s32)p);
    *p = 0;
}

void func_002DEC08(void) {
    if ((D_00437E08 & 2) == 0) {
        func_00316680();
        return;
    }
}

void func_002DEC38(void) {
    func_00316C88();
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DEC50);

void func_002DEC78(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 4) = arg1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DEC80);

void func_002DECD0(void) {
    u64 temp_v0;

    temp_v0 = func_002D3468();
    func_002DEC80(temp_v0);
}

void func_002DECF0(u32 arg0) {
    func_0010A298();
    func_00328E48(arg0);
}

void func_002DED18(s32 arg0) {
    func_002DEC80(arg0 + 4);
}

void func_002DED30(u32 *arg0) {
    *arg0 = 0;
}

void func_002DED38(s32 *arg0) {
    s32 temp_v0;

    temp_v0 = *arg0;
    if (temp_v0 == 0) {
        func_00109D50(arg0[1], (s16)arg0[2], arg0[3], arg0[4]);
        temp_v0 = *arg0;
    }
    *arg0 = temp_v0 + 1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DED88);

void func_002DEF00(s32 arg0) {
    func_0018E850(arg0 + 0xc0);
}

void func_002DEF18(void) {
    func_0018E8F0();
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DEF30);

void func_002DF138(s32 arg0, u32 arg1) {
    *(u32 *)(*(s32 *)(arg0 + 0x24) + 0x2c) = arg1;
}

void func_002DF148(s32 arg0) {
    func_0018EBC8(arg0 + 0xc0);
}

void func_002DF160(void) {
    func_0018ECB8();
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DF178);

void func_002DF348(s32 arg0, u32 arg1) {
    *(u32 *)(*(s32 *)(arg0 + 0x24) + 0x2c) = arg1;
}

void func_002DF358(s32 arg0) {
    func_0018F098(arg0 + 0xc0);
}

void func_002DF370(void) {
    func_0018F1B8();
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DF388);

void func_002DF558(s32 arg0, u32 arg1) {
    *(u32 *)(*(s32 *)(arg0 + 0x24) + 0x2c) = arg1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DF568);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DF6C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DF978);

void func_002DFA80(s32 arg0) {
    func_0018FBF8(arg0 + 0xc0);
}

void func_002DFA98(void) {
    func_0018FC88();
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DFAB0);

void func_002DFC78(s32 arg0, u32 arg1) {
    *(u32 *)(*(s32 *)(arg0 + 0x24) + 0x24) = arg1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DFC88);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DFDB8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DFE70);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DFED8);

void func_002DFF78(s32 arg0) {
    *(u32 *)(arg0 + 0x20) = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DFF80);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DFFE0);

void func_002DFFF0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

void func_002DFFF8(Matrix4 *mat, float value) {
    mat->u.m[1][2] = value;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E0000);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E0130);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E01E8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E0250);

void func_002E02F0(s32 arg0) {
    *(u32 *)(arg0 + 0x20) = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E02F8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E0358);

void func_002E0368(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E0370);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E0378);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E0400);

u32 func_002E0460(u32 arg0) {
    *(s32 *)((s32)arg0 + 4) = *(s32 *)((s32)arg0 + 4) + 1;
    return arg0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E0478);

void func_002E0528(u32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)((s32)arg0 + 100);
    if (temp_v0 != 0) {
        func_00159AF0(temp_v0);
    }
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E0560);

void func_002E0618(s32 arg0, s32 arg1) {
    u32 temp_v0;

    if (*(s32 *)(arg0 + 100) != 0) {
        func_00159AF0(*(s32 *)(arg0 + 100));
    }
    temp_v0 = func_00159A50(*(u32 *)(arg1 + 100));
    *(u32 *)(arg0 + 100) = temp_v0;
}

void func_002E0668(s32 arg0) {
    func_00159ED8(*(u32 *)(arg0 + 100), 0);
    *(u32 *)(arg0 + 0x28) = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E0698);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E0790);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E07A0);

void func_002E07B8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002E07C0(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E07C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E0840);

void func_002E08D0(s32 arg0) {
    func_002E5D28(*(u32 *)(arg0 + 4));
    func_003297C8(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E0900);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E0F30);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E1098);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E1110);

void func_002E11A0(s32 arg0) {
    func_002E5D28(*(u32 *)(arg0 + 4));
    func_003297C8(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E11D0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E1908);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E1A70);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E1AE8);

void func_002E1B80(s32 arg0) {
    func_002E5D28(*(u32 *)(arg0 + 4));
    func_003297C8(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E1BB0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E22C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E2430);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E24A8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E2520);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E25B0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E2610);

void func_002E2670(s32 arg0) {
    func_002E5D28(*(u32 *)(arg0 + 4));
    func_003297C8(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E26A0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E2C48);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E2DB0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E2E28);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E2E88);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E2F18);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E2F78);

void func_002E2FD8(s32 arg0) {
    func_002E5D28(*(u32 *)(arg0 + 4));
    func_003297C8(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E3008);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E35F0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E3758);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E37D0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E3830);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E38C0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E3920);

void func_002E3980(s32 arg0) {
    func_002E5D28(*(u32 *)(arg0 + 4));
    func_003297C8(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E39B0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E4070);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E41D8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E4250);

void func_002E42F0(s32 arg0) {
    func_002E5D28(*(u32 *)(arg0 + 4));
    func_003297C8(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E4320);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E4AC8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E4C30);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E4CA8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E4D00);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E4D90);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E4DF0);

void func_002E4E50(s32 arg0) {
    func_002E5D28(*(u32 *)(arg0 + 4));
    func_003297C8(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E4E80);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E5540);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E56A8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E5760);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E5800);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E5860);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E58A8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E5978);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E59C0);

void func_002E5A20(s32 arg0) {
    D_003E9964[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
}

void func_002E5A58(u32 arg0) {
    func_002E59C0();
    func_002E5A20(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E5A80);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E5A90);

void func_002E5AA8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002E5AB0(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BC10);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E5AB8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E5C50);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E5D28);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E5DE0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E5E88);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E6390);

u32 func_002E63F0(s32 arg0) {
    return (&D_00437E50)[arg0];
}

void func_002E6408(s32 arg0) {
    *(u32 *)(**(s32 **)(arg0 + 0x30) + 4) = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E6418);

void func_002E64C0(u32 arg0) {
    func_002E7698(*(u32 *)arg0);
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E64F0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E6758);

void func_002E68C8(s32 arg0) {
    u32 temp_v0;

    temp_v0 = *(u32 *)(*(s32 *)(arg0 + 0x30) + 4);
    *(u32 *)(*(s32 *)(*(s32 *)(arg0 + 0x30) + 8) + 4) = 0;
    func_002E7488(temp_v0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E68F0);

void func_002E6B30(s32 arg0) {
    func_002E5D28(*(u32 *)(arg0 + 8));
    func_002E7420(*(u32 *)(arg0 + 4));
    func_003297C8(*(u32 *)(arg0 + 0xc));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E6B68);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E6CB8);

void func_002E6E60(s32 arg0) {
    *(u32 *)(**(s32 **)(arg0 + 0x30) + 4) = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E6E70);

void func_002E6F18(u32 arg0) {
    func_002E7698(*(u32 *)arg0);
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E6F48);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7190);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7300);

void func_002E73F0(s32 arg0) {
    u64 temp_v0;

    temp_v0 = func_002D3468();
    func_002E7300(*(u16 *)(arg0 + 0xc), temp_v0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7420);

void func_002E7468(s32 arg0) {
    func_002E7300(*(u16 *)(arg0 + 0x2c), *(u32 *)(arg0 + 0x34));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7488);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E74D0);

void func_002E7530(s32 arg0) {
    D_003E9B90[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
}

void func_002E7568(u32 arg0) {
    func_002E74D0();
    func_002E7530(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7590);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E75A0);

void func_002E75B8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002E75C0(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E75C8);

void func_002E7698(s32 arg0) {
    func_00333918(*(u32 *)(arg0 + 0x18));
    func_003297C8(*(u32 *)(arg0 + 0x1c));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E76C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7A88);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7AE0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7B18);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7B90);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7C00);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7CB8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7D08);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7E70);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7EE0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7F60);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E81B0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E82F0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E8350);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E83C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E8448);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E84C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E85C0);

void func_002E86B8(s32 arg0, u32 arg1) {
    u32 temp_v0;

    if (*(s32 *)(arg0 + 0x48) != 0) {
        func_002DE090(*(s32 *)(arg0 + 0x48));
    }
    temp_v0 = func_002DDF48(arg1);
    *(u32 *)(arg0 + 0x48) = temp_v0;
}

void func_002E8708(s32 arg0) {
    if (*(s32 *)(arg0 + 0x4c) != 0) {
        func_002DC040(*(s32 *)(arg0 + 0x4c));
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E8738);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E8770);

void func_002E9668(u32 arg0) {
    func_002E8738();
    func_002E8770(arg0);
}

void func_002E9690(s32 arg0) {
    func_002DC0C0(*(u32 *)(arg0 + 0x4c));
}

void func_002E96A8(s32 arg0) {
    func_002DC0F0(*(u32 *)(arg0 + 0x4c));
}

void func_002E96C0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 4) = arg1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E96C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E96E8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E9840);

void func_002E9A68(s32 arg0) {
    func_00333918(*(u32 *)(arg0 + 0x3c));
    func_003297C8(*(u32 *)(arg0 + 0x48));
}

void func_002E9A98(s32 arg0) {
    *(u32 *)(arg0 + 0x24) = 0;
    *(u32 *)(arg0 + 0x28) = 3;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E9AA8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E9B20);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E9BC8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E9E48);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E9E98);

void func_002EA0E8(u32 index) {
    u8 *entry = D_003E9CA8[index];
    ((void (*)(u8 *, u32))*(void **)(entry + 0x10))(entry, D_00437E68);
    D_00437E68 = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EA120);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EA3B0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EA410);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EA530);

void func_002EA5D0(s32 arg0) {
    *(u32 *)(arg0 + 0x2c) = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EA5D8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EAB78);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EAB88);

void func_002EABA0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002EABA8(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EABB0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EAC38);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EAFE8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EB058);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EB728);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EB908);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EB968);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EBB88);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EBBF8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EBC58);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EBE28);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EBF40);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EC300);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EC370);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002ECD10);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002ECEF0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002ECF78);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002ED360);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002ED3D0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EDB10);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EDCF0);

void func_002EDDE0(s32 arg0) {
    u64 temp_v0;

    temp_v0 = func_002D3468();
    func_002EDCF0(*(u16 *)(arg0 + 0xc), temp_v0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EDE10);

void func_002EDE58(s32 arg0) {
    func_002EDCF0(*(u16 *)(arg0 + 0x2c), *(u32 *)(arg0 + 0x34));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EDE78);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EDEC0);

void func_002EDF20(s32 arg0) {
    D_003E9D10[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
}

void func_002EDF58(u32 arg0) {
    func_002EDEC0();
    func_002EDF20(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EDF80);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EDF90);

void func_002EDFA8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002EDFB0(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EDFB8);

void func_002EE0A8(s32 arg0) {
    func_00333918(*(u32 *)(arg0 + 0x18));
    func_003297C8(*(u32 *)(arg0 + 0x1c));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EE0D8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EE348);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EE3C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EE400);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EE4A8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EE508);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EE5A8);

void func_002EE608(s32 arg0) {
    if (*(s32 *)(arg0 + 0x34) != 0) {
        func_002DC040(*(s32 *)(arg0 + 0x34));
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EE638);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EE670);

void func_002EED48(u32 arg0) {
    func_002EE638();
    func_002EE670(arg0);
}

void func_002EED70(s32 arg0) {
    func_002DC0C0(*(u32 *)(arg0 + 0x34));
}

void func_002EED88(s32 arg0) {
    func_002DC0F0(*(u32 *)(arg0 + 0x34));
}

void func_002EEDA0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 4) = arg1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EEDA8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EEDC8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EEE18);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EEE88);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EF0D0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EF130);

void func_002EF190(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x30);
    func_002F17B8(*(u32 *)(temp_v0 + 4));
    func_003297C8(*(u32 *)(temp_v0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EF1C0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EF8C0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EFA28);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EFA78);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EFAE8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EFD30);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EFD90);

void func_002EFDF0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x30);
    func_002F17B8(*(u32 *)(temp_v0 + 4));
    func_003297C8(*(u32 *)(temp_v0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EFE20);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F0530);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F0698);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F06E8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F0760);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F09A8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F0A08);

void func_002F0A68(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x30);
    func_002F17B8(*(u32 *)(temp_v0 + 4));
    func_003297C8(*(u32 *)(temp_v0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F0A98);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F10C0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F1228);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F12E0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F1380);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F13E0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F1428);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F14B8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F1500);

void func_002F1560(s32 arg0) {
    D_003E9DEC[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
}

void func_002F1598(u32 arg0) {
    func_002F1500();
    func_002F1560(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F15C0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F15D0);

void func_002F15E8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002F15F0(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F15F8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F1740);

void func_002F17B8(s32 arg0) {
    if (*(s32 *)(arg0 + 0x18) == 0) {
        D_00437E74 = D_00437E74 - 1;
        if (D_00437E74 == 0) {
            func_002DDCC8(D_00437E78);
            D_00437E78 = 0;
        }
    }
    else {
        func_002DDCC8(*(s32 *)(arg0 + 0x18));
    }
    func_00333918(*(u32 *)(arg0 + 0x2c));
    func_003297C8(*(u32 *)(arg0 + 0x30));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F1820);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F1888);

void func_002F1C98(void) {
    D_00437E6C = func_00343ED0("/effect/wind00.tmx", &D_00437E70, 0);
}

u32 func_002F1CC8(void) {
    return D_00437E70;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F1CD0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F1D20);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F1D90);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F1F70);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F1FC0);

void func_002F2020(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x30);
    func_002F3F08(*(u32 *)(temp_v0 + 4));
    func_003297C8(*(u32 *)(temp_v0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F2050);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F2760);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F28C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F2950);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F2978);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F29D8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F2A30);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F2A70);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F2AE8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F3100);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F3168);

void func_002F31D0(s32 arg0) {
    *(u32 *)(*(s32 *)(*(s32 *)(arg0 + 0x30) + 4) + 8) = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F31E0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F3258);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F36A0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F36F0);

void func_002F3750(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x30);
    func_002F3F08(*(u32 *)(temp_v0 + 4));
    func_003297C8(*(u32 *)(temp_v0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F3780);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F37F8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F3988);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F3A40);

void func_002F3AD0(s32 arg0) {
    u64 temp_v0;

    temp_v0 = func_002D3468();
    func_002F3A40(*(u16 *)(arg0 + 0xc), temp_v0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F3B00);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F3B48);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F3BD8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F3C20);

void func_002F3C80(s32 arg0) {
    D_003E9E74[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
}

void func_002F3CB8(u32 arg0) {
    func_002F3C20();
    func_002F3C80(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F3CE0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F3CF0);

void func_002F3D08(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002F3D10(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

u32 func_002F3D18(void) {
    if (D_00437E84 == 0) {
        D_00437E88 = func_002DDCA0(D_00437E80, 0x200);
    }
    D_00437E84 = D_00437E84 + 1;
    return D_00437E88;
}

void func_002F3D58(void) {
    D_00437E84 = D_00437E84 - 1;
    if (D_00437E84 == 0) {
        func_002DDCC8(D_00437E88);
        D_00437E88 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F3D88);

u64 func_002F3ED8(void) {
    u64 temp_v0;

    temp_v0 = func_002F3D88();
    func_002F3D18();
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F3F08);

void func_002F3F40(u8 *work) {
    func_002F3D88(*(u32 *)work, *(u32 *)(work + 0x10));
    D_00437E84++;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F3F70);

void func_002F4570(void) {
    D_00437E7C = func_00343ED0("/effect/scaly00.tmx", &D_00437E80, 0);
}

u32 func_002F45A0(void) {
    return D_00437E80;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F45A8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F46D8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F48E8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F4960);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F5168);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F5358);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F5488);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F54D0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F5520);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F55F0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F5638);

void func_002F5698(s32 arg0) {
    D_003E9F18[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
}

void func_002F56D0(u32 arg0) {
    func_002F5638();
    func_002F5698(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F56F8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F5708);

void func_002F5720(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002F5728(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F5730);

void func_002F5820(s32 arg0) {
    func_00333918(*(u32 *)(arg0 + 0x18));
    func_003297C8(*(u32 *)(arg0 + 0x1c));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F5850);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F5AC0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F5BC8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F5D70);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F5EF0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F6000);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F6450);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F64D8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F66F0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F6790);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F67D8);

void func_002F69B0(u8 *work) {
    u32 *objects = (u32 *)(*(u32 *)(work + 0x30) + 0x24);
    u32 i;
    for (i = 0; i < 5; i++) {
        u32 object = objects[i];
        if (object != 0) {
            *(u32 *)object = 0;
            *(u32 *)(object + 0x0C) = 0;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F69F0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F6A80);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F6BE8);

u64 func_002F6C30(s32 arg0) {
    u64 temp_v0;

    temp_v0 = func_002F69F0(*(u32 *)(arg0 + 0x38), **(u32 **)(arg0 + 0x30),
                                                (*(u32 **)(arg0 + 0x30))[1]);
    func_002F6A80(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F6C70);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F6D00);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F7128);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F7270);

void func_002F72D8(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    if ((*(u32 *)(temp_v0 + 0x218) & 0x6000000) == 0x6000000) {
        func_00200930(temp_v0 + 0x50, temp_v0 + 0x60, 0);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F7320);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F7538);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F7580);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F7720);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F7758);

void func_002F7798(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    if ((*(u32 *)(temp_v0 + 0x218) & 0x6000000) == 0x6000000) {
        func_002F7538();
        return;
    }
}

void func_002F77D0(s32 arg0) {
    s32 temp_v0;
    u32 temp_v1;
    u32 *puVar3;
    u8 *puVar4;
    u32 temp_v2;

    temp_v2 = 0;
    func_001AA6F8();
    temp_v0 = *(s32 *)(arg0 + 0x28);
    puVar4 = (u8 *)(*(s32 *)(arg0 + 0x38) + 0x18);
    puVar3 = (u32 *)(*(s32 *)(arg0 + 0x38) + 0x10);
    do {
        temp_v1 = puVar3[-4];
        if (temp_v1 < *puVar3) {
            return;
        }
        if (temp_v0 == 0) {
            func_002F7758(temp_v2, *puVar4, puVar3[-2]);
            temp_v1 = puVar3[-4];
        }
        if ((temp_v1 != 0) && (temp_v0 == temp_v1 - *puVar3)) {
            func_002F7720(temp_v2, *puVar4, *puVar3);
        }
        temp_v2 = temp_v2 + 1;
        puVar4 = puVar4 + 1;
        puVar3 = puVar3 + 1;
    } while (temp_v2 < 2);
}

void func_002F7890(void) {
    func_002005B0(0xc);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F78A8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F7928);

void func_002F79E0(void) {
    u32 *puVar1;

    puVar1 = (u32 *)func_00328D68(4);
    *puVar1 = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F7A00);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F7A48);

void func_002F7A90(u32 arg0) {
    if (*(s32 *)arg0 != 0) {
        func_002EDE10(*(s32 *)arg0);
    }
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F7AC8);

void func_002F7C40(s32 arg0) {
    func_002EDF20(**(u32 **)(arg0 + 0x30));
}

void func_002F7C60(void) {
    u32 *puVar1;

    puVar1 = (u32 *)func_00328D68(4);
    *puVar1 = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F7C80);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F7CF0);

void func_002F7D38(u32 arg0) {
    if (*(s32 *)arg0 != 0) {
        func_002E7D08(*(s32 *)arg0);
    }
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F7D70);

void func_002F7E88(s32 arg0) {
    func_002E8770(**(u32 **)(arg0 + 0x30));
}

void func_002F7EA8(void) {
    u32 *puVar1;

    puVar1 = (u32 *)func_00328D68(4);
    *puVar1 = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F7EC8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F7F58);

void func_002F8008(u32 arg0) {
    if (*(s32 *)arg0 != 0) {
        func_002DC260(*(s32 *)arg0);
    }
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F8040);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F81A8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F81D0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F8270);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F8328);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F8378);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F84F8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F8590);

void func_002F85D8(void) {
    func_002F8590();
}

void func_002F85F0(s32 arg0) {
    func_002F8590(*(u32 *)(arg0 + 0x38));
}

void func_002F8608(u32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)((s32)arg0 + 4);
    if (temp_v0 != 0) {
        func_00333918(temp_v0);
    }
    func_00328E48(arg0);
}

void func_002F8640(void) {
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F8648);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F8C38);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F8CF0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F8FB8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F9048);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F9118);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F9180);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F91D0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F9550);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F9608);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F96D0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F9720);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F9780);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F9860);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F98C0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F9930);

void func_002F9988(u32 arg0) {
    func_002F98C0();
    func_002F9930(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F99B0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F99C0);

void func_002F99D8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002F99E0(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F99E8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F9BE8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F9C38);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F9D58);

void func_002F9DE8(s32 arg0) {
    *(u32 *)(arg0 + 8) = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F9DF0);

void func_002FA388(u32 *arg0, u32 arg1) {
    *arg0 = arg1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FA390);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FA4D0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FA550);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FA5B8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FA5F0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FA778);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FA848);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FA978);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FAAE0);

void func_002FABA8(s32 arg0, u32 arg1, u32 arg2) {
    s32 temp_v0;
    s32 temp_v1;
    u32 temp_v2;

    if (*(s32 *)(arg0 + 0x60) != 0) {
        func_002DC260(*(s32 *)(arg0 + 0x60));
        *(u32 *)(arg0 + 0x60) = 0;
    }
    if (*(s32 *)(arg0 + 100) != 0) {
        func_00330978(*(s32 *)(arg0 + 100), 1, 1);
        *(u32 *)(arg0 + 100) = 0;
    }
    temp_v1 = func_002DC1D0(arg1, arg2);
    temp_v0 = *(s32 *)(temp_v1 + 0xc);
    *(s32 *)(arg0 + 0x60) = temp_v1;
    temp_v2 = func_00331940(*(u32 *)(temp_v0 + 0x14), *(u32 *)(temp_v0 + 0x18));
    *(u32 *)(arg0 + 100) = temp_v2;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FAC38);

void func_002FAD30(s32 arg0) {
    if (*(s32 *)(arg0 + 0x70) != 0) {
        func_002DC040(*(s32 *)(arg0 + 0x70));
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FAD60);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FAD98);

void func_002FB400(u32 arg0) {
    func_002FAD60();
    func_002FAD98(arg0);
}

void func_002FB428(s32 arg0) {
    func_002DC0C0(*(u32 *)(arg0 + 0x70));
}

void func_002FB440(s32 arg0) {
    func_002DC0F0(*(u32 *)(arg0 + 0x70));
}

void func_002FB458(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 4) = arg1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FB460);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FB480);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FB5C0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FB790);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FB840);

void func_002FB948(s32 arg0, s32 arg1) {
    *(u32 *)(arg0 + 0x478) = *(u32 *)(arg1 + 0x478);
    *(s16 *)(*(s32 *)(arg1 + 0x478) + 0x10) = *(s16 *)(*(s32 *)(arg1 + 0x478) + 0x10) + 1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FB968);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FB998);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC0A8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC0B8);

void func_002FC0D0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x20) = arg1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC0D8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC0E0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC110);

void func_002FC158(void) {
}

void func_002FC160(void) {
    func_00300048();
    func_00303C50();
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC180);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC198);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC240);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC2B0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC340);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC500);

void func_002FC5C8(u32 arg0) {
    if (D_004386B4 == 0) {
        func_002FC2B0();
        func_002D3710(arg0);
        return;
    }
}

void func_002FC610(u32 arg0) {
    if (D_004386B4 != 0) {
        func_002D4548(D_004386B4);
        D_004386B4 = 0;
    }
    if (D_004386C4 != 0) {
        func_002D3528(D_004386C4);
        D_004386C4 = 0;
    }
    func_002D3528(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC668);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC718);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC748);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC808);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC838);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC8F8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC9B8);

u32 func_002FCA40(void) {
    return D_003FFA84[0] + D_004386B0;
}

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BEA0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BEB0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BEC0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BED0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BEE0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BEF0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF00);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF10);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF20);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF30);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF40);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF50);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF60);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF70);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF80);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF90);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BFA0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BFB0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BFC0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BFD0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BFE0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BFF0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C000);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C010);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C020);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C030);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C040);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C050);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C060);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C070);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C080);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C090);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C0A0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C0B0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C0C0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C0E0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C100);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C120);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C140);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C160);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C180);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C1A0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C1C0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C1E0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C200);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C220);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C240);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C260);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C280);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C2A0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C2C0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C2E0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C300);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C320);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C340);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C360);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C380);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C3A0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C3C0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C3E0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C400);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C420);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C440);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C460);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C480);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C4A0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C4C0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C4E0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C500);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C520);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C548);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C568);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C588);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C5A8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C5C8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C5E8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C608);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C628);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C648);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C668);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C688);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C6A8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C6C8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C6E8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C708);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C728);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C748);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C768);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C788);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C7A8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C7C8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C7E8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C808);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C828);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C848);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C868);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C888);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C8A8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C8C8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C8E8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C908);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C928);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C948);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C968);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C988);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C9A8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C9C8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C9E8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CA08);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CA28);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CA48);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CA68);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CA88);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CAB0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CAD8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CB00);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CB28);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CB48);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CB68);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CB88);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CBA8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CBC8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CBE8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CC08);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CC28);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CC48);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CC68);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CC88);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CCA8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CCC8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CCE8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CD08);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CD28);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CD38);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CD48);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CD58);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CD68);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CD78);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CD88);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CD98);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CDA8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CDB8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CDC8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CDD8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CDE8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CDF8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE08);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE18);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE28);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE38);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE48);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE58);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE68);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE78);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE88);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE98);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CEA8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CEB8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CEC8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CED8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CEE8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CEF8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CF08);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CF18);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CF28);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CF38);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CF48);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CF58);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CF70);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CF80);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CF90);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FCA58);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FD748);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FD900);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FDC90);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FDD18);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FDD70);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FDDD8);

u32 func_002FE208(void) {
    D_00439075 = 1;
    func_002FE230(0);
    return 0;
}

u32 func_002FE230(void) {
    if (*(s32 *)(D_004386F0 + 0x34) != 0) {
        D_004386F4 = *(s32 *)(D_004386F0 + 0x34);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FE250);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FE400);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FE5B8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FE948);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FE970);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FE998);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FE9C0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FE9E8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FEA10);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FEA38);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FEA60);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FEA88);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FEAB0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FEAD8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FEB00);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FEB28);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FEB50);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FEB78);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FEBA0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FEBC8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FEBF0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FEC80);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D030);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D048);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FEDD0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FF080);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FF0B8);

void func_002FF338(void) {
    D_00438758 = D_0045C110[0x88];
    D_00438759 = D_0045C110[0x89];
    D_0043875A = D_0045C110[0x8A];
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FF360);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FF870);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FF8A0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FFBA0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FFDA0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FFE48);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00300048);

void func_003000B8(void) {
    if (D_00438760 != 0) {
        func_0020DF68(D_00438760);
        D_00438760 = 0;
    }
    if (D_0043875C != 0) {
        func_0020D8F0(D_0043875C);
        D_0043875C = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00300100);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003001C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00300238);

void func_003002A8(void) {
    if (D_00438768 != 0) {
        func_0020E368(D_00438768);
        D_00438768 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_003002D8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00300328);

void func_003003E0(void) {
    if (D_00438760 != 0) {
        func_0020DF68(D_00438760);
        D_00438760 = 0;
    }
    if (D_0043875C != 0) {
        func_0020D8F0(D_0043875C);
        D_0043875C = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00300428);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00300548);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00300578);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003012B0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00301318);

void func_00301380(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0xb8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_003013A0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003013C8);

void func_00301428(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x34));
}

void func_00301448(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x38));
}

void func_00301468(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x34));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00301488);

void func_003014E8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x34));
}

void func_00301508(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x34));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00301528);

void func_00301590(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x34));
}

void func_003015B0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0 + 0x3c, temp_v0 + 0x60, *(u32 *)(temp_v0 + 0x80));
}

void func_003015D8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x34));
}

void func_003015F8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0 + 0x3c, temp_v0 + 0x60, *(u32 *)(temp_v0 + 0x80));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00301620);

void func_00301688(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x70));
}

void func_003016A8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x34));
}

void func_003016C8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0 + 0x50, temp_v0 + 0x74, *(u32 *)(temp_v0 + 0x84));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_003016F0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00301750);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003017B8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00301820);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00301888);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003018C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003021A8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003021D8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302208);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302238);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302268);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302298);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003022C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003022F8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302328);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302358);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302388);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003023B8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003023E8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302418);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302448);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302478);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003024A8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003024D8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302508);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302538);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302568);

void func_00302598(void) {
    D_00438774 = 1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_003025A8);

void func_00302AD0(s32 arg0) {
    func_003025A8(*(s32 *)(arg0 + 0xc) + 0x3c, *(u32 *)(*(s32 *)(arg0 + 0xc) + 0x4c));
}

void func_00302AF0(s32 arg0) {
    func_003025A8(*(s32 *)(arg0 + 0xc) + 0x60, *(s32 *)(*(s32 *)(arg0 + 0xc) + 0x3c) + 1);
}

void func_00302B18(s32 arg0) {
    func_003025A8(*(s32 *)(arg0 + 0xc) + 0x70, *(s32 *)(*(s32 *)(arg0 + 0xc) + 0x8c) + 1);
}

void func_00302B40(s32 arg0) {
    func_003025A8(*(s32 *)(arg0 + 0xc) + 0x70, *(s32 *)(*(s32 *)(arg0 + 0xc) + 0x8c) + 1);
}

void func_00302B68(s32 arg0) {
    func_003025A8(*(s32 *)(arg0 + 0xc) + 0x60, *(s32 *)(*(s32 *)(arg0 + 0xc) + 0x74) + 1);
}

void func_00302B90(s32 arg0) {
    func_003025A8(*(u32 **)(arg0 + 0xc) + 4, **(u32 **)(arg0 + 0xc));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302BB0);

void func_00302C78(void) {
    func_00302BB0(0x4b);
}

void func_00302C90(void) {
    func_00302BB0(0x4b);
}

void func_00302CA8(void) {
    func_00302BB0(0xb);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302CC0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302D70);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302D90);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302DB0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302DD0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302DF0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302E10);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302E30);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302E50);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302E70);

u32 func_00302E90(void) {
    u32 zero = 0;
    func_002D39C8(D_004386C0, &zero, 4, 4);
    return 0x400002;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302EC8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302EE8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302F08);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302F28);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302F48);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00303130);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00303478);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D0B8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D0C8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D0D8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D0E8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D0F8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D108);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D118);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D128);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D140);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00303660);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00303848);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00303A30);

void func_00303C50(void) {
    D_0043876C = 0;
    D_00438774 = 1;
    D_00438770 = 0;
    D_00438778 = 0;
    D_004387A8 = 0;
    D_004387AC = 0;
    D_004387B0 = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00303C78);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00303D00);

void func_00303D58(void) {
    func_00328E48();
}

u32 func_00303D70(u32 *arg0) {
    return *arg0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00303D78);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00303E30);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00303E88);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304030);

void func_003040B8(u64 arg0, u32 *arg1) {
    u64 temp_v0;
    u32 temp_v1;

    temp_v0 = func_002C8108();
    temp_v1 = func_00305148(temp_v0, 0);
    *arg1 = temp_v1;
    func_003297C8(temp_v0);
    func_002C7D00(arg0);
}

void func_00304110(u64 arg0, u32 *arg1) {
    u64 temp_v0;
    u32 temp_v1;

    temp_v0 = func_002C8108();
    temp_v1 = func_00305148(temp_v0, 1);
    *arg1 = temp_v1;
    func_002C7D00(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304158);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003041F0);

void func_00304258(u64 arg0, u32 *arg1) {
    u64 temp_v0;
    u64 temp_v1;
    u32 temp_v2;

    temp_v0 = func_002C8108();
    temp_v1 = func_003298F8(temp_v0);
    temp_v2 = func_00304938(temp_v1);
    *arg1 = temp_v2;
    func_003297C8(temp_v0);
    func_002C7D00(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_003042B8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304310);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304360);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003043E8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304490);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304530);

u32 func_003045E8(u32 arg0) {
    func_00304490();
    func_00328E48(arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304618);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003046D0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304768);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304938);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304998);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304A38);

u32 func_00304AB8(s32 arg0) {
    func_003297C8(*(u32 *)(arg0 + 0x14));
    return 1;
}

s32 func_00304AD8(s32 arg0, s32 arg1) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = arg1 * 0xa0 + *(s32 *)(arg0 + 0x18);
    temp_v0 = *(s32 *)(temp_v1 + 0x9c);
    if (temp_v0 != 0) {
        temp_v1 = temp_v0;
    }
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304AF8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304B18);

void func_00304CE0(s32 arg0, s32 arg1) {
    func_00304B18(arg0, arg1, *(s32 *)(arg0 + 0x18) + arg1 * 0xa0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304D08);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304D60);

void func_00304D80(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)((s32)arg0 + 0x18) + (s32)arg1 * 0xa0;
    memset(temp_v0, 0, 0xa0);
    func_00304D60(arg0, arg1, temp_v0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304DE8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304EE0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304F40);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304FB0);

void func_00305068(u32 arg0) {
    func_00304FB0(arg0, 0);
}

u8 func_00305080(s32 arg0) {
    return **(s32 **)(arg0 + 0x24) != 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00305090);

u32 func_00305110(u32 arg0) {
    func_003297C8(*(u32 *)arg0);
    func_00328E48(arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00305148);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00305348);

u32 func_003054E8(u32 arg0) {
    s32 *piVar1;

    piVar1 = (s32 *)arg0;
    if (*piVar1 != 0) {
        func_003297C8(*piVar1);
    }
    if (piVar1[1] == 0) {
        func_00305068(arg0);
        func_003297C8(piVar1[8]);
    }
    func_003297C8(piVar1[3]);
    func_00304AB8(arg0);
    func_00328E48(arg0);
    return 1;
}

u32 func_00305558(u32 *arg0, u32 arg1, u32 arg2) {
    *arg0 = arg2;
    arg0[4] = arg1;
    if ((arg2 & 2) != 0) {
        func_00304AF8();
    }
    arg0[2] = arg0[2] + 1;
    return 1;
}

u32 func_003055A0(u32 arg0, s32 arg1, s32 arg2, u32 arg3) {
    func_00305558(arg0, *(s32 *)(arg1 + 8) + arg2 * 0x24, arg3);
    return 1;
}

u32 func_003055D8(s32 arg0) {
    *(u32 *)(arg0 + 0x10) = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_003055E8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00305650);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003056B0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00305848);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003058B0);

u32 func_00305950(s32 arg0, s32 arg1) {
    *(u32 *)(arg1 * 0xa0 + *(s32 *)(arg0 + 0x18) + 0x9c) = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00305970);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003059E0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00305A60);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00305B00);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00305B28);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00305BD0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00305C40);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00305EB0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00306030);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003064C0);

void func_00306500(u32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4, u32 arg5, u32 arg6, u32 arg7) {
    func_00305BD0(arg6, arg7);
    func_003089B8(arg0, arg1, arg2, arg3, arg4, arg5, arg7);
    func_00308478(0x44, arg7);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_003065A0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00306678);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00306970);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00306BF0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00306C28);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00306CD0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00306DC8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00306F80);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00307018);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00307160);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003071D0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00307270);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003072E8);

void func_00307340(s32 arg0, s32 arg1) {
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

INCLUDE_ASM(const s32, "game/code_002DC138", func_00307388);

void func_00307398(u32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4) {
    func_0032C9D8(arg4, arg0, 0, 0, arg1, arg2, arg3, 0);
}

u8 func_003073D0(s32 arg0) {
    return *(u8 *)(arg0 + 0x18);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_003073D8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00307428);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003074F0);

void func_003075A0(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(*(s32 *)(arg1 * 4 + *(s32 *)(arg0 + 0x24)) + 0x28);
    *(u64 *)(temp_v0 + 0x20) = (*(u64 *)(temp_v0 + 0x20) & 0x1fffffffffffffff) | 0x4000000000000000;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_003075D8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00307710);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003078A8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00307A68);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00307BA8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00307C30);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00307D70);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00307EF8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00308020);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00308058);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003081A8);

void func_00308288(u8 arg0, u32 arg1) {
    func_003081A8(arg0, 0, arg1);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_003082A8);

void func_00308380(u32 arg0, u32 arg1) {
    func_003082A8(arg0, 0, arg1);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_003083A0);

void func_00308478(u32 arg0, u32 arg1) {
    func_003083A0(arg0, 0, arg1);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00308498);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00308550);

void func_00308608(QuadU32 *q, u32 value) {
    q->x = value;
    q->y = value;
    q->z = value;
    q->w = value;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00308620);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00308650);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003087D8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00308808);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00308828);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003089B8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003089D8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00308AC8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00308AF0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00308C58);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00308DB0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00308E60);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00308F10);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00308F78);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00308FE8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00309050);

void func_00309090(u32 arg0) {
    func_00308380(0x30000, arg0);
    func_00308478(0x44, arg0);
    func_00308808(0, 0, 0, 0x2000, 0xe00, 0, arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_003090E8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00309138);

void func_003091E8(void) {
}

void func_003091F0(void) {
}

u32 func_003091F8(void) {
    return 0;
}

s32 func_00309200(s32 arg0) {
    if (arg0 == 0) {
        return 0;
    }
    *(s32 *)(arg0 + 0xC) = (*(s32 *)(arg0 + 0xC) & -2) | 2;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00309230);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00309278);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00309390);

u32 func_003093D0(u32 arg0) {
    s64 temp_v0;

    func_00328E48(*(u32 *)arg0);
    do {
        temp_v0 = func_00309638(arg0);
    } while (temp_v0 != 0);
    func_00328E48(arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00309418);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00309480);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003094C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00309538);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00309638);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003097D0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00309880);

void func_00309A20(u32 arg0, u32 arg1, s32 arg2) {
    s32 temp_v0;
    s32 temp_v1;
    float *pfVar3;
    float temp_v2;
    float temp_v3;

    temp_v1 = func_0030A048(arg1, arg0);
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

void func_00309A90(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = (s32)arg0;
    if ((*(u32 *)(temp_v0 + 0xc) & 1) != 0) {
        func_00309A20(arg0, (u32)*(u16 *)(*(s32 *)(temp_v0 + 0x18) + 6) + *(s32 *)(temp_v0 + 0x3c),
                                    arg1);
        return;
    }
}

void func_00309AD8(u32 arg0, u32 arg1, s32 arg2) {
    s32 temp_v0;
    s32 temp_v1;
    float *pfVar3;
    float temp_v2;
    float temp_v3;

    temp_v1 = func_0030A048(arg1, arg0);
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

void func_00309B48(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = (s32)arg0;
    if ((*(u32 *)(temp_v0 + 0xc) & 1) != 0) {
        func_00309AD8(arg0, (u32)*(u16 *)(*(s32 *)(temp_v0 + 0x18) + 6) + *(s32 *)(temp_v0 + 0x3c),
                                    arg1);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00309B90);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D170);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D180);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D190);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D1A0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D1B0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D1C0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D1D0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D1E0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D1F0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00309C00);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00309DF8);

s32 func_0030A048(u32 key, u32 head) {
    u32 n;

    n = *(u32 *)(head + 0x14);
    while (n != 0 && *(u16 *)(n + 6) != key) {
        n = *(u32 *)(n + 0x1C);
    }
    return n;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030A070);

void func_0030A0E8(u32 arg0) {
    func_0030A070(0, arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030A108);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030A128);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030A160);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030A1D0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030A270);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030A300);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030A378);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030A3E8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030A528);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030A700);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030A7C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030A8A8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030A970);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030AA10);

s32 func_0030AA40(void) {
    return func_00101740((u32)D_0042D240) != 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030AA68);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D240);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030AAB0);

void func_0030AB20(s32 arg0) {
    s64 temp_v0;
    u32 temp_v1;

    D_0043908C = 0;
    D_004388AC = 1;
    D_00439090 = 0;
    D_004388B0 = 0;
    temp_v0 = func_0023A170(0x1c);
    temp_v1 = 3;
    if (temp_v0 == 0) {
        temp_v0 = func_0023A170(0x13);
        temp_v1 = 2;
        if (temp_v0 == 0) {
            temp_v1 = 1;
        }
    }
    *(u32 *)(arg0 + 8) = temp_v1;
    *(u32 *)(arg0 + 4) = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030AB88);

u8 func_0030ABF0(void) {
    s64 temp_v0;

    temp_v0 = func_0030AC10();
    return temp_v0 != 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030AC10);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030B078);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030B1E8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030B470);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030B568);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030B600);

u32 func_0030B678(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030B680);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030B6D8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030B728);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030B7D0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030B838);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030B880);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030BA98);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030BBA8);

void func_0030BC30(u32 arg0) {
    func_0030BBA8();
    D_004388B8 = arg0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030BC58);

void func_0030BCA8(void) {
    if (D_004388B8 != 0) {
        func_0030BBA8();
        D_004388B8 = D_004388B8 - 1;
    } else {
        func_0030BBA8();
        D_004388B8 = D_004388BC - 1;
    }
}

s8 func_0030BCE8(void) {
    return D_004388C0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030BCF0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030BD10);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030BED8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030C0C0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030C250);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030C378);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030C568);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030C5D8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030C640);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030C660);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030C690);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030C8E8);

u32 func_0030C9A0(void) {
    return **(u32 **)(*(s32 *)(D_004388C4 + 0x1c) + 0x70);
}

s32 func_0030C9B8(void) {
    return *(s16 *)(*(s32 *)(*(s32 *)(D_004388C4 + 0x1c) + 0x70) + 8);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030C9D0);

float func_0030CA08(void) {
    s32 p;

    p = *(s32 *)(D_004388C4 + 0x30);
    return (float)(*(s32 *)p) / 10.0f;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030CA38);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030CC68);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030CEF0);

void func_0030D278(void) {
    s32 temp_v0;

    temp_v0 = **(s32 **)(D_004388C4 + 0x30);
    if (temp_v0 < 10) {
        **(s32 **)(D_004388C4 + 0x30) = temp_v0 + 1;
    }
}

void func_0030D2A0(void) {
    s32 temp_v0;

    temp_v0 = **(s32 **)(D_004388C4 + 0x30);
    if (temp_v0 != 0) {
        **(s32 **)(D_004388C4 + 0x30) = temp_v0 - 1;
    }
}

void func_0030D2C0(s32 arg0) {
    s32 ptr;

    ptr = *(s32 *)(D_004388C4 + 0x30);
    *(s16 *)(ptr + 6) = arg0;
    *(s16 *)(ptr + 4) = 8;
}

void func_0030D2D8(void) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(D_004388C4 + 0x30);
    if (0 < *(s16 *)(temp_v0 + 4)) {
        *(s16 *)(temp_v0 + 4) = *(s16 *)(temp_v0 + 4) - 1;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030D300);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030D350);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030D3A8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030D3C0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030D408);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030D438);

void func_0030D4F8(void) {
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030D500);

u64 func_0030D780(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0019F448(0, 0, 0, 0, arg0, 0);
    temp_v1 = func_0019D958(temp_v0);
    func_0019C5B0(temp_v0);
    return temp_v1;
}

s32 func_0030D7E0(s32 x, s32 n) {
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

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030D818);

void func_0030D8A0(s32 arg0, s32 arg1) {
    if ((arg1 <= *(s32 *)(arg0 + 0x20)) && (arg1 != 0)) {
        *(s32 *)(arg0 + 0xc) = arg1;
    }
}

void func_0030D8C0(s32 arg0) {
    if (*(s32 *)(arg0 + 0xc) < 10) {
        *(s32 *)(arg0 + 0xc) = *(s32 *)(arg0 + 0xc) + 1;
    }
}

void func_0030D8E0(s32 arg0) {
    if (1 < *(s32 *)(arg0 + 0xc)) {
        *(s32 *)(arg0 + 0xc) = *(s32 *)(arg0 + 0xc) - 1;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030D900);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030D938);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030DAA0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030DAE8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030DB40);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030DBF0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030DE08);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030E010);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030E030);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030E0C8);

void func_0030E130(void) {
    s32 temp_v0;

    D_00439098 = 0;
    temp_v0 = func_0030C9B8();
    D_0043909C = temp_v0 - 1;
    D_004390A0 = 0x3c;
}

void func_0030E160(void) {
    if ((s32)D_00439098 < 0x3C) {
        D_00439098++;
    }
}

void func_0030E180(void) {
    if ((s32)D_00439098 > 0) {
        D_00439098 -= 2;
    } else {
        D_00439098 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030E1A0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030E390);

void func_0030E878(void) {
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030E880);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030E8E8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030E910);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030E940);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030E958);

void func_0030EA70(void) {
    D_0045C7B0[0] = D_0045C7A0[0];
    D_0045C7B0[1] = D_0045C7A0[1];
    D_0045C7B0[2] = D_0045C7A0[2];
    D_004390A4 = 1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030EAA8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030ECC0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030EE40);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030EF18);

void func_0030EF38(s32 arg0, u32 arg1, u32 arg2, u32 arg3) {
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

void func_0030EF88(s32 arg0, u16 arg1) {
    *(u16 *)(arg0 + 0x14) = arg1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030EF90);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030F038);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030F0F8);

u32 func_0030F160(s32 *arg0) {
    if (*arg0 != 0) {
        func_0032BBB0(*arg0);
        *arg0 = 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030F1A0);

void func_0030F2A8(s32 arg0, s32 arg1, u64 arg2, u64 arg3) {
    u64 temp_v0;

    temp_v0 = func_0019F460(arg0 << 4, arg1 << 3, 0, arg2, arg3, 0);
    func_0019D530(temp_v0, 1);
    func_0019C5B0(temp_v0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030F2F8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030F390);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030F420);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030F4B8);

void func_0030F800(float *arg0, float *arg1) {
    *arg0 = *arg0 + *arg1;
    arg0[1] = arg0[1] + arg1[1];
    arg0[2] = arg0[2] + arg1[2];
}

void func_0030F838(float *arg0, float *arg1) {
    *arg0 = *arg0 - *arg1;
    arg0[1] = arg0[1] - arg1[1];
    arg0[2] = arg0[2] - arg1[2];
}

void func_0030F870(float arg0, float arg1, float arg2, float *arg3) {
    *arg3 = *arg3 + arg0;
    arg3[1] = arg3[1] + arg1;
    arg3[2] = arg3[2] + arg2;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030F898);

void func_0030F8A8(float arg0, float *arg1) {
    *arg1 = *arg1 * arg0;
    arg1[1] = arg1[1] * arg0;
    arg1[2] = arg1[2] * arg0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030F8D0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030F938);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030F968);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030FA08);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030FA28);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030FAF0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030FD50);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0030FFB0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00310210);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00310320);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003103C8);

float func_00310608(float x, float y) {
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

INCLUDE_ASM(const s32, "game/code_002DC138", func_00310648);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00310888);

void func_003109D0(float *arg0, float *arg1, float *arg2) {
    *arg0 = *arg1 + *arg2;
    arg0[1] = arg1[1] + arg2[1];
    arg0[2] = arg1[2] + arg2[2];
    arg0[3] = arg1[3] + arg2[3];
}

void func_00310A18(float *arg0, float *arg1, float *arg2) {
    *arg0 = (arg1[3] * *arg2 + *arg1 * arg2[3] + arg1[1] * arg2[2]) -
                          arg1[2] * arg2[1];
    arg0[1] = (arg1[3] * arg2[1] + arg1[1] * arg2[3] + arg1[2] * *arg2) -
                              *arg1 * arg2[2];
    arg0[2] = (arg1[3] * arg2[2] + arg1[2] * arg2[3] + *arg1 * arg2[1]) -
                              arg1[1] * *arg2;
    arg0[3] = ((arg1[3] * arg2[3] - *arg1 * *arg2) - arg1[1] * arg2[1]) -
                              arg1[2] * arg2[2];
}

float func_00310B20(float *arg0, float *arg1) {
    return *arg0 * *arg1 + arg0[1] * arg1[1] + arg0[2] * arg1[2] +
                  arg0[3] * arg1[3];
}

float func_00310B60(float *arg0, float *arg1) {
    return (arg0[1] * arg1[2] - arg0[2] * arg1[1]) +
                  (arg0[2] * *arg1 - *arg0 * arg1[2]) +
                  (*arg0 * arg1[1] - arg0[1] * *arg1);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00310BA8);

float func_00310BC8(float *arg0) {
    return *arg0 * *arg0 + arg0[1] * arg0[1] + arg0[2] * arg0[2] +
                  arg0[3] * arg0[3];
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00310BF8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00310C18);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00310CB0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00310D28);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00310DE8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00310EB0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00310FD0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003110F0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00311178);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00311340);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003114A0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00311538);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003115F0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003116D0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003117F0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00311888);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003118E8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00311978);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00311A90);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00311AE8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00311B58);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00311C50);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00311D00);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00311DB0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00311E60);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00311F20);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003120B8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00312130);

void func_00312178(s32 arg0, s32 arg1) {
    if (arg1 != 0) {
        *(s32 *)(arg0 + 0x18) = (s32)arg1;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00312188);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00312228);

void func_00312310(s32 arg0, s32 arg1) {
    if (arg1 != 0) {
        *(s32 *)(arg0 + 0x14) = (s32)arg1;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00312320);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003123D0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00312438);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003124B0);

void *func_00312550(void *head, s32 key) {
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

INCLUDE_ASM(const s32, "game/code_002DC138", func_00312578);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00312620);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003126D0);

u8 func_00312710(s32 arg0) {
    u8 temp_v0;
    s64 temp_v1;

    temp_v0 = 0;
    if (arg0 != 0) {
        temp_v1 = func_00101740(*(u32 *)((s32)arg0 + 4));
        temp_v0 = temp_v1 != 0;
    }
    return temp_v0;
}

s32 func_00312738(u32 name) {
    return func_00101740(name) != 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00312758);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003127B0);

s32 func_003127E8(void *p, s32 key) {
    void *r;

    r = func_00312550(*(void **)((s32)p + 0xC), key);
    if (r != NULL) {
        return *(s32 *)((s32)r + 0x10);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00312810);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D418);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00312850);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00312910);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00312A00);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00312A48);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00312B10);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00312B50);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00312B70);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00312C78);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00312D48);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00312DA0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00312DF8);

void func_00312E20(void) {
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00312E28);

void func_00312F58(ShortPair2C *p, s32 a, s32 b) {
    p->h2C = a;
    p->h2E = b;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00312F68);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00312FB0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00312FD0);

void func_00313008(void *p, u32 *mod, u32 *div) {
    u32 *t = *(u32 **)((s32)p + 8);
    *mod = *t % *(u32 *)((s32)p + 0x14);
    *div = *t / *(u32 *)((s32)p + 0x14);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00313040);

void func_00313078(s32 arg0, s32 arg1, s32 arg2, u32 arg3) {
    u32 temp_v0;

    temp_v0 = arg2 * *(s32 *)(arg0 + 0x14) + arg1;
    if (temp_v0 < *(u32 *)(arg0 + 0x10)) {
        *(u32 *)(temp_v0 * 8 + *(s32 *)(arg0 + 4) + 4) = arg3;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_003130B0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00313100);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00313158);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003131B0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00313210);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00313280);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003133C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00313538);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003136A0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00313810);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00313898);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003139D8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00313A58);

float func_00313B90(float arg0, float arg1, float arg2) {
    return arg0 + arg1 * arg2;
}

u32 func_00313BA0(void) {
    return 0;
}

void func_00313BA8(void) {
}

u32 func_00313BB0(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00313BB8);

void func_00313C40(void) {
    memset(D_00435DD0 + 0x17210, 0, 0x5800);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00313C70);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00313D80);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00313F88);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00314020);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003140C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00314200);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00314298);

void func_003144E8(u32 arg0) {
    func_00314298(arg0, 0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00314500);

u32 func_00314668(u32 arg0, s32 *arg1) {
    *arg1 = D_00435E50 + (arg0 & 0xffff) * 0x13;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00314690);

void func_003146B8(u32 arg0, u16 arg1, u32 arg2) {
    u32 *puVar1;

    puVar1 = (u32 *)func_00314B80(arg0, arg1);
    *puVar1 = arg2;
}

void func_003146E8(u32 arg0, u16 arg1) {
    u32 *puVar1;
    u32 temp_v0;

    puVar1 = (u32 *)func_00314B80(arg0, arg1);
    temp_v0 = func_00314690(arg1);
    *puVar1 = temp_v0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00314728);

void func_003147A8(u32 v, u32 *a, u32 *b) {
    u32 lo;

    v &= 0xFFFF;
    lo = v & 0xF;
    v >>= 4;
    *a = v;
    *b = lo << 1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_003147C8);

void func_00314838(void) {
    memset(D_00435DD0 + 0x16f10, 0, 0x300);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00314868);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00314990);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00314A08);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00314A80);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00314B00);

u8 func_00314B78(s32 arg0) {
    return *(u8 *)(arg0 + 0x55);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00314B80);

u32 func_00314BC0(u32 arg0, u16 arg1) {
    u32 *puVar1;

    puVar1 = (u32 *)func_00314B80(arg0, arg1);
    return *puVar1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00314BE0);

u8 func_00314C10(s32 arg0) {
    return *(u8 *)(arg0 + 0x55);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00314C18);

void func_00314C48(s32 unused, u32 v, u32 *a, u32 *b) {
    u32 lo;

    v &= 0xFFFF;
    lo = v & 7;
    v >>= 3;
    *a = v;
    *b = lo << 2;
}

void func_00314C68(s32 arg0) {
    memset(arg0 + 0x58, 0, 0x154);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00314C88);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00314CE8);

void func_00314D90(void) {
    u32 temp_v0;
    s32 temp_v1;

    memset(D_00435DD0 + 0x16ef0, 0, 0x10);
    temp_v0 = func_002C4C28(0x10, 0);
    temp_v1 = func_002C4C28(0x10, 1);
    for (; (s32)temp_v0 < temp_v1; temp_v0 = temp_v0 + 1) {
        func_001B7940(temp_v0 & 0xffff, 1);
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00314E20);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00314E80);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00314ED8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00314F30);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00314F90);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00314FE0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00315030);

s32 func_00315098(s32 arg0, s32 arg1) {
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

INCLUDE_ASM(const s32, "game/code_002DC138", func_003150D8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00315118);

u32 func_00315138(u8 *work) {
    u16 *entries = (u16 *)(work + 0x22);
    u32 count = 0;
    u32 index;
    for (index = 0; index < 24; index++) {
        if (entries[index] != 0) {
            count++;
        }
    }
    return count;
}

u16 func_00315170(s32 arg0, s32 arg1, u16 arg2) {
    u16 temp_v0;
    u16 *puVar2;

    puVar2 = (u16 *)(arg1 * 2 + arg0 + 0x22);
    temp_v0 = *puVar2;
    *puVar2 = arg2;
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00315188);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003151D0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003151F8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00315220);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00315248);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00315270);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003152D8);

u32 func_00315318(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00315320);

void func_00315350(u32 arg0, u32 arg1, u32 arg2) {
    memset(arg2, 0, 8);
}

u8 func_00315370(s32 arg0, u16 arg1) {
    return *(u8 *)(arg0 + 0x55) == arg1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00315388);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003154A0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00315628);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00315640);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00315680);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00315700);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003157A0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00315838);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00315920);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00315938);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00315950);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00315A50);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00315BF8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00315C40);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00315C68);

void func_00315FA0(u32 arg0, u32 arg1, u16 arg2) {
    func_00315C68(arg0, 0, arg1, arg2, 0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00315FC8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00315FF0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00316020);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00316118);

u8 func_003161E8(s32 i) {
    return D_00404A90[i].v0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00316208);

u32 func_00316260(s32 i) {
    return D_00404AA4[i].v0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00316280);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003162D8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00316308);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00316350);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003163A0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003163F0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00316430);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00316450);

u8 *func_003164C0(void) {
    return D_00405CA8;
}

void func_003164D0(s32 arg0) {
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

INCLUDE_ASM(const s32, "game/code_002DC138", func_00316528);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00316648);

void func_00316668(s32 arg0) {
    func_003297C8(*(u32 *)(arg0 + 0x54));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00316680);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00316C88);

float func_00316DD0(ScrVmOperand *op) {
    return op->f50;
}

void func_00316DD8(ScrVmOperand *op, float value) {
    op->f50 = value;
}

void func_00316DE0(s32 arg0) {
    func_002D7458(arg0 + 0x14, arg0 + 0x38, 0, 0);
}

void func_00316E08(RgbAlpha *p, u32 color) {
    p->rgb18 = color & 0xFFFFFF;
    p->alpha38 = color >> 24;
}

u32 func_00316E28(s32 arg0) {
    return *(u32 *)(arg0 + 0x3c);
}

void func_00316E30(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x3c) = arg1;
}

void func_00316E38(RgbAlpha *dst, CfSrc *src) {
    dst->rgb18 = src->x04;
    dst->f50 = src->f3C;
    dst->alpha38 = src->x24;
    dst->x3C = src->x28;
}

void func_00316E60(void) {
    D_00438918 = 1;
}

void func_00316E70(void) {
    D_00438918 = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00316E78);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00316ED0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00316EF8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00316F40);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00316FA8);

u32 func_00316FC8(void) {
    u32 temp_v0;
    s64 temp_v1;

    temp_v1 = func_00317FE0(D_0043891C);
    if (temp_v1 == -1) {
        func_00128658();
        temp_v0 = 0xffffffff;
    }
    else {
        func_00318068(D_0043891C);
        temp_v0 = 0;
    }
    return temp_v0;
}

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D4D0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00317010);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00317058);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00317958);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00317988);

u32 func_00317AC8(void) {
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D7A0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00317AD0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00317E48);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00317FE0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00318068);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003180B8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00318570);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00318660);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00318C00);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003191B0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00319388);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00319A58);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00319E48);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00319F48);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00319FF0);

void func_0031A090(void) {
    if ((D_00438930 != 0) && ((*(u32 *)(D_00438930 + 4) & 1) != 0)) {
        *(u32 *)(D_00438930 + 4) = *(u32 *)(D_00438930 + 4) | 0x800;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031A0B8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031A288);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031A638);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031A690);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031A730);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031A770);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031A7F8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031A830);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031A920);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031AA10);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031AD00);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031ADD8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031AE48);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031AEB8);

void func_0031AF58(s32 arg0) {
    *(u32 *)(arg0 + 0x1d4) = 0;
}

void func_0031AF60(void) {
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031AF68);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031B080);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031B0F8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031B188);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031B1F8);

void func_0031B268(void) {
    if (D_00438940 != (u32 *)0x0) {
        func_003297C8(*D_00438940);
        D_00438940 = (u32 *)0x0;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031B290);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031B2E0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031B328);

void func_0031B3B0(s32 arg0) {
    *(u16 *)(arg0 + 0x1da) = 0;
}

void func_0031B3B8(s32 arg0) {
    *(u16 *)(arg0 + 0x1da) = 1;
}

void func_0031B3C8(void) {
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031B3D0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031B4F0);

void func_0031B5F8(s32 *arg0) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *arg0;
    if (0 < arg0[1]) {
        do {
            memset(temp_v0, 0, 0x20);
            temp_v1 = (temp_v1 + 1) & 0xffff;
            temp_v0 = temp_v0 + 0x20;
        } while ((s32)temp_v1 < arg0[1]);
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031B668);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031B6D0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031B748);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031B838);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031B960);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031BA28);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031BB80);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031BBB0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031BC10);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031BDE8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031BFA0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031BFC0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031BFE0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031C0F8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031C1A0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031C208);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031C280);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031C348);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031C3C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031C458);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031C4A0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031C4E8);

void func_0031C578(s32 arg0) {
    *(u32 *)(arg0 + 0x44) = 0;
    **(u32 **)(arg0 + 0x40) = **(u32 **)(arg0 + 0x40) | 1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031C590);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031C5A0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031C5B8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031C5E8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031C630);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031C688);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031C850);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031C888);

void func_0031C8A8(void) {
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031C8B0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031C900);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031C940);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031CA10);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031CAE8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031CBC8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031CDE8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031CE60);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031CF68);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031CF88);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031D120);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031D260);

void func_0031D380(s32 *arg0) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *arg0;
    if (0 < arg0[1]) {
        do {
            memset(temp_v0, 0, 0x34);
            temp_v1 = (temp_v1 + 1) & 0xffff;
            temp_v0 = temp_v0 + 0x34;
        } while ((s32)temp_v1 < arg0[1]);
    }
}

void func_0031D3F0(s32 *arg0, u32 arg1) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *arg0;
    if (0 < arg0[1]) {
        do {
            temp_v1 = temp_v1 + 1;
            *(u32 *)(temp_v0 + 0x20) = (*(u32 *)(temp_v0 + 0x20) & 0xfffff807) | ((arg1 & 0xff) << 3);
            temp_v0 = temp_v0 + 0x34;
        } while (temp_v1 < arg0[1]);
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031D440);

s32 func_0031D4A8(s32 *arg0) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *arg0;
    if (0 < arg0[1]) {
        do {
            if ((*(u32 *)(temp_v0 + 0x20) & 1) == 0) {
                *(u16 *)(temp_v0 + 0x2a) = 10;
                *(u32 *)(temp_v0 + 0x20) = *(u32 *)(temp_v0 + 0x20) | 1;
                *(u16 *)(temp_v0 + 0x28) = 0;
                *(u16 *)(temp_v0 + 0x24) = 0;
                *(u16 *)(temp_v0 + 0x26) = 0;
                return temp_v0;
            }
            temp_v1 = temp_v1 + 1;
            temp_v0 = temp_v0 + 0x34;
        } while (temp_v1 < arg0[1]);
    }
    return 0;
}

void func_0031D508(s32 arg0) {
    *(u32 *)(arg0 + 0x20) = *(u32 *)(arg0 + 0x20) & 0xfffffffe;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031D520);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031D530);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031D540);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031D558);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031D680);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031D890);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031D928);

u32 * func_0031D948(s32 arg0) {
    u32 *puVar1;
    s32 temp_v0;

    temp_v0 = 0;
    puVar1 = *(u32 **)(arg0 + 4);
    if (0 < *(s32 *)(arg0 + 8)) {
        do {
            if ((*puVar1 & 1) == 0) {
                *puVar1 = *puVar1 | 1;
                return puVar1;
            }
            temp_v0 = temp_v0 + 1;
            puVar1 = puVar1 + 5;
        } while (temp_v0 < *(s32 *)(arg0 + 8));
    }
    return (u32 *)0x0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031D998);

void func_0031DA20(u32 *arg0) {
    *arg0 = *arg0 & 0xfffffffe;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031DA38);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031DEB8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031DF48);

u32 * func_0031DF68(s32 arg0) {
    u32 *puVar1;
    s32 temp_v0;

    temp_v0 = 0;
    puVar1 = *(u32 **)(arg0 + 4);
    if (0 < *(s32 *)(arg0 + 8)) {
        do {
            if ((*puVar1 & 1) == 0) {
                *puVar1 = *puVar1 | 1;
                return puVar1;
            }
            temp_v0 = temp_v0 + 1;
            puVar1 = puVar1 + 4;
        } while (temp_v0 < *(s32 *)(arg0 + 8));
    }
    return (u32 *)0x0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031DFB8);

void func_0031E008(u32 *arg0) {
    *arg0 = *arg0 & 0xfffffffe;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031E020);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031E198);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031E240);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031E2E8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031E410);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031E430);

void func_0031E4D0(u32 arg0) {
    D_0043895C = arg0;
}

void func_0031E4D8(void) {
    D_0043895C = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031E4E0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031E530);

u32 func_0031E548(s32 arg0) {
    return *(u32 *)(arg0 + 0x14);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031E550);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031E578);

void func_0031E640(u32 arg0) {
    s64 temp_v0;
    s32 temp_v1;

    temp_v0 = func_0031E550();
    if (temp_v0 != 0) {
        temp_v1 = (s32)arg0;
        func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 7, 0x54);
        func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 8, 0x54);
        func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 9, 0x54);
        func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 10, 0x54);
        func_0031E578(arg0);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031E6E0);

void func_0031E7C8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

void func_0031E7D0(u32 arg0) {
    s64 temp_v0;
    s32 temp_v1;

    temp_v0 = func_0031E550();
    if (temp_v0 != 0) {
        temp_v1 = (s32)arg0;
        func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 3, 0x54);
        func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 2, 0x54);
        func_0031E6E0(0xe0, 0x2a8, *(u32 *)(temp_v1 + 0x14), *(u32 *)(temp_v1 + 0x18));
        func_0031E578(arg0);
        return;
    }
}

void func_0031E850(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

void func_0031E858(u32 arg0) {
    s64 temp_v0;
    s32 temp_v1;

    temp_v0 = func_0031E550();
    if (temp_v0 != 0) {
        temp_v1 = (s32)arg0;
        func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 1, 0x54);
        func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 0, 0x54);
        func_0031E6E0(0xe0, 0x118, *(u32 *)(temp_v1 + 0x14), *(u32 *)(temp_v1 + 0x18));
        func_0031E578(arg0);
        return;
    }
}

void func_0031E8D8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x1c) = arg1;
}

void func_0031E8E0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x20) = arg1;
}

void func_0031E8E8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031E8F0);

void func_0031ED68(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031ED70);

void func_0031EE28(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031EE30);

void func_0031EEE8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031EEF0);

void func_0031EFB8(u32 arg0) {
    s64 temp_v0;
    s32 temp_v1;

    temp_v0 = func_0031E550();
    if (temp_v0 != 0) {
        temp_v1 = (s32)arg0;
        func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 0x1a, 0x54);
        func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 0x23, 0x54);
        func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 0x24, 0x54);
        func_0031E578(arg0);
        return;
    }
}

void func_0031F040(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

void func_0031F048(u32 arg0) {
    s64 temp_v0;
    s32 temp_v1;

    temp_v0 = func_0031E550();
    if (temp_v0 == 0) {
        return;
    }
    temp_v1 = (s32)arg0;
    func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 6, 0x54);
    if (*(s32 *)(temp_v1 + 0x18) != 1) {
        if (*(s32 *)(temp_v1 + 0x18) != 2) goto LAB_0031f0c4;
        func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 5, 0x54);
    }
    func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 4, 0x54);
LAB_0031f0c4:
    func_0031E578(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031F0E8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031F138);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031F168);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031F1B8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031F1E8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031F208);

u64 func_0031F228(s32 arg0) {
    u64 temp_v0;

    temp_v0 = func_0031F0E8();
    func_00320CE0(*(u32 *)(arg0 + 4), 0, temp_v0);
    return temp_v0;
}

u32 func_0031F270(s32 arg0) {
    return *(u32 *)(*(s32 *)(*(s32 *)(arg0 + 4) + 8) + 0x10);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031F280);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031F300);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031F340);

void func_0031F410(u32 arg0, u32 arg1) {
    u32 temp_v0 [4];

    temp_v0[0] = arg1;
    func_0031F340(arg0, temp_v0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031F430);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031F4D8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031F550);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031F5C0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031F618);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031F6A0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031F708);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031F778);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031F840);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031F878);

INCLUDE_ASM(const s32, "game/code_002DC138", func_0031FA60);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00320020);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003201A0);

u32 func_00320380(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00320388);

u64 func_00320510(u64 arg0, u64 arg1) {
    u64 temp_v0;
    u64 temp_v1;
    u32 temp_v2 [4];

    temp_v0 = func_00320AE8(arg0, arg1, temp_v2);
    temp_v1 = func_00325790(temp_v0, temp_v2[0]);
    func_0035A880(temp_v0);
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00320560);

u64 func_003206E8(u64 arg0, u64 arg1) {
    u64 temp_v0;
    u64 temp_v1;
    u32 temp_v2 [4];

    temp_v0 = func_00320AE8(arg0, arg1, temp_v2);
    temp_v1 = func_00325790(temp_v0, temp_v2[0]);
    func_0035A880(temp_v0);
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00320738);

u64 func_003208C0(u64 arg0, u64 arg1) {
    u64 temp_v0;
    u64 temp_v1;
    u32 temp_v2 [4];

    temp_v0 = func_00320AE8(arg0, arg1, temp_v2);
    temp_v1 = func_00325AB8(temp_v0, temp_v2[0]);
    func_0035A880(temp_v0);
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00320910);

u64 func_00320A98(u64 arg0, u64 arg1) {
    u64 temp_v0;
    u64 temp_v1;
    u32 temp_v2 [4];

    temp_v0 = func_00320AE8(arg0, arg1, temp_v2);
    temp_v1 = func_00325BB0(temp_v0, temp_v2[0]);
    func_0035A880(temp_v0);
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00320AE8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00320C28);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00320C88);

void func_00320CD0(s32 arg0, s32 arg1) {
    if (arg1 != 0) {
        *(s32 *)(arg0 + 0x14) = (s32)arg1;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00320CE0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00320D80);

void func_00320EA8(s32 arg0, s32 arg1) {
    if (arg1 != 0) {
        *(s32 *)(arg0 + 0x10) = (s32)arg1;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00320EB8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00320F68);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00320FD0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00321018);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00321090);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00321130);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00321170);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003211B0);

void func_003211F0(void) {
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_003211F8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00321208);

u8 * func_00321238(void) {
    return &D_004390B8;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00321248);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00321258);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003212A8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00321308);

void func_00321318(u32 arg0, u32 arg1) {
    D_004390C0 = arg0;
    D_004390C4 = arg1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00321328);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00321340);

void func_003214C0(void) {
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_003214C8);

void func_003214D0(u32 arg0, s32 arg1) {
    if (arg1 != 0) {
        func_00321908(arg1);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00321500);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00321528);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00321688);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003216A8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00321798);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003218A0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00321908);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00321928);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003219F0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00321A30);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00321C60);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00321E18);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00321E70);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00321EC8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00321ED8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00321EE8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00321F18);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00321F78);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00321F98);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003223F8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00322418);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00322438);

s32 func_00322480(s32 *arg0, s32 arg1) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = arg1 << 3;
    if (0 < arg1) {
        do {
            temp_v0 = *arg0;
            arg0 = arg0 + 2;
            arg1 = arg1 - 1;
            temp_v1 = temp_v1 + temp_v0 * 8;
        } while (arg1 != 0);
    }
    return temp_v1;
}

void func_003224B0(void) {
}

void func_003224B8(void) {
}

void func_003224C0(void) {
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_003224C8);

void func_003224E0(u32 arg0, u32 arg1) {
    D_004390D0 = arg0;
    D_004390D4 = arg1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_003224F0);

void func_00322510(u32 arg0, u32 arg1) {
    D_004390DC = arg0;
    D_004390E0 = arg1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00322520);

void func_00322540(u32 arg0, u32 arg1) {
    D_004390E4 = arg0;
    D_004390E8 = arg1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00322550);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00322570);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003225C0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00322610);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00322670);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003226D8);

void func_00322D08(u32 arg0, u32 arg1) {
    D_004390C8 = arg0;
    D_004390CC = arg1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00322D18);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00322D50);

u32 func_00322D98(void) {
    return D_004390C8;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00322DA0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00322E18);

void func_00322F00(s32 arg0) {
    *(u32 *)(arg0 + 0x40) = *(u32 *)(arg0 + 0x40) & 0xfffffffe;
    if (*(s32 *)(arg0 + 0x3c) != 0) {
        func_00320C88(*(s32 *)(arg0 + 0x3c));
        *(u32 *)(arg0 + 0x3c) = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00322F48);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003230A0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003232A0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003233E8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003236B0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00323748);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003238A0);

void func_00323918(u8 *arg0) {
    D_004389A0 = arg0;
}

void func_00323920(u8 *arg0) {
    D_004389A4 = arg0;
}

void func_00323928(u8 *arg0) {
    D_004389A8 = arg0;
}

void func_00323930(u8 *arg0) {
    D_004389AC = arg0;
}

void func_00323938(u8 *arg0) {
    D_004389B0 = arg0;
}

u32 func_00323940(s32 arg0, s32 arg1) {
    if (*(s16 *)(arg0 + 0x36) - arg1 < 1) {
        *(u16 *)(arg0 + 0x36) = 0;
        *(u32 *)(arg0 + 0x40) = *(u32 *)(arg0 + 0x40) | 4;
        return 1;
    }
    *(s16 *)(arg0 + 0x36) = *(s16 *)(arg0 + 0x36) - (s16)arg1;
    *(u32 *)(arg0 + 0x40) = *(u32 *)(arg0 + 0x40) | 2;
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00323988);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00323BB8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00323DF0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00324070);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00324238);

u32 func_00324268(void) {
    return D_004390D8;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00324270);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003242D0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00324840);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00324AC0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00324B28);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00324C98);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00324D28);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00324D50);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00324DB8);

void func_00324DF8(u32 *arg0, u32 arg1) {
    func_00320CE0(*arg0, 0, arg1);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00324E18);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00324E80);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00324EF0);

void func_00324F20(u32 *arg0) {
    func_003211B0(*arg0);
}

void func_00324F38(u32 *arg0) {
    func_00321170(*arg0);
}

u64 func_00324F50(s32 arg0, u64 arg1) {
    u64 temp_v0;

    temp_v0 = func_0035A828(arg1);
    func_00320CE0(*(u32 *)(arg0 + 4), 0, temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00324F98);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00324FD0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003251C0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00325398);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003255A0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00325688);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00325790);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00325AB8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00325BB0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00325CC8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00325EC8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00326018);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00326158);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003262A8);

void func_003268A8(float *arg0, float *arg1) {
    *arg0 = *arg0 + *arg1;
    arg0[1] = arg0[1] + arg1[1];
    arg0[2] = arg0[2] + arg1[2];
}

void func_003268E0(float *arg0, float *arg1) {
    *arg0 = *arg0 - *arg1;
    arg0[1] = arg0[1] - arg1[1];
    arg0[2] = arg0[2] - arg1[2];
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00326918);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00326940);

void func_00326950(float arg0, float *arg1) {
    *arg1 = *arg1 * arg0;
    arg1[1] = arg1[1] * arg0;
    arg1[2] = arg1[2] * arg0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00326978);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003269F0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00326A40);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00326AE0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00326B00);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00326BC8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003270C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003275C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00327AC8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00327BD8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00327C80);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00328018);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00328160);

void func_003282E8(void) {
}

s32 func_003282F0(u32 initial, u32 option, u32 maximum) {
    struct {
        u32 attr;
        u32 option;
        u32 initial;
        u32 reserved[2];
        u32 maximum;
    } sema;

    sema.initial = initial;
    sema.option = option;
    sema.maximum = maximum;
    return CreateSema(&sema);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00328318);

void func_00328390(u64 arg0, u64 arg1, u64 arg2) {
    u64 temp_v0;

    temp_v0 = func_003283E0(arg1);
    func_00328318(arg0, temp_v0, arg1, arg2);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_003283E0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00328420);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00328470);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003284C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00328520);

INCLUDE_ASM(const s32, "game/code_002DC138", sdfAddHandler);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00328668);

void func_00328748(void) {
    u32 current;
    while ((current = D_004389BC) != 0) {
        func_00328668(current);
    }
}

void func_00328778(u32 arg0, u32 arg1, u32 arg2) {
    iWakeupThread(arg2);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00328790);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003287E0);

u32 func_003287F0(u32 base) {
    u32 now;

    now = *(volatile u32 *)0x10000000;
    return (now - base) & 0xFFFF;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00328808);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00328858);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00328918);

void func_00328988(s32 arg0) {
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

INCLUDE_ASM(const s32, "game/code_002DC138", func_003289C8);
