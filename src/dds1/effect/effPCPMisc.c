#include "common.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"


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
    u32 optionalHandle; /* 0x74 freed if nonzero */
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
    u32 mode;        /* 0xBC mode set through the singleton accessor */
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
    u32 optionalHandle; /* 0x74 freed if nonzero */
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
    u32 mode;        /* 0xBC mode set through the singleton accessor */
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
    u32 optionalHandle; /* 0x74 freed if nonzero */
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
    u32 mode;        /* 0xBC mode set through the singleton accessor */
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
    u32 optionalHandle; /* 0x74 freed if nonzero */
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
    u32 mode;        /* 0xBC mode set through the singleton accessor */
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
    u32 optionalHandle; /* 0x74 freed if nonzero */
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
    u32 mode;        /* 0xBC mode set through the singleton accessor */
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
    u32 optionalHandle; /* 0x74 freed if nonzero */
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
    u32 mode;        /* 0xBC mode set through the singleton accessor */
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
extern void effDestroyNode(s32 handle);
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

extern void func_00177870(EffPCPStaggered *work, s32 index);
extern void func_00178280(void *work, s32 index);
extern void func_0017DBB0(s32 id);
extern void effPcpCopyVector60(void *dst, void *src);
extern void func_0017E4C8(void *dst, void *src);
extern void func_002DDBF8(void);
extern void func_00180540(f32 value);
extern void func_002DD688(f32 scale);
extern void func_002DD8E8(f32 angle);
extern void func_00180BD0(f32 angle);

extern void *effParamWorkGetData(u32 handle);
extern void func_00162A38(u32 handle);
extern void func_00217878(void *obj, void *table);
extern void mdlStorePrimaryVectorVU(void *obj);
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

extern u8 D_00324680[];
extern u8 D_00324690[];
extern void effCopyVector(s32 handle, f32 *src);
extern void billInvokeCallback(s32 handle);
extern void func_00152000(s32 handle, f32 sx, f32 sy);
extern void func_00152010(s32 handle, u32 color);
extern u32 func_0018DDF8(u32 flags, u32 color);

typedef struct EffPCPRingWork {
    f32 pos[4];
    u32 color10;
    u32 color14;
    f32 scale;
    s32 handle;
} EffPCPRingWork;

/* Places a ring of 10 shrinking, brightening copies of the handle along the
 * fixed view direction. */
void func_001771C0(EffPCPRingWork *work) {
    f32 pos[4];
    f32 dir[4];
    f32 size[4];
    s32 handle;
    f32 scale;
    u32 color;
    s32 i;

    __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(D_00324690));
    __asm__ volatile(".set noreorder\n\tlqc2 $vf11, 0(%0)\n\t.set reorder" : : "r"(D_00324680));
    __asm__ volatile(".set noreorder\n\tvsub.xyzw $vf10, $vf10, $vf11\n\t.set reorder");
    __asm__ volatile(
        ".set noreorder\n\t"
        "vmul.xyz $vf2, $vf10, $vf10\n\t"
        "vmulax.w ACC, $vf0, $vf2x\n\t"
        "vmadday.w ACC, $vf0, $vf2y\n\t"
        "vmaddz.w $vf2, $vf0, $vf2z\n\t"
        "vrsqrt Q, $vf0w, $vf2w\n\t"
        "vwaitq\n\t"
        "vmulq.xyz $vf10, $vf10, Q\n\t"
        ".set reorder");
    __asm__ volatile(".set noreorder\n\tsqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(dir) : "memory");
    handle = work->handle;
    size[0] = work->scale;
    size[1] = work->scale;
    size[2] = work->scale;
    __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(size));
    __asm__ volatile(".set noreorder\n\tlqc2 $vf11, 0(%0)\n\t.set reorder" : : "r"(dir));
    __asm__ volatile(".set noreorder\n\tvmul.xyzw $vf10, $vf10, $vf11\n\t.set reorder");
    __asm__ volatile(".set noreorder\n\tlqc2 $vf11, 0(%0)\n\t.set reorder" : : "r"(work));
    __asm__ volatile(".set noreorder\n\tvadd.xyzw $vf10, $vf10, $vf11\n\t.set reorder");
    __asm__ volatile(".set noreorder\n\tsqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(pos) : "memory");
    effCopyVector(handle, pos);
    scale = work->scale;
    color = 0x10808080;
    for (i = 0; i < 10; i++) {
        func_00152000(handle, scale, scale);
        scale *= 0.975f;
        func_00152010(handle, func_0018DDF8(func_0018DDF8(color, work->color14), work->color10));
        color += 0x05000000;
        billInvokeCallback(handle);
    }
}

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

/* Re-rolls slot `index`: random-angle rotation matrix pushed to both handles
 * of the pair, then a new random countdown. */
void func_00177318(EffPCPTwinWork *work, s32 index) {
    u128 mtx[4];

    func_002DD688((func_002E8398(D_0034DF38) - 0.5f) * 2.0f * 6.283185f);
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf28, 0(%0)\n"
        "sqc2 vf29, 0x10(%0)\n"
        "sqc2 vf30, 0x20(%0)\n"
        "sqc2 vf31, 0x30(%0)\n"
        ".set reorder"
        : : "r"(mtx));
    effParamWorkCallback2(work->pair[index][0], mtx);
    effParamWorkCallback2(work->pair[index][1], mtx);
    mdlAddEntryPlain(effParamWorkGetData(work->pair[index][0]), 0, 0);
    mdlAddEntryPlain(effParamWorkGetData(work->pair[index][1]), 0, 0);
    work->counter[index] = effMiscRand(D_0034DF38) % 10;
}

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

extern void effParamWorkCallback1(u32 handle, f32 value);

/* Per-frame update of the 8 twin slots: a slot whose countdown reached zero
 * fires its handle pair; after frame 0x18 its position is pushed to the shared
 * handle. */
void func_00177700(EffPCPTwinWork *work) {
    void *obj[2];
    u128 pos;
    u128 *posp;
    s32 i;

    for (i = 0; i < 8; i++) {
        if (work->counter[i] != 0) {
            work->counter[i]--;
            continue;
        }
        obj[0] = effParamWorkGetData(work->pair[i][0]);
        obj[1] = effParamWorkGetData(work->pair[i][1]);
        __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(work));
        mdlStorePrimaryVectorVU(obj[0]);
        effParamWorkCallback1(work->pair[i][0], work->scale * 1.5f);
        effParamWorkCallback1(work->pair[i][1], 1.75f);
        mdlBroadcastMasked(obj[1], work->color);
        func_00217878(obj[0], D_00325828);
        func_002D9E98(((EffPCPWork *)obj[0])->unk18, 1);
        posp = &pos;
        __asm__ volatile (".set noreorder\nsqc2 vf10, 0(%0)\n.set reorder" : : "r"(posp));
        mdlStorePrimaryVectorVU(obj[1]);
        func_00217878(obj[1], D_00325828);
        if (work->unk14 > 0x18) {
            effParamWorkCallback0(work->shared[i], posp);
            func_00162A38(work->shared[i]);
        }
    }
    work->unk14++;
}

void func_00177850(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00177860(EffPCPWorkF18 *work, f32 val) {
    work->unk18 = val;
}

void func_00177868(EffPCPWork *work, u32 val) {
    work->unk10 = val;
}

/* Re-rolls slot `index` of the staggered effect: random-angle rotation matrix
 * for both handles, new random offset and delay. */
void func_00177870(EffPCPStaggered *work, s32 index) {
    u128 mtx[4];

    func_002DD688((func_002E8398(D_0034DF38) - 0.5f) * 2.0f * 6.283185f);
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf28, 0(%0)\n"
        "sqc2 vf29, 0x10(%0)\n"
        "sqc2 vf30, 0x20(%0)\n"
        "sqc2 vf31, 0x30(%0)\n"
        ".set reorder"
        : : "r"(mtx));
    effParamWorkCallback2(work->handle[index * 2], mtx);
    effParamWorkCallback2(work->handle[index * 2 + 1], mtx);
    mdlAddEntryPlain(effParamWorkGetData(work->handle[index * 2]), 0, 0);
    mdlAddEntryPlain(effParamWorkGetData(work->handle[index * 2 + 1]), 0, 0);
    work->offset[index] = func_002E8398(D_0034DF38) * 150.0f;
    work->delay[index] = effMiscRand(D_0034DF38) % 10;
}

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

extern void effParamWorkCallback1(u32 handle, f32 value);

void effPcpStaggerUpdate(EffPCPStaggered *work) {
    void *obj[2];
    f32 pos[4];
    s32 i;

    for (i = 0; i < 8; i++) {
        if (work->delay[i] != 0) {
            work->delay[i]--;
        } else {
            obj[0] = effParamWorkGetData(work->handle[i * 2]);
            obj[1] = effParamWorkGetData(work->handle[i * 2 + 1]);
            pos[0] = work->x;
            pos[2] = work->z;
            pos[1] = (work->y - work->offset[i] + 100.0f) * work->scale;
            __asm__ volatile (
                ".set noreorder\n"
                "lqc2 vf10, 0(%0)\n"
                ".set reorder"
                : : "r"(pos));
            mdlStorePrimaryVectorVU(obj[0]);
            effParamWorkCallback1(work->handle[i * 2], work->scale * 1.5f);
            effParamWorkCallback1(work->handle[i * 2 + 1], 1.5f);
            mdlBroadcastMasked(obj[1], work->unk10);
            func_00217878(obj[0], D_00325828);
            func_002D9E98(((EffPCPWork *)obj[0])->unk18, 1);
            mdlStorePrimaryVectorVU(obj[1]);
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

extern void func_002DD708(f32 angle);

/* Spawns cross arm (i, j): rotates the model by j sixths of a turn about the axis chosen by i. */
void func_00177CF0(EffPCPCrossWork *work, u32 i, u32 j) {
    u128 mtx[4];
    f32 angle;

    mdlAddEntryPlain(effParamWorkGetData(work->handle[i][j]), 0, 0);
    angle = (f32)j * -1.0471974f;
    if (i == 0) {
        func_002DD688(angle);
    } else if (i == 1) {
        func_002DD688(-angle);
    } else if (i == 2) {
        func_002DD708(-angle);
    } else {
        func_002DD708(angle);
    }
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf28, 0(%0)\n"
        "sqc2 vf29, 0x10(%0)\n"
        "sqc2 vf30, 0x20(%0)\n"
        "sqc2 vf31, 0x30(%0)\n"
        ".set reorder"
        : : "r"(mtx) : "memory");
    effParamWorkCallback2(work->handle[i][j], mtx);
    if ((j + 1) & 1) {
        work->state[i][j] = 0;
    } else {
        work->state[i][j] = 5;
    }
}

EffPCPCrossWork *effCrossEffectCreateFromTable(void *src) {
    EffPCPCrossWork *work = func_002CFEB8(0xDC);
    s32 i;
    s32 j;

    work->base = effParamCreateFromTable(src, 0);
    mdlAddEntryPlain(effParamWorkGetData(work->base), 0, 0);
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

/* Four groups of three handles, starting 0x10 bytes into a block at +0xC. */
typedef struct EffPCPGroupBlock {
    u8 pad00[0x10];
    u32 handle[4][3];
} EffPCPGroupBlock;

/* Destroys the cross effect: releases the main handle and all 12 group handles. */
void func_00177F90(EffPCPWork *work) {
    EffPCPGroupBlock *block = (EffPCPGroupBlock *)((u8 *)work + 0xC);
    s32 i;
    s32 j;

    func_001629F0(work->unk18);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            func_001629F0(block->handle[i][j]);
        }
    }
    func_002CFF98(work);
}


EffPCPCrossWork *effCrossEffectClone(EffPCPCrossWork *src) {
    EffPCPCrossWork *work = func_002CFEB8(0xDC);
    s32 i;
    s32 j;

    work->base = effParamWorkDuplicate(src->base);
    mdlAddEntryPlain(effParamWorkGetData(work->base), 0, 0);
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

extern void effParamWorkCallback1(u32 handle, f32 value);

/* Per-frame update of the cross effect: scales the anchor model and fires each
 * of the 12 slots whose countdown has expired. */
void func_00178130(EffPCPCrossWork *work) {
    EffPCPWork *anchor;
    void *obj;
    s32 i;
    s32 j;

    anchor = effParamWorkGetData(work->base);
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(work));
    mdlStorePrimaryVectorVU(anchor);
    effParamWorkCallback1(work->base, work->scale);
    func_00217878(anchor, D_00325828);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            if (work->state[i][j] != 0) {
                work->state[i][j]--;
                continue;
            }
            obj = effParamWorkGetData(work->handle[i][j]);
            mdlBroadcastMasked(obj, work->color);
            func_002D9E98(anchor->unk18, i * 4 + j + 1);
            mdlStorePrimaryVectorVU(obj);
            func_00217878(obj, D_00325828);
        }
    }
}

void func_00178260(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00178270(EffPCPWorkF14 *work, f32 val) {
    work->unk14 = val;
}

void func_00178278(EffPCPWork *work, u32 val) {
    work->unk10 = val;
}

/* Handle-pair block that starts 8 bytes into the work (delayed pairs). */
typedef struct EffPCPPairBlock {
    u8 pad00[0x10];
    u32 handle[12];
    u32 counter[6];
} EffPCPPairBlock;

/* Re-arms slot `index`: registers both handles of the pair and rolls a new
 * random countdown. */
void func_00178280(void *work, s32 index) {
    EffPCPPairBlock *block = (EffPCPPairBlock *)((u8 *)work + 8);

    mdlAddEntryPlain(effParamWorkGetData(block->handle[index * 2]), 0, 0);
    mdlAddEntryPlain(effParamWorkGetData(block->handle[index * 2 + 1]), 0, index & 1);
    block->counter[index] = effMiscRand(D_0034DF38) % 10;
}

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
            obj[0] = effParamWorkGetData(work->handle[i * 2]);
            obj[1] = effParamWorkGetData(work->handle[i * 2 + 1]);
            __asm__ volatile (
                ".set noreorder\n"
                "lqc2 vf10, 0(%0)\n"
                ".set reorder"
                : : "r"(work) : "memory");
            mdlStorePrimaryVectorVU(obj[0]);
            mdlBroadcastMasked(obj[1], work->unk10);
            func_00217878(obj[0], D_00325828);
            func_002D9E98(((EffPCPWork *)obj[0])->unk18, 1);
            __asm__ volatile (
                ".set noreorder\n"
                "sqc2 vf10, 0(%0)\n"
                ".set reorder"
                : : "r"(&vec));
            mdlStorePrimaryVectorVU(obj[1]);
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

void effPcpCopyVectorAF0(void *work, void *src) {
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

    obj = effParamWorkGetData(work->unk14);
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf10, 0(%0)\n"
        ".set reorder"
        : : "r"(work) : "memory");
    mdlStorePrimaryVectorVU(obj);
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

/* Shared scratch parameter block handed to the particle spawner (D_00354D40)
 * and the shared spawn origin vector at +0x10 (D_00354D50, separate symbol). */
typedef struct EffSpawnParams {
    f32 pos[4];
    f32 vel[4];
    u8 pad20[0x0C];
    f32 unk2C;
    u8 pad30[0x08];
    s16 unk38;
    u8 pad3A[0x0A];
    f32 unk44;
    u8 pad48[0x04];
    f32 unk4C;
    u8 pad50[0x04];
} EffSpawnParams;

typedef struct EffSpawnGroup {
    u8 pad00[0x1C];
    u32 handles[30];
} EffSpawnGroup;

#define EFF_DEG2RAD 0.017453292f

extern EffSpawnParams D_00354D40[];
extern u8 D_00354D50[];
extern f32 func_002E77F8(f32);
extern f32 func_002E78F8(f32);
extern void func_002DD8B8(f32 *axis, f32 angle);

/* Spawns 12 particles in a ring: every second particle advances the ring angle
 * (60 degrees). Direction is normalised on the VU, scaled per axis and offset
 * by the origin vector; short-reach particles get a shorter life. */
void func_00178CC0(EffSpawnGroup *group) {
    s32 i;
    u32 *out;
    f32 angle = 0.0f;
    f32 cosv = 0.0f;
    f32 sinv = 0.0f;
    f32 dir[4];
    f32 scale[4];
    f32 spread;
    f32 radius;
    f32 reach;
    f32 speed;
    s16 life;

    i = 0;
    out = group->handles;
    do {
        if ((i & 1) == 0) {
            sinv = func_002E78F8(angle);
            cosv = func_002E77F8(angle);
            angle += 60.0f * EFF_DEG2RAD;
        }
        spread = func_002E8398(D_0034DF38) * 0.5f + 0.5f;
        D_00354D40->unk44 = spread * 1.25f;
        D_00354D40->unk4C = spread * 12.5f;
        radius = (func_002E8398(D_0034DF38) * 0.25f + 0.75f) * 100.0f;
        D_00354D40->vel[1] = 0;
        D_00354D40->vel[0] = sinv * radius;
        D_00354D40->vel[2] = cosv * radius;
        dir[0] = sinv;
        dir[1] = (func_002E8398(D_0034DF38) - 0.5f) * 2.0f + 2.0f;
        dir[2] = cosv;
        __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(dir));
        __asm__ volatile(
            ".set noreorder\n\t"
            "vmul.xyz $vf2, $vf10, $vf10\n\t"
            "vmulax.w ACC, $vf0, $vf2x\n\t"
            "vmadday.w ACC, $vf0, $vf2y\n\t"
            "vmaddz.w $vf2, $vf0, $vf2z\n\t"
            "vrsqrt Q, $vf0w, $vf2w\n\t"
            "vwaitq\n\t"
            "vmulq.xyz $vf10, $vf10, Q\n\t"
            ".set reorder");
        __asm__ volatile(".set noreorder\n\tsqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(dir) : "memory");
        reach = (func_002E8398(D_0034DF38) * 0.65f + (1.0f - 0.65f)) * 350.0f;
        scale[2] = reach;
        scale[0] = reach;
        scale[1] = -reach;
        __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(dir));
        __asm__ volatile(".set noreorder\n\tlqc2 $vf11, 0(%0)\n\t.set reorder" : : "r"(scale));
        __asm__ volatile(".set noreorder\n\tvmul.xyzw $vf10, $vf10, $vf11\n\t.set reorder");
        __asm__ volatile(".set noreorder\n\tlqc2 $vf11, 0(%0)\n\t.set reorder" : : "r"(D_00354D50));
        __asm__ volatile(".set noreorder\n\tvadd.xyzw $vf10, $vf10, $vf11\n\t.set reorder");
        __asm__ volatile(".set noreorder\n\tsqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(D_00354D40) : "memory");
        if (scale[0] < 250.0f) {
            speed = (func_002E8398(D_0034DF38) - 0.5f) * 2.0f * 2.5f + 5.0f;
            life = effMiscRand(D_0034DF38) % 5 + 5;
        } else {
            speed = (func_002E8398(D_0034DF38) - 0.5f) * 2.0f * 5.0f + 10.0f;
            life = effMiscRand(D_0034DF38) % 6 + 15;
        }
        D_00354D40->unk38 = life;
        D_00354D40->unk2C = speed;
        *out = func_00165418(D_00354D40);
        out++;
        i++;
    } while (i < 12);
}

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

extern EffSpawnParams D_00354DA0[];

/* 12-piece spread on a cone around a random axis (rotation matrix built by
 * func_002DD8B8 into the VU0 matrix registers). */
void func_00179158(EffSpawnGroup *group) {
    s32 i;
    f32 angle = 0.0f;
    f32 sinv = 0.0f;
    f32 cosv = 0.0f;
    f32 axis[4];
    f32 spread;
    f32 radius;
    f32 height;
    f32 speed;
    s32 life;

    i = 0;
    do {
        if ((i & 1) == 0) {
            sinv = func_002E78F8(angle);
            cosv = func_002E77F8(angle);
            angle += 60.0f * EFF_DEG2RAD;
        }
        spread = func_002E8398(D_0034DF38) * 0.5f + 0.5f;
        D_00354DA0->unk44 = spread * 1.25f;
        D_00354DA0->unk4C = spread * 12.5f;
        radius = (func_002E8398(D_0034DF38) * 0.25f + 0.75f) * 100.0f;
        axis[0] = cosv;
        axis[1] = 0;
        axis[2] = -sinv;
        D_00354DA0->vel[1] = 0;
        D_00354DA0->vel[0] = sinv * radius;
        D_00354DA0->vel[2] = cosv * radius;
        func_002DD8B8(axis, -(func_002E8398(D_0034DF38) * (50.0f * EFF_DEG2RAD) + 5.0f * EFF_DEG2RAD));
        height = (func_002E8398(D_0034DF38) * 0.65f + (1.0f - 0.65f)) * 500.0f;
        D_00354DA0->pos[0] = 0;
        D_00354DA0->pos[2] = 0;
        D_00354DA0->pos[1] = -height;
        __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(D_00354DA0->pos));
        __asm__ volatile(
            ".set noreorder\n\t"
            "vmulax.xyzw ACC, $vf28, $vf10x\n\t"
            "vmadday.xyzw ACC, $vf29, $vf10y\n\t"
            "vmaddz.xyzw $vf10, $vf30, $vf10z\n\t"
            ".set reorder");
        __asm__ volatile(".set noreorder\n\tsqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(D_00354DA0->pos) : "memory");
        D_00354DA0->pos[0] += D_00354DA0->vel[0];
        D_00354DA0->pos[2] += D_00354DA0->vel[2];
        if (height < 300.0f) {
            speed = (func_002E8398(D_0034DF38) - 0.5f) * 2.0f * 2.5f + 5.0f;
            life = effMiscRand(D_0034DF38) % 5 + 5;
        } else {
            speed = (func_002E8398(D_0034DF38) - 0.5f) * 2.0f * 5.0f + 10.0f;
            life = effMiscRand(D_0034DF38) % 6 + 15;
        }
        D_00354DA0->unk38 = life;
        D_00354DA0->unk2C = speed;
        group->handles[i] = func_00165418(D_00354DA0);
        i++;
    } while (i < 12);
}

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

extern EffSpawnParams D_00354E00[];

/* 30-piece spread: pieces are placed on a cone around a random axis (rotation
 * matrix built by func_002DD8B8 into the VU0 matrix registers). */
void func_001795D0(EffSpawnGroup *group) {
    s32 i;
    f32 angle = 0.0f;
    f32 sinv = 0.0f;
    f32 cosv = 0.0f;
    f32 axis[4];
    f32 spread;
    f32 height;
    f32 speed;
    s16 life;

    i = 0;
    do {
        if ((i & 1) == 0) {
            sinv = func_002E78F8(angle);
            cosv = func_002E77F8(angle);
            angle += 24.0f * EFF_DEG2RAD;
        }
        spread = func_002E8398(D_0034DF38) * 0.5f + 0.5f;
        D_00354E00->unk44 = spread * 1.25f;
        D_00354E00->unk4C = spread * 12.5f;
        D_00354E00->vel[1] = 0;
        D_00354E00->vel[0] = sinv * ((func_002E8398(D_0034DF38) * 0.25f + 0.75f) * 500.0f);
        D_00354E00->vel[2] = cosv * ((func_002E8398(D_0034DF38) * 0.25f + 0.75f) * 250.0f);
        axis[0] = cosv;
        axis[1] = 0;
        axis[2] = -sinv;
        func_002DD8B8(axis, -(func_002E8398(D_0034DF38) * (55.0f * EFF_DEG2RAD) + 5.0f * EFF_DEG2RAD));
        height = (func_002E8398(D_0034DF38) * 0.65f + (1.0f - 0.65f)) * 500.0f;
        D_00354E00->pos[0] = 0;
        D_00354E00->pos[2] = 0;
        D_00354E00->pos[1] = -height;
        __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(D_00354E00->pos));
        __asm__ volatile(
            ".set noreorder\n\t"
            "vmulax.xyzw ACC, $vf28, $vf10x\n\t"
            "vmadday.xyzw ACC, $vf29, $vf10y\n\t"
            "vmaddz.xyzw $vf10, $vf30, $vf10z\n\t"
            ".set reorder");
        __asm__ volatile(".set noreorder\n\tsqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(D_00354E00->pos) : "memory");
        D_00354E00->pos[0] += D_00354E00->vel[0];
        D_00354E00->pos[2] += D_00354E00->vel[2];
        if (height < 300.0f) {
            speed = (func_002E8398(D_0034DF38) - 0.5f) * 2.0f * 2.5f + 5.0f;
            life = effMiscRand(D_0034DF38) % 5 + 5;
        } else {
            speed = (func_002E8398(D_0034DF38) - 0.5f) * 2.0f * 5.0f + 10.0f;
            life = effMiscRand(D_0034DF38) % 6 + 15;
        }
        D_00354E00->unk38 = life;
        D_00354E00->unk2C = speed;
        group->handles[i] = func_00165418(D_00354E00);
        i++;
    } while (i < 30);
}

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

extern EffSpawnParams D_00354E60[];

/* Same spread as func_00179158 with a wider cone and its own parameter block. */
void func_00179A68(EffSpawnGroup *group) {
    s32 i;
    f32 angle = 0.0f;
    f32 sinv = 0.0f;
    f32 cosv = 0.0f;
    f32 axis[4];
    f32 spread;
    f32 radius;
    f32 height;
    f32 speed;
    s32 life;

    i = 0;
    do {
        if ((i & 1) == 0) {
            sinv = func_002E78F8(angle);
            cosv = func_002E77F8(angle);
            angle += 60.0f * EFF_DEG2RAD;
        }
        spread = func_002E8398(D_0034DF38) * 0.5f + 0.5f;
        D_00354E60->unk44 = spread * 1.25f;
        D_00354E60->unk4C = spread * 12.5f;
        radius = (func_002E8398(D_0034DF38) * 0.25f + 0.75f) * 100.0f;
        axis[0] = cosv;
        axis[1] = 0;
        axis[2] = -sinv;
        D_00354E60->vel[1] = 0;
        D_00354E60->vel[0] = sinv * radius;
        D_00354E60->vel[2] = cosv * radius;
        func_002DD8B8(axis, -(func_002E8398(D_0034DF38) * (55.0f * EFF_DEG2RAD) + 5.0f * EFF_DEG2RAD));
        height = (func_002E8398(D_0034DF38) * 0.65f + (1.0f - 0.65f)) * 500.0f;
        D_00354E60->pos[0] = 0;
        D_00354E60->pos[2] = 0;
        D_00354E60->pos[1] = -height;
        __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(D_00354E60->pos));
        __asm__ volatile(
            ".set noreorder\n\t"
            "vmulax.xyzw ACC, $vf28, $vf10x\n\t"
            "vmadday.xyzw ACC, $vf29, $vf10y\n\t"
            "vmaddz.xyzw $vf10, $vf30, $vf10z\n\t"
            ".set reorder");
        __asm__ volatile(".set noreorder\n\tsqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(D_00354E60->pos) : "memory");
        D_00354E60->pos[0] += D_00354E60->vel[0];
        D_00354E60->pos[2] += D_00354E60->vel[2];
        if (height < 300.0f) {
            speed = (func_002E8398(D_0034DF38) - 0.5f) * 2.0f * 2.5f + 5.0f;
            life = effMiscRand(D_0034DF38) % 5 + 5;
        } else {
            speed = (func_002E8398(D_0034DF38) - 0.5f) * 2.0f * 5.0f + 10.0f;
            life = effMiscRand(D_0034DF38) % 6 + 15;
        }
        D_00354E60->unk38 = life;
        D_00354E60->unk2C = speed;
        group->handles[i] = func_00165418(D_00354E60);
        i++;
    } while (i < 12);
}

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

extern EffSpawnParams D_00354EB8[];

/* 12 pieces thrown from a fixed origin along a random angle; the param block
 * (of two) is picked at random. */
void func_00179EE0(EffSpawnGroup *group) {
    s32 i;
    EffSpawnParams *params;
    f32 angle;
    f32 dist;
    s32 life;

    for (i = 0; i < 12; i++) {
        params = &D_00354EB8[effMiscRand(D_0034DF38) & 1];
        params->unk2C = (func_002E8398(D_0034DF38) - 0.5f) * 2.0f * 5.0f + 20.0f;
        life = effMiscRand(D_0034DF38) % 6 + 15;
        params->pos[0] = -150.0f;
        params->pos[1] = -500.0f;
        params->pos[2] = 0;
        params->unk38 = life;
        angle = func_002E8398(D_0034DF38) * (3.14159265f / 2.0f) + 3.14159265f / 8.0f;
        dist = (func_002E8398(D_0034DF38) * 0.25f + 0.75f) * 600.0f;
        params->vel[0] = params->pos[0] + func_002E78F8(angle) * dist;
        params->vel[1] = params->pos[1] + func_002E77F8(angle) * dist;
        params->vel[2] = 0;
        group->handles[i] = func_00165418(params);
    }
}

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

extern void func_002DD608(f32 angle);
extern void func_002DD9E8(f32 angle);
extern void func_002DDC50(void);
extern EffSpawnParams D_00354F60[];

/* 8 pieces flung sideways from below; the rotation matrix is composed by the
 * three func_002DD* calls into the VU0 matrix registers. */
void func_0017A250(EffSpawnGroup *group) {
    s32 i;
    EffSpawnParams *params;

    for (i = 0; i < 8; i++) {
        params = &D_00354F60[effMiscRand(D_0034DF38) & 1];
        if (i & 1) {
            params->vel[0] = func_002E8398(D_0034DF38) * 500.0f;
        } else {
            params->vel[0] = func_002E8398(D_0034DF38) * -500.0f;
        }
        params->vel[1] = 0;
        params->vel[2] = (func_002E8398(D_0034DF38) - 0.5f) * 2.0f * 250.0f;
        params->pos[0] = 0;
        params->pos[1] = -600.0f;
        params->pos[2] = 0;
        func_002DD608((func_002E8398(D_0034DF38) - 0.5f) * 2.0f * (30.0f * EFF_DEG2RAD));
        func_002DD9E8((func_002E8398(D_0034DF38) - 0.5f) * 2.0f * (30.0f * EFF_DEG2RAD));
        func_002DDC50();
        __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(params->pos));
        __asm__ volatile(
            ".set noreorder\n\t"
            "vmulax.xyzw ACC, $vf28, $vf10x\n\t"
            "vmadday.xyzw ACC, $vf29, $vf10y\n\t"
            "vmaddz.xyzw $vf10, $vf30, $vf10z\n\t"
            ".set reorder");
        __asm__ volatile(".set noreorder\n\tsqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(params->pos) : "memory");
        params->pos[0] += params->vel[0];
        params->pos[2] += params->vel[2];
        group->handles[i] = func_00165418(params);
    }
}

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

typedef struct EffPCPFadeTarget {
    u8 pad00[0x1C];
    f32 unk1C;
    f32 unk20;
} EffPCPFadeTarget;

typedef struct EffPCPFadeWork {
    u8 pad00[0x14];
    u32 unk14;
    u32 frame;
    u32 handle;
} EffPCPFadeWork;

extern void func_00163518(u32 handle, u32 value);
extern void func_00163508(u32 handle, void *work);
extern void func_00163CD0(u32 handle);
extern u32 effBlendColor(u32 colorA, u32 colorB, f32 t);

/* Scales the target over the first 22 frames, fades the colour out from frame 45. */
void func_0017A698(EffPCPFadeWork *work) {
    EffPCPFadeTarget *target;
    f32 phase;
    u32 handle;

    if (work->frame < 0x17) {
        target = func_00163540(work->handle);
        phase = (f32)work->frame / 22.0f;
        target->unk1C = phase * 600.0f + 200.0f;
        target->unk20 = phase * 300.0f + 100.0f;
    }
    if (work->frame >= 0x2D) {
        phase = (f32)(work->frame - 0x2D) / 30.0f;
        handle = effBlendColor(work->unk14, 0, phase);
    } else {
        handle = work->unk14;
    }
    func_00163518(work->handle, handle);
    func_00163508(work->handle, work);
    func_00163CD0(work->handle);
    work->frame++;
}

void func_0017A7F0(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0017A800(EffPCPWorkF10 *work, f32 val) {
    work->unk10 = val;
}

void func_0017A808(EffPCPWork *work, u32 val) {
    work->unk14 = val;
}

extern EffSpawnParams D_00355060[];

/* Same spread as func_001795D0 with its own parameter block. */
void func_0017A810(EffSpawnGroup *group) {
    s32 i;
    f32 angle = 0.0f;
    f32 sinv = 0.0f;
    f32 cosv = 0.0f;
    f32 axis[4];
    f32 spread;
    f32 height;
    f32 speed;
    s16 life;

    i = 0;
    do {
        if ((i & 1) == 0) {
            sinv = func_002E78F8(angle);
            cosv = func_002E77F8(angle);
            angle += 24.0f * EFF_DEG2RAD;
        }
        spread = func_002E8398(D_0034DF38) * 0.5f + 0.5f;
        D_00355060->unk44 = spread * 1.25f;
        D_00355060->unk4C = spread * 12.5f;
        D_00355060->vel[1] = 0;
        D_00355060->vel[0] = sinv * ((func_002E8398(D_0034DF38) * 0.25f + 0.75f) * 500.0f);
        D_00355060->vel[2] = cosv * ((func_002E8398(D_0034DF38) * 0.25f + 0.75f) * 250.0f);
        axis[0] = cosv;
        axis[1] = 0;
        axis[2] = -sinv;
        func_002DD8B8(axis, -(func_002E8398(D_0034DF38) * (55.0f * EFF_DEG2RAD) + 5.0f * EFF_DEG2RAD));
        height = (func_002E8398(D_0034DF38) * 0.65f + (1.0f - 0.65f)) * 500.0f;
        D_00355060->pos[0] = 0;
        D_00355060->pos[2] = 0;
        D_00355060->pos[1] = -height;
        __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(D_00355060->pos));
        __asm__ volatile(
            ".set noreorder\n\t"
            "vmulax.xyzw ACC, $vf28, $vf10x\n\t"
            "vmadday.xyzw ACC, $vf29, $vf10y\n\t"
            "vmaddz.xyzw $vf10, $vf30, $vf10z\n\t"
            ".set reorder");
        __asm__ volatile(".set noreorder\n\tsqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(D_00355060->pos) : "memory");
        D_00355060->pos[0] += D_00355060->vel[0];
        D_00355060->pos[2] += D_00355060->vel[2];
        if (height < 300.0f) {
            speed = (func_002E8398(D_0034DF38) - 0.5f) * 2.0f * 2.5f + 5.0f;
            life = effMiscRand(D_0034DF38) % 5 + 5;
        } else {
            speed = (func_002E8398(D_0034DF38) - 0.5f) * 2.0f * 5.0f + 10.0f;
            life = effMiscRand(D_0034DF38) % 6 + 15;
        }
        D_00355060->unk38 = life;
        D_00355060->unk2C = speed;
        group->handles[i] = func_00165418(D_00355060);
        i++;
    } while (i < 30);
}

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

/* Same fade with a longer scale-up; stops updating once frame 65 is reached. */
void func_0017AD80(EffPCPFadeWork *work) {
    EffPCPFadeTarget *target;
    f32 phase;
    f32 value;
    u32 handle;
    u32 frame;

    frame = work->frame;
    if (frame == 0x41) {
        return;
    }
    if (frame < 0x1E) {
        target = func_00163540(work->handle);
        phase = (f32)frame / 30.0f;
        value = phase * 200.0f + 150.0f;
        target->unk1C = value;
        target->unk20 = value;
    }
    if (frame >= 0x39) {
        phase = ((f32)frame - 57.0f) * 0.125f;
        handle = effBlendColor(work->unk14, 0, phase);
        func_00163518(work->handle, handle);
    } else {
        func_00163518(work->handle, work->unk14);
    }
    func_00163508(work->handle, work);
    func_00163CD0(work->handle);
    work->frame++;
}

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

typedef struct EffPCPTrailObj {
    s32 size;
    u32 color;
    u8 pad08[0x0C];
    s32 x;
    s32 y;
} EffPCPTrailObj;

typedef struct EffPCPTrailWork {
    f32 pos[4];
    u32 flags;
    u32 color;
    u32 frame;
    f32 unk1C;
    u32 count;
    u32 colors[2];
    f32 unk2C;
    s32 minSize;
    EffPCPTrailObj *obj;
} EffPCPTrailWork;

extern s32 func_0018DC58(f32 value);
extern u32 func_0018DDF8(u32 flags, u32 color);
extern void func_00186CD0(EffPCPTrailObj *obj);

/* Shared-work variant of the trail update: every reference counts frames in its
 * own word and only the reference that is in step with the shared frame
 * counter advances the shared work. */
typedef struct EffPCPSharedTrail {
    f32 pos[4];
    u32 flags;
    u32 color;
    u32 frame;
    f32 unk1C;
    u32 limit;
    u32 unk24;
    u32 colors[2];
    f32 scale;
    s32 minSize;
    EffPCPTrailObj *obj;
} EffPCPSharedTrail;

#define EFF_SHARED_TRAIL ((EffPCPSharedTrail *)D_003BD7FC)

void func_0017B008(ref)
    u32 *ref;
{
    EffPCPSharedTrail *work;
    EffPCPTrailObj *obj;
    f32 pos[4];

    if (*ref == 0) {
        EFF_SHARED_TRAIL->frame &= 1;
        if (EFF_SHARED_TRAIL->frame != 0) {
            EFF_SHARED_TRAIL->limit = EFF_SHARED_TRAIL->unk24 + 1;
        }
        *ref = EFF_SHARED_TRAIL->frame;
    }
    work = EFF_SHARED_TRAIL;
    if (*ref != work->frame) {
        *ref = work->frame;
        return;
    }
    obj = work->obj;
    *ref = *ref + 1;
    if (work->frame < work->limit) {
        work->color = work->colors[work->frame & 1];
        __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(work));
        obj->size = (s32)((f32)func_0018DC58(EFF_SHARED_TRAIL->unk1C) * EFF_SHARED_TRAIL->scale);
        __asm__ volatile(".set noreorder\n\tsqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(pos) : "memory");
        obj->x = (s32)pos[0] - 0x800;
        obj->y = ((s32)pos[1] - 0x800) << 1;
        if (obj->size < EFF_SHARED_TRAIL->minSize) {
            obj->size = EFF_SHARED_TRAIL->minSize;
        }
        obj->color = func_0018DDF8(EFF_SHARED_TRAIL->flags, EFF_SHARED_TRAIL->color);
        func_00186CD0(obj);
        EFF_SHARED_TRAIL->frame++;
    }
}

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

/* Per-frame update: places the trail object from the work position and
 * alternates its colour between two entries until the count runs out. */
void func_0017B248(EffPCPTrailWork *work) {
    EffPCPTrailObj *obj;
    f32 pos[4];

    obj = work->obj;
    __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(work->pos));
    obj->size = (s32)((f32)func_0018DC58(work->unk1C) * work->unk2C);
    __asm__ volatile(".set noreorder\n\tsqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(pos) : "memory");
    obj->x = (s32)pos[0] - 0x800;
    obj->y = ((s32)pos[1] - 0x800) << 1;
    if (obj->size < work->minSize) {
        obj->size = work->minSize;
    }
    if (work->frame < work->count) {
        work->color = work->colors[work->frame & 1];
    } else {
        work->color = work->colors[0];
    }
    work->frame++;
    obj->color = func_0018DDF8(work->flags, work->color);
    func_00186CD0(obj);
}

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

typedef struct EffPCPFadeTimer {
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
    u32 color;
    u8 pad10[4];
    u32 unk14;
    u32 unk18;
    u32 unk1C;
    u32 unk20;
    u32 colorFrom;
    u32 colorTo;
    s32 frame;
} EffPCPFadeTimer;

extern void func_00187C08(u32 *color);

/* Timeline update: blend factor ramps up over fadeIn frames, holds at 1.0, then
 * ramps down over the last fadeOut frames. */
void func_0017BAC8(EffPCPFadeTimer *work) {
    s32 frame = work->frame;
    s32 duration = work->duration;
    s32 fadeIn;
    s32 fadeOut;
    f32 t;

    if (duration < frame) {
        return;
    }
    fadeIn = work->fadeIn;
    work->unk14 = 0;
    work->unk18 = 0;
    work->unk1C = 0x200;
    work->unk20 = 0x1C0;
    fadeOut = work->fadeOut;
    if (frame < fadeIn && fadeIn != 0) {
        t = (f32)frame / (f32)fadeIn;
    } else if (duration - frame <= fadeOut && fadeOut != 0) {
        t = (f32)(duration - frame) / (f32)fadeOut;
    } else {
        t = 1.0f;
    }
    work->color = func_0018DDF8(effBlendColor(work->colorFrom & 0xFFFFFF, work->colorFrom, t), work->colorTo);
    func_00187C08(&work->color);
    work->frame++;
}

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

typedef struct EffPCPFadeTimerLong {
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
    u32 color;
    u8 pad10[0x0C];
    u32 unk1C;
    u32 unk20;
    u32 unk24;
    u32 unk28;
    u32 unk2C;
    u32 unk30;
    u32 colorFrom;
    u32 colorTo;
    s32 frame;
} EffPCPFadeTimerLong;

extern void func_00186498(u32 *color);

/* Same timeline as func_0017BAC8 on the longer work layout. */
void func_0017BCE0(EffPCPFadeTimerLong *work) {
    s32 frame = work->frame;
    s32 duration = work->duration;
    s32 fadeIn;
    s32 fadeOut;
    f32 t;

    if (duration < frame) {
        return;
    }
    fadeIn = work->fadeIn;
    work->unk1C = 0;
    work->unk20 = 0;
    work->unk24 = 0;
    work->unk28 = 0;
    work->unk2C = 0x200;
    work->unk30 = 0x1C0;
    fadeOut = work->fadeOut;
    if (frame < fadeIn && fadeIn != 0) {
        t = (f32)frame / (f32)fadeIn;
    } else if (duration - frame <= fadeOut && fadeOut != 0) {
        t = (f32)(duration - frame) / (f32)fadeOut;
    } else {
        t = 1.0f;
    }
    work->color = func_0018DDF8(effBlendColor(work->colorFrom & 0xFFFFFF, work->colorFrom, t), work->colorTo);
    func_00186498(&work->color);
    work->frame++;
}

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

void func_0017C5E0(EffPCPFadeWork *work) {
    EffPCPFadeTarget *target;
    f32 phase;
    f32 value;
    u32 handle;

    if (work->frame < 0x1F) {
        target = func_00163540(work->handle);
        phase = (f32)work->frame / 30.0f;
        value = phase * 150.0f + 100.0f;
        target->unk1C = value;
        target->unk20 = value;
    } else if (work->frame - 0x2D < 0x10) {
        target = func_00163540(work->handle);
        phase = (f32)(work->frame - 0x2D) / 15.0f;
        value = phase * 150.0f + 250.0f;
        target->unk1C = value;
        target->unk20 = value;
    }
    if (work->frame >= 0x2D) {
        phase = (f32)(work->frame - 0x2D) / 35.0f;
        handle = effBlendColor(work->unk14, 0, phase);
    } else {
        handle = work->unk14;
    }
    func_00163518(work->handle, handle);
    func_00163508(work->handle, work);
    func_00163CD0(work->handle);
    work->frame++;
}

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
    handle = work->optionalHandle;
    if (handle != 0) {
        child->optionalHandle = func_0014FEB0(handle);
    }
    return child;
}

void effPcpReleaseOptionalHandle(EffPCPWork *work) {
    s32 handle;

    handle = work->optionalHandle;
    if (handle != 0) {
        effDestroyNode(handle);
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
        effDestroyNode(handle[14]);
        effDestroyNode(handle[7]);
        effDestroyNode(handle[0]);
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

typedef struct EffPCPBlock50 {
    u32 word[20];
} EffPCPBlock50;

/* Clone of the 0x10C block-set work: zeroed, the 0x50-byte parameter block at
 * +0x60 copied from `src`, grey colour, unit matrix, and a back pointer. */
typedef struct EffPCPBlockCloneWork {
    f32 matrix[16];
    u8 pad40[0x20];
    EffPCPBlock50 params;
    u32 unkB0;
    u8 padB4[4];
    u32 color;
    u32 mode;
    u8 padC0[0x48];
    void *source;
} EffPCPBlockCloneWork;

extern void *memset(void *s, int c, u32 n);

extern void func_0017D8A8(void *dst, void *src);

/* Clone of a block-set work that shares the source's blocks (no back pointer). */
EffPCPBlockCloneWork *func_0017DA58(EffPCPBlockCloneWork *src) {
    EffPCPBlockCloneWork *work;

    work = func_002CFEB8(0x10C);
    memset(work, 0, 0x10C);
    work->params = src->params;
    work->unkB0 = 0;
    work->color = 0x80808080;
    work->mode = 0;
    EE_MMI_UNIT_MATRIX(work->matrix);
    func_0017D8A8(work, src);
    work->source = NULL;
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017DBB0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017DCF8);

void effPcpCopyVector60(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0x60, src);
}

void func_0017E4C0(EffPCPWork *work, u32 val) {
    work->unkB8 = val;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void func_0017E4C8(void *dst, void *src) {
    VU0_COPY_MATRIX(dst, src);
}

void func_0017E4F0(void) {
    EffPCPWork *work;

    work = effPcpBuildBlockSet();
    work->mode = 1;
}

EffPCPBlockCloneWork *func_0017E518(EffPCPBlockCloneWork *src) {
    EffPCPBlockCloneWork *work;

    work = func_002CFEB8(0x10C);
    memset(work, 0, 0x10C);
    work->params = src->params;
    work->color = 0x80808080;
    work->mode = 1;
    work->unkB0 = 0;
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->source = src;
    return work;
}

void func_0017E668(void) {
    EffPCPWork *work;

    work = effPcpBuildBlockSet();
    work->mode = 2;
}

EffPCPBlockCloneWork *func_0017E690(EffPCPBlockCloneWork *src) {
    EffPCPBlockCloneWork *work;

    work = func_002CFEB8(0x10C);
    memset(work, 0, 0x10C);
    work->params = src->params;
    work->color = 0x80808080;
    work->mode = 2;
    work->unkB0 = 0;
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->source = src;
    return work;
}

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

typedef struct EffPCPBlockSet18 {
    void *head[3];
    void *pad0C;
    void *group[5];
    void *mid[3];
    void *tail[5];
    void *last;
} EffPCPBlockSet18;

/* Gathers 17 parameter blocks from the effect table (the first goes to the
 * rotate effect as its source, the rest into an on-stack block set). */
void func_0017E978(args)
    void *args;
{
    EffPCPBlockSet18 set;
    void **group;
    void *first;
    s32 idx;
    u32 i;
    u32 j;

    idx = 1;
    first = effParamTableGetBlock(args, 0);
    for (i = 0; i < 3; i++) {
        set.head[i] = effParamTableGetBlock(args, idx++);
    }
    group = set.group;
    for (j = 0; j < 5; j++) {
        group[j] = effParamTableGetBlock(args, idx++);
    }
    for (j = 0; j < 3; j++) {
        set.mid[j] = effParamTableGetBlock(args, idx++);
    }
    for (j = 0; j < 5; j++) {
        group[j + 8] = effParamTableGetBlock(args, idx++);
    }
    set.last = effParamTableGetBlock(args, idx);
    effPcpRotateCreate(first, &set);
}

/* Clones the rotate work: block copy of the head plus one fresh block-clone work
 * per id, each initialised from the source's sub-work. */
EffPCPRotateWork *func_0017EAB0(EffPCPRotateWork *src) {
    EffPCPRotateWork *work;
    EffPCPBlockCloneWork *sub;
    u32 i;

    work = func_002CFEB8(0x10C);
    *(EffPCPBlock252 *)work = *(EffPCPBlock252 *)src;
    work->count = 0;
    for (i = 0; i < 3; i++) {
        work->ids[i] = (s32)func_002CFEB8(0x10C);
        memset((void *)work->ids[i], 0, 0x10C);
        ((EffPCPBlockCloneWork *)work->ids[i])->params = ((EffPCPBlockCloneWork *)src->ids[i])->params;
        sub = (EffPCPBlockCloneWork *)work->ids[i];
        sub->unkB0 = 0;
        sub->color = 0x80808080;
        sub->mode = 0;
        EE_MMI_UNIT_MATRIX(sub->matrix);
        sub->source = (void *)src->ids[i];
    }
    return work;
}

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
        effPcpCopyVector60((void *)id[i], src);
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

extern u32 effMiscRand(void *state);

EffPCPSpinWork *func_0017EF30(void *src) {
    EffPCPSpinWork *work = func_002CFEB8(0x64);

    work->frame = 0;
    work->scale = 1.0f;
    work->color = 0x80808080;
    work->handle0 = (void *)effParamCreateFromTable(src, 0);
    EE_MMI_UNIT_MATRIX(work->matrix);
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
    EE_MMI_UNIT_MATRIX(work->matrix);
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

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effCopyMatrix(EffPCPWork *work, void *src) {
    VU0_LOAD_MATRIX(src);
    VU0_STORE_MATRIX(&work->unk10);
}

EffPCPSpinWork *effSpinEffectCreateFromTable(void *src) {
    EffPCPSpinWork *work = func_002CFEB8(0x68);

    work->frame = 0;
    work->scale = 1.0f;
    work->color = 0x80808080;
    work->handle0 = (void *)effParamCreateFromTable(src, 0);
    work->handle1 = (void *)effParamCreateFromTable(src, 1);
    EE_MMI_UNIT_MATRIX(work->matrix);
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
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->angle = (func_002E8398(D_0034DF38) - 0.5f) * 2.0f * 0.87266457f;
    return work;
}

void func_0017F388(EffPCPWork *work) {
    func_001629F0(work->unk64);
    func_001629F0(work->unk60);
    func_002CFF98(work);
}

/* Per-frame update of a two-handle spin effect: pushes the work state to both
 * handles, rebuilds the rotation matrix from the angle and hands it over. The
 * VU0 blocks are bare asm (no "memory" clobber), as the original macros were. */
void func_0017F3C0(EffPCPSpinWork *work) {
    u32 handle[2];
    u128 mtx[4];

    handle[0] = (u32)work->handle0;
    handle[1] = (u32)work->handle1;
    effParamWorkCallback0(handle[0], work);
    effParamWorkCallback0(handle[1], work);
    effParamWorkCallback1(handle[0], work->scale);
    effParamWorkCallback1(handle[1], work->scale);
    effParamWorkCallback3(handle[0], work->color);
    effParamWorkCallback3(handle[1], work->color);
    func_002DD688(work->angle);
    __asm__ volatile (".set noreorder\nlqc2 vf24, 0(%0)\nlqc2 vf25, 0x10(%0)\nlqc2 vf26, 0x20(%0)\nlqc2 vf27, 0x30(%0)\n.set reorder" : : "r"(work->matrix));
    func_002DDBF8();
    __asm__ volatile (".set noreorder\nsqc2 vf28, 0(%0)\nsqc2 vf29, 0x10(%0)\nsqc2 vf30, 0x20(%0)\nsqc2 vf31, 0x30(%0)\n.set reorder" : : "r"(mtx));
    effParamWorkCallback2(handle[0], mtx);
    effParamWorkCallback2(handle[1], mtx);
    func_00162A38(handle[0]);
    if (work->frame >= 0x1F) {
        func_00162A38(handle[1]);
    }
    work->frame++;
}

void func_0017F4C0(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0017F4D0(EffPCPWork *work, u32 val) {
    work->unk5C = val;
}

void func_0017F4D8(EffPCPWork *work, f32 val) {
    work->unk58 = val;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void func_0017F4E0(EffPCPWork *work, void *src) {
    VU0_LOAD_MATRIX(src);
    VU0_STORE_MATRIX(&work->unk10);
}

EffPCPSpinWork *func_0017F510(void *src) {
    EffPCPSpinWork *work = func_002CFEB8(0x68);

    work->frame = 0;
    work->scale = 1.0f;
    work->color = 0x80808080;
    work->handle0 = (void *)effParamCreateFromTable(src, 0);
    work->handle1 = (void *)effParamCreateFromTable(src, 1);
    EE_MMI_UNIT_MATRIX(work->matrix);
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
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->angle = (func_002E8398(D_0034DF38) - 0.5f) * 2.0f * 0.87266457f;
    return work;
}

void func_0017F688(EffPCPWork *work) {
    func_001629F0(work->unk64);
    func_001629F0(work->unk60);
    func_002CFF98(work);
}

/* Identical update for the second spin effect. */
void func_0017F6C0(EffPCPSpinWork *work) {
    u32 handle[2];
    u128 mtx[4];

    handle[0] = (u32)work->handle0;
    handle[1] = (u32)work->handle1;
    effParamWorkCallback0(handle[0], work);
    effParamWorkCallback0(handle[1], work);
    effParamWorkCallback1(handle[0], work->scale);
    effParamWorkCallback1(handle[1], work->scale);
    effParamWorkCallback3(handle[0], work->color);
    effParamWorkCallback3(handle[1], work->color);
    func_002DD688(work->angle);
    __asm__ volatile (".set noreorder\nlqc2 vf24, 0(%0)\nlqc2 vf25, 0x10(%0)\nlqc2 vf26, 0x20(%0)\nlqc2 vf27, 0x30(%0)\n.set reorder" : : "r"(work->matrix));
    func_002DDBF8();
    __asm__ volatile (".set noreorder\nsqc2 vf28, 0(%0)\nsqc2 vf29, 0x10(%0)\nsqc2 vf30, 0x20(%0)\nsqc2 vf31, 0x30(%0)\n.set reorder" : : "r"(mtx));
    effParamWorkCallback2(handle[0], mtx);
    effParamWorkCallback2(handle[1], mtx);
    func_00162A38(handle[0]);
    if (work->frame >= 0x1F) {
        func_00162A38(handle[1]);
    }
    work->frame++;
}

void func_0017F7C0(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0017F7D0(EffPCPWork *work, u32 val) {
    work->unk5C = val;
}

void func_0017F7D8(EffPCPWork *work, f32 val) {
    work->unk58 = val;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void func_0017F7E0(EffPCPWork *work, void *src) {
    VU0_LOAD_MATRIX(src);
    VU0_STORE_MATRIX(&work->unk10);
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

void effPcpReleaseNestedWork(EffPCPWork *work) {
    sdfQueueAssetRelease(work->unkA8);
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

void effPcpReleaseBeamClone(EffPCPWork *work) {
    effPcpReleaseNestedWork((EffPCPWork *)work->unk5C);
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

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effPcpCopyNestedMatrix(EffPCPWork *work, void *src) {
    VU0_LOAD_MATRIX(src);
    VU0_STORE_MATRIX(&((EffPCPWork *)work->unk5C)->pad3C[4]);
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
    effPcpReleaseNestedWork((EffPCPWork *)work->unk7C);
    func_002CFF98(work);
}

typedef struct EffPCPBeamNode {
    u8 pad00[0x80];
    u128 vec80;
    u8 pad90[8];
    u32 color98;
} EffPCPBeamNode;

typedef struct EffPCPBeamTimer {
    u128 vec;
    u8 pad10[0x14];
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
    u8 pad30[8];
    f32 decay;
    u8 pad3C[0x20];
    s32 frame;
    u32 color;
    u8 pad64[0x10];
    f32 angle;
    f32 speed;
    EffPCPBeamNode *beam;
} EffPCPBeamTimer;

extern void func_00180370(EffPCPBeamNode *beam);

/* Beam update: spins the angle, fades the colour in and out over the timeline,
 * and mirrors the work position/colour into the beam node. */
void func_00181088(EffPCPBeamTimer *work) {
    s32 frame = work->frame;
    s32 duration = work->duration;
    s32 fadeIn = work->fadeIn;
    s32 fadeOut = work->fadeOut;
    f32 t;
    u32 color;
    EffPCPBeamNode *beam;

    if (duration < frame) {
        return;
    }
    work->angle += work->speed;
    func_00180BD0(work->angle);
    work->speed *= work->decay;
    t = 1.0f;
    if (frame < fadeIn && fadeIn != 0) {
        t = (f32)frame / (f32)fadeIn;
    } else if (duration - frame <= fadeOut && fadeOut != 0) {
        t = (f32)(duration - frame) / (f32)fadeOut;
    }
    color = effBlendColor(work->color & 0xFFFFFF, work->color, t);
    beam = work->beam;
    beam->color98 = color;
    PCP_COPY_VECTOR(&beam->vec80, work);
    func_00180370(beam);
    work->frame++;
}

void func_001811B0(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpLinkedWorkSetFloat(EffPCPWork *work, f32 value) {
    ((EffPCPWork *)work->unk7C)->unk90 = value;
}

void func_001811D0(EffPCPWork *work, u32 val) {
    work->unk60 = val;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void func_001811D8(EffPCPWork *work, void *src) {
    VU0_LOAD_MATRIX(src);
    VU0_STORE_MATRIX(&((EffPCPWork *)work->unk7C)->pad3C[4]);
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

typedef struct EffPCPSlot20 {
    u8 pad00[0xC];
    f32 phase;
    f32 speed;
    u32 unk14;
    f32 spin;
    u8 pad1C[4];
} EffPCPSlot20;

typedef struct EffPCPSlotWork {
    u8 pad00[0x18];
    u32 count;
    u8 pad1C[0xC];
    f32 baseSpeed;
    f32 spreadA;
    f32 baseSpin;
    f32 spreadB;
    u8 pad38[0x54];
    EffPCPSlot20 *slots;
} EffPCPSlotWork;

/* Randomises slot `index`: phase spread evenly around a full turn, jittered
 * speed, and a spin whose sign is picked at random. */
void func_00182FC8(EffPCPSlotWork *work, s32 index) {
    EffPCPSlot20 *slot;
    f32 spread;

    slot = &work->slots[index];
    slot->phase = (3.14159265f * 2.0f) / work->count * index;
    spread = work->spreadA;
    slot->speed = work->baseSpeed * (func_002E8398(D_0034DF38) * spread + (1.0f - spread));
    spread = work->spreadB;
    slot->unk14 = 0;
    if (effMiscRand(D_0034DF38) & 1) {
        slot->spin = work->baseSpin * (func_002E8398(D_0034DF38) * spread + (1.0f - spread));
    } else {
        slot->spin = -work->baseSpin * (func_002E8398(D_0034DF38) * spread + (1.0f - spread));
    }
}

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

