#include "common.h"
#include "pcp_vu0.h"

extern u32 D_004371EC;
extern s32 D_004371F0;

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

/* Event work layout overlaps event/evtUnitManager's EventUnit at
 * value6C, flags, and valueBC. */
typedef struct EvtUnit {
    u8 pad00[0x04];     /* 0x00 */
    s32 unk04;          /* 0x04 */
    u8 pad08[0x60];     /* 0x08 */
    s32 unk68;          /* 0x68 */
    u32 value6C;        /* 0x6C */
    s128 vector;         /* 0x70: 16-byte vector copied by the setup helpers */
    u8 pad80[0x0C];     /* 0x80 */
    u32 *modelNode;      /* 0x8C: node flag word updated by script opcodes */
    void *sourceUnit;    /* 0x90: matching secondary-world unit */
    s32 unk94;          /* 0x94 */
    s32 unk98;          /* 0x98 */
    s32 unk9C;          /* 0x9C */
    s32 unkA0;          /* 0xA0 */
    f32 unkA4;          /* 0xA4 */
    u32 flags;          /* 0xA8 */
    s16 mode;            /* 0xAC */
    s16 unkAE;          /* 0xAE */
    s16 unkB0;          /* 0xB0 */
    s16 unkB2;          /* 0xB2 */
    s16 unkB4;          /* 0xB4 */
    s16 unkB6;          /* 0xB6 */
    u8 padB8[0x04];     /* 0xB8 */
    u16 valueBC;        /* 0xBC */
    s16 unkBE;          /* 0xBE */
    s16 unkC0;          /* 0xC0 */
    u8 padC2[0x2E];     /* 0xC2 */
    s16 unkF0[12];      /* 0xF0 */
    s16 unk108[12];     /* 0x108 */
    s16 unk120[12];     /* 0x120 */
    f32 unk138[12];     /* 0x138 */
} EvtUnit;

/* World lookup results carry the address of their vector-bearing data at +0x18. */
typedef struct EvtWorldVectorSource {
    u8 pad00[0x18];
    u32 vectorData;
} EvtWorldVectorSource;

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

/* Effect slot: three vec4 at +0x08/+0x18/+0x28 (the last carries w = 1.0f),
 * then the two floats returned by func_0023E648 at +0x38/+0x3C. */
typedef struct {
    s32 state;          /* 0x00: 2 or 3 when in use */
    s32 id;             /* 0x04: bound object id (state 3) */
    f32 vec[14];        /* 0x08 */
} EvtSlot;

extern EvtSlot D_004536D8[10];

typedef struct EvtSlotEnds {
    f32 (*points)[4];
    s32 unk4;
    s32 unk8;
} EvtSlotEnds;

extern void func_0033A7E8(s32, EvtSlotEnds *, f32 *);

typedef struct {
    u8 pad00[0x10];     /* 0x00 */
    f32 unk10;          /* 0x10 */
    u8 pad14[0x04];     /* 0x14 */
    f32 unk18;          /* 0x18 */
    f32 unk1C;          /* 0x1C */
    u8 pad20[0x250];    /* 0x20 */
} Entry270;

extern Entry270 *D_00435DF0;
extern void func_0025DFE8(f32 *, f32 *, f32 *, f32 *, f32 *);

extern void *func_00110C70(void *arg0, s32 arg1, s32 arg2);

extern void *dds3GetWorldSecondaryObject(void);
extern void dds3FreePathObject(s32);
extern s32 func_00116FA0(void *);
extern void func_001171A0(s32);
extern f32 func_0023B3A0(s32);
extern void evtScaleValueByMultiplier(s32, f32);
extern void func_001177D0(s32, s32);

extern void func_0023D708(EvtUnit *work, s32 arg1, s128 *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);

extern void *dds3GetWorldObject(void);

extern void *func_00110C60(void *arg0);

extern s32 func_0010D818(s32 arg0);

extern s32 scrReadIntParameter(s32 idx);

extern void dds3SetObjectFlags(void *arg0, s32 arg1);

extern void dds3ClearObjectFlags(void *arg0, s32 arg1);

extern void func_00113660(void *arg0, s32 arg1);

extern void func_001136A0(void *arg0);

extern void *memset(void *dst, s32 c, u32 n);

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

extern void effObjSetInnerThirdVec(void *arg0, void *arg1);
extern void func_00340DC8(f32, f32, f32);
extern void effMiscQuatMultiplyVU();
extern void effObjSetInnerSecondVec(void *, void *);

extern EvtUnit *func_0023CC00(s32 idx);

extern s32 func_0023CE30(EvtUnit *unit);

extern s32 mdlCheckNodeByte30(u32 *arg0, s32 arg1);

extern void func_00110B50(void *arg0);

extern void func_0023CF70(EvtUnit *unit, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

extern void func_0035B6E0();

extern u8 D_004219F0[];

extern u8 D_00421AC0[];

extern s32 scrReadStringParameter(s32 idx);

extern void effObjSetFlags(void *arg0, s32 arg1);

extern void *func_00115208(s32 arg0, void *arg1, void *arg2);

extern u8 D_00421A90[];

extern void *func_00115518(s32 arg0, void *arg1, void *arg2);

extern u8 D_00421B20[];

extern void *func_00115AC0(s32 arg0, s32 arg1);

extern u8 D_00421B70[];

extern s32 func_0023A7A0(s32 arg0, s32 arg1);

extern void evtSetUnitStatusFlags(EvtUnit *unit);

extern void func_0023CB68(EvtUnit *unit, s32 arg1);

extern u8 evtTestUnitStatusFlags(EvtUnit *unit);

extern void evtSetUnitValueTransition(EvtUnit *unit, void *arg1, s32 arg2);

extern void evtEndUnitValueTransition(EvtUnit *unit, s32 arg1);

extern void func_0010AE38(const char *fmt, ...);

extern void func_00115580(void *arg0, u32 arg1);

extern char D_00421AF8[];

extern void func_00115BD8(void *arg0);

extern s32 func_0010D8C8(void);

extern s32 func_0025D230(s32 arg0, s32 arg1);

extern s32 evtCreateMotionSeTask(s32 arg0, s32 arg1, s32 arg2);

extern s32 evtFindTaskById(s32 arg0);

extern void func_00101968(s32 arg0, s32 arg1);

extern void func_0023CED8(EvtUnit *unit, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);

extern void func_0023CED8(EvtUnit *, s32, s32, s32, s32, s32);

extern s32 scrGetWindow(void);

extern void func_001A4BB8(s32 arg0, void (*arg1)(void));

extern void func_0023CE98(EvtUnit *unit, s32 arg1, s32 arg2);

extern void func_0023CEA8(u32 arg0);

extern void func_0023D360(EvtUnit *unit);

extern f32 func_00240640(s32);

extern void func_00197F40(void *, f32);

/* Start a bounded vector transition; detach any previous secondary-world source. */
void evtBeginVectorTransition(EvtUnit *eventUnit, s128 *sourceVector, s32 stepCount) {
    s128 *destination = &eventUnit->vector;

    if ((u32)(stepCount - 1) < 100) {
        eventUnit->sourceUnit = NULL;
        eventUnit->mode = 3;
        PCP_COPY_VECTOR(destination, sourceVector);
        eventUnit->unkB4 = stepCount;
        eventUnit->unkB6 = 0;
        eventUnit->unk94 = 0;
        eventUnit->unkB2 = 0;
    }
}

/* Track a secondary-world unit and copy the vector in its subobject at +0x10. */
void evtAttachSecondaryWorldUnit(EvtUnit *eventUnit, s32 objectId, s32 stepCount) {
    EvtWorldVectorSource *sourceUnit;

    sourceUnit = func_00110C70(dds3GetWorldSecondaryObject(), objectId, 0x11);
    if (sourceUnit != NULL) {
        evtBeginVectorTransition(eventUnit, (s128 *)(sourceUnit->vectorData + 0x10), stepCount);
        eventUnit->sourceUnit = sourceUnit;
    }
}

/* Configure a mode-one vector transition, without retaining a world source. */
void func_0023D708(EvtUnit *eventUnit, s32 arg1, s128 *sourceVector, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    s128 *destination = &eventUnit->vector;

    eventUnit->unkB0 = arg1;
    eventUnit->mode = 1;
    eventUnit->unkAE = 0;
    eventUnit->sourceUnit = NULL;
    PCP_COPY_VECTOR(destination, sourceVector);
    eventUnit->unkB4 = arg4;
    eventUnit->unkB6 = arg5;
    eventUnit->unk94 = arg6;
    eventUnit->unkB2 = 0;
}

/* Configure the same transition from a secondary-world object's vector. */
void func_0023D740(EvtUnit *eventUnit, s32 arg1, s32 objectId, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    EvtWorldVectorSource *sourceUnit;

    sourceUnit = func_00110C70(dds3GetWorldSecondaryObject(), objectId, 0x11);
    if (sourceUnit != NULL) {
        func_0023D708(eventUnit, arg1, (s128 *)sourceUnit->vectorData, arg3, arg4, arg5, arg6, arg7);
        eventUnit->unkAE = 1;
        eventUnit->sourceUnit = sourceUnit;
    }
}

void func_0023D800(EvtUnit *work, s32 objectId, s32 arg2, s32 arg3, s32 mode, s32 dirFlag, s32 sideMode) {
    void *pathSource;
    s32 path;

    pathSource = func_00110C70(dds3GetWorldSecondaryObject(), objectId, 0x10);
    if (pathSource == NULL) {
        return;
    }
    if (work->unkA0 != 0) {
        dds3FreePathObject(work->unkA0);
    }
    path = func_00116FA0(pathSource);
    work->unkA0 = path;
    work->unkA4 = 40.0f / func_0023B3A0(path);
    if (dirFlag == 0) {
        evtScaleValueByMultiplier(path, 0.0f);
        func_001177D0(path, 0);
    } else {
        evtScaleValueByMultiplier(path, 1.0f);
        func_001177D0(path, 1);
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
    work->mode = 1;
    work->unkAE = 2;
    work->sourceUnit = pathSource;
    func_001171A0(path);
    VU0_STORE_VF($vf10, &work->vector);
    work->unkB4 = arg2;
    work->unkB6 = arg3;
    work->unk94 = 0;
    work->unkB2 = 0;
}

s32 func_0023DA48(EvtUnit *eventUnit, s32 value) {
    s32 result = 0;

    if (value != 0) {
        eventUnit->unk94 = value;
        eventUnit->unkB2 = 0;
        eventUnit->mode = 4;
        result = 1;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023DA70);

void func_0023E178(void) {
    s32 i;

    for (i = 0; i < 10; i++) {
        func_0025DFE8(&D_004536D8[i].vec[0], &D_004536D8[i].vec[4], &D_004536D8[i].vec[8], &D_004536D8[i].vec[12], &D_004536D8[i].vec[13]);
        D_004536D8[i].state = 0;
        D_004536D8[i].id = 0;
    }
}

void func_0023E220(s32 index, s32 state, s32 id, f32 *a, f32 *b, f32 *c) {
    if (index < 10) {
        D_004536D8[index].vec[0] = a[0];
        D_004536D8[index].state = state;
        D_004536D8[index].vec[1] = a[1];
        D_004536D8[index].vec[2] = a[2];
        D_004536D8[index].vec[3] = 0;
        D_004536D8[index].vec[4] = b[0];
        D_004536D8[index].vec[5] = b[1];
        D_004536D8[index].vec[6] = b[2];
        D_004536D8[index].vec[7] = b[3];
        D_004536D8[index].vec[8] = c[0];
        D_004536D8[index].vec[9] = c[1];
        D_004536D8[index].vec[10] = c[2];
        D_004536D8[index].vec[11] = 1.0f;
        if (state == 3) {
            D_004536D8[index].id = id;
        } else {
            D_004536D8[index].id = 0;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023E320);

/* Copy the second vector of the slot bound to `id` (else the first state-2 slot). */
s32 func_0023E350(s32 id, f32 *out) {
    s32 found = -1;
    s32 i;

    for (i = 0; i < 10; i++) {
        if (D_004536D8[i].state == 3 && D_004536D8[i].id == id) {
            found = i;
            break;
        }
    }
    if (found == -1) {
        for (i = 0; i < 10; i++) {
            if (D_004536D8[i].state == 2) {
                found = i;
                break;
            }
        }
        if (found == -1) {
            return 0;
        }
    }
    out[0] = D_004536D8[found].vec[4];
    out[1] = D_004536D8[found].vec[5];
    out[2] = D_004536D8[found].vec[6];
    return 1;
}

void func_0023E460(EvtUnit *unit) {
    f32 ends[4][4];
    f32 color[4];
    EvtSlotEnds desc = { ends, 0, 0 };
    s32 found = -1;
    s32 i;

    for (i = 0; i < 10; i++) {
        if (D_004536D8[i].state == 3 && D_004536D8[i].id == (s32)unit) {
            found = i;
            break;
        }
    }
    if (found == -1) {
        for (i = 0; i < 10; i++) {
            if (D_004536D8[i].state == 2) {
                found = i;
                break;
            }
        }
        if (found == -1) {
            return;
        }
    }
    ends[0][0] = D_004536D8[found].vec[0];
    ends[0][1] = D_004536D8[found].vec[1];
    ends[0][2] = D_004536D8[found].vec[2];
    ends[0][3] = 0;
    ends[1][0] = D_004536D8[found].vec[4];
    ends[1][1] = D_004536D8[found].vec[5];
    ends[1][2] = D_004536D8[found].vec[6];
    ends[1][3] = 0;
    color[0] = D_004536D8[found].vec[8];
    color[1] = D_004536D8[found].vec[9];
    color[2] = D_004536D8[found].vec[10];
    color[3] = 1.0f;
    for (i = 0; i < 3; i++) {
        if (color[i] > 1.0f) {
            color[i] = 1.0f;
        }
    }
    func_0033A7E8(unit->unk68, &desc, color);
    unit->value6C = unit->unk68;
}

/* Find the vector of the slot bound to `id`, else of the first slot in state 2. */
s32 func_0023E648(s32 id, f32 *outX, f32 *outY) {
    s32 i;

    for (i = 0; i < 10; i++) {
        if (D_004536D8[i].state == 3 && D_004536D8[i].id == id) {
            *outX = D_004536D8[i].vec[12];
            *outY = D_004536D8[i].vec[13];
            return 1;
        }
    }
    for (i = 0; i < 10; i++) {
        if (D_004536D8[i].state == 2) {
            *outX = D_004536D8[i].vec[12];
            *outY = D_004536D8[i].vec[13];
            return 1;
        }
    }
    return 0;
}

void *evtFindWorldObjectByIdAndKind(s32 kind, s32 id) {
    void *world;

    world = dds3GetWorldObject();
    func_00110C70(world, id, kind);
}

u32 evtGetWorldObjectId(void) {
    void *world;
    EvtUnit *object;
    s32 id;

    world = dds3GetWorldObject();
    object = func_00110C60(world);
    id = -1;
    if (object != NULL) {
        id = object->unk04;
    }
    func_0010D818(id);
    return 1;
}

u32 func_0023E758(void) {
    s32 param0;
    s32 param1;
    s32 value;

    param0 = scrReadIntParameter(0);
    param1 = scrReadIntParameter(1);
    value = func_0023A7A0(param0, param1);
    func_0010D818(value);
    return 1;
}

u32 func_0023E7A0(void) {
    s32 param0;
    s32 rid;
    s32 model;
    s32 ret;

    if (func_0010D8C8() == 0) {
        return 1;
    }
    param0 = scrReadIntParameter(0);
    rid = scrReadIntParameter(1);
    model = func_0025D230(param0, rid);
    if (model < 0) {
        func_0010AE38("MODEL_BE not fount RID = %d!\n", scrReadIntParameter(1));
        return 1;
    }
    param0 = scrReadIntParameter(0);
    rid = scrReadIntParameter(1);
    ret = evtCreateMotionSeTask(model, param0, rid);
    if (ret != 0) {
        func_00101968(evtFindTaskById(scrReadIntParameter(0)), ret);
    }
    return func_0010D818(model);
}

u32 func_0023E898(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = scrReadIntParameter(0);
    temp_v1 = scrReadIntParameter(1);
    func_0023A8C0(temp_v0, temp_v1);
    return 1;
}

u32 func_0023E8D8(void) {
    void *ctx;
    s32 id;
    void *unit;
    u32 ret = 1;

    ctx = dds3GetWorldObject();
    id = scrReadIntParameter(0);
    unit = func_00110C70(ctx, id, 5);
    if (unit == NULL) {
        return ret;
    }
    dds3SetObjectFlags(unit, 0x400);
    dds3ClearObjectFlags(unit, 0x200);
    return 1;
}

u32 func_0023E948(void) {
    void *ctx;
    s32 id;
    void *unit;
    u32 ret = 1;

    ctx = dds3GetWorldObject();
    id = scrReadIntParameter(0);
    unit = func_00110C70(ctx, id, 5);
    if (unit == NULL) {
        return ret;
    }
    dds3ClearObjectFlags(unit, 0x400);
    dds3SetObjectFlags(unit, 0x200);
    return 1;
}

u32 func_0023E9B8(void) {
    void *ctx;
    s32 id;
    void *unit;
    u32 ret = 1;

    ctx = dds3GetWorldObject();
    id = scrReadIntParameter(0);
    unit = func_00110C70(ctx, id, 5);
    if (unit == NULL) {
        return ret;
    }
    dds3ClearObjectFlags(unit, 0x400);
    dds3ClearObjectFlags(unit, 0x200);
    return 1;
}

u32 func_0023EA28(void) {
    void *ctx;
    s32 id;
    void *unit;
    u32 ret = 1;

    ctx = dds3GetWorldObject();
    id = scrReadIntParameter(0);
    unit = func_00110C70(ctx, id, 5);
    if (unit == NULL) {
        return ret;
    }
    func_00113660(unit, scrReadIntParameter(1));
    return 1;
}

u32 func_0023EA90(void) {
    void *ctx;
    s32 id;
    void *unit;
    u32 ret = 1;

    ctx = dds3GetWorldObject();
    id = scrReadIntParameter(0);
    unit = func_00110C70(ctx, id, 5);
    if (unit == NULL) {
        return ret;
    }
    func_001136A0(unit);
    return 1;
}

u32 func_0023EAE8(void) {
    s32 lod;
    void *world;
    EvtLodUnit *unit;
    EvtLodRoot *root;
    s32 max;

    lod = scrReadIntParameter(1);
    func_0035B6E0("call: MODEL_LOD_CHG(int,int)\n");
    world = dds3GetWorldObject();
    unit = func_00110C70(world, scrReadIntParameter(0), 5);
    if (unit == NULL) {
        func_0035B6E0("warning!! MODEL_LOD_CHG(int,int) unit pointer null\n");
        return 1;
    }
    if (unit->model->workbase == NULL) {
        func_0035B6E0("warning!! MODEL_LOD_CHG(int,int) workbase pointer null\n");
        return 1;
    }
    if (unit->model->workbase->mh == NULL) {
        func_0035B6E0("warning!! MODEL_LOD_CHG(int,int) mh pointer null\n");
        return 1;
    }
    root = unit->model->workbase->mh->root;
    if (root == NULL) {
        func_0035B6E0("warning!! MODEL_LOD_CHG(int,int) root pointer null\n");
        return 1;
    }
    max = sdfGetLodChunkValue(root);
    if (max < lod) {
        func_0035B6E0("warning!! MODEL_LOD_CHG(int,int) lodno over!! max=%d setval=%d\n", max, lod);
        return 1;
    }
    root->lodIndex = lod;
    func_0035B6E0("success: MODEL_LOD_CHG(int,int)\n");
    return 1;
}

u32 evtSetWorldUnitFirstVector(void) {
    f32 vector[4];
    void *world;
    s32 id;
    void *unit;

    memset(vector, 0, 0x10);
    world = dds3GetWorldObject();
    id = scrReadIntParameter(0);
    unit = func_00110C70(world, id, 5);
    if (unit == NULL) {
        return 1;
    }
    vector[0] = bfWaitReadArgFloat(1);
    vector[1] = bfWaitReadArgFloat(2);
    vector[2] = bfWaitReadArgFloat(3);
    effObjSetInnerFirstVec(unit, vector);
    return 1;
}

u32 func_0023EC80(void) {
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
    unit = func_00110C70(ctx, id, 5);
    if (unit == NULL) {
        return 1;
    }
    toRad = 0.017453293f;
    x = bfWaitReadArgFloat(1) * toRad;
    y = bfWaitReadArgFloat(2) * toRad;
    func_00340DC8(x, y, 0.0f);
    VU0_MOVE_VF(vf11, vf10);
    func_00340DC8(0.0f, 0.0f, bfWaitReadArgFloat(3) * toRad);
    effMiscQuatMultiplyVU();
    VU0_STORE_VF($vf10, v);
    effObjSetInnerSecondVec(unit, v);
    return 1;
}

u32 evtSetWorldUnitThirdVector(void) {
    f32 vector[4];
    void *world;
    s32 id;
    void *unit;

    memset(vector, 0, 0x10);
    vector[3] = 1.0f;
    world = dds3GetWorldObject();
    id = scrReadIntParameter(0);
    unit = func_00110C70(world, id, 5);
    if (unit == NULL) {
        return 1;
    }
    vector[0] = bfWaitReadArgFloat(1);
    vector[1] = bfWaitReadArgFloat(2);
    vector[2] = bfWaitReadArgFloat(3);
    effObjSetInnerThirdVec(unit, vector);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023EE08);

u32 evtOpSetUnitParams5(void) {
    EvtUnit *unit;

    unit = func_0023CC00(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 arg1 = scrReadIntParameter(1);
        s32 arg2 = scrReadIntParameter(2);
        s32 arg3 = scrReadIntParameter(3);
        s32 arg4 = scrReadIntParameter(4);
        func_0023CF70(unit, arg1, arg2, arg3, arg4);
    }
    return 1;
}

u32 evtOpSetUnitParams6(void) {
    EvtUnit *unit;

    unit = func_0023CC00(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 arg1 = scrReadIntParameter(1);
        s32 arg2 = scrReadIntParameter(2);
        s32 arg3 = scrReadIntParameter(3);
        s32 arg4 = scrReadIntParameter(4);
        s32 arg5 = scrReadIntParameter(5);
        func_0023CED8(unit, arg1, arg2, arg3, arg4, arg5);
    }
    return 1;
}

void func_0023EFF8(void) {
    func_0023CEA8(D_004371EC);
}

INCLUDE_RODATA(const s32, "game/code_0023D658", D_00421810);

void func_0023F010(s32 id, s32 motion) {
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
        func_0035B6E0("warning: call evtLipsExecFunction() but not find now reegisted unit same UnitUniqID\n");
        return;
    }
    if (motion >= mdlGetNodeRefHalf(model, 2)) {
        func_0035B6E0("warning: call evtLipsExecFunction() but over have motionno fpr user specified motion no.\n");
        return;
    }
    func_0023CED8(unit, 2, motion, 0, 5, 1);
    D_004371F0 = id;
    func_0035B6E0("<lips %d %d> \n", id, motion);
}

void func_0023F168(void) {
    void *unit = NULL;
    EvtLipsModel *model = NULL;
    EvtLipsNode *node;

    if (D_004371F0 == 0) {
        return;
    }
    for (node = ((EvtLipsWorld *)dds3GetWorldObject())->root->list->head; node != NULL; node = node->next) {
        model = node->link->mh->model;
        if (sdfGetUniqueChunkValue(model->chunk) == D_004371F0) {
            unit = node->link->unit;
            break;
        }
    }
    if (unit == NULL) {
        func_0035B6E0("warning: call evtLipsStopFunction() but not find now reegisted unit same UnitUniqID\n");
        return;
    }
    if (mdlGetNodeRefHalf(model, 2) == 0) {
        func_0035B6E0("warning: call evtLipsStopFunction() but over have motionno fpr user specified motion no.\n");
        return;
    }
    func_0023CED8(unit, 2, 0, 0, 3, 2);
    func_0035B6E0("<lips_stop> stopunitid = %d\n", D_004371F0);
    D_004371F0 = 0;
}

u32 evtOpBeginWindowCallback(void) {
    EvtUnit *unit;

    unit = func_0023CC00(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 arg1 = scrReadIntParameter(1);
        s32 arg2 = scrReadIntParameter(2);
        func_0023CE98(unit, arg1, arg2);
        D_004371EC = (u32)unit;
    }
    {
        s32 window = scrGetWindow();
        if (window < 0) {
            return 1;
        }
        func_001A4BB8(window, func_0023EFF8);
    }
    return 1;
}

u32 func_0023F308(void) {
    EvtUnit *unit;

    unit = func_0023CC00(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 arg1 = scrReadIntParameter(1);
        s32 arg2 = scrReadIntParameter(2);
        func_0023CE98(unit, arg1, arg2);
        func_0023CEA8((u32)unit);
        D_004371EC = (u32)unit;
    }
    {
        s32 window = scrGetWindow();
        if (window < 0) {
            return 1;
        }
        func_001A4BB8(window, func_0023EFF8);
    }
    return 1;
}

u32 func_0023F3A8(void) {
    s32 id;
    EvtUnit *unit;
    u32 ret = 1;

    id = scrReadIntParameter(0);
    unit = func_0023CC00(id);
    if (unit == NULL) {
        return ret;
    }
    return func_0023CE30(unit) != 0;
}

u32 func_0023F3E8(void) {
    s32 id;
    EvtUnit *unit;
    s32 off;
    u32 ret = 1;

    id = scrReadIntParameter(0);
    unit = func_0023CC00(id);
    if (unit == NULL) {
        return ret;
    }
    off = scrReadIntParameter(1);
    if (((((u8 *)(off + (s32)unit))[0xE0] & 1) & 0xFF) == 0) {
        return ret;
    }
    return mdlCheckNodeByte30(unit->modelNode, scrReadIntParameter(1)) != 0;
}

u32 func_0023F460(void) {
    void *ctx;
    s32 id;
    void *unit;
    u32 ret = 1;

    ctx = dds3GetWorldObject();
    id = scrReadIntParameter(0);
    unit = func_00110C70(ctx, id, 5);
    if (unit == NULL) {
        return ret;
    }
    func_00110B50(unit);
    return 1;
}

u32 func_0023F4B8(void) {
    EvtUnit *unit;

    unit = func_0023CC00(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    unit->unkB2 = 0;
    {
        s32 arg1 = scrReadIntParameter(2);
        s32 arg2 = scrReadIntParameter(1);
        s32 arg3 = scrReadIntParameter(3);
        s32 arg4 = scrReadIntParameter(4);
        func_0023D740(unit, arg1, arg2, -1, arg3, arg4, 0, 0);
    }
    if (scrReadIntParameter(2) == 1) {
        func_0023D360(unit);
    }
    return 1;
}

u32 func_0023F580(void) {
    EvtUnit *unit;

    unit = func_0023CC00(scrReadIntParameter(0));
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
        func_0023D800(unit, arg1, arg2, arg3, arg4, arg5, arg6);
    }
    return 1;
}

u32 func_0023F650(void) {
    EvtUnit *unit;

    unit = func_0023CC00(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 index = scrReadIntParameter(1);
        s32 value = scrReadIntParameter(2);
        unit->unkF0[index] = value;
        unit->unk108[index] = 0;
        unit->unk120[index] = 0;
        unit->unk138[index] = 1.0f;
    }
    return 1;
}

u32 evtCommandSetUnitValue(void) {
    s32 id;
    EvtUnit *unit;

    id = scrReadIntParameter(0);
    unit = func_0023CC00(id);
    if (unit == NULL) {
        return 1;
    }
    unit->valueBC = scrReadIntParameter(1);
    return 1;
}

u32 func_0023F730(void) {
    s32 id;
    EvtUnit *unit;

    id = scrReadIntParameter(0);
    unit = func_0023CC00(id);
    if (unit == NULL) {
        return 1;
    }
    unit->unkBE = scrReadIntParameter(1);
    unit->unkC0 = scrReadIntParameter(2);
    return 1;
}

u32 func_0023F788(void) {
    s32 id;
    EvtUnit *unit;

    id = scrReadIntParameter(0);
    unit = func_0023CC00(id);
    if (unit == NULL) {
        return 1;
    }
    func_0023CF70(unit, scrReadIntParameter(1), 5, 7, 1);
    func_0035B6E0(D_004219F0);
    return 1;
}

u32 func_0023F7F8(void) {
    s32 id;
    EvtUnit *unit;
    s32 param1;
    s32 param2;

    id = scrReadIntParameter(0);
    unit = func_0023CC00(id);
    if (unit == NULL) {
        return 1;
    }
    unit->unkB2 = 0;
    param1 = scrReadIntParameter(1);
    param2 = scrReadIntParameter(2);
    evtAttachSecondaryWorldUnit(unit, param1, param2);
    return 1;
}

f32 evtGetShortestAngleDelta(f32 a, f32 b) {
    f32 diff;

    if (a < 0.0f || b < 0.0f) {
        a += 360.0f;
        b += 360.0f;
    }
    a = (s32)a % 360;
    b = (s32)b % 360;
    diff = a - b;
    if (diff > 180.0f || diff < -180.0f) {
        if (a < b) {
            a += 360.0f;
        } else {
            b += 360.0f;
        }
    }
    return b - a;
}

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023F938);

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023FBA8);

u32 evtUnitClearFlagBit(void) {
    s32 id;
    EvtUnit *unit;

    id = scrReadIntParameter(0);
    unit = func_0023CC00(id);
    if (unit != NULL) {
        *unit->modelNode &= ~1;
    }
    return 1;
}

u32 evtUnitSetFlagBit(void) {
    s32 id;
    EvtUnit *unit;

    id = scrReadIntParameter(0);
    unit = func_0023CC00(id);
    if (unit != NULL) {
        *unit->modelNode |= 1;
    }
    return 1;
}

u32 func_0023FDC0(void) {
    s32 id;
    EvtUnit *unit;

    id = scrReadIntParameter(0);
    unit = func_0023CC00(id);
    evtSetUnitStatusFlags(unit);
    return 1;
}

u32 func_0023FDF0(void) {
    s32 id;
    EvtUnit *unit;
    s32 param1;

    id = scrReadIntParameter(0);
    unit = func_0023CC00(id);
    param1 = scrReadIntParameter(1);
    func_0023CB68(unit, param1);
    return 1;
}

u8 evtUnitHasNoStatusFlags(void) {
    s32 id;
    EvtUnit *unit;
    u8 active;

    id = scrReadIntParameter(0);
    unit = func_0023CC00(id);
    active = evtTestUnitStatusFlags(unit);
    return active == 0;
}

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023FE68);

INCLUDE_RODATA(const s32, "game/code_0023D658", D_004219F0);

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023FF90);

INCLUDE_ASM(const s32, "game/code_0023D658", func_00240068);

INCLUDE_ASM(const s32, "game/code_0023D658", func_00240128);

u32 func_002401C0(void) {
    s32 id;
    EvtUnit *unit;
    void *target;

    id = scrReadIntParameter(0);
    unit = func_0023CC00(id);
    target = evtFindWorldObjectByIdAndKind(9, scrReadIntParameter(2));
    if (target == NULL) {
        return 1;
    }
    evtSetUnitValueTransition(unit, target, scrReadIntParameter(1));
    return 1;
}

u32 func_00240238(void) {
    s32 id;
    EvtUnit *unit;
    s32 param1;

    id = scrReadIntParameter(0);
    unit = func_0023CC00(id);
    param1 = scrReadIntParameter(1);
    evtEndUnitValueTransition(unit, param1);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0023D658", func_00240280);

u32 func_00240368(void) {
    u8 buf1[16];
    u8 buf2[16];
    s32 param0;
    EvtUnit *unit;

    memset(buf1, 0, 0x10);
    memset(buf2, 0, 0x10);
    param0 = scrReadStringParameter(0);
    unit = func_00115208(param0, buf1, buf2);
    if (unit == NULL) {
        func_0035B6E0(D_00421A90, 1);
        func_0035B6E0(D_00421AC0, scrReadStringParameter(0));
        func_0010D818(0);
    } else {
        effObjSetFlags(unit, 1);
        func_0010D818(unit->unk04);
    }
    return 1;
}

u32 func_00240420(void) {
    f32 buf1[4];
    f32 buf2[4];
    s32 param0;
    EvtUnit *unit;

    memset(buf1, 0, 0x10);
    memset(buf2, 0, 0x10);
    buf2[3] = 1.0f;
    param0 = scrReadStringParameter(0);
    unit = func_00115518(param0, buf1, buf2);
    if (unit == NULL) {
        func_0035B6E0(D_00421A90, 1);
        func_0035B6E0(D_00421AC0, scrReadStringParameter(0));
        func_0010D818(0);
    } else {
        effObjSetFlags(unit, 1);
        func_0010D818(unit->unk04);
    }
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_0023D658", D_00421A78);

INCLUDE_RODATA(const s32, "game/code_0023D658", D_00421A90);

INCLUDE_RODATA(const s32, "game/code_0023D658", D_00421AC0);

INCLUDE_ASM(const s32, "game/code_0023D658", func_002404E0);

u32 func_002405B8(void) {
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
            func_0010AE38(D_00421AF8, param1);
            return 1;
        } else {
            func_00115580(unit, param1);
        }
    }
    return 1;
}

f32 func_00240640(s32 index) {
    f32 scale = (D_00435DF0[index].unk18 + D_00435DF0[index].unk1C * 0.5f) * 0.5f * D_00435DF0[index].unk10 * (1.0f / 70.0f);

    if (scale > 2.0f) {
        scale = 2.0f;
    } else if (scale < 0.8f) {
        scale = 0.8f;
    }
    return scale;
}

u32 func_002406C0(void) {
    void *unit;

    unit = evtFindWorldObjectByIdAndKind(7, scrReadIntParameter(0));
    if (unit != NULL) {
        s32 mode = scrReadIntParameter(1);
        s32 index;
        if ((u32)mode >= 3) {
            func_0010AE38(D_00421AF8, mode);
            return 1;
        }
        func_00115580(unit, mode);
        index = scrReadIntParameter(2);
        if (index >= 0) {
            f32 value = func_00240640(index);
            void *target = *(void **)(*(u8 **)((u8 *)unit + 0x18) + 0x2C);
            if (target != NULL) {
                func_00197F40(target, value);
            }
        }
    }
    return 1;
}

u32 func_00240770(void) {
    s32 param0;
    EvtUnit *unit;

    param0 = scrReadStringParameter(0);
    unit = func_00115AC0(1, param0);
    if (unit == NULL) {
        func_0035B6E0(D_00421B20, 1);
        func_0035B6E0(D_00421AC0, scrReadStringParameter(0));
        func_0010D818(0);
    } else {
        effObjSetFlags(unit, 1);
        func_0010D818(unit->unk04);
    }
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_0023D658", D_00421AF8);

INCLUDE_RODATA(const s32, "game/code_0023D658", D_00421B20);

INCLUDE_ASM(const s32, "game/code_0023D658", func_00240800);

u32 func_002408A8(void) {
    s32 param0;
    EvtUnit *unit;

    param0 = scrReadStringParameter(0);
    unit = func_00115AC0(2, param0);
    if (unit == NULL) {
        func_0035B6E0(D_00421B70, 1);
        func_0035B6E0(D_00421AC0, scrReadStringParameter(0));
        func_0010D818(0);
    } else {
        effObjSetFlags(unit, 1);
        func_0010D818(unit->unk04);
    }
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_0023D658", D_00421B70);

INCLUDE_ASM(const s32, "game/code_0023D658", func_00240938);

u32 func_002409E0(void) {
    s32 id;
    void *obj;

    id = scrReadIntParameter(0);
    obj = evtFindWorldObjectByIdAndKind(7, id);
    if (obj != NULL) {
        func_00115BD8(obj);
    }
    return 1;
}

u32 func_00240A20(void) {
    s32 id;
    void *obj;

    id = scrReadIntParameter(0);
    obj = evtFindWorldObjectByIdAndKind(7, id);
    if (obj != NULL) {
        func_00115BD8(obj);
    }
    return 1;
}

u32 func_00240A60(void) {
    s32 id;
    void *obj;

    id = scrReadIntParameter(0);
    obj = evtFindWorldObjectByIdAndKind(7, id);
    if (obj != NULL) {
        func_00110B50(obj);
    }
    return 1;
}

u32 func_00240AA0(void) {
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

u32 func_00240B68(void) {
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
        func_00340DC8(x, y, 0.0f);
        VU0_MOVE_VF(vf11, vf10);
        func_00340DC8(0.0f, 0.0f, bfWaitReadArgFloat(3) * toRad);
        effMiscQuatMultiplyVU();
        VU0_STORE_VF($vf10, v);
        effObjSetInnerSecondVec(obj, v);
    }
    return 1;
}

u32 func_00240C48(void) {
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

INCLUDE_SDATA(const s32, "game/code_0023D658", D_004371EC);

INCLUDE_SDATA(const s32, "game/code_0023D658", D_004371F0);

