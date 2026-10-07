#include "common.h"
#include "eff.h"
#include "eff_channel.h"
#include "pcp_vu0.h"

#define EFF_CURVE_COMPONENT_COUNT 3
#define EFF_CURVE_POINT_BYTES 12
#define EFF_CURVE_ROW_FLOATS 4
#define EFF_CURVE_SCALAR_SOURCE_OFFSET 16
#define EFF_CURVE_SOURCE_ROW_COUNT 4
#define EFF_BEZIER_MIN_RECORD_COUNT 4
#define EFF_BEZIER_RECORD_ADVANCE 3
#define EFF_CHANNEL_BYTES 0x18
#define EFF_CHANNEL_DEFAULT_STEP 0.05f
#define EFF_CHANNEL_ALPHA_SCALE 127.0f
#define EFF_CHANNEL_ALPHA_SHIFT 24
#define EFF_CHANNEL_RGB_COLOR 0x00808080
#define EFF_PRIMITIVE_CUBIC_MODE 0

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

/* Four XYZ Bezier controls, cursor position and step: the effMath EffBezierSlot layout. */
typedef struct EffRec38 {
    f32 controlPoints[4][3];
    f32 scaledRandomValue; /* 0x30 */
    f32 randomScale;       /* 0x34 */
} EffRec38;

/* Interpolation output vector; the cubic path writes W while the linear path writes only XYZ. */
typedef struct EffVert {
    f32 unk0; /* 0x0 */
    f32 unk4; /* 0x4 */
    f32 unk8; /* 0x8 */
    f32 unkC; /* 0xC */
} EffVert;

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
typedef struct SdfMemBlock SdfMemBlock;
extern SdfMemBlock *sdfAllocGeneralBlock(s32 arg0);
extern u32 sdfResourceRetainAddress(SdfMemBlock *arg0);
extern void func_00192ED0(EffVert *arg0, EffPrim *arg1, s32 arg2, f32 arg3);
extern void effSampleChannelBezier(EffVert *arg0, EffChan *arg1, s32 arg2, f32 arg3);
extern u32 effMiscRand(void *state);
extern u8 D_0034DF38[];
extern void *effMathGetSlotAt(void *slots, s32 index);
extern void effJitterChannelControlPoints(EffChanWork *arg0, u32 arg1);
extern void func_001931E0(void *arg0, s32 arg1, u32 arg2);
extern void func_001934E8(EffPrim *arg0, f32 *arg1);
extern void func_001935B8(EffPrim *arg0, void *arg1);
extern void effMathReleaseWorkResource(void *work);
extern void effDispatchParameterDataAndFreeWork(void *handle);

typedef struct EffChanSourceOwner {
    u8 pad00[4];
    void *param;    /* 0x04 */
} EffChanSourceOwner;

typedef struct EffChanSource {
    EffChanHead head;
    EffChanSourceOwner *owner; /* 0x168 */
} EffChanSource;

extern void *effAllocSlotArray(u32 count);
extern void *effParamWorkDuplicate(void *param);
extern s32 effMathStepBezierSlot(void *slots, s32 index, f32 *out);
extern void effParamWorkCallback0(void *param, void *value);
extern void effParamWorkInvokeCallback(void *param);

/* Clone channel work, allocate slots and duplicate a parameter template per record.
 * The record count is captured before allocation; a nonpositive delay modulus becomes one. */
EffChanWork *effChanWorkCreate(EffChanSource *source) {
    u32 recordCount = source->head.count;
    SdfMemBlock *allocationHandle = sdfAllocGeneralBlock(recordCount * sizeof(EffChanRecord) + sizeof(EffChanWork));
    EffChanWork *work = (EffChanWork *)sdfResourceRetainAddress(allocationHandle);
    EffChanRecord *recordCursor = (EffChanRecord *)(work + 1);
    void *parameterTemplate;
    s32 delayModulus;
    u32 recordIndex;

    work->head = source->head;
    work->buffer = allocationHandle;
    work->records = recordCursor;
    if (work->head.spread <= 0) {
        work->head.spread = 1;
    }
    work->slots = effAllocSlotArray(recordCount);
    if (recordCount != 0) {
        parameterTemplate = source->owner->param;
        delayModulus = work->head.spread;
        for (recordIndex = 0; recordIndex < recordCount; recordIndex++) {
            recordCursor->param = effParamWorkDuplicate(parameterTemplate);
            recordCursor->delay = -(effMiscRand(D_0034DF38) % delayModulus);
            recordCursor++;
        }
    }
    return work;
}

/* Release slot storage, every duplicated parameter and the backing allocation in native order. */
void effDestroyChannelWork(EffChanWork *work) {
    EffChanRecord *recordCursor;
    u32 recordIndex = 0;
    u32 recordCount;

    effMathReleaseWorkResource(work->slots);
    recordCount = work->head.count;
    recordCursor = work->records;
    if (recordCount != 0) {
        do {
            effDispatchParameterDataAndFreeWork(recordCursor->param);
            recordCursor++;
            recordIndex++;
        } while (recordIndex < recordCount);
    }
    sdfReleaseResourceAllocation(work->buffer);
}

extern f32 sdfViewTargetVector[4];
extern f32 sdfViewEyeVector[4];
extern f32 effMiscRandUnitFloat(void *);
extern void effParamWorkCallback3(void *param, u32 value);

/* vu0 routine: Jitter four control points normal to the path and viewing direction.
 * Reuse the final edge normal for the endpoint; retain XYZ-only scale initialization. */
void effJitterChannelControlPoints(EffChanWork *work, u32 recordIndex) {
    f32 jitterScale[4];
    f32 lastNormal[4];
    f32 viewDirection[4];
    f32 jitteredPoint[4];
    EffChanRecord *record = work->records + recordIndex;
    EffRec38 *bezierSlot;
    f32 cursorStep;
    f32 centeredRandom;

    VU0_LOAD_VF(vf10, sdfViewTargetVector);
    VU0_LOAD_VF(vf11, sdfViewEyeVector);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, viewDirection);
    cursorStep = 1.0f / (f32)work->head.steps;
    bezierSlot = (EffRec38 *)effMathGetSlotAt(work->slots, recordIndex);
    bezierSlot->scaledRandomValue = 0;
    bezierSlot->randomScale = cursorStep;

    centeredRandom = effMiscRandUnitFloat(D_0034DF38) - 0.5f;
    jitterScale[0] = work->head.jitter[0] * (centeredRandom + centeredRandom);
    jitterScale[1] = jitterScale[0];
    jitterScale[2] = jitterScale[0];
    VU0_LOAD_VF(vf10, work->head.controlPoints[0]);
    VU0_LOAD_VF(vf11, work->head.controlPoints[1]);
    VU0_MOVE_VF(vf12, vf10);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, viewDirection);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_LOAD_VF(vf11, jitterScale);
    VU0_MUL(vf10, vf10, vf11);
    VU0_ADD(vf10, vf10, vf12);
    VU0_STORE_VF(vf10, jitteredPoint);
    bezierSlot->controlPoints[0][0] = jitteredPoint[0];
    bezierSlot->controlPoints[0][1] = jitteredPoint[1];
    bezierSlot->controlPoints[0][2] = jitteredPoint[2];

    centeredRandom = effMiscRandUnitFloat(D_0034DF38) - 0.5f;
    jitterScale[0] = work->head.jitter[1] * (centeredRandom + centeredRandom);
    jitterScale[1] = jitterScale[0];
    jitterScale[2] = jitterScale[0];
    VU0_LOAD_VF(vf10, work->head.controlPoints[1]);
    VU0_LOAD_VF(vf11, work->head.controlPoints[2]);
    VU0_MOVE_VF(vf12, vf10);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, viewDirection);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_LOAD_VF(vf11, jitterScale);
    VU0_MUL(vf10, vf10, vf11);
    VU0_ADD(vf10, vf10, vf12);
    VU0_STORE_VF(vf10, jitteredPoint);
    bezierSlot->controlPoints[1][0] = jitteredPoint[0];
    bezierSlot->controlPoints[1][1] = jitteredPoint[1];
    bezierSlot->controlPoints[1][2] = jitteredPoint[2];

    centeredRandom = effMiscRandUnitFloat(D_0034DF38) - 0.5f;
    jitterScale[0] = work->head.jitter[2] * (centeredRandom + centeredRandom);
    jitterScale[1] = jitterScale[0];
    jitterScale[2] = jitterScale[0];
    VU0_LOAD_VF(vf10, work->head.controlPoints[2]);
    VU0_LOAD_VF(vf11, work->head.controlPoints[3]);
    VU0_MOVE_VF(vf12, vf10);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, viewDirection);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, lastNormal);
    VU0_LOAD_VF(vf11, jitterScale);
    VU0_MUL(vf10, vf10, vf11);
    VU0_ADD(vf10, vf10, vf12);
    VU0_STORE_VF(vf10, jitteredPoint);
    bezierSlot->controlPoints[2][0] = jitteredPoint[0];
    bezierSlot->controlPoints[2][1] = jitteredPoint[1];
    bezierSlot->controlPoints[2][2] = jitteredPoint[2];

    centeredRandom = effMiscRandUnitFloat(D_0034DF38) - 0.5f;
    jitterScale[0] = work->head.jitter[3] * (centeredRandom + centeredRandom);
    jitterScale[1] = jitterScale[0];
    jitterScale[2] = jitterScale[0];
    VU0_LOAD_VF(vf10, lastNormal);
    VU0_LOAD_VF(vf11, jitterScale);
    VU0_MUL(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, work->head.controlPoints[3]);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, jitteredPoint);
    bezierSlot->controlPoints[3][0] = jitteredPoint[0];
    bezierSlot->controlPoints[3][1] = jitteredPoint[1];
    bezierSlot->controlPoints[3][2] = jitteredPoint[2];
    effParamWorkCallback3(record->param, 0);
}

/* Step active records, update their packed color/position callbacks, and optionally recycle.
 * The slot-zero lookup precedes the empty-count check; retain the time snapshot and later stored increments. */
void func_001929A0(EffChanWork *work) {
    EffChanRecord *recordCursor = work->records;
    void *bezierSlots = work->slots;
    u32 recordCount = work->head.count;
    u32 recordIndex = 0;
    u8 cycleEnabled = work->head.enabled;
    s32 activeSteps = work->head.steps;
    s32 delayModulus = work->head.spread;
    s32 fadeInSteps = work->head.fadeIn;
    s32 fadeOutSteps = work->head.fadeOut;
    f32 position[3];

    (void)effMathGetSlotAt(bezierSlots, 0);
    if (recordCount == 0) {
        return;
    }
    do {
        s32 recordTime = recordCursor->delay;

        if (recordTime == 0) {
            effJitterChannelControlPoints(work, recordIndex);
        }
        if (recordTime > 0 && recordTime <= activeSteps) {
            f32 opacity;
            s32 fadePhase;
            u32 alpha;
            u32 color;

            effMathStepBezierSlot(bezierSlots, recordIndex, position);
            fadePhase = fadeInSteps > recordTime;
            if (fadePhase) {
                opacity = (f32)recordTime / (f32)fadeInSteps;
            } else {
                fadePhase = activeSteps - recordTime;
                if (fadePhase <= fadeOutSteps) {
                    opacity = (f32)fadePhase / (f32)fadeOutSteps;
                } else {
                    opacity = 1.0f;
                }
            }
            alpha = (u32)(opacity * EFF_CHANNEL_ALPHA_SCALE);
            color = (alpha << EFF_CHANNEL_ALPHA_SHIFT) | EFF_CHANNEL_RGB_COLOR;
            effParamWorkCallback3(recordCursor->param, color);
            effParamWorkCallback0(recordCursor->param, position);
            effParamWorkInvokeCallback(recordCursor->param);
        }
        if (recordTime < activeSteps) {
            recordCursor->delay++;
        } else if (cycleEnabled != 0) {
            recordCursor->delay = -(effMiscRand(D_0034DF38) % delayModulus);
        } else {
            recordCursor->delay++;
        }
        recordCursor++;
        recordIndex++;
    } while (recordIndex < recordCount);
}

/* Copy XYZ from four float4 rows without touching destination W, then copy four trailing scalars. */
void effCopyVertRows(EffChan *channel, f32 *source) {
    f32 *sourceRow;
    u32 rowIndex;
    f32 *scalarCursor;
    f32 *destinationRow;

    sourceRow = source;
    rowIndex = 0;
    source += EFF_CURVE_SCALAR_SOURCE_OFFSET;
    scalarCursor = channel->rows->secondary;
    destinationRow = channel->rows->primary;
    do {
        rowIndex++;
        destinationRow[0] = sourceRow[0];
        destinationRow[1] = sourceRow[1];
        destinationRow[2] = sourceRow[2];
        sourceRow += EFF_CURVE_ROW_FLOATS;
        destinationRow += EFF_CURVE_ROW_FLOATS;
        *scalarCursor = *source++;
        scalarCursor++;
    } while (rowIndex < EFF_CURVE_SOURCE_ROW_COUNT);
}

/* Jitter each slot and choose its initial unsigned-modulo step.
 * Stored record time is one ahead of the Bezier cursor parameter's sampled step. */
void effFillRandRecords(EffEmit *emitter) {
    EffChanWork *channelWork = emitter->primitive;
    u32 stepModulus = channelWork->head.steps;
    u32 recordCount = channelWork->head.count;
    EffChanRecord *recordCursor = channelWork->records;
    u32 recordIndex = 0;
    EffRec38 *bezierSlot;
    s32 randomStep;
    f32 cursorStep;

    if (recordCount == 0) {
        return;
    }
    do {
        effJitterChannelControlPoints(channelWork, recordIndex);
        recordCursor->delay = effMiscRand(&D_0034DF38) % stepModulus;
        bezierSlot = (EffRec38 *)effMathGetSlotAt(channelWork->slots, recordIndex);
        recordIndex++;
        randomStep = recordCursor->delay;
        cursorStep = bezierSlot->randomScale;
        recordCursor->delay = randomStep + 1;
        recordCursor++;
        bezierSlot->scaledRandomValue = cursorStep * (f32)randomStep;
    } while (recordIndex < recordCount);
}

extern void effBuildAndDispatch(EffPrim *, s32);

EffPrim *effCreatePrimitiveCurve(f32 *keys, u32 recordCount, s32 flattenEqualComponents) {
    SdfMemBlock *primaryResource = sdfAllocGeneralBlock(0x2C);
    f32 step = EFF_CHANNEL_DEFAULT_STEP;
    EffPrim *primitive = (EffPrim *)sdfResourceRetainAddress(primaryResource);
    SdfMemBlock *coefficientResource;
    f32 (*coefficients)[EFF_CURVE_COMPONENT_COUNT];

    primitive->primaryResource = primaryResource;
    primitive->recordCount = recordCount;
    primitive->unk10 = (s32)keys;
    primitive->cursorIndex = 0;
    primitive->cursorPosition = 0.0f;
    primitive->cursorStep = step;
    if (recordCount < 3) {
        primitive->unk14 = NULL;
        primitive->unkC = 1;
    } else {
        coefficientResource = sdfAllocGeneralBlock(recordCount * 36);
        coefficients = (f32 (*)[EFF_CURVE_COMPONENT_COUNT])sdfResourceRetainAddress(coefficientResource);
        primitive->primaryResource = primaryResource;
        primitive->secondaryResource = coefficientResource;
        primitive->unkC = EFF_PRIMITIVE_CUBIC_MODE;
        primitive->recordCount = recordCount;
        primitive->unk10 = (s32)keys;
        primitive->unk14 = coefficients;
        primitive->unk18 = coefficients + recordCount;
        primitive->unk1C = coefficients + recordCount * 2;
        primitive->cursorIndex = 0;
        primitive->cursorPosition = 0.0f;
        primitive->cursorStep = step;
        effBuildAndDispatch(primitive, flattenEqualComponents);
    }
    return primitive;
}

/* Release the optional coefficient allocation, then the primary buffer; do not free or clear the object. */
void effFreeBuffers(EffPrim *primitive) {
    if (primitive != NULL) {
        if (primitive->unk14 != NULL) {
            sdfReleaseResourceAllocation(primitive->secondaryResource);
        }
        sdfReleaseResourceAllocation(primitive->primaryResource);
    }
}

/* Sample before stepping. Wrap at >1 only, subtract once, and return zero when the record cursor resets. */
s32 effAdvancePrimCursor(void *vertex, EffPrim *primitive) {
    s32 continuing = 1;
    f32 position = primitive->cursorPosition;
    u32 recordIndex = primitive->cursorIndex;

    func_00192ED0(vertex, primitive, recordIndex, position);
    position += primitive->cursorStep;
    if (position > 1.0f) {
        position -= 1.0f;
        recordIndex += 1;
    }
    if (recordIndex >= primitive->recordCount - 1) {
        position = 0.0f;
        recordIndex = 0;
        continuing = 0;
    }
    primitive->cursorIndex = recordIndex;
    primitive->cursorPosition = position;
    return continuing;
}

/* Evaluate packed XYZ cubic coefficients or adjacent linear keys at t.
 * a/b serve as coefficients or endpoints; only the cubic path writes output W. */
void func_00192ED0(EffVert *vertex, EffPrim *primitive, s32 recordIndex, f32 t) {
    f32 *a;
    f32 *b;
    f32 *c;
    f32 *d;

    if (primitive->unkC == EFF_PRIMITIVE_CUBIC_MODE) {
        a = (f32 *)primitive->unk14 + recordIndex * EFF_CURVE_COMPONENT_COUNT;
        b = (f32 *)primitive->unk18 + recordIndex * EFF_CURVE_COMPONENT_COUNT;
        c = (f32 *)primitive->unk1C + recordIndex * EFF_CURVE_COMPONENT_COUNT;
        d = (f32 *)primitive->unk10 + recordIndex * EFF_CURVE_COMPONENT_COUNT;
        vertex->unk0 = ((a[0] * t + b[0]) * t + c[0]) * t + d[0];
        vertex->unk4 = ((a[1] * t + b[1]) * t + c[1]) * t + d[1];
        vertex->unk8 = ((a[2] * t + b[2]) * t + c[2]) * t + d[2];
        vertex->unkC = 1.0f;
    } else {
        a = (f32 *)primitive->unk10 + recordIndex * EFF_CURVE_COMPONENT_COUNT;
        b = a + EFF_CURVE_COMPONENT_COUNT;
        vertex->unk0 = a[0] + (b[0] - a[0]) * t;
        vertex->unk4 = a[1] + (b[1] - a[1]) * t;
        vertex->unk8 = a[2] + (b[2] - a[2]) * t;
    }
}

/* Evaluate packed XYZ cubic coefficients or adjacent linear keys into vf10.
 * The linear path leaves local W untouched before the native full-vector load. */
void effSamplePrimitiveCurve(EffPrim *primitive, s32 recordIndex, f32 t)
{
    f32 vertex[4];
    f32 *a;
    f32 *b;
    f32 *c;
    f32 *d;

    if (primitive->unkC == EFF_PRIMITIVE_CUBIC_MODE) {
        a = (f32 *)primitive->unk14 + recordIndex * EFF_CURVE_COMPONENT_COUNT;
        b = (f32 *)primitive->unk18 + recordIndex * EFF_CURVE_COMPONENT_COUNT;
        c = (f32 *)primitive->unk1C + recordIndex * EFF_CURVE_COMPONENT_COUNT;
        d = (f32 *)primitive->unk10 + recordIndex * EFF_CURVE_COMPONENT_COUNT;
        vertex[0] = ((a[0] * t + b[0]) * t + c[0]) * t + d[0];
        vertex[1] = ((a[1] * t + b[1]) * t + c[1]) * t + d[1];
        vertex[2] = ((a[2] * t + b[2]) * t + c[2]) * t + d[2];
        vertex[3] = 1.0f;
    } else {
        a = (f32 *)primitive->unk10 + recordIndex * EFF_CURVE_COMPONENT_COUNT;
        b = a + EFF_CURVE_COMPONENT_COUNT;
        vertex[0] = a[0] + (b[0] - a[0]) * t;
        vertex[1] = a[1] + (b[1] - a[1]) * t;
        vertex[2] = a[2] + (b[2] - a[2]) * t;
    }
    VU0_LOAD_VF_FROM(vf10, *(u128 *)vertex);
}

/* Reset the primitive's record index and within-record position without changing its step. */
void effResetPrimitiveRecordCursor(EffPrim *primitive) {
    primitive->cursorIndex = 0;
    primitive->cursorPosition = 0;
}

/* Set the per-call cursor increment; sampling and wrapping are performed by the advance routine. */
void effSetPrimitiveRecordCursorStep(EffPrim *primitive, f32 step) {
    primitive->cursorStep = step;
}

/* Build temporary coordinate-major tangents, select a coefficient policy, then release the temporary buffer. */
void effBuildAndDispatch(EffPrim *primitive, s32 flattenEqualComponents) {
    void *allocation = sdfAllocGeneralBlock(primitive->recordCount * EFF_CURVE_POINT_BYTES);
    void *tangentData = (void *)sdfResourceRetainAddress(allocation);

    func_001931E0(tangentData, primitive->unk10, primitive->recordCount);
    if (flattenEqualComponents == 0) {
        func_001934E8(primitive, tangentData);
    } else {
        func_001935B8(primitive, tangentData);
    }
    sdfReleaseResourceAllocation(allocation);
}


INCLUDE_ASM(const s32, "game/code_00192488", func_001931E0);

/* Solve the cubic tangent system: endpoint diagonal 2, interior diagonal 4. */
void effSolveCubicTangents(f32 *solution, f32 *rhs, s32 count) {
    s32 byteCount = count * sizeof(f32);
    void *allocation = sdfAllocGeneralBlock(byteCount);
    f32 *upper;
    s32 i;
    f32 denominator;
    f32 *upperEnd;
    f32 *solutionEnd;
    f32 *rhsEnd;

    count -= 2;
    upper = (f32 *)sdfResourceRetainAddress(allocation);
    denominator = 2.0f;

    solution[0] = rhs[0] / denominator;
    for (i = 1; i <= count; i++) {
        upper[i] = 1.0f / denominator;
        denominator = 4.0f - upper[i];
        solution[i] = (rhs[i] - solution[i - 1]) / denominator;
    }
    upperEnd = (f32 *)((u8 *)upper + byteCount);
    solutionEnd = (f32 *)((u8 *)solution + byteCount);
    rhsEnd = (f32 *)((u8 *)rhs + byteCount);
    upperEnd[-1] = 1.0f / denominator;
    denominator = 2.0f - upperEnd[-1];
    solutionEnd[-1] = (rhsEnd[-1] - solutionEnd[-2]) / denominator;
    for (i = count; i >= 0; i--) {
        solution[i] -= upper[i + 1] * solution[i + 1];
    }
    sdfReleaseResourceAllocation(allocation);
}

/* Build Hermite power-basis XYZ coefficients from interleaved points and coordinate-major tangents. */
void func_001934E8(EffPrim *primitive, f32 *tangents) {
    f32 *cubic = primitive->unk14;
    f32 *quadratic = primitive->unk18;
    f32 *linear = primitive->unk1C;
    f32 *points = (f32 *)primitive->unk10;
    s32 recordCount = primitive->recordCount;
    s32 segment;
    for (segment = 0; segment < recordCount - 1; segment++) {
        s32 component;
        for (component = 0; component < EFF_CURVE_COMPONENT_COUNT; component++) {
            f32 endValue = points[segment * EFF_CURVE_COMPONENT_COUNT + component + EFF_CURVE_COMPONENT_COUNT];
            f32 startValue = points[segment * EFF_CURVE_COMPONENT_COUNT + component];
            f32 startTangent = tangents[component * recordCount + segment];
            f32 endTangent = tangents[component * recordCount + segment + 1];
            cubic[segment * EFF_CURVE_COMPONENT_COUNT + component] = 2.0f * (startValue - endValue) +
                (startTangent + endTangent);
            quadratic[segment * EFF_CURVE_COMPONENT_COUNT + component] = (endValue - startValue) * 3.0f -
                (2.0f * startTangent + endTangent);
            linear[segment * EFF_CURVE_COMPONENT_COUNT + component] = startTangent;
        }
    }
}

/* Build Hermite coefficients, but flatten exactly equal endpoint components even when tangents are nonzero. */
void func_001935B8(EffPrim *primitive, void *tangentData) {
    f32 *tangents = tangentData;
    f32 *cubic = primitive->unk14;
    f32 *quadratic = primitive->unk18;
    f32 *linear = primitive->unk1C;
    f32 *points = (f32 *)primitive->unk10;
    s32 recordCount = primitive->recordCount;
    s32 segment;
    s32 component;

    for (segment = 0; segment < recordCount - 1; segment++) {
        for (component = 0; component < EFF_CURVE_COMPONENT_COUNT; component++) {
            f32 startValue = points[segment * EFF_CURVE_COMPONENT_COUNT + component];
            f32 endValue = points[segment * EFF_CURVE_COMPONENT_COUNT + component + EFF_CURVE_COMPONENT_COUNT];

            if (startValue == endValue) {
                cubic[segment * EFF_CURVE_COMPONENT_COUNT + component] = 0.0f;
                quadratic[segment * EFF_CURVE_COMPONENT_COUNT + component] = 0.0f;
                linear[segment * EFF_CURVE_COMPONENT_COUNT + component] = 0.0f;
            } else {
                f32 startTangent = tangents[component * recordCount + segment];
                f32 endTangent = tangents[component * recordCount + segment + 1];

                cubic[segment * EFF_CURVE_COMPONENT_COUNT + component] = 2.0f * (startValue - endValue) + (startTangent + endTangent);
                quadratic[segment * EFF_CURVE_COMPONENT_COUNT + component] = (endValue - startValue) * 3.0f - (2.0f * startTangent + endTangent);
                linear[segment * EFF_CURVE_COMPONENT_COUNT + component] = startTangent;
            }
        }
    }
}

/* Allocate the small channel when at least four records exist; borrow the supplied rows and set its default step. */
void *effCreateChannel(void *rows, u32 recordCount) {
    void *channel = NULL;
    void *allocation;
    EffChan *channelObject;

    if (recordCount < EFF_BEZIER_MIN_RECORD_COUNT) {
        return channel;
    }
    allocation = sdfAllocGeneralBlock(EFF_CHANNEL_BYTES);
    channel = (void *)sdfResourceRetainAddress(allocation);
    channelObject = channel;
    channelObject->unk0 = allocation;
    channelObject->cursorStep = EFF_CHANNEL_DEFAULT_STEP;
    channelObject->recordCount = recordCount;
    channelObject->rows = rows;
    channelObject->cursorPosition = 0;
    channelObject->cursorIndex = 0;
    return channel;
}

/* Release the handle at a nonnull address without clearing it.
 * The native s32 signature has no explicit return; retain that contract. */
s32 effReleaseInterpolationChannel(void **handleAddress) {
    if (handleAddress != NULL) {
        sdfReleaseResourceAllocation(*handleAddress);
    }
}

/* Sample before stepping, then advance three records only when position exceeds one.
 * Subtract once; return zero when the record cursor resets at the unsigned count-minus-one boundary. */
s32 effAdvanceChanCursor(void *vertex, EffChan *channel) {
    s32 continuing = 1;
    f32 position = channel->cursorPosition;
    u32 recordIndex = channel->cursorIndex;

    effSampleChannelBezier(vertex, channel, recordIndex, position);
    position += channel->cursorStep;
    if (position > 1.0f) {
        position -= 1.0f;
        recordIndex += EFF_BEZIER_RECORD_ADVANCE;
    }
    if (recordIndex >= channel->recordCount - 1) {
        position = 0.0f;
        recordIndex = 0;
        continuing = 0;
    }
    channel->cursorIndex = recordIndex;
    channel->cursorPosition = position;
    return continuing;
}

/* Evaluate four packed XYZ Bezier controls at t and write homogeneous W=1.
 * Preserve the native weight multiplication and sum association. */
void effSampleChannelBezier(EffVert *vertex, EffChan *channel, s32 recordIndex, f32 t) {
    f32 weights[4];
    f32 oneMinusT = 1.0f - t;
    f32 *p0 = channel->rows->primary + recordIndex * EFF_CURVE_COMPONENT_COUNT;
    f32 *p1 = p0 + EFF_CURVE_COMPONENT_COUNT;
    f32 *p2 = p0 + 6;
    f32 *p3 = p0 + 9;

    weights[0] = oneMinusT * oneMinusT * oneMinusT;
    weights[1] = t * (oneMinusT * oneMinusT) * 3.0f;
    weights[2] = t * t * oneMinusT * 3.0f;
    weights[3] = t * t * t;
    vertex->unk0 = p0[0] * weights[0] + p1[0] * weights[1] + p2[0] * weights[2] + p3[0] * weights[3];
    vertex->unk4 = p0[1] * weights[0] + p1[1] * weights[1] + p2[1] * weights[2] + p3[1] * weights[3];
    vertex->unk8 = p0[2] * weights[0] + p1[2] * weights[1] + p2[2] * weights[2] + p3[2] * weights[3];
    vertex->unkC = 1.0f;
}

/* Reset the channel's record index and within-record position without changing its step. */
void effClearChanCursor(EffChan *channel) {
    channel->cursorIndex = 0;
    channel->cursorPosition = 0;
}

/* Set the per-call channel cursor increment without advancing it. */
void effSetChanStep(EffChan *channel, f32 step) {
    channel->cursorStep = step;
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

