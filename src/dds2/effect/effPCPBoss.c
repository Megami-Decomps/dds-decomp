#include "common.h"
#include "sdf_chip.h"
#include "par_cell_api.h"
#include "sdf_resource.h"
#include "eff.h"
#include "eff_param.h"
#include "eff_pcp_boss.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "mdl.h"
#include "sdf_chunk.h"

/* Packed effect parameter-set accessor (see game/code_001624D0). */
extern void *effParamTableGetBlock(void *data, s32 index);
extern void func_0018C820(void *work);
extern void effBossSetPosition(void *dst, void *src);
extern PairedEffectResources *effBossCreatePairedChainResources(PairedEffectParams *src);
extern EffThunderGroup *effThunderChainGroupCreate(EffThunderGroupParams *src);

extern void *memcpy(void *dst, const void *src, u32 size);
extern u32 func_001695C8(void);
extern u32 effBTLFieldColorGetVariantSelector(void);
extern void btlUnitGetMuzzlePosVU(u32 unit);
extern void sdfVuBuildLookAtBasis(void *origin, void *direction, void *up);
extern void sdfInvertRigidVuTransform(void);
extern u8 D_003B1FF0[];

extern EffRecordPool *effRecordPoolCreateFiveVertexGroups(u32 cellCount);
extern void *effGetIndexedEffectGroupRecord(EffRecordPool *pool, s32 index);
extern void *effGetIndexedEffectGroupIndexEntry(EffRecordPool *pool, s32 index);
extern void effSetVectorIncrementBits(EffRecordPool *pool, u32 bits);
extern u32 effMiscRand(void *state);
extern f32 effMiscRandUnitFloat(void *state);
extern u8 effDefaultRandomState[];
extern f32 D_004334C4;
extern void effBossInitializeModelGroups(EffBossWork *work);
extern EffBossWork *effBossCloneWorkAndParameters(EffBossWork *src);
extern void effReleaseRecordGroupAssetAndHandle(EffRecordPool *pool);

/* Randomize geometry and initial age; the two extents remain proportional. */
void effBossCellRandomize(EffBossWork *work, EffBossCell *cell) {
    f32 blend = work->head.offsetRandomness;
    f32 scale = work->head.scale;
    f32 t;

    cell->offsetDistance = work->head.offsetDistance * (effMiscRandUnitFloat(effDefaultRandomState) * blend + (1.0f - blend)) * scale;
    blend = work->head.extentRandomness;
    t = effMiscRandUnitFloat(effDefaultRandomState) * blend + (1.0f - blend);
    cell->baseExtent = work->head.baseExtent * t * scale;
    cell->tipExtent = work->head.tipExtent * t * scale;
    cell->flip = effMiscRand(effDefaultRandomState) & 1;
    cell->age = -(effMiscRand(effDefaultRandomState) % work->head.delaySpread);
}

/* Create a draw pool and delayed cells for each model map-position group. */
void effBossInitializeModelGroups(EffBossWork *work)
{
    MdlCtx *model = effParamWorkGetData(work->paramWork);
    EffBossGroup *group;
    EffBossCell *nextCells;
    u32 i;
    u32 j;

    if ((s32)work->head.delaySpread <= 0) {
        work->head.delaySpread = 1;
    }
    model->first->frameStep = work->head.frameStep;
    mdlAddEntryPlain(model, 0, 0);
    work->cellCount = model->first->frameCount;
    work->groupCount = sdfCountMapPositionRecords(model->inner);
    work->system = parAllocateCellSystem(work->groupCount, work->cellCount, 1, PAR_CELL_TOPOLOGY_TRIANGLE);
    parSetCellDrawBucket(work->system, work->head.drawBucket);
    work->groupsHandle = NULL;
    if (work->head.hasCells) {
        work->groupsHandle = sdfAllocGeneralBlock(work->groupCount * sizeof(EffBossGroup)
            + work->cellCount * work->groupCount * sizeof(EffBossCell));
        nextCells = (EffBossCell *)sdfResourceRetainAddress(work->groupsHandle);
        work->groups = (EffBossGroup *)nextCells;
        nextCells = (EffBossCell *)((u8 *)nextCells + work->groupCount * sizeof(EffBossGroup));
        for (i = 0, group = work->groups; i < work->groupCount; i++, group++) {
            EffBossCell *cell;

            group->drawPool = effRecordPoolCreateFiveVertexGroups(work->cellCount);
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
    work->paramWork = effParamWorkCreate(EFF_PARAM_WORK_KIND_VIEWER_CONTEXT, param1);
    effBossInitializeModelGroups(work);
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
    effBossInitializeModelGroups(work);
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

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_0018C820);

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
    work->modelResource = effParamWorkCreate(EFF_PARAM_WORK_KIND_VIEWER_CONTEXT, paramA);
    work->auxiliaryResource = effParamWorkCreate(EFF_PARAM_WORK_KIND_EXTENDED_WORK_WITH_MATRIX_CALLBACK, paramB);
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

    if (func_001695C8() != 0) {
        if (work->frame == 0) {
            btlUnitGetMuzzlePosVU(effBTLFieldColorGetVariantSelector());
            VU0_STORE_VF(vf10, work->position);
        }
        direction = work->parameterVector;
        sdfVuBuildLookAtBasis(work, direction, D_003B1FF0);
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

u32 func_0018D540(void) {
    return 0;
}

u32 func_0018D548(void) {
    return 0;
}

u32 func_0018D550(void) {
    return 0;
}

void func_0018D558(void) {
}

void func_0018D560(void) {
}

void func_0018D568(void) {
}

u32 func_0018D570(void) {
    return 0;
}

u32 func_0018D578(void) {
    return 0;
}

u32 func_0018D580(void) {
    return 0;
}

void func_0018D588(void) {
}

void func_0018D590(void) {
}

void func_0018D598(void) {
}

void func_0018D5A0(void) {
}

u32 func_0018D5A8(void) {
    return 0;
}

u32 func_0018D5B0(void) {
    return 0;
}

u32 func_0018D5B8(void) {
    return 0;
}

void func_0018D5C0(void) {
}

void func_0018D5C8(void) {
}

void func_0018D5D0(void) {
}

u32 func_0018D5D8(void) {
    return 0;
}

u32 func_0018D5E0(void) {
    return 0;
}

u32 func_0018D5E8(void) {
    return 0;
}

void func_0018D5F0(void) {
}

void func_0018D5F8(void) {
}

void func_0018D600(void) {
}

u32 func_0018D608(void) {
    return 0;
}

u32 func_0018D610(void) {
    return 0;
}

u32 func_0018D618(void) {
    return 0;
}

void func_0018D620(void) {
}

void func_0018D628(void) {
}

void func_0018D630(void) {
}

void func_0018D638(void) {
}

u32 func_0018D640(void) {
    return 0;
}

u32 func_0018D648(void) {
    return 0;
}

u32 func_0018D650(void) {
    return 0;
}

void func_0018D658(void) {
}

void func_0018D660(void) {
}

void func_0018D668(void) {
}

void func_0018D670(void) {
}

u32 func_0018D678(void) {
    return 0;
}

s32 func_0018D680(void) {
    return 0;
}

s32 func_0018D688(void) {
    return 0;
}

void func_0018D690(void) {
}

void func_0018D698(void) {
}

void func_0018D6A0(void) {
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
    func_0018C820(work);
}

void effBossSetPositionCallback(void *work, void *position) {
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
