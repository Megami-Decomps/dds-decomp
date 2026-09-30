#include "common.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"

/* Event unit/work object shared by the setup helpers below and the
 * script opcodes. Field layout matches event/evtUnitManager's EvtUnit
 * where they overlap (value, flags, unkBC). */
typedef struct EvtUnit {
    u8 pad00[0x04];     /* 0x00 */
    s32 objectId;       /* 0x04: returned to event scripts */
    u8 pad08[0x60];     /* 0x08 */
    s32 unk68;          /* 0x68 */
    u32 value;          /* 0x6C: matches evtUnitManager */
    s128 unk70;          /* 0x70: 16-byte vector copied by the setup helpers */
    u8 pad80[0x0C];     /* 0x80 */
    u32 *flagWord;       /* 0x8C: status opcodes update its first bit */
    void *linkedUnit;    /* 0x90: world unit attached by setup helpers */
    s32 unk94;          /* 0x94 */
    s32 unk98;          /* 0x98 */
    s32 unk9C;          /* 0x9C */
    s32 unkA0;          /* 0xA0 */
    f32 unkA4;          /* 0xA4 */
    u32 flags;          /* 0xA8 */
    s16 unkAC;          /* 0xAC */
    s16 unkAE;          /* 0xAE */
    s16 unkB0;          /* 0xB0 */
    s16 unkB2;          /* 0xB2 */
    s16 unkB4;          /* 0xB4 */
    s16 unkB6;          /* 0xB6 */
    u8 padB8[0x04];     /* 0xB8 */
    u16 unkBC;          /* 0xBC */
    s16 unkBE;          /* 0xBE */
    s16 unkC0;          /* 0xC0 */
    u8 padC2[0x2E];     /* 0xC2 */
    s16 unkF0[1];       /* 0xF0 */
} EvtUnit;

/* Effect slot: three vec4 at +0x08/+0x18/+0x28 (the last carries w = 1.0f),
 * then the two floats returned by func_00223A10 at +0x38/+0x3C. */
typedef struct {
    s32 state;          /* 0x00: 2 or 3 when in use */
    s32 id;             /* 0x04: bound object id (state 3) */
    f32 vec[14];        /* 0x08 */
} EvtSlot;

extern EvtSlot D_003D7BD8[7];

typedef struct EvtSlotEnds {
    f32 (*points)[4];
    s32 unk4;
    s32 unk8;
} EvtSlotEnds;

extern void func_002E1938(s32, EvtSlotEnds *, f32 *);

typedef struct {
    u8 pad00[0x10];     /* 0x00 */
    f32 unk10;          /* 0x10 */
    u8 pad14[0x04];     /* 0x14 */
    f32 unk18;          /* 0x18 */
    f32 unk1C;          /* 0x1C */
    u8 pad20[0x250];    /* 0x20 */
} Entry270;

extern Entry270 *D_003BAA20;

extern void *dds3GetWorldObject(void);
extern void effObjSetInnerThirdVec(void *arg0, void *arg1);

extern u8 evtTestUnitStatusFlags(EvtUnit *unit);

extern EvtUnit *func_00222090(s32 idx);
extern void func_00221D00(EvtUnit *unit, s32 arg, u32 color1, u32 color2);
extern void func_00221E08(EvtUnit *unit, s32 arg, u32 color);
extern void func_00221EF0(EvtUnit *unit, s32 arg, u32 color);

extern u32 D_003BBDAC;
extern s32 D_003BBDB0;

/* Event lip-sync registry: world -> root -> list -> unit links. */
typedef struct EvtLipsModel {
    u8 pad00[0x18];
    void *chunk;        /* 0x18 */
} EvtLipsModel;

typedef struct EvtLipsMh {
    u8 pad00[0x0C];
    EvtLipsModel *model; /* 0x0C */
} EvtLipsMh;

typedef struct EvtLipsLink {
    u8 pad00[0x08];
    void *unit;         /* 0x08 */
    EvtLipsMh *mh;      /* 0x0C */
} EvtLipsLink;

typedef struct EvtLipsNode {
    u8 pad00[0x18];
    EvtLipsLink *link;  /* 0x18 */
    u8 pad1C[0x04];
    struct EvtLipsNode *next; /* 0x20 */
} EvtLipsNode;

typedef struct EvtLipsList {
    u8 pad00[0x40];
    EvtLipsNode *head;  /* 0x40 */
} EvtLipsList;

typedef struct EvtLipsRoot {
    u8 pad00[0x08];
    EvtLipsList *list;  /* 0x08 */
} EvtLipsRoot;

typedef struct EvtLipsWorld {
    u8 pad00[0x18];
    EvtLipsRoot *root;  /* 0x18 */
} EvtLipsWorld;

extern u32 sdfGetUniqueChunkValue();
extern s32 mdlGetNodeRefHalf();

extern s32 scrReadIntParameter(s32 idx);
extern s32 func_0021FC30(s32 arg0, s32 arg1);


extern void *func_00110A48(void *arg0, s32 arg1, s32 arg2);
extern void *func_00110A38(void *arg0);
extern s32 func_00222298(EvtUnit *unit);
extern void func_00115970(void *arg0);
extern void func_00110928(void *arg0);
extern void *dds3GetWorldSecondaryObject(void);
extern void func_00222B70(EvtUnit *work, s32 arg1, s128 *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);
extern void dds3FreePathObject(s32);
extern s32 func_00116D38(void *);
extern void func_00116F38(s32);
extern f32 func_00220830(s32);
extern void evtScaleValueByMultiplier(s32, f32);
extern void func_00117568(s32, s32);
extern void func_002E7F20(f32, f32, f32);
extern void effMiscQuatMultiplyVU();
extern void effObjSetInnerSecondVec(void *, void *);

extern void dds3SetObjectFlags(void *arg0, s32 arg1);
extern void dds3ClearObjectFlags(void *arg0, s32 arg1);
extern void func_00113478(void *arg0);
extern void func_00113438(void *arg0, s32 arg1);
extern s32 func_0010D6A0(void);
extern void func_0010AC10(const char *fmt, ...);
extern s32 func_00241E18(s32 arg0, s32 arg1);
extern s32 evtCreateMotionSeTask(s32 arg0, s32 arg1, s32 arg2);
extern s32 evtFindTaskById(s32 arg0);
extern void func_00101A80(s32 arg0, s32 arg1);
extern void func_002223D8(EvtUnit *unit, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern void func_00222340(EvtUnit *unit, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void func_003003F0();
extern u8 D_003AC480[];
extern void evtSetUnitValueTransition(EvtUnit *unit, void *arg1, s32 arg2);
extern s32 mdlCheckNodeByte30(u32 *arg0, s32 arg1);
extern void *memset(void *dst, s32 c, u32 n);
extern void func_00115318(void *arg0, u32 arg1);
extern void effObjSetInnerFirstVec(void *arg0, void *arg1);
extern f32 bfWaitReadArgFloat(s32 idx);

/* World object views used by the model-parameter opcodes. */
typedef struct EvtModelHeader {
    u8 pad00[0x04];
    u32 flags;          /* 0x04: bit 2 selects the header transform */
    u8 pad08[0x08];
    f32 unk10;          /* 0x10 */
    f32 unk14;          /* 0x14 */
    f32 unk18;          /* 0x18 */
} EvtModelHeader;

typedef struct EvtModelParams {
    u8 pad00[0x40];
    f32 unk40;          /* 0x40 */
    f32 unk44;          /* 0x44 */
    f32 unk48;          /* 0x48 */
    u8 pad4C[0x04];
    f32 unk50;          /* 0x50 */
    f32 unk54;          /* 0x54 */
    f32 unk58;          /* 0x58 */
    f32 unk5C;          /* 0x5C */
    u8 pad60[0x60];
    u32 flagsC0;        /* 0xC0 */
} EvtModelParams;

typedef struct EvtModelObj {
    u8 pad00[0x18];
    EvtModelHeader *header; /* 0x18 */
    EvtModelParams *params; /* 0x1C */
} EvtModelObj;

typedef struct EvtSourceVec {
    f32 unk00;          /* 0x00 */
    f32 unk04;          /* 0x04 */
    f32 unk08;          /* 0x08 */
    u8 pad0C[0x04];
    f32 unk10;          /* 0x10 */
    f32 unk14;          /* 0x14 */
    f32 unk18;          /* 0x18 */
    f32 unk1C;          /* 0x1C */
} EvtSourceVec;

typedef struct EvtSourceObj {
    u8 pad00[0x18];
    EvtSourceVec *vec;  /* 0x18 */
} EvtSourceObj;
extern char D_003AC588[];
extern u8 D_003AC2A0[];
extern u8 D_003AC550[];
extern u8 D_003AC5B0[];
extern u8 D_003AC600[];

typedef struct EvtLodRoot {
    u8 pad00[0x98];
    s8 lodIndex;        /* 0x98 */
} EvtLodRoot;

typedef struct EvtLodMh {
    u8 pad00[0x18];
    EvtLodRoot *root;   /* 0x18 */
} EvtLodMh;

typedef struct EvtLodWork {
    u8 pad00[0x0C];
    EvtLodMh *mh;       /* 0x0C */
} EvtLodWork;

typedef struct EvtLodModel {
    u8 pad00[0x0C];
    EvtLodWork *workbase; /* 0x0C */
} EvtLodModel;

typedef struct EvtLodUnit {
    u8 pad00[0x18];
    EvtLodModel *model;   /* 0x18 */
} EvtLodUnit;

extern s32 sdfGetLodChunkValue();
extern s32 scrGetWindow(void);
extern void func_0019CB98(s32 arg0, void (*arg1)(void));
extern void func_00222300(EvtUnit *unit, s32 arg1, s32 arg2);
extern s32 scrReadStringParameter(s32 idx);
extern void *func_00115858(s32 arg0, s32 arg1);
extern void effObjSetFlags(void *arg0, s32 arg1);
extern void *func_00114FA0(s32 arg0, void *arg1, void *arg2);
extern u8 D_003AC520[];
extern void *func_001152B0(s32 arg0, void *arg1, void *arg2);
extern s32 func_0010D5F0(s32 arg0);
extern void func_0021FD50(s32 arg0, s32 arg1);
extern void evtSetUnitStatusFlags(EvtUnit *unit);
extern void func_00221FF8(EvtUnit *unit, s32 arg1);
extern void evtEndUnitValueTransition(EvtUnit *unit, s32 arg1);
extern void func_00222310(u32 arg0);

typedef struct EvtWorldUnitRef {
    u8 pad00[0x18];
    s128 *transform; /* 0x18: first aligned vector */
} EvtWorldUnitRef;

void evtBeginVectorTransition(EvtUnit *work, s128 *vector, s32 frames) {
    if (frames > 0 && frames <= 100) {
        work->linkedUnit = NULL;
        work->unkAC = 3;
        PCP_COPY_VECTOR(&work->unk70, vector);
        work->unkB4 = frames;
        work->unkB6 = 0;
        work->unk94 = 0;
        work->unkB2 = 0;
    }
}

void evtAttachSecondaryWorldUnit(EvtUnit *work, s32 objectId, s32 frames) {
    EvtWorldUnitRef *worldUnit;

    worldUnit = func_00110A48(dds3GetWorldSecondaryObject(), objectId, 0x11);
    if (worldUnit != NULL) {
        evtBeginVectorTransition(work, worldUnit->transform + 1, frames);
        work->linkedUnit = worldUnit;
    }
}

void func_00222B70(EvtUnit *work, s32 arg1, s128 *vector, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    work->unkB0 = arg1;
    work->unkAC = 1;
    work->unkAE = 0;
    work->linkedUnit = NULL;
    PCP_COPY_VECTOR(&work->unk70, vector);
    work->unkB4 = arg4;
    work->unkB6 = arg5;
    work->unk94 = arg6;
    work->unkB2 = 0;
}

void func_00222BA8(EvtUnit *work, s32 arg1, s32 objectId, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    EvtWorldUnitRef *worldUnit;

    worldUnit = func_00110A48(dds3GetWorldSecondaryObject(), objectId, 0x11);
    if (worldUnit != NULL) {
        func_00222B70(work, arg1, worldUnit->transform, arg3, arg4, arg5, arg6, arg7);
        work->unkAE = 1;
        work->linkedUnit = worldUnit;
    }
}

void func_00222C68(EvtUnit *work, s32 objectId, s32 arg2, s32 arg3, s32 mode, s32 dirFlag, s32 sideMode) {
    void *pathSource;
    s32 path;

    pathSource = func_00110A48(dds3GetWorldSecondaryObject(), objectId, 0x10);
    if (pathSource == NULL) {
        return;
    }
    if (work->unkA0 != 0) {
        dds3FreePathObject(work->unkA0);
    }
    path = func_00116D38(pathSource);
    work->unkA0 = path;
    work->unkA4 = 40.0f / func_00220830(path);
    if (dirFlag == 0) {
        evtScaleValueByMultiplier(path, 0.0f);
        func_00117568(path, 0);
    } else {
        evtScaleValueByMultiplier(path, 1.0f);
        func_00117568(path, 1);
        work->unkA4 = -work->unkA4;
    }
    switch (mode) {
    case 0:
        work->unkB0 = 0;
        work->flags &= ~2;
        break;
    case 1:
        work->unkB0 = 3;
        work->flags |= 2;
        break;
    }
    switch (dirFlag) {
    case 0:
        work->flags &= ~4;
        break;
    case 1:
        work->flags |= 4;
        break;
    }
    switch (sideMode) {
    case 0:
        work->flags &= ~8;
        work->flags &= ~0x10;
        break;
    case 1:
        work->flags |= 8;
        work->flags &= ~0x10;
        break;
    case 2:
        work->flags &= ~8;
        work->flags |= 0x10;
        break;
    }
    work->unkAC = 1;
    work->unkAE = 2;
    work->linkedUnit = pathSource;
    func_00116F38(path);
    VU0_STORE_VF($vf10, &work->unk70);
    work->unkB4 = arg2;
    work->unkB6 = arg3;
    work->unk94 = 0;
    work->unkB2 = 0;
}

s32 func_00222EB0(EvtUnit *work, s32 arg1) {
    s32 ret = 0;

    if (arg1 != 0) {
        work->unk94 = arg1;
        work->unkB2 = 0;
        work->unkAC = 4;
        ret = 1;
    }
    return ret;
}

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00222ED8);

extern void func_00242BD0(f32 *, f32 *, f32 *, f32 *, f32 *);

void func_00223540(void) {
    s32 i;

    for (i = 0; i < 7; i++) {
        func_00242BD0(&D_003D7BD8[i].vec[0], &D_003D7BD8[i].vec[4], &D_003D7BD8[i].vec[8], &D_003D7BD8[i].vec[12], &D_003D7BD8[i].vec[13]);
        D_003D7BD8[i].state = 0;
        D_003D7BD8[i].id = 0;
    }
}

void func_002235E8(s32 index, s32 state, s32 id, f32 *a, f32 *b, f32 *c) {
    if (index < 7) {
        D_003D7BD8[index].vec[0] = a[0];
        D_003D7BD8[index].state = state;
        D_003D7BD8[index].vec[1] = a[1];
        D_003D7BD8[index].vec[2] = a[2];
        D_003D7BD8[index].vec[3] = 0;
        D_003D7BD8[index].vec[4] = b[0];
        D_003D7BD8[index].vec[5] = b[1];
        D_003D7BD8[index].vec[6] = b[2];
        D_003D7BD8[index].vec[7] = b[3];
        D_003D7BD8[index].vec[8] = c[0];
        D_003D7BD8[index].vec[9] = c[1];
        D_003D7BD8[index].vec[10] = c[2];
        D_003D7BD8[index].vec[11] = 1.0f;
        if (state == 3) {
            D_003D7BD8[index].id = id;
        } else {
            D_003D7BD8[index].id = 0;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00222AC0", evtSetSlotVector);

/* Copy the second vector of the slot bound to `id` (else the first state-2 slot). */
s32 func_00223718(s32 id, f32 *out) {
    s32 found = -1;
    s32 i;

    for (i = 0; i < 7; i++) {
        if (D_003D7BD8[i].state == 3 && D_003D7BD8[i].id == id) {
            found = i;
            break;
        }
    }
    if (found == -1) {
        for (i = 0; i < 7; i++) {
            if (D_003D7BD8[i].state == 2) {
                found = i;
                break;
            }
        }
        if (found == -1) {
            return 0;
        }
    }
    out[0] = D_003D7BD8[found].vec[4];
    out[1] = D_003D7BD8[found].vec[5];
    out[2] = D_003D7BD8[found].vec[6];
    return 1;
}

void func_00223828(EvtUnit *unit) {
    f32 ends[4][4];
    f32 color[4];
    EvtSlotEnds desc = { ends, 0, 0 };
    s32 found = -1;
    s32 i;

    for (i = 0; i < 7; i++) {
        if (D_003D7BD8[i].state == 3 && D_003D7BD8[i].id == (s32)unit) {
            found = i;
            break;
        }
    }
    if (found == -1) {
        for (i = 0; i < 7; i++) {
            if (D_003D7BD8[i].state == 2) {
                found = i;
                break;
            }
        }
        if (found == -1) {
            return;
        }
    }
    ends[0][0] = D_003D7BD8[found].vec[0];
    ends[0][1] = D_003D7BD8[found].vec[1];
    ends[0][2] = D_003D7BD8[found].vec[2];
    ends[0][3] = 0;
    ends[1][0] = D_003D7BD8[found].vec[4];
    ends[1][1] = D_003D7BD8[found].vec[5];
    ends[1][2] = D_003D7BD8[found].vec[6];
    ends[1][3] = 0;
    color[0] = D_003D7BD8[found].vec[8];
    color[1] = D_003D7BD8[found].vec[9];
    color[2] = D_003D7BD8[found].vec[10];
    color[3] = 1.0f;
    for (i = 0; i < 3; i++) {
        if (color[i] > 1.0f) {
            color[i] = 1.0f;
        }
    }
    func_002E1938(unit->unk68, &desc, color);
    unit->value = unit->unk68;
}

/* Find the vector of the slot bound to `id`, else of the first slot in state 2. */
s32 func_00223A10(s32 id, f32 *outX, f32 *outY) {
    s32 i;

    for (i = 0; i < 7; i++) {
        if (D_003D7BD8[i].state == 3 && D_003D7BD8[i].id == id) {
            *outX = D_003D7BD8[i].vec[12];
            *outY = D_003D7BD8[i].vec[13];
            return 1;
        }
    }
    for (i = 0; i < 7; i++) {
        if (D_003D7BD8[i].state == 2) {
            *outX = D_003D7BD8[i].vec[12];
            *outY = D_003D7BD8[i].vec[13];
            return 1;
        }
    }
    return 0;
}

void *evtFindWorldObjectByIdAndKind(s32 kind, s32 id) {
    void *world;

    world = dds3GetWorldObject();
    func_00110A48(world, id, kind);
}

u32 evtGetWorldObjectId(void) {
    void *world;
    EvtUnit *object;
    s32 id;

    world = dds3GetWorldObject();
    object = func_00110A38(world);
    id = -1;
    if (object != NULL) {
        id = object->objectId;
    }
    func_0010D5F0(id);
    return 1;
}

u32 func_00223B20(void) {
    s32 param0;
    s32 param1;
    s32 value;

    param0 = scrReadIntParameter(0);
    param1 = scrReadIntParameter(1);
    value = func_0021FC30(param0, param1);
    func_0010D5F0(value);
    return 1;
}

u32 func_00223B68(void) {
    s32 param0;
    s32 rid;
    s32 model;
    s32 ret;

    if (func_0010D6A0() == 0) {
        return 1;
    }
    param0 = scrReadIntParameter(0);
    rid = scrReadIntParameter(1);
    model = func_00241E18(param0, rid);
    if (model < 0) {
        func_0010AC10("MODEL_BE not fount RID = %d!\n", scrReadIntParameter(1));
        return 1;
    }
    param0 = scrReadIntParameter(0);
    rid = scrReadIntParameter(1);
    ret = evtCreateMotionSeTask(model, param0, rid);
    if (ret != 0) {
        func_00101A80(evtFindTaskById(scrReadIntParameter(0)), ret);
    }
    return func_0010D5F0(model);
}

u32 func_00223C60(void) {
    s32 param0;
    s32 param1;

    param0 = scrReadIntParameter(0);
    param1 = scrReadIntParameter(1);
    func_0021FD50(param0, param1);
    return 1;
}

u32 func_00223CA0(void) {
    void *ctx;
    s32 id;
    void *unit;
    u32 ret = 1;

    ctx = dds3GetWorldObject();
    id = scrReadIntParameter(0);
    unit = func_00110A48(ctx, id, 5);
    if (unit == NULL) {
        return ret;
    }
    dds3SetObjectFlags(unit, 0x400);
    dds3ClearObjectFlags(unit, 0x200);
    return 1;
}

u32 func_00223D10(void) {
    void *ctx;
    s32 id;
    void *unit;
    u32 ret = 1;

    ctx = dds3GetWorldObject();
    id = scrReadIntParameter(0);
    unit = func_00110A48(ctx, id, 5);
    if (unit == NULL) {
        return ret;
    }
    dds3ClearObjectFlags(unit, 0x400);
    dds3SetObjectFlags(unit, 0x200);
    return 1;
}

u32 func_00223D80(void) {
    void *ctx;
    s32 id;
    void *unit;
    u32 ret = 1;

    ctx = dds3GetWorldObject();
    id = scrReadIntParameter(0);
    unit = func_00110A48(ctx, id, 5);
    if (unit == NULL) {
        return ret;
    }
    dds3ClearObjectFlags(unit, 0x400);
    dds3ClearObjectFlags(unit, 0x200);
    return 1;
}

u32 func_00223DF0(void) {
    void *ctx;
    s32 id;
    void *unit;
    u32 ret = 1;

    ctx = dds3GetWorldObject();
    id = scrReadIntParameter(0);
    unit = func_00110A48(ctx, id, 5);
    if (unit == NULL) {
        return ret;
    }
    func_00113438(unit, scrReadIntParameter(1));
    return 1;
}

u32 func_00223E58(void) {
    void *ctx;
    s32 id;
    void *unit;
    u32 ret = 1;

    ctx = dds3GetWorldObject();
    id = scrReadIntParameter(0);
    unit = func_00110A48(ctx, id, 5);
    if (unit == NULL) {
        return ret;
    }
    func_00113478(unit);
    return 1;
}

u32 func_00223EB0(void) {
    s32 lod;
    void *world;
    EvtLodUnit *unit;
    EvtLodRoot *root;
    s32 max;

    lod = scrReadIntParameter(1);
    func_003003F0("call: MODEL_LOD_CHG(int,int)\n");
    world = dds3GetWorldObject();
    unit = func_00110A48(world, scrReadIntParameter(0), 5);
    if (unit == NULL) {
        func_003003F0("warning!! MODEL_LOD_CHG(int,int) unit pointer null\n");
        return 1;
    }
    if (unit->model->workbase == NULL) {
        func_003003F0("warning!! MODEL_LOD_CHG(int,int) workbase pointer null\n");
        return 1;
    }
    if (unit->model->workbase->mh == NULL) {
        func_003003F0("warning!! MODEL_LOD_CHG(int,int) mh pointer null\n");
        return 1;
    }
    root = unit->model->workbase->mh->root;
    if (root == NULL) {
        func_003003F0("warning!! MODEL_LOD_CHG(int,int) root pointer null\n");
        return 1;
    }
    max = sdfGetLodChunkValue(root);
    if (max < lod) {
        func_003003F0("warning!! MODEL_LOD_CHG(int,int) lodno over!! max=%d setval=%d\n", max, lod);
        return 1;
    }
    root->lodIndex = lod;
    func_003003F0("success: MODEL_LOD_CHG(int,int)\n");
    return 1;
}

u32 evtSetWorldUnitFirstVector(void) {
    f32 v[4];
    void *ctx;
    s32 id;
    void *unit;

    memset(v, 0, 0x10);
    ctx = dds3GetWorldObject();
    id = scrReadIntParameter(0);
    unit = func_00110A48(ctx, id, 5);
    if (unit == NULL) {
        return 1;
    }
    v[0] = bfWaitReadArgFloat(1);
    v[1] = bfWaitReadArgFloat(2);
    v[2] = bfWaitReadArgFloat(3);
    effObjSetInnerFirstVec(unit, v);
    return 1;
}

u32 func_00224048(void) {
    f32 v[4];
    void *ctx;
    s32 id;
    void *unit;
    f32 toRad;
    f32 x;
    f32 y;

    memset(v, 0, 0x10);
    v[3] = 1.0f;
    ctx = dds3GetWorldObject();
    id = scrReadIntParameter(0);
    unit = func_00110A48(ctx, id, 5);
    if (unit == NULL) {
        return 1;
    }
    toRad = 0.017453293f;
    x = bfWaitReadArgFloat(1) * toRad;
    y = bfWaitReadArgFloat(2) * toRad;
    func_002E7F20(x, y, 0.0f);
    VU0_MOVE_VF(vf11, vf10);
    func_002E7F20(0.0f, 0.0f, bfWaitReadArgFloat(3) * toRad);
    effMiscQuatMultiplyVU();
    VU0_STORE_VF($vf10, v);
    effObjSetInnerSecondVec(unit, v);
    return 1;
}

u32 evtSetWorldUnitThirdVector(void) {
    f32 v[4];
    void *ctx;
    s32 id;
    void *unit;

    memset(v, 0, 0x10);
    v[3] = 1.0f;
    ctx = dds3GetWorldObject();
    id = scrReadIntParameter(0);
    unit = func_00110A48(ctx, id, 5);
    if (unit == NULL) {
        return 1;
    }
    v[0] = bfWaitReadArgFloat(1);
    v[1] = bfWaitReadArgFloat(2);
    v[2] = bfWaitReadArgFloat(3);
    effObjSetInnerThirdVec(unit, v);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00222AC0", func_002241D0);

u32 evtOpSetUnitParams5(void) {
    EvtUnit *unit;

    unit = func_00222090(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 arg1 = scrReadIntParameter(1);
        s32 arg2 = scrReadIntParameter(2);
        s32 arg3 = scrReadIntParameter(3);
        s32 arg4 = scrReadIntParameter(4);
        func_002223D8(unit, arg1, arg2, arg3, arg4);
    }
    return 1;
}

extern void func_00222340(EvtUnit *, s32, s32, s32, s32, s32);

u32 evtOpSetUnitParams6(void) {
    EvtUnit *unit;

    unit = func_00222090(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 arg1 = scrReadIntParameter(1);
        s32 arg2 = scrReadIntParameter(2);
        s32 arg3 = scrReadIntParameter(3);
        s32 arg4 = scrReadIntParameter(4);
        s32 arg5 = scrReadIntParameter(5);
        func_00222340(unit, arg1, arg2, arg3, arg4, arg5);
    }
    return 1;
}

void func_002243C0(void) {
    func_00222310(D_003BBDAC);
}

INCLUDE_RODATA(const s32, "game/code_00222AC0", D_003AC2A0);

void func_002243D8(s32 id, s32 motion) {
    void *unit = NULL;
    EvtLipsModel *model = NULL;
    EvtLipsNode *node;

    if (id == 0) {
        return;
    }
    for (node = ((EvtLipsWorld *)dds3GetWorldObject())->root->list->head; node != NULL; node = node->next) {
        model = node->link->mh->model;
        if (sdfGetUniqueChunkValue(model->chunk) == id) {
            unit = node->link->unit;
            break;
        }
    }
    if (unit == NULL) {
        func_003003F0("warning: call evtLipsExecFunction() but not find now reegisted unit same UnitUniqID\n");
        return;
    }
    if (motion >= mdlGetNodeRefHalf(model, 2)) {
        func_003003F0("warning: call evtLipsExecFunction() but over have motionno fpr user specified motion no.\n");
        return;
    }
    func_00222340(unit, 2, motion, 0, 5, 1);
    D_003BBDB0 = id;
    func_003003F0("<lips %d %d> \n", id, motion);
}

void func_00224530(void) {
    void *unit = NULL;
    EvtLipsModel *model = NULL;
    EvtLipsNode *node;

    if (D_003BBDB0 == 0) {
        return;
    }
    for (node = ((EvtLipsWorld *)dds3GetWorldObject())->root->list->head; node != NULL; node = node->next) {
        model = node->link->mh->model;
        if (sdfGetUniqueChunkValue(model->chunk) == D_003BBDB0) {
            unit = node->link->unit;
            break;
        }
    }
    if (unit == NULL) {
        func_003003F0("warning: call evtLipsStopFunction() but not find now reegisted unit same UnitUniqID\n");
        return;
    }
    if (mdlGetNodeRefHalf(model, 2) == 0) {
        func_003003F0("warning: call evtLipsStopFunction() but over have motionno fpr user specified motion no.\n");
        return;
    }
    func_00222340(unit, 2, 0, 0, 3, 2);
    func_003003F0("<lips_stop> stopunitid = %d\n", D_003BBDB0);
    D_003BBDB0 = 0;
}

u32 evtOpBeginWindowCallback(void) {
    EvtUnit *unit;

    unit = func_00222090(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 arg1 = scrReadIntParameter(1);
        s32 arg2 = scrReadIntParameter(2);
        func_00222300(unit, arg1, arg2);
        D_003BBDAC = (u32)unit;
    }
    {
        s32 window = scrGetWindow();
        if (window < 0) {
            return 1;
        }
        func_0019CB98(window, func_002243C0);
    }
    return 1;
}

u32 func_002246D0(void) {
    EvtUnit *unit;

    unit = func_00222090(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 arg1 = scrReadIntParameter(1);
        s32 arg2 = scrReadIntParameter(2);
        func_00222300(unit, arg1, arg2);
        func_00222310((u32)unit);
        D_003BBDAC = (u32)unit;
    }
    {
        s32 window = scrGetWindow();
        if (window < 0) {
            return 1;
        }
        func_0019CB98(window, func_002243C0);
    }
    return 1;
}

u32 func_00224770(void) {
    s32 id;
    EvtUnit *unit;
    u32 ret = 1;

    id = scrReadIntParameter(0);
    unit = func_00222090(id);
    if (unit == NULL) {
        return ret;
    }
    return func_00222298(unit) != 0;
}

u32 func_002247B0(void) {
    s32 id;
    EvtUnit *unit;
    s32 offset;
    u32 result = 1;

    id = scrReadIntParameter(0);
    unit = func_00222090(id);
    if (unit == NULL) {
        return result;
    }
    offset = scrReadIntParameter(1);
    if (((((u8 *)(offset + (s32)unit))[0xE0] & 1) & 0xFF) == 0) {
        return result;
    }
    return mdlCheckNodeByte30(unit->flagWord, scrReadIntParameter(1)) != 0;
}

u32 func_00224828(void) {
    void *ctx;
    s32 id;
    void *unit;
    u32 ret = 1;

    ctx = dds3GetWorldObject();
    id = scrReadIntParameter(0);
    unit = func_00110A48(ctx, id, 5);
    if (unit == NULL) {
        return ret;
    }
    func_00110928(unit);
    return 1;
}

extern void func_002227C8(EvtUnit *unit);

u32 func_00224880(void) {
    EvtUnit *unit;

    unit = func_00222090(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    unit->unkB2 = 0;
    {
        s32 arg1 = scrReadIntParameter(2);
        s32 arg2 = scrReadIntParameter(1);
        s32 arg3 = scrReadIntParameter(3);
        s32 arg4 = scrReadIntParameter(4);
        func_00222BA8(unit, arg1, arg2, -1, arg3, arg4, 0, 0);
    }
    if (scrReadIntParameter(2) == 1) {
        func_002227C8(unit);
    }
    return 1;
}

u32 func_00224948(void) {
    EvtUnit *unit;

    unit = func_00222090(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    unit->unkB2 = 0;
    {
        s32 arg1 = scrReadIntParameter(1);
        s32 arg2 = scrReadIntParameter(5);
        s32 arg3 = scrReadIntParameter(6);
        s32 arg4 = scrReadIntParameter(2);
        s32 arg5 = scrReadIntParameter(4);
        s32 arg6 = scrReadIntParameter(3);
        func_00222C68(unit, arg1, arg2, arg3, arg4, arg5, arg6);
    }
    return 1;
}

u32 evtOpSetUnitTableEntry(void) {
    EvtUnit *unit;

    unit = func_00222090(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 index = scrReadIntParameter(1);
        s32 value = scrReadIntParameter(2);
        unit->unkF0[index] = value;
    }
    return 1;
}

u32 evtCommandSetUnitValue(void) {
    s32 id;
    EvtUnit *unit;

    id = scrReadIntParameter(0);
    unit = func_00222090(id);
    if (unit == NULL) {
        return 1;
    }
    unit->unkBC = scrReadIntParameter(1);
    return 1;
}

u32 func_00224AD0(void) {
    s32 id;
    EvtUnit *unit;

    id = scrReadIntParameter(0);
    unit = func_00222090(id);
    if (unit == NULL) {
        return 1;
    }
    unit->unkBE = scrReadIntParameter(1);
    unit->unkC0 = scrReadIntParameter(2);
    return 1;
}

u32 func_00224B28(void) {
    s32 id;
    EvtUnit *unit;

    id = scrReadIntParameter(0);
    unit = func_00222090(id);
    if (unit == NULL) {
        return 1;
    }
    func_002223D8(unit, scrReadIntParameter(1), 5, 7, 1);
    func_003003F0(D_003AC480);
    return 1;
}

u32 func_00224B98(void) {
    s32 id;
    EvtUnit *unit;
    s32 param1;
    s32 param2;

    id = scrReadIntParameter(0);
    unit = func_00222090(id);
    if (unit == NULL) {
        return 1;
    }
    unit->unkB2 = 0;
    param1 = scrReadIntParameter(1);
    param2 = scrReadIntParameter(2);
    evtAttachSecondaryWorldUnit(unit, param1, param2);
    return 1;
}

f32 evtGetShortestAngleDelta(f32 fromDegrees, f32 toDegrees) {
    f32 difference;

    if (fromDegrees < 0.0f || toDegrees < 0.0f) {
        fromDegrees += 360.0f;
        toDegrees += 360.0f;
    }
    fromDegrees = (s32)fromDegrees % 360;
    toDegrees = (s32)toDegrees % 360;
    difference = fromDegrees - toDegrees;
    if (difference > 180.0f || difference < -180.0f) {
        if (fromDegrees < toDegrees) {
            fromDegrees += 360.0f;
        } else {
            toDegrees += 360.0f;
        }
    }
    return toDegrees - fromDegrees;
}

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00224CD8);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00224F48);

u32 evtUnitClearFlagBit(void) {
    s32 id;
    EvtUnit *unit;

    id = scrReadIntParameter(0);
    unit = func_00222090(id);
    if (unit != NULL) {
        *unit->flagWord &= ~1;
    }
    return 1;
}

u32 evtUnitSetFlagBit(void) {
    s32 id;
    EvtUnit *unit;

    id = scrReadIntParameter(0);
    unit = func_00222090(id);
    if (unit != NULL) {
        *unit->flagWord |= 1;
    }
    return 1;
}

u32 func_00225160(void) {
    s32 id;
    EvtUnit *unit;

    id = scrReadIntParameter(0);
    unit = func_00222090(id);
    evtSetUnitStatusFlags(unit);
    return 1;
}

u32 func_00225190(void) {
    s32 id;
    EvtUnit *unit;
    s32 param1;

    id = scrReadIntParameter(0);
    unit = func_00222090(id);
    param1 = scrReadIntParameter(1);
    func_00221FF8(unit, param1);
    return 1;
}

u8 evtUnitHasNoStatusFlags(void) {
    s32 id;
    EvtUnit *unit;
    u8 active;

    id = scrReadIntParameter(0);
    unit = func_00222090(id);
    active = evtTestUnitStatusFlags(unit);
    return active == 0;
}

u32 func_00225208(void) {
    EvtUnit *unit;
    s32 color1[4];
    s32 color2[4];
    u32 packed1;
    u32 packed2;
    u32 scale;

    unit = func_00222090(scrReadIntParameter(0));
    VU0_SCALAR_OP(bfWaitReadArgFloat(2), "vaddx.x vf10, vf0, vf2x");
    VU0_SCALAR_OP(bfWaitReadArgFloat(3), "vaddx.y vf10, vf0, vf2x");
    VU0_SCALAR_OP(bfWaitReadArgFloat(4), "vaddx.z vf10, vf0, vf2x");
    VU0_CLEAR_W(vf10);
    scale = 0x43000000;
    EE_MMI_RGBA_PACK_UNIT(packed1, scale);
    color1[0] = packed1;
    VU0_SCALAR_OP(bfWaitReadArgFloat(5), "vaddx.x vf10, vf0, vf2x");
    VU0_SCALAR_OP(bfWaitReadArgFloat(6), "vaddx.y vf10, vf0, vf2x");
    VU0_SCALAR_OP(bfWaitReadArgFloat(7), "vaddx.z vf10, vf0, vf2x");
    VU0_SET_W_ONE(vf10);
    EE_MMI_RGBA_PACK_UNIT(packed2, scale);
    color2[0] = packed2;
    func_00221D00(unit, scrReadIntParameter(1), packed1, packed2);
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00222AC0", D_003AC480);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225330);

u32 func_00225408(void) {
    EvtUnit *unit;
    s32 color[4];
    u32 packed;

    unit = func_00222090(scrReadIntParameter(0));
    VU0_SCALAR_OP(bfWaitReadArgFloat(2), "vaddx.x vf10, vf0, vf2x");
    VU0_SCALAR_OP(bfWaitReadArgFloat(3), "vaddx.y vf10, vf0, vf2x");
    VU0_SET_AXIS_CLEAR_W(bfWaitReadArgFloat(4), z);
    EE_MMI_RGBA_PACK_F128(packed);
    color[0] = packed;
    func_00221E08(unit, scrReadIntParameter(1), packed);
    return 1;
}

u32 func_002254C8(void) {
    EvtUnit *unit;
    s32 color[4];
    u32 packed;

    unit = func_00222090(scrReadIntParameter(0));
    VU0_MOVE_VF(vf10, vf0);
    VU0_SCALAR_OP(bfWaitReadArgFloat(2), "vmulx.w vf10, vf0, vf2x");
    EE_MMI_RGBA_PACK_F128(packed);
    color[0] = packed;
    func_00221EF0(unit, scrReadIntParameter(1), packed);
    return 1;
}

u32 func_00225560(void) {
    s32 id;
    EvtUnit *unit;
    void *target;

    id = scrReadIntParameter(0);
    unit = func_00222090(id);
    target = evtFindWorldObjectByIdAndKind(9, scrReadIntParameter(2));
    if (target == NULL) {
        return 1;
    }
    evtSetUnitValueTransition(unit, target, scrReadIntParameter(1));
    return 1;
}

u32 func_002255D8(void) {
    s32 id;
    EvtUnit *unit;
    s32 param1;

    id = scrReadIntParameter(0);
    unit = func_00222090(id);
    param1 = scrReadIntParameter(1);
    evtEndUnitValueTransition(unit, param1);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225620);

u32 func_00225708(void) {
    u8 buf1[16];
    u8 buf2[16];
    s32 param0;
    EvtUnit *unit;

    memset(buf1, 0, 0x10);
    memset(buf2, 0, 0x10);
    param0 = scrReadStringParameter(0);
    unit = func_00114FA0(param0, buf1, buf2);
    if (unit == NULL) {
        func_003003F0(D_003AC520, 1);
        func_003003F0(D_003AC550, scrReadStringParameter(0));
        func_0010D5F0(0);
    } else {
        effObjSetFlags(unit, 1);
        func_0010D5F0(unit->objectId);
    }
    return 1;
}

u32 func_002257C0(void) {
    f32 buf1[4];
    f32 buf2[4];
    s32 param0;
    EvtUnit *unit;

    memset(buf1, 0, 0x10);
    memset(buf2, 0, 0x10);
    buf2[3] = 1.0f;
    param0 = scrReadStringParameter(0);
    unit = func_001152B0(param0, buf1, buf2);
    if (unit == NULL) {
        func_003003F0(D_003AC520, 1);
        func_003003F0(D_003AC550, scrReadStringParameter(0));
        func_0010D5F0(0);
    } else {
        effObjSetFlags(unit, 1);
        func_0010D5F0(unit->objectId);
    }
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00222AC0", D_003AC508);

INCLUDE_RODATA(const s32, "game/code_00222AC0", D_003AC520);

INCLUDE_RODATA(const s32, "game/code_00222AC0", D_003AC550);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225880);

u32 func_00225958(void) {
    return 1;
}

u32 evtUnitCheckModelCut(void) {
    s32 id;
    void *unit;
    u32 param1;

    id = scrReadIntParameter(0);
    unit = evtFindWorldObjectByIdAndKind(7, id);
    if (unit != NULL) {
        param1 = scrReadIntParameter(1);
        if (param1 >= 3U) {
            func_0010AC10(D_003AC588, param1);
            return 1;
        } else {
            func_00115318(unit, param1);
        }
    }
    return 1;
}

f32 func_002259E0(s32 index) {
    f32 scale = (D_003BAA20[index].unk18 + D_003BAA20[index].unk1C * 0.5f) * 0.5f * D_003BAA20[index].unk10 * (1.0f / 70.0f);

    if (scale > 2.0f) {
        scale = 2.0f;
    } else if (scale < 0.8f) {
        scale = 0.8f;
    }
    return scale;
}

extern f32 func_002259E0(s32);
extern void func_00190308(void *, f32);

u32 func_00225A60(void) {
    void *unit;

    unit = evtFindWorldObjectByIdAndKind(7, scrReadIntParameter(0));
    if (unit != NULL) {
        s32 mode = scrReadIntParameter(1);
        s32 index;
        if ((u32)mode >= 3) {
            func_0010AC10(D_003AC588, mode);
            return 1;
        }
        func_00115318(unit, mode);
        index = scrReadIntParameter(2);
        if (index >= 0) {
            f32 value = func_002259E0(index);
            void *target = *(void **)(*(u8 **)((u8 *)unit + 0x18) + 0x2C);
            if (target != NULL) {
                func_00190308(target, value);
            }
        }
    }
    return 1;
}

u32 func_00225B10(void) {
    s32 param0;
    EvtUnit *unit;

    param0 = scrReadStringParameter(0);
    unit = func_00115858(1, param0);
    if (unit == NULL) {
        func_003003F0(D_003AC5B0, 1);
        func_003003F0(D_003AC550, scrReadStringParameter(0));
        func_0010D5F0(0);
    } else {
        effObjSetFlags(unit, 1);
        func_0010D5F0(unit->objectId);
    }
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00222AC0", D_003AC588);

INCLUDE_RODATA(const s32, "game/code_00222AC0", D_003AC5B0);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225BA0);

u32 func_00225C48(void) {
    s32 param0;
    EvtUnit *unit;

    param0 = scrReadStringParameter(0);
    unit = func_00115858(2, param0);
    if (unit == NULL) {
        func_003003F0(D_003AC600, 1);
        func_003003F0(D_003AC550, scrReadStringParameter(0));
        func_0010D5F0(0);
    } else {
        effObjSetFlags(unit, 1);
        func_0010D5F0(unit->objectId);
    }
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00222AC0", D_003AC600);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225CD8);

u32 func_00225D80(void) {
    s32 id;
    void *obj;

    id = scrReadIntParameter(0);
    obj = evtFindWorldObjectByIdAndKind(7, id);
    if (obj != NULL) {
        func_00115970(obj);
    }
    return 1;
}

u32 func_00225DC0(void) {
    s32 id;
    void *obj;

    id = scrReadIntParameter(0);
    obj = evtFindWorldObjectByIdAndKind(7, id);
    if (obj != NULL) {
        func_00115970(obj);
    }
    return 1;
}

u32 func_00225E00(void) {
    s32 id;
    void *obj;

    id = scrReadIntParameter(0);
    obj = evtFindWorldObjectByIdAndKind(7, id);
    if (obj != NULL) {
        func_00110928(obj);
    }
    return 1;
}

u32 func_00225E40(void) {
    EvtModelObj *obj;
    EvtModelHeader *header;

    obj = evtFindWorldObjectByIdAndKind(7, scrReadIntParameter(0));
    header = obj->header;
    if (header->flags & 4) {
        header->unk10 = bfWaitReadArgFloat(1);
        header->unk14 = bfWaitReadArgFloat(2);
        header->unk18 = bfWaitReadArgFloat(3);
    } else {
        obj->params->unk40 = bfWaitReadArgFloat(1);
        obj->params->unk44 = bfWaitReadArgFloat(2);
        obj->params->unk48 = bfWaitReadArgFloat(3);
        obj->params->flagsC0 = (obj->params->flagsC0 | 1) & ~2;
    }
    return 1;
}

u32 func_00225F08(void) {
    f32 v[4];
    EvtModelObj *obj;
    f32 toRad;
    f32 x;
    f32 y;

    memset(v, 0, 0x10);
    v[3] = 1.0f;
    obj = evtFindWorldObjectByIdAndKind(7, scrReadIntParameter(0));
    if (!(obj->header->flags & 4)) {
        toRad = 0.017453293f;
        x = bfWaitReadArgFloat(1) * toRad;
        y = bfWaitReadArgFloat(2) * toRad;
        func_002E7F20(x, y, 0.0f);
        VU0_MOVE_VF(vf11, vf10);
        func_002E7F20(0.0f, 0.0f, bfWaitReadArgFloat(3) * toRad);
        effMiscQuatMultiplyVU();
        VU0_STORE_VF($vf10, v);
        effObjSetInnerSecondVec(obj, v);
    }
    return 1;
}

u32 func_00225FE8(void) {
    EvtModelObj *obj;
    EvtSourceObj *source;
    EvtSourceVec *vec;
    EvtModelParams *params;

    obj = evtFindWorldObjectByIdAndKind(7, scrReadIntParameter(0));
    if (obj == NULL) {
        return 1;
    }
    source = evtFindWorldObjectByIdAndKind(0x11, scrReadIntParameter(1));
    if (source == NULL) {
        return 1;
    }
    vec = source->vec;
    if (!(obj->header->flags & 4)) {
        params = obj->params;
        params->unk40 = vec->unk00;
        params->unk44 = vec->unk04;
        params->unk48 = vec->unk08;
        params->unk50 = vec->unk10;
        params->unk54 = vec->unk14;
        params->unk58 = vec->unk18;
        params->unk5C = vec->unk1C;
    }
    obj->params->flagsC0 = (obj->params->flagsC0 | 1) & ~2;
    return 1;
}

INCLUDE_SDATA(const s32, "game/code_00222AC0", D_003BBDAC);

INCLUDE_SDATA(const s32, "game/code_00222AC0", D_003BBDB0);

