#include "common.h"
#include "eff.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "mdl.h"

typedef struct EffParamWork EffParamWork;

/* Packed effect parameter-set accessor (see game/code_001624D0). */
extern void *effParamTableGetBlock(void *data, s32 index);
extern void func_00184BC8(void *work);
extern void effBossSetPosition(void *dst, void *src);
extern PairedEffectResources *effBossCreatePairedChainResources(PairedEffectParams *src);
extern EffThunderGroup *effThunderChainGroupCreate(EffThunderGroupParams *src);

extern void *memcpy(void *dst, const void *src, u32 size);
extern void *sdfAllocSizeClassBlock(s32 size);
extern EffParamWork *effParamWorkCreate(s32 kind, void *params);
extern EffParamWork *effParamWorkDuplicate(EffParamWork *param);
extern void effDispatchParameterDataAndFreeWork(EffParamWork *handle);
extern void sdfReleaseChipBlock(void *work);
extern u32 func_001619E8(void);
extern u32 effBTLFieldColorGetVariantSelector(void);
extern void btlUnitGetMuzzlePosVU(u32 unit);
extern void sdfVuBuildLookAtBasis(void *origin, void *direction, void *up);
extern void sdfInvertRigidVuTransform(void);
extern void effParamWorkCallback0(EffParamWork *handle, void *vec);
extern void effParamWorkCallback2(EffParamWork *handle, void *matrix);
extern void effParamWorkCallback3(EffParamWork *handle, u32 value);
extern void effParamWorkInvokeCallback(EffParamWork *handle);
extern u8 D_003556C0[];

/* Boss effect work: two parameter-set handles released on free. */
typedef struct {
    f32 position[4]; /* 0x00 */
    u32 parameterVector[4]; /* 0x10 copied from the parameter block */
    s32 frame;      /* 0x20 counts updates */
    u32 color;      /* 0x24 set by effPCPBossSetParameter, starts 0x80808080 */
    EffParamWork *modelResource; /* Kind 3: supplies the model effect. */
    EffParamWork *auxiliaryResource; /* Kind 6: receives the same basis, vector and tint. */
} EffPCPBossWork;

/* Trail parameters copied verbatim on spawn. Trail colors use the owner frame;
   cell colors and offset ramping use each cell's initially non-positive age. */
typedef struct {
    f32 position[4];
    u8 drawTrail;
    u8 pad11[3];
    f32 unk14;        /* Copied into the motion node's unknown float at 0x20. */
    f32 scale;        /* Applied to model scale and randomized cell extents. */
    f32 trailWidth;
    s32 trailColorStartFrame;
    s32 trailColorTransitionFrames;
    u32 trailStartCenterColor;
    u32 trailStartEdgeColor;
    u32 trailEndCenterColor;
    u32 trailEndEdgeColor;
    u16 systemParam;  /* 0x38 */
    u8 pad3A[2];
    u8 hasCells;      /* 0x3C */
    u8 cellCountFromFrame; /* Mode 1 uses the owner frame as the active sample count. */
    u8 pad3E[2];
    s32 cellStartFrame;
    u32 delaySpread;
    s32 cellDuration;
    s32 cellFadeIn;
    s32 cellFadeOut;
    s32 offsetRampFrames;
    f32 offsetDistance;
    f32 offsetRandomness;
    f32 baseExtent;
    f32 tipExtent;
    f32 extentRandomness;
    s32 cellColorStartAge;
    s32 cellColorTransitionFrames;
    u32 cellStartCenterColor;
    u32 cellStartEdgeColor;
    u32 cellEndCenterColor;
    u32 cellEndEdgeColor;
    u32 incrementBits; /* 0x84 */
    u8 directionMode; /* 0: negative Y; 1: positive Y; 2: model-relative direction. */
    u8 pad89[3];
} EffBossHead; /* 0x8C */

/* Parameter block as read by effBossCreateWithGroups: two words follow the head. */
typedef struct {
    EffBossHead head;
    s32 rotationStartAge;
    f32 angularSpeed;
} EffBossParams;

/* Random offset distance and the extents at the base/tip of a trail cell.
   Both extents share one random factor; age starts at a non-positive delay. */
typedef struct {
    f32 offsetDistance;
    f32 baseExtent;
    f32 tipExtent;
    s32 age;
    u8 flip;          /* 0x10 */
    u8 pad11[3];
} EffBossCell; /* 0x14 */

typedef struct {
    EffRecordPool *drawPool;
    f32 direction[3]; /* Unit Y initially; mode 2 refreshes it from the model. */
    u8 pad10[4];
    f32 rotationAngle;
    s32 rotationStartAge;
    f32 angularSpeed;
    EffBossCell *cells; /* 0x20 */
} EffBossGroup; /* 0x24 */

typedef struct {
    EffBossHead head;
    EffBossGroup *groups; /* 0x8C */
    SdfMemBlock *groupsHandle; /* 0x90 */
    u32 groupCount;   /* 0x94 */
    u16 cellCount;    /* 0x98 */
    u8 pad9A[2];
    u32 frame;
    u32 color;        /* 0xA0 */
    u32 system;       /* 0xA4 */
    EffParamWork *paramWork; /* 0xA8 */
} EffBossWork;

/* Five packed RGBA values paired with the draw pool's five vertex positions.
   Native update writes tinted colors here; these are not vertex indices. */
typedef struct {
    u32 color[5];
} EffBossColorSlot;


extern void *effParamWorkGetData(EffParamWork *handle);
extern void mdlAddEntryPlain(void *work, s32 arg1, s32 arg2);
extern u32 sdfCountMapPositionRecords(void *chunk);
extern u32 parAllocateCellSystem(s32 count, s32 perCell, s32 groupDivisor, u32 kind);
extern void func_0015D078(u32 system, u32 value);
extern SdfMemBlock *sdfAllocGeneralBlock(s32 size);
extern u32 sdfResourceRetainAddress(SdfMemBlock *handle);
extern EffRecordPool *func_0016FB08(u32 cellCount);
extern void *effGetIndexedEffectGroupRecord(EffRecordPool *pool, s32 index);
extern EffBossColorSlot *effGetIndexedEffectGroupIndexEntry(EffRecordPool *pool, s32 index);
extern void effSetVectorIncrementBits(EffRecordPool *pool, u32 bits);
extern u32 effMiscRand(void *state);
extern f32 effMiscRandUnitFloat(void *state);
extern u8 D_0034DF38[];
extern f32 D_003B9308;
extern void func_00184630(EffBossWork *work);
extern EffBossWork *effBossCloneWorkAndParameters(EffBossWork *src);
extern void effReleaseRecordGroupAssetAndHandle(EffRecordPool *pool);
extern void sdfReleaseResourceAllocation(SdfMemBlock *handle);
extern void parReleaseCellSystem(u32 system);

/* Randomize geometry and initial age; the two extents remain proportional. */
void effBossCellRandomize(EffBossWork *work, EffBossCell *cell) {
    f32 blend = work->head.offsetRandomness;
    f32 scale = work->head.scale;
    f32 t;

    cell->offsetDistance = work->head.offsetDistance * (effMiscRandUnitFloat(D_0034DF38) * blend + (1.0f - blend)) * scale;
    blend = work->head.extentRandomness;
    t = effMiscRandUnitFloat(D_0034DF38) * blend + (1.0f - blend);
    cell->baseExtent = work->head.baseExtent * t * scale;
    cell->tipExtent = work->head.tipExtent * t * scale;
    cell->flip = effMiscRand(D_0034DF38) & 1;
    cell->age = -(effMiscRand(D_0034DF38) % work->head.delaySpread);
}

/* Create a draw pool and delayed cells for each model map-position group. */
void func_00184630(EffBossWork *work)
{
    MdlCtx *model = effParamWorkGetData(work->paramWork);
    EffBossGroup *group;
    EffBossCell *nextCells;
    u32 i;
    u32 j;

    if ((s32)work->head.delaySpread <= 0) {
        work->head.delaySpread = 1;
    }
    model->first->frameStep = work->head.unk14;
    mdlAddEntryPlain(model, 0, 0);
    work->cellCount = model->first->frameCount;
    work->groupCount = sdfCountMapPositionRecords(model->inner);
    work->system = parAllocateCellSystem(work->groupCount, work->cellCount, 1, 1);
    func_0015D078(work->system, work->head.systemParam);
    work->groupsHandle = NULL;
    if (work->head.hasCells) {
        work->groupsHandle = sdfAllocGeneralBlock(work->groupCount * sizeof(EffBossGroup)
            + work->cellCount * work->groupCount * sizeof(EffBossCell));
        nextCells = (EffBossCell *)sdfResourceRetainAddress(work->groupsHandle);
        work->groups = (EffBossGroup *)nextCells;
        nextCells = (EffBossCell *)((u8 *)nextCells + work->groupCount * sizeof(EffBossGroup));
        for (i = 0, group = work->groups; i < work->groupCount; i++, group++) {
            EffBossCell *cell;

            group->drawPool = func_0016FB08(work->cellCount);
            if (work->head.directionMode == 0) {
                group->direction[0] = 0.0f;
                group->direction[1] = -1.0f;
                group->direction[2] = 0.0f;
            } else {
                group->direction[0] = 0.0f;
                group->direction[1] = 1.0f;
                group->direction[2] = 0.0f;
            }
            group->angularSpeed = (0.1f * (3.14159265f / 180.0f));
            group->rotationAngle = 0.0f;
            for (j = 0; j < work->cellCount; j++) {
                f32 *vertices = (f32 *)effGetIndexedEffectGroupRecord(group->drawPool, j);
                EffBossColorSlot *colors = effGetIndexedEffectGroupIndexEntry(group->drawPool, j);

                colors->color[0] = 0;
                colors->color[1] = 0;
                colors->color[2] = 0;
                colors->color[3] = 0;
                colors->color[4] = 0;
                VU0_MOVE_VF(vf10, vf0);
                VU0_STORE_VF(vf10, vertices);
                VU0_STORE_VF(vf10, vertices + 4);
                VU0_STORE_VF(vf10, vertices + 8);
                VU0_STORE_VF(vf10, vertices + 12);
                VU0_STORE_VF(vf10, vertices + 16);
            }
            effSetVectorIncrementBits(group->drawPool, work->head.incrementBits);
            group->cells = nextCells;
            nextCells += work->cellCount;
            for (j = 0, cell = group->cells; j < work->cellCount; j++, cell++) {
                effBossCellRandomize(work, cell);
            }
        }
    }
}

/* Copy the trail head and create its model, cell system and per-map groups. */
EffBossWork *effBossCreate(EffBossParams *src, void *param1) {
    EffBossWork *work;

    work = sdfAllocSizeClassBlock(sizeof(EffBossWork));
    work->head = src->head;
    work->frame = 0;
    work->color = 0x80808080;
    work->paramWork = effParamWorkCreate(3, param1);
    func_00184630(work);
    return work;
}

/* Apply the first two packed parameter blocks to the boss effect. */
void effPCPBossApplyTwoBlocks(void *data) {
    void *firstBlock;
    void *secondBlock;

    firstBlock = effParamTableGetBlock(data, 0);
    secondBlock = effParamTableGetBlock(data, 1);
    effBossCreate(firstBlock, secondBlock);
}

/* Duplicate model parameters and rebuild groups with a fresh frame counter. */
EffBossWork *effBossCloneWorkAndParameters(EffBossWork *src) {
    EffBossWork *work;

    work = sdfAllocSizeClassBlock(sizeof(EffBossWork));
    memcpy(&work->head, &src->head, sizeof(EffBossHead));
    work->frame = 0;
    work->color = 0x80808080;
    work->paramWork = effParamWorkDuplicate(src->paramWork);
    func_00184630(work);
    return work;
}

/* Release group draw pools, the shared cell system, model resource and owner. */
void effBossDestroy(EffBossWork *work) {
    u32 i;

    if (work->groupsHandle != 0) {
        for (i = 0; i < work->groupCount; i++) {
            effReleaseRecordGroupAssetAndHandle(work->groups[i].drawPool);
        }
        sdfReleaseResourceAllocation(work->groupsHandle);
    }
    parReleaseCellSystem(work->system);
    effDispatchParameterDataAndFreeWork(work->paramWork);
    sdfReleaseChipBlock(work);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_00184BC8);

void effBossSetPosition(void *dst, void *src)
{
    PCP_COPY_VECTOR(dst, src);
}

void effBossTrailSetColor(EffBossWork *work, s32 value) {
    work->color = value;
}

/* Copy the beam vector and create its model and auxiliary parameter works. */
EffPCPBossWork *effBossBeamCreate(void *vector, void *paramA, void *paramB) {
    EffPCPBossWork *work;

    work = sdfAllocSizeClassBlock(sizeof(EffPCPBossWork));
    memcpy(work->parameterVector, vector, 0x10);
    work->color = 0x80808080;
    work->frame = 0;
    work->modelResource = effParamWorkCreate(3, paramA);
    work->auxiliaryResource = effParamWorkCreate(6, paramB);
    return work;
}

/* Apply three packed parameter blocks to the boss effect. */
void effPCPBossApplyThreeBlocks(void *data) {
    void *firstBlock;
    void *secondBlock;
    void *thirdBlock;

    firstBlock = effParamTableGetBlock(data, 0);
    secondBlock = effParamTableGetBlock(data, 1);
    thirdBlock = effParamTableGetBlock(data, 2);
    effBossBeamCreate(firstBlock, secondBlock, thirdBlock);
}

/* Duplicate both parameter works; the clone begins at frame zero. */
EffPCPBossWork *effBossBeamClone(EffPCPBossWork *src) {
    EffPCPBossWork *work;

    work = sdfAllocSizeClassBlock(sizeof(EffPCPBossWork));
    memcpy(work->parameterVector, src->parameterVector, 0x10);
    work->color = 0x80808080;
    work->frame = 0;
    work->modelResource = effParamWorkDuplicate(src->modelResource);
    work->auxiliaryResource = effParamWorkDuplicate(src->auxiliaryResource);
    return work;
}

/* Release both beam parameter works before freeing the owner. */
void effPCPBossFree(EffPCPBossWork *work) {
    effDispatchParameterDataAndFreeWork(work->auxiliaryResource);
    effDispatchParameterDataAndFreeWork(work->modelResource);
    sdfReleaseChipBlock(work);
}

/* Per-frame update: basis from the muzzle position (or identity) fed to both parameter works. */
void effBossBeamUpdate(EffPCPBossWork *work) {
    u128 matrix[4];
    void *direction;

    if (func_001619E8() != 0) {
        if (work->frame == 0) {
            btlUnitGetMuzzlePosVU(effBTLFieldColorGetVariantSelector());
            VU0_STORE_VF(vf10, work->position);
        }
        direction = work->parameterVector;
        sdfVuBuildLookAtBasis(work, direction, D_003556C0);
        sdfInvertRigidVuTransform();
        VU0_MOVE_VF(vf31, vf0);
        VU0_STORE_MATRIX(matrix);
    } else {
        work->position[0] = 0;
        work->position[1] = 0;
        work->position[2] = 400.0f;
        work->position[3] = 0;
        VU0_STORE_VF(vf0, work->position);
        EE_MMI_UNIT_MATRIX(matrix);
        direction = work->parameterVector;
    }
    effParamWorkCallback2(work->modelResource, matrix);
    effParamWorkCallback2(work->auxiliaryResource, matrix);
    effParamWorkCallback0(work->modelResource, direction);
    effParamWorkCallback0(work->auxiliaryResource, direction);
    effParamWorkCallback3(work->modelResource, work->color);
    effParamWorkCallback3(work->auxiliaryResource, work->color);
    effParamWorkInvokeCallback(work->modelResource);
    effParamWorkInvokeCallback(work->auxiliaryResource);
    work->frame++;
}

void effPCPBossSetParameterVector(EffPCPBossWork *work, void *src) {
    PCP_COPY_VECTOR(&work->parameterVector, src);
}

void effPCPBossSetParameter(EffPCPBossWork *work, u32 value) {
    work->color = value;
}

u32 func_001858E8(void) {
    return 0;
}

u32 func_001858F0(void) {
    return 0;
}

u32 func_001858F8(void) {
    return 0;
}

void func_00185900(void) {
}

void func_00185908(void) {
}

void func_00185910(void) {
}

u32 func_00185918(void) {
    return 0;
}

u32 func_00185920(void) {
    return 0;
}

u32 func_00185928(void) {
    return 0;
}

void func_00185930(void) {
}

void func_00185938(void) {
}

void func_00185940(void) {
}

void func_00185948(void) {
}

void func_00185950(void) {
}

void func_00185958(void) {
}

u32 func_00185960(void) {
    return 0;
}

void func_00185968(void) {
}

void func_00185970(void) {
}

void func_00185978(void) {
}

void func_00185980(void) {
}

void func_00185988(void) {
}

void func_00185990(void) {
}

void func_00185998(void) {
}

void func_001859A0(void) {
}

void func_001859A8(void) {
}

u32 func_001859B0(void) {
    return 0;
}

u32 func_001859B8(void) {
    return 0;
}

u32 func_001859C0(void) {
    return 0;
}

void func_001859C8(void) {
}

void func_001859D0(void) {
}

void func_001859D8(void) {
}

void func_001859E0(void) {
}

u32 func_001859E8(void) {
    return 0;
}

u32 func_001859F0(void) {
    return 0;
}

u32 func_001859F8(void) {
    return 0;
}

void func_00185A00(void) {
}

void func_00185A08(void) {
}

void func_00185A10(void) {
}

void func_00185A18(void) {
}

u32 func_00185A20(void) {
    return 0;
}

s32 func_00185A28(void) {
    return 0;
}

s32 func_00185A30(void) {
    return 0;
}

void func_00185A38(void) {
}

void func_00185A40(void) {
}

void func_00185A48(void) {
}

/* Apply per-cell rotation onset and speed after creating the trail groups. */
EffBossWork *effBossCreateWithGroups(EffBossParams *src, void *param1) {
    EffBossWork *work;
    u32 i;

    work = effBossCreate(src, param1);
    for (i = 0; i < work->groupCount; i++) {
        work->groups[i].rotationStartAge = src->rotationStartAge;
        work->groups[i].rotationAngle = 0.0f;
        work->groups[i].angularSpeed = src->angularSpeed;
    }
    return work;
}

void effBossCreateGroupsFromPackedParams(void *data) {
    void *work0;
    void *work1;

    work0 = effParamTableGetBlock(data, 0);
    work1 = effParamTableGetBlock(data, 1);
    effBossCreateWithGroups(work0, work1);
}

/* Preserve rotation settings, but restart every group's accumulated angle. */
EffBossWork *effBossCloneWithGroups(EffBossWork *src) {
    EffBossWork *work;
    u32 i;

    work = effBossCloneWorkAndParameters(src);
    for (i = 0; i < work->groupCount; i++) {
        work->groups[i].rotationStartAge = src->groups[i].rotationStartAge;
        work->groups[i].rotationAngle = 0.0f;
        work->groups[i].angularSpeed = src->groups[i].angularSpeed;
    }
    return work;
}

void effBossReleaseWorkCallback(void *work) {
    effBossDestroy(work);
}

void effBossUpdateGeometryCallback(void *work) {
    func_00184BC8(work);
}

void func_00185BA8(void *work, void *position) {
    effBossSetPosition(work, position);
}

void effBossApplyGroupTint(EffBossWork *work, s32 value) {
    effBossTrailSetColor(work, value);
}

/* Create paired four-point chains; the update fills their point vectors. */
PairedEffectResources *effBossCreatePairedChainResources(PairedEffectParams *src)
{
    PairedEffectResources *work = sdfAllocSizeClassBlock(sizeof(PairedEffectResources));
    EffThunderGroupParams chain;

    memcpy(work, src, sizeof(PairedEffectParams));
    work->colorWithAlpha = 0x80808080;
    work->frame = 0;
    chain.params = work->fragment;
    chain.count = 4;
    work->resource[0] = effThunderChainGroupCreate(&chain);
    work->resource[1] = effThunderChainGroupCreate(&chain);
    return work;
}

PairedEffectResources *effBossCreatePairedChainsFromTable(void *data) {
    void *work;

    work = effParamTableGetBlock(data, 0);
    return effBossCreatePairedChainResources(work);
}

PairedEffectResources *effBossCreatePairedChainsFromParams(void *work) {
    return effBossCreatePairedChainResources(work);
}
