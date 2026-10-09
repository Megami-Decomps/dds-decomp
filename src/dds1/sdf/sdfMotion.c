#include "common.h"
#include "sdf_asset_state.h"
#include "sdf_chip.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "sdf.h"
#include "sdf_draw.h"
#include "sdf_motion.h"
#include "sdf_motion_bindings.h"

typedef struct {
    f32 *unk0;
    f32 *unk4;
    f32 unk8;
} BlendArg;

typedef struct VTab {
    void (*invoke)(void);
    void (*callback04)(void *, void *);
    void (*sample)(void *, f32);
    void (*callback0C)(void *);
    void (*blend)(void *, f32, f32);
} VTab;

typedef struct VObj {
    VTab *vtable;
} VObj;


typedef SdfMotionTextParamSnapshot Blk;



typedef struct {
    u8 pad[0x14];
    void *unk14;
} HasLink14;

typedef struct {
    u16 u0;
    u16 n;
    void **tbl;
    s32 data[1];
} KeysH;

typedef struct FuncTab {
    void *w0;
    void (*w4)(void *a0, void *a1);
    void (*w8)(void *a0, void *a1, f32 t);
    void (*wC)(void *a0);
    void (*w10)(void *a0, f32 t1, f32 t2);
} FuncTab;

s32 sdfDispatchAssetCommandWord(void *a0, s32 a1, s32 a2);
void sdfDestroyDevRequest(void *a0);
void sdfSetMotionPointerPair(SdfMotionBindingHead *binding, void *source, void *dispatch);
f32 sdfInterpolateMotionKeys(SdfMotionKeyInterval *interval);
void sdfFindMotionKeyInterval(void *bindingArg, void *intervalArg, f32 frame);
s32 sdfMotionInterpolateKeyColor(SdfMotionKeyInterval *a0);
void sdfMotionBindIndexedTrack(SdfMotionIndexedBinding *binding, Motion *motion,
                               void *dispatch, s32 options);
extern void effMiscQuaternionNlerpVU(f32 amount);
extern void effMiscQuaternionToMatrixVU(void);
extern s32 (*D_003981B8[])(void *a0, s32 a1);
extern s32 (*D_00398248[])(void *a0, s32 a1);
extern void *D_003981D0[];
extern void *D_003981E8[];
extern void *D_00398200[];
extern void *D_00398218[];
extern void *D_00398230[];
extern void *D_00398270[];
extern void *D_00398288[];
extern void *D_003982A0[];
extern void *D_003982B8[];
extern void *D_003982D0[];
extern void *D_003982E8[];
extern void *D_00398300[];
extern void *D_00398318[];
extern void *D_00398330[];
extern void *D_00398348[];

void sdfInvokeMotionObjectCallback(VObj *object) {
    object->vtable->invoke();
}

/* Install the binding's dispatch table and source without touching its payload. */
void sdfSetMotionPointerPair(SdfMotionBindingHead *binding, void *source, void *dispatch) {
    binding->dispatch = dispatch;
    binding->source = source;
}


Motion *sdfCreateMotion(SdfModel *model, MotionTable *table) {
    Motion *motion;
    DevRequest *request;
    SdfMotionCommand *command;
    s32 i;
    u16 count;

    motion = sdfAllocAndClearQuadwords(0x34);
    motion->next = model->motionList;
    count = table->commandCount;
    model->motionList = motion;
    motion->owner = model;
    motion->motionTable = table;
    request = sdfDevCreateBufferedRequest(count, 4, 8);
    motion->request = request;
    request->usedCount = count;
    for (i = 0, command = motion->motionTable->commands;
         i < count;
         i++, command++) {
        ((void **)motion->request->buffer)[i] =
            (void *)sdfDispatchAssetCommandWord(motion, command->command, command->argument);
    }
    motion->state = SDF_MOTION_STATE_UNINITIALIZED;
    motion->frameStep = 1.0f;
    return motion;
}


/* Unlinks the node from its owner's list, notifies each request callback, then frees the request and the node. */
void sdfDestroyMotion(Motion *node)
{
    Motion **prev;
    Motion *cur;
    DevRequest *request;
    void **objects;
    s32 objectCount;
    s32 i;

    if (node == NULL) {
        return;
    }
    if (node->owner != NULL) {
        prev = &node->owner->motionList;
        while ((cur = *prev) != NULL) {
            if (cur == node) {
                *prev = node->next;
                break;
            }
            prev = &cur->next;
        }
    }
    request = node->request;
    objectCount = request->usedCount;
    objects = request->buffer;
    for (i = 0; i < objectCount; i++) {
        sdfInvokeMotionObjectCallback(objects[i]);
    }
    sdfDestroyDevRequest(node->request);
    sdfReleaseChipBlock(node);
}

/* Select a motion entry, initialize its timing and state, and bind its setup records. */
void sdfMotionInitialize(Motion *motion, s32 motionIndex, s32 loopEnabled, f32 blendLeadFrames,
                         f32 blendDurationFrames) {
    s32 i;
    MotionEntry *entry;
    u8 *bindingData;
    VObj *object;

    motion->motionIndex = motionIndex;
    if (blendDurationFrames > 0.0f) {
        motion->blendDurationFrames = blendDurationFrames;
        motion->blendStartFrame = -blendLeadFrames;
        for (i = 0; i < motion->request->usedCount; i++) {
            object = ((void **)motion->request->buffer)[i];
            object->vtable->callback0C(object);
        }
    } else {
        motion->blendDurationFrames = 0.0f;
        motion->blendStartFrame = 0.0f;
    }

    motion->loopEnabled = loopEnabled;
    motion->state = SDF_MOTION_STATE_INITIALIZED;
    motion->currentFrame = motion->blendStartFrame;
    entry = motion->motionTable->entries[motionIndex];
    if (entry == NULL) {
        motion->frameCount = 0;
        motion->state = SDF_MOTION_STATE_TERMINAL;
        return;
    }

    motion->frameCount = entry->frameCount;
    bindingData = (u8 *)entry->bindingData;
    for (i = 0; i < motion->request->usedCount; i++) {
        object = ((void **)motion->request->buffer)[i];
        object->vtable->callback04(object, bindingData);
        bindingData += *(u32 *)bindingData;
    }
}
/* Select a motion with no lead-in and no blend duration. */
void sdfMotionInitializeAtZeroTime(Motion *motion, s32 motionIndex, s32 loopEnabled) {
    sdfMotionInitialize(motion, motionIndex, loopEnabled, 0.0f, 0.0f);
}


void sdfMotionSampleAtFrame(Motion *motion, f32 frame) {
    s32 i;
    s32 count;
    f32 elapsed;
    f32 duration;
    f32 weight;
    VObj *object;

    motion->state = SDF_MOTION_STATE_SAMPLING;
    duration = motion->blendDurationFrames;
    elapsed = frame - motion->blendStartFrame;
    motion->currentFrame = frame;
    if (elapsed < duration) {
        if (elapsed < 0.0f) {
            weight = 0.0f;
        } else {
            weight = elapsed / duration;
        }
        if (frame < 0.0f) {
            frame = 0.0f;
        }
        for (i = 0; i < motion->request->usedCount; i++) {
            object = ((void **)motion->request->buffer)[i];
            object->vtable->blend(object, frame, weight);
        }
    } else {
        count = motion->request->usedCount;
        for (i = 0; i < count; i++) {
            object = ((void **)motion->request->buffer)[i];
            object->vtable->sample(object, frame);
        }
    }
}

/* Apply the current motion state and report completion (1) or a loop wrap (2). */
s32 sdfMotionUpdate(Motion *motion) {
    s32 result;
    f32 frame;
    f32 frameCount;

    result = 0;
    switch (motion->state) {
    case SDF_MOTION_STATE_UNINITIALIZED:
    case 2:
    case 3:
    case SDF_MOTION_STATE_TERMINAL:
    case SDF_MOTION_STATE_SUSPENDED:
        break;
    case SDF_MOTION_STATE_INITIALIZED:
        sdfMotionSampleAtFrame(motion, motion->blendStartFrame);
        break;
    case SDF_MOTION_STATE_SAMPLING:
        frame = motion->currentFrame + motion->frameStep;
        frameCount = motion->frameCount;
        if (motion->loopEnabled == 0) {
            if (frameCount <= frame) {
                sdfMotionSampleAtFrame(motion, frameCount);
                motion->state = SDF_MOTION_STATE_TERMINAL;
                result = 1;
                break;
            }
        } else if (frameCount <= frame) {
            motion->blendDurationFrames = 0.0f;
            result = 2;
            motion->blendStartFrame = 0.0f;
            do {
                frame -= frameCount;
            } while (frameCount <= frame);
        }
        sdfMotionSampleAtFrame(motion, frame);
        break;
    default:
        break;
    }
    return result;
}
/* State 6 parks motion processing, retaining the previous state to resume. */
void sdfMotionSuspend(Motion *motion) {
    u8 previousState;

    previousState = motion->state;
    if (previousState != SDF_MOTION_STATE_SUSPENDED) {
        motion->previousState = previousState;
        motion->state = SDF_MOTION_STATE_SUSPENDED;
    }
}

/* Resume only a suspended motion, restoring the state saved by suspend. */
void sdfMotionResume(Motion *motion) {
    if (motion->state == SDF_MOTION_STATE_SUSPENDED) {
        motion->state = motion->previousState;
    }
}

void func_002DB7A8(void *a0) {
    sdfReleaseChipBlock(a0);
}

void sdfMotionBindKeyTrack(SdfMotionKeyBinding *binding, SdfMotionKeyTrack *track) {
    binding->track = track;
}

/* Find the key interval containing frame and return its interpolation weight. */
void sdfFindMotionKeyInterval(void *bindingArg, void *intervalArg, f32 frame) {
    SdfMotionKeyBinding *binding;
    SdfMotionKeyInterval *out;
    SdfMotionKeyTrack *track;
    u16 *keyFrames;
    u8 *keyData;
    u8 *firstKeyData;
    s32 frameNumber;
    s32 firstKeyIndex;
    s32 lastKeyIndex;
    s32 middleKeyIndex;
    s32 currentFrame;
    s32 nextFrame;
    s32 nextKeyIndex;
    s32 duration;
    u16 keyStride;
    u16 keyCount;

    binding = bindingArg;
    out = intervalArg;
    firstKeyIndex = 0;
    track = binding->track;
    keyCount = track->keyCount;
    keyFrames = track->keyFrames;
    keyStride = track->keyStride;
    frameNumber = (s32)frame;
    lastKeyIndex = keyCount - 1;
    do {
        middleKeyIndex = (firstKeyIndex + lastKeyIndex + 1) >> 1;
        currentFrame = keyFrames[middleKeyIndex];
        if (frameNumber < currentFrame) {
            middleKeyIndex--;
            lastKeyIndex = middleKeyIndex;
        } else {
            firstKeyIndex = middleKeyIndex;
        }
    } while (firstKeyIndex < lastKeyIndex);

    keyData = (u8 *)&keyFrames[(keyCount + 1) & ~1];
    firstKeyData = keyData + middleKeyIndex * keyStride;
    nextKeyIndex = middleKeyIndex + 1;
    currentFrame = keyFrames[middleKeyIndex];
    out->firstKey = (f32 *)firstKeyData;
    if (nextKeyIndex == keyCount) {
        if (binding->motion->loopEnabled == 0) {
            out->secondKey = (f32 *)firstKeyData;
            nextFrame = currentFrame;
        } else {
            out->secondKey = (f32 *)keyData;
            nextFrame = binding->motion->frameCount;
        }
    } else {
        nextFrame = keyFrames[nextKeyIndex];
        out->secondKey = (f32 *)(firstKeyData + keyStride);
    }

    duration = nextFrame - currentFrame;
    if (duration == 0) {
        out->secondKey = (f32 *)firstKeyData;
        out->weight = 0.0f;
        return;
    }
    out->weight = (frame - currentFrame) / duration;
}

/* Linear blend of two sampled scalar keys: first + second*weight - first*weight. */
f32 sdfInterpolateMotionKeys(SdfMotionKeyInterval *interval) {
    f32 interpolationWeight;
    f32 firstValue;

    interpolationWeight = interval->weight;
    firstValue = *interval->firstKey;
    return (firstValue + (*interval->secondKey * interpolationWeight)) -
           (firstValue * interpolationWeight);
}

/* vu0 routine: blend the two bracketing vec3 keys by the key weight into vf10. */
void sdfMotionBlendVectorKeys(SdfMotionKeyInterval *a0) {
    VU0_LERP_VEC3_KEYS(a0->firstKey, a0->secondKey, a0->weight);
}

/* vu0 routine: blend the two bracketing RGBA colour keys by the key weight. */
s32 sdfMotionInterpolateKeyColor(SdfMotionKeyInterval *a0) {
    s32 color;

    EE_MMI_RGBA_LERP(color, *(u32 *)a0->firstKey, *(u32 *)a0->secondKey, a0->weight, 0.5f);
    return color;
}

/* Interpolate exactly five scalar channels, retaining the retail arithmetic order. */
void sdfMotionBlendFiveFloats(f32 *dst, f32 *src1, f32 *src2, f32 weight) {
    s32 i;
    f32 inverseWeight;

    i = 0;
    inverseWeight = 1.0f - weight;
    do {
        dst[i] = src1[i] * inverseWeight + src2[i] * weight;
        i++;
    } while (i != 5);
}

void sdfMotionBlendFiveKeyValues(SdfMotionKeyInterval *src, f32 *dst) {
    sdfMotionBlendFiveFloats(dst, src->firstKey, src->secondKey, src->weight);
}

s32 sdfDispatchMotionBySelector(void *a0, s32 a1) {
    return D_003981B8[(u16)a1](a0, a1);
}

void sdfMotionBindDrawNode(SdfMotionDrawTargetBinding *binding, Motion *motion,
                          void *dispatch, s32 nodeIndex) {
    SdfDrawNode *drawNode;

    sdfSetMotionPointerPair((SdfMotionBindingHead *)&binding->keys, motion, dispatch);
    drawNode = sdfModelFindDrawNode(motion->owner, nodeIndex);
    binding->node = drawNode;
}

void *sdfMotionCreateDrawVectorBinding(void *a0, s32 a1, s32 a2) {
    void *r;

    r = sdfAllocSizeClassBlock(0x20);
    sdfMotionBindDrawNode(r, a0, D_003981D0, a2);
    return r;
}

/* vu0 routine: interpolate the sampled translation keys into the draw node. */
void sdfMotionBlendDrawVector(SdfMotionDrawBinding *binding, f32 frame) {
    SdfMotionKeyInterval keyInterval;

    sdfFindMotionKeyInterval(binding, &keyInterval, frame);
    EE_MMI_LOAD_VEC3(vf10, keyInterval.firstKey);
    EE_MMI_LOAD_VEC3(vf11, keyInterval.secondKey);
        VU0_LERP_VF10_W(keyInterval.weight);
    VU0_STORE_VF_UNCLOBBERED(vf10, binding->node->translation);
}

/* vu0 routine: interpolate translation keys, then blend with captured translation. */
void sdfMotionBlendDrawVectorWithCurrent(SdfMotionDrawBinding *binding, f32 frame, f32 blendWeight) {
    SdfMotionKeyInterval keyInterval;

    sdfFindMotionKeyInterval(binding, &keyInterval, frame);
    EE_MMI_LOAD_VEC3(vf10, keyInterval.firstKey);
    EE_MMI_LOAD_VEC3(vf11, keyInterval.secondKey);
        VU0_LERP_VF10_COPY(keyInterval.weight);
    VU0_LOAD_VF(vf10, binding->capturedVector);
        VU0_LERP_VF10_W(blendWeight);
    VU0_STORE_VF_UNCLOBBERED(vf10, binding->node->translation);
}

void *sdfCreateMotionDrawNode(void *a0, s32 a1, s32 a2) {
    void *r;

    r = sdfAllocSizeClassBlock(0x20);
    sdfMotionBindDrawNode(r, a0, D_003981E8, a2);
    return r;
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DBCC0);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DBD80);

void *sdfMotionCreateScaleVectorBinding(void *a0, s32 a1, s32 a2) {
    void *r;

    r = sdfAllocSizeClassBlock(0x20);
    sdfMotionBindDrawNode(r, a0, D_00398200, a2);
    return r;
}

/* vu0 routine: interpolate the sampled scale keys into the draw node. */
void sdfMotionBlendScaleVector(SdfMotionDrawBinding *binding, f32 frame) {
    SdfMotionKeyInterval keyInterval;

    sdfFindMotionKeyInterval(binding, &keyInterval, frame);
    EE_MMI_LOAD_VEC3(vf10, keyInterval.firstKey);
    EE_MMI_LOAD_VEC3(vf11, keyInterval.secondKey);
        VU0_LERP_VF10_W(keyInterval.weight);
    VU0_STORE_VF(vf10, binding->node->scale);
}

/* vu0 routine: interpolate scale keys, then blend with captured scale. */
void sdfMotionBlendScaleVectorWithCurrent(SdfMotionDrawBinding *binding, f32 frame, f32 blendWeight) {
    SdfMotionKeyInterval keyInterval;

    sdfFindMotionKeyInterval(binding, &keyInterval, frame);
    EE_MMI_LOAD_VEC3(vf10, keyInterval.firstKey);
    EE_MMI_LOAD_VEC3(vf11, keyInterval.secondKey);
        VU0_LERP_VF10_COPY(keyInterval.weight);
    VU0_LOAD_VF(vf10, binding->capturedVector);
        VU0_LERP_VF10_W(blendWeight);
    VU0_STORE_VF(vf10, binding->node->scale);
}

void *sdfMotionCreateQuaternionBinding(void *a0, s32 a1, s32 a2) {
    void *r;

    r = sdfAllocSizeClassBlock(0x20);
    sdfMotionBindDrawNode(r, a0, D_00398218, a2);
    return r;
}

/* vu0 routine: nlerp sampled quaternion keys, then store the quaternion and matrix. */
void sdfMotionBlendQuaternionToMatrix(SdfMotionDrawBinding *binding, f32 frame) {
    SdfMotionKeyInterval keyInterval;
    SdfDrawNode *drawNode;
    f32 (*matrix)[4];
    f32 *keyData;

    sdfFindMotionKeyInterval(binding, &keyInterval, frame);
    keyData = keyInterval.firstKey;
    EE_MMI_LOAD_S16X4_FIXED12(vf10, keyData);
    keyData = keyInterval.secondKey;
    EE_MMI_LOAD_S16X4_FIXED12(vf11, keyData);
    effMiscQuaternionNlerpVU(keyInterval.weight);
    drawNode = binding->node;
    VU0_STORE_VF_UNCLOBBERED(vf10, drawNode->quaternion);
    matrix = drawNode->localMatrix;
    effMiscQuaternionToMatrixVU();
    VU0_STORE_VF_UNCLOBBERED(vf28, matrix[0]);
    VU0_STORE_VF_UNCLOBBERED(vf29, matrix[1]);
    VU0_STORE_VF_UNCLOBBERED(vf30, matrix[2]);
}

/* vu0 routine: nlerp sampled keys with the captured quaternion, then build the matrix. */
void sdfMotionBlendKeyQuaternionWithBase(SdfMotionDrawBinding *binding, f32 frame, f32 blendWeight) {
    SdfMotionKeyInterval keyInterval;
    SdfDrawNode *drawNode;
    f32 (*matrix)[4];
    f32 *keyData;

    sdfFindMotionKeyInterval(binding, &keyInterval, frame);
    keyData = keyInterval.firstKey;
    EE_MMI_LOAD_S16X4_FIXED12(vf10, keyData);
    keyData = keyInterval.secondKey;
    EE_MMI_LOAD_S16X4_FIXED12(vf11, keyData);
    effMiscQuaternionNlerpVU(keyInterval.weight);
    drawNode = binding->node;
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, binding->capturedVector);
    effMiscQuaternionNlerpVU(blendWeight);
    VU0_STORE_VF_UNCLOBBERED(vf10, drawNode->quaternion);
    matrix = drawNode->localMatrix;
    effMiscQuaternionToMatrixVU();
    VU0_STORE_VF_UNCLOBBERED(vf28, matrix[0]);
    VU0_STORE_VF_UNCLOBBERED(vf29, matrix[1]);
    VU0_STORE_VF_UNCLOBBERED(vf30, matrix[2]);
}

void *sdfMotionCreateKeyFlagBinding(void *a0, s32 a1, s32 a2) {
    void *r;

    r = sdfAllocSizeClassBlock(0x14);
    sdfMotionBindDrawNode(r, a0, D_00398230, a2);
    return r;
}

void sdfMotionUpdateKeyFlag(SdfMotionKeyFlagBinding *binding, f32 frame) {
    SdfMotionKeyInterval keyInterval;
    SdfDrawNode *node;

    sdfFindMotionKeyInterval(binding, &keyInterval, frame);
    node = binding->node;
    if (*(u8 *)keyInterval.firstKey == 0) {
        node->flags = node->flags | SDF_MOTION_KEY_SAMPLE_BYTE_ZERO;
    } else {
        node->flags = node->flags & ~SDF_MOTION_KEY_SAMPLE_BYTE_ZERO;
    }
}

void func_002DC258(SdfMotionKeyFlagBinding *binding, f32 t) {
    SdfMotionKeyInterval b;
    SdfDrawNode *node;

    sdfFindMotionKeyInterval(binding, &b, t);
    node = binding->node;
    if (*(u8 *)b.firstKey == 0) {
        node->flags = node->flags | SDF_MOTION_KEY_SAMPLE_BYTE_ZERO;
    } else {
        node->flags = node->flags & ~SDF_MOTION_KEY_SAMPLE_BYTE_ZERO;
    }
}

void sdfMotionReadKeyFlag(SdfMotionKeyFlagBinding *binding) {
    binding->keyFlagClear = (s8)(((binding->node->flags >> 4) ^ 1) & 1);
}

void sdfMotionCaptureDrawVector(void *work) {
    SdfMotionDrawBinding *binding = work;
    PCP_COPY_VECTOR(binding->capturedVector, binding->node->translation);
}

void sdfMotionCaptureQuaternion(void *work) {
    SdfMotionDrawBinding *binding = work;
    PCP_COPY_VECTOR(binding->capturedVector, binding->node->quaternion);
}

void sdfMotionCaptureScaleVector(void *work) {
    SdfMotionDrawBinding *binding = work;
    PCP_COPY_VECTOR(binding->capturedVector, binding->node->scale);
}

s32 sdfDispatchMotionHandler(void *a0, s32 a1) {
    return D_00398248[(u16)a1](a0, a1);
}

void sdfMotionBindIndexedTrack(SdfMotionIndexedBinding *binding, Motion *motion,
                               void *dispatch, s32 options) {
    sdfSetMotionPointerPair((SdfMotionBindingHead *)&binding->keys, motion, dispatch);
    binding->target = ((SdfAsset **)motion->owner->resources->buffer)[options];
}

SdfMotionIndexedValueBinding *sdfMotionCreatePrimaryWordSecondBinding(Motion *motion, s32 unused, s32 options) {
    SdfMotionIndexedValueBinding *binding;

    binding = sdfAllocSizeClassBlock(0x14);
    sdfMotionBindIndexedTrack((SdfMotionIndexedBinding *)binding, motion, D_00398270, options);
    return binding;
}

void sdfMotionApplyPrimaryWordSecondKey(SdfMotionIndexedValueBinding *a0, f32 t) {
    SdfMotionKeyInterval b;

    sdfFindMotionKeyInterval(a0, &b, t);
    sdfSetPrimaryStateWordSecond(a0->target, sdfMotionInterpolateKeyColor(&b));
}

/* vu0 routine: blend the captured binding colour toward the keyed colour by weight. */
void sdfMotionBlendPrimaryWordSecondKey(SdfMotionIndexedValueBinding *a0, f32 t, f32 weight) {
    SdfMotionKeyInterval b;
    s32 key;
    s32 color;

    sdfFindMotionKeyInterval(a0, &b, t);
    key = sdfMotionInterpolateKeyColor(&b);
    EE_MMI_RGBA_LERP(color, a0->capturedWord, key, weight, 0.5f);
    sdfSetPrimaryStateWordSecond(a0->target, color);
}

void sdfMotionCapturePrimaryWordSecond(SdfMotionIndexedValueBinding *a0) {
    a0->capturedWord = a0->target->unk14;
}

SdfMotionIndexedValueBinding *sdfMotionCreatePrimaryWordFirstBinding(Motion *motion, s32 unused, s32 options) {
    SdfMotionIndexedValueBinding *binding;

    binding = sdfAllocSizeClassBlock(0x14);
    sdfMotionBindIndexedTrack((SdfMotionIndexedBinding *)binding, motion, D_00398288, options);
    return binding;
}

void sdfMotionApplyPrimaryWordFirstKey(SdfMotionIndexedValueBinding *a0, f32 t) {
    SdfMotionKeyInterval b;

    sdfFindMotionKeyInterval(a0, &b, t);
    sdfSetPrimaryStateWordFirst(a0->target, sdfMotionInterpolateKeyColor(&b));
}

/* vu0 routine: blend the captured binding colour toward the keyed colour by weight. */
void sdfMotionBlendPrimaryWordFirstKey(SdfMotionIndexedValueBinding *a0, f32 t, f32 weight) {
    SdfMotionKeyInterval b;
    s32 key;
    s32 color;

    sdfFindMotionKeyInterval(a0, &b, t);
    key = sdfMotionInterpolateKeyColor(&b);
    EE_MMI_RGBA_LERP(color, a0->capturedWord, key, weight, 0.5f);
    sdfSetPrimaryStateWordFirst(a0->target, color);
}

void sdfMotionCapturePrimaryWordFirst(SdfMotionIndexedValueBinding *a0) {
    a0->capturedWord = a0->target->unk10;
}

SdfMotionIndexedValueBinding *sdfMotionCreatePrimaryWordThirdBinding(Motion *motion, s32 unused, s32 options) {
    SdfMotionIndexedValueBinding *binding;

    binding = sdfAllocSizeClassBlock(0x14);
    sdfMotionBindIndexedTrack((SdfMotionIndexedBinding *)binding, motion, D_003982A0, options);
    return binding;
}

void sdfMotionApplyPrimaryWordThirdKey(SdfMotionIndexedValueBinding *a0, f32 t) {
    SdfMotionKeyInterval b;

    sdfFindMotionKeyInterval(a0, &b, t);
    sdfSetPrimaryStateWordThird(a0->target, sdfMotionInterpolateKeyColor(&b));
}

/* vu0 routine: blend the captured binding colour toward the keyed colour by weight. */
void sdfMotionBlendPrimaryWordThirdKey(SdfMotionIndexedValueBinding *a0, f32 t, f32 weight) {
    SdfMotionKeyInterval b;
    s32 key;
    s32 color;

    sdfFindMotionKeyInterval(a0, &b, t);
    key = sdfMotionInterpolateKeyColor(&b);
    EE_MMI_RGBA_LERP(color, a0->capturedWord, key, weight, 0.5f);
    sdfSetPrimaryStateWordThird(a0->target, color);
}

void sdfMotionCapturePrimaryWordThird(SdfMotionIndexedValueBinding *a0) {
    a0->capturedWord = a0->target->unk20;
}

SdfMotionIndexedValueBinding *sdfMotionCreatePrimaryWordFourthBinding(Motion *motion, s32 unused, s32 options) {
    SdfMotionIndexedValueBinding *binding;

    binding = sdfAllocSizeClassBlock(0x14);
    sdfMotionBindIndexedTrack((SdfMotionIndexedBinding *)binding, motion, D_003982B8, options);
    return binding;
}

void sdfMotionApplyPrimaryWordFourthKey(SdfMotionIndexedValueBinding *a0, f32 t) {
    SdfMotionKeyInterval b;

    sdfFindMotionKeyInterval(a0, &b, t);
    sdfSetPrimaryStateWordFourth(a0->target, sdfMotionInterpolateKeyColor(&b));
}

/* vu0 routine: blend the captured binding colour toward the keyed colour by weight. */
void sdfMotionBlendPrimaryWordFourthKey(SdfMotionIndexedValueBinding *a0, f32 t, f32 weight) {
    SdfMotionKeyInterval b;
    s32 key;
    s32 color;

    sdfFindMotionKeyInterval(a0, &b, t);
    key = sdfMotionInterpolateKeyColor(&b);
    EE_MMI_RGBA_LERP(color, a0->capturedWord, key, weight, 0.5f);
    sdfSetPrimaryStateWordFourth(a0->target, color);
}

void sdfMotionCapturePrimaryWordFourth(SdfMotionIndexedValueBinding *a0) {
    a0->capturedWord = a0->target->unk28;
}

SdfMotionIndexedValueBinding *sdfMotionCreateFloatBinding(Motion *motion, s32 unused, s32 options) {
    SdfMotionIndexedValueBinding *binding;

    binding = sdfAllocSizeClassBlock(0x14);
    sdfMotionBindIndexedTrack((SdfMotionIndexedBinding *)binding, motion, D_003982D0, options);
    return binding;
}

void sdfMotionApplyInterpolatedFloat(SdfMotionIndexedValueBinding *a0, f32 t) {
    SdfMotionKeyInterval b;

    sdfFindMotionKeyInterval(a0, &b, t);
    sdfSetPrimaryStateFloat(a0->target, sdfInterpolateMotionKeys(&b));
}

void sdfMotionBlendInterpolatedFloat(SdfMotionIndexedValueBinding *a0, f32 t1, f32 t2) {
    SdfMotionKeyInterval b;

    sdfFindMotionKeyInterval(a0, &b, t1);
    sdfSetPrimaryStateFloat(a0->target, (a0->capturedFloat + sdfInterpolateMotionKeys(&b) * t2) - (a0->capturedFloat * t2));
}

/* Capture the bound float as the base value for a later blend. */
void sdfMotionReadBoundFloat(SdfMotionIndexedValueBinding *a0) {
    a0->capturedFloat = a0->target->unk1C;
}

SdfMotionIndexedTextBinding *sdfMotionCreateTextBlendBinding(Motion *motion, s32 unused, s32 options) {
    SdfMotionIndexedTextBinding *binding;

    binding = sdfAllocSizeClassBlock(0x24);
    sdfMotionBindIndexedTrack((SdfMotionIndexedBinding *)binding, motion, D_003982E8, options);
    return binding;
}

void sdfMotionApplyFiveFloatKeys(SdfMotionIndexedTextBinding *a0, f32 t) {
    SdfMotionKeyInterval b0;
    f32 b1[5];

    sdfFindMotionKeyInterval(a0, &b0, t);
    sdfMotionBlendFiveKeyValues(&b0, b1);
    sdfCopyPrimaryTextScalars(a0->target, b1);
}

void sdfMotionBlendFiveFloatKeys(SdfMotionIndexedTextBinding *a0, f32 t1, f32 t2) {
    SdfMotionKeyInterval b0;
    f32 b1[5];
    f32 b2[5];

    sdfFindMotionKeyInterval(a0, &b0, t1);
    sdfMotionBlendFiveKeyValues(&b0, b2);
    sdfMotionBlendFiveFloats(b1, a0->capture.capturedValues, b2, t2);
    sdfCopyPrimaryTextScalars(a0->target, b1);
}

/* Save the current primary text scalars for the blend callback. */
void sdfMotionCapturePrimaryTextParams(SdfMotionIndexedTextBinding *binding) {
    Blk *textParams;

    textParams = (Blk *)sdfEnsurePrimaryTextSubParam(binding->target);
    binding->capture.snapshot = *textParams;
}

SdfMotionIndexedTextBinding *sdfMotionCreateSecondaryTextBinding(Motion *motion, s32 unused, s32 options) {
    SdfMotionIndexedTextBinding *binding;

    binding = sdfAllocSizeClassBlock(0x24);
    sdfMotionBindIndexedTrack((SdfMotionIndexedBinding *)binding, motion, D_00398300, options);
    return binding;
}

void sdfMotionApplySecondaryTextKeys(SdfMotionIndexedTextBinding *a0, f32 t) {
    SdfMotionKeyInterval b0;
    f32 b1[5];

    sdfFindMotionKeyInterval(a0, &b0, t);
    sdfMotionBlendFiveKeyValues(&b0, b1);
    sdfCopySecondaryTextScalars(a0->target, b1);
}

void sdfMotionBlendSecondaryTextKeys(SdfMotionIndexedTextBinding *a0, f32 t1, f32 t2) {
    SdfMotionKeyInterval b0;
    f32 b1[5];
    f32 b2[5];

    sdfFindMotionKeyInterval(a0, &b0, t1);
    sdfMotionBlendFiveKeyValues(&b0, b1);
    sdfMotionBlendFiveFloats(b2, a0->capture.capturedValues, b1, t2);
    sdfCopySecondaryTextScalars(a0->target, b2);
}

/* Save the current secondary text scalars for the blend callback. */
void sdfMotionCaptureSecondaryTextParams(SdfMotionIndexedTextBinding *binding) {
    Blk *textParams;

    textParams = (Blk *)sdfEnsureSecondaryTextSubParam(binding->target);
    binding->capture.snapshot = *textParams;
}

SdfMotionIndexedValueBinding *sdfMotionCreateSecondaryColorBinding(Motion *motion, s32 unused,
                                                                  s32 options) {
    SdfMotionIndexedValueBinding *binding;

    binding = sdfAllocSizeClassBlock(0x14);
    sdfMotionBindIndexedTrack((SdfMotionIndexedBinding *)binding, motion, D_00398318, options);
    return binding;
}

void sdfMotionApplySecondaryColorKey(SdfMotionIndexedValueBinding *a0, f32 t) {
    SdfMotionKeyInterval b;

    sdfFindMotionKeyInterval(a0, &b, t);
    sdfSetAssetSecondaryColor(a0->target, sdfMotionInterpolateKeyColor(&b));
}

/* vu0 routine: blend the captured binding colour toward the keyed colour by weight. */
void sdfMotionBlendSecondaryColorKey(SdfMotionIndexedValueBinding *a0, f32 t, f32 weight) {
    SdfMotionKeyInterval b;
    s32 key;
    s32 color;

    sdfFindMotionKeyInterval(a0, &b, t);
    key = sdfMotionInterpolateKeyColor(&b);
    EE_MMI_RGBA_LERP(color, a0->capturedWord, key, weight, 0.5f);
    sdfSetAssetSecondaryColor(a0->target, color);
}

void sdfMotionCaptureSecondaryColor(SdfMotionIndexedValueBinding *a0) {
    a0->capturedWord = a0->target->secondaryColor;
}

SdfMotionIndexedTextBinding *sdfMotionCreateDirectTextKeyBinding(Motion *motion, s32 unused, s32 options) {
    SdfMotionIndexedTextBinding *binding;

    binding = sdfAllocSizeClassBlock(0x24);
    sdfMotionBindIndexedTrack((SdfMotionIndexedBinding *)binding, motion, D_00398330, options);
    return binding;
}

void sdfMotionApplySelectedTextKey(SdfMotionIndexedTextBinding *a0, f32 t) {
    SdfMotionKeyInterval b;

    sdfFindMotionKeyInterval(a0, &b, t);
    sdfCopyPrimaryTextScalars(a0->target, b.firstKey);
}

void sdfMotionApplySampleToTarget(SdfMotionIndexedTextBinding *a0, f32 t) {
    SdfMotionKeyInterval b;

    sdfFindMotionKeyInterval(a0, &b, t);
    sdfCopyPrimaryTextScalars(a0->target, b.firstKey);
}

void func_002DCF60(void) {
}

SdfMotionIndexedTextBinding *sdfMotionCreateSecondaryTextSampleBinding(Motion *motion, s32 unused,
                                                                       s32 options) {
    SdfMotionIndexedTextBinding *binding;

    binding = sdfAllocSizeClassBlock(0x24);
    sdfMotionBindIndexedTrack((SdfMotionIndexedBinding *)binding, motion, D_00398348, options);
    return binding;
}

void sdfMotionApplySampledSecondaryTextValue(SdfMotionIndexedTextBinding *a0, f32 t) {
    SdfMotionKeyInterval b;

    sdfFindMotionKeyInterval(a0, &b, t);
    sdfCopySecondaryTextScalars(a0->target, b.firstKey);
}

void sdfMotionSampleTextScalarsAtTime(SdfMotionIndexedTextBinding *a0, f32 t) {
    SdfMotionKeyInterval b;

    sdfFindMotionKeyInterval(a0, &b, t);
    sdfCopySecondaryTextScalars(a0->target, b.firstKey);
}
