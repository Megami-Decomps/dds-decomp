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
    f32 controlPoints[4][3];
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

typedef struct EffChanWork EffChanWork;

/* Emitter handle for effFillRandRecords: target primitive at +0x8. */
typedef struct EffEmit {
    u8 unk0[8];    /* 0x0 */
    EffChanWork *primitive; /* 0x8: control-point work */
} EffEmit;

/* Word at frFontResourceList+0x18 (list header defined in game/code_00193C08). */
extern s32 D_003D68D8[];
/* List header defined in game/code_00193C08 (unsized: keeps absolute access). */
extern u8 frFontResourceList[];
extern void sdfReleaseResourceAllocation(void *arg0);
extern void *sdfAllocGeneralBlock(s32 arg0);
extern void *sdfResourceRetainAddress(void *arg0);
extern void func_00192ED0(EffVert *arg0, EffPrim *arg1, s32 arg2, f32 arg3);
extern void effSampleChannelBezier(EffVert *arg0, EffChan *arg1, s32 arg2, f32 arg3);
extern u32 effMiscRand(void *state);
extern u8 D_0034DF38[];
extern s32 effMathGetSlotAt(void *slots, s32 index);
extern void effJitterChannelControlPoints(EffChanWork *arg0, u32 arg1);
extern void func_001931E0(void *arg0, s32 arg1, u32 arg2);
extern void func_001934E8(EffPrim *arg0, void *arg1);
extern void func_001935B8(EffPrim *arg0, void *arg1);
extern void effMathReleaseWorkResource(void *work);
extern void effDispatchParameterDataAndFreeWork(void *handle);

/* Header block copied into every channel work (0x168 bytes): the random record count and modulus live inside it. */
typedef struct EffChanHead {
    f32 controlPoints[4][4];
    u8 enabled;    /* 0x40: keep completed channels cycling */
    u8 pad41[3];
    u32 count;      /* 0x44: number of random records */
    s32 steps;
    s32 spread;     /* 0x4C: modulus of the start delay */
    s32 fadeIn;     /* 0x50: frames to reach full opacity */
    s32 fadeOut;    /* 0x54: frames to fade out */
    f32 jitter[4];
    u8 pad68[0x100];
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

struct EffChanWork {
    EffChanHead head;
    EffChanRecord *records; /* 0x168 */
    s32 *slots;             /* 0x16C */
    void *buffer;           /* 0x170 */
};

extern void *effAllocSlotArray(u32 count);
extern void *effParamWorkDuplicate(void *param);
extern s32 effMathStepBezierSlot(void *slots, s32 index, f32 *out);
extern void effParamWorkCallback0(void *param, void *value);
extern void effParamWorkInvokeCallback(void *param);

/* Create a channel work: clone the header, allocate the slot array, then give every record a duplicated parameter and a random negative start delay. */
EffChanWork *effChanWorkCreate(EffChanSource *src) {
    u32 count = src->head.count;
    void *handle = sdfAllocGeneralBlock(count * sizeof(EffChanRecord) + sizeof(EffChanWork));
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
    sdfReleaseResourceAllocation(work->buffer);
}

extern f32 sdfViewTargetVector[4];
extern f32 sdfViewEyeVector[4];
extern f32 effMiscRandUnitFloat(void *);
extern void effParamWorkCallback3(void *param, u32 value);

/* Jitter control points perpendicular to the path and camera viewing direction. */
void effJitterChannelControlPoints(EffChanWork *work, u32 index) {
    f32 scale[4];
    f32 lastNormal[4];
    f32 viewDirection[4];
    f32 point[4];
    EffChanRecord *record = work->records + index;
    EffRec38 *slot;
    f32 step;
    f32 random;

    VU0_LOAD_VF(vf10, sdfViewTargetVector);
    VU0_LOAD_VF(vf11, sdfViewEyeVector);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, viewDirection);
    step = 1.0f / (f32)work->head.steps;
    slot = (EffRec38 *)effMathGetSlotAt(work->slots, index);
    slot->scaledRandomValue = 0;
    slot->randomScale = step;

    random = effMiscRandUnitFloat(D_0034DF38) - 0.5f;
    scale[0] = work->head.jitter[0] * (random + random);
    scale[1] = scale[0];
    scale[2] = scale[0];
    VU0_LOAD_VF(vf10, work->head.controlPoints[0]);
    VU0_LOAD_VF(vf11, work->head.controlPoints[1]);
    VU0_MOVE_VF(vf12, vf10);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, viewDirection);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_LOAD_VF(vf11, scale);
    VU0_MUL(vf10, vf10, vf11);
    VU0_ADD(vf10, vf10, vf12);
    VU0_STORE_VF(vf10, point);
    slot->controlPoints[0][0] = point[0];
    slot->controlPoints[0][1] = point[1];
    slot->controlPoints[0][2] = point[2];

    random = effMiscRandUnitFloat(D_0034DF38) - 0.5f;
    scale[0] = work->head.jitter[1] * (random + random);
    scale[1] = scale[0];
    scale[2] = scale[0];
    VU0_LOAD_VF(vf10, work->head.controlPoints[1]);
    VU0_LOAD_VF(vf11, work->head.controlPoints[2]);
    VU0_MOVE_VF(vf12, vf10);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, viewDirection);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_LOAD_VF(vf11, scale);
    VU0_MUL(vf10, vf10, vf11);
    VU0_ADD(vf10, vf10, vf12);
    VU0_STORE_VF(vf10, point);
    slot->controlPoints[1][0] = point[0];
    slot->controlPoints[1][1] = point[1];
    slot->controlPoints[1][2] = point[2];

    random = effMiscRandUnitFloat(D_0034DF38) - 0.5f;
    scale[0] = work->head.jitter[2] * (random + random);
    scale[1] = scale[0];
    scale[2] = scale[0];
    VU0_LOAD_VF(vf10, work->head.controlPoints[2]);
    VU0_LOAD_VF(vf11, work->head.controlPoints[3]);
    VU0_MOVE_VF(vf12, vf10);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, viewDirection);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, lastNormal);
    VU0_LOAD_VF(vf11, scale);
    VU0_MUL(vf10, vf10, vf11);
    VU0_ADD(vf10, vf10, vf12);
    VU0_STORE_VF(vf10, point);
    slot->controlPoints[2][0] = point[0];
    slot->controlPoints[2][1] = point[1];
    slot->controlPoints[2][2] = point[2];

    random = effMiscRandUnitFloat(D_0034DF38) - 0.5f;
    scale[0] = work->head.jitter[3] * (random + random);
    scale[1] = scale[0];
    scale[2] = scale[0];
    VU0_LOAD_VF(vf10, lastNormal);
    VU0_LOAD_VF(vf11, scale);
    VU0_MUL(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, work->head.controlPoints[3]);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, point);
    slot->controlPoints[3][0] = point[0];
    slot->controlPoints[3][1] = point[1];
    slot->controlPoints[3][2] = point[2];
    effParamWorkCallback3(record->param, 0);
}

void func_001929A0(EffChanWork *work) {
    EffChanRecord *record = work->records;
    void *slots = work->slots;
    u32 count = work->head.count;
    u32 index = 0;
    u8 enabled = work->head.enabled;
    s32 steps = work->head.steps;
    s32 modulus = work->head.spread;
    s32 fadeIn = work->head.fadeIn;
    s32 fadeOut = work->head.fadeOut;
    f32 xyz[3];

    (void)effMathGetSlotAt(slots, 0);
    if (count == 0) {
        return;
    }
    do {
        s32 delay = record->delay;

        if (delay == 0) {
            effJitterChannelControlPoints(work, index);
        }
        if (delay > 0 && delay <= steps) {
            f32 opacity;
            s32 fadePhase;
            u32 alpha;
            u32 color;

            effMathStepBezierSlot(slots, index, xyz);
            fadePhase = fadeIn > delay;
            if (fadePhase) {
                opacity = (f32)delay / (f32)fadeIn;
            } else {
                fadePhase = steps - delay;
                if (fadePhase <= fadeOut) {
                    opacity = (f32)fadePhase / (f32)fadeOut;
                } else {
                    opacity = 1.0f;
                }
            }
            alpha = (u32)(opacity * 127.0f);
            color = (alpha << 24) | 0x00808080;
            effParamWorkCallback3(record->param, color);
            effParamWorkCallback0(record->param, xyz);
            effParamWorkInvokeCallback(record->param);
        }
        if (delay < steps) {
            record->delay++;
        } else if (enabled != 0) {
            record->delay = -(effMiscRand(D_0034DF38) % modulus);
        } else {
            record->delay++;
        }
        record++;
        index++;
    } while (index < count);
}

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
    EffChanWork *primitive = emitter->primitive;
    u32 modulus = primitive->head.steps;
    u32 count = primitive->head.count;
    EffChanRecord *record = primitive->records;
    u32 index = 0;
    EffRec38 *keyframe;
    s32 randomIndex;
    f32 scale;

    if (count == 0) {
        return;
    }
    do {
        effJitterChannelControlPoints(primitive, index);
        record->delay = effMiscRand(&D_0034DF38) % modulus;
        keyframe = (EffRec38 *)effMathGetSlotAt(primitive->slots, index);
        index++;
        randomIndex = record->delay;
        scale = keyframe->randomScale;
        record->delay = randomIndex + 1;
        record++;
        keyframe->scaledRandomValue = scale * (f32)randomIndex;
    } while (index < count);
}

INCLUDE_ASM(const s32, "game/code_00192488", func_00192CC8);

void effFreeBuffers(EffPrim *primitive) {
    if (primitive != NULL) {
        if (primitive->unk14 != NULL) {
            sdfReleaseResourceAllocation(primitive->secondaryResource);
        }
        sdfReleaseResourceAllocation(primitive->primaryResource);
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
void effSamplePrimitiveCurve(EffPrim *primitive, s32 index, f32 t)
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

void effResetPrimitiveRecordCursor(EffPrim *primitive) {
    primitive->cursorIndex = 0;
    primitive->cursorPosition = 0;
}

void effSetPrimitiveRecordCursorStep(EffPrim *primitive, f32 step) {
    primitive->cursorStep = step;
}

/* Build a temporary record array and dispatch it through the selected path. */
void effBuildAndDispatch(EffPrim *primitive, s32 variant) {
    void *allocation = sdfAllocGeneralBlock(primitive->recordCount * 12);
    void *records = sdfResourceRetainAddress(allocation);

    func_001931E0(records, primitive->unk10, primitive->recordCount);
    if (variant == 0) {
        func_001934E8(primitive, records);
    } else {
        func_001935B8(primitive, records);
    }
    sdfReleaseResourceAllocation(allocation);
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
    allocation = sdfAllocGeneralBlock(0x18);
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

    effSampleChannelBezier(vertex, channel, index, position);
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

void effSampleChannelBezier(EffVert *out, EffChan *channel, s32 index, f32 t) {
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
