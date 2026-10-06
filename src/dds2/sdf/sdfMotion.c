#include "common.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "sdf.h"
#include "sdf_draw.h"

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

typedef struct {
    void *dispatch; /* 0x00: binding callback table */
    void *source;   /* 0x04: source supplied during binding */
} Pair;

void sdfSetMotionPointerPair(Pair *binding, void *source, void *dispatch);

typedef struct {
    u8 pad[0x30];
    u8 state;
    u8 previousState;
} MotionState;

typedef struct SdfMotionTrack {
    u8 pad00[0x10];
    u32 value10;
    union {
        u32 word;
        u16 flags;
    } value14;
    u8 pad18[8];
    u32 value20;
    u8 pad24[4];
    u32 value28;
} SdfMotionTrack;

typedef struct SdfMotionBinding {
    u8 pad00[0x0C];
    SdfMotionTrack *track;
    union {
        u32 word;
        u8 lowByte;
    } current;
} SdfMotionBinding;

typedef struct SdfMotionKeyInterval {
    f32 *firstKey;
    f32 *secondKey;
    f32 weight;
} SdfMotionKeyInterval;

typedef struct SdfMotionKeyTrack {
    u16 u0;
    u16 u2;
    u16 keyCount;
    u16 keyStride;
    u16 keyFrames[1];
} SdfMotionKeyTrack;

typedef struct SdfMotionManager SdfMotionManager;

struct SdfMotionManager {
    u8 pad00[0x14];
    Motion *head;
};

typedef struct SdfMotionKeyBinding {
    void *dispatch;
    Motion *motion;
    SdfMotionKeyTrack *track;
} SdfMotionKeyBinding;

/* Draw bindings retain a four-component snapshot after their key header. */
typedef struct SdfMotionDrawBinding {
    SdfMotionKeyBinding keys;
    SdfDrawNode *node;
    f32 capturedVector[4];
} SdfMotionDrawBinding;

f32 sdfInterpolateMotionKeys(SdfMotionKeyInterval *a0);
void sdfFindMotionKeyInterval(void *a0, void *out, f32 t);
extern void effMiscQuaternionNlerpVU(f32 amount);
extern void effMiscQuaternionToMatrixVU(void);

extern s32 (*D_0040B368[])(void *a0, s32 a1);

extern s32 (*D_0040B3F8[])(void *a0, s32 a1);

typedef struct {
    s32 u0;
    s32 u4;
    s32 u8;
    s32 *arr;
} ArrHolder;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    ArrHolder *unkC;
} MidPtr;

typedef struct {
    s32 unk0;
    MidPtr *unk4;
} Src360;

typedef struct SdfMotionTarget {
    u8 pad00[0x18];
    u32 value;
} SdfMotionTarget;

typedef struct SdfMotionOutput {
    Pair pair;
    u32 value;
    SdfMotionTarget *target;
    u32 capturedValue; /* 0x10: first word of the saved target state */
} SdfMotionOutput;

void *sdfAllocSizeClassBlock(s32 size);

void sdfMotionBindIndexedTrack(SdfMotionOutput *output, Src360 *source, void *dispatch, s32 options);

extern void *D_0040B420[];

extern void *D_0040B438[];

extern void *D_0040B450[];

extern void *D_0040B468[];

extern void *D_0040B480[];

extern void *D_0040B498[];

extern void *D_0040B4B0[];

extern void *D_0040B4C8[];

extern void *D_0040B4E0[];

extern void *D_0040B4F8[];

typedef struct ArrObj {
    s32 u0;
    s16 objectCount; /* 0x04: number of allocated binding objects */
    s16 pad6;
    s32 u8;
    void **objects; /* 0x0C: binding objects, each beginning with a callback table */
} ArrObj;


void sdfMotionInitialize(Motion *motion, s32 motionIndex, s32 loopEnabled, f32 blendLeadFrames,
                         f32 blendDurationFrames);

typedef struct {
    Pair pair;
    s32 unk8;
    s32 unkC;
} TmpBuf;

typedef struct {
    s32 unk0;
    void *unk4;
} HasPtr4;

s32 sdfModelFindDrawNode(void *a0, s32 a1);

void sdfMotionBindDrawNode(void *tmp, void *src, void *tbl, s32 x);

extern void *D_0040B380[];

extern void *D_0040B398[];

extern void *D_0040B3B0[];

extern void *D_0040B3C8[];

extern void *D_0040B3E0[];

typedef struct {
    s32 w0;
    s32 w4;
    s32 w8;
    s32 wC;
    s32 w10;
    s32 w14;
    s32 w18;
    f32 f1C;
} SubF;

typedef struct {
    s32 u0;
    s32 u4;
    s32 u8;
    SubF *sub;
    f32 capturedValue; /* 0x10: target value saved before blending */
} CmdF;

typedef struct {
    s64 a;
    s64 b;
} __attribute__((packed)) P16;

typedef struct {
    P16 p;
    s32 c;
} Blk;

typedef struct {
    s32 u0;
    s32 u4;
    s32 u8;
    void *sub;
    Blk capturedParams; /* 0x10: saved primary/secondary text scalars */
} DstBlk;

Blk *sdfEnsurePrimaryTextSubParam(void *a0);

Blk *sdfEnsureSecondaryTextSubParam(void *a0);

void sdfInvokeMotionObjectCallback(VObj *object) {
    object->vtable->invoke();
}

/* Install the binding's dispatch table and source without touching its payload. */
void sdfSetMotionPointerPair(Pair *binding, void *source, void *dispatch) {
    binding->dispatch = dispatch;
    binding->source = source;
}


void *sdfAllocAndClearQuadwords(s32 size);
ArrObj *sdfDevCreateBufferedRequest(u16 n, s32 e1, s32 e2);
s32 sdfDispatchAssetCommandWord(void *a0, s32 a1, s32 a2);

Motion *func_003340E0(SdfMotionManager *manager, MotionTable *table) {
    Motion *motion;
    ArrObj *request;
    SdfMotionCommand *command;
    s32 i;
    u16 count;

    motion = sdfAllocAndClearQuadwords(0x34);
    motion->next = manager->head;
    count = table->commandCount;
    manager->head = motion;
    motion->owner = manager;
    motion->motionTable = table;
    request = sdfDevCreateBufferedRequest(count, 4, 8);
    motion->request = request;
    request->objectCount = count;
    for (i = 0, command = motion->motionTable->commands;
         i < count;
         i++, command++) {
        motion->request->objects[i] =
            (void *)sdfDispatchAssetCommandWord(motion, command->command, command->argument);
    }
    motion->state = 0;
    motion->frameStep = 1.0f;
    return motion;
}

void sdfDestroyDevRequest(void *a0);
void sdfReleaseChipBlock(void *a0);


/* Unlinks the node from its owner's list, notifies each request callback, then frees the request and the node. */
void sdfDestroyMotion(Motion *node)
{
    Motion **prev;
    Motion *cur;
    ArrObj *request;
    void **objects;
    s32 objectCount;
    s32 i;

    if (node == NULL) {
        return;
    }
    if (node->owner != NULL) {
        prev = &node->owner->head;
        while ((cur = *prev) != NULL) {
            if (cur == node) {
                *prev = node->next;
                break;
            }
            prev = &cur->next;
        }
    }
    request = node->request;
    objectCount = request->objectCount;
    objects = request->objects;
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
        for (i = 0; i < motion->request->objectCount; i++) {
            object = motion->request->objects[i];
            object->vtable->callback0C(object);
        }
    } else {
        motion->blendDurationFrames = 0.0f;
        motion->blendStartFrame = 0.0f;
    }

    motion->loopEnabled = loopEnabled;
    motion->state = 1;
    motion->currentFrame = motion->blendStartFrame;
    entry = motion->motionTable->entries[motionIndex];
    if (entry == NULL) {
        motion->frameCount = 0;
        motion->state = 5;
        return;
    }

    motion->frameCount = entry->frameCount;
    bindingData = (u8 *)entry->bindingData;
    for (i = 0; i < motion->request->objectCount; i++) {
        object = motion->request->objects[i];
        object->vtable->callback04(object, bindingData);
        bindingData += *(u32 *)bindingData;
    }
}

/* Select a motion with no lead-in and no blend duration. */
void sdfMotionInitializeAtZeroTime(void *a0, s32 a1, s32 a2) {
    sdfMotionInitialize(a0, a1, a2, 0.0f, 0.0f);
}

void sdfMotionSampleAtFrame(Motion *motion, f32 frame) {
    s32 i;
    s32 count;
    f32 elapsed;
    f32 duration;
    f32 weight;
    VObj *object;

    motion->state = 4;
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
        for (i = 0; i < motion->request->objectCount; i++) {
            object = motion->request->objects[i];
            object->vtable->blend(object, frame, weight);
        }
    } else {
        count = motion->request->objectCount;
        for (i = 0; i < count; i++) {
            object = motion->request->objects[i];
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
    case 0:
    case 2:
    case 3:
    case 5:
    case 6:
        break;
    case 1:
        sdfMotionSampleAtFrame(motion, motion->blendStartFrame);
        break;
    case 4:
        frame = motion->currentFrame + motion->frameStep;
        frameCount = motion->frameCount;
        if (motion->loopEnabled == 0) {
            if (frameCount <= frame) {
                sdfMotionSampleAtFrame(motion, frameCount);
                motion->state = 5;
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
void sdfMotionSuspend(MotionState *state) {
    u8 previousState;

    previousState = state->state;
    if (previousState != 6) {
        state->previousState = previousState;
        state->state = 6;
    }
}

/* Resume only a suspended motion, restoring the state saved by suspend. */
void sdfMotionResume(MotionState *state) {
    if (state->state == 6) {
        state->state = state->previousState;
    }
}

void func_00334658(void *a0) {
    sdfReleaseChipBlock(a0);
}

void sdfSetMotionOutputValue(SdfMotionOutput *output, u32 value) {
    output->value = value;
}

/* Find the key interval containing frame and return its interpolation weight. */
void sdfFindMotionKeyInterval(void *arg, void *outArg, f32 frame) {
    SdfMotionKeyBinding *binding;
    SdfMotionKeyInterval *out;
    SdfMotionKeyTrack *track;
    u16 *frames;
    u8 *keyData;
    u8 *firstKey;
    s32 frameNumber;
    s32 first;
    s32 last;
    s32 middle;
    s32 currentFrame;
    s32 nextFrame;
    s32 nextIndex;
    s32 duration;
    u16 stride;
    u16 count;

    binding = arg;
    out = outArg;
    first = 0;
    track = binding->track;
    count = track->keyCount;
    frames = track->keyFrames;
    stride = track->keyStride;
    frameNumber = (s32)frame;
    last = count - 1;
    do {
        middle = (first + last + 1) >> 1;
        currentFrame = frames[middle];
        if (frameNumber < currentFrame) {
            middle--;
            last = middle;
        } else {
            first = middle;
        }
    } while (first < last);

    keyData = (u8 *)&frames[(count + 1) & ~1];
    firstKey = keyData + middle * stride;
    nextIndex = middle + 1;
    currentFrame = frames[middle];
    out->firstKey = (f32 *)firstKey;
    if (nextIndex == count) {
        if (binding->motion->loopEnabled == 0) {
            out->secondKey = (f32 *)firstKey;
            nextFrame = currentFrame;
        } else {
            out->secondKey = (f32 *)keyData;
            nextFrame = binding->motion->frameCount;
        }
    } else {
        nextFrame = frames[nextIndex];
        out->secondKey = (f32 *)(firstKey + stride);
    }

    duration = nextFrame - currentFrame;
    if (duration == 0) {
        out->secondKey = (f32 *)firstKey;
        out->weight = 0.0f;
        return;
    }
    out->weight = (frame - currentFrame) / duration;
}

/* Linear blend of two sampled scalar keys: first + second*t - first*t. */
f32 sdfInterpolateMotionKeys(SdfMotionKeyInterval *output) {
    f32 first;
    f32 weight;

    weight = output->weight;
    first = *output->firstKey;
    return (first + (*output->secondKey * weight)) - (first * weight);
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

s32 sdfDispatchMotionBySelector(void *object, s32 selector) {
    return D_0040B368[(u16)selector](object, selector);
}

void sdfMotionBindDrawNode(void *tmp, void *src, void *tbl, s32 x) {
    sdfSetMotionPointerPair(tmp, src, tbl);
    ((TmpBuf *)tmp)->unkC = sdfModelFindDrawNode(((HasPtr4 *)src)->unk4, x);
}

void *sdfMotionCreateDrawVectorBinding(void *a0, s32 a1, s32 a2) {
    void *r;

    r = sdfAllocSizeClassBlock(0x20);
    sdfMotionBindDrawNode(r, a0, D_0040B380, a2);
    return r;
}

/* vu0 routine: blend the two vec3 keys by the segment weight, store to sub+0x60 */
void sdfMotionBlendDrawVector(SdfMotionDrawBinding *motion, f32 t1) {
    SdfMotionKeyInterval b;

    sdfFindMotionKeyInterval(motion, &b, t1);
    EE_MMI_LOAD_VEC3(vf10, b.firstKey);
    EE_MMI_LOAD_VEC3(vf11, b.secondKey);
        VU0_LERP_VF10_W(b.weight);
    VU0_STORE_VF_UNCLOBBERED(vf10, motion->node->translation);
}

/* vu0 routine: blend the two vec3 keys by the segment weight, then blend that with the vec3 at +0x10 by t2 into sub+0x60 */
void sdfMotionBlendDrawVectorWithCurrent(SdfMotionDrawBinding *motion, f32 t1, f32 t2) {
    SdfMotionKeyInterval b;

    sdfFindMotionKeyInterval(motion, &b, t1);
    EE_MMI_LOAD_VEC3(vf10, b.firstKey);
    EE_MMI_LOAD_VEC3(vf11, b.secondKey);
        VU0_LERP_VF10_COPY(b.weight);
    VU0_LOAD_VF(vf10, motion->capturedVector);
        VU0_LERP_VF10_W(t2);
    VU0_STORE_VF_UNCLOBBERED(vf10, motion->node->translation);
}

void *sdfCreateMotionDrawNode(void *a0, s32 a1, s32 a2) {
    void *r;

    r = sdfAllocSizeClassBlock(0x20);
    sdfMotionBindDrawNode(r, a0, D_0040B398, a2);
    return r;
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00334B70);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00334C30);

void *sdfMotionCreateScaleVectorBinding(void *a0, s32 a1, s32 a2) {
    void *r;

    r = sdfAllocSizeClassBlock(0x20);
    sdfMotionBindDrawNode(r, a0, D_0040B3B0, a2);
    return r;
}

/* vu0 routine: blend the two vec3 keys by the segment weight, store to sub+0x70 */
void sdfMotionBlendScaleVector(SdfMotionDrawBinding *motion, f32 t1) {
    SdfMotionKeyInterval b;

    sdfFindMotionKeyInterval(motion, &b, t1);
    EE_MMI_LOAD_VEC3(vf10, b.firstKey);
    EE_MMI_LOAD_VEC3(vf11, b.secondKey);
        VU0_LERP_VF10_W(b.weight);
    VU0_STORE_VF_UNCLOBBERED(vf10, motion->node->scale);
}

/* vu0 routine: as sdfMotionBlendDrawVectorWithCurrent, stored to sub+0x70 */
void sdfMotionBlendScaleVectorWithCurrent(SdfMotionDrawBinding *motion, f32 t1, f32 t2) {
    SdfMotionKeyInterval b;

    sdfFindMotionKeyInterval(motion, &b, t1);
    EE_MMI_LOAD_VEC3(vf10, b.firstKey);
    EE_MMI_LOAD_VEC3(vf11, b.secondKey);
        VU0_LERP_VF10_COPY(b.weight);
    VU0_LOAD_VF(vf10, motion->capturedVector);
        VU0_LERP_VF10_W(t2);
    VU0_STORE_VF_UNCLOBBERED(vf10, motion->node->scale);
}

void *sdfMotionCreateQuaternionBinding(void *a0, s32 a1, s32 a2) {
    void *r;

    r = sdfAllocSizeClassBlock(0x20);
    sdfMotionBindDrawNode(r, a0, D_0040B3C8, a2);
    return r;
}

/* vu0 routine: nlerp the two quaternion keys by the segment weight, store quaternion and matrix rows */
void sdfMotionBlendQuaternionToMatrix(SdfMotionDrawBinding *motion, f32 t1) {
    SdfMotionKeyInterval b;
    SdfDrawNode *sub;
    f32 (*matrix)[4];
    f32 *key;

    sdfFindMotionKeyInterval(motion, &b, t1);
    key = b.firstKey;
    EE_MMI_LOAD_S16X4_FIXED12(vf10, key);
    key = b.secondKey;
    EE_MMI_LOAD_S16X4_FIXED12(vf11, key);
    effMiscQuaternionNlerpVU(b.weight);
    sub = motion->node;
    VU0_STORE_VF_UNCLOBBERED(vf10, sub->quaternion);
    matrix = sub->localMatrix;
    effMiscQuaternionToMatrixVU();
    VU0_STORE_VF_UNCLOBBERED(vf28, matrix[0]);
    VU0_STORE_VF_UNCLOBBERED(vf29, matrix[1]);
    VU0_STORE_VF_UNCLOBBERED(vf30, matrix[2]);
}

/* vu0 routine: nlerp the two quaternion keys by the segment weight, nlerp that toward the quaternion at +0x10 by t2, store quaternion and matrix rows */
void sdfMotionBlendKeyQuaternionWithBase(SdfMotionDrawBinding *motion, f32 t1, f32 t2) {
    SdfMotionKeyInterval b;
    SdfDrawNode *sub;
    f32 (*matrix)[4];
    f32 *key;

    sdfFindMotionKeyInterval(motion, &b, t1);
    key = b.firstKey;
    EE_MMI_LOAD_S16X4_FIXED12(vf10, key);
    key = b.secondKey;
    EE_MMI_LOAD_S16X4_FIXED12(vf11, key);
    effMiscQuaternionNlerpVU(b.weight);
    sub = motion->node;
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, motion->capturedVector);
    effMiscQuaternionNlerpVU(t2);
    VU0_STORE_VF_UNCLOBBERED(vf10, sub->quaternion);
    matrix = sub->localMatrix;
    effMiscQuaternionToMatrixVU();
    VU0_STORE_VF_UNCLOBBERED(vf28, matrix[0]);
    VU0_STORE_VF_UNCLOBBERED(vf29, matrix[1]);
    VU0_STORE_VF_UNCLOBBERED(vf30, matrix[2]);
}

void *sdfMotionCreateKeyFlagBinding(void *a0, s32 a1, s32 a2) {
    void *r;

    r = sdfAllocSizeClassBlock(0x14);
    sdfMotionBindDrawNode(r, a0, D_0040B3E0, a2);
    return r;
}

void sdfMotionUpdateKeyFlag(SdfMotionBinding *binding, f32 t) {
    SdfMotionKeyInterval b;
    SdfMotionTrack *track;

    sdfFindMotionKeyInterval(binding, &b, t);
    track = binding->track;
    if (*(u8 *)b.firstKey == 0) {
        track->value14.flags = track->value14.flags | 0x10;
    } else {
        track->value14.flags = track->value14.flags & 0xFFEF;
    }
}

void func_00335108(SdfMotionBinding *binding, f32 t) {
    SdfMotionKeyInterval b;
    SdfMotionTrack *track;

    sdfFindMotionKeyInterval(binding, &b, t);
    track = binding->track;
    if (*(u8 *)b.firstKey == 0) {
        track->value14.flags = track->value14.flags | 0x10;
    } else {
        track->value14.flags = track->value14.flags & 0xFFEF;
    }
}

void sdfMotionReadKeyFlag(SdfMotionBinding *binding) {
    binding->current.lowByte = ((u8)(binding->track->value14.flags >> 4) ^ 1) & 1;
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
    return D_0040B3F8[(u16)a1](a0, a1);
}

void sdfMotionBindIndexedTrack(SdfMotionOutput *output, Src360 *source, void *dispatch, s32 options) {
    sdfSetMotionPointerPair(&output->pair, source, dispatch);
    output->target = source->unk4->unkC->arr[options];
}

void *func_00335268(void *source, s32 unused, s32 options) {
    void *motion;

    motion = sdfAllocSizeClassBlock(0x14);
    sdfMotionBindIndexedTrack(motion, source, D_0040B420, options);
    return motion;
}

extern void func_00333288();

void func_003352C8(u8 *motion, f32 t1) {
    u8 buffer[16];
    sdfFindMotionKeyInterval(motion, buffer, t1);
    func_00333288(*(s32 *)(motion + 0xC), sdfMotionInterpolateKeyColor(buffer));
}

/* vu0 routine: blend the captured binding colour toward the keyed colour by weight. */
void func_00335308(SdfMotionBinding *binding, f32 t, f32 weight) {
    SdfMotionKeyInterval b;
    s32 key;
    s32 color;

    sdfFindMotionKeyInterval(binding, &b, t);
    key = sdfMotionInterpolateKeyColor(&b);
    EE_MMI_RGBA_LERP(color, binding->current.word, key, weight, 0.5f);
    func_00333288(binding->track, color);
}

void sdfCopyTrackStateToBinding(SdfMotionBinding *binding) {
    binding->current.word = binding->track->value14.word;
}

void *func_003353C8(void *source, s32 unused, s32 options) {
    void *motion;

    motion = sdfAllocSizeClassBlock(0x14);
    sdfMotionBindIndexedTrack(motion, source, D_0040B438, options);
    return motion;
}

extern void func_00333270();

void func_00335428(u8 *motion, f32 t1) {
    u8 buffer[16];
    sdfFindMotionKeyInterval(motion, buffer, t1);
    func_00333270(*(s32 *)(motion + 0xC), sdfMotionInterpolateKeyColor(buffer));
}

/* vu0 routine: blend the captured binding colour toward the keyed colour by weight. */
void func_00335468(SdfMotionBinding *binding, f32 t, f32 weight) {
    SdfMotionKeyInterval b;
    s32 key;
    s32 color;

    sdfFindMotionKeyInterval(binding, &b, t);
    key = sdfMotionInterpolateKeyColor(&b);
    EE_MMI_RGBA_LERP(color, binding->current.word, key, weight, 0.5f);
    func_00333270(binding->track, color);
}

void sdfMotionReadBoundTrackInteger(SdfMotionBinding *binding) {
    binding->current.word = binding->track->value10;
}

void *func_00335528(void *source, s32 unused, s32 options) {
    void *motion;

    motion = sdfAllocSizeClassBlock(0x14);
    sdfMotionBindIndexedTrack(motion, source, D_0040B450, options);
    return motion;
}

extern void func_003332A0();

void func_00335588(u8 *motion, f32 t1) {
    u8 buffer[16];
    sdfFindMotionKeyInterval(motion, buffer, t1);
    func_003332A0(*(s32 *)(motion + 0xC), sdfMotionInterpolateKeyColor(buffer));
}

/* vu0 routine: blend the captured binding colour toward the keyed colour by weight. */
void func_003355C8(SdfMotionBinding *binding, f32 t, f32 weight) {
    SdfMotionKeyInterval b;
    s32 key;
    s32 color;

    sdfFindMotionKeyInterval(binding, &b, t);
    key = sdfMotionInterpolateKeyColor(&b);
    EE_MMI_RGBA_LERP(color, binding->current.word, key, weight, 0.5f);
    func_003332A0(binding->track, color);
}

void func_00335678(SdfMotionBinding *binding) {
    binding->current.word = binding->track->value20;
}

void *func_00335688(void *source, s32 unused, s32 options) {
    void *motion;

    motion = sdfAllocSizeClassBlock(0x14);
    sdfMotionBindIndexedTrack(motion, source, D_0040B468, options);
    return motion;
}

extern void func_003332B8();

void func_003356E8(u8 *motion, f32 t1) {
    u8 buffer[16];
    sdfFindMotionKeyInterval(motion, buffer, t1);
    func_003332B8(*(s32 *)(motion + 0xC), sdfMotionInterpolateKeyColor(buffer));
}

/* vu0 routine: blend the captured binding colour toward the keyed colour by weight. */
void func_00335728(SdfMotionBinding *binding, f32 t, f32 weight) {
    SdfMotionKeyInterval b;
    s32 key;
    s32 color;

    sdfFindMotionKeyInterval(binding, &b, t);
    key = sdfMotionInterpolateKeyColor(&b);
    EE_MMI_RGBA_LERP(color, binding->current.word, key, weight, 0.5f);
    func_003332B8(binding->track, color);
}

void sdfMotionCopyTrackValueToBinding(SdfMotionBinding *binding) {
    binding->current.word = binding->track->value28;
}

void *sdfMotionCreateFloatBinding(void *source, s32 unused, s32 options) {
    void *motion;

    motion = sdfAllocSizeClassBlock(0x14);
    sdfMotionBindIndexedTrack(motion, source, D_0040B480, options);
    return motion;
}

extern void func_003332D0(s32, f32);

void sdfMotionApplyInterpolatedFloat(u8 *motion, f32 t1) {
    SdfMotionKeyInterval sample;
    sdfFindMotionKeyInterval(motion, &sample, t1);
    func_003332D0(*(s32 *)(motion + 0xC), sdfInterpolateMotionKeys(&sample));
}

extern void func_003332D0(s32, f32);

void sdfMotionBlendInterpolatedFloat(u8 *motion, f32 unused, f32 scale) {
    SdfMotionKeyInterval sample;
    sdfFindMotionKeyInterval(motion, &sample, unused);
    func_003332D0(*(s32 *)(motion + 0xC),
                  *(f32 *)(motion + 0x10) + sdfInterpolateMotionKeys(&sample) * scale - *(f32 *)(motion + 0x10) * scale);
}

/* Capture the bound float as the base value for a later blend. */
void sdfMotionReadBoundFloat(CmdF *a0) {
    a0->capturedValue = a0->sub->f1C;
}

void *sdfMotionCreateTextBlendBinding(void *source, s32 unused, s32 options) {
    void *motion;

    motion = sdfAllocSizeClassBlock(0x24);
    sdfMotionBindIndexedTrack(motion, source, D_0040B498, options);
    return motion;
}

void sdfMotionApplyFiveFloatKeys(SdfMotionOutput *output, f32 t1) {
    u8 keys[16];
    u8 interpolated[32];

    sdfFindMotionKeyInterval(output, keys, t1);
    sdfMotionBlendFiveKeyValues(keys, interpolated);
    sdfCopyPrimaryTextScalars(output->target, interpolated);
}

void sdfMotionBlendFiveFloatKeys(SdfMotionOutput *output, f32 t1, f32 t2) {
    SdfMotionKeyInterval b0;
    f32 b1[5];
    f32 b2[5];

    sdfFindMotionKeyInterval(output, &b0, t1);
    sdfMotionBlendFiveKeyValues(&b0, b2);
    sdfMotionBlendFiveFloats(b1, (f32 *)&output->capturedValue, b2, t2);
    sdfCopyPrimaryTextScalars(output->target, b1);
}

/* Save the current primary text scalars for the blend callback. */
void sdfMotionCapturePrimaryTextParams(DstBlk *a0) {
    Blk *p;

    p = sdfEnsurePrimaryTextSubParam(a0->sub);
    a0->capturedParams = *p;
}

void *sdfMotionCreateSecondaryTextBinding(void *source, s32 unused, s32 options) {
    void *motion;

    motion = sdfAllocSizeClassBlock(0x24);
    sdfMotionBindIndexedTrack(motion, source, D_0040B4B0, options);
    return motion;
}

void sdfMotionApplySecondaryTextKeys(SdfMotionOutput *output, f32 t1) {
    u8 keys[16];
    u8 interpolated[32];

    sdfFindMotionKeyInterval(output, keys, t1);
    sdfMotionBlendFiveKeyValues(keys, interpolated);
    sdfCopySecondaryTextScalars(output->target, interpolated);
}

void sdfMotionBlendSecondaryTextKeys(SdfMotionOutput *output, f32 t1, f32 t2) {
    SdfMotionKeyInterval b0;
    f32 b1[5];
    f32 b2[5];

    sdfFindMotionKeyInterval(output, &b0, t1);
    sdfMotionBlendFiveKeyValues(&b0, b1);
    sdfMotionBlendFiveFloats(b2, (f32 *)&output->capturedValue, b1, t2);
    sdfCopySecondaryTextScalars(output->target, b2);
}

/* Save the current secondary text scalars for the blend callback. */
void sdfMotionCaptureSecondaryTextParams(DstBlk *a0) {
    Blk *p;

    p = sdfEnsureSecondaryTextSubParam(a0->sub);
    a0->capturedParams = *p;
}

void *func_00335BE0(void *source, s32 unused, s32 options) {
    void *motion;

    motion = sdfAllocSizeClassBlock(0x14);
    sdfMotionBindIndexedTrack(motion, source, D_0040B4C8, options);
    return motion;
}

extern void func_00333460();

void sdfMotionApplyInterpolatedKey(u8 *motion, f32 t1) {
    u8 buffer[16];
    sdfFindMotionKeyInterval(motion, buffer, t1);
    func_00333460(*(s32 *)(motion + 0xC), sdfMotionInterpolateKeyColor(buffer));
}

/* vu0 routine: blend the captured output colour toward the keyed colour by weight. */
void func_00335C80(SdfMotionOutput *output, f32 t, f32 weight) {
    SdfMotionKeyInterval b;
    s32 key;
    s32 color;

    sdfFindMotionKeyInterval(output, &b, t);
    key = sdfMotionInterpolateKeyColor(&b);
    EE_MMI_RGBA_LERP(color, output->capturedValue, key, weight, 0.5f);
    func_00333460(output->target, color);
}

void sdfCopyMotionTargetValue(SdfMotionOutput *output) {
    output->capturedValue = output->target->value;
}

void *sdfMotionCreateDirectTextKeyBinding(void *source, s32 unused, s32 options) {
    void *motion;

    motion = sdfAllocSizeClassBlock(0x24);
    sdfMotionBindIndexedTrack(motion, source, D_0040B4E0, options);
    return motion;
}

void sdfMotionApplySelectedTextKey(SdfMotionOutput *output, f32 t1) {
    u32 sample[4];

    sdfFindMotionKeyInterval(output, sample, t1);
    sdfCopyPrimaryTextScalars(output->target, sample[0]);
}

void sdfMotionApplySampleToTarget(SdfMotionOutput *output, f32 t1) {
    u32 sample[4];

    sdfFindMotionKeyInterval(output, sample, t1);
    sdfCopyPrimaryTextScalars(output->target, sample[0]);
}

void func_00335E10(void) {
}

void *func_00335E18(void *source, s32 unused, s32 options) {
    void *motion;

    motion = sdfAllocSizeClassBlock(0x24);
    sdfMotionBindIndexedTrack(motion, source, D_0040B4F8, options);
    return motion;
}

void sdfMotionApplySampledSecondaryTextValue(SdfMotionOutput *output, f32 t1) {
    u32 sample[4];

    sdfFindMotionKeyInterval(output, sample, t1);
    sdfCopySecondaryTextScalars(output->target, sample[0]);
}

void sdfMotionSampleTextScalarsAtTime(SdfMotionOutput *output, f32 t1) {
    u32 sample[4];

    sdfFindMotionKeyInterval(output, sample, t1);
    sdfCopySecondaryTextScalars(output->target, sample[0]);
}
