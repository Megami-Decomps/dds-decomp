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
    f32 scale;       /* 0x14 effect scale, initialised to 1.0 */
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
    f32 scale;       /* 0x18 effect scale for the ring and twin effects */
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

/* Compact 0x1C effect work built by effPcpSpawnOnceCreate/effPcpSpawnOnceClone: cleared
 * header words, a grey colour and two resource handles released on destroy.
 */
typedef struct {
    u32 unk00;    /* 0x00 cleared on init */
    u32 unk04;    /* 0x04 cleared on init */
    u32 unk08;    /* 0x08 cleared on init */
    u8 pad0C[0x4]; /* 0x0C */
    u32 color10;  /* 0x10 initialised to grey 0x80808080 */
    u32 primaryHandle;   /* 0x14 parameter block 0 */
    u32 secondaryHandle; /* 0x18 parameter block 1 */
} EffPCPWork1C;

/* Shared 0x10-byte task prefix, zeroed when paired resource work is created. */
typedef struct EffPCPTaskHeader {
    u32 word00;
    u32 word04;
    u32 word08;
    u8 pad0C[4];
} EffPCPTaskHeader;

/* Four groups of duplicated handles share a count and two allocation handles. */
typedef struct EffPCPBatchWork {
    u8 pad00[0x58];
    u32 count;            /* 0x58: handles per group */
    u8 pad5C[0x30];
    u8 activeGroups[4];  /* 0x8C */
    u8 pad90[0xD4];
    u32 *entries;         /* 0x164: six-word records */
    u8 pad168[0xC];
    u32 *duplicates;      /* 0x174: four groups of handles */
    u32 duplicateHandle;  /* 0x178 */
    u32 workHandle;       /* 0x17C */
} EffPCPBatchWork;

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
    u32 color;         /* 0x1334: configurable colour */
    f32 scale;         /* 0x1338: configurable scale */
    u32 unk133C;       /* 0x133C cleared on init */
    u32 unk1340;       /* 0x1340 cleared on init */
    u32 color1344;     /* 0x1344 initialised to grey 0x80808080 */
    u32 secondaryHandle; /* 0x1348: parameter block 1 */
    u32 primaryHandle;   /* 0x134C: parameter block 0 */
    u32 allocationHandle; /* 0x1350: backing allocation */
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
extern void sdfReleaseChipBlock(void *ptr);
extern s8 D_003BB04C;
extern s32 D_003BB048;

extern EffPCPWork *effPcpBuildBlockSet();

/* Block `index` of a packed effect parameter set: data + offset table entry. */
extern void *effParamTableGetBlock(void *data, s32 index);

extern EffPCPWork *D_003BD7FC;

extern void *func_002CFEB8(s32 size);

typedef struct EffPCPSpanHead {
    u32 word[4];
    s32 left;
    s32 right;
    s32 width;
    f32 scale;
} EffPCPSpanHead;

typedef struct EffPCPSpanParams {
    EffPCPSpanHead head;
    u32 unk20;
} EffPCPSpanParams;

typedef struct EffPCPSpanWork {
    u128 matrix[4];
    EffPCPSpanParams params;
    u32 color;
    u32 unk68;
    f32 startAngle;  /* 0x6C: negative half angular span */
    f32 angleStep;   /* 0x70: angular span divided by the number of steps */
    u32 optionalHandle;
} EffPCPSpanWork;

extern EffPCPSpanWork *effPcpSpanCreate(void *params, void *handleParams);
extern u32 func_0014FD20(u32 param);
extern u32 func_0014FEB0(u32 handle);
extern void effDestroyNode(s32 handle);
extern void func_00186CB8(u32 handle);
extern u32 effCloneBlurWorkWithSlots(void *params);
extern u32 func_00186F90(void *params);
extern u32 effCloneResourceTemplate(void *params);
extern u32 effCloneBlurTemplate(void *params);
extern void *effPcpTripleHandleCreate(void *block0, u32 *blocks);

extern void *func_002D03F8(s32 size);
extern void *sdfResourceRetainAddress(void *resource);
extern u32 func_001632E0(void *params);
extern u32 effParamCreateFromTable(void *data, s32 index);
extern void effPCPThunderFree3(u32 handle);
extern u32 effThunderFragCreate(void *params);
extern void func_001629F0(u32 handle);
extern u32 effParamWorkDuplicate(u32 param);
typedef struct {
    f32 x;
    f32 y;
    f32 z;
    u8 pad0C[4];
    u32 color;       /* 0x10 sent to the paired object's colour callback */
    f32 scale;
    f32 offset[8];
    u32 handle[16];
    u32 delay[8];
} EffPCPStaggered;

extern void effPcpStaggerRerollSlot(EffPCPStaggered *work, s32 index);
extern void effPcpDelayedPairsRerollSlot(void *work, s32 index);
/* The 0x10C block-set work released by effPcpBlockSetWorkRelease. */
typedef struct EffPCPBlockSetWork {
    u8 pad00[0x7C];
    u32 groupSize[3];
    u8 pad88[0x2C];
    u32 count;
    u8 padB8[8];
    u32 headHandle;
    u32 handleA[5];
    u32 *list[3];
    u32 handleB[5];
    u32 tailHandle;
    u32 alloc[3];
    u32 shared;
} EffPCPBlockSetWork;

extern void effPcpBlockSetWorkRelease(EffPCPBlockSetWork *work);
extern void effPcpCopyVector60(void *dst, void *src);
extern void effPcpCopyBlockMatrix(void *dst, void *src);
extern void sdfComposeVuMatrixFromRegisters(void);
extern void func_00180540(f32 value);
extern void func_002DD688(f32 scale);
extern void func_002DD968(f32 angle);
extern void func_002DD8E8(f32 angle);
extern void func_00180BD0(void *work, f32 radius);

extern void *effParamWorkGetData(u32 handle);
extern void effParamWorkInvokeCallback(u32 handle);
extern void func_00217878(void *obj, void *table);
extern void mdlStorePrimaryVectorVU(void *obj);
extern void sdfLoadMapRecordPositionVector(u32 handle, s32 value);
extern void effParamWorkCallback0(u32 handle, void *vec);
extern void effParamWorkCallback3(u32 handle, u32 value);
extern u8 D_00325828[];
extern void mdlBroadcastMasked(void *obj, u32 mask);
extern void *func_00163540(u32 handle);
extern u8 *func_00165638(u32 handle);
extern void func_00165D80(u32 handle);
extern void effPCPThunderSetParam58(u32 handle, u32 value);
extern s32 func_001619E8();
extern void *effBTLFieldColorGetVariantSelector(void);
extern void btlUnitGetMuzzlePosVU(void *unit);
extern u32 sdfCountMapPositionRecords(void *model);
extern u32 func_00190130(EffPCPEventOwner *owner, s32 kind, void *place);


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

void effPcpDispatchKindAndRelease(EffPCPWork *work) {
    billDispatchByKind(work->unk1C);
    sdfReleaseChipBlock(work);
}

extern u8 D_00324680[];
extern u8 D_00324690[];
extern void effCopyVector(s32 handle, f32 *src);
extern void billInvokeCallback(s32 handle);
extern void billSetChildScaleComponents(s32 handle, f32 sx, f32 sy);
extern void billSetChildParameter(s32 handle, u32 color);
extern u32 effMultiplyPackedColors(u32 flags, u32 color);

typedef struct EffPCPRingWork {
    f32 pos[4];
    u32 color10;
    u32 color14;
    f32 scale;
    s32 handle;
} EffPCPRingWork;

/* Places a ring of 10 shrinking, brightening copies of the handle along the
 * fixed view direction. */
void effPcpDrawViewAlignedRing(EffPCPRingWork *work) {
    f32 pos[4];
    f32 dir[4];
    f32 size[4];
    s32 handle;
    f32 scale;
    u32 color;
    s32 i;

    VU0_LOAD_VF($vf10, D_00324690);
    VU0_LOAD_VF($vf11, D_00324680);
    VU0_SUB(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF($vf10, dir);
    handle = work->handle;
    size[0] = work->scale;
    size[1] = work->scale;
    size[2] = work->scale;
    VU0_LOAD_VF($vf10, size);
    VU0_LOAD_VF($vf11, dir);
    VU0_MUL(vf10, vf10, vf11);
    VU0_LOAD_VF($vf11, work);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF($vf10, pos);
    effCopyVector(handle, pos);
    scale = work->scale;
    color = 0x10808080;
    for (i = 0; i < 10; i++) {
        billSetChildScaleComponents(handle, scale, scale);
        scale *= 0.975f;
        billSetChildParameter(handle, effMultiplyPackedColors(effMultiplyPackedColors(color, work->color14), work->color10));
        color += 0x05000000;
        billInvokeCallback(handle);
    }
}

void effPcpViewAlignedRingSetPosition(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00177308(EffPCPWork *work, u32 val) {
    work->unk14 = val;
}

void effPcpViewAlignedRingSetScale(EffPCPWorkF18 *work, f32 val) {
    work->scale = val;
}

extern f32 effMiscRandUnitFloat(void *state);
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
    u32 frame;          /* 0x14: shared callback starts after frame 24 */
    f32 scale;          /* 0x18 */
    u32 pair[8][2];     /* 0x1C parameter handle pairs (source uses [0] and [1]) */
    u32 shared[8];      /* 0x5C */
    u32 state[8];       /* 0x7C */
    u32 counter[8];     /* 0x9C */
} EffPCPTwinWork; /* 0xBC */

/* Re-rolls slot `index`: random-angle rotation matrix pushed to both handles
 * of the pair, then a new random countdown. */
void effTwinEffectRerollSlot(EffPCPTwinWork *work, s32 index) {
    u128 mtx[4];

    func_002DD688((effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f * 6.283185f);
    VU0_STORE_MATRIX_NOCLOBBER(mtx);
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
        effTwinEffectRerollSlot(work, i);
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
    work->frame = 0;
    return work;
}

void effTwinEffectRelease(EffPCPWork *work) {
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
    sdfReleaseChipBlock(work);
}


EffPCPTwinWork *effTwinEffectClone(EffPCPTwinWork *src) {
    EffPCPTwinWork *work = func_002CFEB8(0xBC);
    s32 i;

    for (i = 0; i < 8; i++) {
        work->pair[i][0] = effParamWorkDuplicate(src->pair[i & 1][0]);
        work->pair[i][1] = effParamWorkDuplicate(src->pair[i & 1][1]);
        work->shared[i] = effParamWorkDuplicate(src->shared[0]);
        work->state[i] = 0;
        effTwinEffectRerollSlot(work, i);
    }
    work->unk00 = 0;
    work->color = 0x80808080;
    work->scale = 1.0f;
    work->unk04 = 0;
    work->unk08 = 0;
    work->frame = 0;
    return work;
}

extern void effParamWorkCallback1(u32 handle, f32 value);

/* Per-frame update of the 8 twin slots: a slot whose countdown reached zero
 * fires its handle pair; after frame 0x18 its position is pushed to the shared
 * handle. */
void effTwinEffectUpdate(EffPCPTwinWork *work) {
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
        VU0_LOAD_VF(vf10, work);
        mdlStorePrimaryVectorVU(obj[0]);
        effParamWorkCallback1(work->pair[i][0], work->scale * 1.5f);
        effParamWorkCallback1(work->pair[i][1], 1.75f);
        mdlBroadcastMasked(obj[1], work->color);
        func_00217878(obj[0], D_00325828);
        sdfLoadMapRecordPositionVector(((EffPCPWork *)obj[0])->unk18, 1);
        posp = &pos;
        VU0_STORE_VF_UNCLOBBERED(vf10, posp);
        mdlStorePrimaryVectorVU(obj[1]);
        func_00217878(obj[1], D_00325828);
        if (work->frame > 0x18) {
            effParamWorkCallback0(work->shared[i], posp);
            effParamWorkInvokeCallback(work->shared[i]);
        }
    }
    work->frame++;
}

void effPcpCopyTwinVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetTwinEffectScale(EffPCPWorkF18 *work, f32 val) {
    work->scale = val;
}

void effPcpSetTwinEffectColor(EffPCPWork *work, u32 val) {
    work->unk10 = val;
}

/* Re-rolls slot `index` of the staggered effect: random-angle rotation matrix
 * for both handles, new random offset and delay. */
void effPcpStaggerRerollSlot(EffPCPStaggered *work, s32 index) {
    u128 mtx[4];

    func_002DD688((effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f * 6.283185f);
    VU0_STORE_MATRIX_NOCLOBBER(mtx);
    effParamWorkCallback2(work->handle[index * 2], mtx);
    effParamWorkCallback2(work->handle[index * 2 + 1], mtx);
    mdlAddEntryPlain(effParamWorkGetData(work->handle[index * 2]), 0, 0);
    mdlAddEntryPlain(effParamWorkGetData(work->handle[index * 2 + 1]), 0, 0);
    work->offset[index] = effMiscRandUnitFloat(D_0034DF38) * 150.0f;
    work->delay[index] = effMiscRand(D_0034DF38) % 10;
}

typedef struct {
    u8 pad00[0x3C];
    u32 handle[6];
} EffPCPHandleBlock;

EffPCPWorkF14 *effPcpStaggerCreate(void *args) {
    EffPCPWorkF14 *work = func_002CFEB8(0x98);
    u32 *handle = ((EffPCPHandleBlock *)work)->handle;
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
        effPcpStaggerRerollSlot(work, i++);
    } while (i < 8);
    work->unk10 = 0x80808080;
    work->scale = 1.0f;
    ((EffPCPTaskHeader *)work)->word00 = 0;
    ((EffPCPTaskHeader *)work)->word04 = 0;
    ((EffPCPTaskHeader *)work)->word08 = 0;
    return work;
}

void effPcpStaggerRelease(EffPCPWork *work) {
    u32 *handle = ((EffPCPHandleBlock *)work)->handle;
    s32 i;

    for (i = 7; i >= 0; i--) {
        func_001629F0(handle[-1]);
        func_001629F0(handle[0]);
        handle += 2;
    }
    sdfReleaseChipBlock(work);
}

EffPCPWorkF14 *effCreatePairedResourceWork(EffPCPWork *source) {
    EffPCPWorkF14 *work = func_002CFEB8(0x98);
    u32 *handle = ((EffPCPHandleBlock *)work)->handle;
    s32 i = 0;

    do {
        handle[-1] = effParamWorkDuplicate(source->unk38);
        handle[0] = effParamWorkDuplicate(*(u32 *)source->pad3C);
        handle += 2;
        effPcpStaggerRerollSlot(work, i);
        i++;
    } while (i < 8);
    work->unk10 = 0x80808080;
    work->scale = 1.0f;
    ((EffPCPTaskHeader *)work)->word00 = 0;
    ((EffPCPTaskHeader *)work)->word04 = 0;
    ((EffPCPTaskHeader *)work)->word08 = 0;
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
            VU0_LOAD_VF(vf10, pos);
            mdlStorePrimaryVectorVU(obj[0]);
            effParamWorkCallback1(work->handle[i * 2], work->scale * 1.5f);
            effParamWorkCallback1(work->handle[i * 2 + 1], 1.5f);
            mdlBroadcastMasked(obj[1], work->color);
            func_00217878(obj[0], D_00325828);
            sdfLoadMapRecordPositionVector(((EffPCPWork *)obj[0])->unk18, 1);
            mdlStorePrimaryVectorVU(obj[1]);
            func_00217878(obj[1], D_00325828);
        }
    }
}

void effPcpCopyStaggerVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetStaggerEffectScale(EffPCPWorkF14 *work, f32 val) {
    work->scale = val;
}

void effPcpSetStaggerEffectColor(EffPCPWork *work, u32 val) {
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
void effCrossArmSpawn(EffPCPCrossWork *work, u32 i, u32 j) {
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
    VU0_STORE_MATRIX(mtx);
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
            effCrossArmSpawn(work, i, j);
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
void effCrossEffectRelease(EffPCPWork *work) {
    EffPCPGroupBlock *block = (EffPCPGroupBlock *)((u8 *)work + 0xC);
    s32 i;
    s32 j;

    func_001629F0(work->unk18);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            func_001629F0(block->handle[i][j]);
        }
    }
    sdfReleaseChipBlock(work);
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
            effCrossArmSpawn(work, i, j);
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
void effCrossEffectUpdate(EffPCPCrossWork *work) {
    EffPCPWork *anchor;
    void *obj;
    s32 i;
    s32 j;

    anchor = effParamWorkGetData(work->base);
    VU0_LOAD_VF(vf10, work);
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
            sdfLoadMapRecordPositionVector(anchor->unk18, i * 4 + j + 1);
            mdlStorePrimaryVectorVU(obj);
            func_00217878(obj, D_00325828);
        }
    }
}

void effPcpCopyCrossVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetCrossEffectScale(EffPCPWorkF14 *work, f32 val) {
    work->scale = val;
}

void effPcpSetCrossEffectColor(EffPCPWork *work, u32 val) {
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
void effPcpDelayedPairsRerollSlot(void *work, s32 index) {
    EffPCPPairBlock *block = (EffPCPPairBlock *)((u8 *)work + 8);

    mdlAddEntryPlain(effParamWorkGetData(block->handle[index * 2]), 0, 0);
    mdlAddEntryPlain(effParamWorkGetData(block->handle[index * 2 + 1]), 0, index & 1);
    block->counter[index] = effMiscRand(D_0034DF38) % 10;
}

typedef struct {
    u8 pad00[0x1C];
    u32 handle[12];
} EffPCPDelayedPairBlock;

EffPCPWorkF14 *effCreateIndexedResourceWork(void *source) {
    EffPCPWorkF14 *work = func_002CFEB8(0x60);
    u32 *handle = ((EffPCPDelayedPairBlock *)work)->handle;
    s32 i;

    for (i = 0; i < 6; i++) {
        if (i == 0) {
            work->unk1C = effParamCreateFromTable(source, 6);
        }
        handle[-1] = effParamCreateFromTable(source, i);
        handle[0] = effParamWorkDuplicate(work->unk1C);
        handle += 2;
        effPcpDelayedPairsRerollSlot(work, i);
    }
    work->unk10 = 0x80808080;
    work->scale = 1.0f;
    ((EffPCPTaskHeader *)work)->word00 = 0;
    ((EffPCPTaskHeader *)work)->word04 = 0;
    ((EffPCPTaskHeader *)work)->word08 = 0;
    return work;
}

void effPcpDelayedPairsRelease(EffPCPWork *work) {
    u32 *handle = ((EffPCPDelayedPairBlock *)work)->handle;
    s32 i;

    for (i = 5; i >= 0; i--) {
        func_001629F0(handle[-1]);
        func_001629F0(handle[0]);
        handle += 2;
    }
    sdfReleaseChipBlock(work);
}

EffPCPWorkF14 *effCopyIndexedResourceWork(EffPCPWork *source) {
    u32 *sourceHandle;
    u32 *workHandle;
    s32 i;
    EffPCPWorkF14 *work = func_002CFEB8(0x60);
    sourceHandle = ((EffPCPDelayedPairBlock *)source)->handle;
    workHandle = ((EffPCPDelayedPairBlock *)work)->handle;

    for (i = 0; i < 6; i++) {
        workHandle[-1] = effParamWorkDuplicate(sourceHandle[-1]);
        workHandle[0] = effParamWorkDuplicate(sourceHandle[0]);
        sourceHandle += 2;
        workHandle += 2;
        effPcpDelayedPairsRerollSlot(work, i);
    }
    work->unk10 = 0x80808080;
    work->scale = 1.0f;
    ((EffPCPTaskHeader *)work)->word00 = 0;
    ((EffPCPTaskHeader *)work)->word04 = 0;
    ((EffPCPTaskHeader *)work)->word08 = 0;
    return work;
}

typedef struct {
    u8 pad00[0x10];
    u32 color;       /* 0x10 sent to the paired object's colour callback */
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
            VU0_LOAD_VF_MEMORY(vf10, work);
            mdlStorePrimaryVectorVU(obj[0]);
            mdlBroadcastMasked(obj[1], work->color);
            func_00217878(obj[0], D_00325828);
            sdfLoadMapRecordPositionVector(((EffPCPWork *)obj[0])->unk18, 1);
            VU0_STORE_VF_UNCLOBBERED(vf10, &vec);
            mdlStorePrimaryVectorVU(obj[1]);
            func_00217878(obj[1], D_00325828);
        }
    }
}

void effPcpCopyDelayedPairVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetDelayedPairScale(EffPCPWorkF14 *work, f32 val) {
    work->scale = val;
}

void effPcpSetDelayedPairColor(EffPCPWork *work, u32 val) {
    work->unk10 = val;
}

void effPcpChargeInitTail(EffPCPChargeWork *work) {
    work->unk133C = 0;
    work->unk1340 = 0;
    work->color1344 = 0x80808080;
}

EffPCPChargeWork *effCreateChargeWork(void *source) {
    void *resource = func_002D03F8(0x1354);
    EffPCPChargeWork *work = sdfResourceRetainAddress(resource);
    work->allocationHandle = (u32)resource;
    work->primaryHandle = effParamCreateFromTable(source, 0);
    work->secondaryHandle = effParamCreateFromTable(source, 1);
    effPcpChargeInitTail(work);
    work->unkAF0 = 0;
    work->unkAF4 = 0;
    work->unkAF8 = 0;
    work->color = 0x80808080;
    work->scale = 1.0f;
    return work;
}

void effPcpChargeReleaseResources(EffPCPChargeWork *work) {
    func_001629F0(work->primaryHandle);
    func_001629F0(work->secondaryHandle);
    func_002D0918(work->allocationHandle);
}

EffPCPChargeWork *effCopyChargeResources(EffPCPChargeWork *source) {
    void *resource = func_002D03F8(0x1354);
    EffPCPChargeWork *work = sdfResourceRetainAddress(resource);
    u32 firstHandle = source->primaryHandle;
    work->allocationHandle = (u32)resource;
    work->primaryHandle = effParamWorkDuplicate(firstHandle);
    work->secondaryHandle = effParamWorkDuplicate(source->secondaryHandle);
    effPcpChargeInitTail(work);
    work->unkAF0 = 0;
    work->unkAF4 = 0;
    work->unkAF8 = 0;
    work->color = 0x80808080;
    work->scale = 1.0f;
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178790);

void effPcpCopyVectorAF0(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0xAF0, src);
}

void effPcpChargeSetScale(EffPCPChargeWork *work, f32 val) {
    work->scale = val;
}

void effPcpChargeSetColor(EffPCPChargeWork *work, u32 val) {
    work->color = val;
}

EffPCPWork1C *effPcpSpawnOnceCreate(EffPCPWork *src) {
    EffPCPWork1C *dst;
    u32 handle;

    dst = func_002CFEB8(0x1C);
    dst->primaryHandle = effParamCreateFromTable(src, 0);
    handle = effParamCreateFromTable(src, 1);
    dst->unk00 = 0;
    dst->secondaryHandle = handle;
    dst->unk04 = 0;
    dst->color10 = 0x80808080;
    dst->unk08 = 0;
    return dst;
}

void effPcpSpawnOnceRelease(EffPCPWork1C *work) {
    func_001629F0(work->primaryHandle);
    func_001629F0(work->secondaryHandle);
    sdfReleaseChipBlock(work);
}

EffPCPWork1C *effPcpSpawnOnceClone(EffPCPWork1C *src) {
    EffPCPWork1C *dst;
    u32 handle;

    dst = func_002CFEB8(0x1C);
    dst->primaryHandle = effParamWorkDuplicate(src->primaryHandle);
    handle = effParamWorkDuplicate(src->secondaryHandle);
    dst->unk00 = 0;
    dst->secondaryHandle = handle;
    dst->unk04 = 0;
    dst->color10 = 0x80808080;
    dst->unk08 = 0;
    return dst;
}

void effPcpSpawnOnce(EffPCPWork1C *work) {
    void *obj;
    u128 vec;

    obj = effParamWorkGetData(work->primaryHandle);
    VU0_LOAD_VF_MEMORY(vf10, work);
    mdlStorePrimaryVectorVU(obj);
    effParamWorkCallback3(work->secondaryHandle, work->color10);
    func_00217878(obj, D_00325828);
    sdfLoadMapRecordPositionVector(((EffPCPWork *)obj)->unk18, 1);
    VU0_STORE_VF_TO_MEMORY(vf10, vec);
    effParamWorkCallback0(work->secondaryHandle, &vec);
    effParamWorkInvokeCallback(work->secondaryHandle);
}

void effPcpCopySpawnOnceVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSpawnOnceSetColor(EffPCPWork1C *work, u32 val) {
    work->color10 = val;
}

/* Shared scratch parameter block handed to the particle spawner (D_00354D40)
 * and the shared spawn origin vector at +0x10 (D_00354D50, separate symbol). */
typedef struct EffSpawnParams {
    f32 pos[4];
    f32 vel[4];
    u8 pad20[0x0C];
    f32 speed;       /* 0x2C: randomised motion speed */
    u8 pad30[0x08];
    s16 lifetime;    /* 0x38: randomised particle lifetime */
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
extern f32 sdfSinPoly(f32);
extern f32 sdfEvaluateCosineViaSinePhaseShift(f32);
extern void sdfBuildVuRotationFromAxisAngle(f32 *axis, f32 angle);

/* Spawns 12 particles in a ring: every second particle advances the ring angle
 * (60 degrees). Direction is normalised on the VU, scaled per axis and offset
 * by the origin vector; short-reach particles get a shorter life. */
void effPcpInitTwelveRadialParticles(EffSpawnGroup *group) {
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
            sinv = sdfEvaluateCosineViaSinePhaseShift(angle);
            cosv = sdfSinPoly(angle);
            angle += 60.0f * EFF_DEG2RAD;
        }
        spread = effMiscRandUnitFloat(D_0034DF38) * 0.5f + 0.5f;
        D_00354D40->unk44 = spread * 1.25f;
        D_00354D40->unk4C = spread * 12.5f;
        radius = (effMiscRandUnitFloat(D_0034DF38) * 0.25f + 0.75f) * 100.0f;
        D_00354D40->vel[1] = 0;
        D_00354D40->vel[0] = sinv * radius;
        D_00354D40->vel[2] = cosv * radius;
        dir[0] = sinv;
        dir[1] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f + 2.0f;
        dir[2] = cosv;
        VU0_LOAD_VF($vf10, dir);
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
        VU0_STORE_VF($vf10, dir);
        reach = (effMiscRandUnitFloat(D_0034DF38) * 0.65f + (1.0f - 0.65f)) * 350.0f;
        scale[2] = reach;
        scale[0] = reach;
        scale[1] = -reach;
        VU0_LOAD_VF($vf10, dir);
        VU0_LOAD_VF($vf11, scale);
        VU0_MUL(vf10, vf10, vf11);
        VU0_LOAD_VF($vf11, D_00354D50);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF($vf10, D_00354D40);
        if (scale[0] < 250.0f) {
            speed = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f * 2.5f + 5.0f;
            life = effMiscRand(D_0034DF38) % 5 + 5;
        } else {
            speed = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f * 5.0f + 10.0f;
            life = effMiscRand(D_0034DF38) % 6 + 15;
        }
        D_00354D40->lifetime = life;
        D_00354D40->speed = speed;
        *out = effThunderFragCreate(D_00354D40);
        out++;
        i++;
    } while (i < 12);
}

void *effPcpCreateTwelveRadialParticles(void) {
    void *work;

    work = func_002CFEB8(0x4c);
    effPcpInitTwelveRadialParticles(work);
    return work;
}

void effPcpReleaseTwelveRadialParticles(EffPCPWork *work) {
    s32 i;
    u32 *handle;

    handle = &work->unk1C;
    for (i = 0; i < 12; i++) {
        effPCPThunderFree3(handle[i]);
    }
    sdfReleaseChipBlock(work);
}

void *effPcpAllocateRadialParticleGroup(void) {
    void *work;

    work = func_002CFEB8(0x4c);
    effPcpInitTwelveRadialParticles(work);
    return work;
}

/* Work whose position offsets a list of thunder objects (starting at 0x1C). */
typedef struct EffPCPThunderShift {
    f32 pos[4];
    u8 pad10[0xC];
    u32 handle[30];
} EffPCPThunderShift;

/* Shifts the first `count` handles' objects by the work position for the
 * duration of their update, then puts the original vectors back. */
static inline void effPcpShiftThunderHandles(EffPCPThunderShift *work, s32 count) {
    u128 saved[2];
    u8 *obj;
    s32 i;

    for (i = 0; i < count; i++) {
        obj = func_00165638(work->handle[i]);
        VU0_LOAD_VF($vf10, obj + 0x10);
        VU0_STORE_VF($vf10, &saved[1]);
        VU0_LOAD_VF($vf11, work);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF($vf10, obj + 0x10);
        VU0_LOAD_VF($vf10, obj);
        VU0_STORE_VF($vf10, &saved[0]);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF($vf10, obj);
        func_00165D80(work->handle[i]);
        PCP_COPY_VECTOR(obj, &saved[0]);
        PCP_COPY_VECTOR(obj + 0x10, &saved[1]);
    }
}

void effPcpThunderShiftUpdateA(EffPCPThunderShift *work) {
    effPcpShiftThunderHandles(work, 12);
}

void effPcpCopyRadialParticleVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetRadialParticleScale(EffPCPWorkF14 *work, f32 val) {
    work->scale = val;
}

void func_00179150(EffPCPWork *work, u32 val) {
    work->unk18 = val;
}

extern EffSpawnParams D_00354DA0[];

/* 12-piece spread on a cone around a random axis (rotation matrix built by
 * sdfBuildVuRotationFromAxisAngle into the VU0 matrix registers). */
void effPcpSpawnConeTwelve(EffSpawnGroup *group) {
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
            sinv = sdfEvaluateCosineViaSinePhaseShift(angle);
            cosv = sdfSinPoly(angle);
            angle += 60.0f * EFF_DEG2RAD;
        }
        spread = effMiscRandUnitFloat(D_0034DF38) * 0.5f + 0.5f;
        D_00354DA0->unk44 = spread * 1.25f;
        D_00354DA0->unk4C = spread * 12.5f;
        radius = (effMiscRandUnitFloat(D_0034DF38) * 0.25f + 0.75f) * 100.0f;
        axis[0] = cosv;
        axis[1] = 0;
        axis[2] = -sinv;
        D_00354DA0->vel[1] = 0;
        D_00354DA0->vel[0] = sinv * radius;
        D_00354DA0->vel[2] = cosv * radius;
        sdfBuildVuRotationFromAxisAngle(axis, -(effMiscRandUnitFloat(D_0034DF38) * (50.0f * EFF_DEG2RAD) + 5.0f * EFF_DEG2RAD));
        height = (effMiscRandUnitFloat(D_0034DF38) * 0.65f + (1.0f - 0.65f)) * 500.0f;
        D_00354DA0->pos[0] = 0;
        D_00354DA0->pos[2] = 0;
        D_00354DA0->pos[1] = -height;
        VU0_LOAD_VF($vf10, D_00354DA0->pos);
                VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF($vf10, D_00354DA0->pos);
        D_00354DA0->pos[0] += D_00354DA0->vel[0];
        D_00354DA0->pos[2] += D_00354DA0->vel[2];
        if (height < 300.0f) {
            speed = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f * 2.5f + 5.0f;
            life = effMiscRand(D_0034DF38) % 5 + 5;
        } else {
            speed = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f * 5.0f + 10.0f;
            life = effMiscRand(D_0034DF38) % 6 + 15;
        }
        D_00354DA0->lifetime = life;
        D_00354DA0->speed = speed;
        group->handles[i] = effThunderFragCreate(D_00354DA0);
        i++;
    } while (i < 12);
}

void *effPcpAllocateNarrowConeGroup(void) {
    void *work;

    work = func_002CFEB8(0x4c);
    effPcpSpawnConeTwelve(work);
    return work;
}

void effPcpNarrowConeGroupRelease(EffPCPWork *work) {
    s32 i;
    u32 *handle;

    handle = &work->unk1C;
    for (i = 0; i < 12; i++) {
        effPCPThunderFree3(handle[i]);
    }
    sdfReleaseChipBlock(work);
}

void *effPcpCreateNarrowConeParticles(void) {
    void *work;

    work = func_002CFEB8(0x4c);
    effPcpSpawnConeTwelve(work);
    return work;
}

void effPcpThunderShiftUpdateB(EffPCPThunderShift *work) {
    effPcpShiftThunderHandles(work, 12);
}

void effPcpCopyNarrowConeVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetNarrowConeScale(EffPCPWorkF14 *work, f32 val) {
    work->scale = val;
}

void func_001795C8(EffPCPWork *work, u32 val) {
    work->unk18 = val;
}

extern EffSpawnParams D_00354E00[];

/* 30-piece spread: pieces are placed on a cone around a random axis (rotation
 * matrix built by sdfBuildVuRotationFromAxisAngle into the VU0 matrix registers). */
void effPcpSpawnVariableHeightParticles(EffSpawnGroup *group) {
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
            sinv = sdfEvaluateCosineViaSinePhaseShift(angle);
            cosv = sdfSinPoly(angle);
            angle += 24.0f * EFF_DEG2RAD;
        }
        spread = effMiscRandUnitFloat(D_0034DF38) * 0.5f + 0.5f;
        D_00354E00->unk44 = spread * 1.25f;
        D_00354E00->unk4C = spread * 12.5f;
        D_00354E00->vel[1] = 0;
        D_00354E00->vel[0] = sinv * ((effMiscRandUnitFloat(D_0034DF38) * 0.25f + 0.75f) * 500.0f);
        D_00354E00->vel[2] = cosv * ((effMiscRandUnitFloat(D_0034DF38) * 0.25f + 0.75f) * 250.0f);
        axis[0] = cosv;
        axis[1] = 0;
        axis[2] = -sinv;
        sdfBuildVuRotationFromAxisAngle(axis, -(effMiscRandUnitFloat(D_0034DF38) * (55.0f * EFF_DEG2RAD) + 5.0f * EFF_DEG2RAD));
        height = (effMiscRandUnitFloat(D_0034DF38) * 0.65f + (1.0f - 0.65f)) * 500.0f;
        D_00354E00->pos[0] = 0;
        D_00354E00->pos[2] = 0;
        D_00354E00->pos[1] = -height;
        VU0_LOAD_VF($vf10, D_00354E00->pos);
                VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF($vf10, D_00354E00->pos);
        D_00354E00->pos[0] += D_00354E00->vel[0];
        D_00354E00->pos[2] += D_00354E00->vel[2];
        if (height < 300.0f) {
            speed = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f * 2.5f + 5.0f;
            life = effMiscRand(D_0034DF38) % 5 + 5;
        } else {
            speed = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f * 5.0f + 10.0f;
            life = effMiscRand(D_0034DF38) % 6 + 15;
        }
        D_00354E00->lifetime = life;
        D_00354E00->speed = speed;
        group->handles[i] = effThunderFragCreate(D_00354E00);
        i++;
    } while (i < 30);
}

void *effPcpAllocateVariableHeightParticleGroup(void) {
    void *work;

    work = func_002CFEB8(0x94);
    effPcpSpawnVariableHeightParticles(work);
    return work;
}

void effPcpWideConeGroupRelease(EffPCPWork *work) {
    s32 i;
    u32 *handle;

    handle = &work->unk1C;
    for (i = 0; i < 30; i++) {
        effPCPThunderFree3(handle[i]);
    }
    sdfReleaseChipBlock(work);
}

void *effPcpCreateVariableHeightParticles(void) {
    void *work;

    work = func_002CFEB8(0x94);
    effPcpSpawnVariableHeightParticles(work);
    return work;
}

void effPcpThunderShiftUpdateC(EffPCPThunderShift *work) {
    effPcpShiftThunderHandles(work, 30);
}

void effPcpCopyVariableHeightVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetVariableHeightParticleScale(EffPCPWorkF14 *work, f32 val) {
    work->scale = val;
}

void func_00179A60(EffPCPWork *work, u32 val) {
    work->unk18 = val;
}

extern EffSpawnParams D_00354E60[];

/* Same spread as effPcpSpawnConeTwelve with a wider cone and its own parameter block. */
void effPcpSpawnWideConeTwelve(EffSpawnGroup *group) {
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
            sinv = sdfEvaluateCosineViaSinePhaseShift(angle);
            cosv = sdfSinPoly(angle);
            angle += 60.0f * EFF_DEG2RAD;
        }
        spread = effMiscRandUnitFloat(D_0034DF38) * 0.5f + 0.5f;
        D_00354E60->unk44 = spread * 1.25f;
        D_00354E60->unk4C = spread * 12.5f;
        radius = (effMiscRandUnitFloat(D_0034DF38) * 0.25f + 0.75f) * 100.0f;
        axis[0] = cosv;
        axis[1] = 0;
        axis[2] = -sinv;
        D_00354E60->vel[1] = 0;
        D_00354E60->vel[0] = sinv * radius;
        D_00354E60->vel[2] = cosv * radius;
        sdfBuildVuRotationFromAxisAngle(axis, -(effMiscRandUnitFloat(D_0034DF38) * (55.0f * EFF_DEG2RAD) + 5.0f * EFF_DEG2RAD));
        height = (effMiscRandUnitFloat(D_0034DF38) * 0.65f + (1.0f - 0.65f)) * 500.0f;
        D_00354E60->pos[0] = 0;
        D_00354E60->pos[2] = 0;
        D_00354E60->pos[1] = -height;
        VU0_LOAD_VF($vf10, D_00354E60->pos);
                VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF($vf10, D_00354E60->pos);
        D_00354E60->pos[0] += D_00354E60->vel[0];
        D_00354E60->pos[2] += D_00354E60->vel[2];
        if (height < 300.0f) {
            speed = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f * 2.5f + 5.0f;
            life = effMiscRand(D_0034DF38) % 5 + 5;
        } else {
            speed = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f * 5.0f + 10.0f;
            life = effMiscRand(D_0034DF38) % 6 + 15;
        }
        D_00354E60->lifetime = life;
        D_00354E60->speed = speed;
        group->handles[i] = effThunderFragCreate(D_00354E60);
        i++;
    } while (i < 12);
}

void *effPcpAllocateWideConeGroup(void) {
    void *work;

    work = func_002CFEB8(0x4c);
    effPcpSpawnWideConeTwelve(work);
    return work;
}

void effPcpReleaseWideConeParticles(EffPCPWork *work) {
    s32 i;
    u32 *handle;

    handle = &work->unk1C;
    for (i = 0; i < 12; i++) {
        effPCPThunderFree3(handle[i]);
    }
    sdfReleaseChipBlock(work);
}

void *effPcpCreateWideConeParticles(void) {
    void *work;

    work = func_002CFEB8(0x4c);
    effPcpSpawnWideConeTwelve(work);
    return work;
}

void effPcpThunderShiftUpdateD(EffPCPThunderShift *work) {
    effPcpShiftThunderHandles(work, 12);
}

void effPcpCopyWideConeVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetWideConeScale(EffPCPWorkF14 *work, f32 val) {
    work->scale = val;
}

void func_00179ED8(EffPCPWork *work, u32 val) {
    work->unk18 = val;
}

extern EffSpawnParams D_00354EB8[];

/* 12 pieces thrown from a fixed origin along a random angle; the param block
 * (of two) is picked at random. */
void effPcpSpawnFixedOriginTwelve(EffSpawnGroup *group) {
    s32 i;
    EffSpawnParams *params;
    f32 angle;
    f32 dist;
    s32 life;

    for (i = 0; i < 12; i++) {
        params = &D_00354EB8[effMiscRand(D_0034DF38) & 1];
        params->speed = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f * 5.0f + 20.0f;
        life = effMiscRand(D_0034DF38) % 6 + 15;
        params->pos[0] = -150.0f;
        params->pos[1] = -500.0f;
        params->pos[2] = 0;
        params->lifetime = life;
        angle = effMiscRandUnitFloat(D_0034DF38) * (3.14159265f / 2.0f) + 3.14159265f / 8.0f;
        dist = (effMiscRandUnitFloat(D_0034DF38) * 0.25f + 0.75f) * 600.0f;
        params->vel[0] = params->pos[0] + sdfEvaluateCosineViaSinePhaseShift(angle) * dist;
        params->vel[1] = params->pos[1] + sdfSinPoly(angle) * dist;
        params->vel[2] = 0;
        group->handles[i] = effThunderFragCreate(params);
    }
}

void *effPcpAllocateFixedOriginGroup(void) {
    void *work;

    work = func_002CFEB8(0x4c);
    effPcpSpawnFixedOriginTwelve(work);
    return work;
}

void effPcpAngledBurstGroupRelease(EffPCPWork *work) {
    s32 i;
    u32 *handle;

    handle = &work->unk1C;
    for (i = 0; i < 12; i++) {
        effPCPThunderFree3(handle[i]);
    }
    sdfReleaseChipBlock(work);
}

void *effPcpCreateFixedOriginParticles(void) {
    void *work;

    work = func_002CFEB8(0x4c);
    effPcpSpawnFixedOriginTwelve(work);
    return work;
}

void effPcpThunderShiftUpdateE(EffPCPThunderShift *work) {
    effPcpShiftThunderHandles(work, 12);
}

void effPcpCopyFixedOriginVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetFixedOriginScale(EffPCPWorkF14 *work, f32 val) {
    work->scale = val;
}

void func_0017A248(EffPCPWork *work, u32 val) {
    work->unk18 = val;
}

extern void func_002DD608(f32 angle);
extern void func_002DD9E8(f32 angle);
extern void sdfMultiplyVuMatrixInPlace(void);
extern EffSpawnParams D_00354F60[];

/* 8 pieces flung sideways from below; the rotation matrix is composed by the
 * three func_002DD* calls into the VU0 matrix registers. */
void effPcpSpawnSidewaysEight(EffSpawnGroup *group) {
    s32 i;
    EffSpawnParams *params;

    for (i = 0; i < 8; i++) {
        params = &D_00354F60[effMiscRand(D_0034DF38) & 1];
        if (i & 1) {
            params->vel[0] = effMiscRandUnitFloat(D_0034DF38) * 500.0f;
        } else {
            params->vel[0] = effMiscRandUnitFloat(D_0034DF38) * -500.0f;
        }
        params->vel[1] = 0;
        params->vel[2] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f * 250.0f;
        params->pos[0] = 0;
        params->pos[1] = -600.0f;
        params->pos[2] = 0;
        func_002DD608((effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f * (30.0f * EFF_DEG2RAD));
        func_002DD9E8((effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f * (30.0f * EFF_DEG2RAD));
        sdfMultiplyVuMatrixInPlace();
        VU0_LOAD_VF($vf10, params->pos);
                VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF($vf10, params->pos);
        params->pos[0] += params->vel[0];
        params->pos[2] += params->vel[2];
        group->handles[i] = effThunderFragCreate(params);
    }
}

void *effPcpAllocateSideBurstGroup(void) {
    void *work;

    work = func_002CFEB8(0x3c);
    effPcpSpawnSidewaysEight(work);
    return work;
}

void effPcpSideBurstGroupRelease(EffPCPWork *work) {
    s32 i;
    u32 *handle;

    handle = &work->unk1C;
    for (i = 0; i < 8; i++) {
        effPCPThunderFree3(handle[i]);
    }
    sdfReleaseChipBlock(work);
}

void *effPcpCreateSideBurstParticles(void) {
    void *work;

    work = func_002CFEB8(0x3c);
    effPcpSpawnSidewaysEight(work);
    return work;
}

void effPcpThunderShiftUpdateF(EffPCPThunderShift *work) {
    effPcpShiftThunderHandles(work, 8);
}

void effPcpCopySideBurstVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetSideBurstScale(EffPCPWorkF14 *work, f32 val) {
    work->scale = val;
}

void func_0017A5B8(EffPCPWork *work, u32 val) {
    work->unk18 = val;
}

void effPcpInitializeThunderHandleWork(EffPCPWork *work) {
    u32 handle;

    handle = func_001632E0(D_00355008);
    work->unk18 = 0;
    work->unk1C = handle;
}

void *effPcpCreateThunderHandleWork(void) {
    void *work;

    work = func_002CFEB8(0x20);
    effPcpInitializeThunderHandleWork(work);
    return work;
}

void effPcpReleaseThunderHandleWork(EffPCPWork *work) {
    effPCPThunderFree(work->unk1C);
    sdfReleaseChipBlock(work);
}

void *effAllocateThunderHandleWork(void) {
    void *work;

    work = func_002CFEB8(0x20);
    effPcpInitializeThunderHandleWork(work);
    return work;
}

typedef struct EffPCPFadeTarget {
    u8 pad00[0x1C];
    f32 sizeX; /* animated dimension on the first axis */
    f32 sizeY; /* animated dimension on the second axis */
} EffPCPFadeTarget;

typedef struct EffPCPFadeWork {
    u8 pad00[0x14];
    u32 color;   /* fades from this colour to zero */
    u32 frame;
    u32 handle;
} EffPCPFadeWork;

extern void effPCPThunderSetParam50(u32 handle, u32 value);
extern void func_00163508(u32 handle, void *work);
extern void func_00163CD0(u32 handle);
extern u32 effBlendColor(u32 colorA, u32 colorB, f32 t);

/* Scales the target over the first 22 frames, fades the colour out from frame 45. */
void effPcpThunderExpandThenFadeUpdate(EffPCPFadeWork *work) {
    EffPCPFadeTarget *target;
    f32 phase;
    u32 fadedColor;

    if (work->frame < 0x17) {
        target = func_00163540(work->handle);
        phase = (f32)work->frame / 22.0f;
        target->sizeX = phase * 600.0f + 200.0f;
        target->sizeY = phase * 300.0f + 100.0f;
    }
    if (work->frame >= 0x2D) {
        phase = (f32)(work->frame - 0x2D) / 30.0f;
        fadedColor = effBlendColor(work->color, 0, phase);
    } else {
        fadedColor = work->color;
    }
    effPCPThunderSetParam50(work->handle, fadedColor);
    func_00163508(work->handle, work);
    func_00163CD0(work->handle);
    work->frame++;
}

void effPcpCopyThunderHandleVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0017A800(EffPCPWorkF10 *work, f32 val) {
    work->unk10 = val;
}

void effPcpSetExpandingThunderFadeColor(EffPCPWork *work, u32 val) {
    work->unk14 = val;
}

extern EffSpawnParams D_00355060[];

/* Same spread as effPcpSpawnVariableHeightParticles with its own parameter block. */
void effPcpSpawnConeThirty(EffSpawnGroup *group) {
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
            sinv = sdfEvaluateCosineViaSinePhaseShift(angle);
            cosv = sdfSinPoly(angle);
            angle += 24.0f * EFF_DEG2RAD;
        }
        spread = effMiscRandUnitFloat(D_0034DF38) * 0.5f + 0.5f;
        D_00355060->unk44 = spread * 1.25f;
        D_00355060->unk4C = spread * 12.5f;
        D_00355060->vel[1] = 0;
        D_00355060->vel[0] = sinv * ((effMiscRandUnitFloat(D_0034DF38) * 0.25f + 0.75f) * 500.0f);
        D_00355060->vel[2] = cosv * ((effMiscRandUnitFloat(D_0034DF38) * 0.25f + 0.75f) * 250.0f);
        axis[0] = cosv;
        axis[1] = 0;
        axis[2] = -sinv;
        sdfBuildVuRotationFromAxisAngle(axis, -(effMiscRandUnitFloat(D_0034DF38) * (55.0f * EFF_DEG2RAD) + 5.0f * EFF_DEG2RAD));
        height = (effMiscRandUnitFloat(D_0034DF38) * 0.65f + (1.0f - 0.65f)) * 500.0f;
        D_00355060->pos[0] = 0;
        D_00355060->pos[2] = 0;
        D_00355060->pos[1] = -height;
        VU0_LOAD_VF($vf10, D_00355060->pos);
                VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF($vf10, D_00355060->pos);
        D_00355060->pos[0] += D_00355060->vel[0];
        D_00355060->pos[2] += D_00355060->vel[2];
        if (height < 300.0f) {
            speed = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f * 2.5f + 5.0f;
            life = effMiscRand(D_0034DF38) % 5 + 5;
        } else {
            speed = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f * 5.0f + 10.0f;
            life = effMiscRand(D_0034DF38) % 6 + 15;
        }
        D_00355060->lifetime = life;
        D_00355060->speed = speed;
        group->handles[i] = effThunderFragCreate(D_00355060);
        i++;
    } while (i < 30);
}

void *effPcpAllocateDenseConeGroup(void) {
    void *work;

    work = func_002CFEB8(0x94);
    effPcpSpawnConeThirty(work);
    return work;
}

void effPcpReleaseConeParticleGroup(EffPCPWork *work) {
    s32 i;
    u32 *handle;

    handle = &work->unk1C;
    for (i = 0; i < 30; i++) {
        effPCPThunderFree3(handle[i]);
    }
    sdfReleaseChipBlock(work);
}

void *effPcpCreateDenseConeParticles(void) {
    void *work;

    work = func_002CFEB8(0x94);
    effPcpSpawnConeThirty(work);
    return work;
}

void effPcpThunderShiftUpdateG(EffPCPThunderShift *work) {
    effPcpShiftThunderHandles(work, 30);
}

void effPcpCopyDenseConeVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetDenseConeScale(EffPCPWorkF14 *work, f32 val) {
    work->scale = val;
}

void func_0017ACA0(EffPCPWork *work, u32 val) {
    work->unk18 = val;
}

void effPcpInitGrowingThunderFadeWork(EffPCPWork *work) {
    u32 handle;

    handle = func_001632E0(D_003550B8);
    work->unk18 = 0;
    work->unk1C = handle;
}

void *effPcpCreateGrowingThunderFadeWork(void) {
    void *work;

    work = func_002CFEB8(0x20);
    effPcpInitGrowingThunderFadeWork(work);
    return work;
}

void effPcpReleaseGrowingThunderFadeWork(EffPCPWork *work) {
    effPCPThunderFree(work->unk1C);
    sdfReleaseChipBlock(work);
}

void *effAllocateGrowingThunderFadeWork(void) {
    void *work;

    work = func_002CFEB8(0x20);
    effPcpInitGrowingThunderFadeWork(work);
    return work;
}

/* Same fade with a longer scale-up; stops updating once frame 65 is reached. */
void effPcpThunderUniformGrowFadeUpdate(EffPCPFadeWork *work) {
    EffPCPFadeTarget *target;
    f32 phase;
    f32 value;
    u32 fadedColor;
    u32 frame;

    frame = work->frame;
    if (frame == 0x41) {
        return;
    }
    if (frame < 0x1E) {
        target = func_00163540(work->handle);
        phase = (f32)frame / 30.0f;
        value = phase * 200.0f + 150.0f;
        target->sizeX = value;
        target->sizeY = value;
    }
    if (frame >= 0x39) {
        phase = ((f32)frame - 57.0f) * 0.125f;
        fadedColor = effBlendColor(work->color, 0, phase);
        effPCPThunderSetParam50(work->handle, fadedColor);
    } else {
        effPCPThunderSetParam50(work->handle, work->color);
    }
    func_00163508(work->handle, work);
    func_00163CD0(work->handle);
    work->frame++;
}

void effPcpCopyGrowingThunderFadeVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0017AEE0(EffPCPWorkF10 *work, f32 val) {
    work->unk10 = val;
}

void effPcpSetGrowingThunderFadeColor(EffPCPWork *work, u32 val) {
    work->unk14 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017AEF0);

void effPcpSharedWorkRelease(EffPCPWork *work)
{
    sdfReleaseChipBlock(work);
    if (--D_003BB048 != 0) {
        return;
    }
    func_00186CB8(D_003BD7FC->unk38);
    sdfReleaseChipBlock(D_003BD7FC);
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
    f32 scale; /* multiplies the computed trail size */
    s32 minSize;
    EffPCPTrailObj *obj;
} EffPCPTrailWork;

extern s32 func_0018DC58(f32 value);
extern u32 effMultiplyPackedColors(u32 flags, u32 color);
extern void effDrawBlurPixelRectWithResource(EffPCPTrailObj *obj);

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

void effPcpUpdateSharedTrail(ref)
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
        VU0_LOAD_VF($vf10, work);
        obj->size = (s32)((f32)func_0018DC58(EFF_SHARED_TRAIL->unk1C) * EFF_SHARED_TRAIL->scale);
        VU0_STORE_VF($vf10, pos);
        obj->x = (s32)pos[0] - 0x800;
        obj->y = ((s32)pos[1] - 0x800) << 1;
        if (obj->size < EFF_SHARED_TRAIL->minSize) {
            obj->size = EFF_SHARED_TRAIL->minSize;
        }
        obj->color = effMultiplyPackedColors(EFF_SHARED_TRAIL->flags, EFF_SHARED_TRAIL->color);
        effDrawBlurPixelRectWithResource(obj);
        EFF_SHARED_TRAIL->frame++;
    }
}

void effPcpSharedTrailSetPosition(void *work, void *src) {
    PCP_COPY_VECTOR(D_003BD7FC, src);
}

void func_0017B160(u32 unused, u32 val) {
    D_003BD7FC->unk10 = val;
}

void effSetSharedScale(u32 unused, f32 value) {
    ((EffPCPWorkF1C *)D_003BD7FC)->unk1C = value;
}

typedef struct EffPCPTrailParams {
    u32 count;
    u32 colors[2];
    f32 scale;
    s32 minSize;
    u32 objParams;
    u32 color;
} EffPCPTrailParams;

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B180);

void effPcpTrailRelease(EffPCPWork *work) {
    func_00186CB8(work->unk34);
    sdfReleaseChipBlock(work);
}

/* Per-frame update: places the trail object from the work position and
 * alternates its colour between two entries until the count runs out. */
void effPcpTrailUpdate(EffPCPTrailWork *work) {
    EffPCPTrailObj *obj;
    f32 pos[4];

    obj = work->obj;
    VU0_LOAD_VF($vf10, work->pos);
    obj->size = (s32)((f32)func_0018DC58(work->unk1C) * work->scale);
    VU0_STORE_VF($vf10, pos);
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
    obj->color = effMultiplyPackedColors(work->flags, work->color);
    effDrawBlurPixelRectWithResource(obj);
}

void effPcpCopySharedTrailVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetTrailFlags(EffPCPWork *work, u32 val) {
    work->unk10 = val;
}

void effPcpTrailSetSizeInput(EffPCPWorkF1C *work, f32 val) {
    work->unk1C = val;
}

EffPCPCompactWork3C *effPcpCompactEffectCreate(EffPCPCompactParams *params) {
    EffPCPCompactWork3C *work;

    work = func_002CFEB8(0x3C);
    work->resource = effCloneResourceTemplate(&params->unk18);
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

void effPcpCompactEffectCreateFromTable(void *args) {
    void *param0;

    param0 = effParamTableGetBlock(args, 0);
    effPcpCompactEffectCreate(param0);
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
    effPcpCompactEffectCreate((EffPCPCompactParams *)&params);
}

void effPcpCompactEffectRelease(EffPCPWork *work) {
    func_00188050(work->unk38);
    sdfReleaseChipBlock(work);
}

typedef struct EffPCPLerpObj {
    s32 size;
    s32 x;
    s32 y;
    u32 color;
} EffPCPLerpObj;

/* Update work whose size runs from `base` to `target` over `duration` frames
 * while its colour fades in and out. */
typedef struct EffPCPLerpWork {
    f32 pos[4];
    u8 fixed;
    u8 pad11[3];
    u32 colorFrom;
    u32 colorTo;
    s32 frame;
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
    s32 base;
    s32 target;
    u8 pad34[4];
    EffPCPLerpObj *obj;
} EffPCPLerpWork;

extern void sdfProjectVuVectorToScreen();
extern void func_00188068(EffPCPLerpObj *obj);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B4C8);

void func_0017B670(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetCompactColor(EffPCPWork *work, u32 val) {
    work->unk14 = val;
}

void func_0017B688(EffPCPWorkF34 *work, f32 val) {
    work->unk34 = val;
}

EffPCPCompactWork3C *func_0017B690(EffPCPCompactParams *params) {
    EffPCPCompactWork3C *work;

    work = func_002CFEB8(0x3C);
    work->resource = effCloneBlurTemplate(&params->unk18);
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
    sdfReleaseChipBlock(work);
}

typedef struct EffPCPLerpWorkB {
    f32 pos[4];
    u8 fixed;
    u8 pad11[3];
    u32 colorFrom;
    u32 colorTo;
    s32 frame;
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
    s32 base;
    s32 target;
    u8 pad34[4];
    EffPCPTrailObj *obj;
} EffPCPLerpWorkB;

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B820);

void func_0017B9C8(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetLongCompactColor(EffPCPWork *work, u32 val) {
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

void effPcpCreateFadeTimerFromTable(void *args) {
    void *param0;

    param0 = effParamTableGetBlock(args, 0);
    effPcpCopyWork(param0);
}

void func_0017BA98(void) {
    effPcpCopyWork();
}

void effPcpReleaseFadeTimerWork(EffPCPWork *work) {
    sdfReleaseChipBlock(work);
}

typedef struct EffPCPFadeTimer {
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
    u32 color;
    u8 pad10[4];
    u32 unk14;
    u32 unk18;
    u32 width;  /* 0x200: PS2 render width */
    u32 height; /* 0x1C0: PS2 render height */
    u32 colorFrom;
    u32 colorTo;
    s32 frame;
} EffPCPFadeTimer;

extern void func_00187C08(u32 *color);

/* Timeline update: blend factor ramps up over fadeIn frames, holds at 1.0, then
 * ramps down over the last fadeOut frames. */
void effPcpFadeTimerUpdate(EffPCPFadeTimer *work) {
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
    work->width = 0x200;
    work->height = 0x1C0;
    fadeOut = work->fadeOut;
    if (frame < fadeIn && fadeIn != 0) {
        t = (f32)frame / (f32)fadeIn;
    } else if (duration - frame <= fadeOut && fadeOut != 0) {
        t = (f32)(duration - frame) / (f32)fadeOut;
    } else {
        t = 1.0f;
    }
    work->color = effMultiplyPackedColors(effBlendColor(work->colorFrom & 0xFFFFFF, work->colorFrom, t), work->colorTo);
    func_00187C08(&work->color);
    work->frame++;
}

void effPcpFadeTimerSetSourceColor(EffPCPFadeTimer *work, u32 val) {
    work->colorFrom = val;
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

void effPcpCreateLongFadeTimerFromTable(void *args) {
    void *param0;

    param0 = effParamTableGetBlock(args, 0);
    effPcpCopyWorkLong(param0);
}

void func_0017BCB0(void) {
    effPcpCopyWorkLong();
}

void effPcpReleaseLongFadeTimerWork(EffPCPWork *work) {
    sdfReleaseChipBlock(work);
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
    u32 width;  /* 0x200: PS2 render width */
    u32 height; /* 0x1C0: PS2 render height */
    u32 colorFrom;
    u32 colorTo;
    s32 frame;
} EffPCPFadeTimerLong;

extern void effDrawBlurRectangle(u32 *color);

/* Same timeline as effPcpFadeTimerUpdate on the longer work layout. */
void effPcpFadeTimerLongUpdate(EffPCPFadeTimerLong *work) {
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
    work->width = 0x200;
    work->height = 0x1C0;
    fadeOut = work->fadeOut;
    if (frame < fadeIn && fadeIn != 0) {
        t = (f32)frame / (f32)fadeIn;
    } else if (duration - frame <= fadeOut && fadeOut != 0) {
        t = (f32)(duration - frame) / (f32)fadeOut;
    } else {
        t = 1.0f;
    }
    work->color = effMultiplyPackedColors(effBlendColor(work->colorFrom & 0xFFFFFF, work->colorFrom, t), work->colorTo);
    effDrawBlurRectangle(&work->color);
    work->frame++;
}

void effPcpLongFadeTimerSetSourceColor(EffPCPFadeTimerLong *work, u32 val) {
    work->colorFrom = val;
}

EffPCPCompactWork *effPcpCreateCompactWorkFromParams(EffPCPCompactParams *params) {
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

void effPcpCreateChargeWorkFromTable(void *args) {
    void *param0;

    param0 = effParamTableGetBlock(args, 0);
    effPcpCreateCompactWorkFromParams(param0);
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
    effPcpCreateCompactWorkFromParams((EffPCPCompactParams *)&params);
}

void effPcpReleaseCompactBlurWork(EffPCPWork *work) {
    effBlurReleaseFirstResource(work->unk34);
    sdfReleaseChipBlock(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017BF90);

void effPcpCopyCompactBlurVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpCompactSetColor(EffPCPWork *work, u32 val) {
    work->unk20 = val;
}

EffPCPCompactWork *effPcpCreateCompactResourceWork(EffPCPCompactParams *params) {
    EffPCPCompactWork *work;

    work = func_002CFEB8(0x38);
    work->resource = effCloneBlurWorkWithSlots(&params->unk18);
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

void effPcpCreateChargeResourceWorkFromTable(void *args) {
    void *param0;

    param0 = effParamTableGetBlock(args, 0);
    effPcpCreateCompactResourceWork(param0);
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
    effPcpCreateCompactResourceWork((EffPCPCompactParams *)&params);
}

void effPcpReleaseSecondaryBlurWork(EffPCPWork *work) {
    effBlurReleaseSecondResource(work->unk34);
    sdfReleaseChipBlock(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017C300);

void effPcpCopyShortThunderFadeVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0017C500(EffPCPWork *work, u32 val) {
    work->unk20 = val;
}

void effPcpInitShortThunderFadeWork(EffPCPWork *work) {
    u32 handle;

    handle = func_001632E0(D_00355108);
    work->unk18 = 0;
    work->unk1C = handle;
}

void *effPcpCreateShortThunderFadeWork(void) {
    void *work;

    work = func_002CFEB8(0x20);
    effPcpInitShortThunderFadeWork(work);
    return work;
}

void effPcpReleaseShortThunderFadeWork(EffPCPWork *work) {
    effPCPThunderFree(work->unk1C);
    sdfReleaseChipBlock(work);
}

void *effAllocateInitializedThunderFadeWork(void) {
    void *work;

    work = func_002CFEB8(0x20);
    effPcpInitShortThunderFadeWork(work);
    return work;
}

void effPcpFadeUpdateShort(EffPCPFadeWork *work) {
    EffPCPFadeTarget *target;
    f32 phase;
    f32 value;
    u32 fadedColor;

    if (work->frame < 0x1F) {
        target = func_00163540(work->handle);
        phase = (f32)work->frame / 30.0f;
        value = phase * 150.0f + 100.0f;
        target->sizeX = value;
        target->sizeY = value;
    } else if (work->frame - 0x2D < 0x10) {
        target = func_00163540(work->handle);
        phase = (f32)(work->frame - 0x2D) / 15.0f;
        value = phase * 150.0f + 250.0f;
        target->sizeX = value;
        target->sizeY = value;
    }
    if (work->frame >= 0x2D) {
        phase = (f32)(work->frame - 0x2D) / 35.0f;
        fadedColor = effBlendColor(work->color, 0, phase);
    } else {
        fadedColor = work->color;
    }
    effPCPThunderSetParam50(work->handle, fadedColor);
    func_00163508(work->handle, work);
    func_00163CD0(work->handle);
    work->frame++;
}

void effPcpCopyScalingThunderFadeVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0017C7A8(EffPCPWorkF10 *work, f32 val) {
    work->unk10 = val;
}

void effPcpSetScalingThunderFadeColor(EffPCPWork *work, u32 val) {
    work->unk14 = val;
}

void effPcpInitScalingThunderFadeWork(EffPCPWork *work) {
    u32 handle;

    handle = func_001632E0(D_00355158);
    work->unk18 = 0;
    work->unk1C = handle;
}

void *effPcpCreateScalingThunderFadeWork(void) {
    void *work;

    work = func_002CFEB8(0x20);
    effPcpInitScalingThunderFadeWork(work);
    return work;
}

void effPcpReleaseScalingThunderFadeWork(EffPCPWork *work) {
    effPCPThunderFree(work->unk1C);
    sdfReleaseChipBlock(work);
}

void *effAllocateScalingThunderFadeWork(void) {
    void *work;

    work = func_002CFEB8(0x20);
    effPcpInitScalingThunderFadeWork(work);
    return work;
}

/* Scales the target through two frame windows and fades the colour out from
 * frame 60. */
void effPcpThunderScaleFadeUpdate(EffPCPFadeWork *work) {
    EffPCPFadeTarget *target;
    f32 phase;
    u32 fadedColor;

    if (work->frame < 0x2E) {
        target = func_00163540(work->handle);
        phase = (f32)work->frame / 45.0f;
        target->sizeY = target->sizeX = phase * 950.0f + 200.0f;
    } else if (work->frame >= 0x3C && work->frame < 0x4C) {
        target = func_00163540(work->handle);
        phase = (f32)(work->frame - 0x3C) / 15.0f;
        target->sizeY = target->sizeX = phase * 150.0f + 1050.0f;
    }
    if (work->frame >= 0x3C) {
        phase = (f32)(work->frame - 0x3C) / 15.0f;
        fadedColor = effBlendColor(work->color, 0, phase);
    } else {
        fadedColor = work->color;
    }
    effPCPThunderSetParam50(work->handle, fadedColor);
    func_00163508(work->handle, work);
    func_00163CD0(work->handle);
    work->frame++;
}

void effPcpCopyThunderScaleFadeVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0017CA50(EffPCPWorkF10 *work, f32 val) {
    work->unk10 = val;
}

void effPcpSetThunderScaleFadeColor(EffPCPWork *work, u32 val) {
    work->unk14 = val;
}

EffPCPSpanWork *effPcpSpanCreate(void *params, void *handleParams) {
    EffPCPSpanParams *src = params;
    EffPCPSpanWork *work;
    f32 size;

    work = func_002CFEB8(0x78);
    work->params = *src;
    work->color = 0x80808080;
    work->unk68 = 0;
    size = work->params.head.scale * 0.017453292f;
    work->startAngle = -(size * 0.5f);
    work->angleStep = size / (f32)(work->params.head.right - ((work->params.head.width >> 1) + work->params.head.left));
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->optionalHandle = 0;
    if (handleParams != NULL) {
        work->optionalHandle = func_0014FD20((u32)handleParams);
    }
    return work;
}

void effPcpSpanCreateFromTable(void *args) {
    void *param0;
    void *param1;

    param0 = effParamTableGetBlock(args, 0);
    param1 = effParamTableGetBlock(args, 1);
    effPcpSpanCreate(param0, param1);
}

EffPCPSpanWork *effPcpCloneWithOptionalHandle(EffPCPWork *work) {
    EffPCPSpanWork *child;
    u32 handle;

    child = effPcpSpanCreate(&((EffPCPSpanWork *)work)->params, NULL);
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
    sdfReleaseChipBlock(work);
}

typedef struct EffPCPAimParams {
    u8 vec[0x10];          /* 0x00: anchor position */
    u32 holdFrames;        /* 0x10 */
    u32 rampOut;           /* 0x14 */
    s32 rampIn;            /* 0x18 */
    f32 aimOffset;         /* 0x1C: degrees subtracted from the aim angle */
    f32 radius;            /* 0x20 */
} EffPCPAimParams;

typedef struct EffPCPAimWork {
    u8 mtx[0x40];          /* 0x00 */
    EffPCPAimParams params; /* 0x40 */
    u32 color;             /* 0x64 */
    u32 frame;             /* 0x68 */
    f32 angle;             /* 0x6C */
    f32 spin;              /* 0x70 */
    s32 node;              /* 0x74 */
} EffPCPAimWork;

typedef struct EffPCPAimBattle {
    u8 pad00[0x110];
    u32 flags;             /* 0x110 */
} EffPCPAimBattle;

extern f32 func_001F66D8(u32 mask, f32 *maxTop, f32 *minTop);
extern u32 effBTLFieldColorGetOriginalSelector(void);
extern f32 sdfAtan2(f32 y, f32 x);
extern void func_0014FBF0(s32 node, f32 *pos);
extern void func_0014FC28(s32 node, void *mtx);
extern void func_0014FC60(s32 node, u32 color);
extern void effUpdateNode(s32 node);

/* Per-frame update of the aimed beam: on its first frame, turn toward the battle group's centre; then place the node at a radius around the anchor, rotate it and fade by the ramp-in. */
void func_0017CC60(EffPCPAimWork *work) {
    EffPCPAimParams *params = &work->params;
    s32 node = work->node;
    s32 rampIn = params->rampIn;
    u32 end = rampIn + params->rampOut;
    u32 frame = work->frame;
    s32 remaining = end - frame;
    f32 mtx[16];
    f32 muzzle[4];
    f32 aim[4];
    f32 center[4];
    f32 look[4];
    f32 t;

    if (end < frame) {
        return;
    }
    if (func_001619E8() && work->frame == 0) {
        func_001F66D8(((EffPCPAimBattle *)effBTLFieldColorGetVariantSelector())->flags & 0x600, NULL, NULL);
        VU0_STORE_VF(vf10, center);
        btlUnitGetMuzzlePosVU((void *)effBTLFieldColorGetOriginalSelector());
        VU0_STORE_VF(vf10, muzzle);
        look[0] = center[0] - muzzle[0];
        look[2] = center[2] - muzzle[2];
        work->angle = sdfAtan2(look[0], look[2]) - params->aimOffset * EFF_DEG2RAD * 0.5f;
    }
    if (node != 0) {
        VU0_LOAD_MATRIX(work);
        func_002DD968(work->angle);
        sdfComposeVuMatrixFromRegisters();
        aim[0] = aim[1] = 0.0f;
        aim[2] = 1.0f;
        VU0_LOAD_VF(vf10, aim);
        VU0_ROTATE_VEC(vf10, vf10);
        VEC3_SPLAT(muzzle, params->radius);
        VU0_LOAD_VF(vf11, muzzle);
        VU0_MUL(vf10, vf10, vf11);
        VU0_LOAD_VF(vf11, params->vec);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, muzzle);
        func_0014FBF0(node, muzzle);
        if (work->frame > params->holdFrames) {
            work->angle += work->spin;
        }
        func_002DD688(work->angle);
        VU0_STORE_MATRIX(mtx);
        func_0014FC28(node, mtx);
        if (rampIn >= remaining && rampIn != 0) {
            t = (f32)remaining / (f32)rampIn;
        } else {
            t = 1.0f;
        }
        func_0014FC60(node, effBlendColor(work->color & 0xFFFFFF, work->color, t));
        effUpdateNode(node);
    }
    work->frame++;
}

void func_0017CEB8(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0x40, src);
}

void func_0017CED0(EffPCPWork *work, u32 val) {
    work->unk64 = val;
}

void effPcpCopyHalfTurnMatrix(void *dst, void *src) {
    func_002DD688(3.1415927f);
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf24, 0(%0)\n"
        "lqc2 vf25, 0x10(%0)\n"
        "lqc2 vf26, 0x20(%0)\n"
        "lqc2 vf27, 0x30(%0)\n"
        ".set reorder"
        : : "r"(src) : "memory");
    sdfComposeVuMatrixFromRegisters();
    VU0_STORE_MATRIX(dst);
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

void effPcpTripleHandleCreateFromTable(void *data) {
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

void effPcpTripleHandleRelease(EffPCPWork *work) {
    s32 *handle;
    u32 i;

    handle = (s32 *)&work->unk58;
    for (i = 0; i < 7; i++) {
        effDestroyNode(handle[14]);
        effDestroyNode(handle[7]);
        effDestroyNode(handle[0]);
        handle++;
    }
    sdfReleaseChipBlock(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017D2F8);

void effPcpCopySpanVector(void *dst, void *src) {
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
EffPCPBlockCloneWork *effPcpBlockSetCloneShared(EffPCPBlockCloneWork *src) {
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

void effPcpBlockSetWorkRelease(EffPCPBlockSetWork *work) {
    u32 i;
    u32 j;
    u32 n;

    if (work->shared == 0) {
        func_001629F0(work->headHandle);
        for (j = 0; j < 5; j++) {
            func_001629F0(work->handleA[j]);
        }
        for (i = 0; i < 3; i++) {
            if (work->alloc[i] != 0) {
                n = work->count * work->groupSize[i];
                for (j = 0; j < n; j++) {
                    if (work->list[i][j] != 0) {
                        func_001629F0(work->list[i][j]);
                    }
                }
                func_002D0918(work->alloc[i]);
            }
        }
        for (j = 0; j < 5; j++) {
            func_001629F0(work->handleB[j]);
        }
        func_001629F0(work->tailHandle);
    }
    sdfReleaseChipBlock(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017DCF8);

void effPcpCopyVector60(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0x60, src);
}

void func_0017E4C0(EffPCPWork *work, u32 val) {
    work->unkB8 = val;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effPcpCopyBlockMatrix(void *dst, void *src) {
    VU0_COPY_MATRIX(dst, src);
}

void func_0017E4F0(void) {
    EffPCPWork *work;

    work = effPcpBuildBlockSet();
    work->mode = 1;
}

EffPCPBlockCloneWork *effPcpCloneBlockWithUnitMatrix(EffPCPBlockCloneWork *src) {
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

EffPCPBlockCloneWork *effCloneBlockWorkFromSource(EffPCPBlockCloneWork *src) {
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
void effPcpRotateCreateFromTable(args)
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
EffPCPRotateWork *effPcpRotateClone(EffPCPRotateWork *src) {
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

void effPcpRotateRelease(EffPCPRotateWork *work) {
    u32 i;
    s32 *id;

    id = work->ids;
    for (i = 0; i < 3; i++) {
        effPcpBlockSetWorkRelease((EffPCPBlockSetWork *)id[i]);
    }
    sdfReleaseChipBlock(work);
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

void effPcpRotateSetChildVectors(EffPCPRotateWork *work, void *src) {
    u32 i;
    s32 *id;

    id = work->ids;
    for (i = 0; i < 3; i++) {
        effPcpCopyVector60((void *)id[i], src);
    }
}

void effPcpRotateSetChildValues(EffPCPRotateWork *work, u32 val) {
    u32 i;
    s32 *id;

    id = work->ids;
    for (i = 0; i < 3; i++) {
        func_0017E4C0((EffPCPWork *)id[i], val);
    }
}

void effPcpRotateSetChildMatrices(EffPCPRotateWork *work, void *src) {
    u32 i;
    s32 *id;

    id = work->ids;
    for (i = 0; i < 3; i++) {
        effPcpCopyBlockMatrix((void *)id[i], src);
    }
}

extern f32 effMiscRandUnitFloat(void *state);
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

EffPCPSpinWork *effSpinSingleCreateFromTable(void *src) {
    EffPCPSpinWork *work = func_002CFEB8(0x64);

    work->frame = 0;
    work->scale = 1.0f;
    work->color = 0x80808080;
    work->handle0 = (void *)effParamCreateFromTable(src, 0);
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->angle = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f * 0.87266457f;
    if (effMiscRand(D_0034DF38) & 1) {
        work->angle += 3.14159265f;
    }
    return work;
}

/* Single-handle variant: allocation stops before handle1 (0x64 bytes). */
EffPCPSpinWork *effSpinSingleClone(EffPCPSpinWork *src) {
    EffPCPSpinWork *work = func_002CFEB8(0x64);

    work->frame = 0;
    work->scale = 1.0f;
    work->color = 0x80808080;
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->angle = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f * 0.87266457f;
    if (effMiscRand(D_0034DF38) & 1) {
        work->angle += 3.14159265f;
    }
    work->handle0 = (void *)effParamWorkDuplicate((u32)src->handle0);
    return work;
}

void effSpinSingleRelease(EffPCPWork *work) {
    func_001629F0(work->unk60);
    sdfReleaseChipBlock(work);
}

extern void effParamWorkCallback2(u32 handle, void *mtx);

void effSpinEffectUpdate(EffPCPSpinWork *work) {
    u32 handle = (u32)work->handle0;
    u128 mtx[4];

    effParamWorkCallback0(handle, work);
    effParamWorkCallback1(handle, work->scale);
    effParamWorkCallback3(handle, work->color);
    func_002DD688(work->angle);
    VU0_LOAD_MATRIX_B(work->matrix);
    sdfComposeVuMatrixFromRegisters();
    VU0_STORE_MATRIX(mtx);
    effParamWorkCallback2(handle, mtx);
    effParamWorkInvokeCallback(handle);
    work->frame++;
}

void effPcpCopySpinSingleVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effSpinSingleSetColor(EffPCPWork *work, u32 val) {
    work->unk5C = val;
}

void effSpinSingleSetScale(EffPCPWork *work, f32 val) {
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
    work->angle = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f * 0.87266457f;
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
    work->angle = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f * 0.87266457f;
    return work;
}

void effSpinPairRelease(EffPCPWork *work) {
    func_001629F0(work->unk64);
    func_001629F0(work->unk60);
    sdfReleaseChipBlock(work);
}

/* Per-frame update of a two-handle spin effect: pushes the work state to both
 * handles, rebuilds the rotation matrix from the angle and hands it over. The
 * VU0 blocks are bare asm (no "memory" clobber), as the original macros were. */
void effSpinPairUpdateDelayed(EffPCPSpinWork *work) {
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
    VU0_LOAD_MATRIX_B(work->matrix);
    sdfComposeVuMatrixFromRegisters();
    VU0_STORE_MATRIX_UNCLOBBERED(mtx);
    effParamWorkCallback2(handle[0], mtx);
    effParamWorkCallback2(handle[1], mtx);
    effParamWorkInvokeCallback(handle[0]);
    if (work->frame >= 0x1F) {
        effParamWorkInvokeCallback(handle[1]);
    }
    work->frame++;
}

void func_0017F4C0(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effSpinPairSetColor(EffPCPWork *work, u32 val) {
    work->unk5C = val;
}

void effSpinPairSetScale(EffPCPWork *work, f32 val) {
    work->unk58 = val;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effSpinPairSetMatrix(EffPCPWork *work, void *src) {
    VU0_LOAD_MATRIX(src);
    VU0_STORE_MATRIX(&work->unk10);
}

EffPCPSpinWork *effPcpCreateSpinningPair(void *src) {
    EffPCPSpinWork *work = func_002CFEB8(0x68);

    work->frame = 0;
    work->scale = 1.0f;
    work->color = 0x80808080;
    work->handle0 = (void *)effParamCreateFromTable(src, 0);
    work->handle1 = (void *)effParamCreateFromTable(src, 1);
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->angle = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f * 0.87266457f;
    return work;
}

EffPCPSpinWork *effPcpCloneSpinningPair(EffPCPSpinWork *src) {
    EffPCPSpinWork *work = func_002CFEB8(0x68);

    work->frame = 0;
    work->scale = 1.0f;
    work->color = 0x80808080;
    work->handle0 = (void *)effParamWorkDuplicate((u32)src->handle0);
    work->handle1 = (void *)effParamWorkDuplicate((u32)src->handle1);
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->angle = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f * 0.87266457f;
    return work;
}

void effPcpReleaseSpinningPair(EffPCPWork *work) {
    func_001629F0(work->unk64);
    func_001629F0(work->unk60);
    sdfReleaseChipBlock(work);
}

/* Identical update for the second spin effect. */
void effPcpUpdateSpinningPair(EffPCPSpinWork *work) {
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
    VU0_LOAD_MATRIX_B(work->matrix);
    sdfComposeVuMatrixFromRegisters();
    VU0_STORE_MATRIX_UNCLOBBERED(mtx);
    effParamWorkCallback2(handle[0], mtx);
    effParamWorkCallback2(handle[1], mtx);
    effParamWorkInvokeCallback(handle[0]);
    if (work->frame >= 0x1F) {
        effParamWorkInvokeCallback(handle[1]);
    }
    work->frame++;
}

void func_0017F7C0(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetSpinColor(EffPCPWork *work, u32 val) {
    work->unk5C = val;
}

void effPcpSetSpinScale(EffPCPWork *work, f32 val) {
    work->unk58 = val;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effPcpSetSpinMatrix(EffPCPWork *work, void *src) {
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

void effPcpClearGlobalEffectFlag(void) {
    if (D_003BB04C != 0) {
        D_003BB04C = 0;
    }
}

EffPCPWorkF1C *effAllocateThunderFragmentWork(u32 unused) {
    EffPCPWorkF1C *work;

    work = func_002CFEB8(0x24);
    work->unk18 = 0;
    work->unk1C = 375.0f;
    work->unk20 = effThunderFragCreate(D_003551C0);
    return work;
}

void func_0017F898(void) {
    effAllocateThunderFragmentWork(0);
}

EffPCPBurstWork *effAllocateThunderFragmentBurstWork(u32 unused) {
    EffPCPBurstWork *work;

    work = func_002CFEB8(0x24);
    work->unk18 = 325.0f;
    work->unk1C = 225.0f;
    work->unk20 = effThunderFragCreate(D_00355220);
    return work;
}

void func_0017F900(void) {
    effAllocateThunderFragmentBurstWork(0);
}

EffPCPWorkF1C *func_0017F918(u32 unused) {
    EffPCPWorkF1C *work;

    work = func_002CFEB8(0x24);
    work->unk18 = 0;
    work->unk1C = 375.0f;
    work->unk20 = effThunderFragCreate(D_00355280);
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
    work->unk20 = effThunderFragCreate(D_003552E0);
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
    work->unk20 = effThunderFragCreate(D_00355340);
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
    work->unk20 = effThunderFragCreate(D_003553A0);
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
    work->unk20 = effThunderFragCreate(D_00355400);
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
    work->unk20 = effThunderFragCreate(D_00355460);
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
    work->unk20 = effThunderFragCreate(D_003554C0);
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
    work->unk20 = effThunderFragCreate(D_00355520);
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
    work->unk20 = effThunderFragCreate(D_00355580);
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
    work->unk20 = effThunderFragCreate(D_003555E0);
    return work;
}

void func_0017FCE8(void) {
    func_0017FC98(0);
}

void effPcpThunderBurstRelease(EffPCPWork *work) {
    effPCPThunderFree3(work->unk20);
    sdfReleaseChipBlock(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FD30);

void effPcpCopyThunderBurstVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0017FE48(EffPCPWork *work, u32 val) {
    work->unk10 = val;
}

typedef struct {
    f32 pos[3];
    u32 word0C[4];
    f32 scaleA;
    f32 scaleB;
    u32 word24[10];
} EffPCPRes76;

typedef struct {
    u32 unk00;
    u32 unk04;
    u32 unk08;
    s32 unk0C;
    u32 unk10;
    EffPCPRes76 res;
} EffPCPParams76;

typedef struct {
    f32 pos[3];
    u8 pad0C[4];
    u32 unk10;
    u32 unk14;
    u32 unk18;
    u32 unk1C;
    u32 unk20;
    u32 color;
    u32 unk28;
    f32 scale;
    u32 handle;
} EffPCPScaledWork;

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FE50);

void effPcpScaledEffectCreateFromTable(void *args) {
    void *param0;

    param0 = effParamTableGetBlock(args, 0);
    func_0017FE50(param0);
}


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

void effPcpScaledEffectRelease(EffPCPWork *work) {
    effPCPThunderFree(work->unk30);
    sdfReleaseChipBlock(work);
}

typedef struct EffPCPGrowWork {
    u8 pad00[0x10];
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
    s32 base;
    s32 target;
    u32 color;
    s32 frame;
    f32 scale;
    u32 handle;
} EffPCPGrowWork;

extern void effPCPThunderScale(u32 handle, f32 scale);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180080);

void effPcpCopyScaledThunderVector(void *dst, void *src) {
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
    sdfReleaseChipBlock(work);
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

/* The clone copies the 0x50-byte parameter prefix, then attaches a node whose
 * entries receive three colors from that prefix. */
typedef struct EffPCPBeamWork {
    u8 pad00[0x30];
    u32 kind;              /* 0x30 */
    u32 nodeColor;         /* 0x34 */
    u8 pad38[0x04];
    u32 firstColor;        /* 0x3C */
    u8 pad40[0x04];
    u32 middleColor;       /* 0x44 */
    u8 pad48[0x04];
    u32 lastColor;         /* 0x4C */
    u32 state;             /* 0x50 */
    u32 color;             /* 0x54 */
    u32 colorCount;        /* 0x58 */
    struct EffPCPBeamColorNode *node; /* 0x5C */
} EffPCPBeamWork;

typedef struct EffPCPBeamColorNode {
    u8 pad00[0x94];
    u32 color;             /* 0x94 */
    u8 pad98[0x04];
    u32 colorCount;        /* 0x9C */
    u8 padA0[0x04];
    u32 *colors;           /* 0xA4 */
} EffPCPBeamColorNode;

extern u8 *func_001801F8(u32);

u8 *effBeamEffectClone(src)
    EffPCPBeamWork *src;
{
    EffPCPBeamWork *work = func_002CFEB8(0x60);
    u32 kind;
    EffPCPBeamColorNode *node;
    u32 *entry;
    s32 groups;
    u32 i;

    *(EffPCPBlock50 *)work = *(EffPCPBlock50 *)src;
    work->color = 0x80808080;
    work->state = 0;
    kind = src->kind;
    if (kind < 3) {
        src->kind = 3;
        kind = 3;
    }
    node = (EffPCPBeamColorNode *)func_001801F8(kind);
    i = 0;
    work->node = node;
    work->colorCount = node->colorCount;
    groups = (s32)node->colorCount >> 2;
    entry = node->colors;
    for (i = 0; i < groups; i++) {
        u32 second;

        entry[0] = src->firstColor;
        second = src->middleColor;
        entry[1] = entry[2] = second;
        entry[3] = src->lastColor;
        entry += 4;
    }
    effResetChild((EffPCPSubEffectWork *)work);
    work->node->color = src->nodeColor;
    return (u8 *)work;
}

void effPcpBeamCreateFromTable(void *args) {
    void *param0;

    param0 = effParamTableGetBlock(args, 0);
    effBeamEffectClone(param0);
}

void func_001808B0(void) {
    effBeamEffectClone();
}

void effPcpReleaseBeamClone(EffPCPWork *work) {
    effPcpReleaseNestedWork((EffPCPWork *)work->unk5C);
    sdfReleaseChipBlock(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001808F8);

void effPcpCopyBeamVector(void *dst, void *src) {
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
    VU0_LOAD_MATRIX(src);
    func_002DD8E8(1.5707963f);
    sdfComposeVuMatrixFromRegisters();
    VU0_STORE_MATRIX((void *)work->unk5C);
}

typedef struct EffPCPRingGeoNode {
    u8 pad00[0x9C];
    s32 size;            /* 0x9C: four words per segment */
    f32 *points;         /* 0xA0 */
} EffPCPRingGeoNode;

typedef struct EffPCPRingGeoWork {
    u8 pad00[0x1C];
    f32 decay;           /* 0x1C */
    u8 mode;             /* 0x20: 0 = camera-facing ring, else rotated about the work's own axes */
    u8 pad21[0x1B];
    u32 segments;        /* 0x3C */
    u8 pad40[4];
    f32 radiusStepA;     /* 0x44 */
    u8 pad48[4];
    f32 radiusStepB;     /* 0x4C */
    u8 pad50[4];
    f32 radiusStepC;     /* 0x54 */
    u8 pad58[0xC];
    f32 rotY;            /* 0x64 */
    f32 rotX;            /* 0x68 */
    f32 spin;            /* 0x6C */
    u8 pad70[0xC];
    EffPCPRingGeoNode *node; /* 0x7C */
} EffPCPRingGeoWork;

extern u8 D_003246A0[];
extern void sdfVuBuildLookAtBasis(void *, void *, void *);
extern void sdfInvertRigidVuTransform(void);

/* Fill the node's point buffer with four concentric rings (radius, +stepA, +stepB, +stepC) of unit directions, rotated by the VU matrix. */
void func_00180BD0(void *obj, f32 radius) {
    EffPCPRingGeoWork *work = obj;
    f32 dir[4];
    f32 rowA[4];
    f32 rowB[4];
    f32 rowC[4];
    f32 rowD[4];
    f32 *points;
    f32 angle = 0.0f;
    f32 step;
    f32 r1;
    f32 r2;
    f32 r3;
    s32 n;
    u32 i;

    VEC3_SPLAT(rowA, radius);
    r1 = radius + work->radiusStepA;
    VEC3_SPLAT(rowB, r1);
    r2 = r1 + work->radiusStepB;
    VEC3_SPLAT(rowC, r2);
    r3 = r2 + work->radiusStepC;
    VEC3_SPLAT(rowD, r3);
    n = work->node->size >> 2;
    points = work->node->points;
    step = 6.2831851f / (f32)work->segments;
    if (work->mode == 0) {
        sdfVuBuildLookAtBasis(D_00324680, D_00324690, D_003246A0);
        sdfInvertRigidVuTransform();
    } else {
        func_002DD708(work->rotY);
        func_002DD968(work->rotX);
        sdfMultiplyVuMatrixInPlace();
        work->rotX += work->spin;
        work->spin *= work->decay;
    }
    for (i = 0; i < n; i++) {
        if (work->mode == 0) {
            dir[0] = sdfEvaluateCosineViaSinePhaseShift(angle);
            dir[1] = sdfSinPoly(angle);
            dir[2] = 0;
        } else {
            dir[0] = sdfEvaluateCosineViaSinePhaseShift(angle);
            dir[1] = 0;
            dir[2] = sdfSinPoly(angle);
        }
        dir[3] = 0;
        VU0_LOAD_VF(vf10, dir);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_MOVE_VF(vf11, vf10);
        VU0_LOAD_VF(vf10, rowA);
        VU0_MUL(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, points);
        VU0_LOAD_VF(vf10, rowB);
        VU0_MUL(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, points + 4);
        VU0_LOAD_VF(vf10, rowC);
        VU0_MUL(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, points + 8);
        VU0_LOAD_VF(vf10, rowD);
        VU0_MUL(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, points + 12);
        angle += step;
        points += 16;
    }
}

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
    func_00180BD0(work, work->angle);
    work->child = 0;
}

typedef struct EffPCPBlock5C {
    u32 word[23];
} EffPCPBlock5C;

typedef struct EffPCPBeamLargeWork {
    u8 pad00[0x3C];
    u32 kind;               /* 0x3C */
    u32 nodeColor;          /* 0x40 */
    u8 pad44[0x04];
    u32 firstColor;         /* 0x48 */
    u8 pad4C[0x04];
    u32 middleColor;        /* 0x50 */
    u8 pad54[0x04];
    u32 lastColor;          /* 0x58 */
    u32 state;              /* 0x5C */
    u32 color;              /* 0x60 */
    u8 pad64[0x18];
    EffPCPBeamColorNode *node; /* 0x7C */
} EffPCPBeamLargeWork;

u8 *effBeamEffectCloneLarge(src)
    EffPCPBeamLargeWork *src;
{
    EffPCPBeamLargeWork *work = func_002CFEB8(0x80);
    u32 kind;
    EffPCPBeamColorNode *node;
    u32 *entry;
    s32 groups;
    u32 i;

    *(EffPCPBlock5C *)work = *(EffPCPBlock5C *)src;
    work->color = 0x80808080;
    work->state = 0;
    kind = src->kind;
    if (kind < 3) {
        src->kind = 3;
        kind = 3;
    }
    node = (EffPCPBeamColorNode *)func_001801F8(kind);
    i = 0;
    work->node = node;
    groups = (s32)node->colorCount >> 2;
    entry = node->colors;
    for (i = 0; i < groups; i++) {
        u32 second;

        entry[0] = src->firstColor;
        second = src->middleColor;
        entry[1] = entry[2] = second;
        entry[3] = src->lastColor;
        entry += 4;
    }
    effPrepareAngles((EffPCPAngleWork *)work);
    work->node->color = src->nodeColor;
    return (u8 *)work;
}

void effPcpBeamLargeCreateFromTable(void *args) {
    void *param0;

    param0 = effParamTableGetBlock(args, 0);
    effBeamEffectCloneLarge(param0);
}

void func_00181040(void) {
    effBeamEffectCloneLarge();
}

void effPcpReleaseLinkedBeamClone(EffPCPWork *work) {
    effPcpReleaseNestedWork((EffPCPWork *)work->unk7C);
    sdfReleaseChipBlock(work);
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
void effPcpUpdateBeamTimeline(EffPCPBeamTimer *work) {
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
    func_00180BD0(work, work->angle);
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

void effPcpCopyLinkedBeamVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpLinkedWorkSetFloat(EffPCPWork *work, f32 value) {
    ((EffPCPWork *)work->unk7C)->unk90 = value;
}

void func_001811D0(EffPCPWork *work, u32 val) {
    work->unk60 = val;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effPcpCopyLinkedBeamMatrix(EffPCPWork *work, void *src) {
    VU0_LOAD_MATRIX(src);
    VU0_STORE_MATRIX(&((EffPCPWork *)work->unk7C)->pad3C[4]);
}

/* Block-set parameter head (0x164 bytes, copied whole into each new work). */
typedef struct EffPCPGroupHead {
    u8 pad00[0x50];
    u8 unk50;            /* 0x50 */
    u8 pad51[3];
    s32 framesPerEntry;   /* 0x54: interpolation duration */
    u32 count;           /* 0x58: handles per group */
    s32 delaySpread;      /* 0x5C: random initial delay */
    u32 unk60;           /* 0x60 */
    u32 unk64;           /* 0x64 */
    f32 startPosition;    /* 0x68: interpolated position at frame zero */
    f32 endPosition;      /* 0x6C: interpolated position at final frame */
    f32 startJitter;      /* 0x70: fractional random variation */
    f32 endJitter;        /* 0x74: fractional random variation */
    f32 unk78;           /* 0x78 */
    f32 unk7C;           /* 0x7C */
    f32 unk80;           /* 0x80 */
    f32 unk84;           /* 0x84 */
    f32 unk88;           /* 0x88 */
    u8 activeGroups[4];  /* 0x8C */
    u8 spawnParams[4];    /* 0x90: beginning of the particle spawn template */
    f32 unk94;           /* 0x94 */
    u8 pad98[0xCC];
} EffPCPGroupHead;

typedef struct EffPCPGroupEntry {
    u32 handle;          /* 0x00 */
    s32 frame;            /* 0x04: negative until the start delay expires */
    f32 position;         /* 0x08: initial interpolated position */
    f32 positionStep;     /* 0x0C: change per frame */
    f32 angle;            /* 0x10: evenly spaced angle in radians */
    f32 unk14;           /* 0x14 */
} EffPCPGroupEntry;

typedef struct EffPCPGroupSet {
    EffPCPGroupHead head;
    EffPCPGroupEntry *entries;  /* 0x164 */
    f32 scale;            /* 0x168: multiplies start/end positions */
    f32 unk16C;
    u32 color;           /* 0x170 */
    u32 *duplicates;     /* 0x174: four groups of handles */
    void *duplicateHandle;
    void *workHandle;
} EffPCPGroupSet;

EffPCPGroupSet *effPcpGroupSetCreate(EffPCPGroupHead *first, u32 *blocks) {
    u32 count = first->count;
    void *resource = func_002D03F8(count * 0x18 + 0x180);
    EffPCPGroupSet *copy = sdfResourceRetainAddress(resource);
    EffPCPGroupEntry *entry;
    u32 g;
    u32 i;

    copy->head = *first;
    copy->scale = 1.0f;
    copy->color = 0x80808080;
    copy->workHandle = resource;
    copy->unk16C = first->unk94;
    entry = (EffPCPGroupEntry *)((u8 *)copy + 0x180);
    copy->entries = entry;
    copy->duplicates = 0;
    if (blocks != NULL) {
        u32 size = count * 16;
        u32 *list = blocks;
        u32 offset;
        u32 stride;
        u8 *flags;

        g = 0;
        copy->duplicateHandle = func_002D03F8(size);
        flags = first->activeGroups;
        offset = 0;
        stride = count * 4;
        copy->duplicates = sdfResourceRetainAddress(copy->duplicateHandle);
        memset(copy->duplicates, 0, size);
        for (; g < 4; g++) {
            u32 *slot = (u32 *)((u8 *)copy->duplicates + offset);

            if (*flags != 0) {
                u32 head;

                if (g < 2) {
                    *slot = effParamWorkCreate(0, (void *)*list);
                } else {
                    *slot = effParamWorkCreate(6, (void *)*list);
                }
                head = *slot;
                for (i = 1; i < count; i++) {
                    slot++;
                    *slot = effParamWorkDuplicate(head);
                }
            }
            list++;
            flags++;
            offset += stride;
        }
    }
    for (g = 0; g < count; g++) {
        entry->handle = effThunderFragCreate(first->spawnParams);
        entry->frame = 0;
        entry++;
    }
    return copy;
}

void effPcpGroupSetCreateFromTable(void *args) {
    EffPCPGroupHead *work;
    void *resources[4];
    u32 i;

    work = effParamTableGetBlock(args, 0);
    for (i = 0; i < 4; i++) {
        if (work->activeGroups[i] != 0) {
            resources[i] = effParamTableGetBlock(args, i + 1);
        } else {
            resources[i] = NULL;
        }
    }
    effPcpGroupSetCreate(work, resources);
}

u8 *effBlockSetCloneWithDuplicates(u8 *work) {
    u8 *copy = effPcpGroupSetCreate(work, 0);
    u32 group;
    u32 offset;
    u32 count;
    u32 size;
    u32 stride;
    u8 *flags;
    u32 i;

    if (((EffPCPBatchWork *)work)->duplicates != 0) {
        count = ((EffPCPBatchWork *)work)->count;
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
                u32 first = *(u32 *)(offset + (u32)((EffPCPBatchWork *)work)->duplicates);
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

void effBlockSetRelease(u8 *work) {
    u32 i = 0;
    u32 count = ((EffPCPBatchWork *)work)->count;
    u32 *entry = ((EffPCPBatchWork *)work)->entries;
    u32 *list;
    u32 *p;

    if (count != 0) {
        do {
            u32 handle = *entry;
            entry += 6;
            i++;
            effPCPThunderFree3(handle);
        } while (i < count);
    }
    list = ((EffPCPBatchWork *)work)->duplicates;
    count = count * 4;
    if (list != NULL) {
        p = list;
        i = 0;
        if (count != 0) {
            do {
                u32 handle = *p++;
                if (handle != 0) {
                    func_001629F0(handle);
                }
                i++;
            } while (i < count);
        }
        func_002D0918(((EffPCPBatchWork *)work)->duplicateHandle);
    }
    func_002D0918(((EffPCPBatchWork *)work)->workHandle);
}

/* Randomises entry `index` of a group set: a start delay, a jittered
 * position step per frame and a spread angle step. */
void effPcpGroupSetInitEntry(EffPCPGroupSet *work, s32 index) {
    EffPCPGroupEntry *entry = work->entries + index;
    s32 spread = work->head.delaySpread;
    f32 scale = work->scale;
    f32 ratio;

    if (spread > 0) {
        entry->frame = -(effMiscRand(D_0034DF38) % spread);
    }
    ratio = work->head.startJitter;
    entry->position = work->head.startPosition * (effMiscRandUnitFloat(D_0034DF38) * ratio + (1.0f - ratio)) * scale;
    ratio = work->head.endJitter;
    entry->positionStep = (work->head.endPosition * (effMiscRandUnitFloat(D_0034DF38) * ratio + (1.0f - ratio)) * scale - entry->position) / (f32)work->head.framesPerEntry;
    entry->angle = 3.14159265f * 2.0f / (f32)work->head.count * (f32)index;
    ratio = work->head.unk80;
    entry->unk14 = work->head.unk78 * (effMiscRandUnitFloat(D_0034DF38) * ratio + (1.0f - ratio));
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001818A8);

void func_00181C60(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetGroupScale(EffPCPGroupSet *work, f32 val) {
    work->scale = val;
}

void effPcpSetGroupColor(EffPCPGroupSet *work, u32 val) {
    work->color = val;
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
        work->angle[i] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f * 0.87266457f + 3.14159265f;
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
        work->angle[i] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f * 0.87266457f + 3.14159265f;
    }
    work->handle[0] = effParamWorkDuplicate(src->handle[0]);
    return work;
}

void effDestroyIndexedResources(EffPCPSprayWork *work) {
    u32 i = 0;

    if (work->count != 0) {
        u32 *handle = work->handle;
        do {
            if (*handle != 0) {
                func_001629F0(*handle);
            }
            handle++;
            i++;
        } while (i < work->count);
    }
    sdfReleaseChipBlock(work);
}

typedef struct EffPCPNode {
    u8 pad0[4];
    struct EffPCPNode *next;
    u8 pad8[4];
    struct EffPCPNode *child;
    u8 pad10[0x60];
    u8 vector70[0x10];
} EffPCPNode;

void effPcpCaptureNodeVectors(EffPCPNode *node) {
    EffPCPNode *child;

    VU0_STORE_VF(vf10, node->vector70);
    child = node->child;
    if (child != NULL) {
        do {
            effPcpCaptureNodeVectors(child);
            child = child->next;
        } while (child != node->child);
    }
}

typedef struct EffPCPPulseWork {
    u128 pos;               /* 0x00 */
    f32 scale;              /* 0x10 */
    u32 mask;               /* 0x14 */
    u32 count;              /* 0x18 */
    s32 frame;              /* 0x1C */
    s32 startFrame[10];     /* 0x20 */
    f32 rotY[10];           /* 0x48 */
    u32 handle[10];         /* 0x70 */
} EffPCPPulseWork;

typedef struct EffPCPPulseHead {
    u8 pad00[0xC];
    EffPCPNode **roots;     /* 0x0C */
} EffPCPPulseHead;

typedef struct EffPCPPulseModel {
    EffPCPPulseHead *head;  /* 0x00 */
} EffPCPPulseModel;

typedef struct EffPCPPulseChild {
    u8 pad00[0x30];
    u8 active;              /* 0x30 */
} EffPCPPulseChild;

typedef struct EffPCPPulseData {
    u32 flags;              /* 0x00 */
    u8 pad04[0x14];
    EffPCPPulseModel *model; /* 0x18 */
    u8 pad1C[4];
    EffPCPPulseChild *child[4]; /* 0x20 */
} EffPCPPulseData;

typedef struct EffPCPPulseBattle {
    u8 pad00[0x110];
    u32 flags;              /* 0x110 */
} EffPCPPulseBattle;

extern u8 D_003A0EF0[];
extern void func_002E7F20(f32 x, f32 y, f32 z);
extern void effMiscQuatMultiplyVU(void);
extern void mdlUpdateContextRotationBasisFromQuaternion(void *work);
extern void mdlStoreTertiaryVectorVU(void *work);
extern void sdfModelUpdateCurrentFrameTransforms(void *model);
extern void func_002D9238(void *table, void *model);
extern void func_002DB660(void *obj);

/* Per-frame update: for each of `count` slots, spawn its model on its start frame, orient/scale it, refresh its children and capture the node vectors. */
void func_00181F48(EffPCPPulseWork *work) {
    u32 i = 0;
    u32 count;
    EffPCPPulseData *data;
    EffPCPPulseModel *model;
    EffPCPPulseChild *child;
    EffPCPNode *root;
    f32 vec[4];
    s32 j;

    effBTLFieldColorGetVariantSelector();
    count = work->count;
    for (; i < count; i++) {
        if (work->startFrame[i] == work->frame && work->handle[i] == 0) {
            work->handle[i] = effParamWorkDuplicate(work->handle[0]);
        }
        if (work->handle[i] == 0) {
            continue;
        }
        data = effParamWorkGetData(work->handle[i]);
        func_002E7F20(0.0f, work->rotY[i], 0.0f);
        if (func_001619E8() && (((EffPCPPulseBattle *)effBTLFieldColorGetVariantSelector())->flags & 0x400)) {
            VU0_LOAD_VF(vf11, D_003A0EF0);
            effMiscQuatMultiplyVU();
        }
        mdlUpdateContextRotationBasisFromQuaternion(data);
        vec[3] = 0;
        VEC3_SPLAT(vec, work->scale * work->scale);
        VU0_LOAD_VF(vf10, vec);
        mdlStoreTertiaryVectorVU(data);
        mdlBroadcastMasked(data, work->mask);
        VU0_LOAD_VF(vf10, work);
        mdlStorePrimaryVectorVU(data);
        for (j = 0; j != 4; j++) {
            child = data->child[j];
            if (child != NULL && child->active) {
                func_002DB660(child);
            }
        }
        if (!(data->flags & 1)) {
            model = data->model;
            root = model->head->roots[0];
            VEC3_SPLAT(vec, 1.0f / work->scale);
            VU0_LOAD_VF(vf10, vec);
            effPcpCaptureNodeVectors(root);
            sdfModelUpdateCurrentFrameTransforms(model);
            func_002D9238(D_00325828, model);
        }
    }
    work->frame++;
}

void effPcpCopyCaptureNodeVector(void *dst, void *src) {
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

typedef struct EffPCPDriftSlot {
    u8 pad00[8];
    f32 position;
    f32 positionStep;
    f32 angle;
    f32 unk14;
} EffPCPDriftSlot;

typedef struct EffPCPDriftWork {
    u8 pad00[0x54];
    s32 count;
    u32 steps;
    u8 pad5C[0xC];
    f32 startPosition;
    f32 endPosition;
    f32 startJitter;
    f32 endJitter;
    f32 unk78;
    u8 pad7C[4];
    f32 unk80;
    u8 pad84[0x80];
    EffPCPDriftSlot *slots;
    u8 pad108[8];
    f32 scale;
} EffPCPDriftWork;

void effPcpDriftRandomizeSlot(EffPCPDriftWork *work, s32 index) {
    EffPCPDriftSlot *slot;
    f32 spread;
    f32 scale;

    slot = &work->slots[index];
    scale = work->scale;
    spread = work->startJitter;
    slot->position = work->startPosition * (effMiscRandUnitFloat(D_0034DF38) * spread + (1.0f - spread)) * scale;
    spread = work->endJitter;
    slot->positionStep = (work->endPosition * (effMiscRandUnitFloat(D_0034DF38) * spread + (1.0f - spread)) * scale - slot->position) / (f32)work->count;
    slot->angle = 6.2831852f / (f32)work->steps * (f32)index;
    spread = work->unk80;
    slot->unk14 = work->unk78 * (effMiscRandUnitFloat(D_0034DF38) * spread + (1.0f - spread));
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001827D0);

void effPcpCopyDriftVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpDriftSetScale(EffPCPWork *work, f32 val) {
    work->unk110 = val;
}

void func_00182A38(EffPCPWork *work, u32 val) {
    work->unk114 = val;
}

/* Placement block handed to every spawned event entry. */
typedef struct EffPCPEventPlace {
    f32 pos[7];
    f32 scaleA;
    f32 scaleB;
    f32 scaleC;
    f32 scaleD;
    u32 color;
} EffPCPEventPlace;

extern EffPCPEventOwner *func_00190100(void *params);

typedef struct EffPCPDriftSrc {
    u8 pad00[0x18];
    u32 count;            /* 0x18 */
    u8 pad1C[0x1C];
    u8 params[0x54];      /* 0x38: effThunderFragCreate parameter block */
} EffPCPDriftSrc;

typedef struct EffPCPDriftBlock {
    u32 word[35];
} EffPCPDriftBlock;      /* 0x8C-byte header copied from the source */

typedef struct EffPCPDriftEntry {
    u32 frag;             /* 0x00 */
    void *eventA;         /* 0x04 */
    void *eventB;         /* 0x08 */
    u8 pad0C[0x10];
    s32 delay;            /* 0x1C */
} EffPCPDriftEntry;

typedef struct EffPCPDriftSpawn {
    u8 pad00[0x1C];
    s32 life;             /* 0x1C */
    u8 pad20[0x6C];
    EffPCPDriftEntry *entries; /* 0x8C */
    EffPCPEventOwner *ownerA;  /* 0x90 */
    EffPCPEventOwner *ownerB;  /* 0x94 */
    u8 flag;              /* 0x98 */
    u8 pad99[3];
    u32 color;            /* 0x9C */
    u32 handle;           /* 0xA0 */
} EffPCPDriftSpawn;

/* Clone the source effect header, then give every entry two events (placed at the unit scale) and a random negative start delay. */
EffPCPDriftSpawn *func_00182A40(EffPCPDriftSrc *src, void *paramsA, void *paramsB) {
    u32 count = src->count;
    u32 handle = (u32)func_002D03F8(count * 32 + 0xA4);
    EffPCPDriftSpawn *work = sdfResourceRetainAddress((void *)handle);
    EffPCPEventPlace place;
    EffPCPDriftEntry *entry;
    s32 life;
    u32 i;

    *(EffPCPDriftBlock *)work = *(EffPCPDriftBlock *)src;
    entry = (EffPCPDriftEntry *)((u8 *)work + 0xA4);
    work->handle = handle;
    work->entries = entry;
    work->color = 0x80808080;
    if (work->life <= 0) {
        work->life = 1;
    }
    work->flag = 1;
    work->ownerA = func_00190100(paramsA);
    work->ownerB = func_00190100(paramsB);
    place.pos[0] = 0;
    place.pos[1] = 0;
    place.pos[2] = 0;
    place.pos[3] = 0;
    place.pos[4] = 0;
    place.pos[5] = 0;
    place.pos[6] = 0;
    place.scaleA = 1.0f;
    place.scaleB = 100.0f;
    place.scaleC = 100.0f;
    place.scaleD = 1.0f;
    place.color = 0x80808080;
    life = work->life;
    for (i = 0; i < count; i++) {
        entry->frag = effThunderFragCreate(src->params);
        entry->eventA = (void *)func_00190130(work->ownerA, 2, &place);
        entry->eventB = (void *)func_00190130(work->ownerB, 2, &place);
        entry->delay = -(effMiscRand(D_0034DF38) % life);
        entry++;
    }
    return work;
}

void effPcpDriftCreateFromTable(void *args) {
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
            effPCPThunderFree3(entry->handle);
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
void effPcpRandomizeSlot(EffPCPSlotWork *work, s32 index) {
    EffPCPSlot20 *slot;
    f32 spread;

    slot = &work->slots[index];
    slot->phase = (3.14159265f * 2.0f) / work->count * index;
    spread = work->spreadA;
    slot->speed = work->baseSpeed * (effMiscRandUnitFloat(D_0034DF38) * spread + (1.0f - spread));
    spread = work->spreadB;
    slot->unk14 = 0;
    if (effMiscRand(D_0034DF38) & 1) {
        slot->spin = work->baseSpin * (effMiscRandUnitFloat(D_0034DF38) * spread + (1.0f - spread));
    } else {
        slot->spin = -work->baseSpin * (effMiscRandUnitFloat(D_0034DF38) * spread + (1.0f - spread));
    }
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001830F8);

void effPcpCopyRandomizedVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_001834D0(EffPCPWork *work, u32 val) {
    work->unk9C = val;
}

typedef struct EffPCPFramedSrc {
    u8 pad00[0x58];
    u32 count; /* 0x58 */
} EffPCPFramedSrc;

typedef struct EffPCPFramedBlock {
    u32 word[37];
} EffPCPFramedBlock; /* 0x94-byte header copied from the source */

typedef struct EffPCPFramedEntry {
    void *event; /* 0x00 */
    s32 delay;   /* 0x04 */
    u8 pad08[0x18];
} EffPCPFramedEntry;

typedef struct EffPCPFramedWork {
    u8 pad00[0x5C];
    s32 life; /* 0x5C */
    u8 pad60[0x34];
    EffPCPFramedEntry *entries; /* 0x94 */
    EffPCPEventOwner *owner;    /* 0x98 */
    u8 flag;                    /* 0x9C */
    u8 pad9D[3];
    f32 scale;                  /* 0xA0 */
    u32 color;                  /* 0xA4 */
    u32 handle;                 /* 0xA8 */
} EffPCPFramedWork;

/* Clone the source header, then give every entry one event (placed at the unit scale) and a random negative start delay. */
EffPCPFramedWork *func_001834D8(EffPCPFramedSrc *src, void *params) {
    u32 count = src->count;
    u32 handle = (u32)func_002D03F8(count * 32 + 0xAC);
    EffPCPFramedWork *work = sdfResourceRetainAddress((void *)handle);
    EffPCPEventPlace place;
    EffPCPFramedEntry *entry;
    s32 life;
    u32 i;

    *(EffPCPFramedBlock *)work = *(EffPCPFramedBlock *)src;
    entry = (EffPCPFramedEntry *)((u8 *)work + 0xAC);
    work->handle = handle;
    work->color = 0x80808080;
    work->flag = 1;
    work->entries = entry;
    work->scale = 1.0f;
    work->owner = func_00190100(params);
    place.pos[0] = 0;
    place.pos[1] = 0;
    place.pos[2] = 0;
    place.pos[3] = 0;
    place.pos[4] = 0;
    place.pos[5] = 0;
    place.pos[6] = 0;
    place.scaleA = 1.0f;
    place.scaleB = 100.0f;
    place.scaleC = 100.0f;
    place.scaleD = 1.0f;
    place.color = 0x80808080;
    life = work->life;
    for (i = 0; i < count; i++) {
        entry->event = (void *)func_00190130(work->owner, 2, &place);
        if (life > 0) {
            entry->delay = -(effMiscRand(D_0034DF38) % life);
        } else {
            entry->delay = 0;
        }
        entry++;
    }
    return work;
}

void effPcpSlotEffectCreateFromTable(void *args) {
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

typedef struct EffPCPSpawnSlot {
    u8 pad00[4];
    u32 unk04;
    f32 unk08;
    f32 unk0C;
    f32 angle;         /* sampled over one full turn */
    f32 unk14;
    f32 position;
    f32 positionStep;
} EffPCPSpawnSlot;

typedef struct EffPCPSpawnRange {
    u8 pad00[0x54];
    s32 count;
    u8 pad58[0x10];
    f32 unk68;
    f32 unk6C;
    u8 pad70[4];
    f32 unk74;
    f32 unk78;
    u8 pad7C[4];
    f32 startPosition;
    f32 endPosition;
    f32 startJitter;
    f32 endJitter;
    f32 unk90;
    EffPCPSpawnSlot *slots;
    u8 pad98[8];
    f32 scale;
} EffPCPSpawnRange;

/* Randomises spawn slot `index` inside the ranges held by the work. */
void effPcpRandomizeSpawnSlot(EffPCPSpawnRange *work, s32 index) {
    EffPCPSpawnSlot *slot;
    f32 spread;
    f32 scale;

    slot = &work->slots[index];
    scale = work->scale;
    slot->unk04 = 0;
    slot->unk08 = -work->unk90 * effMiscRandUnitFloat(D_0034DF38);
    spread = work->unk78;
    slot->unk0C = work->unk74 * (effMiscRandUnitFloat(D_0034DF38) * spread + (1.0f - spread));
    slot->angle = effMiscRandUnitFloat(D_0034DF38) * (3.14159265f * 2.0f);
    spread = work->unk6C;
    slot->unk14 = work->unk68 * (effMiscRandUnitFloat(D_0034DF38) * spread + (1.0f - spread));
    spread = work->startJitter;
    slot->position = work->startPosition * (effMiscRandUnitFloat(D_0034DF38) * spread + (1.0f - spread)) * scale;
    spread = work->endJitter;
    slot->positionStep = (work->endPosition * (effMiscRandUnitFloat(D_0034DF38) * spread + (1.0f - spread)) * scale - slot->position) / (f32)work->count;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183B20);

void effPcpCopySpawnRangeVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSpawnRangeSetScale(EffPCPWork *work, f32 val) {
    work->unkA0 = val;
}

void func_00183DB8(EffPCPWork *work, u32 val) {
    work->unkA4 = val;
}

typedef struct {
    u8 pad00[0x10];
    u8 unk10;
    u8 pad11[3];
    u32 unk14;
    u32 unk18;
    void *event;
} EffPCPEventEntry32;

typedef struct {
    u8 pad00[0x18];
    f32 unk18;
    EffPCPEventEntry32 *entries;
    u32 handle;
    EffPCPEventOwner *owner;
    u32 resource;
    f32 scale;
    u32 unk30;
    u32 unk34;
    u32 count;
    u32 color;
} EffPCPEventWork32;

typedef struct EffPCPEventModelInfo {
    u8 pad00[0x20];
    f32 unk20;
    u8 pad24[0xA];
    u16 unk2E;
} EffPCPEventModelInfo;

typedef struct EffPCPEventModel {
    u8 pad00[0x18];
    void *unk18;
    EffPCPEventModelInfo *info;
} EffPCPEventModel;

void effPcpEventWorkInitEntries(EffPCPEventWork32 *work) {
    EffPCPEventPlace place;
    u32 i = 0;
    u32 count;
    EffPCPEventModel *model;
    EffPCPEventEntry32 *entry;

    model = effParamWorkGetData(work->resource);
    model->info->unk20 = work->unk18;
    mdlAddEntryPlain(model, 0, 0);
    work->unk34 = model->info->unk2E;
    count = sdfCountMapPositionRecords(model->unk18);
    work->count = count;
    work->handle = (u32)func_002D03F8(count << 5);
    entry = sdfResourceRetainAddress((void *)work->handle);
    work->entries = entry;
    place.pos[0] = 0;
    place.pos[1] = 0;
    place.pos[2] = 0;
    place.pos[3] = 0;
    place.pos[4] = 0;
    place.pos[5] = 0;
    place.pos[6] = 0;
    place.scaleA = 1.0f;
    place.scaleB = 100.0f;
    place.scaleC = 100.0f;
    place.scaleD = 1.0f;
    place.color = 0x80808080;
    for (; i < count; i++) {
        entry->unk10 = 0;
        entry->unk14 = 0;
        entry->unk18 = 0;
        entry->event = (void *)func_00190130(work->owner, 2, &place);
        entry++;
    }
}

typedef struct {
    u32 word[7];
} EffPCPEventHead28;

extern u32 effParamWorkCreate(s32 kind, void *params);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183EE0);

void effPcpEventWorkCreateFromTable(void *args) {
    void *param0;
    void *param1;
    void *param2;

    param0 = effParamTableGetBlock(args, 0);
    param1 = effParamTableGetBlock(args, 1);
    param2 = effParamTableGetBlock(args, 2);
    func_00183EE0(param0, param1, param2);
}

EffPCPEventWork32 *effEventWorkClone(EffPCPEventWork32 *src) {
    EffPCPEventWork32 *work;

    work = func_00183EE0(src, 0, 0);
    work->resource = effParamWorkDuplicate(src->resource);
    work->owner = src->owner;
    effPcpEventWorkInitEntries(work);
    return work;
}

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
    sdfReleaseChipBlock(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184130);

INCLUDE_RODATA(const s32, "effect/effPCPMisc", D_003A0EF0);

INCLUDE_SDATA(const s32, "effect/effPCPMisc", D_003BB048);

INCLUDE_SDATA(const s32, "effect/effPCPMisc", D_003BB04C);

INCLUDE_SDATA(const s32, "effect/effPCPMisc", D_003BB04D);

