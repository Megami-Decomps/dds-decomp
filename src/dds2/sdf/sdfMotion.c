#include "sdf_request.h"
#include "common.h"
#include "sdf_vu0.h"
#include "sdf_asset_state.h"
#include "sdf_chip.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "sdf.h"
#include "sdf_draw.h"
#include "sdf_motion_bindings.h"
#include "sdf_motion.h"
#include "sdf_motion_node_api.h"
#include "sdf_vu_matrix.h"
#include "eff_quaternion_nlerp.h"





f32 sdfMotInterpolateKeyFloat(SdfMotionKeyInterval *interval);

extern s32 (*D_0040B368[])(void *binding, s32 selector, s32 targetIndex);

extern s32 (*D_0040B3F8[])(void *handlerContext, s32 handlerIndex, s32 targetIndex);


extern SdfMotionBindingDispatch D_0040B420;

extern SdfMotionBindingDispatch D_0040B438;

extern SdfMotionBindingDispatch D_0040B450;

extern SdfMotionBindingDispatch D_0040B468;

extern SdfMotionBindingDispatch D_0040B480;

extern SdfMotionBindingDispatch D_0040B498;

extern SdfMotionBindingDispatch D_0040B4B0;

extern SdfMotionBindingDispatch D_0040B4C8;

extern SdfMotionBindingDispatch D_0040B4E0;

extern SdfMotionBindingDispatch D_0040B4F8;






extern SdfMotionBindingDispatch D_0040B380;

extern SdfMotionBindingDispatch D_0040B398;

extern SdfMotionBindingDispatch D_0040B3B0;

extern SdfMotionBindingDispatch D_0040B3C8;

extern SdfMotionBindingDispatch D_0040B3E0;

/* Release the allocation retained by this motion binding. */
void sdfMotionNodeFree(SdfMotionBindingHead *binding) {
    binding->dispatch->releaseStorage(binding);
}

/* Install the binding's dispatch table and motion without touching its payload. */
void sdfMotionNodeBaseInit(SdfMotionBindingHead *binding, sdfMotionCtrlr_t *motion, SdfMotionBindingDispatch *dispatch) {
    binding->dispatch = dispatch;
    binding->motion = motion;
}



sdfMotionCtrlr_t *sdfMotionCtrlrNew(sdfModelRootNode_t *model, MotionTable *table) {
    sdfMotionCtrlr_t *motion;
    DevRequest *request;
    SdfMotionCommand *command;
    s32 commandIndex;
    u16 commandCount;

    motion = sdfAllocAndClearQuadwords(sizeof(sdfMotionCtrlr_t));
    motion->next = model->motionList;
    commandCount = table->commandCount;
    model->motionList = motion;
    motion->owner = model;
    motion->motionTable = table;
    request = sdfDevCreateBufferedRequest(commandCount, 4, 8);
    motion->request = request;
    request->usedCount = commandCount;
    for (commandIndex = 0, command = motion->motionTable->commands;
         commandIndex < commandCount;
         commandIndex++, command++) {
        ((SdfMotionBindingHead **)motion->request->buffer)[commandIndex] =
            sdfMotionNodeNew(motion, command->packedCommand, command->targetIndex);
    }
    motion->state = SDF_MOTION_STATE_UNINITIALIZED;
    motion->frameStep = 1.0f;
    return motion;
}



/* Unlinks the node from its owner's list, notifies each request callback, then frees the request and the node. */
void sdfMotionCtrlrFree(sdfMotionCtrlr_t *node)
{
    sdfMotionCtrlr_t **prev;
    sdfMotionCtrlr_t *cur;
    DevRequest *request;
    SdfMotionBindingHead **bindings;
    s32 bindingCount;
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
    bindingCount = request->usedCount;
    bindings = (SdfMotionBindingHead **)request->buffer;
    for (i = 0; i < bindingCount; i++) {
        sdfMotionNodeFree(bindings[i]);
    }
    sdfItemArrayFree(node->request);
    sdfChipFreeDirect(node);
}

/* Select a motion entry, initialize its timing and state, and bind its setup records. */
void sdfMotionChangeBlend(sdfMotionCtrlr_t *motion, s32 motionIndex, s32 loopEnabled, f32 blendLeadFrames,
                         f32 blendDurationFrames) {
    s32 i;
    const MotionEntry *entry;
    u8 *bindingData;
    SdfMotionBindingHead *binding;

    motion->motionIndex = motionIndex;
    if (blendDurationFrames > 0.0f) {
        motion->blendDurationFrames = blendDurationFrames;
        motion->blendStartFrame = -blendLeadFrames;
        for (i = 0; i < motion->request->usedCount; i++) {
            binding = ((SdfMotionBindingHead **)motion->request->buffer)[i];
            binding->dispatch->capturePrevious(binding);
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
        binding = ((SdfMotionBindingHead **)motion->request->buffer)[i];
        binding->dispatch->bindKeyTrack(binding, (SdfMotionKeyTrack *)bindingData);
        bindingData += ((SdfMotionKeyTrack *)bindingData)->nextTrackOffsetBytes;
    }
}

/* Select a motion with no lead-in and no blend duration. */
void sdfMotionChange(sdfMotionCtrlr_t *motion, s32 motionIndex, s32 loopEnabled) {
    sdfMotionChangeBlend(motion, motionIndex, loopEnabled, 0.0f, 0.0f);
}

void sdfMotionSeek(sdfMotionCtrlr_t *motion, f32 frame) {
    s32 i;
    s32 count;
    f32 elapsed;
    f32 duration;
    f32 weight;
    SdfMotionBindingHead *binding;

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
            binding = ((SdfMotionBindingHead **)motion->request->buffer)[i];
            binding->dispatch->blend(binding, frame, weight);
        }
    } else {
        count = motion->request->usedCount;
        for (i = 0; i < count; i++) {
            binding = ((SdfMotionBindingHead **)motion->request->buffer)[i];
            binding->dispatch->sample(binding, frame);
        }
    }
}

/* Apply the current motion state and report completion (1) or a loop wrap (2). */
s32 sdfMotionNext(sdfMotionCtrlr_t *motion) {
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
        sdfMotionSeek(motion, motion->blendStartFrame);
        break;
    case SDF_MOTION_STATE_SAMPLING:
        frame = motion->currentFrame + motion->frameStep;
        frameCount = motion->frameCount;
        if (motion->loopEnabled == 0) {
            if (frameCount <= frame) {
                sdfMotionSeek(motion, frameCount);
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
        sdfMotionSeek(motion, frame);
        break;
    default:
        break;
    }
    return result;
}

/* State 6 parks motion processing, retaining the previous state to resume. */
void sdfMotionPause(sdfMotionCtrlr_t *motion) {
    u8 previousState;

    previousState = motion->state;
    if (previousState != SDF_MOTION_STATE_SUSPENDED) {
        motion->previousState = previousState;
        motion->state = SDF_MOTION_STATE_SUSPENDED;
    }
}

/* Resume only a suspended motion, restoring the state saved by suspend. */
void sdfMotionPauseOff(sdfMotionCtrlr_t *motion) {
    if (motion->state == SDF_MOTION_STATE_SUSPENDED) {
        motion->state = motion->previousState;
    }
}

void sdfMotCLBK_Destroy(void *storage) {
    sdfChipFreeDirect(storage);
}

void sdfMotCLBK_SetCut(SdfMotionKeyBinding *binding, SdfMotionKeyTrack *track) {
    binding->track = track;
}

/* Find the key interval containing frame and return its interpolation weight. */
void sdfMotGetKey(SdfMotionKeyBinding *binding,
                              SdfMotionKeyInterval *out, f32 frame) {
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
f32 sdfMotInterpolateKeyFloat(SdfMotionKeyInterval *interval) {
    f32 firstValue;
    f32 interpolationWeight;

    interpolationWeight = interval->weight;
    firstValue = *interval->firstKey;
    return (firstValue + (*interval->secondKey * interpolationWeight)) -
           (firstValue * interpolationWeight);
}

/* vu0 routine: blend the two bracketing vec3 keys by the key weight into vf10. */
void sdfMotInterpolateKeyXYZ(SdfMotionKeyInterval *interval) {
    VU0_LERP_VEC3_KEYS(interval->firstKey, interval->secondKey, interval->weight);
}

/* vu0 routine: blend the two bracketing RGBA colour keys by the key weight. */
u32 sdfMotInterpolateKeyColor(SdfMotionKeyInterval *interval) {
    u32 color;

    EE_MMI_RGBA_LERP(color, *(u32 *)interval->firstKey, *(u32 *)interval->secondKey, interval->weight, 0.5f);
    return color;
}

/* Interpolate exactly five scalar channels, retaining the retail arithmetic order. */
void sdfMotInterpolateKeyUvwSub(f32 *dst, f32 *src1, f32 *src2, f32 weight) {
    s32 channelIndex;
    f32 inverseWeight;

    channelIndex = 0;
    inverseWeight = 1.0f - weight;
    do {
        dst[channelIndex] = src1[channelIndex] * inverseWeight + src2[channelIndex] * weight;
        channelIndex++;
    } while (channelIndex != 5);
}

void sdfMotInterpolateKeyUvw(SdfMotionKeyInterval *src, f32 *dst) {
    sdfMotInterpolateKeyUvwSub(dst, src->firstKey, src->secondKey, src->weight);
}

s32 sdfMotGenNode(void *binding, s32 selector, s32 targetIndex) {
    return D_0040B368[(u16)selector](binding, selector, targetIndex);
}

void sdfMotGenModelInit(SdfMotionDrawTargetBinding *binding, sdfMotionCtrlr_t *motion,
                          SdfMotionBindingDispatch *dispatch, s32 nodeIndex) {
    sdfModelNode_t *drawNode;

    sdfMotionNodeBaseInit((SdfMotionBindingHead *)&binding->keys, motion, dispatch);
    drawNode = sdfModelSearchNode(motion->owner, nodeIndex);
    binding->node = drawNode;
}

SdfMotionDrawBinding *sdfMotGenModelLinerPos(sdfMotionCtrlr_t *motion, s32 unused, s32 nodeIndex) {
    SdfMotionDrawBinding *binding;

    binding = sdfChipAllocDirect(0x20);
    sdfMotGenModelInit((SdfMotionDrawTargetBinding *)binding, motion, &D_0040B380, nodeIndex);
    return binding;
}

/* vu0 routine: interpolate the sampled translation keys into the draw node. */
void sdfMotCLBK_modelLinerPosSeek(SdfMotionDrawBinding *binding, f32 frame) {
    SdfMotionKeyInterval keyInterval;

    sdfMotGetKey((SdfMotionKeyBinding *)binding, &keyInterval, frame);
    EE_MMI_LOAD_VEC3(vf10, keyInterval.firstKey);
    SDF_VU0_LOADVECTOR_UNALIGN_B(keyInterval.secondKey);
        VU0_LERP_VF10_W(keyInterval.weight);
    SDF_VU0_STOREVECTOR_NOMEM(binding->node->translation);
}

/* vu0 routine: interpolate translation keys, then blend with captured translation. */
void sdfMotCLBK_modelLinerPosBlendSeek(SdfMotionDrawBinding *binding, f32 frame, f32 blendWeight) {
    SdfMotionKeyInterval keyInterval;

    sdfMotGetKey((SdfMotionKeyBinding *)binding, &keyInterval, frame);
    EE_MMI_LOAD_VEC3(vf10, keyInterval.firstKey);
    SDF_VU0_LOADVECTOR_UNALIGN_B(keyInterval.secondKey);
        VU0_LERP_VF10_COPY(keyInterval.weight);
    SDF_VU0_LOADVECTOR(binding->capturedVector);
        VU0_LERP_VF10_W(blendWeight);
    SDF_VU0_STOREVECTOR_NOMEM(binding->node->translation);
}

SdfMotionDrawBinding *sdfMotGenModelLinerRotXYZ(sdfMotionCtrlr_t *motion, s32 unused, s32 nodeIndex) {
    SdfMotionDrawBinding *binding;

    binding = sdfChipAllocDirect(0x20);
    sdfMotGenModelInit((SdfMotionDrawTargetBinding *)binding, motion, &D_0040B398, nodeIndex);
    return binding;
}

/* vu0 routine: interpolate the Euler-angle keys, then rebuild the node's quaternion
 * and rotation rows. The angle vector is read back only through the stack, so it
 * is volatile (retail reloads it in order and leaves the call's delay slot empty). */
void sdfMotCLBK_modelLinerRotXYZSeek(SdfMotionDrawBinding *binding, f32 frame) {
    SdfMotionKeyInterval keyInterval;
    volatile f32 angles[4] __attribute__((aligned(16)));
    sdfModelNode_t *drawNode;
    f32 (*matrix)[4];

    sdfMotGetKey((SdfMotionKeyBinding *)binding, &keyInterval, frame);
    EE_MMI_LOAD_VEC3(vf10, keyInterval.firstKey);
    SDF_VU0_LOADVECTOR_UNALIGN_B(keyInterval.secondKey);
    VU0_LERP_VF10_CLEAR_W(keyInterval.weight);
    SDF_VU0_STOREVECTOR((f32 *)angles);
    sdfConvertEulerAnglesToQuaternionVU(angles[0], angles[1], angles[2]);
    drawNode = binding->node;
    SDF_VU0_STOREVECTOR(drawNode->quaternion);
    matrix = drawNode->localMatrix;
    effMiscQuaternionToMatrixVU();
    SDF_VU0_STORE_ROW0_NOMEM(matrix[0]);
    SDF_VU0_STORE_ROW1_NOMEM(matrix[1]);
    SDF_VU0_STORE_ROW2_NOMEM(matrix[2]);
}

/* vu0 routine: as above, then nlerp the result with the captured quaternion. */
void sdfMotCLBK_modelLinerRotXYZBlendSeek(SdfMotionDrawBinding *binding, f32 frame, f32 blendWeight) {
    SdfMotionKeyInterval keyInterval;
    volatile f32 angles[4] __attribute__((aligned(16)));
    sdfModelNode_t *drawNode;
    f32 (*matrix)[4];

    sdfMotGetKey((SdfMotionKeyBinding *)binding, &keyInterval, frame);
    EE_MMI_LOAD_VEC3(vf10, keyInterval.firstKey);
    SDF_VU0_LOADVECTOR_UNALIGN_B(keyInterval.secondKey);
    VU0_LERP_VF10_CLEAR_W(keyInterval.weight);
    SDF_VU0_STOREVECTOR((f32 *)angles);
    sdfConvertEulerAnglesToQuaternionVU(angles[0], angles[1], angles[2]);
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(binding->capturedVector);
    Lerp(blendWeight);
    drawNode = binding->node;
    SDF_VU0_STOREVECTOR(drawNode->quaternion);
    matrix = drawNode->localMatrix;
    effMiscQuaternionToMatrixVU();
    SDF_VU0_STORE_ROW0_NOMEM(matrix[0]);
    SDF_VU0_STORE_ROW1_NOMEM(matrix[1]);
    SDF_VU0_STORE_ROW2_NOMEM(matrix[2]);
}

SdfMotionDrawBinding *sdfMotGenModelLinerScale(sdfMotionCtrlr_t *motion, s32 unused, s32 nodeIndex) {
    SdfMotionDrawBinding *binding;

    binding = sdfChipAllocDirect(0x20);
    sdfMotGenModelInit((SdfMotionDrawTargetBinding *)binding, motion, &D_0040B3B0, nodeIndex);
    return binding;
}

/* vu0 routine: interpolate the sampled scale keys into the draw node. */
void sdfMotCLBK_modelLinerScaleSeek(SdfMotionDrawBinding *binding, f32 frame) {
    SdfMotionKeyInterval keyInterval;

    sdfMotGetKey((SdfMotionKeyBinding *)binding, &keyInterval, frame);
    EE_MMI_LOAD_VEC3(vf10, keyInterval.firstKey);
    SDF_VU0_LOADVECTOR_UNALIGN_B(keyInterval.secondKey);
        VU0_LERP_VF10_W(keyInterval.weight);
    SDF_VU0_STOREVECTOR_NOMEM(binding->node->scale);
}

/* vu0 routine: interpolate scale keys, then blend with captured scale. */
void sdfMotCLBK_modelLinerScaleBlendSeek(SdfMotionDrawBinding *binding, f32 frame, f32 blendWeight) {
    SdfMotionKeyInterval keyInterval;

    sdfMotGetKey((SdfMotionKeyBinding *)binding, &keyInterval, frame);
    EE_MMI_LOAD_VEC3(vf10, keyInterval.firstKey);
    SDF_VU0_LOADVECTOR_UNALIGN_B(keyInterval.secondKey);
        VU0_LERP_VF10_COPY(keyInterval.weight);
    SDF_VU0_LOADVECTOR(binding->capturedVector);
        VU0_LERP_VF10_W(blendWeight);
    SDF_VU0_STOREVECTOR(binding->node->scale);
}

SdfMotionDrawBinding *sdfMotGenModelLerpRotQuat(sdfMotionCtrlr_t *motion, s32 unused, s32 nodeIndex) {
    SdfMotionDrawBinding *binding;

    binding = sdfChipAllocDirect(0x20);
    sdfMotGenModelInit((SdfMotionDrawTargetBinding *)binding, motion, &D_0040B3C8, nodeIndex);
    return binding;
}

/* vu0 routine: nlerp sampled quaternion keys, then store the quaternion and matrix. */
void sdfMotCLBK_modelLerpRotQuatSeek(SdfMotionDrawBinding *binding, f32 frame) {
    SdfMotionKeyInterval keyInterval;
    sdfModelNode_t *drawNode;
    f32 (*matrix)[4];
    f32 *keyData;

    sdfMotGetKey((SdfMotionKeyBinding *)binding, &keyInterval, frame);
    keyData = keyInterval.firstKey;
    SDF_VU0_LOADSHORTVECTOR(keyData);
    keyData = keyInterval.secondKey;
    SDF_VU0_LOADSHORTVECTOR_B(keyData);
    Lerp(keyInterval.weight);
    drawNode = binding->node;
    SDF_VU0_STOREVECTOR_NOMEM(drawNode->quaternion);
    matrix = drawNode->localMatrix;
    effMiscQuaternionToMatrixVU();
    SDF_VU0_STORE_ROW0_NOMEM(matrix[0]);
    SDF_VU0_STORE_ROW1_NOMEM(matrix[1]);
    SDF_VU0_STORE_ROW2_NOMEM(matrix[2]);
}

/* vu0 routine: nlerp sampled keys with the captured quaternion, then build the matrix. */
void sdfMotCLBK_modelLerpRotQuatBlendSeek(SdfMotionDrawBinding *binding, f32 frame, f32 blendWeight) {
    SdfMotionKeyInterval keyInterval;
    sdfModelNode_t *drawNode;
    f32 (*matrix)[4];
    f32 *keyData;

    sdfMotGetKey((SdfMotionKeyBinding *)binding, &keyInterval, frame);
    keyData = keyInterval.firstKey;
    SDF_VU0_LOADSHORTVECTOR(keyData);
    keyData = keyInterval.secondKey;
    SDF_VU0_LOADSHORTVECTOR_B(keyData);
    Lerp(keyInterval.weight);
    drawNode = binding->node;
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(binding->capturedVector);
    Lerp(blendWeight);
    SDF_VU0_STOREVECTOR_NOMEM(drawNode->quaternion);
    matrix = drawNode->localMatrix;
    effMiscQuaternionToMatrixVU();
    SDF_VU0_STORE_ROW0_NOMEM(matrix[0]);
    SDF_VU0_STORE_ROW1_NOMEM(matrix[1]);
    SDF_VU0_STORE_ROW2_NOMEM(matrix[2]);
}

SdfMotionKeyFlagBinding *sdfMotGenModelVisible(sdfMotionCtrlr_t *motion, s32 unused, s32 nodeIndex) {
    SdfMotionKeyFlagBinding *binding;

    binding = sdfChipAllocDirect(0x14);
    sdfMotGenModelInit((SdfMotionDrawTargetBinding *)binding, motion, &D_0040B3E0, nodeIndex);
    return binding;
}

void sdfMotCLBK_modelVisibleSeek(SdfMotionKeyFlagBinding *binding, f32 frame) {
    SdfMotionKeyInterval keyInterval;
    sdfModelNode_t *node;

    sdfMotGetKey((SdfMotionKeyBinding *)binding, &keyInterval, frame);
    node = binding->node;
    if (*(u8 *)keyInterval.firstKey == 0) {
        node->flags = node->flags | SDF_MOTION_KEY_SAMPLE_BYTE_ZERO;
    } else {
        node->flags = node->flags & ~SDF_MOTION_KEY_SAMPLE_BYTE_ZERO;
    }
}

/* The blend slot resamples the discrete flag; it does not consume blend weight. */
void sdfMotCLBK_modelVisibleBlendSeek(SdfMotionKeyFlagBinding *binding, f32 frame) {
    SdfMotionKeyInterval keyInterval;
    sdfModelNode_t *node;

    sdfMotGetKey((SdfMotionKeyBinding *)binding, &keyInterval, frame);
    node = binding->node;
    if (*(u8 *)keyInterval.firstKey == 0) {
        node->flags = node->flags | SDF_MOTION_KEY_SAMPLE_BYTE_ZERO;
    } else {
        node->flags = node->flags & ~SDF_MOTION_KEY_SAMPLE_BYTE_ZERO;
    }
}

void sdfMotCLBK_modelVisibleInitBlend(SdfMotionKeyFlagBinding *binding) {
    binding->keyFlagClear = ((u8)(binding->node->flags >> 4) ^ 1) & 1;
}

void sdfMotCLBK_modelPosInitBlend(SdfMotionDrawBinding *binding) {
    SDF_VU0_COPYVECTOR(binding->capturedVector, binding->node->translation);
}

void sdfMotCLBK_modelRotInitBlend(SdfMotionDrawBinding *binding) {
    SDF_VU0_COPYVECTOR(binding->capturedVector, binding->node->quaternion);
}

void sdfMotCLBK_modelScaleInitBlend(SdfMotionDrawBinding *binding) {
    SDF_VU0_COPYVECTOR(binding->capturedVector, binding->node->scale);
}

s32 sdfMotGenMatNode(void *handlerContext, s32 handlerIndex, s32 targetIndex) {
    return D_0040B3F8[(u16)handlerIndex](handlerContext, handlerIndex, targetIndex);
}

void sdfMotGenMatInit(SdfMotionIndexedBinding *binding, sdfMotionCtrlr_t *motion,
                               SdfMotionBindingDispatch *dispatch, s32 options) {
    sdfMotionNodeBaseInit((SdfMotionBindingHead *)&binding->keys, motion, dispatch);
    binding->target = ((sdfMat_t **)motion->owner->resources->buffer)[options];
}

SdfMotionIndexedValueBinding *sdfMotGenMatDiffuse(sdfMotionCtrlr_t *motion, s32 unused, s32 options) {
    SdfMotionIndexedValueBinding *binding;

    binding = sdfChipAllocDirect(0x14);
    sdfMotGenMatInit((SdfMotionIndexedBinding *)binding, motion, &D_0040B420, options);
    return binding;
}

void sdfMotCLBK_matDiffuseSeek(SdfMotionIndexedValueBinding *binding, f32 frame) {
    u8 buffer[16];
    sdfMotGetKey((SdfMotionKeyBinding *)binding, (SdfMotionKeyInterval *)buffer, frame);
    sdfMatSetDiffuse(binding->target, sdfMotInterpolateKeyColor(buffer));
}

/* vu0 routine: blend the captured binding colour toward the keyed colour by weight. */
void sdfMotCLBK_matDiffuseBlendSeek(SdfMotionIndexedValueBinding *binding, f32 frame, f32 weight) {
    SdfMotionKeyInterval keyInterval;
    u32 key;
    u32 color;

    sdfMotGetKey((SdfMotionKeyBinding *)binding, &keyInterval, frame);
    key = sdfMotInterpolateKeyColor(&keyInterval);
    EE_MMI_RGBA_LERP(color, binding->capturedWord, key, weight, 0.5f);
    sdfMatSetDiffuse(binding->target, color);
}

void sdfMotCLBK_matDiffuseInitBlend(SdfMotionIndexedValueBinding *binding) {
    binding->capturedWord = binding->target->primaryStateWordSecond;
}

SdfMotionIndexedValueBinding *sdfMotGenMatAmbient(sdfMotionCtrlr_t *motion, s32 unused, s32 options) {
    SdfMotionIndexedValueBinding *binding;

    binding = sdfChipAllocDirect(0x14);
    sdfMotGenMatInit((SdfMotionIndexedBinding *)binding, motion, &D_0040B438, options);
    return binding;
}

void sdfMotCLBK_matAmbientSeek(SdfMotionIndexedValueBinding *binding, f32 frame) {
    u8 buffer[16];
    sdfMotGetKey((SdfMotionKeyBinding *)binding, (SdfMotionKeyInterval *)buffer, frame);
    sdfMatSetAmbient(binding->target, sdfMotInterpolateKeyColor(buffer));
}

/* vu0 routine: blend the captured binding colour toward the keyed colour by weight. */
void sdfMotCLBK_matAmbientBlendSeek(SdfMotionIndexedValueBinding *binding, f32 frame, f32 weight) {
    SdfMotionKeyInterval keyInterval;
    u32 key;
    u32 color;

    sdfMotGetKey((SdfMotionKeyBinding *)binding, &keyInterval, frame);
    key = sdfMotInterpolateKeyColor(&keyInterval);
    EE_MMI_RGBA_LERP(color, binding->capturedWord, key, weight, 0.5f);
    sdfMatSetAmbient(binding->target, color);
}

void sdfMotCLBK_matAmbientInitBlend(SdfMotionIndexedValueBinding *binding) {
    binding->capturedWord = binding->target->primaryStateWordFirst;
}

SdfMotionIndexedValueBinding *sdfMotGenMatSpecular(sdfMotionCtrlr_t *motion, s32 unused, s32 options) {
    SdfMotionIndexedValueBinding *binding;

    binding = sdfChipAllocDirect(0x14);
    sdfMotGenMatInit((SdfMotionIndexedBinding *)binding, motion, &D_0040B450, options);
    return binding;
}

void sdfMotCLBK_matSpecularSeek(SdfMotionIndexedValueBinding *binding, f32 frame) {
    u8 buffer[16];
    sdfMotGetKey((SdfMotionKeyBinding *)binding, (SdfMotionKeyInterval *)buffer, frame);
    sdfMatSetSpecular(binding->target, sdfMotInterpolateKeyColor(buffer));
}

/* vu0 routine: blend the captured binding colour toward the keyed colour by weight. */
void sdfMotCLBK_matSpecularBlendSeek(SdfMotionIndexedValueBinding *binding, f32 frame, f32 weight) {
    SdfMotionKeyInterval keyInterval;
    u32 key;
    u32 color;

    sdfMotGetKey((SdfMotionKeyBinding *)binding, &keyInterval, frame);
    key = sdfMotInterpolateKeyColor(&keyInterval);
    EE_MMI_RGBA_LERP(color, binding->capturedWord, key, weight, 0.5f);
    sdfMatSetSpecular(binding->target, color);
}

void sdfMotCLBK_matSpecularInitBlend(SdfMotionIndexedValueBinding *binding) {
    binding->capturedWord = binding->target->primaryStateWordThird;
}

SdfMotionIndexedValueBinding *sdfMotGenMatRefMap(sdfMotionCtrlr_t *motion, s32 unused, s32 options) {
    SdfMotionIndexedValueBinding *binding;

    binding = sdfChipAllocDirect(0x14);
    sdfMotGenMatInit((SdfMotionIndexedBinding *)binding, motion, &D_0040B468, options);
    return binding;
}

void sdfMotCLBK_matRefMapSeek(SdfMotionIndexedValueBinding *binding, f32 frame) {
    u8 buffer[16];
    sdfMotGetKey((SdfMotionKeyBinding *)binding, (SdfMotionKeyInterval *)buffer, frame);
    sdfSetPrimaryStateWordFourth(binding->target, sdfMotInterpolateKeyColor(buffer));
}

/* vu0 routine: blend the captured binding colour toward the keyed colour by weight. */
void sdfMotCLBK_matRefMapBlendSeek(SdfMotionIndexedValueBinding *binding, f32 frame, f32 weight) {
    SdfMotionKeyInterval keyInterval;
    u32 key;
    u32 color;

    sdfMotGetKey((SdfMotionKeyBinding *)binding, &keyInterval, frame);
    key = sdfMotInterpolateKeyColor(&keyInterval);
    EE_MMI_RGBA_LERP(color, binding->capturedWord, key, weight, 0.5f);
    sdfSetPrimaryStateWordFourth(binding->target, color);
}

void sdfMotCLBK_matRefMapInitBlend(SdfMotionIndexedValueBinding *binding) {
    binding->capturedWord = binding->target->primaryStateWordFourth;
}

SdfMotionIndexedValueBinding *sdfMotGenMatConstant(sdfMotionCtrlr_t *motion, s32 unused, s32 options) {
    SdfMotionIndexedValueBinding *binding;

    binding = sdfChipAllocDirect(0x14);
    sdfMotGenMatInit((SdfMotionIndexedBinding *)binding, motion, &D_0040B480, options);
    return binding;
}

void sdfMotCLBK_matConstantSeek(SdfMotionIndexedValueBinding *binding, f32 frame) {
    SdfMotionKeyInterval sample;
    sdfMotGetKey((SdfMotionKeyBinding *)binding, &sample, frame);
    sdfMatSetConstant(binding->target, sdfMotInterpolateKeyFloat(&sample));
}

void sdfMotCLBK_matConstantBlendSeek(SdfMotionIndexedValueBinding *binding, f32 unused, f32 scale) {
    SdfMotionKeyInterval sample;
    sdfMotGetKey((SdfMotionKeyBinding *)binding, &sample, unused);
    sdfMatSetConstant(binding->target,
                  binding->capturedFloat + sdfMotInterpolateKeyFloat(&sample) * scale -
                      binding->capturedFloat * scale);
}

/* Capture the bound float as the base value for a later blend. */
void sdfMotCLBK_matConstantInitBlend(SdfMotionIndexedValueBinding *binding) {
    binding->capturedFloat = binding->target->primaryStateFloat;
}

SdfMotionIndexedTextBinding *sdfMotGenMatUvw(sdfMotionCtrlr_t *motion, s32 unused, s32 options) {
    SdfMotionIndexedTextBinding *binding;

    binding = sdfChipAllocDirect(0x24);
    sdfMotGenMatInit((SdfMotionIndexedBinding *)binding, motion, &D_0040B498, options);
    return binding;
}

void sdfMotCLBK_matUvwSeek(SdfMotionIndexedTextBinding *binding, f32 frame) {
    u8 keys[16];
    u8 interpolated[32];

    sdfMotGetKey((SdfMotionKeyBinding *)binding, (SdfMotionKeyInterval *)keys, frame);
    sdfMotInterpolateKeyUvw(keys, interpolated);
    sdfMatSetTexUvwByPtr(binding->target, (const f32 *)interpolated);
}

void sdfMotCLBK_matUvwBlendSeek(SdfMotionIndexedTextBinding *binding, f32 frame, f32 blendWeight) {
    SdfMotionKeyInterval keyInterval;
    f32 b1[5];
    f32 b2[5];

    sdfMotGetKey((SdfMotionKeyBinding *)binding, &keyInterval, frame);
    sdfMotInterpolateKeyUvw(&keyInterval, b2);
    sdfMotInterpolateKeyUvwSub(b1, binding->capture.capturedValues, b2, blendWeight);
    sdfMatSetTexUvwByPtr(binding->target, b1);
}

/* Save the current primary text scalars for the blend callback. */
void sdfMotCLBK_matUvwInitBlend(SdfMotionIndexedTextBinding *binding) {
    SdfMotionTextParamSnapshot *textParams;

    textParams = (SdfMotionTextParamSnapshot *)sdfMatGetTexUvwParm(binding->target);
    binding->capture.snapshot = *textParams;
}

SdfMotionIndexedTextBinding *sdfMotGenMatMultiUvw(sdfMotionCtrlr_t *motion, s32 unused, s32 options) {
    SdfMotionIndexedTextBinding *binding;

    binding = sdfChipAllocDirect(0x24);
    sdfMotGenMatInit((SdfMotionIndexedBinding *)binding, motion, &D_0040B4B0, options);
    return binding;
}

void sdfMotCLBK_matMultiUvwSeek(SdfMotionIndexedTextBinding *binding, f32 frame) {
    u8 keys[16];
    u8 interpolated[32];

    sdfMotGetKey((SdfMotionKeyBinding *)binding, (SdfMotionKeyInterval *)keys, frame);
    sdfMotInterpolateKeyUvw(keys, interpolated);
    sdfMatSetMultiTexUvwByPtr(binding->target, (const f32 *)interpolated);
}

void sdfMotCLBK_matMultiUvwBlendSeek(SdfMotionIndexedTextBinding *binding, f32 frame, f32 blendWeight) {
    SdfMotionKeyInterval keyInterval;
    f32 b1[5];
    f32 b2[5];

    sdfMotGetKey((SdfMotionKeyBinding *)binding, &keyInterval, frame);
    sdfMotInterpolateKeyUvw(&keyInterval, b1);
    sdfMotInterpolateKeyUvwSub(b2, binding->capture.capturedValues, b1, blendWeight);
    sdfMatSetMultiTexUvwByPtr(binding->target, b2);
}

/* Save the current secondary text scalars for the blend callback. */
void sdfMotCLBK_matMultiUvwInitBlend(SdfMotionIndexedTextBinding *binding) {
    SdfMotionTextParamSnapshot *textParams;

    textParams = (SdfMotionTextParamSnapshot *)sdfMatGetMultiTexUvwParm(binding->target);
    binding->capture.snapshot = *textParams;
}

SdfMotionIndexedValueBinding *sdfMotGenMatMultiDiffuse(sdfMotionCtrlr_t *motion, s32 unused,
                                                                    s32 options) {
    SdfMotionIndexedValueBinding *binding;

    binding = sdfChipAllocDirect(0x14);
    sdfMotGenMatInit((SdfMotionIndexedBinding *)binding, motion, &D_0040B4C8, options);
    return binding;
}

void sdfMotCLBK_matMultiDiffuseSeek(SdfMotionIndexedValueBinding *binding, f32 frame) {
    u8 buffer[16];
    sdfMotGetKey((SdfMotionKeyBinding *)binding, (SdfMotionKeyInterval *)buffer, frame);
    sdfMatSetMultiDiffuse(binding->target, sdfMotInterpolateKeyColor(buffer));
}

/* vu0 routine: blend the captured binding colour toward the keyed colour by weight. */
void sdfMotCLBK_matMultiDiffuseBlendSeek(SdfMotionIndexedValueBinding *binding, f32 frame, f32 weight) {
    SdfMotionKeyInterval keyInterval;
    u32 key;
    u32 color;

    sdfMotGetKey((SdfMotionKeyBinding *)binding, &keyInterval, frame);
    key = sdfMotInterpolateKeyColor(&keyInterval);
    EE_MMI_RGBA_LERP(color, binding->capturedWord, key, weight, 0.5f);
    sdfMatSetMultiDiffuse(binding->target, color);
}

void sdfMotCLBK_matMultiDiffuseInitBlend(SdfMotionIndexedValueBinding *binding) {
    binding->capturedWord = binding->target->secondaryColor;
}

SdfMotionIndexedTextBinding *sdfMotGenMatUvw2(sdfMotionCtrlr_t *motion, s32 unused, s32 options) {
    SdfMotionIndexedTextBinding *binding;

    binding = sdfChipAllocDirect(0x24);
    sdfMotGenMatInit((SdfMotionIndexedBinding *)binding, motion, &D_0040B4E0, options);
    return binding;
}

void sdfMotCLBK_matUvw2Seek(SdfMotionIndexedTextBinding *binding, f32 frame) {
    SdfMotionKeyInterval sample;

    sdfMotGetKey(&binding->keys, &sample, frame);
    sdfMatSetTexUvwByPtr(binding->target, sample.firstKey);
}

void sdfMotCLBK_matUvw2BlendSeek(SdfMotionIndexedTextBinding *binding, f32 frame) {
    SdfMotionKeyInterval sample;

    sdfMotGetKey(&binding->keys, &sample, frame);
    sdfMatSetTexUvwByPtr(binding->target, sample.firstKey);
}

/* Direct primary-text sampling has an intentionally empty capturePrevious callback. */
void sdfMotCLBK_matUvw2InitBlend(void) {
}

SdfMotionIndexedTextBinding *sdfMotGenMatMultiUvw2(sdfMotionCtrlr_t *motion, s32 unused,
                                                                         s32 options) {
    SdfMotionIndexedTextBinding *binding;

    binding = sdfChipAllocDirect(0x24);
    sdfMotGenMatInit((SdfMotionIndexedBinding *)binding, motion, &D_0040B4F8, options);
    return binding;
}

void sdfMotCLBK_matMultiUvw2Seek(SdfMotionIndexedTextBinding *binding, f32 frame) {
    SdfMotionKeyInterval sample;

    sdfMotGetKey(&binding->keys, &sample, frame);
    sdfMatSetMultiTexUvwByPtr(binding->target, sample.firstKey);
}

void sdfMotCLBK_matMultiUvw2BlendSeek(SdfMotionIndexedTextBinding *binding, f32 frame) {
    SdfMotionKeyInterval sample;

    sdfMotGetKey(&binding->keys, &sample, frame);
    sdfMatSetMultiTexUvwByPtr(binding->target, sample.firstKey);
}
