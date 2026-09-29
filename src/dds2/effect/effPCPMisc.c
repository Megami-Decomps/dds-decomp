#include "common.h"
#include "pcp_vu0.h"

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

extern u32 effParamCreateFromTable(void *data, s32 index);

extern u32 effParamWorkDuplicate(u32 param);

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
extern void *effParamTableGetBlock(void *data, s32 index);

extern void func_00186120(void *dst, void *src);

/* Round-robin selector work behind effPcpRotateFireIds: three key/ID pairs plus a
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

extern EffPCPWork *D_00438F04;

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

extern u32 func_0018FBF8(void *params);

extern u32 func_0018E850(void *params);

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

extern u32 func_0018EBC8(void *params);

extern u32 func_0018F098(void *params);

/* Compact burst variant with a floating-point word at 0x18. */
typedef struct {
    u8 pad00[0x18];
    f32 unk18;
    f32 unk1C;
    u32 unk20;
} EffPCPBurstWork;

extern u8 D_003B1B50[];

extern u8 D_003B1C10[];

extern u8 D_003B1CD0[];

extern u8 D_003B1D90[];

extern u8 D_003B1E50[];

extern u8 D_003B1F10[];

extern void func_00188198(f32 value);

typedef struct {
    u8 pad00[0x28];
    f32 scale;
    u8 pad2C[0x24];
    u32 state;
} EffPCPSubEffectWork;

extern void func_00336798(f32 angle);

extern void func_00188828(f32 angle);

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

extern void func_0017F4C8(EffPCPWorkF14 *work, s32 index);

extern void func_0017FED8(EffPCPWorkF14 *work, s32 index);

extern void *func_003292A8(s32 size);

extern void *sdfResourceRetainAddress(void *resource);

extern void func_0017EF50();

extern void func_0017F4A8();

extern void func_0017F928();

extern void func_0017FEB8();

extern void func_00180240();

extern void func_00180900();

extern void func_00180D90();

extern void func_00181208();

extern void func_001816A0();

extern void func_00181B18();

extern void func_00181E88();

extern void func_001821F8();

extern void func_00182448();

extern void func_001828E0();

extern void func_00182B28();

extern void func_00182F80();

extern void func_001832C8();

extern void func_00183620();

extern void func_00183DB0();

extern void func_00184148();

extern void func_001843F0();

extern void func_00184698();

extern void func_00185100();

extern void func_00186E18();

extern void func_00187118();

extern void func_00187418();

extern void func_00187A90();

extern void func_00187E30();

extern void func_00188778();

extern void func_00188E08();

extern void func_001898B8();

extern void func_00189DC8();

extern void func_0018A678();

extern void func_0018B118();

extern void func_0018B9F8();

/* Boss effect work: settable param plus two handles released on free. */
typedef struct {
    u8 pad00[0x10];
    u128 parameterVector; /* 0x10 */
    u8 pad20[4];
    u32 unk24;      /* 0x24 settable param */
    u32 resource28;  /* 0x28 released by func_001629F0 */
    u32 resource2C;  /* 0x2C released by func_001629F0 */
} EffPCPBossWork;

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

extern void func_0017EF70(void *, s32);

extern void *func_0016A5A0(u32 handle);

extern void mdlAddEntryPlain(void *obj, s32 a, s32 b);

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

extern void func_0017F948(void *, s32, s32);

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

typedef struct {
    u32 word[9];
} EffPCPBlock36;

typedef struct {
    EffPCPBlock36 head;
    u32 color24;
    u32 unk28;
    u32 unk2C;
} EffPCPFlat30;

typedef struct {
    u32 word[13];
} EffPCPBlock52;

typedef struct {
    EffPCPBlock52 head;
    u32 color34;
    u32 unk38;
    u32 unk3C;
} EffPCPFlat40;

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

extern void *effPcpTripleHandleCreate(void *block0, u32 *blocks);

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

extern u32 func_001578C0(u32 param);

extern EffPCPWork *effPcpBuildBlockSet();

extern EffPCPWork *func_00185118(void *first, void **blocks);

typedef struct {
    void *block1;
    void *group[5];
    void *block7;
    void *block8;
    void *block9;
    void *tail[5];
    void *block15;
} EffPCPBlockSet;

extern void func_00180748(void *dst, void *src);

extern void func_00184B10(void *dst, void *src);

typedef struct {
    u32 word[63];
} EffPCPBlock252;

typedef struct EffPCPBlock50 {
    u32 word[20];
} EffPCPBlock50;

extern u8 *func_00187E50(u32);

typedef struct EffPCPBlock5C {
    u32 word[23];
} EffPCPBlock5C;

extern f32 func_00341240(void *state);

extern u8 D_003AA868[];

extern f32 func_00341240(void *state);

extern u8 D_003AA868[];

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

extern void func_00232390(void *obj, void *table);

extern void func_00232AA0(void *obj);

extern void func_00332D48(u32 handle, s32 value);

extern u8 D_00380828[];

extern void mdlBroadcastMasked(void *obj, u32 mask);

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

typedef struct {
    u8 pad00[0x10];
    u32 unk10;
    u8 pad14[4];
    u32 handle[12];
    u32 delay[6];
} EffPCPDelayedPairs;

extern void func_0016A668(u32 handle);

extern void effParamWorkCallback0(u32 handle, void *vec);

extern void effParamWorkCallback3(u32 handle, u32 value);

extern void effParamWorkCallback2(u32 handle, void *mtx);

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

typedef struct {
    u8 pad00[0xC3C];
    u32 active;
} EffPCPEventOwner;

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

extern void effEventReleaseNode(void *event);

extern void func_00197D50();

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

extern void func_0016D228(u32 handle);

extern void func_00187AA8(void *work);

extern void *func_00189DE8();

/* Effect initializers implemented in assembly below (func_001708A0 lives in
   another unit). Each is entered with and without spawn arguments, so they
   are declared unchecked. */
extern void *func_0018A698();

extern void *func_0018B130();

/* Effect initializers implemented in assembly below (func_001708A0 lives in
   another unit). Each is entered with and without spawn arguments, so they
   are declared unchecked. */
extern void *func_0018BB38();

void func_0017EDE8(u32 arg0) {
    billDispatchByKind(*(u32 *)((s32)arg0 + 0x1c));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017EE18);

void func_0017EF50(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0017EF60(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

void func_0017EF68(EffPCPWorkF18 *work, f32 val) {
    work->unk18 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017EF70);

EffPCPTwinWork *effTwinEffectCreateFromTable(void *src) {
    EffPCPTwinWork *work = func_00328D68(0xBC);
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
        func_0017EF70(work, i);
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

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F1E8);

EffPCPTwinWork *effTwinEffectClone(EffPCPTwinWork *src) {
    EffPCPTwinWork *work = func_00328D68(0xBC);
    s32 i;

    for (i = 0; i < 8; i++) {
        work->pair[i][0] = effParamWorkDuplicate(src->pair[i & 1][0]);
        work->pair[i][1] = effParamWorkDuplicate(src->pair[i & 1][1]);
        work->shared[i] = effParamWorkDuplicate(src->shared[0]);
        work->state[i] = 0;
        func_0017EF70(work, i);
    }
    work->unk00 = 0;
    work->color = 0x80808080;
    work->scale = 1.0f;
    work->unk04 = 0;
    work->unk08 = 0;
    work->unk14 = 0;
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F358);

void func_0017F4A8(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0017F4B8(EffPCPWorkF18 *work, f32 val) {
    work->unk18 = val;
}

void func_0017F4C0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x10) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F4C8);

EffPCPWorkF14 *effPcpStaggerCreate(void *args) {
    EffPCPWorkF14 *work = func_00328D68(0x98);
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
        func_0017F4C8(work, i++);
    } while (i < 8);
    work->unk10 = 0x80808080;
    work->unk14 = 1.0f;
    *(u32 *)work = 0;
    *(u32 *)((u8 *)work + 4) = 0;
    *(u32 *)((u8 *)work + 8) = 0;
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F6C0);

EffPCPWorkF14 *effCreatePairedResourceWork(EffPCPWork *source) {
    EffPCPWorkF14 *work = func_00328D68(0x98);
    u32 *handle = (u32 *)((u8 *)work + 0x3C);
    s32 i = 0;

    do {
        handle[-1] = effParamWorkDuplicate(source->unk38);
        handle[0] = effParamWorkDuplicate(*(u32 *)source->pad3C);
        handle += 2;
        func_0017F4C8(work, i);
        i++;
    } while (i < 8);
    work->unk10 = 0x80808080;
    work->unk14 = 1.0f;
    *(u32 *)work = 0;
    *(u32 *)((u8 *)work + 4) = 0;
    *(u32 *)((u8 *)work + 8) = 0;
    return work;
}

void effPcpStaggerUpdate(EffPCPStaggered *work) {
    void *obj[2];
    f32 pos[4];
    s32 i;

    for (i = 0; i < 8; i++) {
        if (work->delay[i] != 0) {
            work->delay[i]--;
        } else {
            obj[0] = func_0016A5A0(work->handle[i * 2]);
            obj[1] = func_0016A5A0(work->handle[i * 2 + 1]);
            pos[0] = work->x;
            pos[2] = work->z;
            pos[1] = (work->y - work->offset[i] + 100.0f) * work->scale;
            __asm__ volatile (
                ".set noreorder\n"
                "lqc2 vf10, 0(%0)\n"
                ".set reorder"
                : : "r"(pos));
            func_00232AA0(obj[0]);
            effParamWorkCallback1(work->handle[i * 2], work->scale * 1.5f);
            effParamWorkCallback1(work->handle[i * 2 + 1], 1.5f);
            mdlBroadcastMasked(obj[1], work->unk10);
            func_00232390(obj[0], D_00380828);
            func_00332D48(((EffPCPWork *)obj[0])->unk18, 1);
            func_00232AA0(obj[1]);
            func_00232390(obj[1], D_00380828);
        }
    }
}

void func_0017F928(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0017F938(EffPCPWorkF14 *work, f32 val) {
    work->unk14 = val;
}

void func_0017F940(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x10) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F948);

EffPCPCrossWork *effCrossEffectCreateFromTable(void *src) {
    EffPCPCrossWork *work = func_00328D68(0xDC);
    s32 i;
    s32 j;

    work->base = effParamCreateFromTable(src, 0);
    mdlAddEntryPlain(func_0016A5A0(work->base), 0, 0);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            if (j == 0) {
                work->handle[i][j] = effParamCreateFromTable(src, i + 1);
            } else {
                work->handle[i][j] = effParamWorkDuplicate(work->handle[i][0]);
            }
            func_0017F948(work, i, j);
        }
    }
    work->unk00 = 0;
    work->color = 0x80808080;
    work->scale = 1.0f;
    work->unk04 = 0;
    work->unk08 = 0;
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FBE8);

EffPCPCrossWork *effCrossEffectClone(EffPCPCrossWork *src) {
    EffPCPCrossWork *work = func_00328D68(0xDC);
    s32 i;
    s32 j;

    work->base = effParamWorkDuplicate(src->base);
    mdlAddEntryPlain(func_0016A5A0(work->base), 0, 0);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            work->handle[i][j] = effParamWorkDuplicate(src->handle[i][0]);
            func_0017F948(work, i, j);
        }
    }
    work->unk00 = 0;
    work->color = 0x80808080;
    work->scale = 1.0f;
    work->unk04 = 0;
    work->unk08 = 0;
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FD88);

void func_0017FEB8(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0017FEC8(EffPCPWorkF14 *work, f32 val) {
    work->unk14 = val;
}

void func_0017FED0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x10) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FED8);

EffPCPWorkF14 *effCreateIndexedResourceWork(void *source) {
    EffPCPWorkF14 *work = func_00328D68(0x60);
    u32 *handle = (u32 *)((u8 *)work + 0x1C);
    s32 i;

    for (i = 0; i < 6; i++) {
        if (i == 0) {
            work->unk1C = effParamCreateFromTable(source, 6);
        }
        handle[-1] = effParamCreateFromTable(source, i);
        handle[0] = effParamWorkDuplicate(work->unk1C);
        handle += 2;
        func_0017FED8(work, i);
    }
    work->unk10 = 0x80808080;
    work->unk14 = 1.0f;
    *(u32 *)work = 0;
    *(u32 *)((u8 *)work + 4) = 0;
    *(u32 *)((u8 *)work + 8) = 0;
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180040);

EffPCPWorkF14 *effCopyIndexedResourceWork(EffPCPWork *source) {
    u32 *sourceHandle;
    u32 *workHandle;
    s32 i;
    EffPCPWorkF14 *work = func_00328D68(0x60);
    sourceHandle = (u32 *)((u8 *)source + 0x1C);
    workHandle = (u32 *)((u8 *)work + 0x1C);

    for (i = 0; i < 6; i++) {
        workHandle[-1] = effParamWorkDuplicate(sourceHandle[-1]);
        workHandle[0] = effParamWorkDuplicate(sourceHandle[0]);
        sourceHandle += 2;
        workHandle += 2;
        func_0017FED8(work, i);
    }
    work->unk10 = 0x80808080;
    work->unk14 = 1.0f;
    *(u32 *)work = 0;
    *(u32 *)((u8 *)work + 4) = 0;
    *(u32 *)((u8 *)work + 8) = 0;
    return work;
}

void effPcpDelayedPairsUpdate(EffPCPDelayedPairs *work) {
    void *obj[2];
    u128 vec;
    s32 i;

    for (i = 0; i < 6; i++) {
        if (work->delay[i] != 0) {
            work->delay[i]--;
        } else {
            obj[0] = func_0016A5A0(work->handle[i * 2]);
            obj[1] = func_0016A5A0(work->handle[i * 2 + 1]);
            __asm__ volatile (
                ".set noreorder\n"
                "lqc2 vf10, 0(%0)\n"
                ".set reorder"
                : : "r"(work) : "memory");
            func_00232AA0(obj[0]);
            mdlBroadcastMasked(obj[1], work->unk10);
            func_00232390(obj[0], D_00380828);
            func_00332D48(((EffPCPWork *)obj[0])->unk18, 1);
            __asm__ volatile (
                ".set noreorder\n"
                "sqc2 vf10, 0(%0)\n"
                ".set reorder"
                : : "r"(&vec));
            func_00232AA0(obj[1]);
            func_00232390(obj[1], D_00380828);
        }
    }
}

void func_00180240(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

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

EffPCPChargeWork *effCreateChargeWork(void *source) {
    void *resource = func_003292A8(0x1354);
    EffPCPChargeWork *work = sdfResourceRetainAddress(resource);
    work->unk1350 = (u32)resource;
    work->unk134C = effParamCreateFromTable(source, 0);
    work->unk1348 = effParamCreateFromTable(source, 1);
    func_00180260(work);
    work->unkAF0 = 0;
    work->unkAF4 = 0;
    work->unkAF8 = 0;
    work->unk1334 = 0x80808080;
    work->unk1338 = 1.0f;
    return work;
}

void func_00180318(s32 arg0) {
    func_0016A620(*(u32 *)(arg0 + 0x134c));
    func_0016A620(*(u32 *)(arg0 + 0x1348));
    func_003297C8(*(u32 *)(arg0 + 0x1350));
}

EffPCPChargeWork *effCopyChargeResources(EffPCPChargeWork *source) {
    void *resource = func_003292A8(0x1354);
    EffPCPChargeWork *work = sdfResourceRetainAddress(resource);
    u32 firstHandle = source->unk134C;
    work->unk1350 = (u32)resource;
    work->unk134C = effParamWorkDuplicate(firstHandle);
    work->unk1348 = effParamWorkDuplicate(source->unk1348);
    func_00180260(work);
    work->unkAF0 = 0;
    work->unkAF4 = 0;
    work->unkAF8 = 0;
    work->unk1334 = 0x80808080;
    work->unk1338 = 1.0f;
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001803E8);

void func_00180748(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0xaf0, src);
}

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
    dst->unk14 = effParamCreateFromTable(src, 0);
    handle = effParamCreateFromTable(src, 1);
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

    obj = func_0016A5A0(work->unk14);
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf10, 0(%0)\n"
        ".set reorder"
        : : "r"(work) : "memory");
    func_00232AA0(obj);
    effParamWorkCallback3(work->unk18, work->unk10);
    func_00232390(obj, D_00380828);
    func_00332D48(((EffPCPWork *)obj)->unk18, 1);
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf10, %0\n"
        ".set reorder"
        : "=m"(vec) : : "memory");
    effParamWorkCallback0(work->unk18, &vec);
    func_0016A668(work->unk18);
}

void func_00180900(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

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

typedef struct MenuPanelChildren1C {
    u8 pad00[0x1C];
    void *children[30];
} MenuPanelChildren1C;

void func_00180C10(MenuPanelChildren1C *group) {
    s32 i;
    for (i = 0; i < 12; i++) {
        func_0016D228(group->children[i]);
    }
    func_00328E48(group);
}

u64 func_00180C68(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x4c);
    func_00180918(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180CA0);

void func_00180D90(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

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

void func_00181088(MenuPanelChildren1C *group) {
    s32 i;
    for (i = 0; i < 12; i++) {
        func_0016D228(group->children[i]);
    }
    func_00328E48(group);
}

u64 func_001810E0(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x4c);
    func_00180DB0(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181118);

void func_00181208(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

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

void func_00181520(MenuPanelChildren1C *group) {
    s32 i;
    for (i = 0; i < 30; i++) {
        func_0016D228(group->children[i]);
    }
    func_00328E48(group);
}

u64 func_00181578(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x94);
    func_00181228(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001815B0);

void func_001816A0(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

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

void func_00181998(MenuPanelChildren1C *group) {
    s32 i;
    for (i = 0; i < 12; i++) {
        func_0016D228(group->children[i]);
    }
    func_00328E48(group);
}

u64 func_001819F0(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x4c);
    func_001816C0(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181A28);

void func_00181B18(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

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

void func_00181D08(MenuPanelChildren1C *group) {
    s32 i;
    for (i = 0; i < 12; i++) {
        func_0016D228(group->children[i]);
    }
    func_00328E48(group);
}

u64 func_00181D60(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x4c);
    func_00181B38(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181D98);

void func_00181E88(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

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

void func_00182078(MenuPanelChildren1C *group) {
    s32 i;
    for (i = 0; i < 8; i++) {
        func_0016D228(group->children[i]);
    }
    func_00328E48(group);
}

u64 func_001820D0(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x3c);
    func_00181EA8(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182108);

void func_001821F8(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

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

void *func_00182250(void) {
    void *work;

    work = func_00328D68(0x20);
    func_00182218(work);
    return work;
}

void func_00182288(u32 arg0) {
    func_0016B130(*(u32 *)((s32)arg0 + 0x1c));
    func_00328E48(arg0);
}

void *func_001822B8(void) {
    void *work;

    work = func_00328D68(0x20);
    func_00182218(work);
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001822F0);

void func_00182448(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

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

void func_00182760(MenuPanelChildren1C *group) {
    s32 i;
    for (i = 0; i < 30; i++) {
        func_0016D228(group->children[i]);
    }
    func_00328E48(group);
}

u64 func_001827B8(void) {
    u64 temp_v0;

    temp_v0 = func_00328D68(0x94);
    func_00182468(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001827F0);

void func_001828E0(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

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

void *func_00182938(void) {
    void *work;

    work = func_00328D68(0x20);
    func_00182900(work);
    return work;
}

void func_00182970(u32 arg0) {
    func_0016B130(*(u32 *)((s32)arg0 + 0x1c));
    func_00328E48(arg0);
}

void *func_001829A0(void) {
    void *work;

    work = func_00328D68(0x20);
    func_00182900(work);
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001829D8);

void func_00182B28(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00182B38(EffPCPWorkF10 *work, f32 val) {
    work->unk10 = val;
}

void func_00182B40(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182B48);

INCLUDE_ASM(const s32, "effect/effPCPMisc", effPcpSharedWorkRelease);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182C60);

void func_00182DA0(void *work, void *src) {
    PCP_COPY_VECTOR(D_00438F04, src);
}

void func_00182DB8(u32 unused, u32 val) {
    D_00438F04->unk10 = val;
}

void effSetSharedScale(u32 unused, f32 value) {
    ((EffPCPWorkF1C *)D_00438F04)->unk1C = value;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182DD8);

void func_00182E70(u32 arg0) {
    func_0018E8F0(*(u32 *)((s32)arg0 + 0x34));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182EA0);

void func_00182F80(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00182F90(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x10) = arg1;
}

void func_00182F98(EffPCPWorkF1C *work, f32 val) {
    work->unk1C = val;
}

EffPCPCompactWork3C *func_00182FA0(EffPCPCompactParams *params) {
    EffPCPCompactWork3C *work;

    work = func_00328D68(0x3C);
    work->resource = func_0018FBF8(&params->unk18);
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

void func_00183030(void *args) {
    void *param0;

    param0 = effParamTableGetBlock(args, 0);
    func_00182FA0(param0);
}

void effPcpCompactRespawn(EffPCPCompactSrc *work) {
    EffPCPCompactParams3C params;

    params.flags = work->flags;
    params.unk04 = work->unk20;
    params.unk08 = work->unk24;
    params.unk0C = work->unk28;
    params.unk10 = work->unk2C;
    params.unk14 = work->unk30;
    params.res = *work->resource;
    func_00182FA0((EffPCPCompactParams *)&params);
}

void func_001830F0(u32 arg0) {
    func_0018FC88(*(u32 *)((s32)arg0 + 0x38));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183120);

void func_001832C8(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_001832D8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

void func_001832E0(EffPCPWorkF34 *work, f32 val) {
    work->unk34 = val;
}

EffPCPCompactWork3C *func_001832E8(EffPCPCompactParams *params) {
    EffPCPCompactWork3C *work;

    work = func_00328D68(0x3C);
    work->resource = func_0018E850(&params->unk18);
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

void func_00183378(void *args) {
    void *param0;

    param0 = effParamTableGetBlock(args, 0);
    func_001832E8(param0);
}

void effPcpCompactLongRespawn(EffPCPSrcA *work) {
    EffPCPParams44 params;

    params.flags = work->flags;
    params.unk04 = work->unk20;
    params.unk08 = work->unk24;
    params.unk0C = work->unk28;
    params.unk10 = work->unk2C;
    params.unk14 = work->unk30;
    params.res = *work->resource;
    func_001832E8((EffPCPCompactParams *)&params);
}

void func_00183448(u32 arg0) {
    func_0018E8F0(*(u32 *)((s32)arg0 + 0x38));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183478);

void func_00183620(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00183630(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

void func_00183638(EffPCPWorkF34 *work, f32 val) {
    work->unk34 = val;
}

void *effPcpCopyWork(src)
    EffPCPFlat30 *src;
{
    EffPCPFlat30 *dst;

    dst = func_00328D68(0x30);
    dst->head = src->head;
    dst->color24 = 0x80808080;
    dst->unk2C = 0;
    dst->unk28 = src->head.word[3];
    return dst;
}

void func_001836D0(void *args) {
    void *param0;

    param0 = effParamTableGetBlock(args, 0);
    effPcpCopyWork(param0);
}

void func_001836F0(void) {
    effPcpCopyWork();
}

void func_00183708(void) {
    func_00328E48();
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183720);

void func_00183830(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void *effPcpCopyWorkLong(src)
    EffPCPFlat40 *src;
{
    EffPCPFlat40 *dst;

    dst = func_00328D68(0x40);
    dst->head = src->head;
    dst->color34 = 0x80808080;
    dst->unk3C = 0;
    dst->unk38 = src->head.word[3];
    return dst;
}

void func_001838E8(void *args) {
    void *param0;

    param0 = effParamTableGetBlock(args, 0);
    effPcpCopyWorkLong(param0);
}

void func_00183908(void) {
    effPcpCopyWorkLong();
}

void func_00183920(void) {
    func_00328E48();
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183938);

void func_00183A50(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x34) = arg1;
}

EffPCPCompactWork *func_00183A58(EffPCPCompactParams *params) {
    EffPCPCompactWork *work;

    work = func_00328D68(0x38);
    work->resource = func_0018EBC8(&params->unk18);
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

void func_00183AE8(void *args) {
    void *param0;

    param0 = effParamTableGetBlock(args, 0);
    func_00183A58(param0);
}

typedef struct {
    u8 pad00[0x10];
    u8 flags;
    u8 pad11[3];
    u32 unk14;
    u32 unk18;
    u32 unk1C;
    u8 pad20[0x8];
    u32 unk28;
    u32 unk2C;
    u8 pad30[0x4];
    EffPCPRes44 *resource;
} EffPCPSrcD;

void effPcpChargeRespawn(EffPCPSrcD *work) {
    EffPCPParams44 params;

    params.flags = work->flags;
    params.unk04 = work->unk14;
    params.unk08 = work->unk18;
    params.unk0C = work->unk1C;
    params.unk10 = work->unk28;
    params.unk14 = work->unk2C;
    params.res = *work->resource;
    func_00183A58((EffPCPCompactParams *)&params);
}

void func_00183BB8(u32 arg0) {
    func_0018ECB8(*(u32 *)((s32)arg0 + 0x34));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183BE8);

void func_00183DB0(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00183DC0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x20) = arg1;
}

EffPCPCompactWork *func_00183DC8(EffPCPCompactParams *params) {
    EffPCPCompactWork *work;

    work = func_00328D68(0x38);
    work->resource = func_0018F098(&params->unk18);
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

void func_00183E58(void *args) {
    void *param0;

    param0 = effParamTableGetBlock(args, 0);
    func_00183DC8(param0);
}

void effPcpChargeLongRespawn(EffPCPSrcD *work) {
    EffPCPParams44 params;

    params.flags = work->flags;
    params.unk04 = work->unk14;
    params.unk08 = work->unk18;
    params.unk0C = work->unk1C;
    params.unk10 = work->unk28;
    params.unk14 = work->unk2C;
    params.res = *work->resource;
    func_00183DC8((EffPCPCompactParams *)&params);
}

void func_00183F28(u32 arg0) {
    func_0018F1B8(*(u32 *)((s32)arg0 + 0x34));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183F58);

void func_00184148(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00184158(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x20) = arg1;
}

void func_00184160(EffPCPWork *work) {
    u32 handle;

    handle = func_0016AF38(D_003B1A38);
    work->unk18 = 0;
    work->unk1C = handle;
}

void *func_00184198(void) {
    void *work;

    work = func_00328D68(0x20);
    func_00184160(work);
    return work;
}

void func_001841D0(u32 arg0) {
    func_0016B130(*(u32 *)((s32)arg0 + 0x1c));
    func_00328E48(arg0);
}

void *func_00184200(void) {
    void *work;

    work = func_00328D68(0x20);
    func_00184160(work);
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184238);

void func_001843F0(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

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

void *func_00184448(void) {
    void *work;

    work = func_00328D68(0x20);
    func_00184410(work);
    return work;
}

void func_00184480(u32 arg0) {
    func_0016B130(*(u32 *)((s32)arg0 + 0x1c));
    func_00328E48(arg0);
}

void *func_001844B0(void) {
    void *work;

    work = func_00328D68(0x20);
    func_00184410(work);
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001844E8);

void func_00184698(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_001846A8(EffPCPWorkF10 *work, f32 val) {
    work->unk10 = val;
}

void func_001846B0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001846B8);

void func_001847E0(void *args) {
    void *param0;
    void *param1;

    param0 = effParamTableGetBlock(args, 0);
    param1 = effParamTableGetBlock(args, 1);
    func_001846B8(param0, param1);
}

EffPCPWork *effPcpCloneWithOptionalHandle(EffPCPWork *work) {
    EffPCPWork *child;
    u32 handle;

    child = func_001846B8(&work->pad3C[4], NULL);
    handle = work->optionalHandle;
    if (handle != 0) {
        child->optionalHandle = func_00157A50(handle);
    }
    return child;
}

void effPcpReleaseOptionalHandle(u32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)((s32)arg0 + 0x74);
    if (temp_v0 != 0) {
        func_00157658(temp_v0);
    }
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001848B8);

void func_00184B10(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0x40, src);
}

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

void *effPcpTripleHandleCreate(void *block0, u32 *blocks) {
    EffPCPTripleWork *work;
    u32 *handle;
    u32 i;

    work = func_00328D68(0xAC);
    work->head = *(EffPCPBlock80 *)block0;
    work->unk50 = 0;
    work->color54 = 0x80808080;
    handle = work->handleA;
    for (i = 0; i < 7; i++) {
        handle[0] = func_001578C0(blocks[i]);
        handle[7] = func_00157A50(handle[0]);
        handle[14] = func_00157A50(handle[0]);
        handle++;
    }
    return work;
}

void func_00184CF8(void *data) {
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

    work = func_00328D68(0xAC);
    work->head = src->head;
    work->unk50 = 0;
    work->color54 = 0x80808080;
    from = src->handleC;
    to = work->handleC;
    for (i = 0; i < 7; i++) {
        to[-14] = func_00157A50(from[-14]);
        to[-7] = func_00157A50(from[-7]);
        to[0] = func_00157A50(from[0]);
        from++;
        to++;
    }
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184EE0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184F50);

void func_00185100(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00185110(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x54) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00185118);

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
    return func_00185118(first, &set);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00185500);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001856B0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00185808);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00185950);

void func_00186100(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0x60, src);
}

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

    temp_v0 = effPcpBuildBlockSet();
    *(u32 *)(temp_v0 + 0xbc) = 1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00186170);

void func_001862C0(void) {
    s32 temp_v0;

    temp_v0 = effPcpBuildBlockSet();
    *(u32 *)(temp_v0 + 0xbc) = 2;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001862E8);

EffPCPRotateWork *effPcpRotateCreate(EffPCPBlock252 *src, u32 *blocks) {
    EffPCPRotateWork *work;
    u8 *sub;
    u32 *p;
    u32 i;

    work = func_00328D68(0x10C);
    *(EffPCPBlock252 *)work = *src;
    sub = (u8 *)src + 0xC;
    work->count = 0;
    for (i = 0; i < 3; i++) {
        blocks[3] = blocks[i];
        work->ids[i] = (s32)func_00185118(sub, (void **)&blocks[3]);
        sub += 0x50;
    }
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001865D0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00186708);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00186990);

void effPcpRotateFireIds(s32 *arg0) {
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

void effSpinEffectUpdate(EffPCPSpinWork *work) {
    u32 handle = (u32)work->handle0;
    u128 mtx[4];

    effParamWorkCallback0(handle, work);
    effParamWorkCallback1(handle, work->scale);
    effParamWorkCallback3(handle, work->color);
    func_00336538(work->angle);
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf24, 0(%0)\n"
        "lqc2 vf25, 0x10(%0)\n"
        "lqc2 vf26, 0x20(%0)\n"
        "lqc2 vf27, 0x30(%0)\n"
        ".set reorder"
        : : "r"(work->matrix) : "memory");
    func_00336AA8();
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf28, 0(%0)\n"
        "sqc2 vf29, 0x10(%0)\n"
        "sqc2 vf30, 0x20(%0)\n"
        "sqc2 vf31, 0x30(%0)\n"
        ".set reorder"
        : : "r"(mtx) : "memory");
    effParamWorkCallback2(handle, mtx);
    func_0016A668(handle);
    work->frame++;
}

void func_00186E18(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00186E28(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x5c) = arg1;
}

void func_00186E30(EffPCPWork *work, f32 val) {
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

INCLUDE_ASM(const s32, "effect/effPCPMisc", effSpinEffectCreateFromTable);

INCLUDE_ASM(const s32, "effect/effPCPMisc", effSpinEffectClone);

void func_00186FE0(u32 arg0) {
    func_0016A620(*(u32 *)((s32)arg0 + 100));
    func_0016A620(*(u32 *)((s32)arg0 + 0x60));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187018);

void func_00187118(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00187128(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x5c) = arg1;
}

void func_00187130(EffPCPWork *work, f32 val) {
    work->unk58 = val;
}

void func_00187138(EffPCPWork *work, void *src) {
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

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187168);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187228);

void func_001872E0(u32 arg0) {
    func_0016A620(*(u32 *)((s32)arg0 + 100));
    func_0016A620(*(u32 *)((s32)arg0 + 0x60));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187318);

void func_00187418(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00187428(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x5c) = arg1;
}

void func_00187430(EffPCPWork *work, f32 val) {
    work->unk58 = val;
}

void func_00187438(EffPCPWork *work, void *src) {
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

EffPCPBurstWork *func_00187508(u32 unused) {
    EffPCPBurstWork *work;

    work = func_00328D68(0x24);
    work->unk18 = 325.0f;
    work->unk1C = 225.0f;
    work->unk20 = func_0016D070(D_003B1B50);
    return work;
}

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

EffPCPBurstWork *func_001875D0(u32 unused) {
    EffPCPBurstWork *work;

    work = func_00328D68(0x24);
    work->unk18 = 325.0f;
    work->unk1C = 225.0f;
    work->unk20 = func_0016D070(D_003B1C10);
    return work;
}

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

EffPCPBurstWork *func_00187698(u32 unused) {
    EffPCPBurstWork *work;

    work = func_00328D68(0x24);
    work->unk18 = 325.0f;
    work->unk1C = 225.0f;
    work->unk20 = func_0016D070(D_003B1CD0);
    return work;
}

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

EffPCPBurstWork *func_00187760(u32 unused) {
    EffPCPBurstWork *work;

    work = func_00328D68(0x24);
    work->unk18 = 325.0f;
    work->unk1C = 225.0f;
    work->unk20 = func_0016D070(D_003B1D90);
    return work;
}

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

EffPCPBurstWork *func_00187828(u32 unused) {
    EffPCPBurstWork *work;

    work = func_00328D68(0x24);
    work->unk18 = 325.0f;
    work->unk1C = 225.0f;
    work->unk20 = func_0016D070(D_003B1E50);
    return work;
}

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

EffPCPBurstWork *func_001878F0(u32 unused) {
    EffPCPBurstWork *work;

    work = func_00328D68(0x24);
    work->unk18 = 325.0f;
    work->unk1C = 225.0f;
    work->unk20 = func_0016D070(D_003B1F10);
    return work;
}

void func_00187940(void) {
    func_001878F0(0);
}

void func_00187958(u32 arg0) {
    func_0016D228(*(u32 *)((s32)arg0 + 0x20));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187988);

void func_00187A90(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00187AA0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x10) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187AA8);

void func_00187B68(void *data) {
    void *work;

    work = effParamTableGetBlock(data, 0);
    func_00187AA8(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187B88);

void func_00187CA8(u32 arg0) {
    func_0016B130(*(u32 *)((s32)arg0 + 0x30));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187CD8);

void func_00187E30(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

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

void effResetChild(EffPCPSubEffectWork *work) {
    func_00188198(work->scale);
    work->state = 0;
}

u8 *effBeamEffectClone(src)
    u8 *src;
{
    u8 *work = func_00328D68(0x60);
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
    node = func_00187E50(kind);
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

void func_001884E8(void *args) {
    void *param0;

    param0 = effParamTableGetBlock(args, 0);
    effBeamEffectClone(param0);
}

void func_00188508(void) {
    effBeamEffectClone();
}

void func_00188520(u32 arg0) {
    func_00187F90(*(u32 *)((s32)arg0 + 0x5c));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188550);

void func_00188778(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpNestedWorkSetFloat(EffPCPWork *work, f32 value) {
    ((EffPCPWork *)work->unk5C)->unk90 = value;
}

void func_00188798(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x54) = arg1;
}

void func_001887A0(EffPCPWork *work, void *src) {
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
    func_00336798(1.5707963f);
    func_00336AA8();
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf28, 0(%0)\n"
        "sqc2 vf29, 0x10(%0)\n"
        "sqc2 vf30, 0x20(%0)\n"
        "sqc2 vf31, 0x30(%0)\n"
        ".set reorder"
        : : "r"((void *)work->unk5C) : "memory");
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188828);

void effPrepareAngles(EffPCPAngleWork *work) {
    work->radiansX = work->degreesX * 0.017453291f;
    work->radiansY = work->degreesY * 0.017453291f;
    work->radiansZ = work->degreesZ * 0.017453291f;
    work->childAngle = work->angle;
    work->childAngle2 = work->angle2;
    func_00188828(work->angle);
    work->child = 0;
}

u8 *effBeamEffectCloneLarge(src)
    u8 *src;
{
    u8 *work = func_00328D68(0x80);
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
    node = func_00187E50(kind);
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

void func_00188C78(void *args) {
    void *param0;

    param0 = effParamTableGetBlock(args, 0);
    effBeamEffectCloneLarge(param0);
}

void func_00188C98(void) {
    effBeamEffectCloneLarge();
}

void func_00188CB0(u32 arg0) {
    func_00187F90(*(u32 *)((s32)arg0 + 0x7c));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188CE0);

void func_00188E08(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpLinkedWorkSetFloat(EffPCPWork *work, f32 value) {
    ((EffPCPWork *)work->unk7C)->unk90 = value;
}

void func_00188E28(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x60) = arg1;
}

void func_00188E30(EffPCPWork *work, void *src) {
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

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188E60);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001890E8);

u8 *func_00189190(u8 *work) {
    u8 *copy = func_00188E60(work, 0);
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
        *(void **)(copy + 0x178) = func_003292A8(size);
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

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001892A8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00189360);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00189500);

void func_001898B8(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_001898C8(EffPCPWork *work, f32 val) {
    work->unk168 = val;
}

void func_001898D0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x170) = arg1;
}

EffPCPSprayWork *effSprayEffectCreateFromTable(void *src) {
    EffPCPSprayWork *work = func_00328D68(0x98);
    u32 i;

    work->scale = 1.0f;
    work->color = 0x80808080;
    work->count = 10;
    work->unk1C = 0;
    for (i = 0; i < work->count; i++) {
        work->handle[i] = 0;
        work->id[i] = i;
        work->angle[i] = (func_00341240(D_003AA868) - 0.5f) * 2.0f * 0.87266457f + 3.14159265f;
    }
    work->handle[0] = effParamCreateFromTable(src, 0);
    return work;
}

EffPCPSprayWork *effSprayEffectClone(EffPCPSprayWork *src) {
    EffPCPSprayWork *work = func_00328D68(0x98);
    u32 i;

    work->unk1C = 0;
    work->scale = src->scale;
    work->count = src->count;
    work->color = 0x80808080;
    for (i = 0; i < work->count; i++) {
        work->handle[i] = 0;
        work->id[i] = i;
        work->angle[i] = (func_00341240(D_003AA868) - 0.5f) * 2.0f * 0.87266457f + 3.14159265f;
    }
    work->handle[0] = effParamWorkDuplicate(src->handle[0]);
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", effDestroyIndexedResources);

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

void func_00189DC8(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00189DD8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

void func_00189DE0(EffPCPWorkF10 *work, f32 val) {
    work->unk10 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00189DE8);

void func_00189FF0(void *data) {
    void *work0;
    void *work1;

    work0 = effParamTableGetBlock(data, 0);
    work1 = effParamTableGetBlock(data, 1);
    func_00189DE8(work0, work1);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018A038);

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
        func_00197D50(work->owner);
    }
    func_003297C8(work->handle);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018A2B8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018A428);

void func_0018A678(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0018A688(EffPCPWork *work, f32 val) {
    work->unk110 = val;
}

void func_0018A690(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x114) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018A698);

void func_0018A8E8(void *data) {
    void *work0;
    void *work1;
    void *work2;

    work0 = effParamTableGetBlock(data, 0);
    work1 = effParamTableGetBlock(data, 1);
    work2 = effParamTableGetBlock(data, 2);
    func_0018A698(work0, work1, work2);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018A950);

void effPcpPairedEventGroupRelease(EffPCPEventPairGroup *work) {
    u32 i = 0;
    u32 count = work->count;
    EffPCPEventPair *entry = work->entries;

    if (count != 0) {
        do {
            effEventReleaseNode(entry->first);
            effEventReleaseNode(entry->second);
            func_0016D228(entry->handle);
            entry++;
            i++;
        } while (i < count);
    }
    if (work->firstOwner->active == 0) {
        func_00197D50(work->firstOwner);
    }
    if (work->secondOwner->active == 0) {
        func_00197D50(work->secondOwner);
    }
    func_003297C8(work->handle);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018AC20);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018AD50);

void func_0018B118(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0018B128(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x9c) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018B130);

void func_0018B350(void *data) {
    void *work0;
    void *work1;

    work0 = effParamTableGetBlock(data, 0);
    work1 = effParamTableGetBlock(data, 1);
    func_0018B130(work0, work1);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018B398);

typedef struct {
    void *event;
    u8 pad04[0x1C];
} EffPCPEventEntry20;

typedef struct {
    u8 pad00[0x58];
    u32 count;
    u8 pad5C[0x38];
    EffPCPEventEntry20 *entries;
    EffPCPEventOwner *owner;
    u8 pad9C[0xC];
    u32 handle;
} EffPCPEventGroup94;

void effPcpEventBatchRelease(EffPCPEventGroup94 *work) {
    u32 i = 0;
    u32 count = work->count;
    EffPCPEventEntry20 *entry = work->entries;

    if (count != 0) {
        do {
            effEventReleaseNode(entry->event);
            entry++;
            i++;
        } while (i < count);
    }
    if (work->owner->active == 0) {
        func_00197D50(work->owner);
    }
    func_003297C8(work->handle);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018B628);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018B778);

void func_0018B9F8(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0018BA08(EffPCPWork *work, f32 val) {
    work->unkA0 = val;
}

void func_0018BA10(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0xa4) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018BA18);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018BB38);

void func_0018BC28(void *data) {
    void *work0;
    void *work1;
    void *work2;

    work0 = effParamTableGetBlock(data, 0);
    work1 = effParamTableGetBlock(data, 1);
    work2 = effParamTableGetBlock(data, 2);
    func_0018BB38(work0, work1, work2);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018BC90);

INCLUDE_ASM(const s32, "effect/effPCPMisc", effDestroyParticleEvents);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018BD88);

INCLUDE_RODATA(const s32, "effect/effPCPMisc", D_00414610);

INCLUDE_SDATA(const s32, "effect/effPCPMisc", D_00436438);

INCLUDE_SDATA(const s32, "effect/effPCPMisc", D_0043643C);

INCLUDE_SDATA(const s32, "effect/effPCPMisc", D_0043643D);
