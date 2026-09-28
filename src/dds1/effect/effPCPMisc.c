#include "common.h"

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
    u8 pad80[0x10];  /* 0x80 */
    f32 unk90;       /* 0x90 nested work parameter */
    u8 pad94[0x8];   /* 0x94 */
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

/* Compact burst variant with a floating-point word at 0x18. */
typedef struct {
    u8 pad00[0x18];
    f32 unk18;
    f32 unk1C;
    u32 unk20;
} EffPCPBurstWork;

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

typedef struct {
    u8 flags;
    u8 pad01[3];
    u32 unk04;
    u32 unk08;
    u32 unk0C;
    u32 unk10;
    u32 unk14;
    u32 unk18;
    u32 unk1C;
    u32 unk20;
    u32 unk24;
} EffPCPCompactParams;

typedef struct {
    u8 pad00[0x10];
    u8 flags;
    u8 pad11[3];
    u32 unk14;
    u32 unk18;
    u32 unk1C;
    u32 color20;
    u32 unk24;
    u32 unk28;
    u32 unk2C;
    u32 unk30;
    u32 resource;
} EffPCPCompactWork;

typedef struct {
    u8 pad00[0x10];
    u8 flags;
    u8 pad11[3];
    u32 color14;
    u32 unk18;
    u32 unk1C;
    u32 unk20;
    u32 unk24;
    u32 unk28;
    u32 unk2C;
    u32 unk30;
    u32 unk34;
    u32 resource;
} EffPCPCompactWork3C;

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

/* Round-robin selector work behind func_0017ED98: three key/ID pairs plus a
 * counter at 0x108. Each call fires the IDs whose key has caught up.
 */
typedef struct {
    s32 keys[3];    /* 0x00 compared against count */
    u8 pad0C[0xF0]; /* 0x0C */
    s32 ids[3];     /* 0xFC fired through func_0017DCF8 */
    s32 count;      /* 0x108 round-robin counter */
} EffPCPRotateWork;

typedef struct {
    void *event;
    u8 pad04[0x1C];
} EffPCPEventEntry;

typedef struct {
    u8 pad00[0xC3C];
    u32 active;
} EffPCPEventOwner;

typedef struct {
    u8 pad00[0x58];
    u32 count;
    u8 pad5C[0x38];
    EffPCPEventEntry *entries;
    EffPCPEventOwner *owner;
    u8 pad9C[0xC];
    u32 handle;
} EffPCPEventGroup;

typedef struct {
    void *event;
    u8 pad04[0x14];
} EffPCPEventEntry18;

typedef struct {
    u8 pad00[0x58];
    u32 count;
    u8 pad5C[0xA8];
    EffPCPEventEntry18 *entries;
    EffPCPEventOwner *owner;
    u8 pad10C[0xC];
    u32 handle;
} EffPCPEventGroup18;

typedef struct {
    u32 handle;
    void *first;
    void *second;
    u8 pad0C[0x14];
} EffPCPEventPair;

typedef struct {
    u8 pad00[0x18];
    u32 count;
    u8 pad1C[0x70];
    EffPCPEventPair *entries;
    EffPCPEventOwner *firstOwner;
    EffPCPEventOwner *secondOwner;
    u8 pad98[0x8];
    u32 handle;
} EffPCPEventPairGroup;

extern void func_00190208(void *event);
extern void func_00190118();
extern s8 D_003BB04C;

extern EffPCPWork *func_0017D7A8(void);

/* Block `index` of a packed effect parameter set: data + offset table entry. */
extern void *func_00163258(void *data, s32 index);

extern EffPCPWork *D_003BD7FC;

extern void *func_002CFEB8(s32 size);

extern EffPCPWork *func_0017CA60(void *param0, void *param1);
extern u32 func_0014FEB0(u32 handle);
extern void func_0014FAB8(s32 handle);
extern void func_00186CB8(u32 handle);
extern u32 func_00187460(void *params);
extern u32 func_00186F90(void *params);
extern u32 func_00187FC0(void *params);
extern u32 func_00186C18(void *params);
extern void func_0017CF38(void *block0, void *blocks);

extern u32 func_001632E0(void *params);
extern u32 func_00163290(void *data, s32 index);
extern void func_001655D0(u32 handle);
extern u32 func_00165418(void *params);
extern void func_001629F0(u32 handle);
extern u32 func_00162A70(u32 param);
extern void func_0017DBB0(s32 id);
extern void func_0017E4A8(void *dst, void *src);
extern void func_0017E4C8(void *dst, void *src);
extern void func_002DDBF8(void);
extern void func_002DD688(f32 scale);


extern u8 D_00355008[];
extern u8 D_003550B8[];
extern u8 D_00355108[];
extern u8 D_00355158[];
extern u8 D_003551C0[];
extern u8 D_00355280[];
extern u8 D_00355340[];
extern u8 D_00355400[];
extern u8 D_003554C0[];
extern u8 D_00355580[];
extern u8 D_00355220[];
extern u8 D_003552E0[];
extern u8 D_003553A0[];
extern u8 D_00355460[];
extern u8 D_00355520[];
extern u8 D_003555E0[];

void func_00177190(EffPCPWork *work) {
    func_00151F00(work->unk1C);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001771C0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001772F8);

void func_00177308(EffPCPWork *work, u32 val) {
    work->unk14 = val;
}

void func_00177310(EffPCPWorkF18 *work, f32 val) {
    work->unk18 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177318);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177418);

void func_00177590(EffPCPWork *work) {
    s32 i;
    u32 *a;
    u32 *b;

    a = &work->unk1C;
    b = &work->unk5C;
    for (i = 7; i >= 0; i--) {
        func_001629F0(*b);
        b++;
        func_001629F0(a[1]);
        func_001629F0(a[0]);
        a += 2;
    }
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177608);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177700);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177850);

void func_00177860(EffPCPWorkF18 *work, f32 val) {
    work->unk18 = val;
}

void func_00177868(EffPCPWork *work, u32 val) {
    work->unk10 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177870);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177990);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177A68);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177AC8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177B70);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177CD0);

void func_00177CE0(EffPCPWorkF14 *work, f32 val) {
    work->unk14 = val;
}

void func_00177CE8(EffPCPWork *work, u32 val) {
    work->unk10 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177CF0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177E38);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177F90);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178028);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178130);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178260);

void func_00178270(EffPCPWorkF14 *work, f32 val) {
    work->unk14 = val;
}

void func_00178278(EffPCPWork *work, u32 val) {
    work->unk10 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178280);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178320);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001783E8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178448);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178500);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001785E8);

void func_001785F8(EffPCPWorkF14 *work, f32 val) {
    work->unk14 = val;
}

void func_00178600(EffPCPWork *work, u32 val) {
    work->unk10 = val;
}

void func_00178608(EffPCPChargeWork *work) {
    work->unk133C = 0;
    work->unk1340 = 0;
    work->color1344 = 0x80808080;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178620);

void func_001786C0(EffPCPChargeWork *work) {
    func_001629F0(work->unk134C);
    func_001629F0(work->unk1348);
    func_002D0918(work->unk1350);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001786F8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178790);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178AF0);

void func_00178B08(EffPCPChargeWork *work, f32 val) {
    work->unk1338 = val;
}

void func_00178B10(EffPCPChargeWork *work, u32 val) {
    work->unk1334 = val;
}

EffPCPWork1C *func_00178B18(EffPCPWork *src) {
    EffPCPWork1C *dst;
    u32 handle;

    dst = func_002CFEB8(0x1C);
    dst->unk14 = func_00163290(src, 0);
    handle = func_00163290(src, 1);
    dst->unk00 = 0;
    dst->unk18 = handle;
    dst->unk04 = 0;
    dst->color10 = 0x80808080;
    dst->unk08 = 0;
    return dst;
}

void func_00178B88(EffPCPWork *work) {
    func_001629F0(work->unk14);
    func_001629F0(work->unk18);
    func_002CFF98(work);
}

EffPCPWork1C *func_00178BC0(EffPCPWork *src) {
    EffPCPWork1C *dst;
    u32 handle;

    dst = func_002CFEB8(0x1C);
    dst->unk14 = func_00162A70(src->unk14);
    handle = func_00162A70(src->unk18);
    dst->unk00 = 0;
    dst->unk18 = handle;
    dst->unk04 = 0;
    dst->color10 = 0x80808080;
    dst->unk08 = 0;
    return dst;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178C28);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178CA8);

void func_00178CB8(EffPCPWork *work, u32 val) {
    work->unk10 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178CC0);

void *func_00178F80(void) {
    void *work;

    work = func_002CFEB8(0x4c);
    func_00178CC0(work);
    return work;
}

void func_00178FB8(EffPCPWork *work) {
    s32 i;
    u32 *handle;

    handle = &work->unk1C;
    for (i = 0; i < 12; i++) {
        func_001655D0(handle[i]);
    }
    func_002CFF98(work);
}

void *func_00179010(void) {
    void *work;

    work = func_002CFEB8(0x4c);
    func_00178CC0(work);
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00179048);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00179138);

void func_00179148(EffPCPWorkF14 *work, f32 val) {
    work->unk14 = val;
}

void func_00179150(EffPCPWork *work, u32 val) {
    work->unk18 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00179158);

void *func_001793F8(void) {
    void *work;

    work = func_002CFEB8(0x4c);
    func_00179158(work);
    return work;
}

void func_00179430(EffPCPWork *work) {
    s32 i;
    u32 *handle;

    handle = &work->unk1C;
    for (i = 0; i < 12; i++) {
        func_001655D0(handle[i]);
    }
    func_002CFF98(work);
}

void *func_00179488(void) {
    void *work;

    work = func_002CFEB8(0x4c);
    func_00179158(work);
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001794C0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001795B0);

void func_001795C0(EffPCPWorkF14 *work, f32 val) {
    work->unk14 = val;
}

void func_001795C8(EffPCPWork *work, u32 val) {
    work->unk18 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001795D0);

void *func_00179890(void) {
    void *work;

    work = func_002CFEB8(0x94);
    func_001795D0(work);
    return work;
}

void func_001798C8(EffPCPWork *work) {
    s32 i;
    u32 *handle;

    handle = &work->unk1C;
    for (i = 0; i < 30; i++) {
        func_001655D0(handle[i]);
    }
    func_002CFF98(work);
}

void *func_00179920(void) {
    void *work;

    work = func_002CFEB8(0x94);
    func_001795D0(work);
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00179958);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00179A48);

void func_00179A58(EffPCPWorkF14 *work, f32 val) {
    work->unk14 = val;
}

void func_00179A60(EffPCPWork *work, u32 val) {
    work->unk18 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00179A68);

void *func_00179D08(void) {
    void *work;

    work = func_002CFEB8(0x4c);
    func_00179A68(work);
    return work;
}

void func_00179D40(EffPCPWork *work) {
    s32 i;
    u32 *handle;

    handle = &work->unk1C;
    for (i = 0; i < 12; i++) {
        func_001655D0(handle[i]);
    }
    func_002CFF98(work);
}

void *func_00179D98(void) {
    void *work;

    work = func_002CFEB8(0x4c);
    func_00179A68(work);
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00179DD0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00179EC0);

void func_00179ED0(EffPCPWorkF14 *work, f32 val) {
    work->unk14 = val;
}

void func_00179ED8(EffPCPWork *work, u32 val) {
    work->unk18 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00179EE0);

void *func_0017A078(void) {
    void *work;

    work = func_002CFEB8(0x4c);
    func_00179EE0(work);
    return work;
}

void func_0017A0B0(EffPCPWork *work) {
    s32 i;
    u32 *handle;

    handle = &work->unk1C;
    for (i = 0; i < 12; i++) {
        func_001655D0(handle[i]);
    }
    func_002CFF98(work);
}

void *func_0017A108(void) {
    void *work;

    work = func_002CFEB8(0x4c);
    func_00179EE0(work);
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017A140);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017A230);

void func_0017A240(EffPCPWorkF14 *work, f32 val) {
    work->unk14 = val;
}

void func_0017A248(EffPCPWork *work, u32 val) {
    work->unk18 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017A250);

void *func_0017A3E8(void) {
    void *work;

    work = func_002CFEB8(0x3c);
    func_0017A250(work);
    return work;
}

void func_0017A420(EffPCPWork *work) {
    s32 i;
    u32 *handle;

    handle = &work->unk1C;
    for (i = 0; i < 8; i++) {
        func_001655D0(handle[i]);
    }
    func_002CFF98(work);
}

void *func_0017A478(void) {
    void *work;

    work = func_002CFEB8(0x3c);
    func_0017A250(work);
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017A4B0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017A5A0);

void func_0017A5B0(EffPCPWorkF14 *work, f32 val) {
    work->unk14 = val;
}

void func_0017A5B8(EffPCPWork *work, u32 val) {
    work->unk18 = val;
}

void func_0017A5C0(EffPCPWork *work) {
    u32 handle;

    handle = func_001632E0(D_00355008);
    work->unk18 = 0;
    work->unk1C = handle;
}

void *func_0017A5F8(void) {
    void *work;

    work = func_002CFEB8(0x20);
    func_0017A5C0(work);
    return work;
}

void func_0017A630(EffPCPWork *work) {
    func_001634D8(work->unk1C);
    func_002CFF98(work);
}

void *func_0017A660(void) {
    void *work;

    work = func_002CFEB8(0x20);
    func_0017A5C0(work);
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017A698);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017A7F0);

void func_0017A800(EffPCPWorkF10 *work, f32 val) {
    work->unk10 = val;
}

void func_0017A808(EffPCPWork *work, u32 val) {
    work->unk14 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017A810);

void *func_0017AAD0(void) {
    void *work;

    work = func_002CFEB8(0x94);
    func_0017A810(work);
    return work;
}

void func_0017AB08(EffPCPWork *work) {
    s32 i;
    u32 *handle;

    handle = &work->unk1C;
    for (i = 0; i < 30; i++) {
        func_001655D0(handle[i]);
    }
    func_002CFF98(work);
}

void *func_0017AB60(void) {
    void *work;

    work = func_002CFEB8(0x94);
    func_0017A810(work);
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017AB98);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017AC88);

void func_0017AC98(EffPCPWorkF14 *work, f32 val) {
    work->unk14 = val;
}

void func_0017ACA0(EffPCPWork *work, u32 val) {
    work->unk18 = val;
}

void func_0017ACA8(EffPCPWork *work) {
    u32 handle;

    handle = func_001632E0(D_003550B8);
    work->unk18 = 0;
    work->unk1C = handle;
}

void *func_0017ACE0(void) {
    void *work;

    work = func_002CFEB8(0x20);
    func_0017ACA8(work);
    return work;
}

void func_0017AD18(EffPCPWork *work) {
    func_001634D8(work->unk1C);
    func_002CFF98(work);
}

void *func_0017AD48(void) {
    void *work;

    work = func_002CFEB8(0x20);
    func_0017ACA8(work);
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017AD80);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017AED0);

void func_0017AEE0(EffPCPWorkF10 *work, f32 val) {
    work->unk10 = val;
}

void func_0017AEE8(EffPCPWork *work, u32 val) {
    work->unk14 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017AEF0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017AFB8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B008);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B148);

void func_0017B160(u32 unused, u32 val) {
    D_003BD7FC->unk10 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B170);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B180);

void func_0017B218(EffPCPWork *work) {
    func_00186CB8(work->unk34);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B248);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B328);

void func_0017B338(EffPCPWork *work, u32 val) {
    work->unk10 = val;
}

void func_0017B340(EffPCPWorkF1C *work, f32 val) {
    work->unk1C = val;
}

EffPCPCompactWork3C *func_0017B348(EffPCPCompactParams *params) {
    EffPCPCompactWork3C *work;

    work = func_002CFEB8(0x3C);
    work->resource = func_00187FC0(&params->unk18);
    work->unk1C = 0;
    work->color14 = 0x80808080;
    work->flags = params->flags;
    work->unk20 = params->unk04;
    work->unk24 = params->unk08;
    work->unk28 = params->unk0C;
    work->unk2C = params->unk10;
    work->unk30 = params->unk14;
    work->unk18 = params->unk24;
    return work;
}

void func_0017B3D8(void *args) {
    void *param0;

    param0 = func_00163258(args, 0);
    func_0017B348(param0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B3F8);

void func_0017B498(EffPCPWork *work) {
    func_00188050(work->unk38);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B4C8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B670);

void func_0017B680(EffPCPWork *work, u32 val) {
    work->unk14 = val;
}

void func_0017B688(EffPCPWorkF34 *work, f32 val) {
    work->unk34 = val;
}

EffPCPCompactWork3C *func_0017B690(EffPCPCompactParams *params) {
    EffPCPCompactWork3C *work;

    work = func_002CFEB8(0x3C);
    work->resource = func_00186C18(&params->unk18);
    work->unk1C = 0;
    work->color14 = 0x80808080;
    work->flags = params->flags;
    work->unk20 = params->unk04;
    work->unk24 = params->unk08;
    work->unk28 = params->unk0C;
    work->unk2C = params->unk10;
    work->unk30 = params->unk14;
    work->unk18 = params->unk1C;
    return work;
}

void func_0017B720(void *args) {
    void *param0;

    param0 = func_00163258(args, 0);
    func_0017B690(param0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B740);

void func_0017B7F0(EffPCPWork *work) {
    func_00186CB8(work->unk38);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B820);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B9C8);

void func_0017B9D8(EffPCPWork *work, u32 val) {
    work->unk14 = val;
}

void func_0017B9E0(EffPCPWorkF34 *work, f32 val) {
    work->unk34 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B9E8);

void func_0017BA78(void *args) {
    void *param0;

    param0 = func_00163258(args, 0);
    func_0017B9E8(param0);
}

void func_0017BA98(void) {
    func_0017B9E8();
}

void func_0017BAB0(EffPCPWork *work) {
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017BAC8);

void func_0017BBD8(EffPCPWork *work, u32 val) {
    work->unk24 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017BBE0);

void func_0017BC90(void *args) {
    void *param0;

    param0 = func_00163258(args, 0);
    func_0017BBE0(param0);
}

void func_0017BCB0(void) {
    func_0017BBE0();
}

void func_0017BCC8(EffPCPWork *work) {
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017BCE0);

void func_0017BDF8(EffPCPWork *work, u32 val) {
    work->unk34 = val;
}

EffPCPCompactWork *func_0017BE00(EffPCPCompactParams *params) {
    EffPCPCompactWork *work;

    work = func_002CFEB8(0x38);
    work->resource = func_00186F90(&params->unk18);
    work->flags = params->flags;
    work->unk14 = params->unk04;
    work->unk18 = params->unk08;
    work->unk1C = params->unk0C;
    work->unk28 = params->unk10;
    work->unk2C = params->unk14;
    work->color20 = 0x80808080;
    work->unk30 = 0;
    work->unk24 = params->unk24;
    return work;
}

void func_0017BE90(void *args) {
    void *param0;

    param0 = func_00163258(args, 0);
    func_0017BE00(param0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017BEB0);

void func_0017BF60(EffPCPWork *work) {
    func_00187080(work->unk34);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017BF90);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017C158);

void func_0017C168(EffPCPWork *work, u32 val) {
    work->unk20 = val;
}

EffPCPCompactWork *func_0017C170(EffPCPCompactParams *params) {
    EffPCPCompactWork *work;

    work = func_002CFEB8(0x38);
    work->resource = func_00187460(&params->unk18);
    work->flags = params->flags;
    work->unk14 = params->unk04;
    work->unk18 = params->unk08;
    work->unk1C = params->unk0C;
    work->unk28 = params->unk10;
    work->unk2C = params->unk14;
    work->color20 = 0x80808080;
    work->unk30 = 0;
    work->unk24 = params->unk24;
    return work;
}

void func_0017C200(void *args) {
    void *param0;

    param0 = func_00163258(args, 0);
    func_0017C170(param0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017C220);

void func_0017C2D0(EffPCPWork *work) {
    func_00187580(work->unk34);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017C300);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017C4F0);

void func_0017C500(EffPCPWork *work, u32 val) {
    work->unk20 = val;
}

void func_0017C508(EffPCPWork *work) {
    u32 handle;

    handle = func_001632E0(D_00355108);
    work->unk18 = 0;
    work->unk1C = handle;
}

void *func_0017C540(void) {
    void *work;

    work = func_002CFEB8(0x20);
    func_0017C508(work);
    return work;
}

void func_0017C578(EffPCPWork *work) {
    func_001634D8(work->unk1C);
    func_002CFF98(work);
}

void *func_0017C5A8(void) {
    void *work;

    work = func_002CFEB8(0x20);
    func_0017C508(work);
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017C5E0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017C798);

void func_0017C7A8(EffPCPWorkF10 *work, f32 val) {
    work->unk10 = val;
}

void func_0017C7B0(EffPCPWork *work, u32 val) {
    work->unk14 = val;
}

void func_0017C7B8(EffPCPWork *work) {
    u32 handle;

    handle = func_001632E0(D_00355158);
    work->unk18 = 0;
    work->unk1C = handle;
}

void *func_0017C7F0(void) {
    void *work;

    work = func_002CFEB8(0x20);
    func_0017C7B8(work);
    return work;
}

void func_0017C828(EffPCPWork *work) {
    func_001634D8(work->unk1C);
    func_002CFF98(work);
}

void *func_0017C858(void) {
    void *work;

    work = func_002CFEB8(0x20);
    func_0017C7B8(work);
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017C890);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017CA40);

void func_0017CA50(EffPCPWorkF10 *work, f32 val) {
    work->unk10 = val;
}

void func_0017CA58(EffPCPWork *work, u32 val) {
    work->unk14 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017CA60);

void func_0017CB88(void *args) {
    void *param0;
    void *param1;

    param0 = func_00163258(args, 0);
    param1 = func_00163258(args, 1);
    func_0017CA60(param0, param1);
}

EffPCPWork *func_0017CBD0(EffPCPWork *work) {
    EffPCPWork *child;
    u32 handle;

    child = func_0017CA60(&work->pad3C[4], NULL);
    handle = work->unk74;
    if (handle != 0) {
        child->unk74 = func_0014FEB0(handle);
    }
    return child;
}

void func_0017CC28(EffPCPWork *work) {
    s32 handle;

    handle = work->unk74;
    if (handle != 0) {
        func_0014FAB8(handle);
    }
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017CC60);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017CEB8);

void func_0017CED0(EffPCPWork *work, u32 val) {
    work->unk64 = val;
}

void func_0017CED8(void *dst, void *src) {
    func_002DD688(3.1415927f);
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf24, 0(%0)\n"
        "lqc2 vf25, 0x10(%0)\n"
        "lqc2 vf26, 0x20(%0)\n"
        "lqc2 vf27, 0x30(%0)\n"
        ".set reorder"
        : : "r"(src) : "memory");
    func_002DDBF8();
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf28, 0(%0)\n"
        "sqc2 vf29, 0x10(%0)\n"
        "sqc2 vf30, 0x20(%0)\n"
        "sqc2 vf31, 0x30(%0)\n"
        ".set reorder"
        : : "r"(dst) : "memory");
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017CF38);

void func_0017D0A0(void *data) {
    void *block0;
    void *blocks[7];
    void **dst;
    u32 i;

    block0 = func_00163258(data, 0);
    dst = blocks;
    i = 0;
    do {
        i++;
        *dst = func_00163258(data, i);
        dst++;
    } while (i < 7);
    func_0017CF38(block0, blocks);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017D118);

void func_0017D288(EffPCPWork *work) {
    s32 *handle;
    u32 i;

    handle = (s32 *)&work->unk58;
    for (i = 0; i < 7; i++) {
        func_0014FAB8(handle[14]);
        func_0014FAB8(handle[7]);
        func_0014FAB8(handle[0]);
        handle++;
    }
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017D2F8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017D4A8);

void func_0017D4B8(EffPCPWork *work, u32 val) {
    work->unk54 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017D4C0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017D7A8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017D8A8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017DA58);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017DBB0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017DCF8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017E4A8);

void func_0017E4C0(EffPCPWork *work, u32 val) {
    work->unkB8 = val;
}

void func_0017E4C8(void *dst, void *src) {
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

void func_0017E4F0(void) {
    EffPCPWork *work;

    work = func_0017D7A8();
    work->unkBC = 1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017E518);

void func_0017E668(void) {
    EffPCPWork *work;

    work = func_0017D7A8();
    work->unkBC = 2;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017E690);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017E7E0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017E978);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017EAB0);

void func_0017ED38(EffPCPRotateWork *work) {
    u32 i;
    s32 *id;

    id = work->ids;
    for (i = 0; i < 3; i++) {
        func_0017DBB0(id[i]);
    }
    func_002CFF98(work);
}

void func_0017ED98(EffPCPRotateWork *work) {
    s32 limit;
    s32 *key;
    u32 i;

    i = 0;
    limit = work->count;
    key = work->keys;
    do {
        if (*key <= limit) {
            func_0017DCF8(key[0x3F]);
            limit = work->count;
        }
        i = i + 1;
        key = key + 1;
    } while (i < 3);
    work->count = limit + 1;
}

void func_0017EE10(EffPCPRotateWork *work, void *src) {
    u32 i;
    s32 *id;

    id = work->ids;
    for (i = 0; i < 3; i++) {
        func_0017E4A8((void *)id[i], src);
    }
}

void func_0017EE70(EffPCPRotateWork *work, u32 val) {
    u32 i;
    s32 *id;

    id = work->ids;
    for (i = 0; i < 3; i++) {
        func_0017E4C0((EffPCPWork *)id[i], val);
    }
}

void func_0017EED0(EffPCPRotateWork *work, void *src) {
    u32 i;
    s32 *id;

    id = work->ids;
    for (i = 0; i < 3; i++) {
        func_0017E4C8((void *)id[i], src);
    }
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017EF30);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F008);

void func_0017F0E8(EffPCPWork *work) {
    func_001629F0(work->unk60);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F118);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F1C0);

void func_0017F1D0(EffPCPWork *work, u32 val) {
    work->unk5C = val;
}

void func_0017F1D8(EffPCPWork *work, f32 val) {
    work->unk58 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F1E0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F210);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F2D0);

void func_0017F388(EffPCPWork *work) {
    func_001629F0(work->unk64);
    func_001629F0(work->unk60);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F3C0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F4C0);

void func_0017F4D0(EffPCPWork *work, u32 val) {
    work->unk5C = val;
}

void func_0017F4D8(EffPCPWork *work, f32 val) {
    work->unk58 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F4E0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F510);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F5D0);

void func_0017F688(EffPCPWork *work) {
    func_001629F0(work->unk64);
    func_001629F0(work->unk60);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F6C0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F7C0);

void func_0017F7D0(EffPCPWork *work, u32 val) {
    work->unk5C = val;
}

void func_0017F7D8(EffPCPWork *work, f32 val) {
    work->unk58 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F7E0);

u32 func_0017F810(void) {
    D_003BB04C = 1;
    return 0;
}

u32 func_0017F820(void) {
    D_003BB04C = 1;
    return 0;
}

void func_0017F830(void) {
}

void func_0017F838(void) {
    if (D_003BB04C != 0) {
        D_003BB04C = 0;
    }
}

EffPCPWorkF1C *func_0017F850(u32 unused) {
    EffPCPWorkF1C *work;

    work = func_002CFEB8(0x24);
    work->unk18 = 0;
    work->unk1C = 375.0f;
    work->unk20 = func_00165418(D_003551C0);
    return work;
}

void func_0017F898(void) {
    func_0017F850(0);
}

EffPCPBurstWork *func_0017F8B0(u32 unused) {
    EffPCPBurstWork *work;

    work = func_002CFEB8(0x24);
    work->unk18 = 325.0f;
    work->unk1C = 225.0f;
    work->unk20 = func_00165418(D_00355220);
    return work;
}

void func_0017F900(void) {
    func_0017F8B0(0);
}

EffPCPWorkF1C *func_0017F918(u32 unused) {
    EffPCPWorkF1C *work;

    work = func_002CFEB8(0x24);
    work->unk18 = 0;
    work->unk1C = 375.0f;
    work->unk20 = func_00165418(D_00355280);
    return work;
}

void func_0017F960(void) {
    func_0017F918(0);
}

EffPCPBurstWork *func_0017F978(u32 unused) {
    EffPCPBurstWork *work;

    work = func_002CFEB8(0x24);
    work->unk18 = 325.0f;
    work->unk1C = 225.0f;
    work->unk20 = func_00165418(D_003552E0);
    return work;
}

void func_0017F9C8(void) {
    func_0017F978(0);
}

EffPCPWorkF1C *func_0017F9E0(u32 unused) {
    EffPCPWorkF1C *work;

    work = func_002CFEB8(0x24);
    work->unk18 = 0;
    work->unk1C = 375.0f;
    work->unk20 = func_00165418(D_00355340);
    return work;
}

void func_0017FA28(void) {
    func_0017F9E0(0);
}

EffPCPBurstWork *func_0017FA40(u32 unused) {
    EffPCPBurstWork *work;

    work = func_002CFEB8(0x24);
    work->unk18 = 325.0f;
    work->unk1C = 225.0f;
    work->unk20 = func_00165418(D_003553A0);
    return work;
}

void func_0017FA90(void) {
    func_0017FA40(0);
}

EffPCPWorkF1C *func_0017FAA8(u32 unused) {
    EffPCPWorkF1C *work;

    work = func_002CFEB8(0x24);
    work->unk18 = 0;
    work->unk1C = 375.0f;
    work->unk20 = func_00165418(D_00355400);
    return work;
}

void func_0017FAF0(void) {
    func_0017FAA8(0);
}

EffPCPBurstWork *func_0017FB08(u32 unused) {
    EffPCPBurstWork *work;

    work = func_002CFEB8(0x24);
    work->unk18 = 325.0f;
    work->unk1C = 225.0f;
    work->unk20 = func_00165418(D_00355460);
    return work;
}

void func_0017FB58(void) {
    func_0017FB08(0);
}

EffPCPWorkF1C *func_0017FB70(u32 unused) {
    EffPCPWorkF1C *work;

    work = func_002CFEB8(0x24);
    work->unk18 = 0;
    work->unk1C = 375.0f;
    work->unk20 = func_00165418(D_003554C0);
    return work;
}

void func_0017FBB8(void) {
    func_0017FB70(0);
}

EffPCPBurstWork *func_0017FBD0(u32 unused) {
    EffPCPBurstWork *work;

    work = func_002CFEB8(0x24);
    work->unk18 = 325.0f;
    work->unk1C = 225.0f;
    work->unk20 = func_00165418(D_00355520);
    return work;
}

void func_0017FC20(void) {
    func_0017FBD0(0);
}

EffPCPWorkF1C *func_0017FC38(u32 unused) {
    EffPCPWorkF1C *work;

    work = func_002CFEB8(0x24);
    work->unk18 = 0;
    work->unk1C = 375.0f;
    work->unk20 = func_00165418(D_00355580);
    return work;
}

void func_0017FC80(void) {
    func_0017FC38(0);
}

EffPCPBurstWork *func_0017FC98(u32 unused) {
    EffPCPBurstWork *work;

    work = func_002CFEB8(0x24);
    work->unk18 = 325.0f;
    work->unk1C = 225.0f;
    work->unk20 = func_00165418(D_003555E0);
    return work;
}

void func_0017FCE8(void) {
    func_0017FC98(0);
}

void func_0017FD00(EffPCPWork *work) {
    func_001655D0(work->unk20);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FD30);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FE38);

void func_0017FE48(EffPCPWork *work, u32 val) {
    work->unk10 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FE50);

void func_0017FF10(void *args) {
    void *param0;

    param0 = func_00163258(args, 0);
    func_0017FE50(param0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FF30);

void func_00180050(EffPCPWork *work) {
    func_001634D8(work->unk30);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180080);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001801D8);

void func_001801E8(EffPCPWork *work, u32 val) {
    work->unk24 = val;
}

void func_001801F0(EffPCPWork *work, f32 val) {
    work->unk2C = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001801F8);

void func_00180338(EffPCPWork *work) {
    func_002DAA68(work->unkA8);
    func_002D0918(work->unkAC);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180370);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180540);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001806C8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001806F8);

void func_00180890(void *args) {
    void *param0;

    param0 = func_00163258(args, 0);
    func_001806F8(param0);
}

void func_001808B0(void) {
    func_001806F8();
}

void func_001808C8(EffPCPWork *work) {
    func_00180338((EffPCPWork *)work->unk5C);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001808F8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180B20);

void func_00180B30(EffPCPWork *work, f32 value) {
    ((EffPCPWork *)work->unk5C)->unk90 = value;
}

void func_00180B40(EffPCPWork *work, u32 val) {
    work->unk54 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180B48);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180B78);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180BD0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180E18);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180E78);

void func_00181020(void *args) {
    void *param0;

    param0 = func_00163258(args, 0);
    func_00180E78(param0);
}

void func_00181040(void) {
    func_00180E78();
}

void func_00181058(EffPCPWork *work) {
    func_00180338((EffPCPWork *)work->unk7C);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181088);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001811B0);

void func_001811C0(EffPCPWork *work, f32 value) {
    ((EffPCPWork *)work->unk7C)->unk90 = value;
}

void func_001811D0(EffPCPWork *work, u32 val) {
    work->unk60 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001811D8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181208);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181490);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181538);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181650);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181708);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001818A8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181C60);

void func_00181C70(EffPCPWork *work, f32 val) {
    work->unk168 = val;
}

void func_00181C78(EffPCPWork *work, u32 val) {
    work->unk170 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181C80);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181D80);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181E80);

typedef struct EffPCPNode {
    u8 pad0[4];
    struct EffPCPNode *next;
    u8 pad8[4];
    struct EffPCPNode *child;
    u8 pad10[0x60];
    u8 vector70[0x10];
} EffPCPNode;

void func_00181EF0(EffPCPNode *node) {
    EffPCPNode *child;

    __asm__ volatile ("sqc2 vf10, 0(%0)" :: "r" (node->vector70) : "memory");
    child = node->child;
    if (child != NULL) {
        do {
            func_00181EF0(child);
            child = child->next;
        } while (child != node->child);
    }
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181F48);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182170);

void func_00182180(EffPCPWork *work, u32 val) {
    work->unk14 = val;
}

void func_00182188(EffPCPWorkF10 *work, f32 val) {
    work->unk10 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182190);

void func_00182398(void *args) {
    void *param0;
    void *param1;

    param0 = func_00163258(args, 0);
    param1 = func_00163258(args, 1);
    func_00182190(param0, param1);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001823E0);

void func_001825E0(EffPCPEventGroup18 *work) {
    u32 i = 0;
    u32 count = work->count;
    EffPCPEventEntry18 *entry = work->entries;

    if (count != 0) {
        do {
            func_00190208(entry->event);
            entry++;
            i++;
        } while (i < count);
    }
    if (work->owner->active == 0) {
        func_00190118(work->owner);
    }
    func_002D0918(work->handle);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182660);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001827D0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182A20);

void func_00182A30(EffPCPWork *work, f32 val) {
    work->unk110 = val;
}

void func_00182A38(EffPCPWork *work, u32 val) {
    work->unk114 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182A40);

void func_00182C90(void *args) {
    void *param0;
    void *param1;
    void *param2;

    param0 = func_00163258(args, 0);
    param1 = func_00163258(args, 1);
    param2 = func_00163258(args, 2);
    func_00182A40(param0, param1, param2);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182CF8);

void func_00182F20(EffPCPEventPairGroup *work) {
    u32 i = 0;
    u32 count = work->count;
    EffPCPEventPair *entry = work->entries;

    if (count != 0) {
        do {
            func_00190208(entry->first);
            func_00190208(entry->second);
            func_001655D0(entry->handle);
            entry++;
            i++;
        } while (i < count);
    }
    if (work->firstOwner->active == 0) {
        func_00190118(work->firstOwner);
    }
    if (work->secondOwner->active == 0) {
        func_00190118(work->secondOwner);
    }
    func_002D0918(work->handle);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182FC8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001830F8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001834C0);

void func_001834D0(EffPCPWork *work, u32 val) {
    work->unk9C = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001834D8);

void func_001836F8(void *args) {
    void *param0;
    void *param1;

    param0 = func_00163258(args, 0);
    param1 = func_00163258(args, 1);
    func_001834D8(param0, param1);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183740);

void func_00183950(EffPCPEventGroup *work) {
    u32 i = 0;
    u32 count = work->count;
    EffPCPEventEntry *entry = work->entries;

    if (count != 0) {
        do {
            func_00190208(entry->event);
            entry++;
            i++;
        } while (i < count);
    }
    if (work->owner->active == 0) {
        func_00190118(work->owner);
    }
    func_002D0918(work->handle);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001839D0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183B20);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183DA0);

void func_00183DB0(EffPCPWork *work, f32 val) {
    work->unkA0 = val;
}

void func_00183DB8(EffPCPWork *work, u32 val) {
    work->unkA4 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183DC0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183EE0);

void func_00183FD0(void *args) {
    void *param0;
    void *param1;
    void *param2;

    param0 = func_00163258(args, 0);
    param1 = func_00163258(args, 1);
    param2 = func_00163258(args, 2);
    func_00183EE0(param0, param1, param2);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184038);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184090);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184130);







INCLUDE_RODATA(const s32, "effect/effPCPMisc", D_003A0EF0);


INCLUDE_SDATA(const s32, "effect/effPCPMisc", D_003BB048);

INCLUDE_SDATA(const s32, "effect/effPCPMisc", D_003BB04C);

INCLUDE_SDATA(const s32, "effect/effPCPMisc", D_003BB04D);


INCLUDE_SDATA(const s32, "effect/effPCPMisc", D_003BB050);

