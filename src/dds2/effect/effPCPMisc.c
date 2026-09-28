#include "common.h"

extern s32 D_00438F04;

extern s32 func_00185400(void);

typedef struct {
    u8 pad00[0x10];  /* 0x00 task header */
    u32 unk10;       /* 0x10 resource handle */
    u32 unk14;       /* 0x14 resource handle */
    f32 unk18;       /* 0x18 spawn parameter */
    u32 unk1C;       /* 0x1C resource released on destroy */
    u32 unk20;       /* 0x20 spawn parameter */
    u32 unk24;       /* 0x24 spawn parameter */
    u8 pad28[0x4];   /* 0x28 */
    f32 unk2C;       /* 0x2C spawn parameter */
    u32 unk30;       /* 0x30 resource released on destroy */
    u32 unk34;       /* 0x34 resource released on destroy */
    u32 unk38;       /* 0x38 resource released on destroy */
    u8 pad3C[0x18];  /* 0x3C */
    u32 unk54;       /* 0x54 spawn parameter */
    f32 unk58;       /* 0x58 spawn parameter */
    u32 unk5C;       /* 0x5C nested work handle */
    u32 unk60;       /* 0x60 resource handle */
    u32 unk64;       /* 0x64 resource handle */
    u8 pad68[0xC];   /* 0x68 */
    u32 unk74;       /* 0x74 optional handle (freed if != 0) */
    u8 pad78[0x4];   /* 0x78 */
    u32 unk7C;       /* 0x7C nested work handle */
    u8 pad80[0x1C];  /* 0x80 */
    u32 unk9C;       /* 0x9C spawn parameter */
    f32 unkA0;       /* 0xA0 spawn parameter */
    u32 unkA4;       /* 0xA4 spawn parameter */
    u32 unkA8;       /* 0xA8 resource released on destroy */
    u32 unkAC;       /* 0xAC resource released on destroy */
    u8 padB0[0x8];   /* 0xB0 */
    u32 unkB8;       /* 0xB8 spawn parameter */
    u32 unkBC;       /* 0xBC mode set through the singleton accessor */
    u8 padC0[0x50];  /* 0xC0 */
    f32 unk110;      /* 0x110 spawn parameter */
    u32 unk114;      /* 0x114 spawn parameter */
    u8 pad118[0x50]; /* 0x118 */
    f32 unk168;      /* 0x168 spawn parameter */
    u8 pad16C[0x4];  /* 0x16C */
    u32 unk170;      /* 0x170 spawn parameter */
} EffPCPWorkF18;

typedef struct {
    u8 pad00[0x10];  /* 0x00 task header */
    u32 unk10;       /* 0x10 resource handle */
    f32 unk14;       /* 0x14 spawn parameter */
    u32 unk18;       /* 0x18 resource handle */
    u32 unk1C;       /* 0x1C resource released on destroy */
    u32 unk20;       /* 0x20 spawn parameter */
    u32 unk24;       /* 0x24 spawn parameter */
    u8 pad28[0x4];   /* 0x28 */
    f32 unk2C;       /* 0x2C spawn parameter */
    u32 unk30;       /* 0x30 resource released on destroy */
    u32 unk34;       /* 0x34 resource released on destroy */
    u32 unk38;       /* 0x38 resource released on destroy */
    u8 pad3C[0x18];  /* 0x3C */
    u32 unk54;       /* 0x54 spawn parameter */
    f32 unk58;       /* 0x58 spawn parameter */
    u32 unk5C;       /* 0x5C nested work handle */
    u32 unk60;       /* 0x60 resource handle */
    u32 unk64;       /* 0x64 resource handle */
    u8 pad68[0xC];   /* 0x68 */
    u32 unk74;       /* 0x74 optional handle (freed if != 0) */
    u8 pad78[0x4];   /* 0x78 */
    u32 unk7C;       /* 0x7C nested work handle */
    u8 pad80[0x1C];  /* 0x80 */
    u32 unk9C;       /* 0x9C spawn parameter */
    f32 unkA0;       /* 0xA0 spawn parameter */
    u32 unkA4;       /* 0xA4 spawn parameter */
    u32 unkA8;       /* 0xA8 resource released on destroy */
    u32 unkAC;       /* 0xAC resource released on destroy */
    u8 padB0[0x8];   /* 0xB0 */
    u32 unkB8;       /* 0xB8 spawn parameter */
    u32 unkBC;       /* 0xBC mode set through the singleton accessor */
    u8 padC0[0x50];  /* 0xC0 */
    f32 unk110;      /* 0x110 spawn parameter */
    u32 unk114;      /* 0x114 spawn parameter */
    u8 pad118[0x50]; /* 0x118 */
    f32 unk168;      /* 0x168 spawn parameter */
    u8 pad16C[0x4];  /* 0x16C */
    u32 unk170;      /* 0x170 spawn parameter */
} EffPCPWorkF14;

/* Large charge-style effect work (allocation 0x1354). Only the tail is
 * touched by the matched C functions: a colour initialised to grey plus
 * handles released on destroy.
 */
typedef struct {
    u8 pad000[0x1334]; /* 0x0000 */
    u32 unk1334;       /* 0x1334 spawn parameter */
    f32 unk1338;       /* 0x1338 spawn parameter */
    u32 unk133C;       /* 0x133C cleared on init */
    u32 unk1340;       /* 0x1340 cleared on init */
    u32 color1344;     /* 0x1344 initialised to grey 0x80808080 */
    u32 unk1348;       /* 0x1348 resource released on destroy */
    u32 unk134C;       /* 0x134C resource released on destroy */
    u32 unk1350;       /* 0x1350 resource released on destroy */
} EffPCPChargeWork;

/* Particle/effect work layouts shared by the matched functions of this TU.
 * Every effect in ../effect/src/effPCPMisc.c keeps its own allocation size,
 * but the small PCP effects share a common header: a task link area followed
 * by resource handles and spawn parameters. Offsets below were recovered
 * from the matched C functions; fields the C code never touches are padding.
 */
typedef struct {
    u8 pad00[0x10];  /* 0x00 task header */
    u32 unk10;       /* 0x10 resource handle */
    u32 unk14;       /* 0x14 resource handle */
    u32 unk18;       /* 0x18 resource handle */
    u32 unk1C;       /* 0x1C resource released on destroy */
    u32 unk20;       /* 0x20 spawn parameter */
    u32 unk24;       /* 0x24 spawn parameter */
    u8 pad28[0x4];   /* 0x28 */
    f32 unk2C;       /* 0x2C spawn parameter */
    u32 unk30;       /* 0x30 resource released on destroy */
    u32 unk34;       /* 0x34 resource released on destroy */
    u32 unk38;       /* 0x38 resource released on destroy */
    u8 pad3C[0x18];  /* 0x3C */
    u32 unk54;       /* 0x54 spawn parameter */
    f32 unk58;       /* 0x58 spawn parameter */
    u32 unk5C;       /* 0x5C nested work handle */
    u32 unk60;       /* 0x60 resource handle */
    u32 unk64;       /* 0x64 resource handle */
    u8 pad68[0xC];   /* 0x68 */
    u32 unk74;       /* 0x74 optional handle (freed if != 0) */
    u8 pad78[0x4];   /* 0x78 */
    u32 unk7C;       /* 0x7C nested work handle */
    u8 pad80[0x1C];  /* 0x80 */
    u32 unk9C;       /* 0x9C spawn parameter */
    f32 unkA0;       /* 0xA0 spawn parameter */
    u32 unkA4;       /* 0xA4 spawn parameter */
    u32 unkA8;       /* 0xA8 resource released on destroy */
    u32 unkAC;       /* 0xAC resource released on destroy */
    u8 padB0[0x8];   /* 0xB0 */
    u32 unkB8;       /* 0xB8 spawn parameter */
    u32 unkBC;       /* 0xBC mode set through the singleton accessor */
    u8 padC0[0x50];  /* 0xC0 */
    f32 unk110;      /* 0x110 spawn parameter */
    u32 unk114;      /* 0x114 spawn parameter */
    u8 pad118[0x50]; /* 0x118 */
    f32 unk168;      /* 0x168 spawn parameter */
    u8 pad16C[0x4];  /* 0x16C */
    u32 unk170;      /* 0x170 spawn parameter */
} EffPCPWork;

/* Compact 0x1C effect work built by func_00178B18/func_00178BC0: cleared
 * header words, a grey colour and two resource handles released on destroy.
 */
typedef struct {
    u32 unk00;    /* 0x00 cleared on init */
    u32 unk04;    /* 0x04 cleared on init */
    u32 unk08;    /* 0x08 cleared on init */
    u8 pad0C[0x4]; /* 0x0C */
    u32 color10;  /* 0x10 initialised to grey 0x80808080 */
    u32 unk14;    /* 0x14 resource released on destroy */
    u32 unk18;    /* 0x18 resource released on destroy */
} EffPCPWork1C;

extern void *func_00328D68(s32 size);

extern u32 func_0016AEE8(void *data, s32 index);

extern u32 func_0016A6A0(u32 param);

extern u32 func_0016AF38(void *params);

extern u8 D_003B1938[];

/* Per-effect views of the same work area for effects whose spawn parameters
 * at 0x10/0x14/0x18/0x1C/0x34 are floats. Each variant below matches
 * EffPCPWork field for field except for the one float noted; a variant is
 * only used by the effect groups whose C code touches that word as a float.
 */
typedef struct {
    u8 pad00[0x10];  /* 0x00 task header */
    f32 unk10;       /* 0x10 spawn parameter */
    u32 unk14;       /* 0x14 resource handle */
    u32 unk18;       /* 0x18 resource handle */
    u32 unk1C;       /* 0x1C resource released on destroy */
    u32 unk20;       /* 0x20 spawn parameter */
    u32 unk24;       /* 0x24 spawn parameter */
    u8 pad28[0x4];   /* 0x28 */
    f32 unk2C;       /* 0x2C spawn parameter */
    u32 unk30;       /* 0x30 resource released on destroy */
    u32 unk34;       /* 0x34 resource released on destroy */
    u32 unk38;       /* 0x38 resource released on destroy */
    u8 pad3C[0x18];  /* 0x3C */
    u32 unk54;       /* 0x54 spawn parameter */
    f32 unk58;       /* 0x58 spawn parameter */
    u32 unk5C;       /* 0x5C nested work handle */
    u32 unk60;       /* 0x60 resource handle */
    u32 unk64;       /* 0x64 resource handle */
    u8 pad68[0xC];   /* 0x68 */
    u32 unk74;       /* 0x74 optional handle (freed if != 0) */
    u8 pad78[0x4];   /* 0x78 */
    u32 unk7C;       /* 0x7C nested work handle */
    u8 pad80[0x1C];  /* 0x80 */
    u32 unk9C;       /* 0x9C spawn parameter */
    f32 unkA0;       /* 0xA0 spawn parameter */
    u32 unkA4;       /* 0xA4 spawn parameter */
    u32 unkA8;       /* 0xA8 resource released on destroy */
    u32 unkAC;       /* 0xAC resource released on destroy */
    u8 padB0[0x8];   /* 0xB0 */
    u32 unkB8;       /* 0xB8 spawn parameter */
    u32 unkBC;       /* 0xBC mode set through the singleton accessor */
    u8 padC0[0x50];  /* 0xC0 */
    f32 unk110;      /* 0x110 spawn parameter */
    u32 unk114;      /* 0x114 spawn parameter */
    u8 pad118[0x50]; /* 0x118 */
    f32 unk168;      /* 0x168 spawn parameter */
    u8 pad16C[0x4];  /* 0x16C */
    u32 unk170;      /* 0x170 spawn parameter */
} EffPCPWorkF10;

extern u8 D_003B19E8[];

typedef struct {
    u8 pad00[0x10];  /* 0x00 task header */
    u32 unk10;       /* 0x10 resource handle */
    u32 unk14;       /* 0x14 resource handle */
    u32 unk18;       /* 0x18 resource handle */
    f32 unk1C;       /* 0x1C spawn parameter */
    u32 unk20;       /* 0x20 spawn parameter */
    u32 unk24;       /* 0x24 spawn parameter */
    u8 pad28[0x4];   /* 0x28 */
    f32 unk2C;       /* 0x2C spawn parameter */
    u32 unk30;       /* 0x30 resource released on destroy */
    u32 unk34;       /* 0x34 resource released on destroy */
    u32 unk38;       /* 0x38 resource released on destroy */
    u8 pad3C[0x18];  /* 0x3C */
    u32 unk54;       /* 0x54 spawn parameter */
    f32 unk58;       /* 0x58 spawn parameter */
    u32 unk5C;       /* 0x5C nested work handle */
    u32 unk60;       /* 0x60 resource handle */
    u32 unk64;       /* 0x64 resource handle */
    u8 pad68[0xC];   /* 0x68 */
    u32 unk74;       /* 0x74 optional handle (freed if != 0) */
    u8 pad78[0x4];   /* 0x78 */
    u32 unk7C;       /* 0x7C nested work handle */
    u8 pad80[0x1C];  /* 0x80 */
    u32 unk9C;       /* 0x9C spawn parameter */
    f32 unkA0;       /* 0xA0 spawn parameter */
    u32 unkA4;       /* 0xA4 spawn parameter */
    u32 unkA8;       /* 0xA8 resource released on destroy */
    u32 unkAC;       /* 0xAC resource released on destroy */
    u8 padB0[0x8];   /* 0xB0 */
    u32 unkB8;       /* 0xB8 spawn parameter */
    u32 unkBC;       /* 0xBC mode set through the singleton accessor */
    u8 padC0[0x50];  /* 0xC0 */
    f32 unk110;      /* 0x110 spawn parameter */
    u32 unk114;      /* 0x114 spawn parameter */
    u8 pad118[0x50]; /* 0x118 */
    f32 unk168;      /* 0x168 spawn parameter */
    u8 pad16C[0x4];  /* 0x16C */
    u32 unk170;      /* 0x170 spawn parameter */
} EffPCPWorkF1C;

typedef struct {
    u8 pad00[0x10];  /* 0x00 task header */
    u32 unk10;       /* 0x10 resource handle */
    u32 unk14;       /* 0x14 resource handle */
    u32 unk18;       /* 0x18 resource handle */
    u32 unk1C;       /* 0x1C resource released on destroy */
    u32 unk20;       /* 0x20 spawn parameter */
    u32 unk24;       /* 0x24 spawn parameter */
    u8 pad28[0x4];   /* 0x28 */
    f32 unk2C;       /* 0x2C spawn parameter */
    u32 unk30;       /* 0x30 resource released on destroy */
    f32 unk34;       /* 0x34 spawn parameter */
    u32 unk38;       /* 0x38 resource released on destroy */
    u8 pad3C[0x18];  /* 0x3C */
    u32 unk54;       /* 0x54 spawn parameter */
    f32 unk58;       /* 0x58 spawn parameter */
    u32 unk5C;       /* 0x5C nested work handle */
    u32 unk60;       /* 0x60 resource handle */
    u32 unk64;       /* 0x64 resource handle */
    u8 pad68[0xC];   /* 0x68 */
    u32 unk74;       /* 0x74 optional handle (freed if != 0) */
    u8 pad78[0x4];   /* 0x78 */
    u32 unk7C;       /* 0x7C nested work handle */
    u8 pad80[0x1C];  /* 0x80 */
    u32 unk9C;       /* 0x9C spawn parameter */
    f32 unkA0;       /* 0xA0 spawn parameter */
    u32 unkA4;       /* 0xA4 spawn parameter */
    u32 unkA8;       /* 0xA8 resource released on destroy */
    u32 unkAC;       /* 0xAC resource released on destroy */
    u8 padB0[0x8];   /* 0xB0 */
    u32 unkB8;       /* 0xB8 spawn parameter */
    u32 unkBC;       /* 0xBC mode set through the singleton accessor */
    u8 padC0[0x50];  /* 0xC0 */
    f32 unk110;      /* 0x110 spawn parameter */
    u32 unk114;      /* 0x114 spawn parameter */
    u8 pad118[0x50]; /* 0x118 */
    f32 unk168;      /* 0x168 spawn parameter */
    u8 pad16C[0x4];  /* 0x16C */
    u32 unk170;      /* 0x170 spawn parameter */
} EffPCPWorkF34;

extern u8 D_003B1A38[];

extern u8 D_003B1A88[];

extern EffPCPWork *func_001846B8(void *param0, void *param1);

extern u32 func_00157A50(u32 handle);

extern void func_00336AA8(void);

extern void func_00336538(f32 scale);

/* Block `index` of a packed effect parameter set: data + offset table entry. */
extern void *func_0016AEB0(void *data, s32 index);

extern void func_00184B90(void *block0, void *blocks);

extern void func_00186120(void *dst, void *src);

/* Round-robin selector work behind func_0017ED98: three key/ID pairs plus a
 * counter at 0x108. Each call fires the IDs whose key has caught up.
 */
typedef struct {
    s32 keys[3];    /* 0x00 compared against count */
    u8 pad0C[0xF0]; /* 0x0C */
    s32 ids[3];     /* 0xFC fired through func_0017DCF8 */
    s32 count;      /* 0x108 round-robin counter */
} EffPCPRotateWork;

extern void func_00186100(void *dst, void *src);

extern s8 D_0043643C;

extern u32 func_0016D070(void *params);

extern u8 D_003B1AF0[];

extern u8 D_003B1BB0[];

extern u8 D_003B1C70[];

extern u8 D_003B1D30[];

extern u8 D_003B1DF0[];

extern u8 D_003B1EB0[];

typedef struct EffPCPNode {
    u8 pad0[4];
    struct EffPCPNode *next;
    u8 pad8[4];
    struct EffPCPNode *child;
    u8 pad10[0x60];
    u8 vector70[0x10];
} EffPCPNode;

void func_0017EDE8(u32 arg0) {
    func_00159AF0(*(u32 *)((s32)arg0 + 0x1c));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017EE18);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017EF50);

void func_0017EF60(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

void func_0017EF68(EffPCPWorkF18 *work, f32 val) {
    work->unk18 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017EF70);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F070);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F1E8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F260);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F358);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F4A8);

void func_0017F4B8(EffPCPWorkF18 *work, f32 val) {
    work->unk18 = val;
}

void func_0017F4C0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x10) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F4C8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F5E8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F6C0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F720);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F7C8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F928);

void func_0017F938(EffPCPWorkF14 *work, f32 val) {
    work->unk14 = val;
}

void func_0017F940(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x10) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F948);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FA90);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FBE8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FC80);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FD88);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FEB8);

void func_0017FEC8(EffPCPWorkF14 *work, f32 val) {
    work->unk14 = val;
}

void func_0017FED0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x10) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FED8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FF78);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180040);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001800A0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180158);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180240);

void func_00180250(EffPCPWorkF14 *work, f32 val) {
    work->unk14 = val;
}

void func_00180258(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x10) = arg1;
}

void func_00180260(s32 arg0) {
    *(u32 *)(arg0 + 0x133c) = 0;
    *(u32 *)(arg0 + 0x1340) = 0;
    *(u32 *)(arg0 + 0x1344) = 0x80808080;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180278);

void func_00180318(s32 arg0) {
    func_0016A620(*(u32 *)(arg0 + 0x134c));
    func_0016A620(*(u32 *)(arg0 + 0x1348));
    func_003297C8(*(u32 *)(arg0 + 0x1350));
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180350);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001803E8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180748);

void func_00180760(EffPCPChargeWork *work, f32 val) {
    work->unk1338 = val;
}

void func_00180768(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x1334) = arg1;
}

EffPCPWork1C *func_00180770(EffPCPWork *src) {
    EffPCPWork1C *dst;
    u32 handle;

    dst = func_00328D68(0x1C);
    dst->unk14 = func_0016AEE8(src, 0);
    handle = func_0016AEE8(src, 1);
    dst->unk00 = 0;
    dst->unk18 = handle;
    dst->unk04 = 0;
    dst->color10 = 0x80808080;
    dst->unk08 = 0;
    return dst;
}

void func_001807E0(u32 arg0) {
    func_0016A620(*(u32 *)((s32)arg0 + 0x14));
    func_0016A620(*(u32 *)((s32)arg0 + 0x18));
    func_00328E48(arg0);
}

EffPCPWork1C *func_00180818(EffPCPWork *src) {
    EffPCPWork1C *dst;
    u32 handle;

    dst = func_00328D68(0x1C);
    dst->unk14 = func_0016A6A0(src->unk14);
    handle = func_0016A6A0(src->unk18);
    dst->unk00 = 0;
    dst->unk18 = handle;
    dst->unk04 = 0;
    dst->color10 = 0x80808080;
    dst->unk08 = 0;
    return dst;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180880);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180900);

void func_00180910(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x10) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180918);

u64 func_00180BD8(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x4c);
    func_00180918(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180C10);

u64 func_00180C68(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x4c);
    func_00180918(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180CA0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180D90);

void func_00180DA0(EffPCPWorkF14 *work, f32 val) {
    work->unk14 = val;
}

void func_00180DA8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180DB0);

u64 func_00181050(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x4c);
    func_00180DB0(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181088);

u64 func_001810E0(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x4c);
    func_00180DB0(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181118);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181208);

void func_00181218(EffPCPWorkF14 *work, f32 val) {
    work->unk14 = val;
}

void func_00181220(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181228);

u64 func_001814E8(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x94);
    func_00181228(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181520);

u64 func_00181578(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x94);
    func_00181228(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001815B0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001816A0);

void func_001816B0(EffPCPWorkF14 *work, f32 val) {
    work->unk14 = val;
}

void func_001816B8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001816C0);

u64 func_00181960(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x4c);
    func_001816C0(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181998);

u64 func_001819F0(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x4c);
    func_001816C0(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181A28);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181B18);

void func_00181B28(EffPCPWorkF14 *work, f32 val) {
    work->unk14 = val;
}

void func_00181B30(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181B38);

u64 func_00181CD0(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x4c);
    func_00181B38(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181D08);

u64 func_00181D60(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x4c);
    func_00181B38(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181D98);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181E88);

void func_00181E98(EffPCPWorkF14 *work, f32 val) {
    work->unk14 = val;
}

void func_00181EA0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181EA8);

u64 func_00182040(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x3c);
    func_00181EA8(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182078);

u64 func_001820D0(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x3c);
    func_00181EA8(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182108);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001821F8);

void func_00182208(EffPCPWorkF14 *work, f32 val) {
    work->unk14 = val;
}

void func_00182210(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

void func_00182218(EffPCPWork *work) {
    u32 handle;

    handle = func_0016AF38(D_003B1938);
    work->unk18 = 0;
    work->unk1C = handle;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182250);

void func_00182288(u32 arg0) {
    func_0016B130(*(u32 *)((s32)arg0 + 0x1c));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001822B8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001822F0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182448);

void func_00182458(EffPCPWorkF10 *work, f32 val) {
    work->unk10 = val;
}

void func_00182460(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182468);

u64 func_00182728(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x94);
    func_00182468(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182760);

u64 func_001827B8(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x94);
    func_00182468(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001827F0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001828E0);

void func_001828F0(EffPCPWorkF14 *work, f32 val) {
    work->unk14 = val;
}

void func_001828F8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

void func_00182900(EffPCPWork *work) {
    u32 handle;

    handle = func_0016AF38(D_003B19E8);
    work->unk18 = 0;
    work->unk1C = handle;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182938);

void func_00182970(u32 arg0) {
    func_0016B130(*(u32 *)((s32)arg0 + 0x1c));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001829A0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001829D8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182B28);

void func_00182B38(EffPCPWorkF10 *work, f32 val) {
    work->unk10 = val;
}

void func_00182B40(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182B48);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182C10);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182C60);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182DA0);

void func_00182DB8(u32 arg0, u32 arg1) {
    *(u32 *)(D_00438F04 + 0x10) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182DC8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182DD8);

void func_00182E70(u32 arg0) {
    func_0018E8F0(*(u32 *)((s32)arg0 + 0x34));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182EA0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182F80);

void func_00182F90(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x10) = arg1;
}

void func_00182F98(EffPCPWorkF1C *work, f32 val) {
    work->unk1C = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182FA0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183030);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183050);

void func_001830F0(u32 arg0) {
    func_0018FC88(*(u32 *)((s32)arg0 + 0x38));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183120);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001832C8);

void func_001832D8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

void func_001832E0(EffPCPWorkF34 *work, f32 val) {
    work->unk34 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001832E8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183378);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183398);

void func_00183448(u32 arg0) {
    func_0018E8F0(*(u32 *)((s32)arg0 + 0x38));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183478);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183620);

void func_00183630(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

void func_00183638(EffPCPWorkF34 *work, f32 val) {
    work->unk34 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183640);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001836D0);

void func_001836F0(void) {
    func_00183640();
}

void func_00183708(void) {
    func_00328E48();
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183720);

void func_00183830(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183838);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001838E8);

void func_00183908(void) {
    func_00183838();
}

void func_00183920(void) {
    func_00328E48();
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183938);

void func_00183A50(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x34) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183A58);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183AE8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183B08);

void func_00183BB8(u32 arg0) {
    func_0018ECB8(*(u32 *)((s32)arg0 + 0x34));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183BE8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183DB0);

void func_00183DC0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x20) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183DC8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183E58);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183E78);

void func_00183F28(u32 arg0) {
    func_0018F1B8(*(u32 *)((s32)arg0 + 0x34));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183F58);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184148);

void func_00184158(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x20) = arg1;
}

void func_00184160(EffPCPWork *work) {
    u32 handle;

    handle = func_0016AF38(D_003B1A38);
    work->unk18 = 0;
    work->unk1C = handle;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184198);

void func_001841D0(u32 arg0) {
    func_0016B130(*(u32 *)((s32)arg0 + 0x1c));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184200);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184238);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001843F0);

void func_00184400(EffPCPWorkF10 *work, f32 val) {
    work->unk10 = val;
}

void func_00184408(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

void func_00184410(EffPCPWork *work) {
    u32 handle;

    handle = func_0016AF38(D_003B1A88);
    work->unk18 = 0;
    work->unk1C = handle;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184448);

void func_00184480(u32 arg0) {
    func_0016B130(*(u32 *)((s32)arg0 + 0x1c));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001844B0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001844E8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184698);

void func_001846A8(EffPCPWorkF10 *work, f32 val) {
    work->unk10 = val;
}

void func_001846B0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001846B8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001847E0);

EffPCPWork *func_00184828(EffPCPWork *work) {
    EffPCPWork *child;
    u32 handle;

    child = func_001846B8(&work->pad3C[4], NULL);
    handle = work->unk74;
    if (handle != 0) {
        child->unk74 = func_00157A50(handle);
    }
    return child;
}

void func_00184880(u32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)((s32)arg0 + 0x74);
    if (temp_v0 != 0) {
        func_00157658(temp_v0);
    }
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001848B8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184B10);

void func_00184B28(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 100) = arg1;
}

void func_00184B30(void *dst, void *src) {
    func_00336538(3.1415927f);
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf24, 0(%0)\n"
        "lqc2 vf25, 0x10(%0)\n"
        "lqc2 vf26, 0x20(%0)\n"
        "lqc2 vf27, 0x30(%0)\n"
        ".set reorder"
        : : "r"(src) : "memory");
    func_00336AA8();
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf28, 0(%0)\n"
        "sqc2 vf29, 0x10(%0)\n"
        "sqc2 vf30, 0x20(%0)\n"
        "sqc2 vf31, 0x30(%0)\n"
        ".set reorder"
        : : "r"(dst) : "memory");
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184B90);

void func_00184CF8(void *data) {
    void *block0;
    void *blocks[7];
    void **dst;
    u32 i;

    block0 = func_0016AEB0(data, 0);
    dst = blocks;
    i = 0;
    do {
        i++;
        *dst = func_0016AEB0(data, i);
        dst++;
    } while (i < 7);
    func_00184B90(block0, blocks);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184D70);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184EE0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184F50);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00185100);

void func_00185110(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x54) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00185118);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00185400);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00185500);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001856B0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00185808);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00185950);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00186100);

void func_00186118(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0xb8) = arg1;
}

void func_00186120(void *dst, void *src) {
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf28, 0(%1)\n"
        "lqc2 vf29, 0x10(%1)\n"
        "lqc2 vf30, 0x20(%1)\n"
        "lqc2 vf31, 0x30(%1)\n"
        "sqc2 vf28, 0(%0)\n"
        "sqc2 vf29, 0x10(%0)\n"
        "sqc2 vf30, 0x20(%0)\n"
        "sqc2 vf31, 0x30(%0)\n"
        ".set reorder"
        : : "r"(dst), "r"(src) : "memory");
}

void func_00186148(void) {
    s32 temp_v0;

    temp_v0 = func_00185400();
    *(u32 *)(temp_v0 + 0xbc) = 1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00186170);

void func_001862C0(void) {
    s32 temp_v0;

    temp_v0 = func_00185400();
    *(u32 *)(temp_v0 + 0xbc) = 2;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001862E8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00186438);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001865D0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00186708);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00186990);

void func_001869F0(s32 *arg0) {
    s32 temp_v0;
    s32 *piVar2;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = arg0[0x42];
    piVar2 = arg0;
    do {
        if (*piVar2 <= temp_v0) {
            func_00185950(piVar2[0x3f]);
            temp_v0 = arg0[0x42];
        }
        temp_v1 = temp_v1 + 1;
        piVar2 = piVar2 + 1;
    } while (temp_v1 < 3);
    arg0[0x42] = temp_v0 + 1;
}

void func_00186A68(EffPCPRotateWork *work, void *src) {
    u32 i;
    s32 *id;

    id = work->ids;
    for (i = 0; i < 3; i++) {
        func_00186100((void *)id[i], src);
    }
}

void func_00186AC8(EffPCPRotateWork *work, u32 val) {
    u32 i;
    s32 *id;

    id = work->ids;
    for (i = 0; i < 3; i++) {
        func_00186118((EffPCPWork *)id[i], val);
    }
}

void func_00186B28(EffPCPRotateWork *work, void *src) {
    u32 i;
    s32 *id;

    id = work->ids;
    for (i = 0; i < 3; i++) {
        func_00186120((void *)id[i], src);
    }
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00186B88);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00186C60);

void func_00186D40(u32 arg0) {
    func_0016A620(*(u32 *)((s32)arg0 + 0x60));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00186D70);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00186E18);

void func_00186E28(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x5c) = arg1;
}

void func_00186E30(EffPCPWork *work, f32 val) {
    work->unk58 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00186E38);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00186E68);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00186F28);

void func_00186FE0(u32 arg0) {
    func_0016A620(*(u32 *)((s32)arg0 + 100));
    func_0016A620(*(u32 *)((s32)arg0 + 0x60));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187018);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187118);

void func_00187128(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x5c) = arg1;
}

void func_00187130(EffPCPWork *work, f32 val) {
    work->unk58 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187138);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187168);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187228);

void func_001872E0(u32 arg0) {
    func_0016A620(*(u32 *)((s32)arg0 + 100));
    func_0016A620(*(u32 *)((s32)arg0 + 0x60));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187318);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187418);

void func_00187428(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x5c) = arg1;
}

void func_00187430(EffPCPWork *work, f32 val) {
    work->unk58 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187438);

u32 func_00187468(void) {
    D_0043643C = 1;
    return 0;
}

u32 func_00187478(void) {
    D_0043643C = 1;
    return 0;
}

void func_00187488(void) {
}

void func_00187490(void) {
    if (D_0043643C != 0) {
        D_0043643C = 0;
    }
}

EffPCPWorkF1C *func_001874A8(u32 unused) {
    EffPCPWorkF1C *work;

    work = func_00328D68(0x24);
    work->unk18 = 0;
    work->unk1C = 375.0f;
    work->unk20 = func_0016D070(D_003B1AF0);
    return work;
}

void func_001874F0(void) {
    func_001874A8(0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187508);

void func_00187558(void) {
    func_00187508(0);
}

EffPCPWorkF1C *func_00187570(u32 unused) {
    EffPCPWorkF1C *work;

    work = func_00328D68(0x24);
    work->unk18 = 0;
    work->unk1C = 375.0f;
    work->unk20 = func_0016D070(D_003B1BB0);
    return work;
}

void func_001875B8(void) {
    func_00187570(0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001875D0);

void func_00187620(void) {
    func_001875D0(0);
}

EffPCPWorkF1C *func_00187638(u32 unused) {
    EffPCPWorkF1C *work;

    work = func_00328D68(0x24);
    work->unk18 = 0;
    work->unk1C = 375.0f;
    work->unk20 = func_0016D070(D_003B1C70);
    return work;
}

void func_00187680(void) {
    func_00187638(0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187698);

void func_001876E8(void) {
    func_00187698(0);
}

EffPCPWorkF1C *func_00187700(u32 unused) {
    EffPCPWorkF1C *work;

    work = func_00328D68(0x24);
    work->unk18 = 0;
    work->unk1C = 375.0f;
    work->unk20 = func_0016D070(D_003B1D30);
    return work;
}

void func_00187748(void) {
    func_00187700(0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187760);

void func_001877B0(void) {
    func_00187760(0);
}

EffPCPWorkF1C *func_001877C8(u32 unused) {
    EffPCPWorkF1C *work;

    work = func_00328D68(0x24);
    work->unk18 = 0;
    work->unk1C = 375.0f;
    work->unk20 = func_0016D070(D_003B1DF0);
    return work;
}

void func_00187810(void) {
    func_001877C8(0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187828);

void func_00187878(void) {
    func_00187828(0);
}

EffPCPWorkF1C *func_00187890(u32 unused) {
    EffPCPWorkF1C *work;

    work = func_00328D68(0x24);
    work->unk18 = 0;
    work->unk1C = 375.0f;
    work->unk20 = func_0016D070(D_003B1EB0);
    return work;
}

void func_001878D8(void) {
    func_00187890(0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001878F0);

void func_00187940(void) {
    func_001878F0(0);
}

void func_00187958(u32 arg0) {
    func_0016D228(*(u32 *)((s32)arg0 + 0x20));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187988);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187A90);

void func_00187AA0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x10) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187AA8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187B68);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187B88);

void func_00187CA8(u32 arg0) {
    func_0016B130(*(u32 *)((s32)arg0 + 0x30));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187CD8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187E30);

void func_00187E40(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_00187E48(EffPCPWork *work, f32 val) {
    work->unk2C = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187E50);

void func_00187F90(u32 arg0) {
    func_00333918(*(u32 *)((s32)arg0 + 0xa8));
    func_003297C8(*(u32 *)((s32)arg0 + 0xac));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187FC8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188198);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188320);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188350);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001884E8);

void func_00188508(void) {
    func_00188350();
}

void func_00188520(u32 arg0) {
    func_00187F90(*(u32 *)((s32)arg0 + 0x5c));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188550);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188778);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188788);

void func_00188798(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x54) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001887A0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001887D0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188828);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188A70);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188AD0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188C78);

void func_00188C98(void) {
    func_00188AD0();
}

void func_00188CB0(u32 arg0) {
    func_00187F90(*(u32 *)((s32)arg0 + 0x7c));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188CE0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188E08);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188E18);

void func_00188E28(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x60) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188E30);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188E60);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001890E8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00189190);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001892A8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00189360);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00189500);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001898B8);

void func_001898C8(EffPCPWork *work, f32 val) {
    work->unk168 = val;
}

void func_001898D0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x170) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001898D8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001899D8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00189AD8);

void func_00189B48(EffPCPNode *node) {
    EffPCPNode *child;

    __asm__ volatile ("sqc2 vf10, 0(%0)" :: "r" (node->vector70) : "memory");
    child = node->child;
    if (child != NULL) {
        do {
            func_00189B48(child);
            child = child->next;
        } while (child != node->child);
    }
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00189BA0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00189DC8);

void func_00189DD8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

void func_00189DE0(EffPCPWorkF10 *work, f32 val) {
    work->unk10 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00189DE8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00189FF0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018A038);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018A238);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018A2B8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018A428);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018A678);

void func_0018A688(EffPCPWork *work, f32 val) {
    work->unk110 = val;
}

void func_0018A690(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x114) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018A698);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018A8E8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018A950);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018AB78);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018AC20);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018AD50);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018B118);

void func_0018B128(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x9c) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018B130);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018B350);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018B398);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018B5A8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018B628);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018B778);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018B9F8);

void func_0018BA08(EffPCPWork *work, f32 val) {
    work->unkA0 = val;
}

void func_0018BA10(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0xa4) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018BA18);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018BB38);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018BC28);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018BC90);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018BCE8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018BD88);




INCLUDE_RODATA(const s32, "effect/effPCPMisc", D_00414610);

