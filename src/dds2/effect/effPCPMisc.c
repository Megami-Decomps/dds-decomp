#include "common.h"
#include "pcp_vu0.h"
extern u32 effMiscRand(void *state);
#include "pcp_vu0.h"
#include "ee_mmi.h"
extern void effDispatchParameterDataAndFreeWork(u32);
extern void effDestroyNode(u32);
extern u8 *func_0016D290(u32 handle);
extern void func_0016D9D8(u32 handle);
extern u32 sdfCountMapPositionRecords(void *model);
extern u32 func_00197D68(void *owner, s32 kind, void *place);



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
    u32 baseColor;    /* 0x1344 initialised to grey 0x80808080 */
    u32 secondaryHandle; /* 0x1348: parameter block 1 */
    u32 primaryHandle;   /* 0x134C: parameter block 0 */
    u32 allocationHandle; /* 0x1350: backing allocation */
} EffPCPChargeWork;

/* Prefix of the SDK model data used to query attachment positions. */
typedef struct EffPCPMapSource {
    u8 pad00[0x18];
    u32 mapHandle;
} EffPCPMapSource;

/* Compact spawn-once work: cleared header words, grey colour, and two
   independently retained parameter handles released on destroy. */
typedef struct {
    u32 unk00;    /* 0x00 cleared on init */
    u32 unk04;    /* 0x04 cleared on init */
    u32 unk08;    /* 0x08 cleared on init */
    u8 pad0C[0x4]; /* 0x0C */
    u32 color;    /* 0x10 initialised to grey 0x80808080 */
    u32 primaryHandle;   /* 0x14 parameter block 0 */
    u32 secondaryHandle; /* 0x18 parameter block 1 */
} EffPCPSpawnOnceWork;

extern void *func_00328D68(s32 size);
extern void sdfReleaseChipBlock();

extern u32 effParamCreateFromTable(void *data, s32 index);

extern u32 effParamWorkDuplicate(u32 param);

extern u32 effCreateThunderCellSystemWork(void *params);

extern u8 D_003B1938[];


extern u8 D_003B19E8[];





extern u8 D_003B1A38[];

extern u8 D_003B1A88[];

extern struct EffPCPSpanWork *effPcpSpanCreate(void *param0, void *param1);

extern u32 effCloneSourceWithTypeHandler(u32 handle);

extern void sdfComposeVuMatrixFromRegisters(void);

extern void func_00336538(f32 scale);
extern void func_00336818(f32 angle);

/* Block `index` of a packed effect parameter set: data + offset table entry. */
extern void *effParamTableGetBlock(void *data, s32 index);

extern void effPcpCopyBlockMatrix(void *dst, void *src);


extern void effPcpCopyVector60(void *dst, void *src);
/* The 0x50-byte parameter block copied by all block-set clones. */
typedef struct EffPCPBlockSetParams {
    u32 unk00[7];
    s32 groupSize[3];
    u32 unk28[10];
} EffPCPBlockSetParams;

/* One 0x10C record for construction, cloning, setters and teardown.
   A non-NULL source borrows its resources; NULL owns the listed handles. */
typedef struct EffPCPBlockSetWork {
    f32 matrix[16];
    u8 pad40[0x20];
    EffPCPBlockSetParams params;
    u32 unkB0;
    u32 count;
    u32 color;
    u32 mode;
    u32 headHandle;
    u32 handleA[5];
    u32 *list[3];
    u32 handleB[5];
    u32 tailHandle;
    u32 alloc[3];
    struct EffPCPBlockSetWork *source;
} EffPCPBlockSetWork;

/* Three frame thresholds and their block-set inputs form the copied prefix. */
typedef struct EffPCPRotateParams {
    s32 startFrame[3];
    EffPCPBlockSetParams group[3];
} EffPCPRotateParams;

typedef struct EffPCPRotateWork {
    EffPCPRotateParams params;
    s32 ids[3];
    s32 frame;
} EffPCPRotateWork;

extern void effPcpBlockSetWorkRelease(EffPCPBlockSetWork *work);

extern s8 D_0043643C;

extern u32 effThunderFragCreate(void *params);

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

/* Shared trail allocation is 0x3C; each referencing task owns one u32 frame.
   The private trail is a different 0x38-byte record, without unk24. */
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
    struct EffPCPTrailObj *obj;
} EffPCPSharedTrail;

extern EffPCPSharedTrail *effPcpSharedTrailWork;

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

/* The two compact fade variants share their 0x3C work; only the SDK
   resource payload copied during respawn differs between variants. */
typedef struct {
    u8 pad00[0x10];
    u8 flags;
    u8 pad11[3];
    u32 color;
    u32 unk18;
    u32 unk1C;
    u32 unk20;
    u32 unk24;
    u32 unk28;
    u32 unk2C;
    u32 unk30;
    f32 unk34;
    u32 resource;
} EffPCPCompactFadeWork;

extern u32 effCloneResourceTemplate(void *params);

extern u32 effCloneBlurTemplate(void *params);

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

extern u32 effCloneBlurWorkWithSlots(void *params);

/* Shared 0x24-byte thunder ray work. The renderer places its first point
   startDistance along the muzzle direction, then advances by length. */
typedef struct {
    f32 pos[4];
    u32 unk10;
    u8 pad14[4];
    f32 startDistance;
    f32 length;
    u32 handle;
} EffPCPBurstWork;

extern u8 D_003B1B50[];

extern u8 D_003B1C10[];

extern u8 D_003B1CD0[];

extern u8 D_003B1D90[];

extern u8 D_003B1E50[];

extern u8 D_003B1F10[];

struct EffPCPBeamWork;
extern void func_00188198(f32 value, struct EffPCPBeamWork *work);


extern void func_00336798(f32 angle);

extern void effPcpBuildConcentricRingPoints(void *work, f32 radius);


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

/* Six delayed resource pairs share this 0x60-byte allocation throughout
   creation, cloning, rerolling, update and release. */
typedef struct {
    u32 unk00;
    u32 unk04;
    u32 unk08;
    u8 pad0C[4];
    u32 color;
    f32 unk14; /* Settable size input; not read by the observed update. */
    u32 handle[12];
    u32 delay[6];
} EffPCPDelayedPairs;

extern void effPcpDelayedPairsRerollSlot(EffPCPDelayedPairs *work, s32 index);

extern void *func_003292A8(s32 size);

extern void *sdfResourceRetainAddress(void *resource);

extern void effPcpViewAlignedRingSetPosition();

extern void effPcpCopyTwinVector();

extern void effPcpCopyStaggerVector();

extern void effPcpCopyCrossVector();

extern void effPcpCopyDelayedPairVector();

extern void effPcpCopySpawnOnceVector();

extern void effPcpCopyRadialParticleVector();

extern void effPcpCopyNarrowConeVector();

extern void effPcpCopyVariableHeightVector();

extern void effPcpCopyWideConeVector();

extern void effPcpCopyFixedOriginVector();

extern void effPcpCopySideBurstVector();

extern void effPcpCopyThunderHandleVector();

extern void effPcpCopyDenseConeVector();

extern void effPcpCopyGrowingThunderFadeVector();

extern void effPcpCopySharedTrailVector();

extern void func_001832C8();

extern void func_00183620();

extern void effPcpCopyCompactBlurVector();

extern void effPcpCopyShortThunderFadeVector();

extern void effPcpCopyScalingThunderFadeVector();

extern void effPcpCopyThunderScaleFadeVector();

extern void effPcpCopySpanVector();

extern void effPcpCopySpinSingleVector();

extern void func_00187118();

extern void func_00187418();

extern void effPcpCopyThunderBurstVector();

extern void effPcpCopyScaledThunderVector();

extern void effPcpCopyBeamVector();

extern void effPcpCopyLinkedBeamVector();

extern void effPcpCopyGroupSetVector();

extern void effPcpCopyCaptureNodeVector();

extern void effPcpCopyDriftVector();

extern void effPcpCopyRandomizedVector();

extern void effPcpCopySpawnRangeVector();

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
    u32 frame;          /* 0x14: shared callback starts after frame 24 */
    f32 scale;          /* 0x18 */
    u32 pair[8][2];     /* 0x1C parameter handle pairs (source uses [0] and [1]) */
    u32 shared[8];      /* 0x5C */
    u32 state[8];       /* 0x7C */
    u32 counter[8];     /* 0x9C */
} EffPCPTwinWork; /* 0xBC */

extern void effTwinEffectRerollSlot(EffPCPTwinWork *work, s32 index);

extern void *effParamWorkGetData(u32 handle);

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

extern void func_003365B8(f32 angle);

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


/* Timer constructors copy these parameter prefixes before initializing
   the live source/target colors and frame counter. */
typedef struct EffPCPFadeTimerParams {
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
    u32 color;
    u8 pad10[4];
    u32 unk14;
    u32 unk18;
    u32 width;
    u32 height;
} EffPCPFadeTimerParams;

typedef struct EffPCPFadeTimer {
    EffPCPFadeTimerParams params;
    u32 colorFrom;
    u32 colorTo;
    s32 frame;
} EffPCPFadeTimer;

typedef struct EffPCPFadeTimerLongParams {
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
    u32 color;
    u8 pad10[0x0C];
    u32 unk1C;
    u32 unk20;
    u32 unk24;
    u32 unk28;
    u32 width;
    u32 height;
} EffPCPFadeTimerLongParams;

typedef struct EffPCPFadeTimerLong {
    EffPCPFadeTimerLongParams params;
    u32 colorFrom;
    u32 colorTo;
    s32 frame;
} EffPCPFadeTimerLong;

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
    u32 color;
    u32 handleA[7];
    u32 handleB[7];
    u32 handleC[7];
} EffPCPTripleWork;

extern u32 func_001578C0(u32 param);

extern EffPCPBlockSetWork *effPcpBuildBlockSet();

extern EffPCPBlockSetWork *func_00185118(void *first, void **blocks);

typedef struct {
    void *block1;
    void *group[5];
    void *block7;
    void *block8;
    void *block9;
    void *tail[5];
    void *block15;
} EffPCPBlockSet;

extern void effPcpCopyVectorAF0(void *dst, void *src);

extern void func_00184B10(void *dst, void *src);



extern u8 *func_00187E50(u32);


/* Both beam clones own this node. The color array, point array, two
   transforms and release handles are parts of one SDK allocation. */
typedef struct EffPCPBeamNode {
    f32 matrix[16];
    f32 localMatrix[16];
    u128 position;
    f32 scale;
    u32 drawKind; /* Indexes the node draw-dispatch table, not a color. */
    u32 color;
    u32 vertexCount;
    f32 *points;
    u32 *colors;
    u32 assetHandle;
    u32 allocationHandle;
} EffPCPBeamNode;

typedef struct EffPCPBeamParams {
    u8 pad00[0x28];
    f32 unk28;
    u8 pad2C[4];
    u32 segments;          /* 0x30 */
    u32 drawKind;          /* 0x34 */
    f32 firstWidth;
    u32 firstColor;            /* 0x3C */
    f32 middleWidth;
    u32 middleColor;           /* 0x44 */
    f32 lastWidth;
    u32 lastColor;             /* 0x4C */
} EffPCPBeamParams;

typedef struct EffPCPBeamWork {
    EffPCPBeamParams params;
    u32 unk50;
    u32 color;                 /* 0x54 */
    u32 vertexCount;       /* 0x58 */
    EffPCPBeamNode *node;   /* 0x5C */
} EffPCPBeamWork;

/* The ring geometry, angle preparation and timeline operate on the same
   0x80-byte clone. Radius and rotation each have independent decay inputs. */
/* The 0x5C-byte input prefix ends at lastColor; animation state follows it. */
typedef struct EffPCPBeamLargeHead {
    f32 pos[4];
    f32 degreesA;
    f32 degreesB;
    f32 stepDegrees;
    f32 rotationDecay;
    u8 mode;
    u8 pad21[3];
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
    f32 initialRadius;
    f32 initialRadiusStep;
    f32 radiusDecay;
    u32 segments;
    u32 drawKind;
    f32 radiusStepA;
    u32 firstColor;
    f32 radiusStepB;
    u32 middleColor;
    f32 radiusStepC;
    u32 lastColor;
} EffPCPBeamLargeHead;

typedef struct EffPCPBeamLargeWork {
    EffPCPBeamLargeHead head;
    s32 frame;
    u32 color;
    f32 rotationA;
    f32 rotationB;
    f32 rotationStep;
    u8 pad70[4];
    f32 radius;
    f32 radiusStep;
    EffPCPBeamNode *node;
} EffPCPBeamLargeWork;

extern f32 effMiscRandUnitFloat(void *state);

extern u8 D_003AA868[];

extern f32 effMiscRandUnitFloat(void *state);

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

extern void mdlProcessContextNodesAndTransforms(void *obj, void *table);

extern void mdlStorePrimaryVectorVU(void *obj);

extern void sdfLoadMapRecordPositionVector(u32 handle, s32 value);

extern u8 D_00380828[];

extern void mdlBroadcastMasked(void *obj, u32 mask);

extern void effParamWorkCallback1(u32 handle, f32 value);


extern void effParamWorkInvokeCallback(u32 handle);

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


extern void effEventReleaseNode(void *event);

extern void func_00197D50();


extern void effPCPThunderFree3(u32 handle);

extern void func_00187AA8(void *work);

/* Effect initializers implemented in assembly below (func_001708A0 lives in
   another unit). Each is entered with and without spawn arguments, so they
   are declared unchecked. */
extern void *effPcpCreateDelayedEventEntries();

/* Effect initializers implemented in assembly below (func_001708A0 lives in
   another unit). Each is entered with and without spawn arguments, so they
   are declared unchecked. */
extern void *effPcpEventWorkCreate();

typedef struct EffPCPRingWork {
    f32 pos[4];
    u32 color10;
    u32 color14;
    f32 scale;
    s32 handle;
} EffPCPRingWork;
void effPcpDispatchKindAndRelease(EffPCPRingWork *work) {
    billDispatchByKind((u32)work->handle);
    sdfReleaseChipBlock(work);
}

extern u8 sdfViewEyeVector[];
extern u8 sdfViewTargetVector[];
extern void effCopyVector(s32 handle, f32 *src);
extern void billInvokeCallback(s32 handle);
extern void billSetChildScaleComponents(s32 handle, f32 sx, f32 sy);
extern void billSetChildParameter(s32 handle, u32 color);
extern u32 effMultiplyPackedColors(u32 flags, u32 color);


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

    VU0_LOAD_VF($vf10, sdfViewTargetVector);
    VU0_LOAD_VF($vf11, sdfViewEyeVector);
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

void func_0017EF60(EffPCPRingWork *work, u32 value) {
    work->color14 = value;
}

void effPcpViewAlignedRingSetScale(EffPCPRingWork *work, f32 val) {
    work->scale = val;
}

/* Re-rolls slot `index`: random-angle rotation matrix pushed to both handles
 * of the pair, then a new random countdown. */
void effTwinEffectRerollSlot(EffPCPTwinWork *work, s32 index) {
    u128 mtx[4];

    func_00336538((effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f * 6.283185f);
            VU0_STORE_MATRIX_UNCLOBBERED(mtx);

;
    effParamWorkCallback2(work->pair[index][0], mtx);
    effParamWorkCallback2(work->pair[index][1], mtx);
    mdlAddEntryPlain(effParamWorkGetData(work->pair[index][0]), 0, 0);
    mdlAddEntryPlain(effParamWorkGetData(work->pair[index][1]), 0, 0);
    work->counter[index] = effMiscRand(D_003AA868) % 10;
}

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

void effTwinEffectRelease(EffPCPTwinWork *work) {
    s32 *a = (s32 *)work->shared;
    s32 *b = (s32 *)work->pair;
    s32 i;

    for (i = 0; i < 8; i++) {
        effDispatchParameterDataAndFreeWork(a[0]);
        effDispatchParameterDataAndFreeWork(b[1]);
        effDispatchParameterDataAndFreeWork(b[0]);
        a++;
        b += 2;
    }
    sdfReleaseChipBlock(work);
}

EffPCPTwinWork *effTwinEffectClone(EffPCPTwinWork *src) {
    EffPCPTwinWork *work = func_00328D68(0xBC);
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
        mdlProcessContextNodesAndTransforms(obj[0], D_00380828);
        sdfLoadMapRecordPositionVector(((EffPCPMapSource *)obj[0])->mapHandle, 1);
        posp = &pos;
        VU0_STORE_VF_UNCLOBBERED(vf10, posp);
        mdlStorePrimaryVectorVU(obj[1]);
        mdlProcessContextNodesAndTransforms(obj[1], D_00380828);
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

void effPcpSetTwinEffectScale(EffPCPTwinWork *work, f32 val) {
    work->scale = val;
}

void effPcpSetTwinEffectColor(EffPCPTwinWork *work, u32 value) {
    work->color = value;
}

/* Re-rolls slot `index` of the staggered effect: random-angle rotation matrix
 * for both handles, new random offset and delay. */
void effPcpStaggerRerollSlot(EffPCPStaggered *work, s32 index) {
    u128 mtx[4];

    func_00336538((effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f * 6.283185f);
            VU0_STORE_MATRIX_UNCLOBBERED(mtx);

;
    effParamWorkCallback2(work->handle[index * 2], mtx);
    effParamWorkCallback2(work->handle[index * 2 + 1], mtx);
    mdlAddEntryPlain(effParamWorkGetData(work->handle[index * 2]), 0, 0);
    mdlAddEntryPlain(effParamWorkGetData(work->handle[index * 2 + 1]), 0, 0);
    work->offset[index] = effMiscRandUnitFloat(D_003AA868) * 150.0f;
    work->delay[index] = effMiscRand(D_003AA868) % 10;
}


EffPCPStaggered *effPcpStaggerCreate(void *args) {
    EffPCPStaggered *work = func_00328D68(0x98);
    u32 *handle = &work->handle[1];
    s32 i = 0;

    do {
        if (i == 0) {
            work->handle[0] = effParamCreateFromTable(args, 0);
            work->handle[1] = effParamCreateFromTable(args, 1);
        } else {
            handle[-1] = effParamWorkDuplicate(work->handle[0]);
            handle[0] = effParamWorkDuplicate(work->handle[1]);
        }
        handle += 2;
        effPcpStaggerRerollSlot(work, i++);
    } while (i < 8);
    work->color = 0x80808080;
    work->scale = 1.0f;
    work->x = 0;
    work->y = 0;
    work->z = 0;
    return work;
}

void effPcpStaggerRelease(EffPCPStaggered *work) {
    s32 *p = (s32 *)&work->handle[1];
    s32 i;

    for (i = 0; i < 8; i++) {
        effDispatchParameterDataAndFreeWork(p[-1]);
        effDispatchParameterDataAndFreeWork(p[0]);
        p += 2;
    }
    sdfReleaseChipBlock(work);
}

EffPCPStaggered *effCreatePairedResourceWork(EffPCPStaggered *source) {
    EffPCPStaggered *work = func_00328D68(0x98);
    u32 *handle = &work->handle[1];
    s32 i = 0;

    do {
        handle[-1] = effParamWorkDuplicate(source->handle[0]);
        handle[0] = effParamWorkDuplicate(source->handle[1]);
        handle += 2;
        effPcpStaggerRerollSlot(work, i);
        i++;
    } while (i < 8);
    work->color = 0x80808080;
    work->scale = 1.0f;
    work->x = 0;
    work->y = 0;
    work->z = 0;
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
            mdlProcessContextNodesAndTransforms(obj[0], D_00380828);
            sdfLoadMapRecordPositionVector(((EffPCPMapSource *)obj[0])->mapHandle, 1);
            mdlStorePrimaryVectorVU(obj[1]);
            mdlProcessContextNodesAndTransforms(obj[1], D_00380828);
        }
    }
}

void effPcpCopyStaggerVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetStaggerEffectScale(EffPCPStaggered *work, f32 val) {
    work->scale = val;
}

void effPcpSetStaggerEffectColor(EffPCPStaggered *work, u32 value) {
    work->color = value;
}

/* Spawns cross arm (i, j): rotates the model by j sixths of a turn about the axis chosen by i. */
void effCrossArmSpawn(EffPCPCrossWork *work, u32 i, u32 j) {
    u128 mtx[4];
    f32 angle;

    mdlAddEntryPlain(effParamWorkGetData(work->handle[i][j]), 0, 0);
    angle = (f32)j * -1.0471974f;
    if (i == 0) {
        func_00336538(angle);
    } else if (i == 1) {
        func_00336538(-angle);
    } else if (i == 2) {
        func_003365B8(-angle);
    } else {
        func_003365B8(angle);
    }
        VU0_STORE_MATRIX(mtx);
;
    effParamWorkCallback2(work->handle[i][j], mtx);
    if ((j + 1) & 1) {
        work->state[i][j] = 0;
    } else {
        work->state[i][j] = 5;
    }
}

EffPCPCrossWork *effCrossEffectCreateFromTable(void *src) {
    EffPCPCrossWork *work = func_00328D68(0xDC);
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

/* Destroys the cross effect: releases the main handle and all 12 group handles. */
void effCrossEffectRelease(EffPCPCrossWork *work) {
    s32 i;
    s32 j;

    effDispatchParameterDataAndFreeWork(work->base);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            effDispatchParameterDataAndFreeWork(work->handle[i][j]);
        }
    }
    sdfReleaseChipBlock(work);
}

EffPCPCrossWork *effCrossEffectClone(EffPCPCrossWork *src) {
    EffPCPCrossWork *work = func_00328D68(0xDC);
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

/* Per-frame update of the cross effect: scales the anchor model and fires each
 * of the 12 slots whose countdown has expired. */
void effCrossEffectUpdate(EffPCPCrossWork *work) {
    EffPCPMapSource *anchor;
    void *obj;
    s32 i;
    s32 j;

    anchor = effParamWorkGetData(work->base);
    VU0_LOAD_VF(vf10, work);
    mdlStorePrimaryVectorVU(anchor);
    effParamWorkCallback1(work->base, work->scale);
    mdlProcessContextNodesAndTransforms(anchor, D_00380828);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            if (work->state[i][j] != 0) {
                work->state[i][j]--;
                continue;
            }
            obj = effParamWorkGetData(work->handle[i][j]);
            mdlBroadcastMasked(obj, work->color);
            sdfLoadMapRecordPositionVector(anchor->mapHandle, i * 4 + j + 1);
            mdlStorePrimaryVectorVU(obj);
            mdlProcessContextNodesAndTransforms(obj, D_00380828);
        }
    }
}

void effPcpCopyCrossVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetCrossEffectScale(EffPCPCrossWork *work, f32 val) {
    work->scale = val;
}

void effPcpSetCrossEffectColor(EffPCPCrossWork *work, u32 value) {
    work->color = value;
}


/* Re-arms slot `index`: registers both handles of the pair and rolls a new
 * random countdown. */
void effPcpDelayedPairsRerollSlot(EffPCPDelayedPairs *work, s32 index) {

    mdlAddEntryPlain(effParamWorkGetData(work->handle[index * 2]), 0, 0);
    mdlAddEntryPlain(effParamWorkGetData(work->handle[index * 2 + 1]), 0, index & 1);
    work->delay[index] = effMiscRand(D_003AA868) % 10;
}


EffPCPDelayedPairs *effCreateIndexedResourceWork(void *source) {
    EffPCPDelayedPairs *work = func_00328D68(0x60);
    u32 *handle = &work->handle[1];
    s32 i;

    for (i = 0; i < 6; i++) {
        if (i == 0) {
            work->handle[1] = effParamCreateFromTable(source, 6);
        }
        handle[-1] = effParamCreateFromTable(source, i);
        handle[0] = effParamWorkDuplicate(work->handle[1]);
        handle += 2;
        effPcpDelayedPairsRerollSlot(work, i);
    }
    work->color = 0x80808080;
    work->unk14 = 1.0f;
    work->unk00 = 0;
    work->unk04 = 0;
    work->unk08 = 0;
    return work;
}

void effPcpDelayedPairsRelease(EffPCPDelayedPairs *work) {
    s32 *p = (s32 *)&work->handle[1];
    s32 i;

    for (i = 0; i < 6; i++) {
        effDispatchParameterDataAndFreeWork(p[-1]);
        effDispatchParameterDataAndFreeWork(p[0]);
        p += 2;
    }
    sdfReleaseChipBlock(work);
}

EffPCPDelayedPairs *effCopyIndexedResourceWork(EffPCPDelayedPairs *source) {
    u32 *sourceHandle;
    u32 *workHandle;
    s32 i;
    EffPCPDelayedPairs *work = func_00328D68(0x60);
    sourceHandle = &source->handle[1];
    workHandle = &work->handle[1];

    for (i = 0; i < 6; i++) {
        workHandle[-1] = effParamWorkDuplicate(sourceHandle[-1]);
        workHandle[0] = effParamWorkDuplicate(sourceHandle[0]);
        sourceHandle += 2;
        workHandle += 2;
        effPcpDelayedPairsRerollSlot(work, i);
    }
    work->color = 0x80808080;
    work->unk14 = 1.0f;
    work->unk00 = 0;
    work->unk04 = 0;
    work->unk08 = 0;
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
            obj[0] = effParamWorkGetData(work->handle[i * 2]);
            obj[1] = effParamWorkGetData(work->handle[i * 2 + 1]);
                        VU0_LOAD_VF_MEMORY(vf10, work);
            mdlStorePrimaryVectorVU(obj[0]);
            mdlBroadcastMasked(obj[1], work->color);
            mdlProcessContextNodesAndTransforms(obj[0], D_00380828);
            sdfLoadMapRecordPositionVector(((EffPCPMapSource *)obj[0])->mapHandle, 1);
                        VU0_STORE_VF_UNCLOBBERED(vf10, &vec);
            mdlStorePrimaryVectorVU(obj[1]);
            mdlProcessContextNodesAndTransforms(obj[1], D_00380828);
        }
    }
}

void effPcpCopyDelayedPairVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetDelayedPairScale(EffPCPDelayedPairs *work, f32 val) {
    work->unk14 = val;
}

void effPcpSetDelayedPairColor(EffPCPDelayedPairs *work, u32 value) {
    work->color = value;
}

void effPcpChargeInitTail(EffPCPChargeWork *work) {
    work->unk133C = 0;
    work->unk1340 = 0;
    work->baseColor = 0x80808080;
}

EffPCPChargeWork *effCreateChargeWork(void *source) {
    void *resource = func_003292A8(0x1354);
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
    effDispatchParameterDataAndFreeWork(work->primaryHandle);
    effDispatchParameterDataAndFreeWork(work->secondaryHandle);
    func_003297C8(work->allocationHandle);
}

EffPCPChargeWork *effCopyChargeResources(EffPCPChargeWork *source) {
    void *resource = func_003292A8(0x1354);
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

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001803E8);

void effPcpCopyVectorAF0(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0xaf0, src);
}

void effPcpChargeSetScale(EffPCPChargeWork *work, f32 val) {
    work->scale = val;
}

void effPcpChargeSetColor(EffPCPChargeWork *work, u32 value) {
    work->color = value;
}

EffPCPSpawnOnceWork *effPcpSpawnOnceCreate(void *src) {
    EffPCPSpawnOnceWork *dst;
    u32 handle;

    dst = func_00328D68(0x1C);
    dst->primaryHandle = effParamCreateFromTable(src, 0);
    handle = effParamCreateFromTable(src, 1);
    dst->unk00 = 0;
    dst->secondaryHandle = handle;
    dst->unk04 = 0;
    dst->color = 0x80808080;
    dst->unk08 = 0;
    return dst;
}

void effPcpSpawnOnceRelease(EffPCPSpawnOnceWork *work) {
    effDispatchParameterDataAndFreeWork(work->primaryHandle);
    effDispatchParameterDataAndFreeWork(work->secondaryHandle);
    sdfReleaseChipBlock(work);
}

EffPCPSpawnOnceWork *effPcpSpawnOnceClone(EffPCPSpawnOnceWork *src) {
    EffPCPSpawnOnceWork *dst;
    u32 handle;

    dst = func_00328D68(0x1C);
    dst->primaryHandle = effParamWorkDuplicate(src->primaryHandle);
    handle = effParamWorkDuplicate(src->secondaryHandle);
    dst->unk00 = 0;
    dst->secondaryHandle = handle;
    dst->unk04 = 0;
    dst->color = 0x80808080;
    dst->unk08 = 0;
    return dst;
}

void effPcpSpawnOnce(EffPCPSpawnOnceWork *work) {
    void *obj;
    u128 vec;

    obj = effParamWorkGetData(work->primaryHandle);
        VU0_LOAD_VF_MEMORY(vf10, work);
    mdlStorePrimaryVectorVU(obj);
    effParamWorkCallback3(work->secondaryHandle, work->color);
    mdlProcessContextNodesAndTransforms(obj, D_00380828);
    sdfLoadMapRecordPositionVector(((EffPCPMapSource *)obj)->mapHandle, 1);
        VU0_STORE_VF_TO_MEMORY(vf10, vec);
    effParamWorkCallback0(work->secondaryHandle, &vec);
    effParamWorkInvokeCallback(work->secondaryHandle);
}

void effPcpCopySpawnOnceVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSpawnOnceSetColor(EffPCPSpawnOnceWork *work, u32 value) {
    work->color = value;
}

/* Shared scratch parameter block handed to the particle spawner (D_003B1670)
 * and the shared spawn origin vector at +0x10 (D_003B1680, separate symbol). */
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

/* The 8-, 12- and 30-particle variants allocate a common prefix followed
   by their actual handle count. Spawn, shift and release use the same tail. */
typedef struct EffPCPThunderGroup {
    f32 pos[4];
    u32 unk10;
    f32 unk14; /* Settable, but not read by the observed group updates. */
    u32 unk18;
    u32 handles[1];
} EffPCPThunderGroup;

#define EFF_DEG2RAD 0.017453292f

extern EffSpawnParams D_003B1670[];
extern u8 D_003B1680[];
extern f32 sdfSinPoly(f32);
extern f32 sdfEvaluateCosineViaSinePhaseShift(f32);
extern void sdfBuildVuRotationFromAxisAngle(f32 *axis, f32 angle);

/* Spawns 12 particles in a ring: every second particle advances the ring angle
 * (60 degrees). Direction is normalised on the VU, scaled per axis and offset
 * by the origin vector; short-reach particles get a shorter life. */
void effPcpInitTwelveRadialParticles(EffPCPThunderGroup *group) {
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
        spread = effMiscRandUnitFloat(D_003AA868) * 0.5f + 0.5f;
        D_003B1670->unk44 = spread * 1.25f;
        D_003B1670->unk4C = spread * 12.5f;
        radius = (effMiscRandUnitFloat(D_003AA868) * 0.25f + 0.75f) * 100.0f;
        D_003B1670->vel[1] = 0;
        D_003B1670->vel[0] = sinv * radius;
        D_003B1670->vel[2] = cosv * radius;
        dir[0] = sinv;
        dir[1] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f + 2.0f;
        dir[2] = cosv;
        VU0_LOAD_VF($vf10, dir);
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF($vf10, dir);
        reach = (effMiscRandUnitFloat(D_003AA868) * 0.65f + (1.0f - 0.65f)) * 350.0f;
        scale[2] = reach;
        scale[0] = reach;
        scale[1] = -reach;
        VU0_LOAD_VF($vf10, dir);
        VU0_LOAD_VF($vf11, scale);
        VU0_MUL(vf10, vf10, vf11);
        VU0_LOAD_VF($vf11, D_003B1680);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF($vf10, D_003B1670);
        if (scale[0] < 250.0f) {
            speed = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f * 2.5f + 5.0f;
            life = effMiscRand(D_003AA868) % 5 + 5;
        } else {
            speed = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f * 5.0f + 10.0f;
            life = effMiscRand(D_003AA868) % 6 + 15;
        }
        D_003B1670->lifetime = life;
        D_003B1670->speed = speed;
        *out = effThunderFragCreate(D_003B1670);
        out++;
        i++;
    } while (i < 12);
}

void *effPcpCreateTwelveRadialParticles(void) {
    void *work;

    work = func_00328D68(0x4c);
    effPcpInitTwelveRadialParticles(work);
    return work;
}

typedef struct MenuPanelChildren1C {
    u8 pad00[0x1C];
    void *children[30];
} MenuPanelChildren1C;

void effPcpReleaseTwelveRadialParticles(EffPCPThunderGroup *group) {
    s32 i;
    for (i = 0; i < 12; i++) {
        effPCPThunderFree3(group->handles[i]);
    }
    sdfReleaseChipBlock(group);
}

void *effPcpAllocateRadialParticleGroup(void) {
    void *work;

    work = func_00328D68(0x4c);
    effPcpInitTwelveRadialParticles(work);
    return work;
}


/* Shifts the first `count` handles' objects by the work position for the
 * duration of their update, then puts the original vectors back. */
static inline void effPcpShiftThunderHandles(EffPCPThunderGroup *work, s32 count) {
    u128 saved[2];
    u8 *obj;
    s32 i;

    for (i = 0; i < count; i++) {
        obj = func_0016D290(work->handles[i]);
        VU0_LOAD_VF($vf10, obj + 0x10);
        VU0_STORE_VF($vf10, &saved[1]);
        VU0_LOAD_VF($vf11, work);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF($vf10, obj + 0x10);
        VU0_LOAD_VF($vf10, obj);
        VU0_STORE_VF($vf10, &saved[0]);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF($vf10, obj);
        func_0016D9D8(work->handles[i]);
        PCP_COPY_VECTOR(obj, &saved[0]);
        PCP_COPY_VECTOR(obj + 0x10, &saved[1]);
    }
}

void effPcpThunderShiftUpdateA(EffPCPThunderGroup *work) {
    effPcpShiftThunderHandles(work, 12);
}

void effPcpCopyRadialParticleVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetRadialParticleScale(EffPCPThunderGroup *work, f32 val) {
    work->unk14 = val;
}

void effPcpSetRadialParticleColor(EffPCPThunderGroup *work, u32 value) {
    work->unk18 = value;
}

extern EffSpawnParams D_003B16D0[];

/* 12-piece spread on a cone around a random axis (rotation matrix built by
 * sdfBuildVuRotationFromAxisAngle into the VU0 matrix registers). */
void effPcpSpawnConeTwelve(EffPCPThunderGroup *group) {
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
        spread = effMiscRandUnitFloat(D_003AA868) * 0.5f + 0.5f;
        D_003B16D0->unk44 = spread * 1.25f;
        D_003B16D0->unk4C = spread * 12.5f;
        radius = (effMiscRandUnitFloat(D_003AA868) * 0.25f + 0.75f) * 100.0f;
        axis[0] = cosv;
        axis[1] = 0;
        axis[2] = -sinv;
        D_003B16D0->vel[1] = 0;
        D_003B16D0->vel[0] = sinv * radius;
        D_003B16D0->vel[2] = cosv * radius;
        sdfBuildVuRotationFromAxisAngle(axis, -(effMiscRandUnitFloat(D_003AA868) * (50.0f * EFF_DEG2RAD) + 5.0f * EFF_DEG2RAD));
        height = (effMiscRandUnitFloat(D_003AA868) * 0.65f + (1.0f - 0.65f)) * 500.0f;
        D_003B16D0->pos[0] = 0;
        D_003B16D0->pos[2] = 0;
        D_003B16D0->pos[1] = -height;
        VU0_LOAD_VF($vf10, D_003B16D0->pos);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF($vf10, D_003B16D0->pos);
        D_003B16D0->pos[0] += D_003B16D0->vel[0];
        D_003B16D0->pos[2] += D_003B16D0->vel[2];
        if (height < 300.0f) {
            speed = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f * 2.5f + 5.0f;
            life = effMiscRand(D_003AA868) % 5 + 5;
        } else {
            speed = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f * 5.0f + 10.0f;
            life = effMiscRand(D_003AA868) % 6 + 15;
        }
        D_003B16D0->lifetime = life;
        D_003B16D0->speed = speed;
        group->handles[i] = effThunderFragCreate(D_003B16D0);
        i++;
    } while (i < 12);
}

void *effPcpAllocateNarrowConeGroup(void) {
    void *work;

    work = func_00328D68(0x4c);
    effPcpSpawnConeTwelve(work);
    return work;
}

void effPcpNarrowConeGroupRelease(EffPCPThunderGroup *group) {
    s32 i;
    for (i = 0; i < 12; i++) {
        effPCPThunderFree3(group->handles[i]);
    }
    sdfReleaseChipBlock(group);
}

void *effPcpCreateNarrowConeParticles(void) {
    void *work;

    work = func_00328D68(0x4c);
    effPcpSpawnConeTwelve(work);
    return work;
}

void effPcpThunderShiftUpdateB(EffPCPThunderGroup *work) {
    effPcpShiftThunderHandles(work, 12);
}

void effPcpCopyNarrowConeVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetNarrowConeScale(EffPCPThunderGroup *work, f32 val) {
    work->unk14 = val;
}

void effPcpSetNarrowConeColor(EffPCPThunderGroup *work, u32 value) {
    work->unk18 = value;
}

extern EffSpawnParams D_003B1730[];

/* 30-piece spread: pieces are placed on a cone around a random axis (rotation
 * matrix built by sdfBuildVuRotationFromAxisAngle into the VU0 matrix registers). */
void effPcpSpawnVariableHeightParticles(EffPCPThunderGroup *group) {
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
        spread = effMiscRandUnitFloat(D_003AA868) * 0.5f + 0.5f;
        D_003B1730->unk44 = spread * 1.25f;
        D_003B1730->unk4C = spread * 12.5f;
        D_003B1730->vel[1] = 0;
        D_003B1730->vel[0] = sinv * ((effMiscRandUnitFloat(D_003AA868) * 0.25f + 0.75f) * 500.0f);
        D_003B1730->vel[2] = cosv * ((effMiscRandUnitFloat(D_003AA868) * 0.25f + 0.75f) * 250.0f);
        axis[0] = cosv;
        axis[1] = 0;
        axis[2] = -sinv;
        sdfBuildVuRotationFromAxisAngle(axis, -(effMiscRandUnitFloat(D_003AA868) * (55.0f * EFF_DEG2RAD) + 5.0f * EFF_DEG2RAD));
        height = (effMiscRandUnitFloat(D_003AA868) * 0.65f + (1.0f - 0.65f)) * 500.0f;
        D_003B1730->pos[0] = 0;
        D_003B1730->pos[2] = 0;
        D_003B1730->pos[1] = -height;
        VU0_LOAD_VF($vf10, D_003B1730->pos);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF($vf10, D_003B1730->pos);
        D_003B1730->pos[0] += D_003B1730->vel[0];
        D_003B1730->pos[2] += D_003B1730->vel[2];
        if (height < 300.0f) {
            speed = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f * 2.5f + 5.0f;
            life = effMiscRand(D_003AA868) % 5 + 5;
        } else {
            speed = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f * 5.0f + 10.0f;
            life = effMiscRand(D_003AA868) % 6 + 15;
        }
        D_003B1730->lifetime = life;
        D_003B1730->speed = speed;
        group->handles[i] = effThunderFragCreate(D_003B1730);
        i++;
    } while (i < 30);
}

void *effPcpAllocateVariableHeightParticleGroup(void) {
    void *work;

    work = func_00328D68(0x94);
    effPcpSpawnVariableHeightParticles(work);
    return work;
}

void effPcpWideConeGroupRelease(EffPCPThunderGroup *group) {
    s32 i;
    for (i = 0; i < 30; i++) {
        effPCPThunderFree3(group->handles[i]);
    }
    sdfReleaseChipBlock(group);
}

void *effPcpCreateVariableHeightParticles(void) {
    void *work;

    work = func_00328D68(0x94);
    effPcpSpawnVariableHeightParticles(work);
    return work;
}

void effPcpThunderShiftUpdateC(EffPCPThunderGroup *work) {
    effPcpShiftThunderHandles(work, 30);
}

void effPcpCopyVariableHeightVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetVariableHeightParticleScale(EffPCPThunderGroup *work, f32 val) {
    work->unk14 = val;
}

void effPcpSetVariableHeightParticleColor(EffPCPThunderGroup *work, u32 value) {
    work->unk18 = value;
}

extern EffSpawnParams D_003B1790[];

/* Same spread as effPcpSpawnConeTwelve with a wider cone and its own parameter block. */
void effPcpSpawnWideConeTwelve(EffPCPThunderGroup *group) {
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
        spread = effMiscRandUnitFloat(D_003AA868) * 0.5f + 0.5f;
        D_003B1790->unk44 = spread * 1.25f;
        D_003B1790->unk4C = spread * 12.5f;
        radius = (effMiscRandUnitFloat(D_003AA868) * 0.25f + 0.75f) * 100.0f;
        axis[0] = cosv;
        axis[1] = 0;
        axis[2] = -sinv;
        D_003B1790->vel[1] = 0;
        D_003B1790->vel[0] = sinv * radius;
        D_003B1790->vel[2] = cosv * radius;
        sdfBuildVuRotationFromAxisAngle(axis, -(effMiscRandUnitFloat(D_003AA868) * (55.0f * EFF_DEG2RAD) + 5.0f * EFF_DEG2RAD));
        height = (effMiscRandUnitFloat(D_003AA868) * 0.65f + (1.0f - 0.65f)) * 500.0f;
        D_003B1790->pos[0] = 0;
        D_003B1790->pos[2] = 0;
        D_003B1790->pos[1] = -height;
        VU0_LOAD_VF($vf10, D_003B1790->pos);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF($vf10, D_003B1790->pos);
        D_003B1790->pos[0] += D_003B1790->vel[0];
        D_003B1790->pos[2] += D_003B1790->vel[2];
        if (height < 300.0f) {
            speed = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f * 2.5f + 5.0f;
            life = effMiscRand(D_003AA868) % 5 + 5;
        } else {
            speed = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f * 5.0f + 10.0f;
            life = effMiscRand(D_003AA868) % 6 + 15;
        }
        D_003B1790->lifetime = life;
        D_003B1790->speed = speed;
        group->handles[i] = effThunderFragCreate(D_003B1790);
        i++;
    } while (i < 12);
}

void *effPcpAllocateWideConeGroup(void) {
    void *work;

    work = func_00328D68(0x4c);
    effPcpSpawnWideConeTwelve(work);
    return work;
}

void effPcpReleaseWideConeParticles(EffPCPThunderGroup *group) {
    s32 i;
    for (i = 0; i < 12; i++) {
        effPCPThunderFree3(group->handles[i]);
    }
    sdfReleaseChipBlock(group);
}

void *effPcpCreateWideConeParticles(void) {
    void *work;

    work = func_00328D68(0x4c);
    effPcpSpawnWideConeTwelve(work);
    return work;
}

void effPcpThunderShiftUpdateD(EffPCPThunderGroup *work) {
    effPcpShiftThunderHandles(work, 12);
}

void effPcpCopyWideConeVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetWideConeScale(EffPCPThunderGroup *work, f32 val) {
    work->unk14 = val;
}

void effPcpSetWideConeColor(EffPCPThunderGroup *work, u32 value) {
    work->unk18 = value;
}

extern EffSpawnParams D_003B17E8[];

/* 12 pieces thrown from a fixed origin along a random angle; the param block
 * (of two) is picked at random. */
void effPcpSpawnFixedOriginTwelve(EffPCPThunderGroup *group) {
    s32 i;
    EffSpawnParams *params;
    f32 angle;
    f32 dist;
    s32 life;

    for (i = 0; i < 12; i++) {
        params = &D_003B17E8[effMiscRand(D_003AA868) & 1];
        params->speed = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f * 5.0f + 20.0f;
        life = effMiscRand(D_003AA868) % 6 + 15;
        params->pos[0] = -150.0f;
        params->pos[1] = -500.0f;
        params->pos[2] = 0;
        params->lifetime = life;
        angle = effMiscRandUnitFloat(D_003AA868) * (3.14159265f / 2.0f) + 3.14159265f / 8.0f;
        dist = (effMiscRandUnitFloat(D_003AA868) * 0.25f + 0.75f) * 600.0f;
        params->vel[0] = params->pos[0] + sdfEvaluateCosineViaSinePhaseShift(angle) * dist;
        params->vel[1] = params->pos[1] + sdfSinPoly(angle) * dist;
        params->vel[2] = 0;
        group->handles[i] = effThunderFragCreate(params);
    }
}

void *effPcpAllocateFixedOriginGroup(void) {
    void *work;

    work = func_00328D68(0x4c);
    effPcpSpawnFixedOriginTwelve(work);
    return work;
}

void effPcpAngledBurstGroupRelease(EffPCPThunderGroup *group) {
    s32 i;
    for (i = 0; i < 12; i++) {
        effPCPThunderFree3(group->handles[i]);
    }
    sdfReleaseChipBlock(group);
}

void *effPcpCreateFixedOriginParticles(void) {
    void *work;

    work = func_00328D68(0x4c);
    effPcpSpawnFixedOriginTwelve(work);
    return work;
}

void effPcpThunderShiftUpdateE(EffPCPThunderGroup *work) {
    effPcpShiftThunderHandles(work, 12);
}

void effPcpCopyFixedOriginVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetFixedOriginScale(EffPCPThunderGroup *work, f32 val) {
    work->unk14 = val;
}

void effPcpSetFixedOriginParticleColor(EffPCPThunderGroup *work, u32 value) {
    work->unk18 = value;
}

extern void func_003364B8(f32 angle);
extern void func_00336898(f32 angle);
extern void sdfMultiplyVuMatrixInPlace(void);
extern EffSpawnParams D_003B1890[];

/* 8 pieces flung sideways from below; the rotation matrix is composed by the
 * three func_003364B8/336898/336B00 calls into the VU0 matrix registers. */
void effPcpSpawnSidewaysEight(EffPCPThunderGroup *group) {
    s32 i;
    EffSpawnParams *params;

    for (i = 0; i < 8; i++) {
        params = &D_003B1890[effMiscRand(D_003AA868) & 1];
        if (i & 1) {
            params->vel[0] = effMiscRandUnitFloat(D_003AA868) * 500.0f;
        } else {
            params->vel[0] = effMiscRandUnitFloat(D_003AA868) * -500.0f;
        }
        params->vel[1] = 0;
        params->vel[2] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f * 250.0f;
        params->pos[0] = 0;
        params->pos[1] = -600.0f;
        params->pos[2] = 0;
        func_003364B8((effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f * (30.0f * EFF_DEG2RAD));
        func_00336898((effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f * (30.0f * EFF_DEG2RAD));
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

    work = func_00328D68(0x3c);
    effPcpSpawnSidewaysEight(work);
    return work;
}

void effPcpSideBurstGroupRelease(EffPCPThunderGroup *group) {
    s32 i;
    for (i = 0; i < 8; i++) {
        effPCPThunderFree3(group->handles[i]);
    }
    sdfReleaseChipBlock(group);
}

void *effPcpCreateSideBurstParticles(void) {
    void *work;

    work = func_00328D68(0x3c);
    effPcpSpawnSidewaysEight(work);
    return work;
}

void effPcpThunderShiftUpdateF(EffPCPThunderGroup *work) {
    effPcpShiftThunderHandles(work, 8);
}

void effPcpCopySideBurstVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetSideBurstScale(EffPCPThunderGroup *work, f32 val) {
    work->unk14 = val;
}

void effPcpSetSideBurstColor(EffPCPThunderGroup *work, u32 value) {
    work->unk18 = value;
}

/* All four thunder fade variants allocate this 0x20-byte record.
   Their constructors, color setters, frame updates and release share it. */
typedef struct EffPCPFadeWork {
    u8 pad00[0x10];
    f32 unk10; /* Settable input; not read by the observed fade updates. */
    u32 color;
    u32 frame;
    u32 handle;
} EffPCPFadeWork;

void effPcpInitializeThunderHandleWork(EffPCPFadeWork *work) {
    u32 handle;

    handle = effCreateThunderCellSystemWork(D_003B1938);
    work->frame = 0;
    work->handle = handle;
}

void *effPcpCreateThunderHandleWork(void) {
    void *work;

    work = func_00328D68(0x20);
    effPcpInitializeThunderHandleWork(work);
    return work;
}

void effPcpReleaseThunderHandleWork(EffPCPFadeWork *work) {
    effPCPThunderFree(work->handle);
    sdfReleaseChipBlock(work);
}

void *effAllocateThunderHandleWork(void) {
    void *work;

    work = func_00328D68(0x20);
    effPcpInitializeThunderHandleWork(work);
    return work;
}

typedef struct EffPCPFadeTarget {
    u8 pad00[0x1C];
    f32 sizeX; /* animated dimension on the first axis */
    f32 sizeY; /* animated dimension on the second axis */
} EffPCPFadeTarget;


extern void *func_0016B198(u32 handle);
extern void effPCPThunderSetParam50(u32 handle, u32 value);
extern void func_0016B160(u32 handle, void *work);
extern void func_0016B928(u32 handle);
extern u32 effBlendColor(u32 colorA, u32 colorB, f32 t);

/* Scales the target over the first 22 frames, fades the colour out from frame 45. */
void effPcpThunderExpandThenFadeUpdate(EffPCPFadeWork *work) {
    EffPCPFadeTarget *target;
    f32 phase;
    u32 fadedColor;

    if (work->frame < 0x17) {
        target = func_0016B198(work->handle);
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
    func_0016B160(work->handle, work);
    func_0016B928(work->handle);
    work->frame++;
}

void effPcpCopyThunderHandleVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00182458(EffPCPFadeWork *work, f32 val) {
    work->unk10 = val;
}

void effPcpSetExpandingThunderFadeColor(EffPCPFadeWork *work, u32 value) {
    work->color = value;
}

extern EffSpawnParams D_003B1990[];

/* Same spread as effPcpSpawnVariableHeightParticles with its own parameter block. */
void effPcpSpawnConeThirty(EffPCPThunderGroup *group) {
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
        spread = effMiscRandUnitFloat(D_003AA868) * 0.5f + 0.5f;
        D_003B1990->unk44 = spread * 1.25f;
        D_003B1990->unk4C = spread * 12.5f;
        D_003B1990->vel[1] = 0;
        D_003B1990->vel[0] = sinv * ((effMiscRandUnitFloat(D_003AA868) * 0.25f + 0.75f) * 500.0f);
        D_003B1990->vel[2] = cosv * ((effMiscRandUnitFloat(D_003AA868) * 0.25f + 0.75f) * 250.0f);
        axis[0] = cosv;
        axis[1] = 0;
        axis[2] = -sinv;
        sdfBuildVuRotationFromAxisAngle(axis, -(effMiscRandUnitFloat(D_003AA868) * (55.0f * EFF_DEG2RAD) + 5.0f * EFF_DEG2RAD));
        height = (effMiscRandUnitFloat(D_003AA868) * 0.65f + (1.0f - 0.65f)) * 500.0f;
        D_003B1990->pos[0] = 0;
        D_003B1990->pos[2] = 0;
        D_003B1990->pos[1] = -height;
        VU0_LOAD_VF($vf10, D_003B1990->pos);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF($vf10, D_003B1990->pos);
        D_003B1990->pos[0] += D_003B1990->vel[0];
        D_003B1990->pos[2] += D_003B1990->vel[2];
        if (height < 300.0f) {
            speed = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f * 2.5f + 5.0f;
            life = effMiscRand(D_003AA868) % 5 + 5;
        } else {
            speed = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f * 5.0f + 10.0f;
            life = effMiscRand(D_003AA868) % 6 + 15;
        }
        D_003B1990->lifetime = life;
        D_003B1990->speed = speed;
        group->handles[i] = effThunderFragCreate(D_003B1990);
        i++;
    } while (i < 30);
}

void *effPcpAllocateDenseConeGroup(void) {
    void *work;

    work = func_00328D68(0x94);
    effPcpSpawnConeThirty(work);
    return work;
}

void effPcpReleaseConeParticleGroup(EffPCPThunderGroup *group) {
    s32 i;
    for (i = 0; i < 30; i++) {
        effPCPThunderFree3(group->handles[i]);
    }
    sdfReleaseChipBlock(group);
}

void *effPcpCreateDenseConeParticles(void) {
    void *work;

    work = func_00328D68(0x94);
    effPcpSpawnConeThirty(work);
    return work;
}

void effPcpThunderShiftUpdateG(EffPCPThunderGroup *work) {
    effPcpShiftThunderHandles(work, 30);
}

void effPcpCopyDenseConeVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetDenseConeScale(EffPCPThunderGroup *work, f32 val) {
    work->unk14 = val;
}

void effPcpSetDenseConeColor(EffPCPThunderGroup *work, u32 value) {
    work->unk18 = value;
}

void effPcpInitGrowingThunderFadeWork(EffPCPFadeWork *work) {
    u32 handle;

    handle = effCreateThunderCellSystemWork(D_003B19E8);
    work->frame = 0;
    work->handle = handle;
}

void *effPcpCreateGrowingThunderFadeWork(void) {
    void *work;

    work = func_00328D68(0x20);
    effPcpInitGrowingThunderFadeWork(work);
    return work;
}

void effPcpReleaseGrowingThunderFadeWork(EffPCPFadeWork *work) {
    effPCPThunderFree(work->handle);
    sdfReleaseChipBlock(work);
}

void *effAllocateGrowingThunderFadeWork(void) {
    void *work;

    work = func_00328D68(0x20);
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
        target = func_0016B198(work->handle);
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
    func_0016B160(work->handle, work);
    func_0016B928(work->handle);
    work->frame++;
}

void effPcpCopyGrowingThunderFadeVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00182B38(EffPCPFadeWork *work, f32 val) {
    work->unk10 = val;
}

void effPcpSetGrowingThunderFadeColor(EffPCPFadeWork *work, u32 value) {
    work->color = value;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182B48);

extern s32 D_00436438;
extern void func_0018E8F0(u32 handle);

void effPcpSharedWorkRelease(u32 *work)
{
    sdfReleaseChipBlock(work);
    if (--D_00436438 != 0) {
        return;
    }
    func_0018E8F0((u32)effPcpSharedTrailWork->obj);
    sdfReleaseChipBlock(effPcpSharedTrailWork);
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

extern s32 func_00195890(f32 value);
extern u32 effMultiplyPackedColors(u32 flags, u32 color);
extern void effDrawBlurPixelRectWithResource(EffPCPTrailObj *obj);


#define EFF_SHARED_TRAIL ((EffPCPSharedTrail *)effPcpSharedTrailWork)

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
        obj->size = (s32)((f32)func_00195890(EFF_SHARED_TRAIL->unk1C) * EFF_SHARED_TRAIL->scale);
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
    PCP_COPY_VECTOR(effPcpSharedTrailWork, src);
}

void func_00182DB8(u32 unused, u32 val) {
    effPcpSharedTrailWork->flags = val;
}

void effSetSharedScale(u32 unused, f32 value) {
    effPcpSharedTrailWork->unk1C = value;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182DD8);

void effPcpTrailRelease(EffPCPTrailWork *work) {
    func_0018E8F0((u32)work->obj);
    sdfReleaseChipBlock(work);
}

/* Per-frame update: places the trail object from the work position and
 * alternates its colour between two entries until the count runs out. */
void effPcpTrailUpdate(EffPCPTrailWork *work) {
    EffPCPTrailObj *obj;
    f32 pos[4];

    obj = work->obj;
    VU0_LOAD_VF($vf10, work->pos);
    obj->size = (s32)((f32)func_00195890(work->unk1C) * work->scale);
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

void effPcpSetTrailFlags(EffPCPTrailWork *work, u32 value) {
    work->flags = value;
}

void effPcpTrailSetSizeInput(EffPCPTrailWork *work, f32 val) {
    work->unk1C = val;
}

EffPCPCompactFadeWork *effPcpCompactEffectCreate(EffPCPCompactParams *params) {
    EffPCPCompactFadeWork *work;

    work = func_00328D68(0x3C);
    work->resource = effCloneResourceTemplate(&params->unk18);
    work->unk1C = 0;
    work->color = 0x80808080;
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

void effPcpCompactRespawn(EffPCPCompactFadeWork *work) {
    EffPCPCompactParams3C params;

    params.flags = work->flags;
    params.unk04 = work->unk20;
    params.unk08 = work->unk24;
    params.unk0C = work->unk28;
    params.unk10 = work->unk2C;
    params.unk14 = work->unk30;
    params.res = *(EffPCPCompactRes *)work->resource;
    effPcpCompactEffectCreate((EffPCPCompactParams *)&params);
}

void effPcpCompactEffectRelease(EffPCPCompactFadeWork *work) {
    func_0018FC88(work->resource);
    sdfReleaseChipBlock(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183120);

void func_001832C8(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetCompactColor(EffPCPCompactFadeWork *work, u32 value) {
    work->color = value;
}

void func_001832E0(EffPCPCompactFadeWork *work, f32 val) {
    work->unk34 = val;
}

EffPCPCompactFadeWork *func_001832E8(EffPCPCompactParams *params) {
    EffPCPCompactFadeWork *work;

    work = func_00328D68(0x3C);
    work->resource = effCloneBlurTemplate(&params->unk18);
    work->unk1C = 0;
    work->color = 0x80808080;
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

void effPcpCompactLongRespawn(EffPCPCompactFadeWork *work) {
    EffPCPParams44 params;

    params.flags = work->flags;
    params.unk04 = work->unk20;
    params.unk08 = work->unk24;
    params.unk0C = work->unk28;
    params.unk10 = work->unk2C;
    params.unk14 = work->unk30;
    params.res = *(EffPCPRes44 *)work->resource;
    func_001832E8((EffPCPCompactParams *)&params);
}

void func_00183448(EffPCPCompactFadeWork *work) {
    func_0018E8F0(work->resource);
    sdfReleaseChipBlock(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183478);

void func_00183620(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetLongCompactColor(EffPCPCompactFadeWork *work, u32 value) {
    work->color = value;
}

void func_00183638(EffPCPCompactFadeWork *work, f32 val) {
    work->unk34 = val;
}

void *effPcpCopyWork(src)
    EffPCPFadeTimerParams *src;
{
    EffPCPFadeTimer *dst;

    dst = func_00328D68(0x30);
    dst->params = *src;
    dst->colorFrom = 0x80808080;
    dst->frame = 0;
    dst->colorTo = src->color;
    return dst;
}

void effPcpCreateFadeTimerFromTable(void *args) {
    void *param0;

    param0 = effParamTableGetBlock(args, 0);
    effPcpCopyWork(param0);
}

void effPcpRecreateFadeTimer(void) {
    effPcpCopyWork();
}

void effPcpReleaseFadeTimerWork(EffPCPFadeTimer *work) {
    sdfReleaseChipBlock(work);
}


extern void func_0018F840(u32 *color);

/* Timeline update: blend factor ramps up over fadeIn frames, holds at 1.0, then
 * ramps down over the last fadeOut frames. */
void effPcpFadeTimerUpdate(EffPCPFadeTimer *work) {
    s32 frame = work->frame;
    s32 duration = work->params.duration;
    s32 fadeIn;
    s32 fadeOut;
    f32 t;

    if (duration < frame) {
        return;
    }
    fadeIn = work->params.fadeIn;
    work->params.unk14 = 0;
    work->params.unk18 = 0;
    work->params.width = 0x200;
    work->params.height = 0x1C0;
    fadeOut = work->params.fadeOut;
    if (frame < fadeIn && fadeIn != 0) {
        t = (f32)frame / (f32)fadeIn;
    } else if (duration - frame <= fadeOut && fadeOut != 0) {
        t = (f32)(duration - frame) / (f32)fadeOut;
    } else {
        t = 1.0f;
    }
    work->params.color = effMultiplyPackedColors(effBlendColor(work->colorFrom & 0xFFFFFF, work->colorFrom, t), work->colorTo);
    func_0018F840(&work->params.color);
    work->frame++;
}

void effPcpFadeTimerSetSourceColor(EffPCPFadeTimer *work, u32 value) {
    work->colorFrom = value;
}

void *effPcpCopyWorkLong(src)
    EffPCPFadeTimerLongParams *src;
{
    EffPCPFadeTimerLong *dst;

    dst = func_00328D68(0x40);
    dst->params = *src;
    dst->colorFrom = 0x80808080;
    dst->frame = 0;
    dst->colorTo = src->color;
    return dst;
}

void effPcpCreateLongFadeTimerFromTable(void *args) {
    void *param0;

    param0 = effParamTableGetBlock(args, 0);
    effPcpCopyWorkLong(param0);
}

void effPcpRecreateLongFadeTimer(void) {
    effPcpCopyWorkLong();
}

void effPcpReleaseLongFadeTimerWork(EffPCPFadeTimerLong *work) {
    sdfReleaseChipBlock(work);
}


extern void effDrawBlurRectangle(u32 *color);

/* Same timeline as effPcpFadeTimerUpdate on the longer work layout. */
void effPcpFadeTimerLongUpdate(EffPCPFadeTimerLong *work) {
    s32 frame = work->frame;
    s32 duration = work->params.duration;
    s32 fadeIn;
    s32 fadeOut;
    f32 t;

    if (duration < frame) {
        return;
    }
    fadeIn = work->params.fadeIn;
    work->params.unk1C = 0;
    work->params.unk20 = 0;
    work->params.unk24 = 0;
    work->params.unk28 = 0;
    work->params.width = 0x200;
    work->params.height = 0x1C0;
    fadeOut = work->params.fadeOut;
    if (frame < fadeIn && fadeIn != 0) {
        t = (f32)frame / (f32)fadeIn;
    } else if (duration - frame <= fadeOut && fadeOut != 0) {
        t = (f32)(duration - frame) / (f32)fadeOut;
    } else {
        t = 1.0f;
    }
    work->params.color = effMultiplyPackedColors(effBlendColor(work->colorFrom & 0xFFFFFF, work->colorFrom, t), work->colorTo);
    effDrawBlurRectangle(&work->params.color);
    work->frame++;
}

void effPcpLongFadeTimerSetSourceColor(EffPCPFadeTimerLong *work, u32 value) {
    work->colorFrom = value;
}

EffPCPCompactWork *effPcpCreateCompactWorkFromParams(EffPCPCompactParams *params) {
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

void effPcpCreateChargeWorkFromTable(void *args) {
    void *param0;

    param0 = effParamTableGetBlock(args, 0);
    effPcpCreateCompactWorkFromParams(param0);
}


void effPcpChargeRespawn(EffPCPCompactWork *work) {
    EffPCPParams44 params;

    params.flags = work->flags;
    params.unk04 = work->unk14;
    params.unk08 = work->unk18;
    params.unk0C = work->unk1C;
    params.unk10 = work->unk28;
    params.unk14 = work->unk2C;
    params.res = *(EffPCPRes44 *)work->resource;
    effPcpCreateCompactWorkFromParams((EffPCPCompactParams *)&params);
}

void effPcpReleaseCompactBlurWork(EffPCPCompactWork *work) {
    effBlurReleaseFirstResource(work->resource);
    sdfReleaseChipBlock(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183BE8);

void effPcpCopyCompactBlurVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpCompactSetColor(EffPCPCompactWork *work, u32 color) {
    work->color20 = color;
}

EffPCPCompactWork *effPcpCreateCompactResourceWork(EffPCPCompactParams *params) {
    EffPCPCompactWork *work;

    work = func_00328D68(0x38);
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

void effPcpChargeLongRespawn(EffPCPCompactWork *work) {
    EffPCPParams44 params;

    params.flags = work->flags;
    params.unk04 = work->unk14;
    params.unk08 = work->unk18;
    params.unk0C = work->unk1C;
    params.unk10 = work->unk28;
    params.unk14 = work->unk2C;
    params.res = *(EffPCPRes44 *)work->resource;
    effPcpCreateCompactResourceWork((EffPCPCompactParams *)&params);
}

void effPcpReleaseSecondaryBlurWork(EffPCPCompactWork *work) {
    effBlurReleaseSecondResource(work->resource);
    sdfReleaseChipBlock(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183F58);

void effPcpCopyShortThunderFadeVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetChargeResourceColor(EffPCPCompactWork *work, u32 color) {
    work->color20 = color;
}

void effPcpInitShortThunderFadeWork(EffPCPFadeWork *work) {
    u32 handle;

    handle = effCreateThunderCellSystemWork(D_003B1A38);
    work->frame = 0;
    work->handle = handle;
}

void *effPcpCreateShortThunderFadeWork(void) {
    void *work;

    work = func_00328D68(0x20);
    effPcpInitShortThunderFadeWork(work);
    return work;
}

void effPcpReleaseShortThunderFadeWork(EffPCPFadeWork *work) {
    effPCPThunderFree(work->handle);
    sdfReleaseChipBlock(work);
}

void *effAllocateInitializedThunderFadeWork(void) {
    void *work;

    work = func_00328D68(0x20);
    effPcpInitShortThunderFadeWork(work);
    return work;
}

void effPcpFadeUpdateShort(EffPCPFadeWork *work) {
    EffPCPFadeTarget *target;
    f32 phase;
    f32 value;
    u32 fadedColor;

    if (work->frame < 0x1F) {
        target = func_0016B198(work->handle);
        phase = (f32)work->frame / 30.0f;
        value = phase * 150.0f + 100.0f;
        target->sizeX = value;
        target->sizeY = value;
    } else if (work->frame - 0x2D < 0x10) {
        target = func_0016B198(work->handle);
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
    func_0016B160(work->handle, work);
    func_0016B928(work->handle);
    work->frame++;
}

void effPcpCopyScalingThunderFadeVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00184400(EffPCPFadeWork *work, f32 val) {
    work->unk10 = val;
}

void effPcpSetScalingThunderFadeColor(EffPCPFadeWork *work, u32 value) {
    work->color = value;
}

void effPcpInitScalingThunderFadeWork(EffPCPFadeWork *work) {
    u32 handle;

    handle = effCreateThunderCellSystemWork(D_003B1A88);
    work->frame = 0;
    work->handle = handle;
}

void *effPcpCreateScalingThunderFadeWork(void) {
    void *work;

    work = func_00328D68(0x20);
    effPcpInitScalingThunderFadeWork(work);
    return work;
}

void effPcpReleaseScalingThunderFadeWork(EffPCPFadeWork *work) {
    effPCPThunderFree(work->handle);
    sdfReleaseChipBlock(work);
}

void *effAllocateScalingThunderFadeWork(void) {
    void *work;

    work = func_00328D68(0x20);
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
        target = func_0016B198(work->handle);
        phase = (f32)work->frame / 45.0f;
        target->sizeY = target->sizeX = phase * 950.0f + 200.0f;
    } else if (work->frame >= 0x3C && work->frame < 0x4C) {
        target = func_0016B198(work->handle);
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
    func_0016B160(work->handle, work);
    func_0016B928(work->handle);
    work->frame++;
}

void effPcpCopyThunderScaleFadeVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_001846A8(EffPCPFadeWork *work, f32 val) {
    work->unk10 = val;
}

void effPcpSetThunderScaleFadeColor(EffPCPFadeWork *work, u32 value) {
    work->color = value;
}

/* Constructor and aimed update share the 0x78-byte angular-sweep work.
   The update uses unsigned frame bounds; the initial sweep interval is
   computed as a signed difference of the parameter words. */
typedef struct EffPCPSpanParams {
    f32 anchor[4];
    u32 holdFrames;
    u32 activeFrames;
    s32 fadeFrames;
    f32 sweepDegrees;
    f32 radius;
} EffPCPSpanParams;

typedef struct EffPCPSpanWork {
    f32 matrix[16];
    EffPCPSpanParams params;
    u32 color;
    u32 frame;
    f32 angle;
    f32 spin;
    u32 optionalHandle;
} EffPCPSpanWork;

EffPCPSpanWork *effPcpSpanCreate(void *params, void *handleParams) {
    EffPCPSpanParams *src = params;
    EffPCPSpanWork *work;
    f32 size;

    work = func_00328D68(0x78);
    work->params = *src;
    work->color = 0x80808080;
    work->frame = 0;
    size = work->params.sweepDegrees * 0.017453292f;
    work->angle = -(size * 0.5f);
    work->spin = size / (f32)((s32)work->params.activeFrames - ((work->params.fadeFrames >> 1) + (s32)work->params.holdFrames));
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->optionalHandle = 0;
    if (handleParams != NULL) {
        work->optionalHandle = func_001578C0((u32)handleParams);
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

EffPCPSpanWork *effPcpCloneWithOptionalHandle(EffPCPSpanWork *work) {
    EffPCPSpanWork *child;
    u32 handle;

    child = effPcpSpanCreate(&work->params, NULL);
    handle = work->optionalHandle;
    if (handle != 0) {
        child->optionalHandle = effCloneSourceWithTypeHandler(handle);
    }
    return child;
}

void effPcpReleaseOptionalHandle(EffPCPSpanWork *work) {
    u32 handle;

    handle = work->optionalHandle;
    if (handle != 0) {
        effDestroyNode(handle);
    }
    sdfReleaseChipBlock(work);
}


typedef struct EffPCPAimBattle {
    u8 pad00[0x110];
    u32 flags;             /* 0x110 */
} EffPCPAimBattle;

extern void *effBTLFieldColorGetVariantSelector(void);
extern s32 func_001695C8();
extern f32 func_00208000(u32 mask, f32 *maxTop, f32 *minTop);
extern u32 effBTLFieldColorGetOriginalSelector(void);
extern void btlUnitGetMuzzlePosVU(void *unit);
extern f32 sdfAtan2(f32 y, f32 x);
extern void effCopyVectorToNodeInstance(s32 node, f32 *pos);
extern void effApplyNodeTransformMatrix(s32 node, void *mtx);
extern void effSetNodeParameterValue(s32 node, u32 color);
extern void effUpdateNode(s32 node);

/* On the first frame, center the sweep on the battle group's direction.
   Place the optional node around the anchor, then fade its final frames. */
void effPcpUpdateOrbitingAimNode(EffPCPSpanWork *work) {
    EffPCPSpanParams *params = &work->params;
    s32 node = work->optionalHandle;
    s32 fadeFrames = params->fadeFrames;
    u32 end = fadeFrames + params->activeFrames;
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
    if (func_001695C8() && work->frame == 0) {
        func_00208000(((EffPCPAimBattle *)effBTLFieldColorGetVariantSelector())->flags & 0x600, NULL, NULL);
        VU0_STORE_VF(vf10, center);
        btlUnitGetMuzzlePosVU((void *)effBTLFieldColorGetOriginalSelector());
        VU0_STORE_VF(vf10, muzzle);
        look[0] = center[0] - muzzle[0];
        look[2] = center[2] - muzzle[2];
        work->angle = sdfAtan2(look[0], look[2]) - params->sweepDegrees * EFF_DEG2RAD * 0.5f;
    }
    if (node != 0) {
        VU0_LOAD_MATRIX(work);
        func_00336818(work->angle);
        sdfComposeVuMatrixFromRegisters();
        aim[0] = aim[1] = 0.0f;
        aim[2] = 1.0f;
        VU0_LOAD_VF(vf10, aim);
        VU0_ROTATE_VEC(vf10, vf10);
        VEC3_SPLAT(muzzle, params->radius);
        VU0_LOAD_VF(vf11, muzzle);
        VU0_MUL(vf10, vf10, vf11);
        VU0_LOAD_VF(vf11, params->anchor);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, muzzle);
        effCopyVectorToNodeInstance(node, muzzle);
        if (work->frame > params->holdFrames) {
            work->angle += work->spin;
        }
        func_00336538(work->angle);
        VU0_STORE_MATRIX(mtx);
        effApplyNodeTransformMatrix(node, mtx);
        if (fadeFrames >= remaining && fadeFrames != 0) {
            t = (f32)remaining / (f32)fadeFrames;
        } else {
            t = 1.0f;
        }
        effSetNodeParameterValue(node, effBlendColor(work->color & 0xFFFFFF, work->color, t));
        effUpdateNode(node);
    }
    work->frame++;
}

void func_00184B10(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0x40, src);
}

void func_00184B28(EffPCPSpanWork *work, u32 value) {
    work->color = value;
}

void effPcpCopyHalfTurnMatrix(void *dst, void *src) {
    func_00336538(3.1415927f);
        VU0_LOAD_MATRIX_B(src);
;
    sdfComposeVuMatrixFromRegisters();
        VU0_STORE_MATRIX(dst);
;
}

void *effPcpTripleHandleCreate(void *block0, u32 *blocks) {
    EffPCPTripleWork *work;
    u32 *handle;
    u32 i;

    work = func_00328D68(0xAC);
    work->head = *(EffPCPBlock80 *)block0;
    work->unk50 = 0;
    work->color = 0x80808080;
    handle = work->handleA;
    for (i = 0; i < 7; i++) {
        handle[0] = func_001578C0(blocks[i]);
        handle[7] = effCloneSourceWithTypeHandler(handle[0]);
        handle[14] = effCloneSourceWithTypeHandler(handle[0]);
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

    work = func_00328D68(0xAC);
    work->head = src->head;
    work->unk50 = 0;
    work->color = 0x80808080;
    from = src->handleC;
    to = work->handleC;
    for (i = 0; i < 7; i++) {
        to[-14] = effCloneSourceWithTypeHandler(from[-14]);
        to[-7] = effCloneSourceWithTypeHandler(from[-7]);
        to[0] = effCloneSourceWithTypeHandler(from[0]);
        from++;
        to++;
    }
    return work;
}

void effPcpTripleHandleRelease(EffPCPTripleWork *work) {
    u32 *p = work->handleA;
    u32 i;

    for (i = 0; i < 7; i++) {
        effDestroyNode(p[14]);
        effDestroyNode(p[7]);
        effDestroyNode(p[0]);
        p++;
    }
    sdfReleaseChipBlock(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184F50);

void effPcpCopySpanVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00185110(EffPCPTripleWork *work, u32 value) {
    work->color = value;
}

extern u32 effParamWorkCreate(s32 kind, void *params);

typedef struct EffPCPBlockModelInfo {
    u8 pad00[0x2E];
    u16 unk2E;
} EffPCPBlockModelInfo;

typedef struct EffPCPBlockModel {
    u8 pad00[0x1C];
    EffPCPBlockModelInfo *info;
} EffPCPBlockModel;

/* Build a block-set work: copy the parameter block, then create one parameter handle per input block. */
EffPCPBlockSetWork *func_00185118(void *first, void **blocks) {
    EffPCPBlockSetWork *work;
    EffPCPBlockModel *model;
    u32 i;
    u32 j;
    u32 n;

    work = func_00328D68(0x10C);
    memset(work, 0, 0x10C);
    work->params = *(EffPCPBlockSetParams *)first;
    work->unkB0 = 0;
    work->color = 0x80808080;
    work->mode = 0;
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->headHandle = effParamWorkCreate(3, blocks[0]);
    work->handleA[0] = effParamWorkCreate(0, blocks[1]);
    work->handleA[1] = effParamWorkCreate(0, blocks[2]);
    work->handleA[2] = effParamWorkCreate(0, blocks[3]);
    work->handleA[3] = effParamWorkCreate(0, blocks[4]);
    work->handleA[4] = effParamWorkCreate(3, blocks[5]);
    model = effParamWorkGetData(work->headHandle);
    work->count = model->info->unk2E;
    for (i = 0; i < 3; i++) {
        if (work->params.groupSize[i] > 0) {
            n = work->count * work->params.groupSize[i];
            work->alloc[i] = (u32)func_003292A8(n * 4);
            work->list[i] = sdfResourceRetainAddress((void *)work->alloc[i]);
            work->list[i][0] = effParamWorkCreate(0, blocks[6 + i]);
            for (j = 1; j < n; j++) {
                work->list[i][j] = 0;
            }
        } else {
            work->alloc[i] = 0;
        }
    }
    work->handleB[0] = effParamWorkCreate(0, blocks[9]);
    work->handleB[1] = effParamWorkCreate(0, blocks[10]);
    work->handleB[2] = effParamWorkCreate(0, blocks[11]);
    work->handleB[3] = effParamWorkCreate(0, blocks[12]);
    work->handleB[4] = effParamWorkCreate(6, blocks[13]);
    work->tailHandle = effParamWorkCreate(0, blocks[14]);
    return work;
}

EffPCPBlockSetWork *effPcpBuildBlockSet(args)
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


extern void *memset(void *s, int c, u32 n);

extern void func_00185500(void *dst, void *src);

/* Clone of a block-set work that shares the source's blocks (no back pointer). */
EffPCPBlockSetWork *effPcpBlockSetCloneShared(EffPCPBlockSetWork *src) {
    EffPCPBlockSetWork *work;

    work = func_00328D68(0x10C);
    memset(work, 0, 0x10C);
    work->params = src->params;
    work->unkB0 = 0;
    work->color = 0x80808080;
    work->mode = 0;
    EE_MMI_UNIT_MATRIX(work->matrix);
    func_00185500(work, src);
    work->source = NULL;
    return work;
}

void effPcpBlockSetWorkRelease(EffPCPBlockSetWork *work) {
    u32 i;
    u32 j;
    u32 n;

    if (work->source == NULL) {
        effDispatchParameterDataAndFreeWork(work->headHandle);
        for (j = 0; j < 5; j++) {
            effDispatchParameterDataAndFreeWork(work->handleA[j]);
        }
        for (i = 0; i < 3; i++) {
            if (work->alloc[i] != 0) {
                n = work->count * work->params.groupSize[i];
                for (j = 0; j < n; j++) {
                    if (work->list[i][j] != 0) {
                        effDispatchParameterDataAndFreeWork(work->list[i][j]);
                    }
                }
                func_003297C8(work->alloc[i]);
            }
        }
        for (j = 0; j < 5; j++) {
            effDispatchParameterDataAndFreeWork(work->handleB[j]);
        }
        effDispatchParameterDataAndFreeWork(work->tailHandle);
    }
    sdfReleaseChipBlock(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00185950);

void effPcpCopyVector60(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0x60, src);
}

void func_00186118(EffPCPBlockSetWork *work, u32 value) {
    work->color = value;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effPcpCopyBlockMatrix(void *dst, void *src) {
    VU0_COPY_MATRIX(dst, src);
}

void func_00186148(void) {
    EffPCPBlockSetWork *work;

    work = effPcpBuildBlockSet();
    work->mode = 1;
}

EffPCPBlockSetWork *effPcpCloneBlockWithUnitMatrix(EffPCPBlockSetWork *src) {
    EffPCPBlockSetWork *work;

    work = func_00328D68(0x10C);
    memset(work, 0, 0x10C);
    work->params = src->params;
    work->color = 0x80808080;
    work->mode = 1;
    work->unkB0 = 0;
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->source = src;
    return work;
}

void func_001862C0(void) {
    EffPCPBlockSetWork *work;

    work = effPcpBuildBlockSet();
    work->mode = 2;
}

EffPCPBlockSetWork *effCloneBlockWorkFromSource(EffPCPBlockSetWork *src) {
    EffPCPBlockSetWork *work;

    work = func_00328D68(0x10C);
    memset(work, 0, 0x10C);
    work->params = src->params;
    work->color = 0x80808080;
    work->mode = 2;
    work->unkB0 = 0;
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->source = src;
    return work;
}

EffPCPRotateWork *effPcpRotateCreate(EffPCPRotateParams *src, u32 *blocks) {
    EffPCPRotateWork *work;
    u8 *sub;
    u32 i;

    work = func_00328D68(0x10C);
    work->params = *src;
    sub = (u8 *)src->group;
    work->frame = 0;
    for (i = 0; i < 3; i++) {
        blocks[3] = blocks[i];
        work->ids[i] = (s32)func_00185118(sub, (void **)&blocks[3]);
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
    EffPCPBlockSetWork *sub;
    u32 i;

    work = func_00328D68(0x10C);
    work->params = src->params;
    work->frame = 0;
    for (i = 0; i < 3; i++) {
        work->ids[i] = (s32)func_00328D68(0x10C);
        memset((void *)work->ids[i], 0, 0x10C);
        ((EffPCPBlockSetWork *)work->ids[i])->params = ((EffPCPBlockSetWork *)src->ids[i])->params;
        sub = (EffPCPBlockSetWork *)work->ids[i];
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

/* Dispatch children whose start threshold has been reached. Reload the shared
   frame after each callback before checking the next child. */
void effPcpRotateFireIds(EffPCPRotateWork *work) {
    s32 frame;
    u32 i;

    i = 0;
    frame = work->frame;
    do {
        if (work->params.startFrame[i] <= frame) {
            func_00185950(work->ids[i]);
            frame = work->frame;
        }
        i = i + 1;
    } while (i < 3);
    work->frame = frame + 1;
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
        func_00186118((EffPCPBlockSetWork *)id[i], val);
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

EffPCPSpinWork *effSpinSingleCreateFromTable(void *src) {
    EffPCPSpinWork *work = func_00328D68(0x64);

    work->frame = 0;
    work->scale = 1.0f;
    work->color = 0x80808080;
    work->handle0 = (void *)effParamCreateFromTable(src, 0);
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->angle = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f * 0.87266457f;
    if (effMiscRand(D_003AA868) & 1) {
        work->angle += 3.14159265f;
    }
    return work;
}

/* Single-handle variant: allocation stops before handle1 (0x64 bytes). */
EffPCPSpinWork *effSpinSingleClone(EffPCPSpinWork *src) {
    EffPCPSpinWork *work = func_00328D68(0x64);

    work->frame = 0;
    work->scale = 1.0f;
    work->color = 0x80808080;
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->angle = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f * 0.87266457f;
    if (effMiscRand(D_003AA868) & 1) {
        work->angle += 3.14159265f;
    }
    work->handle0 = (void *)effParamWorkDuplicate((u32)src->handle0);
    return work;
}

void effSpinSingleRelease(EffPCPSpinWork *work) {
    effDispatchParameterDataAndFreeWork((u32)work->handle0);
    sdfReleaseChipBlock(work);
}

void effSpinEffectUpdate(EffPCPSpinWork *work) {
    u32 handle = (u32)work->handle0;
    u128 mtx[4];

    effParamWorkCallback0(handle, work);
    effParamWorkCallback1(handle, work->scale);
    effParamWorkCallback3(handle, work->color);
    func_00336538(work->angle);
        VU0_LOAD_MATRIX_B(work->matrix);
;
    sdfComposeVuMatrixFromRegisters();
        VU0_STORE_MATRIX(mtx);
;
    effParamWorkCallback2(handle, mtx);
    effParamWorkInvokeCallback(handle);
    work->frame++;
}

void effPcpCopySpinSingleVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effSpinSingleSetColor(EffPCPSpinWork *work, u32 color) {
    work->color = color;
}

void effSpinSingleSetScale(EffPCPSpinWork *work, f32 val) {
    work->scale = val;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effCopyMatrix(EffPCPSpinWork *work, void *src) {
    VU0_LOAD_MATRIX(src);
    VU0_STORE_MATRIX(work->matrix);
}

EffPCPSpinWork *effSpinEffectCreateFromTable(void *table) {
    EffPCPSpinWork *work = func_00328D68(0x68);

    work->frame = 0;
    work->scale = 1.0f;
    work->color = 0x80808080;
    work->handle0 = (void *)effParamCreateFromTable(table, 0);
    work->handle1 = (void *)effParamCreateFromTable(table, 1);
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->angle = 2.0f * (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 0.87266457f;
    return work;
}

EffPCPSpinWork *effSpinEffectClone(EffPCPSpinWork *src) {
    EffPCPSpinWork *work = func_00328D68(0x68);

    work->scale = 1.0f;
    work->frame = 0;
    work->color = 0x80808080;
    work->handle0 = (void *)effParamWorkDuplicate((u32)src->handle0);
    work->handle1 = (void *)effParamWorkDuplicate((u32)src->handle1);
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->angle = 2.0f * (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 0.87266457f;
    return work;
}

void effSpinPairRelease(EffPCPSpinWork *work) {
    effDispatchParameterDataAndFreeWork((u32)work->handle1);
    effDispatchParameterDataAndFreeWork((u32)work->handle0);
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
    func_00336538(work->angle);
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

void func_00187118(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effSpinPairSetColor(EffPCPSpinWork *work, u32 color) {
    work->color = color;
}

void effSpinPairSetScale(EffPCPSpinWork *work, f32 val) {
    work->scale = val;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effSpinPairSetMatrix(EffPCPSpinWork *work, void *src) {
    VU0_LOAD_MATRIX(src);
    VU0_STORE_MATRIX(work->matrix);
}

EffPCPSpinWork *effPcpCreateSpinningPair(void *table) {
    EffPCPSpinWork *work = func_00328D68(0x68);

    work->frame = 0;
    work->scale = 1.0f;
    work->color = 0x80808080;
    work->handle0 = (void *)effParamCreateFromTable(table, 0);
    work->handle1 = (void *)effParamCreateFromTable(table, 1);
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->angle = 2.0f * (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 0.87266457f;
    return work;
}

EffPCPSpinWork *effPcpCloneSpinningPair(EffPCPSpinWork *src) {
    EffPCPSpinWork *work = func_00328D68(0x68);

    work->scale = 1.0f;
    work->frame = 0;
    work->color = 0x80808080;
    work->handle0 = (void *)effParamWorkDuplicate((u32)src->handle0);
    work->handle1 = (void *)effParamWorkDuplicate((u32)src->handle1);
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->angle = 2.0f * (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 0.87266457f;
    return work;
}

void effPcpReleaseSpinningPair(EffPCPSpinWork *work) {
    effDispatchParameterDataAndFreeWork((u32)work->handle1);
    effDispatchParameterDataAndFreeWork((u32)work->handle0);
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
    func_00336538(work->angle);
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

void func_00187418(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetSpinColor(EffPCPSpinWork *work, u32 color) {
    work->color = color;
}

void effPcpSetSpinScale(EffPCPSpinWork *work, f32 val) {
    work->scale = val;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effPcpSetSpinMatrix(EffPCPSpinWork *work, void *src) {
    VU0_LOAD_MATRIX(src);
    VU0_STORE_MATRIX(work->matrix);
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

void effPcpClearGlobalEffectFlag(void) {
    if (D_0043643C != 0) {
        D_0043643C = 0;
    }
}

EffPCPBurstWork *effAllocateThunderFragmentWork(u32 unused) {
    EffPCPBurstWork *work;

    work = func_00328D68(0x24);
    work->startDistance = 0;
    work->length = 375.0f;
    work->handle = effThunderFragCreate(D_003B1AF0);
    return work;
}

void func_001874F0(void) {
    effAllocateThunderFragmentWork(0);
}

EffPCPBurstWork *effAllocateThunderFragmentBurstWork(u32 unused) {
    EffPCPBurstWork *work;

    work = func_00328D68(0x24);
    work->startDistance = 325.0f;
    work->length = 225.0f;
    work->handle = effThunderFragCreate(D_003B1B50);
    return work;
}

void func_00187558(void) {
    effAllocateThunderFragmentBurstWork(0);
}

EffPCPBurstWork *effPcpCreateCyanBlueFragmentPair(u32 unused) {
    EffPCPBurstWork *work;

    work = func_00328D68(0x24);
    work->startDistance = 0;
    work->length = 375.0f;
    work->handle = effThunderFragCreate(D_003B1BB0);
    return work;
}

void effPcpRecreateCyanBlueFragmentPair(void) {
    effPcpCreateCyanBlueFragmentPair(0);
}

EffPCPBurstWork *effPcpCreateCyanBlueFragmentSingle(u32 unused) {
    EffPCPBurstWork *work;

    work = func_00328D68(0x24);
    work->startDistance = 325.0f;
    work->length = 225.0f;
    work->handle = effThunderFragCreate(D_003B1C10);
    return work;
}

void effPcpRecreateCyanBlueFragmentSingle(void) {
    effPcpCreateCyanBlueFragmentSingle(0);
}

EffPCPBurstWork *effPcpCreateWhiteYellowFragmentPair(u32 unused) {
    EffPCPBurstWork *work;

    work = func_00328D68(0x24);
    work->startDistance = 0;
    work->length = 375.0f;
    work->handle = effThunderFragCreate(D_003B1C70);
    return work;
}

void effPcpRecreateWhiteYellowFragmentPair(void) {
    effPcpCreateWhiteYellowFragmentPair(0);
}

EffPCPBurstWork *effPcpCreateWhiteYellowFragmentSingle(u32 unused) {
    EffPCPBurstWork *work;

    work = func_00328D68(0x24);
    work->startDistance = 325.0f;
    work->length = 225.0f;
    work->handle = effThunderFragCreate(D_003B1CD0);
    return work;
}

void effPcpRecreateWhiteYellowFragmentSingle(void) {
    effPcpCreateWhiteYellowFragmentSingle(0);
}

EffPCPBurstWork *effPcpCreateYellowOrangeFragmentPair(u32 unused) {
    EffPCPBurstWork *work;

    work = func_00328D68(0x24);
    work->startDistance = 0;
    work->length = 375.0f;
    work->handle = effThunderFragCreate(D_003B1D30);
    return work;
}

void effPcpRecreateYellowOrangeFragmentPair(void) {
    effPcpCreateYellowOrangeFragmentPair(0);
}

EffPCPBurstWork *effPcpCreateYellowOrangeFragmentSingle(u32 unused) {
    EffPCPBurstWork *work;

    work = func_00328D68(0x24);
    work->startDistance = 325.0f;
    work->length = 225.0f;
    work->handle = effThunderFragCreate(D_003B1D90);
    return work;
}

void effPcpRecreateYellowOrangeFragmentSingle(void) {
    effPcpCreateYellowOrangeFragmentSingle(0);
}

EffPCPBurstWork *effPcpCreateBrownRedFragmentPair(u32 unused) {
    EffPCPBurstWork *work;

    work = func_00328D68(0x24);
    work->startDistance = 0;
    work->length = 375.0f;
    work->handle = effThunderFragCreate(D_003B1DF0);
    return work;
}

void effPcpRecreateBrownRedFragmentPair(void) {
    effPcpCreateBrownRedFragmentPair(0);
}

EffPCPBurstWork *effPcpCreateBrownRedFragmentSingle(u32 unused) {
    EffPCPBurstWork *work;

    work = func_00328D68(0x24);
    work->startDistance = 325.0f;
    work->length = 225.0f;
    work->handle = effThunderFragCreate(D_003B1E50);
    return work;
}

void effPcpRecreateBrownRedFragmentSingle(void) {
    effPcpCreateBrownRedFragmentSingle(0);
}

EffPCPBurstWork *effPcpCreateOrangeVioletFragmentPair(u32 unused) {
    EffPCPBurstWork *work;

    work = func_00328D68(0x24);
    work->startDistance = 0;
    work->length = 375.0f;
    work->handle = effThunderFragCreate(D_003B1EB0);
    return work;
}

void effPcpRecreateOrangeVioletFragmentPair(void) {
    effPcpCreateOrangeVioletFragmentPair(0);
}

EffPCPBurstWork *effPcpCreateOrangeVioletFragmentSingle(u32 unused) {
    EffPCPBurstWork *work;

    work = func_00328D68(0x24);
    work->startDistance = 325.0f;
    work->length = 225.0f;
    work->handle = effThunderFragCreate(D_003B1F10);
    return work;
}

void effPcpRecreateOrangeVioletFragmentSingle(void) {
    effPcpCreateOrangeVioletFragmentSingle(0);
}

void effPcpThunderBurstRelease(EffPCPBurstWork *work) {
    effPCPThunderFree3(work->handle);
    sdfReleaseChipBlock(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187988);

void effPcpCopyThunderBurstVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetThunderBurstColor(EffPCPBurstWork *work, u32 value) {
    work->unk10 = value;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187AA8);

void effPcpScaledEffectCreateFromTable(void *data) {
    void *work;

    work = effParamTableGetBlock(data, 0);
    func_00187AA8(work);
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

void effPcpCreateScaledEffectFromResource(EffPCPGrowWork *work) {
    EffPCPParams76 params;
    EffPCPRes76 *res;

    res = func_0016B198(work->handle);
    params.unk00 = work->duration;
    params.unk04 = work->fadeIn;
    params.unk08 = work->fadeOut;
    params.unk0C = work->base;
    params.unk10 = work->target;
    params.res = *res;
    func_00187AA8(&params);
}

void effPcpScaledEffectRelease(EffPCPGrowWork *work) {
    effPCPThunderFree(work->handle);
    sdfReleaseChipBlock(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187CD8);

void effPcpCopyScaledThunderVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetScaledThunderColor(EffPCPGrowWork *work, u32 value) {
    work->color = value;
}

void effPcpSetScaledThunderScale(EffPCPGrowWork *work, f32 val) {
    work->scale = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187E50);

void effPcpReleaseNestedWork(EffPCPBeamNode *work) {
    sdfQueueAssetRelease(work->assetHandle);
    func_003297C8(work->allocationHandle);
    sdfReleaseChipBlock(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00187FC8);

void func_00188198(f32 radius, EffPCPBeamWork *work) {
    f32 direction[4];
    f32 inner[4];
    f32 first[4];
    f32 middle[4];
    f32 outer[4];
    f32 angle = 0.0f;
    f32 step;
    f32 *out;
    u32 count;
    u32 i;

    VEC3_SPLAT(inner, radius);
    VEC3_SPLAT(first, radius + work->params.firstWidth);
    VEC3_SPLAT(middle, first[0] + work->params.middleWidth);
    VEC3_SPLAT(outer, middle[0] + work->params.lastWidth);
    count = (s32)work->node->vertexCount >> 2;
    out = work->node->points;
    step = 3.14159265f * 2.0f / (f32)work->params.segments;
    direction[1] = 0.0f;
    for (i = 0; i < count; i++, out += 16) {
        direction[0] = sdfEvaluateCosineViaSinePhaseShift(angle);
        direction[2] = sdfSinPoly(angle);
        VU0_LOAD_VF(vf11, direction);
        VU0_LOAD_VF(vf10, inner);
        VU0_MUL(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, out);
        VU0_LOAD_VF(vf10, first);
        VU0_MUL(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, out + 4);
        VU0_LOAD_VF(vf10, middle);
        VU0_MUL(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, out + 8);
        VU0_LOAD_VF(vf10, outer);
        VU0_MUL(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, out + 12);
        angle += step;
    }
}

void effResetChild(EffPCPBeamWork *work) {
    func_00188198(work->params.unk28, work);
    work->unk50 = 0;
}

u8 *effBeamEffectClone(src)
    EffPCPBeamParams *src;
{
    EffPCPBeamWork *work = func_00328D68(0x60);
    u32 segments;
    EffPCPBeamNode *node;
    u32 *entry;
    s32 groups;
    u32 i;

    work->params = *src;
    work->color = 0x80808080;
    work->unk50 = 0;
    segments = src->segments;
    if (segments < 3) {
        src->segments = 3;
        segments = 3;
    }
    node = (EffPCPBeamNode *)func_00187E50(segments);
    i = 0;
    work->node = node;
    work->vertexCount = node->vertexCount;
    groups = (s32)node->vertexCount >> 2;
    entry = node->colors;
    for (i = 0; i < groups; i++) {
        u32 second;

        entry[0] = src->firstColor;
        second = src->middleColor;
        entry[1] = entry[2] = second;
        entry[3] = src->lastColor;
        entry += 4;
    }
    effResetChild(work);
    work->node->drawKind = src->drawKind;
    return (u8 *)work;
}

void effPcpBeamCreateFromTable(void *args) {
    void *param0;

    param0 = effParamTableGetBlock(args, 0);
    effBeamEffectClone(param0);
}

void effPcpRecreateBeam(void) {
    effBeamEffectClone();
}

void effPcpReleaseBeamClone(EffPCPBeamWork *work) {
    effPcpReleaseNestedWork(work->node);
    sdfReleaseChipBlock(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00188550);

void effPcpCopyBeamVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpNestedWorkSetFloat(EffPCPBeamWork *work, f32 value) {
    work->node->scale = value;
}

void effPcpSetBeamColor(EffPCPBeamWork *work, u32 value) {
    work->color = value;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effPcpCopyNestedMatrix(EffPCPBeamWork *work, void *src) {
    VU0_LOAD_MATRIX(src);
    VU0_STORE_MATRIX(work->node->localMatrix);
}

void effRotateNested(EffPCPBeamWork *work, void *src) {
    VU0_LOAD_MATRIX(src);
    func_00336798(1.5707963f);
    sdfComposeVuMatrixFromRegisters();
    VU0_STORE_MATRIX(work->node);
}


extern u8 sdfViewUpVector[];
extern void sdfVuBuildLookAtBasis(void *, void *, void *);
extern void sdfInvertRigidVuTransform(void);

/* Fill the node's point buffer with four concentric rings (radius, +stepA, +stepB, +stepC) of unit directions, rotated by the VU matrix. */
void effPcpBuildConcentricRingPoints(void *obj, f32 radius) {
    EffPCPBeamLargeWork *work = obj;
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
    r1 = radius + work->head.radiusStepA;
    VEC3_SPLAT(rowB, r1);
    r2 = r1 + work->head.radiusStepB;
    VEC3_SPLAT(rowC, r2);
    r3 = r2 + work->head.radiusStepC;
    VEC3_SPLAT(rowD, r3);
    n = (s32)work->node->vertexCount >> 2;
    points = work->node->points;
    step = 6.2831851f / (f32)work->head.segments;
    if (work->head.mode == 0) {
        sdfVuBuildLookAtBasis(sdfViewEyeVector, sdfViewTargetVector, sdfViewUpVector);
        sdfInvertRigidVuTransform();
    } else {
        func_003365B8(work->rotationA);
        func_00336818(work->rotationB);
        sdfMultiplyVuMatrixInPlace();
        work->rotationB += work->rotationStep;
        work->rotationStep *= work->head.rotationDecay;
    }
    for (i = 0; i < n; i++) {
        if (work->head.mode == 0) {
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

void effPrepareAngles(EffPCPBeamLargeWork *work) {
    work->rotationA = work->head.degreesA * 0.017453291f;
    work->rotationB = work->head.degreesB * 0.017453291f;
    work->rotationStep = work->head.stepDegrees * 0.017453291f;
    work->radius = work->head.initialRadius;
    work->radiusStep = work->head.initialRadiusStep;
    effPcpBuildConcentricRingPoints(work, work->head.initialRadius);
    work->frame = 0;
}

u8 *effBeamEffectCloneLarge(src)
    EffPCPBeamLargeHead *src;
{
    EffPCPBeamLargeWork *work = func_00328D68(0x80);
    u32 segments;
    EffPCPBeamNode *node;
    u32 *entry;
    s32 groups;
    u32 i;

    work->head = *src;
    work->color = 0x80808080;
    work->frame = 0;
    segments = src->segments;
    if (segments < 3) {
        src->segments = 3;
        segments = 3;
    }
    node = (EffPCPBeamNode *)func_00187E50(segments);
    i = 0;
    work->node = node;
    groups = (s32)node->vertexCount >> 2;
    entry = node->colors;
    for (i = 0; i < groups; i++) {
        u32 second;

        entry[0] = src->firstColor;
        second = src->middleColor;
        entry[1] = entry[2] = second;
        entry[3] = src->lastColor;
        entry += 4;
    }
    effPrepareAngles(work);
    work->node->drawKind = src->drawKind;
    return (u8 *)work;
}

void effPcpBeamLargeCreateFromTable(void *args) {
    void *param0;

    param0 = effParamTableGetBlock(args, 0);
    effBeamEffectCloneLarge(param0);
}

void effPcpRecreateLinkedBeam(void) {
    effBeamEffectCloneLarge();
}

void effPcpReleaseLinkedBeamClone(EffPCPBeamLargeWork *work) {
    effPcpReleaseNestedWork(work->node);
    sdfReleaseChipBlock(work);
}


extern void func_00187FC8(EffPCPBeamNode *beam);

/* Advance the ring radius, fade the color and mirror the position/color
   into the clone's beam node. */
void effPcpUpdateBeamTimeline(EffPCPBeamLargeWork *work) {
    s32 frame = work->frame;
    s32 duration = work->head.duration;
    s32 fadeIn = work->head.fadeIn;
    s32 fadeOut = work->head.fadeOut;
    f32 t;
    u32 color;
    EffPCPBeamNode *beam;

    if (duration < frame) {
        return;
    }
    work->radius += work->radiusStep;
    effPcpBuildConcentricRingPoints(work, work->radius);
    work->radiusStep *= work->head.radiusDecay;
    t = 1.0f;
    if (frame < fadeIn && fadeIn != 0) {
        t = (f32)frame / (f32)fadeIn;
    } else if (duration - frame <= fadeOut && fadeOut != 0) {
        t = (f32)(duration - frame) / (f32)fadeOut;
    }
    color = effBlendColor(work->color & 0xFFFFFF, work->color, t);
    beam = work->node;
    beam->color = color;
    PCP_COPY_VECTOR(&beam->position, work);
    func_00187FC8(beam);
    work->frame++;
}

void effPcpCopyLinkedBeamVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpLinkedWorkSetFloat(EffPCPBeamLargeWork *work, f32 value) {
    work->node->scale = value;
}

void effPcpSetLinkedBeamColor(EffPCPBeamLargeWork *work, u32 value) {
    work->color = value;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effPcpCopyLinkedBeamMatrix(EffPCPBeamLargeWork *work, void *src) {
    VU0_LOAD_MATRIX(src);
    VU0_STORE_MATRIX(work->node->localMatrix);
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
    void *resource = func_003292A8(count * 0x18 + 0x180);
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
        copy->duplicateHandle = func_003292A8(size);
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

EffPCPGroupSet *effBlockSetCloneWithDuplicates(EffPCPGroupSet *work) {
    EffPCPGroupSet *copy = effPcpGroupSetCreate(&work->head, 0);
    u32 group;
    u32 offset;
    u32 count;
    u32 size;
    u32 stride;
    u8 *flags;
    u32 i;

    if (work->duplicates != 0) {
        count = work->head.count;
        size = count * 16;
        stride = count * 4;
        group = 0;
        flags = work->head.activeGroups;
        offset = 0;
        copy->duplicateHandle = func_003292A8(size);
        copy->duplicates = sdfResourceRetainAddress(copy->duplicateHandle);
        memset(copy->duplicates, 0, size);
        for (; group < 4; group++) {
            u32 *slot = (u32 *)((u8 *)copy->duplicates + offset);

            if (*flags != 0) {
                u32 first = *(u32 *)(offset + (u32)work->duplicates);
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

void effBlockSetRelease(EffPCPGroupSet *work) {
    u32 i = 0;
    u32 count = work->head.count;
    EffPCPGroupEntry *entry = work->entries;
    u32 *list;
    u32 *duplicate;

    if (count != 0) {
        do {
            u32 handle = entry->handle;
            entry++;
            i++;
            effPCPThunderFree3(handle);
        } while (i < count);
    }
    list = work->duplicates;
    count = count * 4;
    if (list != NULL) {
        duplicate = list;
        i = 0;
        if (count != 0) {
            do {
                u32 handle = *duplicate++;
                if (handle != 0) {
                    effDispatchParameterDataAndFreeWork(handle);
                }
                i++;
            } while (i < count);
        }
        func_003297C8(work->duplicateHandle);
    }
    func_003297C8(work->workHandle);
}

/* Randomises entry `index` of a group set: a start delay, a jittered
 * position step per frame and a spread angle step. */
void effPcpGroupSetInitEntry(EffPCPGroupSet *work, s32 index) {
    EffPCPGroupEntry *entry = work->entries + index;
    s32 spread = work->head.delaySpread;
    f32 scale = work->scale;
    f32 ratio;

    if (spread > 0) {
        entry->frame = -(effMiscRand(D_003AA868) % spread);
    }
    ratio = work->head.startJitter;
    entry->position = work->head.startPosition * (effMiscRandUnitFloat(D_003AA868) * ratio + (1.0f - ratio)) * scale;
    ratio = work->head.endJitter;
    entry->positionStep = (work->head.endPosition * (effMiscRandUnitFloat(D_003AA868) * ratio + (1.0f - ratio)) * scale - entry->position) / (f32)work->head.framesPerEntry;
    entry->angle = 3.14159265f * 2.0f / (f32)work->head.count * (f32)index;
    ratio = work->head.unk80;
    entry->unk14 = work->head.unk78 * (effMiscRandUnitFloat(D_003AA868) * ratio + (1.0f - ratio));
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00189500);

void effPcpCopyGroupSetVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetGroupScale(EffPCPGroupSet *work, f32 val) {
    work->scale = val;
}

void effPcpSetGroupColor(EffPCPGroupSet *work, u32 value) {
    work->color = value;
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
        work->angle[i] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f * 0.87266457f + 3.14159265f;
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
        work->angle[i] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f * 0.87266457f + 3.14159265f;
    }
    work->handle[0] = effParamWorkDuplicate(src->handle[0]);
    return work;
}

void effDestroyIndexedResources(s32 *obj) {
    u32 i;

    for (i = 0; i < (u32)obj[6]; i++) {
        if (obj[0x1C + i] != 0) {
            effDispatchParameterDataAndFreeWork(obj[0x1C + i]);
        }
    }
    sdfReleaseChipBlock(obj);
}

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

extern u8 D_00414610[];
extern void func_00340DC8(f32 x, f32 y, f32 z);
extern void effMiscQuatMultiplyVU(void);
extern void mdlUpdateContextRotationBasisFromQuaternion(void *work);
extern void mdlStoreTertiaryVectorVU(void *work);
extern void sdfModelUpdateCurrentFrameTransforms(void *model);
extern void func_003320E8(void *table, void *model);
extern void func_00334510(void *obj);

/* Per-frame update: for each of `count` slots, spawn its model on its start frame, orient/scale it, refresh its children and capture the node vectors. */
void effPcpUpdateStaggeredPulseModels(EffPCPPulseWork *work) {
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
        func_00340DC8(0.0f, work->rotY[i], 0.0f);
        if (func_001695C8() && (((EffPCPPulseBattle *)effBTLFieldColorGetVariantSelector())->flags & 0x400)) {
            VU0_LOAD_VF(vf11, D_00414610);
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
                func_00334510(child);
            }
        }
        if (!(data->flags & 1)) {
            model = data->model;
            root = model->head->roots[0];
            VEC3_SPLAT(vec, 1.0f / work->scale);
            VU0_LOAD_VF(vf10, vec);
            effPcpCaptureNodeVectors(root);
            sdfModelUpdateCurrentFrameTransforms(model);
            func_003320E8(D_00380828, model);
        }
    }
    work->frame++;
}

void effPcpCopyCaptureNodeVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSetSprayColor(EffPCPPulseWork *work, u32 value) {
    work->mask = value;
}

void effPcpSetSprayScale(EffPCPPulseWork *work, f32 val) {
    work->scale = val;
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

extern EffPCPEventOwner *func_00197D38(void *params);

/* Serialized drift ranges followed by the retained event array. */
typedef struct EffPCPDriftEventParams {
    u8 pad00[0x54];
    s32 duration;
    u32 count;
    s32 delaySpread;
    u8 pad60[8];
    f32 startPosition, endPosition, startJitter, endJitter;
    f32 unk78;
    u8 pad7C[4];
    f32 unk80;
    u8 pad84[0x80];
} EffPCPDriftEventParams;

typedef struct EffPCPDriftEvent {
    void *event;
    s32 delay;
    f32 position, positionStep, angle, unk14;
} EffPCPDriftEvent;

typedef struct EffPCPDriftEventWork {
    EffPCPDriftEventParams params;
    EffPCPDriftEvent *entries;
    EffPCPEventOwner *owner;
    u8 unk10C;
    u8 pad10D[3];
    f32 scale;
    u32 color;
    u32 handle;
} EffPCPDriftEventWork;

/* Clone the source header (a longer one with 0x18-byte entries), then give every entry one event and a random negative start delay. */
EffPCPDriftEventWork *effPcpEntryWorkBCreate(EffPCPDriftEventParams *src, void *params) {
    u32 count = src->count;
    u32 handle = (u32)func_003292A8(count * 24 + 0x11C);
    EffPCPDriftEventWork *work = sdfResourceRetainAddress((void *)handle);
    EffPCPEventPlace place;
    EffPCPDriftEvent *entry;
    s32 life;
    u32 i;

    work->params = *src;
    entry = (EffPCPDriftEvent *)((u8 *)work + 0x11C);
    work->handle = handle;
    work->color = 0x80808080;
    work->unk10C = 1;
    work->entries = entry;
    work->scale = 1.0f;
    work->owner = func_00197D38(params);
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
    life = work->params.delaySpread;
    for (i = 0; i < count; i++) {
        entry->event = (void *)func_00197D68(work->owner, 2, &place);
        if (life > 0) {
            entry->delay = -(effMiscRand(D_003AA868) % life);
        } else {
            entry->delay = 0;
        }
        entry++;
    }
    return work;
}

void func_00189FF0(void *data) {
    void *work0;
    void *work1;

    work0 = effParamTableGetBlock(data, 0);
    work1 = effParamTableGetBlock(data, 1);
    effPcpEntryWorkBCreate(work0, work1);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018A038);

void effPcpEventGroupRelease(EffPCPDriftEventWork *work) {
    u32 i = 0;
    u32 count = work->params.count;
    EffPCPDriftEvent *entry = work->entries;

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


void effPcpDriftRandomizeSlot(EffPCPDriftEventWork *work, s32 index) {
    EffPCPDriftEvent *slot;
    f32 spread;
    f32 scale;

    slot = &work->entries[index];
    scale = work->scale;
    spread = work->params.startJitter;
    slot->position = work->params.startPosition * (effMiscRandUnitFloat(D_003AA868) * spread + (1.0f - spread)) * scale;
    spread = work->params.endJitter;
    slot->positionStep = (work->params.endPosition * (effMiscRandUnitFloat(D_003AA868) * spread + (1.0f - spread)) * scale - slot->position) / (f32)work->params.duration;
    slot->angle = 6.2831852f / (f32)work->params.count * (f32)index;
    spread = work->params.unk80;
    slot->unk14 = work->params.unk78 * (effMiscRandUnitFloat(D_003AA868) * spread + (1.0f - spread));
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018A428);

void effPcpCopyDriftVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpDriftSetScale(EffPCPDriftEventWork *work, f32 val) {
    work->scale = val;
}

void func_0018A690(EffPCPDriftEventWork *work, u32 value) {
    work->color = value;
}

typedef struct EffPCPPairedEventParams {
    u8 pad00[0x18];
    u32 count;
    s32 delaySpread;
    u8 pad20[8];
    f32 baseSpeed, spreadA, baseSpin, spreadB;
    u8 fragmentParams[0x54];
} EffPCPPairedEventParams;

typedef struct EffPCPPairedEvent {
    u32 fragment;
    void *eventA, *eventB;
    f32 phase, speed;
    u32 unk14;
    f32 spin;
    s32 delay;
} EffPCPPairedEvent;

typedef struct EffPCPPairedEventWork {
    EffPCPPairedEventParams params;
    EffPCPPairedEvent *entries;
    EffPCPEventOwner *ownerA, *ownerB;
    u8 unk98;
    u8 pad99[3];
    u32 color, handle;
} EffPCPPairedEventWork;

/* Clone the source effect header, then give every entry two events (placed at the unit scale) and a random negative start delay. */
EffPCPPairedEventWork *effPcpCreateDelayedDriftEntries(EffPCPPairedEventParams *src, void *paramsA, void *paramsB) {
    u32 count = src->count;
    u32 handle = (u32)func_003292A8(count * 32 + 0xA4);
    EffPCPPairedEventWork *work = sdfResourceRetainAddress((void *)handle);
    EffPCPEventPlace place;
    EffPCPPairedEvent *entry;
    s32 life;
    u32 i;

    work->params = *src;
    entry = (EffPCPPairedEvent *)((u8 *)work + 0xA4);
    work->handle = handle;
    work->entries = entry;
    work->color = 0x80808080;
    if (work->params.delaySpread <= 0) {
        work->params.delaySpread = 1;
    }
    work->unk98 = 1;
    work->ownerA = func_00197D38(paramsA);
    work->ownerB = func_00197D38(paramsB);
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
    life = work->params.delaySpread;
    for (i = 0; i < count; i++) {
        entry->fragment = effThunderFragCreate(src->fragmentParams);
        entry->eventA = (void *)func_00197D68(work->ownerA, 2, &place);
        entry->eventB = (void *)func_00197D68(work->ownerB, 2, &place);
        entry->delay = -(effMiscRand(D_003AA868) % life);
        entry++;
    }
    return work;
}

void effPcpDriftCreateFromTable(void *data) {
    void *work0;
    void *work1;
    void *work2;

    work0 = effParamTableGetBlock(data, 0);
    work1 = effParamTableGetBlock(data, 1);
    work2 = effParamTableGetBlock(data, 2);
    effPcpCreateDelayedDriftEntries(work0, work1, work2);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018A950);

void effPcpPairedEventGroupRelease(EffPCPPairedEventWork *work) {
    u32 i = 0;
    u32 count = work->params.count;
    EffPCPPairedEvent *entry = work->entries;

    if (count != 0) {
        do {
            effEventReleaseNode(entry->eventA);
            effEventReleaseNode(entry->eventB);
            effPCPThunderFree3(entry->fragment);
            entry++;
            i++;
        } while (i < count);
    }
    if (work->ownerA->active == 0) {
        func_00197D50(work->ownerA);
    }
    if (work->ownerB->active == 0) {
        func_00197D50(work->ownerB);
    }
    func_003297C8(work->handle);
}


/* Randomises slot `index`: phase spread evenly around a full turn, jittered
 * speed, and a spin whose sign is picked at random. */
void effPcpRandomizeSlot(EffPCPPairedEventWork *work, s32 index) {
    EffPCPPairedEvent *slot;
    f32 spread;

    slot = &work->entries[index];
    slot->phase = (3.14159265f * 2.0f) / work->params.count * index;
    spread = work->params.spreadA;
    slot->speed = work->params.baseSpeed * (effMiscRandUnitFloat(D_003AA868) * spread + (1.0f - spread));
    spread = work->params.spreadB;
    slot->unk14 = 0;
    if (effMiscRand(D_003AA868) & 1) {
        slot->spin = work->params.baseSpin * (effMiscRandUnitFloat(D_003AA868) * spread + (1.0f - spread));
    } else {
        slot->spin = -work->params.baseSpin * (effMiscRandUnitFloat(D_003AA868) * spread + (1.0f - spread));
    }
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018AD50);

void effPcpCopyRandomizedVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0018B128(EffPCPPairedEventWork *work, u32 value) {
    work->color = value;
}

typedef struct EffPCPSpawnRangeParams {
    u8 pad00[0x54];
    s32 duration;
    u32 count;
    s32 delaySpread;
    u8 pad60[8];
    f32 unk68, unk6C;
    u8 pad70[4];
    f32 unk74, unk78;
    u8 pad7C[4];
    f32 startPosition, endPosition, startJitter, endJitter;
    f32 unk90;
} EffPCPSpawnRangeParams;

typedef struct EffPCPSpawnRangeEvent {
    void *event;
    s32 delay;
    f32 unk08, unk0C, angle, unk14, position, positionStep;
} EffPCPSpawnRangeEvent;

typedef struct EffPCPSpawnRangeWork {
    EffPCPSpawnRangeParams params;
    EffPCPSpawnRangeEvent *entries;
    EffPCPEventOwner *owner;
    u8 unk9C;
    u8 pad9D[3];
    f32 scale;
    u32 color, handle;
} EffPCPSpawnRangeWork;

/* Clone the source header, then give every entry one event (placed at the unit scale) and a random negative start delay. */
void *effPcpCreateDelayedEventEntries(EffPCPSpawnRangeParams *src, void *params) {
    u32 count = src->count;
    u32 handle = (u32)func_003292A8(count * 32 + 0xAC);
    EffPCPSpawnRangeWork *work = sdfResourceRetainAddress((void *)handle);
    EffPCPEventPlace place;
    EffPCPSpawnRangeEvent *entry;
    s32 life;
    u32 i;

    work->params = *src;
    entry = (EffPCPSpawnRangeEvent *)((u8 *)work + 0xAC);
    work->handle = handle;
    work->color = 0x80808080;
    work->unk9C = 1;
    work->entries = entry;
    work->scale = 1.0f;
    work->owner = func_00197D38(params);
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
    life = work->params.delaySpread;
    for (i = 0; i < count; i++) {
        entry->event = (void *)func_00197D68(work->owner, 2, &place);
        if (life > 0) {
            entry->delay = -(effMiscRand(D_003AA868) % life);
        } else {
            entry->delay = 0;
        }
        entry++;
    }
    return work;
}

void effPcpSlotEffectCreateFromTable(void *data) {
    void *work0;
    void *work1;

    work0 = effParamTableGetBlock(data, 0);
    work1 = effParamTableGetBlock(data, 1);
    effPcpCreateDelayedEventEntries(work0, work1);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018B398);


void effPcpEventBatchRelease(EffPCPSpawnRangeWork *work) {
    u32 i = 0;
    u32 count = work->params.count;
    EffPCPSpawnRangeEvent *entry = work->entries;

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


/* Randomises spawn slot `index` inside the ranges held by the work. */
void effPcpRandomizeSpawnSlot(EffPCPSpawnRangeWork *work, s32 index) {
    EffPCPSpawnRangeEvent *slot;
    f32 spread;
    f32 scale;

    slot = &work->entries[index];
    scale = work->scale;
    slot->delay = 0;
    slot->unk08 = -work->params.unk90 * effMiscRandUnitFloat(D_003AA868);
    spread = work->params.unk78;
    slot->unk0C = work->params.unk74 * (effMiscRandUnitFloat(D_003AA868) * spread + (1.0f - spread));
    slot->angle = effMiscRandUnitFloat(D_003AA868) * (3.14159265f * 2.0f);
    spread = work->params.unk6C;
    slot->unk14 = work->params.unk68 * (effMiscRandUnitFloat(D_003AA868) * spread + (1.0f - spread));
    spread = work->params.startJitter;
    slot->position = work->params.startPosition * (effMiscRandUnitFloat(D_003AA868) * spread + (1.0f - spread)) * scale;
    spread = work->params.endJitter;
    slot->positionStep = (work->params.endPosition * (effMiscRandUnitFloat(D_003AA868) * spread + (1.0f - spread)) * scale - slot->position) / (f32)work->params.duration;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018B778);

void effPcpCopySpawnRangeVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effPcpSpawnRangeSetScale(EffPCPSpawnRangeWork *work, f32 val) {
    work->scale = val;
}

void effPcpSetSpawnRangeColor(EffPCPSpawnRangeWork *work, u32 value) {
    work->color = value;
}

typedef struct EffPCPEventInitEntry {
    u8 pad00[0x10];
    u8 unk10;
    u8 pad11[3];
    u32 unk14;
    u32 unk18;
    void *event;
} EffPCPEventInitEntry;

typedef struct EffPCPEventInitWork {
    u8 pad00[0x18];
    f32 unk18;
    EffPCPEventInitEntry *entries;
    u32 handle;
    EffPCPEventOwner *owner;
    u32 resource;
    f32 scale;
    u32 unk30;
    u32 unk34;
    u32 count;
    u32 color;
} EffPCPEventInitWork;

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

void effPcpEventWorkInitEntries(EffPCPEventInitWork *work) {
    EffPCPEventPlace place;
    u32 i = 0;
    u32 count;
    EffPCPEventModel *model;
    EffPCPEventInitEntry *entry;

    model = effParamWorkGetData(work->resource);
    model->info->unk20 = work->unk18;
    mdlAddEntryPlain(model, 0, 0);
    work->unk34 = model->info->unk2E;
    count = sdfCountMapPositionRecords(model->unk18);
    work->count = count;
    work->handle = (u32)func_003292A8(count << 5);
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
        entry->event = (void *)func_00197D68(work->owner, 2, &place);
        entry++;
    }
}

/* Leading float block of an event work, copied verbatim from its parameter block. */
typedef struct {
    f32 word[7];
} EffPCPEventParamHead;

extern EffPCPEventOwner *func_00197D38(void *params);

/* Allocate an event work: copy the parameter head, clear the links, then create the resource and owner from the optional parameters. */
void *effPcpEventWorkCreate(EffPCPEventParamHead *head, void *resourceParams, void *ownerParams) {
    EffPCPEventInitWork *work = func_00328D68(0x40);

    *(EffPCPEventParamHead *)work = *head;
    work->color = 0x80808080;
    work->scale = 1.0f;
    work->owner = 0;
    work->entries = 0;
    work->resource = 0;
    work->unk30 = 0;
    if (resourceParams != 0) {
        work->resource = effParamWorkCreate(3, resourceParams);
    }
    if (ownerParams != 0) {
        work->owner = func_00197D38(ownerParams);
    }
    if (work->owner != 0) {
        effPcpEventWorkInitEntries(work);
    }
    return work;
}

void effPcpEventWorkCreateFromTable(void *data) {
    void *work0;
    void *work1;
    void *work2;

    work0 = effParamTableGetBlock(data, 0);
    work1 = effParamTableGetBlock(data, 1);
    work2 = effParamTableGetBlock(data, 2);
    effPcpEventWorkCreate(work0, work1, work2);
}

typedef struct {
    u8 pad00[0x24];
    void *owner;
    u32 resource;
} EffPCPEventCloneWork;

EffPCPEventCloneWork *effEventWorkClone(EffPCPEventCloneWork *src) {
    EffPCPEventCloneWork *work;

    work = effPcpEventWorkCreate(src, 0, 0);
    work->resource = effParamWorkDuplicate(src->resource);
    work->owner = src->owner;
    effPcpEventWorkInitEntries(work);
    return work;
}

/* Fixed-stride event entries retained by the particle effect work. */
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
    u32 i = 0;
    u32 count;
    EffPCPEventEntry32 *entry;

    effDispatchParameterDataAndFreeWork(work->resource);
    count = work->count;
    entry = work->entries;
    if (count != 0) {
        do {
            u32 event = (u32)entry->event;
            entry++;
            if (event != 0) {
                effEventReleaseNode((void *)event);
            }
            i++;
        } while (i < count);
    }
    if (work->owner->active == 0) {
        func_00197D50(work->owner);
    }
    func_003297C8(work->handle);
    sdfReleaseChipBlock(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0018BD88);

INCLUDE_RODATA(const s32, "effect/effPCPMisc", D_00414610);

INCLUDE_SDATA(const s32, "effect/effPCPMisc", D_00436438);

INCLUDE_SDATA(const s32, "effect/effPCPMisc", D_0043643C);

INCLUDE_SDATA(const s32, "effect/effPCPMisc", D_0043643D);

