#include "common.h"
#include "ps2asm.h"
#include "sdfFrame.h"
#include "libsdf.h"
#include "model.h"
#include "motion.h"


typedef struct
{
    sdfMotionNode_t nodeBase;
    sdfModelMorph_t *morphNode;
    sdfModelMorph_t blendSrc;
} sdfMotNodeMorph_t;

extern void FromEuler(f32 aRotateX, f32 aRotateY, f32 aRotateZ);
extern void Lerp(f32 aRatio);
extern void ToMatrix(void);
extern sdfMotionNodeVTBL_t sdfMotMorphTable;
void sdfMotionSeek(sdfMotionCtrlr_t *aCtrlr, f32 aFrame);

typedef sdfMotionNode_t *(*sdfMotGenNodeFunc)(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId);

extern void *sdfChipAllocDirect(s32 aSize);
extern void sdfChipFreeDirect(void *aPtr);
extern sdfModelNode_t *sdfModelSearchNode(sdfModelRootNode_t *aRoot, u32 aId);

extern void sdfMotionChangeBlend(sdfMotionCtrlr_t *aCtrlr, s32 aCutNum, s32 aType, f32 aPreFrame,
                                 f32 aBlendLeng);

/* The type id's high half picks one of these group tables, its low half the
 * generator inside the group. */
extern void *sdfChipAllocZeroDirect(s32 aSize);
extern void *sdfItemArrayNew(s32 aInitCapa, s32 aElemSize, s32 aGrowStep);
extern void sdfItemArrayFree(void *aArray);
extern void sdfMatSetDiffuse(sdfMat_t *aMat, u32 aColor);
extern void sdfMatSetAmbient(sdfMat_t *aMat, u32 aColor);
extern void sdfMatSetSpecular(sdfMat_t *aMat, u32 aColor);
extern void sdfMatSetRefMap(sdfMat_t *aMat, u32 aColor);
extern void sdfMatSetMultiDiffuse(sdfMat_t *aMat, u32 aColor);
extern void sdfMatSetConstant(sdfMat_t *aMat, f32 aRatio);
extern void sdfMatSetTexUvwByPtr(sdfMat_t *aMat, void *aParm);
extern void sdfMatSetMultiTexUvwByPtr(sdfMat_t *aMat, void *aParm);

extern void sdfMotGetKey(sdfMotionNode_t *aThis, f32 aFrame, sdfMotGetKeyIf_t *aItf);
extern /* vu0 routine: blend the two colour keys */
u32 sdfMotInterpolateKeyColor(sdfMotGetKeyIf_t *aItf);
extern f32 sdfMotInterpolateKeyFloat(sdfMotGetKeyIf_t *aItf);
extern void sdfMotInterpolateKeyUvw(sdfMotGetKeyIf_t *aItf, void *aDst);
extern void sdfMotInterpolateKeyUvwSub(f32 *aDst, f32 *aSrc0, f32 *aSrc1, f32 aRate);

extern sdfMotGenNodeFunc sdfMotGenNodeTable[];
extern sdfMotGenNodeFunc sdfMotGenNodeDef[];
extern sdfMotGenNodeFunc sdfMotGenMatTable[];
extern sdfMotGenNodeFunc sdfMotGenMorphTable[];

extern sdfMotionNodeVTBL_t sdfMotModelLinerPosTable; /* model liner pos */
extern sdfMotionNodeVTBL_t sdfMotModelLinerRotXYZTable;
extern sdfMotionNodeVTBL_t sdfMotModelLinerScaleTable;
extern sdfMotionNodeVTBL_t sdfMotModelLerpRotQuatTable;

typedef struct
{
    sdfMotNodeModelBase_t modelBase;
    s8 blendFrom;
} sdfMotNodeModelFlag_t;

typedef struct
{
    sdfMotNodeMatBase_t base;
    u32 blendSrc;
} sdfMotNodeMatColor_t;

typedef struct
{
    sdfMotNodeMatBase_t matBase;
    f32 blendFrom;
} sdfMotNodeMatFloat_t;

/* Five weights, not the six-word sdfMatUvwParm_t: only offset, tile and angle
 * are keyed. */
typedef struct
{
    f32 parm[5];
} sdfMotUvwKey_t;

typedef struct
{
    sdfMotNodeMatBase_t base;
    sdfMotUvwKey_t blendFrom;
} sdfMotNodeMatUvw_t;


extern sdfMatUvwParm_t *sdfMatGetTexUvwParm(sdfMat_t *aMat);
extern sdfMatUvwParm_t *sdfMatGetMultiTexUvwParm(sdfMat_t *aMat);

extern sdfMotionNodeVTBL_t sdfMotModelVisibleTable;
extern sdfMotionNodeVTBL_t sdfMotMatDiffuseTable;
extern sdfMotionNodeVTBL_t sdfMotMatAmbientTable;
extern sdfMotionNodeVTBL_t sdfMotMatSpecularTable;
extern sdfMotionNodeVTBL_t sdfMotMatRefMapTable;
extern sdfMotionNodeVTBL_t sdfMotMatConstantTable;
extern sdfMotionNodeVTBL_t sdfMotMatUvwTable;
extern sdfMotionNodeVTBL_t sdfMotMatMultiUvwTable;
extern sdfMotionNodeVTBL_t sdfMotMatMultiDiffuseTable;
extern sdfMotionNodeVTBL_t sdfMotMatUvw2Table;
extern sdfMotionNodeVTBL_t sdfMotMatMultiUvw2Table;

sdfMotionNode_t *sdfMotionNodeNew(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId)
{
    return sdfMotGenNodeTable[aTypeId >> 16](aCtrlr, aTypeId, aNodeId);
}

void sdfMotionNodeFree(sdfMotionNode_t *aNode)
{
    aNode->vtbl->destroy(aNode);
}

void sdfMotionNodeBaseInit(sdfMotionNode_t *aNode, sdfMotionCtrlr_t *aCtrlr, sdfMotionNodeVTBL_t *aVTBL)
{
    aNode->vtbl = aVTBL;
    aNode->owner = aCtrlr;
}

sdfMotionCtrlr_t *sdfMotionCtrlrNew(sdfModelRootNode_t *aModel, sdfMotionPlayBook_t *aBook)
{
    sdfMotionCtrlr_t *ctrlr;
    sdfMotionNodeDef_t *def;
    s32 numOfNode;
    s32 i;

    ctrlr = sdfChipAllocZeroDirect(sizeof(sdfMotionCtrlr_t));
    numOfNode = aBook->numOfNode;
    ctrlr->next = aModel->motion;
    aModel->motion = ctrlr;
    ctrlr->rootNode = aModel;
    ctrlr->playbook = aBook;
    ctrlr->nodeList = sdfItemArrayNew(numOfNode, 4, 8);
    ctrlr->nodeList->count = numOfNode;
    def = ctrlr->playbook->nodeDefList;
    for (i = 0; i < numOfNode; i++) {
        ctrlr->nodeList->items[i] = sdfMotionNodeNew(ctrlr, def->typeId, def->nodeId);
        def++;
    }
    ctrlr->stat = 0;
    ctrlr->speed = 1.0f;
    return ctrlr;
}

void sdfMotionCtrlrFree(sdfMotionCtrlr_t *aCtrlr)
{
    sdfMotionCtrlr_t **link;
    sdfMotionCtrlr_t *cur;
    sdfMotionNode_t **items;
    s32 count;
    s32 i;

    if (aCtrlr == NULL) {
        return;
    }
    if (aCtrlr->rootNode != NULL) {
        link = &aCtrlr->rootNode->motion;
        for (;;) {
            cur = *link;
            if (cur == NULL) {
                break;
            }
            if (cur == aCtrlr) {
                *link = aCtrlr->next;
                break;
            }
            link = &cur->next;
        }
    }
    count = aCtrlr->nodeList->count;
    items = aCtrlr->nodeList->items;
    for (i = 0; i < count; i++) {
        sdfMotionNodeFree(items[i]);
    }
    sdfItemArrayFree(aCtrlr->nodeList);
    sdfChipFreeDirect(aCtrlr);
}

void sdfMotionChangeBlend(sdfMotionCtrlr_t *aCtrlr, s32 aCutNum, s32 aType, f32 aPreFrame,
                          f32 aBlendLeng)
{
    s32 i;
    sdfMotionCutDef_t *cut;
    sdfMotionCutChunk_t *chunk;
    sdfMotionNode_t *node;

    aCtrlr->cutNumber = aCutNum;
    if (aBlendLeng > 0.0f) {
        aCtrlr->blendLength = aBlendLeng;
        aCtrlr->blendPreLength = -aPreFrame;
        for (i = 0; i < aCtrlr->nodeList->count; i++) {
            node = aCtrlr->nodeList->items[i];
            node->vtbl->initBlend(node);
        }
    } else {
        aCtrlr->blendLength = 0.0f;
        aCtrlr->blendPreLength = 0.0f;
    }
    aCtrlr->playType = aType;
    aCtrlr->stat = SDF_MOTION_STAT_SET;
    cut = aCtrlr->playbook->cutList[aCutNum];
    aCtrlr->frame = aCtrlr->blendPreLength;
    if (cut == NULL) {
        aCtrlr->frameLength = 0;
        aCtrlr->stat = SDF_MOTION_STAT_NOCUT;
        return;
    }
    aCtrlr->frameLength = cut->frameLength;
    chunk = cut->chunk;
    for (i = 0; i < aCtrlr->nodeList->count; i++) {
        node = aCtrlr->nodeList->items[i];
        node->vtbl->setCut(node, chunk);
        chunk = (sdfMotionCutChunk_t *)((u8 *)chunk + chunk->size);
    }
}

void sdfMotionChange(sdfMotionCtrlr_t *aCtrlr, s32 aCutNum, s32 aType)
{
    sdfMotionChangeBlend(aCtrlr, aCutNum, aType, 0.0f, 0.0f);
}

void sdfMotionSeek(sdfMotionCtrlr_t *aCtrlr, f32 aFrame) {
    s32 i;
    s32 count;
    f32 elapsed;
    f32 duration;
    f32 weight;
    sdfMotionNode_t *binding;

    aCtrlr->stat = SDF_MOTION_STAT_SEEK;
    duration = aCtrlr->blendLength;
    elapsed = aFrame - aCtrlr->blendPreLength;
    aCtrlr->frame = aFrame;
    if (elapsed < duration) {
        if (elapsed < 0.0f) {
            weight = 0.0f;
        } else {
            weight = elapsed / duration;
        }
        if (aFrame < 0.0f) {
            aFrame = 0.0f;
        }
        for (i = 0; i < aCtrlr->nodeList->count; i++) {
            binding = aCtrlr->nodeList->items[i];
            binding->vtbl->blendSeek(binding, aFrame, weight);
        }
    } else {
        count = aCtrlr->nodeList->count;
        for (i = 0; i < count; i++) {
            binding = aCtrlr->nodeList->items[i];
            binding->vtbl->seek(binding, aFrame);
        }
    }
}

s32 sdfMotionNext(sdfMotionCtrlr_t *aCtrlr) {
    s32 result;
    f32 frame;
    f32 frameLength;

    result = 0;
    switch (aCtrlr->stat) {
    case 0:
    case 2:
    case 3:
    case SDF_MOTION_STAT_NOCUT:
    case SDF_MOTION_STAT_PAUSE:
        break;
    case SDF_MOTION_STAT_SET:
        sdfMotionSeek(aCtrlr, aCtrlr->blendPreLength);
        break;
    case SDF_MOTION_STAT_SEEK:
        frame = aCtrlr->frame + SDF_FRAME_STEP(aCtrlr->speed);
        frameLength = aCtrlr->frameLength;
        if (aCtrlr->playType == 0) {
            if (frameLength <= frame) {
                sdfMotionSeek(aCtrlr, frameLength);
                aCtrlr->stat = SDF_MOTION_STAT_NOCUT;
                result = 1;
                break;
            }
        } else if (frameLength <= frame) {
            aCtrlr->blendLength = 0.0f;
            result = 2;
            aCtrlr->blendPreLength = 0.0f;
            do {
                frame -= frameLength;
            } while (frameLength <= frame);
        }
        sdfMotionSeek(aCtrlr, frame);
        break;
    default:
        break;
    }
    return result;
}

void sdfMotionPause(sdfMotionCtrlr_t *aCtrlr)
{
    u8 stat;

    stat = aCtrlr->stat;
    if (stat != SDF_MOTION_STAT_PAUSE) {
        aCtrlr->statBeforePause = stat;
        aCtrlr->stat = SDF_MOTION_STAT_PAUSE;
    }
}

void sdfMotionPauseOff(sdfMotionCtrlr_t *aCtrlr)
{
    if (aCtrlr->stat == SDF_MOTION_STAT_PAUSE) {
        aCtrlr->stat = aCtrlr->statBeforePause;
    }
}

void sdfMotCLBK_Destroy(sdfMotionNode_t *aNode)
{
    sdfChipFreeDirect(aNode);
}

void sdfMotCLBK_SetCut(sdfMotionNode_t *aNode, sdfMotionCutChunk_t *aTable)
{
    aNode->table = aTable;
}

void sdfMotGetKey(sdfMotionNode_t *aThis, f32 aFrame, sdfMotGetKeyIf_t *aItf)
{
    sdfMotionCutChunk_t *chunk;
    u16 *frameList;
    u8 *keyData;
    u8 *key0;
    s32 frame;
    s32 numOfKey;
    s32 elemSize;
    s32 lo;
    s32 hi;
    s32 index;
    s32 next;
    s32 frame0;
    s32 frame1;
    s32 span;

    frame = aFrame;
    chunk = aThis->table;
    numOfKey = chunk->numOfKey;
    frameList = chunk->keyFrameList;
    elemSize = chunk->keyElemSize;
    lo = 0;
    hi = numOfKey - 1;
    do {
        index = (lo + hi + 1) >> 1;
        frame0 = frameList[index];
        if (frame < frame0) {
            index--;
            hi = index;
        } else {
            lo = index;
        }
    } while (lo < hi);
    keyData = (u8 *)&frameList[(numOfKey + 1) & ~1];
    frame0 = frameList[index];
    key0 = keyData + index * elemSize;
    aItf->key0 = key0;
    next = index + 1;
    if (next == numOfKey) {
        if (aThis->owner->playType == 0) {
            aItf->key1 = key0;
            frame1 = frame0;
        } else {
            aItf->key1 = keyData;
            frame1 = aThis->owner->frameLength;
        }
    } else {
        aItf->key1 = key0 + elemSize;
        frame1 = frameList[next];
    }
    span = frame1 - frame0;
    if (span == 0) {
        aItf->key1 = key0;
        aItf->rate = 0.0f;
        return;
    }
    aItf->rate = (aFrame - frame0) / span;
}

f32 sdfMotInterpolateKeyFloat(sdfMotGetKeyIf_t *aItf)
{
    f32 *key0;
    f32 *key1;

    key0 = aItf->key0;
    key1 = aItf->key1;
    return *key0 + *key1 * aItf->rate - *key0 * aItf->rate;
}

/* vu0 routine: sample and blend the channel using the SDK packed operations. */
void sdfMotInterpolateKeyXYZ(sdfMotGetKeyIf_t *aItf) {
    VU0_LERP_VEC3_KEYS(aItf->key0, aItf->key1, aItf->rate);
}

/* vu0 routine: blend the two colour keys */
u32 sdfMotInterpolateKeyColor(sdfMotGetKeyIf_t *aItf)
{
    u32 *key0;
    u32 *key1;
    u32 color;

    key0 = aItf->key0;
    key1 = aItf->key1;
    VU0_RGBA_LERP(color, *key0, *key1, aItf->rate, 0.5f);
    return color;
}

void sdfMotInterpolateKeyUvwSub(f32 *aDst, f32 *aSrc0, f32 *aSrc1, f32 aRate)
{
    s32 i;

    for (i = 0; i != 5; i++) {
        aDst[i] = aSrc0[i] * (1.0f - aRate) + aSrc1[i] * aRate;
    }
}

void sdfMotInterpolateKeyUvw(sdfMotGetKeyIf_t *aItf, void *aDst)
{
    sdfMotInterpolateKeyUvwSub(aDst, aItf->key0, aItf->key1, aItf->rate);
}

sdfMotionNode_t *sdfMotGenNode(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId)
{
    return sdfMotGenNodeDef[aTypeId & 0xFFFF](aCtrlr, aTypeId, aNodeId);
}

void sdfMotGenModelInit(sdfMotNodeModelBase_t *aBase, sdfMotionCtrlr_t *aCtrlr, sdfMotionNodeVTBL_t *aVTBL, u32 aNodeId)
{
    sdfMotionNodeBaseInit(&aBase->nodeBase, aCtrlr, aVTBL);
    aBase->modelNode = sdfModelSearchNode(aCtrlr->rootNode, aNodeId);
}

sdfMotionNode_t *sdfMotGenModelLinerPos(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId)
{
    sdfMotNodeModelVec_t *node;

    node = sdfChipAllocDirect(sizeof(sdfMotNodeModelVec_t));
    sdfMotGenModelInit(&node->modelBase, aCtrlr, &sdfMotModelLinerPosTable, aNodeId);
    return &node->modelBase.nodeBase;
}

/* vu0 routine: sample and blend the channel using the SDK packed operations. */
void sdfMotCLBK_modelLinerPosSeek(sdfMotNodeModelVec_t *aThis, f32 aFrame) {
    sdfMotGetKeyIf_t keyInterval;

    sdfMotGetKey((sdfMotionNode_t *)aThis, aFrame, &keyInterval);
    SDF_VU0_LOADVECTOR_UNALIGN(keyInterval.key0);
    SDF_VU0_LOADVECTOR_UNALIGN_B(keyInterval.key1);
    VU0_LERP_VF10_W(keyInterval.rate);
    SDF_VU0_STOREVECTOR_NOMEM(&aThis->modelBase.modelNode->pos);
}

/* vu0 routine: sample and blend the channel using the SDK packed operations. */
void sdfMotCLBK_modelLinerPosBlendSeek(sdfMotNodeModelVec_t *aThis, f32 aFrame, f32 aBlendRate) {
    sdfMotGetKeyIf_t keyInterval;

    sdfMotGetKey((sdfMotionNode_t *)aThis, aFrame, &keyInterval);
    SDF_VU0_LOADVECTOR_UNALIGN(keyInterval.key0);
    SDF_VU0_LOADVECTOR_UNALIGN_B(keyInterval.key1);
    VU0_LERP_VF10_COPY(keyInterval.rate);
    SDF_VU0_LOADVECTOR(&aThis->blendFrom);
    VU0_LERP_VF10_W(aBlendRate);
    SDF_VU0_STOREVECTOR_NOMEM(&aThis->modelBase.modelNode->pos);
}

sdfMotionNode_t *sdfMotGenModelLinerRotXYZ(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId)
{
    sdfMotNodeModelVec_t *node;

    node = sdfChipAllocDirect(sizeof(sdfMotNodeModelVec_t));
    sdfMotGenModelInit(&node->modelBase, aCtrlr, &sdfMotModelLinerRotXYZTable, aNodeId);
    return &node->modelBase.nodeBase;
}

/* vu0 routine: sample and blend the channel using the SDK packed operations. */
void sdfMotCLBK_modelLinerRotXYZSeek(sdfMotNodeModelVec_t *aThis, f32 aFrame) {
    sdfMotGetKeyIf_t keyInterval;
    /* volatile: retail reads the three angles with the call's delay slot left
     * empty, which only a volatile local reaches. */
    volatile sdfVector4_t euler;
    sdfModelNode_t *drawNode;
    f32 (*matrix)[4];

    sdfMotGetKey((sdfMotionNode_t *)aThis, aFrame, &keyInterval);
    SDF_VU0_LOADVECTOR_UNALIGN(keyInterval.key0);
    SDF_VU0_LOADVECTOR_UNALIGN_B(keyInterval.key1);
    VU0_LERP_VF10_VF11(keyInterval.rate);
    SDF_VU0_VECTOR_W_SET_ZERO();
    SDF_VU0_STOREVECTOR_NOMEM(&euler);
    FromEuler(euler.x, euler.y, euler.z);
    drawNode = aThis->modelBase.modelNode;
    SDF_VU0_STOREVECTOR_NOMEM(&drawNode->rotate);
    matrix = (f32 (*)[4])drawNode->localNoScaledTm.m;
    ToMatrix();
    SDF_VU0_STORE_ROW0_NOMEM(matrix[0]);
    SDF_VU0_STORE_ROW1_NOMEM(matrix[1]);
    SDF_VU0_STORE_ROW2_NOMEM(matrix[2]);
}

/* vu0 routine: sample and blend the channel using the SDK packed operations. */
void sdfMotCLBK_modelLinerRotXYZBlendSeek(sdfMotNodeModelVec_t *aThis, f32 aFrame, f32 aBlendRate) {
    sdfMotGetKeyIf_t keyInterval;
    volatile sdfVector4_t euler;
    sdfModelNode_t *drawNode;
    f32 (*matrix)[4];

    sdfMotGetKey((sdfMotionNode_t *)aThis, aFrame, &keyInterval);
    SDF_VU0_LOADVECTOR_UNALIGN(keyInterval.key0);
    SDF_VU0_LOADVECTOR_UNALIGN_B(keyInterval.key1);
    VU0_LERP_VF10_VF11(keyInterval.rate);
    SDF_VU0_VECTOR_W_SET_ZERO();
    SDF_VU0_STOREVECTOR_NOMEM(&euler);
    FromEuler(euler.x, euler.y, euler.z);
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(&aThis->blendFrom);
    Lerp(aBlendRate);
    drawNode = aThis->modelBase.modelNode;
    SDF_VU0_STOREVECTOR_NOMEM(&drawNode->rotate);
    matrix = (f32 (*)[4])drawNode->localNoScaledTm.m;
    ToMatrix();
    SDF_VU0_STORE_ROW0_NOMEM(matrix[0]);
    SDF_VU0_STORE_ROW1_NOMEM(matrix[1]);
    SDF_VU0_STORE_ROW2_NOMEM(matrix[2]);
}

sdfMotionNode_t *sdfMotGenModelLinerScale(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId)
{
    sdfMotNodeModelVec_t *node;

    node = sdfChipAllocDirect(sizeof(sdfMotNodeModelVec_t));
    sdfMotGenModelInit(&node->modelBase, aCtrlr, &sdfMotModelLinerScaleTable, aNodeId);
    return &node->modelBase.nodeBase;
}

/* vu0 routine: sample and blend the channel using the SDK packed operations. */
void sdfMotCLBK_modelLinerScaleSeek(sdfMotNodeModelVec_t *aThis, f32 aFrame) {
    sdfMotGetKeyIf_t keyInterval;

    sdfMotGetKey((sdfMotionNode_t *)aThis, aFrame, &keyInterval);
    SDF_VU0_LOADVECTOR_UNALIGN(keyInterval.key0);
    SDF_VU0_LOADVECTOR_UNALIGN_B(keyInterval.key1);
    VU0_LERP_VF10_W(keyInterval.rate);
    SDF_VU0_STOREVECTOR(&aThis->modelBase.modelNode->scale);
}

/* vu0 routine: sample and blend the channel using the SDK packed operations. */
void sdfMotCLBK_modelLinerScaleBlendSeek(sdfMotNodeModelVec_t *aThis, f32 aFrame, f32 aBlendRate) {
    sdfMotGetKeyIf_t keyInterval;

    sdfMotGetKey((sdfMotionNode_t *)aThis, aFrame, &keyInterval);
    SDF_VU0_LOADVECTOR_UNALIGN(keyInterval.key0);
    SDF_VU0_LOADVECTOR_UNALIGN_B(keyInterval.key1);
    VU0_LERP_VF10_COPY(keyInterval.rate);
    SDF_VU0_LOADVECTOR(&aThis->blendFrom);
    VU0_LERP_VF10_W(aBlendRate);
    SDF_VU0_STOREVECTOR(&aThis->modelBase.modelNode->scale);
}

sdfMotionNode_t *sdfMotGenModelLerpRotQuat(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId)
{
    sdfMotNodeModelVec_t *node;

    node = sdfChipAllocDirect(sizeof(sdfMotNodeModelVec_t));
    sdfMotGenModelInit(&node->modelBase, aCtrlr, &sdfMotModelLerpRotQuatTable, aNodeId);
    return &node->modelBase.nodeBase;
}

/* vu0 routine: sample and blend the channel using the SDK packed operations. */
void sdfMotCLBK_modelLerpRotQuatSeek(sdfMotNodeModelVec_t *aThis, f32 aFrame) {
    sdfMotGetKeyIf_t keyInterval;
    sdfModelNode_t *drawNode;
    f32 (*matrix)[4];
    f32 *keyData;

    sdfMotGetKey((sdfMotionNode_t *)aThis, aFrame, &keyInterval);
    keyData = keyInterval.key0;
    SDF_VU0_LOADSHORTVECTOR(keyData);
    keyData = keyInterval.key1;
    SDF_VU0_LOADSHORTVECTOR_B(keyData);
    Lerp(keyInterval.rate);
    drawNode = aThis->modelBase.modelNode;
    SDF_VU0_STOREVECTOR_NOMEM(&drawNode->rotate);
    matrix = (f32 (*)[4])drawNode->localNoScaledTm.m;
    ToMatrix();
    SDF_VU0_STORE_ROW0_NOMEM(matrix[0]);
    SDF_VU0_STORE_ROW1_NOMEM(matrix[1]);
    SDF_VU0_STORE_ROW2_NOMEM(matrix[2]);
}

/* vu0 routine: sample and blend the channel using the SDK packed operations. */
void sdfMotCLBK_modelLerpRotQuatBlendSeek(sdfMotNodeModelVec_t *aThis, f32 aFrame, f32 aBlendRate) {
    sdfMotGetKeyIf_t keyInterval;
    sdfModelNode_t *drawNode;
    f32 (*matrix)[4];
    f32 *keyData;

    sdfMotGetKey((sdfMotionNode_t *)aThis, aFrame, &keyInterval);
    keyData = keyInterval.key0;
    SDF_VU0_LOADSHORTVECTOR(keyData);
    keyData = keyInterval.key1;
    SDF_VU0_LOADSHORTVECTOR_B(keyData);
    Lerp(keyInterval.rate);
    drawNode = aThis->modelBase.modelNode;
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(&aThis->blendFrom);
    Lerp(aBlendRate);
    SDF_VU0_STOREVECTOR_NOMEM(&drawNode->rotate);
    matrix = (f32 (*)[4])drawNode->localNoScaledTm.m;
    ToMatrix();
    SDF_VU0_STORE_ROW0_NOMEM(matrix[0]);
    SDF_VU0_STORE_ROW1_NOMEM(matrix[1]);
    SDF_VU0_STORE_ROW2_NOMEM(matrix[2]);
}

sdfMotionNode_t *sdfMotGenModelVisible(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId)
{
    sdfMotNodeModelFlag_t *node;

    node = sdfChipAllocDirect(sizeof(sdfMotNodeModelFlag_t));
    sdfMotGenModelInit(&node->modelBase, aCtrlr, &sdfMotModelVisibleTable, aNodeId);
    return &node->modelBase.nodeBase;
}

void sdfMotCLBK_modelVisibleSeek(sdfMotNodeModelBase_t *aThis, f32 aFrame)
{
    sdfMotGetKeyIf_t itf;
    sdfModelNode_t *node;
    u8 *key;

    sdfMotGetKey(&aThis->nodeBase, aFrame, &itf);
    key = itf.key0;
    node = aThis->modelNode;
    if (*key == 0) {
        node->flags |= SDF_MODEL_NODE_FLAG_MOTION_HIDE;
    } else {
        node->flags &= ~SDF_MODEL_NODE_FLAG_MOTION_HIDE;
    }
}

void sdfMotCLBK_modelVisibleBlendSeek(sdfMotNodeModelBase_t *aThis, f32 aFrame, f32 aBlendRate)
{
    sdfMotGetKeyIf_t itf;
    sdfModelNode_t *node;
    u8 *key;

    sdfMotGetKey(&aThis->nodeBase, aFrame, &itf);
    key = itf.key0;
    node = aThis->modelNode;
    if (*key == 0) {
        node->flags |= SDF_MODEL_NODE_FLAG_MOTION_HIDE;
    } else {
        node->flags &= ~SDF_MODEL_NODE_FLAG_MOTION_HIDE;
    }
}

void sdfMotCLBK_modelVisibleInitBlend(sdfMotNodeModelFlag_t *aThis)
{
    aThis->blendFrom = ((aThis->modelBase.modelNode->flags >> 4) ^ 1) & 1;
}

void sdfMotCLBK_modelPosInitBlend(sdfMotNodeModelVec_t *aThis)
{
    SDF_VU0_COPYVECTOR(&aThis->blendFrom, &aThis->modelBase.modelNode->pos);
}

void sdfMotCLBK_modelRotInitBlend(sdfMotNodeModelVec_t *aThis)
{
    SDF_VU0_COPYVECTOR(&aThis->blendFrom, &aThis->modelBase.modelNode->rotate);
}

void sdfMotCLBK_modelScaleInitBlend(sdfMotNodeModelVec_t *aThis)
{
    SDF_VU0_COPYVECTOR(&aThis->blendFrom, &aThis->modelBase.modelNode->scale);
}

sdfMotionNode_t *sdfMotGenMatNode(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId)
{
    return sdfMotGenMatTable[aTypeId & 0xFFFF](aCtrlr, aTypeId, aNodeId);
}

void sdfMotGenMatInit(sdfMotNodeMatBase_t *aBase, sdfMotionCtrlr_t *aCtrlr, sdfMotionNodeVTBL_t *aVTBL, u32 aNodeId)
{
    sdfMotionNodeBaseInit(&aBase->nodeBase, aCtrlr, aVTBL);
    aBase->mat = aCtrlr->rootNode->matList->items[aNodeId];
}

sdfMotionNode_t *sdfMotGenMatDiffuse(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId)
{
    sdfMotNodeMatColor_t *node;

    node = sdfChipAllocDirect(sizeof(sdfMotNodeMatColor_t));
    sdfMotGenMatInit(&node->base, aCtrlr, &sdfMotMatDiffuseTable, aNodeId);
    return &node->base.nodeBase;
}

void sdfMotCLBK_matDiffuseSeek(sdfMotNodeMatBase_t *aThis, f32 aFrame)
{
    sdfMotGetKeyIf_t itf;

    sdfMotGetKey(&aThis->nodeBase, aFrame, &itf);
    sdfMatSetDiffuse(aThis->mat, sdfMotInterpolateKeyColor(&itf));
}

void sdfMotCLBK_matDiffuseBlendSeek(sdfMotNodeMatColor_t *aThis, f32 aFrame, f32 aBlendRate)
{
    sdfMotGetKeyIf_t itf;
    u32 to;
    u32 color;

    sdfMotGetKey(&aThis->base.nodeBase, aFrame, &itf);
    to = sdfMotInterpolateKeyColor(&itf);
    VU0_RGBA_LERP(color, aThis->blendSrc, to, aBlendRate, 0.5f);
    sdfMatSetDiffuse(aThis->base.mat, color);
}

void sdfMotCLBK_matDiffuseInitBlend(sdfMotNodeMatColor_t *aThis)
{
    aThis->blendSrc = aThis->base.mat->diffuse;
}

sdfMotionNode_t *sdfMotGenMatAmbient(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId)
{
    sdfMotNodeMatColor_t *node;

    node = sdfChipAllocDirect(sizeof(sdfMotNodeMatColor_t));
    sdfMotGenMatInit(&node->base, aCtrlr, &sdfMotMatAmbientTable, aNodeId);
    return &node->base.nodeBase;
}

void sdfMotCLBK_matAmbientSeek(sdfMotNodeMatBase_t *aThis, f32 aFrame)
{
    sdfMotGetKeyIf_t itf;

    sdfMotGetKey(&aThis->nodeBase, aFrame, &itf);
    sdfMatSetAmbient(aThis->mat, sdfMotInterpolateKeyColor(&itf));
}

void sdfMotCLBK_matAmbientBlendSeek(sdfMotNodeMatColor_t *aThis, f32 aFrame, f32 aBlendRate)
{
    sdfMotGetKeyIf_t itf;
    u32 to;
    u32 color;

    sdfMotGetKey(&aThis->base.nodeBase, aFrame, &itf);
    to = sdfMotInterpolateKeyColor(&itf);
    VU0_RGBA_LERP(color, aThis->blendSrc, to, aBlendRate, 0.5f);
    sdfMatSetAmbient(aThis->base.mat, color);
}

void sdfMotCLBK_matAmbientInitBlend(sdfMotNodeMatColor_t *aThis)
{
    aThis->blendSrc = aThis->base.mat->ambient;
}

sdfMotionNode_t *sdfMotGenMatSpecular(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId)
{
    sdfMotNodeMatColor_t *node;

    node = sdfChipAllocDirect(sizeof(sdfMotNodeMatColor_t));
    sdfMotGenMatInit(&node->base, aCtrlr, &sdfMotMatSpecularTable, aNodeId);
    return &node->base.nodeBase;
}

void sdfMotCLBK_matSpecularSeek(sdfMotNodeMatBase_t *aThis, f32 aFrame)
{
    sdfMotGetKeyIf_t itf;

    sdfMotGetKey(&aThis->nodeBase, aFrame, &itf);
    sdfMatSetSpecular(aThis->mat, sdfMotInterpolateKeyColor(&itf));
}

void sdfMotCLBK_matSpecularBlendSeek(sdfMotNodeMatColor_t *aThis, f32 aFrame, f32 aBlendRate)
{
    sdfMotGetKeyIf_t itf;
    u32 to;
    u32 color;

    sdfMotGetKey(&aThis->base.nodeBase, aFrame, &itf);
    to = sdfMotInterpolateKeyColor(&itf);
    VU0_RGBA_LERP(color, aThis->blendSrc, to, aBlendRate, 0.5f);
    sdfMatSetSpecular(aThis->base.mat, color);
}

void sdfMotCLBK_matSpecularInitBlend(sdfMotNodeMatColor_t *aThis)
{
    aThis->blendSrc = aThis->base.mat->specular;
}

sdfMotionNode_t *sdfMotGenMatRefMap(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId)
{
    sdfMotNodeMatColor_t *node;

    node = sdfChipAllocDirect(sizeof(sdfMotNodeMatColor_t));
    sdfMotGenMatInit(&node->base, aCtrlr, &sdfMotMatRefMapTable, aNodeId);
    return &node->base.nodeBase;
}

void sdfMotCLBK_matRefMapSeek(sdfMotNodeMatBase_t *aThis, f32 aFrame)
{
    sdfMotGetKeyIf_t itf;

    sdfMotGetKey(&aThis->nodeBase, aFrame, &itf);
    sdfMatSetRefMap(aThis->mat, sdfMotInterpolateKeyColor(&itf));
}

void sdfMotCLBK_matRefMapBlendSeek(sdfMotNodeMatColor_t *aThis, f32 aFrame, f32 aBlendRate)
{
    sdfMotGetKeyIf_t itf;
    u32 to;
    u32 color;

    sdfMotGetKey(&aThis->base.nodeBase, aFrame, &itf);
    to = sdfMotInterpolateKeyColor(&itf);
    VU0_RGBA_LERP(color, aThis->blendSrc, to, aBlendRate, 0.5f);
    sdfMatSetRefMap(aThis->base.mat, color);
}

void sdfMotCLBK_matRefMapInitBlend(sdfMotNodeMatColor_t *aThis)
{
    aThis->blendSrc = aThis->base.mat->refmap;
}

sdfMotionNode_t *sdfMotGenMatConstant(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId)
{
    sdfMotNodeMatFloat_t *node;

    node = sdfChipAllocDirect(sizeof(sdfMotNodeMatFloat_t));
    sdfMotGenMatInit(&node->matBase, aCtrlr, &sdfMotMatConstantTable, aNodeId);
    return &node->matBase.nodeBase;
}

void sdfMotCLBK_matConstantSeek(sdfMotNodeMatBase_t *aThis, f32 aFrame)
{
    sdfMotGetKeyIf_t itf;

    sdfMotGetKey(&aThis->nodeBase, aFrame, &itf);
    sdfMatSetConstant(aThis->mat, sdfMotInterpolateKeyFloat(&itf));
}

void sdfMotCLBK_matConstantBlendSeek(sdfMotNodeMatFloat_t *aThis, f32 aFrame, f32 aBlendRate)
{
    sdfMotGetKeyIf_t itf;
    f32 to;
    f32 from;

    sdfMotGetKey(&aThis->matBase.nodeBase, aFrame, &itf);
    to = sdfMotInterpolateKeyFloat(&itf);
    from = aThis->blendFrom;
    sdfMatSetConstant(aThis->matBase.mat, from + to * aBlendRate - from * aBlendRate);
}

void sdfMotCLBK_matConstantInitBlend(sdfMotNodeMatFloat_t *aThis)
{
    aThis->blendFrom = aThis->matBase.mat->constant;
}

sdfMotionNode_t *sdfMotGenMatUvw(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId)
{
    sdfMotNodeMatUvw_t *node;

    node = sdfChipAllocDirect(sizeof(sdfMotNodeMatUvw_t));
    sdfMotGenMatInit(&node->base, aCtrlr, &sdfMotMatUvwTable, aNodeId);
    return &node->base.nodeBase;
}

void sdfMotCLBK_matUvwSeek(sdfMotNodeMatBase_t *aThis, f32 aFrame)
{
    sdfMotGetKeyIf_t itf;
    sdfMatUvwParm_t uvw;

    sdfMotGetKey(&aThis->nodeBase, aFrame, &itf);
    sdfMotInterpolateKeyUvw(&itf, &uvw);
    sdfMatSetTexUvwByPtr(aThis->mat, &uvw);
}

void sdfMotCLBK_matUvwBlendSeek(sdfMotNodeMatUvw_t *aThis, f32 aFrame, f32 aBlendRate)
{
    sdfMotGetKeyIf_t itf;
    sdfMotUvwKey_t dst;
    sdfMotUvwKey_t key;

    sdfMotGetKey(&aThis->base.nodeBase, aFrame, &itf);
    sdfMotInterpolateKeyUvw(&itf, &key);
    sdfMotInterpolateKeyUvwSub(dst.parm, aThis->blendFrom.parm, key.parm, aBlendRate);
    sdfMatSetTexUvwByPtr(aThis->base.mat, &dst);
}

void sdfMotCLBK_matUvwInitBlend(sdfMotNodeMatUvw_t *aThis)
{
    aThis->blendFrom = *(sdfMotUvwKey_t *)sdfMatGetTexUvwParm(aThis->base.mat);
}

sdfMotionNode_t *sdfMotGenMatMultiUvw(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId)
{
    sdfMotNodeMatUvw_t *node;

    node = sdfChipAllocDirect(sizeof(sdfMotNodeMatUvw_t));
    sdfMotGenMatInit(&node->base, aCtrlr, &sdfMotMatMultiUvwTable, aNodeId);
    return &node->base.nodeBase;
}

void sdfMotCLBK_matMultiUvwSeek(sdfMotNodeMatBase_t *aThis, f32 aFrame)
{
    sdfMotGetKeyIf_t itf;
    sdfMatUvwParm_t uvw;

    sdfMotGetKey(&aThis->nodeBase, aFrame, &itf);
    sdfMotInterpolateKeyUvw(&itf, &uvw);
    sdfMatSetMultiTexUvwByPtr(aThis->mat, &uvw);
}

void sdfMotCLBK_matMultiUvwBlendSeek(sdfMotNodeMatUvw_t *aThis, f32 aFrame, f32 aBlendRate)
{
    sdfMotGetKeyIf_t itf;
    sdfMotUvwKey_t key;
    sdfMotUvwKey_t dst;

    sdfMotGetKey(&aThis->base.nodeBase, aFrame, &itf);
    sdfMotInterpolateKeyUvw(&itf, &key);
    sdfMotInterpolateKeyUvwSub(dst.parm, aThis->blendFrom.parm, key.parm, aBlendRate);
    sdfMatSetMultiTexUvwByPtr(aThis->base.mat, &dst);
}

void sdfMotCLBK_matMultiUvwInitBlend(sdfMotNodeMatUvw_t *aThis)
{
    aThis->blendFrom = *(sdfMotUvwKey_t *)sdfMatGetMultiTexUvwParm(aThis->base.mat);
}

sdfMotionNode_t *sdfMotGenMatMultiDiffuse(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId)
{
    sdfMotNodeMatColor_t *node;

    node = sdfChipAllocDirect(sizeof(sdfMotNodeMatColor_t));
    sdfMotGenMatInit(&node->base, aCtrlr, &sdfMotMatMultiDiffuseTable, aNodeId);
    return &node->base.nodeBase;
}

void sdfMotCLBK_matMultiDiffuseSeek(sdfMotNodeMatBase_t *aThis, f32 aFrame)
{
    sdfMotGetKeyIf_t itf;

    sdfMotGetKey(&aThis->nodeBase, aFrame, &itf);
    sdfMatSetMultiDiffuse(aThis->mat, sdfMotInterpolateKeyColor(&itf));
}

void sdfMotCLBK_matMultiDiffuseBlendSeek(sdfMotNodeMatColor_t *aThis, f32 aFrame, f32 aBlendRate)
{
    sdfMotGetKeyIf_t itf;
    u32 to;
    u32 color;

    sdfMotGetKey(&aThis->base.nodeBase, aFrame, &itf);
    to = sdfMotInterpolateKeyColor(&itf);
    VU0_RGBA_LERP(color, aThis->blendSrc, to, aBlendRate, 0.5f);
    sdfMatSetMultiDiffuse(aThis->base.mat, color);
}

void sdfMotCLBK_matMultiDiffuseInitBlend(sdfMotNodeMatColor_t *aThis)
{
    aThis->blendSrc = aThis->base.mat->multiTexDiffuse;
}

sdfMotionNode_t *sdfMotGenMatUvw2(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId)
{
    sdfMotNodeMatUvw_t *node;

    node = sdfChipAllocDirect(sizeof(sdfMotNodeMatUvw_t));
    sdfMotGenMatInit(&node->base, aCtrlr, &sdfMotMatUvw2Table, aNodeId);
    return &node->base.nodeBase;
}

void sdfMotCLBK_matUvw2Seek(sdfMotNodeMatBase_t *aThis, f32 aFrame)
{
    sdfMotGetKeyIf_t itf;

    sdfMotGetKey(&aThis->nodeBase, aFrame, &itf);
    sdfMatSetTexUvwByPtr(aThis->mat, itf.key0);
}

void sdfMotCLBK_matUvw2BlendSeek(sdfMotNodeMatBase_t *aThis, f32 aFrame, f32 aBlendRate)
{
    sdfMotGetKeyIf_t itf;

    sdfMotGetKey(&aThis->nodeBase, aFrame, &itf);
    sdfMatSetTexUvwByPtr(aThis->mat, itf.key0);
}

void sdfMotCLBK_matUvw2InitBlend(void) {
}

sdfMotionNode_t *sdfMotGenMatMultiUvw2(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId)
{
    sdfMotNodeMatUvw_t *node;

    node = sdfChipAllocDirect(sizeof(sdfMotNodeMatUvw_t));
    sdfMotGenMatInit(&node->base, aCtrlr, &sdfMotMatMultiUvw2Table, aNodeId);
    return &node->base.nodeBase;
}

void sdfMotCLBK_matMultiUvw2Seek(sdfMotNodeMatBase_t *aThis, f32 aFrame)
{
    sdfMotGetKeyIf_t itf;

    sdfMotGetKey(&aThis->nodeBase, aFrame, &itf);
    sdfMatSetMultiTexUvwByPtr(aThis->mat, itf.key0);
}

void sdfMotCLBK_matMultiUvw2BlendSeek(sdfMotNodeMatBase_t *aThis, f32 aFrame, f32 aBlendRate)
{
    sdfMotGetKeyIf_t itf;

    sdfMotGetKey(&aThis->nodeBase, aFrame, &itf);
    sdfMatSetMultiTexUvwByPtr(aThis->mat, itf.key0);
}

void sdfMotCLBK_matMultiUvw2InitBlend(void) {
}

sdfMotionNode_t *sdfMotGenMorphNode(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId)
{
    return sdfMotGenMorphTable[aTypeId & 0xFFFF](aCtrlr, aTypeId, aNodeId);
}

void sdfMotGenMorphInit(sdfMotNodeMorph_t *aBase,
                                 sdfMotionCtrlr_t *aCtrlr, sdfMotionNodeVTBL_t *aVTBL, s32 aNodeId) {
    sdfMotionNodeBaseInit((sdfMotionNode_t *)&aBase->nodeBase,
                            aCtrlr, aVTBL);
    aBase->morphNode = &((sdfModelMorph_t *)aCtrlr->rootNode->morphList->items)[aNodeId];
}

sdfMotionNode_t *sdfMotGenMorph(sdfMotionCtrlr_t *aCtrlr,
                                                           s32 aTypeId, s32 aNodeId) {
    sdfMotNodeMorph_t *entry = sdfChipAllocDirect(sizeof(sdfMotNodeMorph_t));

    sdfMotGenMorphInit(entry, aCtrlr, &sdfMotMorphTable, aNodeId);
    return &entry->nodeBase;
}

void sdfMotCLBK_morphSeek(sdfMotNodeMorph_t *aThis, f32 aFrame) {
    sdfMotGetKeyIf_t sample;
    sdfModelMorphParm_t *out;
    s32 firstId;
    s32 secondId;
    f32 firstValue;
    f32 secondValue;
    f32 weight;
    f32 inverse;

    sdfMotGetKey(&aThis->nodeBase, aFrame, &sample);
    firstId = ((sdfModelMorphParm_t *)sample.key0)->targetId;
    secondId = ((sdfModelMorphParm_t *)sample.key1)->targetId;
    firstValue = ((sdfModelMorphParm_t *)sample.key0)->blend;
    secondValue = ((sdfModelMorphParm_t *)sample.key1)->blend;
    weight = sample.rate;
    inverse = 1.0f - weight;
    out = aThis->morphNode->target;
    if (firstId == secondId) {
        out[0].targetId = firstId;
        out[0].blend = firstValue * inverse + secondValue * weight;
        out[1].targetId = secondId;
        out[1].blend = 0;
    } else {
        out[0].targetId = firstId;
        out[0].blend = firstValue * inverse;
        out[1].targetId = secondId;
        out[1].blend = secondValue * weight;
    }
}

void sdfMotCLBK_morphBlendSeek(sdfMotNodeMorph_t *aThis, f32 aFrame, f32 aBlendRate) {
    sdfMotGetKeyIf_t sample;
    sdfModelMorph_t *current;
    sdfModelMorphParm_t *previous;
    u32 count;
    s32 previousPairIndex;
    s32 firstId;
    s32 secondId;
    f32 firstValue;
    f32 secondValue;
    f32 contributionWeight;
    f32 secondWeight;

    sdfMotGetKey(&aThis->nodeBase, aFrame, &sample);
    firstId = ((sdfModelMorphParm_t *)sample.key0)->targetId;
    secondId = ((sdfModelMorphParm_t *)sample.key1)->targetId;
    firstValue = ((sdfModelMorphParm_t *)sample.key0)->blend;
    secondValue = ((sdfModelMorphParm_t *)sample.key1)->blend;
    secondWeight = sample.rate;
    {
        const f32 sampleComplement = 1.0f - secondWeight;
        contributionWeight = sampleComplement * aBlendRate;
    }
    secondWeight *= aBlendRate;
    current = aThis->morphNode;

    if (firstId == secondId) {
        current->target[0].targetId = secondId;
        current->target[0].blend = firstValue * contributionWeight + secondValue * secondWeight;
        count = 1;
    } else {
        current->target[0].targetId = firstId;
        current->target[0].blend = firstValue * contributionWeight;
        current->target[1].targetId = secondId;
        current->target[1].blend = secondValue * secondWeight;
        count = 2;
    }

    contributionWeight = 1.0f - aBlendRate;
    for (previous = aThis->blendSrc.target, previousPairIndex = 0;
         previousPairIndex < 2; previousPairIndex++, previous++) {
        f32 previousContributionWeight = previous->blend * contributionWeight;

        if (!(previousContributionWeight <= 1.0e-6f)) {
            s32 id = previous->targetId;

            if (count == 1) {
                if (current->target[0].targetId == id) {
                    current->target[0].blend += previousContributionWeight;
                } else {
                    current->target[1].targetId = id;
                    current->target[1].blend = previousContributionWeight;
                    count = 2;
                }
            } else if (current->target[0].targetId == id) {
                current->target[0].blend += previousContributionWeight;
            } else if (current->target[1].targetId == id) {
                current->target[1].blend += previousContributionWeight;
            } else if (current->target[0].blend < current->target[1].blend) {
                if (current->target[0].blend < previousContributionWeight) {
                    current->target[0].targetId = id;
                    current->target[0].blend = previousContributionWeight;
                }
            } else {
                if (current->target[1].blend < previousContributionWeight) {
                    current->target[1].targetId = id;
                    current->target[1].blend = previousContributionWeight;
                }
            }
        }

    }

    if (count == 1) {
        current->target[1].targetId = 0;
        current->target[1].blend = 0.0f;
    }

}

void sdfMotCLBK_morphInitBlend(sdfMotNodeMorph_t *aThis) {
    aThis->blendSrc = *aThis->morphNode;
}

sdfMotGenNodeFunc sdfMotGenNodeTable[4] = { sdfMotGenNode, sdfMotGenMatNode, sdfMotGenMorphNode, NULL };
sdfMotGenNodeFunc sdfMotGenNodeDef[5] = { sdfMotGenModelLinerPos, sdfMotGenModelLinerRotXYZ, sdfMotGenModelLinerScale, sdfMotGenModelLerpRotQuat, sdfMotGenModelVisible };
sdfMotionNodeVTBL_t sdfMotModelLinerPosTable = { sdfMotCLBK_Destroy, sdfMotCLBK_SetCut, (void (*)(sdfMotionNode_t *, f32))sdfMotCLBK_modelLinerPosSeek, (void (*)(sdfMotionNode_t *))sdfMotCLBK_modelPosInitBlend, (void (*)(sdfMotionNode_t *, f32, f32))sdfMotCLBK_modelLinerPosBlendSeek };
sdfMotionNodeVTBL_t sdfMotModelLinerRotXYZTable = { sdfMotCLBK_Destroy, sdfMotCLBK_SetCut, (void (*)(sdfMotionNode_t *, f32))sdfMotCLBK_modelLinerRotXYZSeek, (void (*)(sdfMotionNode_t *))sdfMotCLBK_modelRotInitBlend, (void (*)(sdfMotionNode_t *, f32, f32))sdfMotCLBK_modelLinerRotXYZBlendSeek };
sdfMotionNodeVTBL_t sdfMotModelLinerScaleTable = { sdfMotCLBK_Destroy, sdfMotCLBK_SetCut, (void (*)(sdfMotionNode_t *, f32))sdfMotCLBK_modelLinerScaleSeek, (void (*)(sdfMotionNode_t *))sdfMotCLBK_modelScaleInitBlend, (void (*)(sdfMotionNode_t *, f32, f32))sdfMotCLBK_modelLinerScaleBlendSeek };
sdfMotionNodeVTBL_t sdfMotModelLerpRotQuatTable = { sdfMotCLBK_Destroy, sdfMotCLBK_SetCut, (void (*)(sdfMotionNode_t *, f32))sdfMotCLBK_modelLerpRotQuatSeek, (void (*)(sdfMotionNode_t *))sdfMotCLBK_modelRotInitBlend, (void (*)(sdfMotionNode_t *, f32, f32))sdfMotCLBK_modelLerpRotQuatBlendSeek };
sdfMotionNodeVTBL_t sdfMotModelVisibleTable = { sdfMotCLBK_Destroy, sdfMotCLBK_SetCut, (void (*)(sdfMotionNode_t *, f32))sdfMotCLBK_modelVisibleSeek, (void (*)(sdfMotionNode_t *))sdfMotCLBK_modelVisibleInitBlend, (void (*)(sdfMotionNode_t *, f32, f32))sdfMotCLBK_modelVisibleBlendSeek };
sdfMotGenNodeFunc sdfMotGenMatTable[10] = { sdfMotGenMatDiffuse, sdfMotGenMatAmbient, sdfMotGenMatUvw, sdfMotGenMatMultiUvw, sdfMotGenMatMultiDiffuse, sdfMotGenMatConstant, sdfMotGenMatUvw2, sdfMotGenMatMultiUvw2, sdfMotGenMatSpecular, sdfMotGenMatRefMap };
sdfMotionNodeVTBL_t sdfMotMatDiffuseTable = { sdfMotCLBK_Destroy, sdfMotCLBK_SetCut, (void (*)(sdfMotionNode_t *, f32))sdfMotCLBK_matDiffuseSeek, (void (*)(sdfMotionNode_t *))sdfMotCLBK_matDiffuseInitBlend, (void (*)(sdfMotionNode_t *, f32, f32))sdfMotCLBK_matDiffuseBlendSeek };
sdfMotionNodeVTBL_t sdfMotMatAmbientTable = { sdfMotCLBK_Destroy, sdfMotCLBK_SetCut, (void (*)(sdfMotionNode_t *, f32))sdfMotCLBK_matAmbientSeek, (void (*)(sdfMotionNode_t *))sdfMotCLBK_matAmbientInitBlend, (void (*)(sdfMotionNode_t *, f32, f32))sdfMotCLBK_matAmbientBlendSeek };
sdfMotionNodeVTBL_t sdfMotMatSpecularTable = { sdfMotCLBK_Destroy, sdfMotCLBK_SetCut, (void (*)(sdfMotionNode_t *, f32))sdfMotCLBK_matSpecularSeek, (void (*)(sdfMotionNode_t *))sdfMotCLBK_matSpecularInitBlend, (void (*)(sdfMotionNode_t *, f32, f32))sdfMotCLBK_matSpecularBlendSeek };
sdfMotionNodeVTBL_t sdfMotMatRefMapTable = { sdfMotCLBK_Destroy, sdfMotCLBK_SetCut, (void (*)(sdfMotionNode_t *, f32))sdfMotCLBK_matRefMapSeek, (void (*)(sdfMotionNode_t *))sdfMotCLBK_matRefMapInitBlend, (void (*)(sdfMotionNode_t *, f32, f32))sdfMotCLBK_matRefMapBlendSeek };
sdfMotionNodeVTBL_t sdfMotMatConstantTable = { sdfMotCLBK_Destroy, sdfMotCLBK_SetCut, (void (*)(sdfMotionNode_t *, f32))sdfMotCLBK_matConstantSeek, (void (*)(sdfMotionNode_t *))sdfMotCLBK_matConstantInitBlend, (void (*)(sdfMotionNode_t *, f32, f32))sdfMotCLBK_matConstantBlendSeek };
sdfMotionNodeVTBL_t sdfMotMatUvwTable = { sdfMotCLBK_Destroy, sdfMotCLBK_SetCut, (void (*)(sdfMotionNode_t *, f32))sdfMotCLBK_matUvwSeek, (void (*)(sdfMotionNode_t *))sdfMotCLBK_matUvwInitBlend, (void (*)(sdfMotionNode_t *, f32, f32))sdfMotCLBK_matUvwBlendSeek };
sdfMotionNodeVTBL_t sdfMotMatMultiUvwTable = { sdfMotCLBK_Destroy, sdfMotCLBK_SetCut, (void (*)(sdfMotionNode_t *, f32))sdfMotCLBK_matMultiUvwSeek, (void (*)(sdfMotionNode_t *))sdfMotCLBK_matMultiUvwInitBlend, (void (*)(sdfMotionNode_t *, f32, f32))sdfMotCLBK_matMultiUvwBlendSeek };
sdfMotionNodeVTBL_t sdfMotMatMultiDiffuseTable = { sdfMotCLBK_Destroy, sdfMotCLBK_SetCut, (void (*)(sdfMotionNode_t *, f32))sdfMotCLBK_matMultiDiffuseSeek, (void (*)(sdfMotionNode_t *))sdfMotCLBK_matMultiDiffuseInitBlend, (void (*)(sdfMotionNode_t *, f32, f32))sdfMotCLBK_matMultiDiffuseBlendSeek };
sdfMotionNodeVTBL_t sdfMotMatUvw2Table = { sdfMotCLBK_Destroy, sdfMotCLBK_SetCut, (void (*)(sdfMotionNode_t *, f32))sdfMotCLBK_matUvw2Seek, (void (*)(sdfMotionNode_t *))sdfMotCLBK_matUvw2InitBlend, (void (*)(sdfMotionNode_t *, f32, f32))sdfMotCLBK_matUvw2BlendSeek };
sdfMotionNodeVTBL_t sdfMotMatMultiUvw2Table = { sdfMotCLBK_Destroy, sdfMotCLBK_SetCut, (void (*)(sdfMotionNode_t *, f32))sdfMotCLBK_matMultiUvw2Seek, (void (*)(sdfMotionNode_t *))sdfMotCLBK_matMultiUvw2InitBlend, (void (*)(sdfMotionNode_t *, f32, f32))sdfMotCLBK_matMultiUvw2BlendSeek };
sdfMotGenNodeFunc sdfMotGenMorphTable[2] __attribute__((section(".data"))) = { (sdfMotGenNodeFunc)sdfMotGenMorph, NULL };
sdfMotionNodeVTBL_t sdfMotMorphTable = { sdfMotCLBK_Destroy, sdfMotCLBK_SetCut, (void (*)(sdfMotionNode_t *, f32))sdfMotCLBK_morphSeek, (void (*)(sdfMotionNode_t *))sdfMotCLBK_morphInitBlend, (void (*)(sdfMotionNode_t *, f32, f32))sdfMotCLBK_morphBlendSeek };
