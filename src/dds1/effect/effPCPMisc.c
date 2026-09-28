#include "common.h"
#include "pcp_vu0.h"

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

/* Large charge-style effect work (allocation 0x1354). Three fields at
 * 0xAF0 are cleared on construction; resource handles occupy the tail.
 */
typedef struct {
    u8 pad000[0xAF0];
    u32 unkAF0;
    u32 unkAF4;
    u32 unkAF8;
    u8 padAFC[0x838];
    u32 unk1334;       /* 0x1334 spawn parameter */
    f32 unk1338;       /* 0x1338 spawn parameter */
    u32 unk133C;       /* 0x133C cleared on init */
    u32 unk1340;       /* 0x1340 cleared on init */
    u32 color1344;     /* 0x1344 initialised to grey 0x80808080 */
    u32 unk1348;       /* 0x1348 resource released on destroy */
    u32 unk134C;       /* 0x134C resource released on destroy */
    u32 unk1350;       /* 0x1350 resource released on destroy */
} EffPCPChargeWork;

/* Round-robin selector work behind effPcpRotateFireIds: three key/ID pairs plus a
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

extern void effEventReleaseNode(void *event);
extern void func_00190118();
extern void func_002CFF98(void *ptr);
extern s8 D_003BB04C;
extern s32 D_003BB048;

extern EffPCPWork *effPcpBuildBlockSet();

/* Block `index` of a packed effect parameter set: data + offset table entry. */
extern void *effParamTableGetBlock(void *data, s32 index);

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
extern void *effPcpTripleHandleCreate(void *block0, u32 *blocks);

extern void *func_002D03F8(s32 size);
extern void *sdfResourceRetainAddress(void *resource);
extern u32 func_001632E0(void *params);
extern u32 effParamCreateFromTable(void *data, s32 index);
extern void func_001655D0(u32 handle);
extern u32 func_00165418(void *params);
extern void func_001629F0(u32 handle);
extern u32 effParamWorkDuplicate(u32 param);
extern void func_00177870(EffPCPWorkF14 *work, s32 index);
extern void func_00178280(void *work, s32 index);
extern void func_0017DBB0(s32 id);
extern void func_0017E4A8(void *dst, void *src);
extern void func_0017E4C8(void *dst, void *src);
extern void func_002DDBF8(void);
extern void func_00180540(f32 value);
extern void func_002DD688(f32 scale);
extern void func_002DD8E8(f32 angle);
extern void func_00180BD0(f32 angle);

extern void *func_00162970(u32 handle);
extern void func_00162A38(u32 handle);
extern void func_00217878(void *obj, void *table);
extern void func_00217F88(void *obj);
extern void func_002D9E98(u32 handle, s32 value);
extern void effParamWorkCallback0(u32 handle, void *vec);
extern void effParamWorkCallback3(u32 handle, u32 value);
extern u8 D_00325828[];
extern void mdlBroadcastMasked(void *obj, u32 mask);
extern void *func_00163540(u32 handle);


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
    billDispatchByKind(work->unk1C);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001771C0);

void func_001772F8(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00177308(EffPCPWork *work, u32 val) {
    work->unk14 = val;
}

void func_00177310(EffPCPWorkF18 *work, f32 val) {
    work->unk18 = val;
}

extern f32 func_002E8398(void *state);
extern u32 effMiscRand(void *state);
extern u8 D_0034DF38[];
extern void effParamWorkCallback2(u32 handle, void *mtx);
extern void mdlAddEntryPlain(void *obj, s32 a, s32 b);

typedef struct EffPCPTwinWork {
    u32 unk00;
    u32 unk04;
    u32 unk08;
    u8 pad0C[4];
    u32 color;          /* 0x10 */
    u32 unk14;
    f32 scale;          /* 0x18 */
    u32 pair[8][2];     /* 0x1C parameter handle pairs (source uses [0] and [1]) */
    u32 shared[8];      /* 0x5C */
    u32 state[8];       /* 0x7C */
    u32 counter[8];     /* 0x9C */
} EffPCPTwinWork; /* 0xBC */

extern void func_00177318(void *, s32);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177318);

EffPCPTwinWork *effTwinEffectCreateFromTable(void *src) {
    EffPCPTwinWork *work = func_002CFEB8(0xBC);
    s32 i;

    for (i = 0; i < 8; i++) {
        if (i == 0) {
            work->pair[0][0] = effParamCreateFromTable(src, 0);
            work->pair[0][1] = effParamCreateFromTable(src, 2);
        } else if (i == 1) {
            work->pair[1][0] = effParamCreateFromTable(src, 1);
            work->pair[1][1] = effParamWorkDuplicate(work->pair[0][1]);
        } else {
            work->pair[i][0] = effParamWorkDuplicate(work->pair[i & 1][0]);
            work->pair[i][1] = effParamWorkDuplicate(work->pair[i & 1][1]);
        }
        func_00177318(work, i);
    }
    work->shared[0] = effParamCreateFromTable(src, 3);
    work->state[0] = 0;
    for (i = 1; i < 8; i++) {
        work->shared[i] = effParamWorkDuplicate(work->shared[0]);
        work->state[i] = 0;
    }
    work->unk00 = 0;
    work->color = 0x80808080;
    work->scale = 1.0f;
    work->unk04 = 0;
    work->unk08 = 0;
    work->unk14 = 0;
    return work;
}

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


EffPCPTwinWork *effTwinEffectClone(EffPCPTwinWork *src) {
    EffPCPTwinWork *work = func_002CFEB8(0xBC);
    s32 i;

    for (i = 0; i < 8; i++) {
        work->pair[i][0] = effParamWorkDuplicate(src->pair[i & 1][0]);
        work->pair[i][1] = effParamWorkDuplicate(src->pair[i & 1][1]);
        work->shared[i] = effParamWorkDuplicate(src->shared[0]);
        work->state[i] = 0;
        func_00177318(work, i);
    }
    work->unk00 = 0;
    work->color = 0x80808080;
    work->scale = 1.0f;
    work->unk04 = 0;
    work->unk08 = 0;
    work->unk14 = 0;
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177700);

void func_00177850(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00177860(EffPCPWorkF18 *work, f32 val) {
    work->unk18 = val;
}

void func_00177868(EffPCPWork *work, u32 val) {
    work->unk10 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177870);

EffPCPWorkF14 *effPcpStaggerCreate(void *args) {
    EffPCPWorkF14 *work = func_002CFEB8(0x98);
    u32 *handle = (u32 *)((u8 *)work + 0x3C);
    s32 i = 0;

    do {
        if (i == 0) {
            work->unk38 = effParamCreateFromTable(args, 0);
            *(u32 *)work->pad3C = effParamCreateFromTable(args, 1);
        } else {
            handle[-1] = effParamWorkDuplicate(work->unk38);
            handle[0] = effParamWorkDuplicate(*(u32 *)work->pad3C);
        }
        handle += 2;
        func_00177870(work, i++);
    } while (i < 8);
    work->unk10 = 0x80808080;
    work->unk14 = 1.0f;
    *(u32 *)work = 0;
    *(u32 *)((u8 *)work + 4) = 0;
    *(u32 *)((u8 *)work + 8) = 0;
    return work;
}

void func_00177A68(EffPCPWork *work) {
    u32 *handle = (u32 *)((u8 *)work + 0x3C);
    s32 i;

    for (i = 7; i >= 0; i--) {
        func_001629F0(handle[-1]);
        func_001629F0(handle[0]);
        handle += 2;
    }
    func_002CFF98(work);
}

EffPCPWorkF14 *effCreatePairedResourceWork(EffPCPWork *source) {
    EffPCPWorkF14 *work = func_002CFEB8(0x98);
    u32 *handle = (u32 *)((u8 *)work + 0x3C);
    s32 i = 0;

    do {
        handle[-1] = effParamWorkDuplicate(source->unk38);
        handle[0] = effParamWorkDuplicate(*(u32 *)source->pad3C);
        handle += 2;
        func_00177870(work, i);
        i++;
    } while (i < 8);
    work->unk10 = 0x80808080;
    work->unk14 = 1.0f;
    *(u32 *)work = 0;
    *(u32 *)((u8 *)work + 4) = 0;
    *(u32 *)((u8 *)work + 8) = 0;
    return work;
}

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    u8 pad0C[4];
    u32 unk10;
    f32 scale;
    f32 offset[8];
    u32 handle[16];
    u32 delay[8];
} EffPCPStaggered;

extern void effParamWorkCallback1(u32 handle, f32 value);

void effPcpStaggerUpdate(EffPCPStaggered *work) {
    void *obj[2];
    f32 pos[4];
    s32 i;

    for (i = 0; i < 8; i++) {
        if (work->delay[i] != 0) {
            work->delay[i]--;
        } else {
            obj[0] = func_00162970(work->handle[i * 2]);
            obj[1] = func_00162970(work->handle[i * 2 + 1]);
            pos[0] = work->x;
            pos[2] = work->z;
            pos[1] = (work->y - work->offset[i] + 100.0f) * work->scale;
            __asm__ volatile (
                ".set noreorder\n"
                "lqc2 vf10, 0(%0)\n"
                ".set reorder"
                : : "r"(pos));
            func_00217F88(obj[0]);
            effParamWorkCallback1(work->handle[i * 2], work->scale * 1.5f);
            effParamWorkCallback1(work->handle[i * 2 + 1], 1.5f);
            mdlBroadcastMasked(obj[1], work->unk10);
            func_00217878(obj[0], D_00325828);
            func_002D9E98(((EffPCPWork *)obj[0])->unk18, 1);
            func_00217F88(obj[1]);
            func_00217878(obj[1], D_00325828);
        }
    }
}

void func_00177CD0(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00177CE0(EffPCPWorkF14 *work, f32 val) {
    work->unk14 = val;
}

void func_00177CE8(EffPCPWork *work, u32 val) {
    work->unk10 = val;
}

typedef struct EffPCPCrossWork {
    u32 unk00;
    u32 unk04;
    u32 unk08;
    u8 pad0C[4];
    u32 color;        /* 0x10 */
    f32 scale;        /* 0x14 */
    u32 base;         /* 0x18 handle of the anchor model */
    u32 handle[4][3]; /* 0x1C */
    u8 pad4C[0x60];
    u32 state[4][3];  /* 0xAC */
} EffPCPCrossWork; /* 0xDC */

extern void func_00177CF0(void *, s32, s32);
extern void func_002DD708(f32 angle);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177CF0);

EffPCPCrossWork *effCrossEffectCreateFromTable(void *src) {
    EffPCPCrossWork *work = func_002CFEB8(0xDC);
    s32 i;
    s32 j;

    work->base = effParamCreateFromTable(src, 0);
    mdlAddEntryPlain(func_00162970(work->base), 0, 0);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            if (j == 0) {
                work->handle[i][j] = effParamCreateFromTable(src, i + 1);
            } else {
                work->handle[i][j] = effParamWorkDuplicate(work->handle[i][0]);
            }
            func_00177CF0(work, i, j);
        }
    }
    work->unk00 = 0;
    work->color = 0x80808080;
    work->scale = 1.0f;
    work->unk04 = 0;
    work->unk08 = 0;
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177F90);


EffPCPCrossWork *effCrossEffectClone(EffPCPCrossWork *src) {
    EffPCPCrossWork *work = func_002CFEB8(0xDC);
    s32 i;
    s32 j;

    work->base = effParamWorkDuplicate(src->base);
    mdlAddEntryPlain(func_00162970(work->base), 0, 0);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            work->handle[i][j] = effParamWorkDuplicate(src->handle[i][0]);
            func_00177CF0(work, i, j);
        }
    }
    work->unk00 = 0;
    work->color = 0x80808080;
    work->scale = 1.0f;
    work->unk04 = 0;
    work->unk08 = 0;
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178130);

void func_00178260(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00178270(EffPCPWorkF14 *work, f32 val) {
    work->unk14 = val;
}

void func_00178278(EffPCPWork *work, u32 val) {
    work->unk10 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178280);

EffPCPWorkF14 *effCreateIndexedResourceWork(void *source) {
    EffPCPWorkF14 *work = func_002CFEB8(0x60);
    u32 *handle = (u32 *)((u8 *)work + 0x1C);
    s32 i;

    for (i = 0; i < 6; i++) {
        if (i == 0) {
            work->unk1C = effParamCreateFromTable(source, 6);
        }
        handle[-1] = effParamCreateFromTable(source, i);
        handle[0] = effParamWorkDuplicate(work->unk1C);
        handle += 2;
        func_00178280(work, i);
    }
    work->unk10 = 0x80808080;
    work->unk14 = 1.0f;
    *(u32 *)work = 0;
    *(u32 *)((u8 *)work + 4) = 0;
    *(u32 *)((u8 *)work + 8) = 0;
    return work;
}

void func_001783E8(EffPCPWork *work) {
    u32 *handle = (u32 *)((u8 *)work + 0x1C);
    s32 i;

    for (i = 5; i >= 0; i--) {
        func_001629F0(handle[-1]);
        func_001629F0(handle[0]);
        handle += 2;
    }
    func_002CFF98(work);
}

EffPCPWorkF14 *effCopyIndexedResourceWork(EffPCPWork *source) {
    u32 *sourceHandle;
    u32 *workHandle;
    s32 i;
    EffPCPWorkF14 *work = func_002CFEB8(0x60);
    sourceHandle = (u32 *)((u8 *)source + 0x1C);
    workHandle = (u32 *)((u8 *)work + 0x1C);

    for (i = 0; i < 6; i++) {
        workHandle[-1] = effParamWorkDuplicate(sourceHandle[-1]);
        workHandle[0] = effParamWorkDuplicate(sourceHandle[0]);
        sourceHandle += 2;
        workHandle += 2;
        func_00178280(work, i);
    }
    work->unk10 = 0x80808080;
    work->unk14 = 1.0f;
    *(u32 *)work = 0;
    *(u32 *)((u8 *)work + 4) = 0;
    *(u32 *)((u8 *)work + 8) = 0;
    return work;
}

typedef struct {
    u8 pad00[0x10];
    u32 unk10;
    u8 pad14[4];
    u32 handle[12];
    u32 delay[6];
} EffPCPDelayedPairs;

void effPcpDelayedPairsUpdate(EffPCPDelayedPairs *work) {
    void *obj[2];
    u128 vec;
    s32 i;

    for (i = 0; i < 6; i++) {
        if (work->delay[i] != 0) {
            work->delay[i]--;
        } else {
            obj[0] = func_00162970(work->handle[i * 2]);
            obj[1] = func_00162970(work->handle[i * 2 + 1]);
            __asm__ volatile (
                ".set noreorder\n"
                "lqc2 vf10, 0(%0)\n"
                ".set reorder"
                : : "r"(work) : "memory");
            func_00217F88(obj[0]);
            mdlBroadcastMasked(obj[1], work->unk10);
            func_00217878(obj[0], D_00325828);
            func_002D9E98(((EffPCPWork *)obj[0])->unk18, 1);
            __asm__ volatile (
                ".set noreorder\n"
                "sqc2 vf10, 0(%0)\n"
                ".set reorder"
                : : "r"(&vec));
            func_00217F88(obj[1]);
            func_00217878(obj[1], D_00325828);
        }
    }
}

void func_001785E8(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

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

EffPCPChargeWork *effCreateChargeWork(void *source) {
    void *resource = func_002D03F8(0x1354);
    EffPCPChargeWork *work = sdfResourceRetainAddress(resource);
    work->unk1350 = (u32)resource;
    work->unk134C = effParamCreateFromTable(source, 0);
    work->unk1348 = effParamCreateFromTable(source, 1);
    func_00178608(work);
    work->unkAF0 = 0;
    work->unkAF4 = 0;
    work->unkAF8 = 0;
    work->unk1334 = 0x80808080;
    work->unk1338 = 1.0f;
    return work;
}

void func_001786C0(EffPCPChargeWork *work) {
    func_001629F0(work->unk134C);
    func_001629F0(work->unk1348);
    func_002D0918(work->unk1350);
}

EffPCPChargeWork *effCopyChargeResources(EffPCPChargeWork *source) {
    void *resource = func_002D03F8(0x1354);
    EffPCPChargeWork *work = sdfResourceRetainAddress(resource);
    u32 firstHandle = source->unk134C;
    work->unk1350 = (u32)resource;
    work->unk134C = effParamWorkDuplicate(firstHandle);
    work->unk1348 = effParamWorkDuplicate(source->unk1348);
    func_00178608(work);
    work->unkAF0 = 0;
    work->unkAF4 = 0;
    work->unkAF8 = 0;
    work->unk1334 = 0x80808080;
    work->unk1338 = 1.0f;
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178790);

void func_00178AF0(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0xAF0, src);
}

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
    dst->unk14 = effParamCreateFromTable(src, 0);
    handle = effParamCreateFromTable(src, 1);
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
    dst->unk14 = effParamWorkDuplicate(src->unk14);
    handle = effParamWorkDuplicate(src->unk18);
    dst->unk00 = 0;
    dst->unk18 = handle;
    dst->unk04 = 0;
    dst->color10 = 0x80808080;
    dst->unk08 = 0;
    return dst;
}

void effPcpSpawnOnce(EffPCPWork *work) {
    void *obj;
    u128 vec;

    obj = func_00162970(work->unk14);
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf10, 0(%0)\n"
        ".set reorder"
        : : "r"(work) : "memory");
    func_00217F88(obj);
    effParamWorkCallback3(work->unk18, work->unk10);
    func_00217878(obj, D_00325828);
    func_002D9E98(((EffPCPWork *)obj)->unk18, 1);
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf10, %0\n"
        ".set reorder"
        : "=m"(vec) : : "memory");
    effParamWorkCallback0(work->unk18, &vec);
    func_00162A38(work->unk18);
}

void func_00178CA8(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

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

void func_00179138(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

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

void func_001795B0(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

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

void func_00179A48(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

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

void func_00179EC0(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

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

void func_0017A230(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

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

void func_0017A5A0(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

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

void func_0017A7F0(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

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

void func_0017AC88(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

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

void func_0017AED0(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0017AEE0(EffPCPWorkF10 *work, f32 val) {
    work->unk10 = val;
}

void func_0017AEE8(EffPCPWork *work, u32 val) {
    work->unk14 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017AEF0);

void effPcpSharedWorkRelease(EffPCPWork *work)
{
    func_002CFF98(work);
    if (--D_003BB048 != 0) {
        return;
    }
    func_00186CB8(D_003BD7FC->unk38);
    func_002CFF98(D_003BD7FC);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B008);

void func_0017B148(void *work, void *src) {
    PCP_COPY_VECTOR(D_003BD7FC, src);
}

void func_0017B160(u32 unused, u32 val) {
    D_003BD7FC->unk10 = val;
}

void effSetSharedScale(u32 unused, f32 value) {
    ((EffPCPWorkF1C *)D_003BD7FC)->unk1C = value;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B180);

void func_0017B218(EffPCPWork *work) {
    func_00186CB8(work->unk34);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B248);

void func_0017B328(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

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

    param0 = effParamTableGetBlock(args, 0);
    func_0017B348(param0);
}

/* Effect parameter block rebuilt on the stack from a live work area: the
 * trailing resource description (11 words) is copied by struct assignment. */
typedef struct {
    u32 word[11];
} EffPCPRes44;

typedef struct {
    u8 flags;
    u8 pad01[3];
    u32 unk04;
    u32 unk08;
    u32 unk0C;
    u32 unk10;
    u32 unk14;
    EffPCPRes44 res;
} EffPCPParams44;

typedef struct {
    u32 word[9];
} EffPCPCompactRes;

typedef struct {
    u8 flags;
    u8 pad01[3];
    u32 unk04;
    u32 unk08;
    u32 unk0C;
    u32 unk10;
    u32 unk14;
    EffPCPCompactRes res;
} EffPCPCompactParams3C;

typedef struct {
    u8 pad00[0x10];
    u8 flags;
    u8 pad11[0xF];
    u32 unk20;
    u32 unk24;
    u32 unk28;
    u32 unk2C;
    u32 unk30;
    u32 unk34;
    EffPCPCompactRes *resource;
} EffPCPCompactSrc;

void effPcpCompactRespawn(EffPCPCompactSrc *work) {
    EffPCPCompactParams3C params;

    params.flags = work->flags;
    params.unk04 = work->unk20;
    params.unk08 = work->unk24;
    params.unk0C = work->unk28;
    params.unk10 = work->unk2C;
    params.unk14 = work->unk30;
    params.res = *work->resource;
    func_0017B348((EffPCPCompactParams *)&params);
}

void func_0017B498(EffPCPWork *work) {
    func_00188050(work->unk38);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B4C8);

void func_0017B670(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

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

    param0 = effParamTableGetBlock(args, 0);
    func_0017B690(param0);
}

typedef struct {
    u8 pad00[0x10];
    u8 flags;
    u8 pad11[0xf];
    u32 unk20;
    u32 unk24;
    u32 unk28;
    u32 unk2C;
    u32 unk30;
    u8 pad34[0x4];
    EffPCPRes44 *resource;
} EffPCPSrcA;

void effPcpCompactLongRespawn(EffPCPSrcA *work) {
    EffPCPParams44 params;

    params.flags = work->flags;
    params.unk04 = work->unk20;
    params.unk08 = work->unk24;
    params.unk0C = work->unk28;
    params.unk10 = work->unk2C;
    params.unk14 = work->unk30;
    params.res = *work->resource;
    func_0017B690((EffPCPCompactParams *)&params);
}

void func_0017B7F0(EffPCPWork *work) {
    func_00186CB8(work->unk38);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B820);

void func_0017B9C8(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0017B9D8(EffPCPWork *work, u32 val) {
    work->unk14 = val;
}

void func_0017B9E0(EffPCPWorkF34 *work, f32 val) {
    work->unk34 = val;
}

typedef struct {
    u32 word[9];
} EffPCPBlock36;

typedef struct {
    EffPCPBlock36 head;
    u32 color24;
    u32 unk28;
    u32 unk2C;
} EffPCPFlat30;

void *effPcpCopyWork(src)
    EffPCPFlat30 *src;
{
    EffPCPFlat30 *dst;

    dst = func_002CFEB8(0x30);
    dst->head = src->head;
    dst->color24 = 0x80808080;
    dst->unk2C = 0;
    dst->unk28 = src->head.word[3];
    return dst;
}

void func_0017BA78(void *args) {
    void *param0;

    param0 = effParamTableGetBlock(args, 0);
    effPcpCopyWork(param0);
}

void func_0017BA98(void) {
    effPcpCopyWork();
}

void func_0017BAB0(EffPCPWork *work) {
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017BAC8);

void func_0017BBD8(EffPCPWork *work, u32 val) {
    work->unk24 = val;
}

typedef struct {
    u32 word[13];
} EffPCPBlock52;

typedef struct {
    EffPCPBlock52 head;
    u32 color34;
    u32 unk38;
    u32 unk3C;
} EffPCPFlat40;

void *effPcpCopyWorkLong(src)
    EffPCPFlat40 *src;
{
    EffPCPFlat40 *dst;

    dst = func_002CFEB8(0x40);
    dst->head = src->head;
    dst->color34 = 0x80808080;
    dst->unk3C = 0;
    dst->unk38 = src->head.word[3];
    return dst;
}

void func_0017BC90(void *args) {
    void *param0;

    param0 = effParamTableGetBlock(args, 0);
    effPcpCopyWorkLong(param0);
}

void func_0017BCB0(void) {
    effPcpCopyWorkLong();
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

    param0 = effParamTableGetBlock(args, 0);
    func_0017BE00(param0);
}

typedef struct {
    u8 pad00[0x10];
    u8 flags;
    u8 pad11[0x3];
    u32 unk14;
    u32 unk18;
    u32 unk1C;
    u8 pad20[0x8];
    u32 unk28;
    u32 unk2C;
    u8 pad30[0x4];
    EffPCPRes44 *resource;
} EffPCPSrcB;

void effPcpChargeRespawn(EffPCPSrcB *work) {
    EffPCPParams44 params;

    params.flags = work->flags;
    params.unk04 = work->unk14;
    params.unk08 = work->unk18;
    params.unk0C = work->unk1C;
    params.unk10 = work->unk28;
    params.unk14 = work->unk2C;
    params.res = *work->resource;
    func_0017BE00((EffPCPCompactParams *)&params);
}

void func_0017BF60(EffPCPWork *work) {
    func_00187080(work->unk34);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017BF90);

void func_0017C158(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

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

    param0 = effParamTableGetBlock(args, 0);
    func_0017C170(param0);
}

typedef struct {
    u8 pad00[0x10];
    u8 flags;
    u8 pad11[0x3];
    u32 unk14;
    u32 unk18;
    u32 unk1C;
    u8 pad20[0x8];
    u32 unk28;
    u32 unk2C;
    u8 pad30[0x4];
    EffPCPRes44 *resource;
} EffPCPSrcC;

void effPcpChargeLongRespawn(EffPCPSrcC *work) {
    EffPCPParams44 params;

    params.flags = work->flags;
    params.unk04 = work->unk14;
    params.unk08 = work->unk18;
    params.unk0C = work->unk1C;
    params.unk10 = work->unk28;
    params.unk14 = work->unk2C;
    params.res = *work->resource;
    func_0017C170((EffPCPCompactParams *)&params);
}

void func_0017C2D0(EffPCPWork *work) {
    func_00187580(work->unk34);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017C300);

void func_0017C4F0(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

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

void func_0017C798(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

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

void func_0017CA40(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

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

    param0 = effParamTableGetBlock(args, 0);
    param1 = effParamTableGetBlock(args, 1);
    func_0017CA60(param0, param1);
}

EffPCPWork *effPcpCloneWithOptionalHandle(EffPCPWork *work) {
    EffPCPWork *child;
    u32 handle;

    child = func_0017CA60(&work->pad3C[4], NULL);
    handle = work->unk74;
    if (handle != 0) {
        child->unk74 = func_0014FEB0(handle);
    }
    return child;
}

void effPcpReleaseOptionalHandle(EffPCPWork *work) {
    s32 handle;

    handle = work->unk74;
    if (handle != 0) {
        func_0014FAB8(handle);
    }
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017CC60);

void func_0017CEB8(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0x40, src);
}

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

typedef struct {
    u32 word[20];
} EffPCPBlock80;

typedef struct {
    EffPCPBlock80 head;
    u32 unk50;
    u32 color54;
    u32 handleA[7];
    u32 handleB[7];
    u32 handleC[7];
} EffPCPTripleWork;

extern u32 func_0014FD20(u32 param);
extern u32 func_0014FEB0(u32 handle);

void *effPcpTripleHandleCreate(void *block0, u32 *blocks) {
    EffPCPTripleWork *work;
    u32 *handle;
    u32 i;

    work = func_002CFEB8(0xAC);
    work->head = *(EffPCPBlock80 *)block0;
    work->unk50 = 0;
    work->color54 = 0x80808080;
    handle = work->handleA;
    for (i = 0; i < 7; i++) {
        handle[0] = func_0014FD20(blocks[i]);
        handle[7] = func_0014FEB0(handle[0]);
        handle[14] = func_0014FEB0(handle[0]);
        handle++;
    }
    return work;
}

void func_0017D0A0(void *data) {
    void *block0;
    void *blocks[7];
    void **dst;
    u32 i;

    block0 = effParamTableGetBlock(data, 0);
    dst = blocks;
    i = 0;
    do {
        i++;
        *dst = effParamTableGetBlock(data, i);
        dst++;
    } while (i < 7);
    effPcpTripleHandleCreate(block0, blocks);
}

EffPCPTripleWork *effPcpTripleHandleDuplicate(EffPCPTripleWork *src) {
    EffPCPTripleWork *work;
    u32 *from;
    u32 *to;
    u32 i;

    work = func_002CFEB8(0xAC);
    work->head = src->head;
    work->unk50 = 0;
    work->color54 = 0x80808080;
    from = src->handleC;
    to = work->handleC;
    for (i = 0; i < 7; i++) {
        to[-14] = func_0014FEB0(from[-14]);
        to[-7] = func_0014FEB0(from[-7]);
        to[0] = func_0014FEB0(from[0]);
        from++;
        to++;
    }
    return work;
}

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

void func_0017D4A8(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0017D4B8(EffPCPWork *work, u32 val) {
    work->unk54 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017D4C0);

extern EffPCPWork *func_0017D4C0(void *first, void **blocks);

typedef struct {
    void *block1;
    void *group[5];
    void *block7;
    void *block8;
    void *block9;
    void *tail[5];
    void *block15;
} EffPCPBlockSet;

EffPCPWork *effPcpBuildBlockSet(args)
    void *args;
{
    EffPCPBlockSet set;
    void *first;
    u32 i;

    first = effParamTableGetBlock(args, 0);
    set.block1 = effParamTableGetBlock(args, 1);
    for (i = 0; i < 5; i++) {
        set.group[i] = effParamTableGetBlock(args, 2 + i);
    }
    set.block7 = effParamTableGetBlock(args, 7);
    set.block8 = effParamTableGetBlock(args, 8);
    set.block9 = effParamTableGetBlock(args, 9);
    for (i = 0; i < 5; i++) {
        set.tail[i] = effParamTableGetBlock(args, 10 + i);
    }
    set.block15 = effParamTableGetBlock(args, 15);
    return func_0017D4C0(first, &set);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017D8A8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017DA58);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017DBB0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017DCF8);

void func_0017E4A8(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0x60, src);
}

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

    work = effPcpBuildBlockSet();
    work->unkBC = 1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017E518);

void func_0017E668(void) {
    EffPCPWork *work;

    work = effPcpBuildBlockSet();
    work->unkBC = 2;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017E690);

typedef struct {
    u32 word[63];
} EffPCPBlock252;

EffPCPRotateWork *effPcpRotateCreate(EffPCPBlock252 *src, u32 *blocks) {
    EffPCPRotateWork *work;
    u8 *sub;
    u32 *p;
    u32 i;

    work = func_002CFEB8(0x10C);
    *(EffPCPBlock252 *)work = *src;
    sub = (u8 *)src + 0xC;
    work->count = 0;
    for (i = 0; i < 3; i++) {
        blocks[3] = blocks[i];
        work->ids[i] = (s32)func_0017D4C0(sub, (void **)&blocks[3]);
        sub += 0x50;
    }
    return work;
}

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

void effPcpRotateFireIds(EffPCPRotateWork *work) {
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

extern f32 func_002E8398(void *state);
extern u8 D_0034DF38[];

/* Small PCP effect with a 4x4 matrix at 0x10 (0x68 bytes). */
typedef struct EffPCPSpinWork {
    u8 pad00[0x10];  /* 0x00 task header */
    u128 matrix[4];  /* 0x10 transform */
    s32 frame;       /* 0x50 */
    f32 angle;       /* 0x54 random start angle */
    f32 scale;       /* 0x58 */
    u32 color;       /* 0x5C */
    void *handle0;   /* 0x60 */
    void *handle1;   /* 0x64 */
} EffPCPSpinWork; /* 0x68 */

/* libvu0 sceVu0UnitMatrix expansion: qmfc2 of vf0 (0,0,0,1), then MMI shuffles */
#define PCP_UNIT_MATRIX(dst) __asm__ volatile ( \
    ".set noreorder\n\tqmfc2.ni $5, $vf0\n\tpextuw $4, $0, $5\n\tpextuw $2, $0, $4\n\tpextuw $3, $4, $0\n\t" \
    "sq $2, 0(%0)\n\tsq $3, 0x10(%0)\n\tsq $4, 0x20(%0)\n\tsq $5, 0x30(%0)\n\t.set reorder" \
    : : "r" (dst) : "$2", "$3", "$4", "$5", "memory")

extern u32 effMiscRand(void *state);

EffPCPSpinWork *func_0017EF30(void *src) {
    EffPCPSpinWork *work = func_002CFEB8(0x64);

    work->frame = 0;
    work->scale = 1.0f;
    work->color = 0x80808080;
    work->handle0 = (void *)effParamCreateFromTable(src, 0);
    PCP_UNIT_MATRIX(work->matrix);
    work->angle = (func_002E8398(D_0034DF38) - 0.5f) * 2.0f * 0.87266457f;
    if (effMiscRand(D_0034DF38) & 1) {
        work->angle += 3.14159265f;
    }
    return work;
}

/* Single-handle variant: allocation stops before handle1 (0x64 bytes). */
EffPCPSpinWork *func_0017F008(EffPCPSpinWork *src) {
    EffPCPSpinWork *work = func_002CFEB8(0x64);

    work->frame = 0;
    work->scale = 1.0f;
    work->color = 0x80808080;
    PCP_UNIT_MATRIX(work->matrix);
    work->angle = (func_002E8398(D_0034DF38) - 0.5f) * 2.0f * 0.87266457f;
    if (effMiscRand(D_0034DF38) & 1) {
        work->angle += 3.14159265f;
    }
    work->handle0 = (void *)effParamWorkDuplicate((u32)src->handle0);
    return work;
}

void func_0017F0E8(EffPCPWork *work) {
    func_001629F0(work->unk60);
    func_002CFF98(work);
}

extern void effParamWorkCallback2(u32 handle, void *mtx);

void effSpinEffectUpdate(EffPCPSpinWork *work) {
    u32 handle = (u32)work->handle0;
    u128 mtx[4];

    effParamWorkCallback0(handle, work);
    effParamWorkCallback1(handle, work->scale);
    effParamWorkCallback3(handle, work->color);
    func_002DD688(work->angle);
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf24, 0(%0)\n"
        "lqc2 vf25, 0x10(%0)\n"
        "lqc2 vf26, 0x20(%0)\n"
        "lqc2 vf27, 0x30(%0)\n"
        ".set reorder"
        : : "r"(work->matrix) : "memory");
    func_002DDBF8();
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf28, 0(%0)\n"
        "sqc2 vf29, 0x10(%0)\n"
        "sqc2 vf30, 0x20(%0)\n"
        "sqc2 vf31, 0x30(%0)\n"
        ".set reorder"
        : : "r"(mtx) : "memory");
    effParamWorkCallback2(handle, mtx);
    func_00162A38(handle);
    work->frame++;
}

void func_0017F1C0(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0017F1D0(EffPCPWork *work, u32 val) {
    work->unk5C = val;
}

void func_0017F1D8(EffPCPWork *work, f32 val) {
    work->unk58 = val;
}

void effCopyMatrix(EffPCPWork *work, void *src) {
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf28, 0(%0)\n"
        "lqc2 vf29, 0x10(%0)\n"
        "lqc2 vf30, 0x20(%0)\n"
        "lqc2 vf31, 0x30(%0)\n"
        ".set reorder"
        : : "r"(src) : "memory");
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf28, 0(%0)\n"
        "sqc2 vf29, 0x10(%0)\n"
        "sqc2 vf30, 0x20(%0)\n"
        "sqc2 vf31, 0x30(%0)\n"
        ".set reorder"
        : : "r"(&work->unk10) : "memory");
}

EffPCPSpinWork *effSpinEffectCreateFromTable(void *src) {
    EffPCPSpinWork *work = func_002CFEB8(0x68);

    work->frame = 0;
    work->scale = 1.0f;
    work->color = 0x80808080;
    work->handle0 = (void *)effParamCreateFromTable(src, 0);
    work->handle1 = (void *)effParamCreateFromTable(src, 1);
    PCP_UNIT_MATRIX(work->matrix);
    work->angle = (func_002E8398(D_0034DF38) - 0.5f) * 2.0f * 0.87266457f;
    return work;
}

EffPCPSpinWork *effSpinEffectClone(EffPCPSpinWork *src) {
    EffPCPSpinWork *work = func_002CFEB8(0x68);

    work->frame = 0;
    work->scale = 1.0f;
    work->color = 0x80808080;
    work->handle0 = (void *)effParamWorkDuplicate((u32)src->handle0);
    work->handle1 = (void *)effParamWorkDuplicate((u32)src->handle1);
    PCP_UNIT_MATRIX(work->matrix);
    work->angle = (func_002E8398(D_0034DF38) - 0.5f) * 2.0f * 0.87266457f;
    return work;
}

void func_0017F388(EffPCPWork *work) {
    func_001629F0(work->unk64);
    func_001629F0(work->unk60);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F3C0);

void func_0017F4C0(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0017F4D0(EffPCPWork *work, u32 val) {
    work->unk5C = val;
}

void func_0017F4D8(EffPCPWork *work, f32 val) {
    work->unk58 = val;
}

void func_0017F4E0(EffPCPWork *work, void *src) {
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf28, 0(%0)\n"
        "lqc2 vf29, 0x10(%0)\n"
        "lqc2 vf30, 0x20(%0)\n"
        "lqc2 vf31, 0x30(%0)\n"
        ".set reorder"
        : : "r"(src) : "memory");
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf28, 0(%0)\n"
        "sqc2 vf29, 0x10(%0)\n"
        "sqc2 vf30, 0x20(%0)\n"
        "sqc2 vf31, 0x30(%0)\n"
        ".set reorder"
        : : "r"(&work->unk10) : "memory");
}

EffPCPSpinWork *func_0017F510(void *src) {
    EffPCPSpinWork *work = func_002CFEB8(0x68);

    work->frame = 0;
    work->scale = 1.0f;
    work->color = 0x80808080;
    work->handle0 = (void *)effParamCreateFromTable(src, 0);
    work->handle1 = (void *)effParamCreateFromTable(src, 1);
    PCP_UNIT_MATRIX(work->matrix);
    work->angle = (func_002E8398(D_0034DF38) - 0.5f) * 2.0f * 0.87266457f;
    return work;
}

EffPCPSpinWork *func_0017F5D0(EffPCPSpinWork *src) {
    EffPCPSpinWork *work = func_002CFEB8(0x68);

    work->frame = 0;
    work->scale = 1.0f;
    work->color = 0x80808080;
    work->handle0 = (void *)effParamWorkDuplicate((u32)src->handle0);
    work->handle1 = (void *)effParamWorkDuplicate((u32)src->handle1);
    PCP_UNIT_MATRIX(work->matrix);
    work->angle = (func_002E8398(D_0034DF38) - 0.5f) * 2.0f * 0.87266457f;
    return work;
}

void func_0017F688(EffPCPWork *work) {
    func_001629F0(work->unk64);
    func_001629F0(work->unk60);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F6C0);

void func_0017F7C0(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0017F7D0(EffPCPWork *work, u32 val) {
    work->unk5C = val;
}

void func_0017F7D8(EffPCPWork *work, f32 val) {
    work->unk58 = val;
}

void func_0017F7E0(EffPCPWork *work, void *src) {
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf28, 0(%0)\n"
        "lqc2 vf29, 0x10(%0)\n"
        "lqc2 vf30, 0x20(%0)\n"
        "lqc2 vf31, 0x30(%0)\n"
        ".set reorder"
        : : "r"(src) : "memory");
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf28, 0(%0)\n"
        "sqc2 vf29, 0x10(%0)\n"
        "sqc2 vf30, 0x20(%0)\n"
        "sqc2 vf31, 0x30(%0)\n"
        ".set reorder"
        : : "r"(&work->unk10) : "memory");
}

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

void func_0017FE38(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0017FE48(EffPCPWork *work, u32 val) {
    work->unk10 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FE50);

void func_0017FF10(void *args) {
    void *param0;

    param0 = effParamTableGetBlock(args, 0);
    func_0017FE50(param0);
}

typedef struct {
    u32 word[19];
} EffPCPRes76;

typedef struct {
    u32 unk00;
    u32 unk04;
    u32 unk08;
    u32 unk0C;
    u32 unk10;
    EffPCPRes76 res;
} EffPCPParams76;

typedef struct {
    u8 pad00[0x10];
    u32 unk10;
    u32 unk14;
    u32 unk18;
    u32 unk1C;
    u32 unk20;
    u8 pad24[0xC];
    u32 resource;
} EffPCPSrc76;

extern void func_0017FE50(EffPCPParams76 *params);

void effPcpScaledRespawn(EffPCPSrc76 *work) {
    EffPCPParams76 params;
    EffPCPRes76 *res;

    res = func_00163540(work->resource);
    params.unk00 = work->unk10;
    params.unk04 = work->unk14;
    params.unk08 = work->unk18;
    params.unk0C = work->unk1C;
    params.unk10 = work->unk20;
    params.res = *res;
    func_0017FE50(&params);
}

void func_00180050(EffPCPWork *work) {
    func_001634D8(work->unk30);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180080);

void func_001801D8(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

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

typedef struct {
    u8 pad00[0x28];
    f32 scale;
    u8 pad2C[0x24];
    u32 state;
} EffPCPSubEffectWork;

void effResetChild(EffPCPSubEffectWork *work) {
    func_00180540(work->scale);
    work->state = 0;
}

typedef struct EffPCPBlock50 {
    u32 word[20];
} EffPCPBlock50;

extern u8 *func_001801F8(u32);

u8 *effBeamEffectClone(src)
    u8 *src;
{
    u8 *work = func_002CFEB8(0x60);
    u32 kind;
    u8 *node;
    u32 *entry;
    s32 groups;
    u32 i;

    *(EffPCPBlock50 *)work = *(EffPCPBlock50 *)src;
    *(u32 *)(work + 0x54) = 0x80808080;
    *(u32 *)(work + 0x50) = 0;
    kind = *(u32 *)(src + 0x30);
    if (kind < 3) {
        *(u32 *)(src + 0x30) = 3;
        kind = 3;
    }
    node = func_001801F8(kind);
    i = 0;
    *(u8 **)(work + 0x5C) = node;
    *(u32 *)(work + 0x58) = *(u32 *)(node + 0x9C);
    groups = *(s32 *)(node + 0x9C) >> 2;
    entry = *(u32 **)(node + 0xA4);
    for (i = 0; i < groups; i++) {
        u32 second;

        entry[0] = *(u32 *)(src + 0x3C);
        second = *(u32 *)(src + 0x44);
        entry[1] = entry[2] = second;
        entry[3] = *(u32 *)(src + 0x4C);
        entry += 4;
    }
    effResetChild((EffPCPSubEffectWork *)work);
    *(u32 *)(*(u8 **)(work + 0x5C) + 0x94) = *(u32 *)(src + 0x34);
    return work;
}

void func_00180890(void *args) {
    void *param0;

    param0 = effParamTableGetBlock(args, 0);
    effBeamEffectClone(param0);
}

void func_001808B0(void) {
    effBeamEffectClone();
}

void func_001808C8(EffPCPWork *work) {
    func_00180338((EffPCPWork *)work->unk5C);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001808F8);

void func_00180B20(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpNestedWorkSetFloat(EffPCPWork *work, f32 value) {
    ((EffPCPWork *)work->unk5C)->unk90 = value;
}

void func_00180B40(EffPCPWork *work, u32 val) {
    work->unk54 = val;
}

void func_00180B48(EffPCPWork *work, void *src) {
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf28, 0(%0)\n"
        "lqc2 vf29, 0x10(%0)\n"
        "lqc2 vf30, 0x20(%0)\n"
        "lqc2 vf31, 0x30(%0)\n"
        ".set reorder"
        : : "r"(src) : "memory");
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf28, 0(%0)\n"
        "sqc2 vf29, 0x10(%0)\n"
        "sqc2 vf30, 0x20(%0)\n"
        "sqc2 vf31, 0x30(%0)\n"
        ".set reorder"
        : : "r"(&((EffPCPWork *)work->unk5C)->pad3C[4]) : "memory");
}

void effRotateNested(EffPCPWork *work, void *src) {
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf28, 0(%0)\n"
        "lqc2 vf29, 0x10(%0)\n"
        "lqc2 vf30, 0x20(%0)\n"
        "lqc2 vf31, 0x30(%0)\n"
        ".set reorder"
        : : "r"(src) : "memory");
    func_002DD8E8(1.5707963f);
    func_002DDBF8();
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf28, 0(%0)\n"
        "sqc2 vf29, 0x10(%0)\n"
        "sqc2 vf30, 0x20(%0)\n"
        "sqc2 vf31, 0x30(%0)\n"
        ".set reorder"
        : : "r"((void *)work->unk5C) : "memory");
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180BD0);

typedef struct {
    u8 pad00[0x10];
    f32 degreesX;
    f32 degreesY;
    f32 degreesZ;
    u8 pad1C[0x14];
    f32 angle;
    f32 angle2;
    u8 pad38[0x24];
    u32 child;
    u8 pad60[0x4];
    f32 radiansX;
    f32 radiansY;
    f32 radiansZ;
    u8 pad70[0x4];
    f32 childAngle;
    f32 childAngle2;
} EffPCPAngleWork;

void effPrepareAngles(EffPCPAngleWork *work) {
    work->radiansX = work->degreesX * 0.017453291f;
    work->radiansY = work->degreesY * 0.017453291f;
    work->radiansZ = work->degreesZ * 0.017453291f;
    work->childAngle = work->angle;
    work->childAngle2 = work->angle2;
    func_00180BD0(work->angle);
    work->child = 0;
}

typedef struct EffPCPBlock5C {
    u32 word[23];
} EffPCPBlock5C;

u8 *effBeamEffectCloneLarge(src)
    u8 *src;
{
    u8 *work = func_002CFEB8(0x80);
    u32 kind;
    u8 *node;
    u32 *entry;
    s32 groups;
    u32 i;

    *(EffPCPBlock5C *)work = *(EffPCPBlock5C *)src;
    *(u32 *)(work + 0x60) = 0x80808080;
    *(u32 *)(work + 0x5C) = 0;
    kind = *(u32 *)(src + 0x3C);
    if (kind < 3) {
        *(u32 *)(src + 0x3C) = 3;
        kind = 3;
    }
    node = func_001801F8(kind);
    i = 0;
    *(u8 **)(work + 0x7C) = node;
    groups = *(s32 *)(node + 0x9C) >> 2;
    entry = *(u32 **)(node + 0xA4);
    for (i = 0; i < groups; i++) {
        u32 second;

        entry[0] = *(u32 *)(src + 0x48);
        second = *(u32 *)(src + 0x50);
        entry[1] = entry[2] = second;
        entry[3] = *(u32 *)(src + 0x58);
        entry += 4;
    }
    effPrepareAngles(work);
    *(u32 *)(*(u8 **)(work + 0x7C) + 0x94) = *(u32 *)(src + 0x40);
    return work;
}

void func_00181020(void *args) {
    void *param0;

    param0 = effParamTableGetBlock(args, 0);
    effBeamEffectCloneLarge(param0);
}

void func_00181040(void) {
    effBeamEffectCloneLarge();
}

void func_00181058(EffPCPWork *work) {
    func_00180338((EffPCPWork *)work->unk7C);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181088);

void func_001811B0(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpLinkedWorkSetFloat(EffPCPWork *work, f32 value) {
    ((EffPCPWork *)work->unk7C)->unk90 = value;
}

void func_001811D0(EffPCPWork *work, u32 val) {
    work->unk60 = val;
}

void func_001811D8(EffPCPWork *work, void *src) {
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf28, 0(%0)\n"
        "lqc2 vf29, 0x10(%0)\n"
        "lqc2 vf30, 0x20(%0)\n"
        "lqc2 vf31, 0x30(%0)\n"
        ".set reorder"
        : : "r"(src) : "memory");
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf28, 0(%0)\n"
        "sqc2 vf29, 0x10(%0)\n"
        "sqc2 vf30, 0x20(%0)\n"
        "sqc2 vf31, 0x30(%0)\n"
        ".set reorder"
        : : "r"(&((EffPCPWork *)work->unk7C)->pad3C[4]) : "memory");
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181208);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181490);

u8 *func_00181538(u8 *work) {
    u8 *copy = func_00181208(work, 0);
    u32 group;
    u32 offset;
    u32 count;
    u32 size;
    u32 stride;
    u8 *flags;
    u32 i;

    if (*(u32 *)(work + 0x174) != 0) {
        count = *(u32 *)(work + 0x58);
        size = count * 16;
        stride = count * 4;
        group = 0;
        flags = work + 0x8C;
        offset = 0;
        *(void **)(copy + 0x178) = func_002D03F8(size);
        *(void **)(copy + 0x174) = sdfResourceRetainAddress(*(void **)(copy + 0x178));
        memset(*(void **)(copy + 0x174), 0, size);
        for (; group < 4; group++) {
            u32 *slot = (u32 *)(*(u8 **)(copy + 0x174) + offset);

            if (*flags != 0) {
                u32 first = *(u32 *)(offset + *(u32 *)(work + 0x174));
                for (i = 0; i < count; i++) {
                    *slot++ = effParamWorkDuplicate(first);
                }
            }
            flags++;
            offset += stride;
        }
    }
    return copy;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181650);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181708);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001818A8);

void func_00181C60(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00181C70(EffPCPWork *work, f32 val) {
    work->unk168 = val;
}

void func_00181C78(EffPCPWork *work, u32 val) {
    work->unk170 = val;
}

typedef struct EffPCPSprayWork {
    u8 pad00[0x10];
    f32 scale;        /* 0x10 */
    u32 color;        /* 0x14 */
    u32 count;        /* 0x18 particle count */
    u32 unk1C;        /* 0x1C */
    u32 id[10];       /* 0x20 */
    f32 angle[10];    /* 0x48 random start angles */
    u32 handle[10];   /* 0x70 handle 0 is the parameter block */
} EffPCPSprayWork; /* 0x98 */

EffPCPSprayWork *effSprayEffectCreateFromTable(void *src) {
    EffPCPSprayWork *work = func_002CFEB8(0x98);
    u32 i;

    work->scale = 1.0f;
    work->color = 0x80808080;
    work->count = 10;
    work->unk1C = 0;
    for (i = 0; i < work->count; i++) {
        work->handle[i] = 0;
        work->id[i] = i;
        work->angle[i] = (func_002E8398(D_0034DF38) - 0.5f) * 2.0f * 0.87266457f + 3.14159265f;
    }
    work->handle[0] = effParamCreateFromTable(src, 0);
    return work;
}

EffPCPSprayWork *effSprayEffectClone(EffPCPSprayWork *src) {
    EffPCPSprayWork *work = func_002CFEB8(0x98);
    u32 i;

    work->unk1C = 0;
    work->scale = src->scale;
    work->count = src->count;
    work->color = 0x80808080;
    for (i = 0; i < work->count; i++) {
        work->handle[i] = 0;
        work->id[i] = i;
        work->angle[i] = (func_002E8398(D_0034DF38) - 0.5f) * 2.0f * 0.87266457f + 3.14159265f;
    }
    work->handle[0] = effParamWorkDuplicate(src->handle[0]);
    return work;
}

void effDestroyIndexedResources(EffPCPWork *work) {
    u32 i = 0;

    if (work->unk18 != 0) {
        u32 *handle = (u32 *)((u8 *)work + 0x70);
        do {
            if (*handle != 0) {
                func_001629F0(*handle);
            }
            handle++;
            i++;
        } while (i < work->unk18);
    }
    func_002CFF98(work);
}

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

void func_00182170(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

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

    param0 = effParamTableGetBlock(args, 0);
    param1 = effParamTableGetBlock(args, 1);
    func_00182190(param0, param1);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001823E0);

void effPcpEventGroupRelease(EffPCPEventGroup18 *work) {
    u32 i = 0;
    u32 count = work->count;
    EffPCPEventEntry18 *entry = work->entries;

    if (count != 0) {
        do {
            effEventReleaseNode(entry->event);
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

void func_00182A20(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

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

    param0 = effParamTableGetBlock(args, 0);
    param1 = effParamTableGetBlock(args, 1);
    param2 = effParamTableGetBlock(args, 2);
    func_00182A40(param0, param1, param2);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182CF8);

void effPcpPairedEventGroupRelease(EffPCPEventPairGroup *work) {
    u32 i = 0;
    u32 count = work->count;
    EffPCPEventPair *entry = work->entries;

    if (count != 0) {
        do {
            effEventReleaseNode(entry->first);
            effEventReleaseNode(entry->second);
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

void func_001834C0(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_001834D0(EffPCPWork *work, u32 val) {
    work->unk9C = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001834D8);

void func_001836F8(void *args) {
    void *param0;
    void *param1;

    param0 = effParamTableGetBlock(args, 0);
    param1 = effParamTableGetBlock(args, 1);
    func_001834D8(param0, param1);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183740);

void effPcpEventBatchRelease(EffPCPEventGroup *work) {
    u32 i = 0;
    u32 count = work->count;
    EffPCPEventEntry *entry = work->entries;

    if (count != 0) {
        do {
            effEventReleaseNode(entry->event);
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

void func_00183DA0(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

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

    param0 = effParamTableGetBlock(args, 0);
    param1 = effParamTableGetBlock(args, 1);
    param2 = effParamTableGetBlock(args, 2);
    func_00183EE0(param0, param1, param2);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184038);

typedef struct {
    u8 pad00[0x1C];
    void *event;
} EffPCPEventEntry32;

typedef struct {
    u8 pad00[0x1C];
    EffPCPEventEntry32 *entries;
    u32 handle;
    EffPCPEventOwner *owner;
    u32 resource;
    u8 pad2C[0xC];
    u32 count;
} EffPCPEventWork32;

void effDestroyParticleEvents(EffPCPEventWork32 *work) {
    u32 i;
    u32 count;
    EffPCPEventEntry32 *entry;

    func_001629F0(work->resource);
    count = work->count;
    entry = work->entries;
    for (i = 0; i < count; i++) {
        if (entry->event != NULL) {
            effEventReleaseNode(entry->event);
        }
        entry++;
    }
    if (work->owner->active == 0) {
        func_00190118(work->owner);
    }
    func_002D0918(work->handle);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184130);

INCLUDE_RODATA(const s32, "effect/effPCPMisc", D_003A0EF0);

INCLUDE_SDATA(const s32, "effect/effPCPMisc", D_003BB048);

INCLUDE_SDATA(const s32, "effect/effPCPMisc", D_003BB04C);

INCLUDE_SDATA(const s32, "effect/effPCPMisc", D_003BB04D);

