#include "common.h"
#include "eff.h"
#include "pcp_vu0.h"

/* The channel copies three coordinates from each four-float source row,
 * then copies the four trailing values to a second block. */
typedef struct EffFloatRows {
    f32 primary[16];
    u8 pad40[0x18];
    f32 secondary[4];
} EffFloatRows;

/* Small channel object (0x18 bytes, created by effCreateChannel): float block
 * plus the channel-A cursor (count at +0x4, index at +0xC). */
typedef struct EffChan {
    void *unk0; /* 0x0: mem handle */
    u32 recordCount; /* 0x4: number of keyframe records */
    EffFloatRows *rows; /* 0x8: interpolation rows owned by the channel */
    u32 cursorIndex; /* 0xC: channel-A record index */
    f32 cursorPosition; /* 0x10: channel-A interpolation position */
    f32 cursorStep; /* 0x14: channel-A position increment */
} EffChan;

/* 0x38-byte keyframe record addressed by effMathGetSlotAt. */
typedef struct EffRec38 {
    u8 unk0[0x30]; /* 0x0 */
    f32 scaledRandomValue; /* 0x30 */
    f32 randomScale;       /* 0x34 */
} EffRec38;

/* Interpolated vertex (x, y, z, w) written by func_00192ED0. */
typedef struct EffVert {
    f32 unk0; /* 0x0 */
    f32 unk4; /* 0x4 */
    f32 unk8; /* 0x8 */
    f32 unkC; /* 0xC */
} EffVert;

/* Emitter handle for effFillRandRecords: target primitive at +0x8. */
typedef struct EffEmit {
    u8 unk0[8];    /* 0x0 */
    EffPrim *primitive; /* 0x8: target primitive */
} EffEmit;

/* Word at frFontResourceList+0x18 (list header defined in game/code_00193C08). */
extern s32 D_003D68D8[];
/* List header defined in game/code_00193C08 (unsized: keeps absolute access). */
extern u8 frFontResourceList[];
extern void func_002D0918(void *arg0);
extern void *func_002D03F8(s32 arg0);
extern void *sdfResourceRetainAddress(void *arg0);
extern void func_00192ED0(EffVert *arg0, EffPrim *arg1, s32 arg2, f32 arg3);
extern void func_001937E0(EffVert *arg0, EffChan *arg1, s32 arg2, f32 arg3);
extern u32 effMiscRand(void *state);
extern u8 D_0034DF38[];
extern s32 effMathGetSlotAt(s32 *arg0, s32 arg1);
extern void func_001926A8(EffPrim *arg0, u32 arg1);
extern void func_001931E0(void *arg0, s32 arg1, u32 arg2);
extern void func_001934E8(EffPrim *arg0, void *arg1);
extern void func_001935B8(EffPrim *arg0, void *arg1);
extern void effMathReleaseWorkResource(void *work);
extern void effDispatchParameterDataAndFreeWork(void *handle);

/* Header block copied into every channel work (0x168 bytes): the random record count and modulus live inside it. */
typedef struct EffChanHead {
    u8 pad00[0x44];
    u32 count;      /* 0x44: number of random records */
    u8 pad48[4];
    s32 spread;     /* 0x4C: modulus of the start delay */
    u8 pad50[0x118];
} EffChanHead; /* 0x168 */

typedef struct EffChanRecord {
    s32 delay;      /* 0x00 */
    void *param;    /* 0x04 */
} EffChanRecord; /* 0x8 */

typedef struct EffChanSourceOwner {
    u8 pad00[4];
    void *param;    /* 0x04 */
} EffChanSourceOwner;

typedef struct EffChanSource {
    EffChanHead head;
    EffChanSourceOwner *owner; /* 0x168 */
} EffChanSource;

typedef struct EffChanWork {
    EffChanHead head;
    EffChanRecord *records; /* 0x168 */
    s32 *slots;             /* 0x16C */
    void *buffer;           /* 0x170 */
} EffChanWork;

extern void *effAllocSlotArray(u32 count);
extern void *effParamWorkDuplicate(void *param);

/* Create a channel work: clone the header, allocate the slot array, then give every record a duplicated parameter and a random negative start delay. */
EffChanWork *effChanWorkCreate(EffChanSource *src) {
    u32 count = src->head.count;
    void *handle = func_002D03F8(count * sizeof(EffChanRecord) + sizeof(EffChanWork));
    EffChanWork *work = sdfResourceRetainAddress(handle);
    EffChanRecord *record = (EffChanRecord *)(work + 1);
    void *param;
    s32 spread;
    u32 i;

    work->head = src->head;
    work->buffer = handle;
    work->records = record;
    if (work->head.spread <= 0) {
        work->head.spread = 1;
    }
    work->slots = effAllocSlotArray(count);
    if (count != 0) {
        param = src->owner->param;
        spread = work->head.spread;
        for (i = 0; i < count; i++) {
            record->param = effParamWorkDuplicate(param);
            record->delay = -(effMiscRand(D_0034DF38) % spread);
            record++;
        }
    }
    return work;
}

void effDestroyChannelWork(EffChanWork *work) {
    EffChanRecord *record;
    u32 i = 0;
    u32 count;

    effMathReleaseWorkResource(work->slots);
    count = work->head.count;
    record = work->records;
    if (count != 0) {
        do {
            effDispatchParameterDataAndFreeWork(record->param);
            record++;
            i++;
        } while (i < count);
    }
    func_002D0918(work->buffer);
}

INCLUDE_ASM(const s32, "game/code_00192488", func_001926A8);

INCLUDE_ASM(const s32, "game/code_00192488", func_001929A0);

/* Copy four rows of three coordinates and their separate scalar values. */
void effCopyVertRows(EffChan *channel, f32 *source) {
    f32 *row;
    u32 index;
    f32 *secondary;
    f32 *primary;

    row = source;
    index = 0;
    source += 16;
    secondary = channel->rows->secondary;
    primary = channel->rows->primary;
    do {
        index++;
        primary[0] = row[0];
        primary[1] = row[1];
        primary[2] = row[2];
        row += 4;
        primary += 4;
        *secondary = *source++;
        secondary++;
    } while (index < 4);
}

void effFillRandRecords(EffEmit *emitter) {
    EffPrim *primitive = emitter->primitive;
    u32 modulus = primitive->randomModulus;
    u32 count = primitive->randomCount;
    EffCntRec *record = primitive->counterRecords;
    u32 index = 0;
    EffRec38 *keyframe;
    s32 randomIndex;
    f32 scale;

    if (count == 0) {
        return;
    }
    do {
        func_001926A8(primitive, index);
        record->randomIndex = effMiscRand(&D_0034DF38) % modulus;
        keyframe = (EffRec38 *)effMathGetSlotAt(primitive->slotLookup, index);
        index++;
        randomIndex = record->randomIndex;
        scale = keyframe->randomScale;
        record->randomIndex = randomIndex + 1;
        record++;
        keyframe->scaledRandomValue = scale * (f32)randomIndex;
    } while (index < count);
}

INCLUDE_ASM(const s32, "game/code_00192488", func_00192CC8);

void effFreeBuffers(EffPrim *primitive) {
    if (primitive != NULL) {
        if (primitive->unk14 != NULL) {
            func_002D0918(primitive->secondaryResource);
        }
        func_002D0918(primitive->primaryResource);
    }
}

/* Interpolate the current primitive record, then advance its wrapping cursor. */
s32 effAdvancePrimCursor(void *vertex, EffPrim *primitive) {
    s32 continuing = 1;
    f32 position = primitive->cursorPosition;
    u32 index = primitive->cursorIndex;

    func_00192ED0(vertex, primitive, index, position);
    position += primitive->cursorStep;
    if (position > 1.0f) {
        position -= 1.0f;
        index += 1;
    }
    if (index >= primitive->recordCount - 1) {
        position = 0.0f;
        index = 0;
        continuing = 0;
    }
    primitive->cursorIndex = index;
    primitive->cursorPosition = position;
    return continuing;
}

INCLUDE_ASM(const s32, "game/code_00192488", func_00192ED0);

/* Evaluate cubic coefficients or adjacent linear keys into the VU input vector. */
void func_00193000(EffPrim *primitive, s32 index, f32 t)
{
    f32 result[4];
    f32 *a;
    f32 *b;
    f32 *c;
    f32 *d;

    if (primitive->unkC == 0) {
        a = (f32 *)primitive->unk14 + index * 3;
        b = (f32 *)primitive->unk18 + index * 3;
        c = (f32 *)primitive->unk1C + index * 3;
        d = (f32 *)primitive->unk10 + index * 3;
        result[0] = ((a[0] * t + b[0]) * t + c[0]) * t + d[0];
        result[1] = ((a[1] * t + b[1]) * t + c[1]) * t + d[1];
        result[2] = ((a[2] * t + b[2]) * t + c[2]) * t + d[2];
        result[3] = 1.0f;
    } else {
        a = (f32 *)primitive->unk10 + index * 3;
        b = a + 3;
        result[0] = a[0] + (b[0] - a[0]) * t;
        result[1] = a[1] + (b[1] - a[1]) * t;
        result[2] = a[2] + (b[2] - a[2]) * t;
    }
    VU0_LOAD_VF_FROM(vf10, *(u128 *)result);
}

void func_00193130(EffPrim *primitive) {
    primitive->cursorIndex = 0;
    primitive->cursorPosition = 0;
}

void func_00193140(EffPrim *primitive, f32 step) {
    primitive->cursorStep = step;
}

/* Build a temporary record array and dispatch it through the selected path. */
void effBuildAndDispatch(EffPrim *primitive, s32 variant) {
    void *allocation = func_002D03F8(primitive->recordCount * 12);
    void *records = sdfResourceRetainAddress(allocation);

    func_001931E0(records, primitive->unk10, primitive->recordCount);
    if (variant == 0) {
        func_001934E8(primitive, records);
    } else {
        func_001935B8(primitive, records);
    }
    func_002D0918(allocation);
}


INCLUDE_ASM(const s32, "game/code_00192488", func_001931E0);

INCLUDE_ASM(const s32, "game/code_00192488", func_00193368);

INCLUDE_ASM(const s32, "game/code_00192488", func_001934E8);

INCLUDE_ASM(const s32, "game/code_00192488", func_001935B8);

/* Create a channel only when there are enough records for interpolation. */
void *effCreateChannel(void *rows, u32 count) {
    void *channel = NULL;
    void *allocation;
    EffChan *cursor;

    if (count < 4) {
        return channel;
    }
    allocation = func_002D03F8(0x18);
    channel = sdfResourceRetainAddress(allocation);
    cursor = channel;
    cursor->unk0 = allocation;
    cursor->cursorStep = 0.05f;
    cursor->recordCount = count;
    cursor->rows = rows;
    cursor->cursorPosition = 0;
    cursor->cursorIndex = 0;
    return channel;
}

INCLUDE_ASM(const s32, "game/code_00192488", effReleaseInterpolationChannel);

/* Interpolate the channel; advance three records when its position wraps. */
s32 effAdvanceChanCursor(void *vertex, EffChan *channel) {
    s32 continuing = 1;
    f32 position = channel->cursorPosition;
    u32 index = channel->cursorIndex;

    func_001937E0(vertex, channel, index, position);
    position += channel->cursorStep;
    if (position > 1.0f) {
        position -= 1.0f;
        index += 3;
    }
    if (index >= channel->recordCount - 1) {
        position = 0.0f;
        index = 0;
        continuing = 0;
    }
    channel->cursorIndex = index;
    channel->cursorPosition = position;
    return continuing;
}

void func_001937E0(EffVert *out, EffChan *channel, s32 index, f32 t) {
    f32 weights[4];
    f32 inverse = 1.0f - t;
    f32 *p0 = channel->rows->primary + index * 3;
    f32 *p1 = p0 + 3;
    f32 *p2 = p0 + 6;
    f32 *p3 = p0 + 9;

    weights[0] = inverse * inverse * inverse;
    weights[1] = t * (inverse * inverse) * 3.0f;
    weights[2] = t * t * inverse * 3.0f;
    weights[3] = t * t * t;
    out->unk0 = p0[0] * weights[0] + p1[0] * weights[1] + p2[0] * weights[2] + p3[0] * weights[3];
    out->unk4 = p0[1] * weights[0] + p1[1] * weights[1] + p2[1] * weights[2] + p3[1] * weights[3];
    out->unk8 = p0[2] * weights[0] + p1[2] * weights[1] + p2[2] * weights[2] + p3[2] * weights[3];
    out->unkC = 1.0f;
}

void effClearChanCursor(EffChan *chan) {
    chan->cursorIndex = 0;
    chan->cursorPosition = 0;
}

void effSetChanStep(EffChan *chan, f32 step) {
    chan->cursorStep = step;
}

void *effGetFontListHead(void) {
    return frFontResourceList;
}

s32 effGetFontListCount(void) {
    return D_003D68D8[0];
}

INCLUDE_ASM(const s32, "game/code_00192488", func_00193920);

INCLUDE_SDATA(const s32, "game/code_00192488", D_003BB150);

INCLUDE_SDATA(const s32, "game/code_00192488", D_003BB15C);

