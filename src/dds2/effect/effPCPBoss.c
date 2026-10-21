#include "sdk_ps2.h"
#include "common.h"
#include "btl_effect_position.h"
#include "eff_random.h"
#include "sdf_chip.h"
#include "par_cell_api.h"
#include "sdf_resource.h"
#include "eff.h"
#include "eff_param.h"
#include "eff_pcp_boss.h"
#include "pcp_vu0.h"
#include "sdf_vu_transform_api.h"
#include "ee_mmi.h"
#include "mdl.h"
#include "sdf_chunk.h"
#include "sdf_projection.h"

extern void effBossUpdateGeometry(void *work);
extern void effBossSetPosition(void *dst, void *src);
extern PairedEffectResources *effBossCreatePairedChainResources(PairedEffectParams *src);
extern EffThunderGroup *effThunderChainGroupCreate(EffThunderGroupParams *src);

extern u32 effBTLFieldColorUsesActorSelectors(void);
extern u32 effBTLFieldColorGetVariantSelector(void);
extern u8 effBossLookAtReferenceAxis[];

extern EffRecordPool *effRecordPoolCreateFiveVertexGroups(u32 cellCount);
extern void *effGetIndexedEffectGroupRecord(EffRecordPool *pool, s32 index);
extern void *effGetIndexedEffectGroupIndexEntry(EffRecordPool *pool, s32 index);
extern void effSetVectorIncrementBits(EffRecordPool *pool, u32 bits);
extern EffRandState effDefaultRandomState[];
extern f32 D_004334C4;
extern void effBossInitializeModelGroups(EffBossWork *work);
extern EffBossWork *effBossCloneWorkAndParameters(EffBossWork *src);
extern void effReleaseRecordGroupAssetAndHandle(EffRecordPool *pool);

/* Randomize geometry and initial age; the two extents remain proportional. */
void effBossCellRandomize(EffBossWork *work, EffBossCell *cell) {
    f32 blend = work->head.offsetRandomness;
    f32 scale = work->head.scale;
    f32 t;

    cell->offsetDistance = work->head.offsetDistance * (sdfGetRandFloat(effDefaultRandomState) * blend + (1.0f - blend)) * scale;
    blend = work->head.extentRandomness;
    t = sdfGetRandFloat(effDefaultRandomState) * blend + (1.0f - blend);
    cell->baseExtent = work->head.baseExtent * t * scale;
    cell->tipExtent = work->head.tipExtent * t * scale;
    cell->flip = sdfGetRand((EffRandState *)effDefaultRandomState) & 1;
    cell->age = -(sdfGetRand((EffRandState *)effDefaultRandomState) % work->head.delaySpread);
}

/* Create a draw pool and delayed cells for each model map-position group. */
void effBossInitializeModelGroups(EffBossWork *work)
{
    MdlCtx *model = effParamWorkGetData(work->paramWork);
    EffBossGroup *group;
    EffBossCell *nextCells;
    u32 groupIndex;
    u32 cellIndex;

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
        work->groupsHandle = sdfMemAllocDirect(work->groupCount * sizeof(EffBossGroup)
            + work->cellCount * work->groupCount * sizeof(EffBossCell));
        nextCells = (EffBossCell *)sdfMemLock(work->groupsHandle);
        work->groups = (EffBossGroup *)nextCells;
        nextCells = (EffBossCell *)((u8 *)nextCells + work->groupCount * sizeof(EffBossGroup));
        for (groupIndex = 0, group = work->groups; groupIndex < work->groupCount; groupIndex++, group++) {
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
            for (cellIndex = 0; cellIndex < work->cellCount; cellIndex++) {
                f32 *vertices = (f32 *)effGetIndexedEffectGroupRecord(group->drawPool, cellIndex);
                EffBossColorSlot *colors = effGetIndexedEffectGroupIndexEntry(group->drawPool, cellIndex);

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
            for (cellIndex = 0, cell = group->cells; cellIndex < work->cellCount; cellIndex++, cell++) {
                effBossCellRandomize(work, cell);
            }
        }
    }
}

/* Copy the trail head and create its model, cell system and per-map groups. */
EffBossWork *effBossCreate(EffBossParams *bossParams, void *effectParams) {
    EffBossWork *work;

    work = sdfChipAllocDirect(sizeof(EffBossWork));
    work->head = bossParams->head;
    work->frame = 0;
    work->color = 0x80808080;
    work->paramWork = effParamWorkCreate(EFF_PARAM_WORK_KIND_VIEWER_CONTEXT, effectParams);
    effBossInitializeModelGroups(work);
    return work;
}

/* Apply the first two packed parameter blocks to the boss effect. */
EffBossWork *effPCPBossApplyTwoBlocks(void *parameterTable) {
    EffBossParams *bossParams;
    void *effectParams;

    bossParams = effParamTableGetBlock(parameterTable, 0);
    effectParams = effParamTableGetBlock(parameterTable, 1);
    return effBossCreate(bossParams, effectParams);
}

/* Duplicate model parameters and rebuild groups with a fresh frame counter. */
EffBossWork *effBossCloneWorkAndParameters(EffBossWork *src) {
    EffBossWork *work;

    work = sdfChipAllocDirect(sizeof(EffBossWork));
    memcpy(&work->head, &src->head, sizeof(EffBossHead));
    work->frame = 0;
    work->color = 0x80808080;
    work->paramWork = effParamWorkDuplicate(src->paramWork);
    effBossInitializeModelGroups(work);
    return work;
}

/* Release group draw pools, the shared cell system, model resource and owner. */
void effBossDestroy(EffBossWork *work) {
    u32 groupIndex;

    if (work->groupsHandle != 0) {
        for (groupIndex = 0; groupIndex < work->groupCount; groupIndex++) {
            effReleaseRecordGroupAssetAndHandle(work->groups[groupIndex].drawPool);
        }
        sdfMemFreeDirect(work->groupsHandle);
    }
    parReleaseCellSystem(work->system);
    effDispatchParameterDataAndFreeWork(work->paramWork);
    sdfChipFreeDirect(work);
}

#include "eff_math.h"
#include "sdf_quaternion.h"
extern u32 effMultiplyPackedColors(u32, u32);
extern struct SdfPoolNode *D_00380828[];
extern f32 D_003B1FE0[4];
extern s32 sdfLoadMapRecordLookAtBasis(struct SdfModel *, s32);
extern void effSetFanRecordPoolColor(EffRecordPool *, u32);
extern void effDrawScaledRecordPool(EffRecordPool *);

/* Independent native reconstruction; prior KiB/Opal/Brass algorithms credited. */
void effBossUpdateGeometry(void *opaque) {
    EffBossWork *work = opaque;
    f32 perpendicular[4];
    f32 direction[4];
    f32 axis[4];
    f32 scaleVector[4];
    f32 widthVector[4];
    f32 displacement[4];
    f32 baseVector[4];
    f32 tipVector[4];
    MdlCtx *model;
    struct SdfModel *map;
    s32 i;
    s32 colorStart;
    s32 mode;
    u32 tint;
    u32 center;
    u128 *historyStart;
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
    s32 ramp;
    s32 colorFrames;
    u8 cellCountFromFrame;
    EffBossGroup *group;
    EffBossCell *cell;
    ParCell *historyCell;
    u128 *positions; /* Position workspace reused for trail history and cell vertices. */
    u32 *colorWords;
    s32 frame;
    s32 groupCount;
    s32 colorCellCount;
    s32 j, count;
    s32 historyCount;
    s32 age, currentAge;
    u32 edge, alphaColor;
    f32 blend, trailLength, cellLength, taperStep;
    f32 taperRange;
    f32 baseExtent, tipExtent;

    frame = work->frame;
    mode = work->head.directionMode;
    model = effParamWorkGetData(work->paramWork);
    map = model->inner;
    VU0_LOAD_VF(vf10, work->head.position);
    mdlStorePrimaryVectorVU(model);
    VEC3_SPLAT(scaleVector, work->head.scale);
    scaleVector[3] = 0.0f;
    VU0_LOAD_VF(vf10, scaleVector);
    mdlStoreTertiaryVectorVU(model);
    mdlProcessContextNodesAndTransforms(model, D_00380828);
    if (work->head.drawTrail) {
        colorStart = work->head.trailColorStartFrame;
        if (frame > colorStart) {
            if (work->head.trailColorTransitionFrames > 0) {
                blend = (f32)(frame - colorStart) / work->head.trailColorTransitionFrames;
                center = effBlendColor(work->head.trailStartCenterColor, work->head.trailEndCenterColor, blend);
                edge = effBlendColor(work->head.trailStartEdgeColor, work->head.trailEndEdgeColor, blend);
            } else {
                center = work->head.trailEndCenterColor;
                edge = work->head.trailEndEdgeColor;
            }
        } else {
            center = work->head.trailStartCenterColor;
            edge = work->head.trailStartEdgeColor;
        }
        groupCount = work->groupCount;
        for (i = 0; i < groupCount; i++) {
            colorWords = work->system->cells[i].colors;
            colorCellCount = work->cellCount;
            if (colorCellCount != 0) {
                j = colorCellCount;
                do {
                    colorWords[0] = edge;
                    colorWords[1] = center;
                    colorWords[2] = edge;
                    colorWords += 3;
                    --j;
                } while (j != 0);
            }
            groupCount = work->groupCount;
        }
    }
    groupCount = work->groupCount;
    VEC3_SPLAT(widthVector, (work->head.trailWidth * 0.5f) * scaleVector[0]);
    widthVector[3] = 0.0f;
    perpendicular[0] = perpendicular[1] = perpendicular[2] = perpendicular[3] = widthVector[3];
    tint = work->color;
    i = 0;
    if (groupCount > 0) {
        taperRange = -1.0f;
        do {
            historyCell = &work->system->cells[i];
            positions = historyCell->history;
            historyCell->color = tint;
            if (mode == 2) {
                direction[0] = 0.0f;
                direction[1] = 0.0f;
                direction[2] = 1.0f;
                sdfLoadMapRecordLookAtBasis(map, i);
                VU0_LOAD_VF(vf10, direction);
                VU0_ROTATE_VEC(vf10, vf10);
                VU0_STORE_VF(vf10, direction);
                work->groups[i].direction[0] = direction[0];
                work->groups[i].direction[1] = direction[1];
                work->groups[i].direction[2] = direction[2];
            }
            sdfLoadMapRecordPositionVector(map, i);
            if (frame < work->cellCount) {
                historyCount = historyCell->vertexCount;
                VU0_STORE_VF_UNCLOBBERED(vf10, positions + historyCount);
                VU0_STORE_VF_UNCLOBBERED(vf10, positions + historyCount + 1);
                VU0_STORE_VF_UNCLOBBERED(vf10, positions + historyCount + 2);
                historyCell->vertexCount += 3;
                taperStep = taperRange / frame;
                blend = 1.0f;
                VU0_LOAD_VF(vf12, D_003B1FE0);
                if (frame > 0) {
                    const f32 *trailWidth = widthVector;
                    j = frame;
                    do {
                        VU0_LOAD_VF(vf10, positions + 4);
                        VU0_LOAD_VF(vf11, positions + 1);
                        VU0_SUB(vf10, vf10, vf11);
                        VU0_LENGTH_VF10(trailLength);
                        if (trailLength > 1.0f) {
                            VU0_MOVE_VF(vf11, vf12);
                            VU0_CROSS_XYZ(vf10, vf10, vf11);
                            VU0_NORMALIZE_VF10();
                            VU0_STORE_VF(vf10, perpendicular);
                        } else {
                            VU0_LOAD_VF(vf10, perpendicular);
                        }
                        VU0_LOAD_VF(vf11, trailWidth);
                        VU0_SCALE_VF(vf11, blend);
                        VU0_MUL(vf10, vf10, vf11);
                        VU0_MOVE_VF(vf11, vf10);
                        VU0_LOAD_VF(vf10, positions + 1);
                        VU0_ADD(vf10, vf10, vf11);
                        VU0_STORE_VF(vf10, positions);
                        VU0_SUB(vf10, vf10, vf11);
                        VU0_SUB(vf10, vf10, vf11);
                        VU0_STORE_VF(vf10, positions + 2);
                        --j;
                        blend += taperStep;
                        positions += 3;
                    } while (j != 0);
                }
            }
            i++;
        } while (i < groupCount);
    }
    if (work->head.drawTrail) {
        parPrependCellNode(work->system);
    }
    if (work->groupsHandle != NULL) {
        duration = work->head.cellDuration;
        count = frame - work->head.cellStartFrame;
        fadeIn = work->head.cellFadeIn;
        fadeOut = work->head.cellFadeOut;
        colorStart = work->head.cellColorStartAge;
        colorFrames = work->head.cellColorTransitionFrames;
        cellCountFromFrame = work->head.cellCountFromFrame;
        if (count > 0) {
            ramp = work->head.offsetRampFrames;
            if (cellCountFromFrame == 1) count = frame;
            if (count > work->cellCount) count = work->cellCount;
            group = work->groups;
            for (i = 0; i < groupCount; i++, group++) {
                historyCell = &work->system->cells[i];
                historyStart = historyCell->history;
                cell = group->cells;
                effSetFanRecordPoolColor(group->drawPool, tint);
                for (j = 0; j < count - 1; j++, cell++, historyStart += 3) {
                    positions = effGetIndexedEffectGroupRecord(group->drawPool, j);
                    colorWords = ((EffBossColorSlot *)effGetIndexedEffectGroupIndexEntry(group->drawPool, j))->color;
                    age = cell->age;
                    if (age > colorStart) {
                        if (work->head.cellColorTransitionFrames > 0 && age < colorStart + colorFrames) {
                            blend = (f32)(age - colorStart) / colorFrames;
                            center = effBlendColor(work->head.cellStartCenterColor, work->head.cellEndCenterColor, blend);
                            edge = effBlendColor(work->head.cellStartEdgeColor, work->head.cellEndEdgeColor, blend);
                        } else {
                            center = work->head.cellEndCenterColor;
                            edge = work->head.cellEndEdgeColor;
                        }
                    } else {
                        center = work->head.cellStartCenterColor;
                        edge = work->head.cellStartEdgeColor;
                    }
                    center &= 0xFFFFFF;
                    edge &= 0xFFFFFF;
                    currentAge = cell->age;
                    if (currentAge >= 0) {
                        if (currentAge >= ramp) direction[1] = cell->offsetDistance;
                        else direction[1] = (cell->offsetDistance * currentAge) / ramp;
                        displacement[0] = group->direction[0] * direction[1];
                        displacement[1] = group->direction[1] * direction[1];
                        displacement[2] = group->direction[2] * direction[1];
                        displacement[3] = 0.0f;
                        if (mode == 2 && currentAge >= group->rotationStartAge) {
                            VU0_LOAD_VF(vf10, historyStart + 4);
                            VU0_LOAD_VF(vf11, historyStart + 1);
                            VU0_SUB(vf10, vf10, vf11);
                            VU0_NORMALIZE_VF10();
                            VU0_STORE_VF_UNCLOBBERED(vf10, axis);
                            sdfBuildVuRotationFromAxisAngle(group->rotationAngle, (const struct RwV3d *)axis);
                            VU0_LOAD_VF(vf10, displacement);
                            VU0_ROTATE_VEC(vf10, vf10);
                            VU0_STORE_VF(vf10, displacement);
                            group->rotationAngle += group->angularSpeed;
                        }
                        baseExtent = cell->baseExtent;
                        VEC3_SPLAT(baseVector, baseExtent);
                        baseVector[3] = 0.0f;
                        tipExtent = cell->tipExtent;
                        VEC3_SPLAT(tipVector, tipExtent);
                        tipVector[3] = 0.0f;
                        VU0_LOAD_VF(vf10, historyStart + 4);
                        VU0_LOAD_VF(vf11, historyStart + 1);
                        VU0_SUB(vf10, vf10, vf11);
                        VU0_LENGTH_VF10(cellLength);
                        if (cellLength > 1.0f) {
                            VU0_NORMALIZE_VF10();
                            VU0_MOVE_VF(vf11, vf10);
                            VU0_LOAD_VF(vf10, tipVector);
                            VU0_MUL(vf10, vf10, vf11);
                            VU0_STORE_VF(vf10, tipVector);
                            VU0_LOAD_VF(vf10, baseVector);
                            VU0_MUL(vf10, vf10, vf11);
                            VU0_STORE_VF(vf10, baseVector);
                            VU0_LOAD_VF(vf12, displacement);
                            VU0_LOAD_VF(vf10, historyStart + 4);
                            VU0_LOAD_VF(vf11, baseVector);
                            VU0_STORE_VF(vf10, positions + 3);
                            VU0_ADD(vf10, vf10, vf11);
                            VU0_STORE_VF(vf10, positions + 2);
                            VU0_SUB(vf10, vf10, vf11);
                            VU0_SUB(vf10, vf10, vf11);
                            VU0_STORE_VF(vf10, positions + 4);
                            VU0_LOAD_VF(vf10, historyStart + 4);
                            VU0_LOAD_VF(vf11, tipVector);
                            VU0_ADD(vf10, vf10, vf12);
                            VU0_ADD(vf10, vf10, vf11);
                            VU0_STORE_VF(vf10, positions);
                            VU0_SUB(vf10, vf10, vf11);
                            VU0_SUB(vf10, vf10, vf11);
                            VU0_STORE_VF(vf10, positions + 1);
                        } else {
                            VU0_MOVE_VF(vf10, vf0);
                            VU0_STORE_VF(vf10, positions);
                            VU0_STORE_VF(vf10, positions + 1);
                            VU0_STORE_VF(vf10, positions + 2);
                            VU0_STORE_VF(vf10, positions + 3);
                            VU0_STORE_VF(vf10, positions + 4);
                        }
                        alphaColor = 0;
                        if (age <= duration) {
                            if (age < fadeIn && fadeIn != 0) {
                                blend = (f32)age / fadeIn;
                                alphaColor = effMultiplyPackedColors(((u32)(blend * 2147483648.0f) & 0xFF000000) | 0x808080, tint);
                            } else if (duration - age <= fadeOut && fadeOut != 0) {
                                blend = (f32)(duration - age) / fadeOut;
                                alphaColor = effMultiplyPackedColors(((u32)(blend * 2147483648.0f) & 0xFF000000) | 0x808080, tint);
                            } else alphaColor = tint;
                        }
                        colorWords[0] = effMultiplyPackedColors(edge, alphaColor);
                        colorWords[1] = effMultiplyPackedColors(edge, alphaColor);
                        if (cell->flip) {
                            colorWords[2] = effMultiplyPackedColors(0x80000000, alphaColor);
                            colorWords[3] = effMultiplyPackedColors(center | 0xFF000000, alphaColor);
                            colorWords[4] = colorWords[2];
                        } else {
                            colorWords[2] = effMultiplyPackedColors(0xFF000000, alphaColor);
                            colorWords[3] = effMultiplyPackedColors(center | 0x40000000, alphaColor);
                            colorWords[4] = colorWords[2];
                        }
                    }
                    cell->age++;
                }
                effDrawScaledRecordPool(group->drawPool);
            }
        }
    }
    work->frame++;
}


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

    work = sdfChipAllocDirect(sizeof(EffPCPBossWork));
    memcpy(work->parameterVector, vector, 0x10);
    work->color = 0x80808080;
    work->frame = 0;
    work->modelResource = effParamWorkCreate(EFF_PARAM_WORK_KIND_VIEWER_CONTEXT, paramA);
    work->auxiliaryResource = effParamWorkCreate(EFF_PARAM_WORK_KIND_EXTENDED_WORK_WITH_MATRIX_CALLBACK, paramB);
    return work;
}

/* Apply three packed parameter blocks to the boss effect. */
EffPCPBossWork *effPCPBossApplyThreeBlocks(void *parameterTable) {
    void *firstBlock;
    void *secondBlock;
    void *thirdBlock;

    firstBlock = effParamTableGetBlock(parameterTable, 0);
    secondBlock = effParamTableGetBlock(parameterTable, 1);
    thirdBlock = effParamTableGetBlock(parameterTable, 2);
    return effBossBeamCreate(firstBlock, secondBlock, thirdBlock);
}

/* Duplicate both parameter works; the clone begins at frame zero. */
EffPCPBossWork *effBossBeamClone(EffPCPBossWork *src) {
    EffPCPBossWork *work;

    work = sdfChipAllocDirect(sizeof(EffPCPBossWork));
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
    sdfChipFreeDirect(work);
}

/* Per-frame update: basis from the muzzle position (or identity) fed to both parameter works. */
void effBossBeamUpdate(EffPCPBossWork *work) {
    u128 matrix[4];
    void *direction;

    if (effBTLFieldColorUsesActorSelectors() != 0) {
        if (work->frame == 0) {
            btlUnitGetMuzzlePosVU((struct BtlUnit *)effBTLFieldColorGetVariantSelector());
            VU0_STORE_VF(vf10, work->position);
        }
        direction = work->parameterVector;
        sdfVuBuildLookAtBasis(work, direction, effBossLookAtReferenceAxis);
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

/* Kind 87's registered factory returns zero. */
u32 effPcpKind87CreateDisabled(void) {
    return 0;
}

/* Kind 87's registered duplicate operation returns zero. */
u32 effPcpKind87DuplicateDisabled(void) {
    return 0;
}

/* Kind 87's registered release operation does nothing. */
void effPcpKind87ReleaseNoOp(void) {
}

/* Kind 87's registered per-work dispatch does nothing. */
void effPcpKind87DispatchNoOp(void) {
}

/* Kind 87's optional value operation is present but does nothing. */
void effPcpKind87ValueNoOp(void) {
}

u32 func_0018D570(void) {
    return 0;
}

/* Kind 89's registered factory returns zero. */
u32 effPcpKind89CreateDisabled(void) {
    return 0;
}

/* Kind 89's registered duplicate operation returns zero. */
u32 effPcpKind89DuplicateDisabled(void) {
    return 0;
}

/* Kind 89's registered release operation does nothing. */
void effPcpKind89ReleaseNoOp(void) {
}

/* Kind 89's registered per-work dispatch does nothing. */
void effPcpKind89DispatchNoOp(void) {
}

/* Kind 89's optional source-pointer operation is present but does nothing. */
void effPcpKind89SourceNoOp(void) {
}

/* Kind 89's optional value operation is present but does nothing. */
void effPcpKind89ValueNoOp(void) {
}

u32 func_0018D5A8(void) {
    return 0;
}

/* Kind 90 factory: DDS2 explicitly returns zero, unlike DDS1's empty entry. */
u32 effPcpKind90CreateDisabled(void) {
    return 0;
}

/* Kind 90's registered duplicate operation returns zero. */
u32 effPcpKind90DuplicateDisabled(void) {
    return 0;
}

/* Kind 90's registered release operation does nothing. */
void effPcpKind90ReleaseNoOp(void) {
}

/* Kind 90's registered per-work dispatch does nothing. */
void effPcpKind90DispatchNoOp(void) {
}

/* Kind 90's optional value operation is present but does nothing. */
void effPcpKind90ValueNoOp(void) {
}

u32 func_0018D5D8(void) {
    return 0;
}

/* Kind 91 factory: DDS2 explicitly returns zero, unlike DDS1's empty entry. */
u32 effPcpKind91CreateDisabled(void) {
    return 0;
}

/* Kind 91 duplicate: DDS2 explicitly returns zero, unlike DDS1's empty entry. */
u32 effPcpKind91DuplicateDisabled(void) {
    return 0;
}

/* Kind 91's registered release operation does nothing. */
void effPcpKind91ReleaseNoOp(void) {
}

/* Kind 91's registered per-work dispatch does nothing. */
void effPcpKind91DispatchNoOp(void) {
}

/* Kind 91's optional value operation is present but does nothing. */
void effPcpKind91ValueNoOp(void) {
}

u32 func_0018D608(void) {
    return 0;
}

/* Both kinds 92 and 94 register this zero-returning factory. */
u32 effPcpKinds92And94CreateDisabled(void) {
    return 0;
}

/* Both kinds 92 and 94 register this zero-returning duplicate operation. */
u32 effPcpKinds92And94DuplicateDisabled(void) {
    return 0;
}

/* Both kinds 92 and 94 register this empty release operation. */
void effPcpKinds92And94ReleaseNoOp(void) {
}

/* Kind 92 has its own empty dispatch entry, distinct from kind 94's. */
void effPcpKind92DispatchNoOp(void) {
}

/* Both kinds 92 and 94 register this empty source-pointer operation. */
void effPcpKinds92And94SourceNoOp(void) {
}

/* Both kinds 92 and 94 register this empty value operation. */
void effPcpKinds92And94ValueNoOp(void) {
}

u32 func_0018D640(void) {
    return 0;
}

/* Kind 93's registered factory returns zero. */
u32 effPcpKind93CreateDisabled(void) {
    return 0;
}

/* Kind 93's registered duplicate operation returns zero. */
u32 effPcpKind93DuplicateDisabled(void) {
    return 0;
}

/* Kind 93's registered release operation does nothing. */
void effPcpKind93ReleaseNoOp(void) {
}

/* Kind 93's registered per-work dispatch does nothing. */
void effPcpKind93DispatchNoOp(void) {
}

/* Kind 93's optional value operation is present but does nothing. */
void effPcpKind93ValueNoOp(void) {
}

/* Kind 94 has its own empty dispatch entry, distinct from kind 92's. */
void effPcpKind94DispatchNoOp(void) {
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
EffBossWork *effBossCreateWithGroups(EffBossParams *bossParams, void *effectParams) {
    EffBossWork *work;
    u32 groupIndex;

    work = effBossCreate(bossParams, effectParams);
    for (groupIndex = 0; groupIndex < work->groupCount; groupIndex++) {
        work->groups[groupIndex].rotationStartAge = bossParams->rotationStartAge;
        work->groups[groupIndex].rotationAngle = 0.0f;
        work->groups[groupIndex].angularSpeed = bossParams->angularSpeed;
    }
    return work;
}

EffBossWork *effBossCreateGroupsFromPackedParams(void *data) {
    EffBossParams *bossParams;
    void *effectParams;

    bossParams = effParamTableGetBlock(data, 0);
    effectParams = effParamTableGetBlock(data, 1);
    return effBossCreateWithGroups(bossParams, effectParams);
}

/* Preserve rotation settings, but restart every group's accumulated angle. */
EffBossWork *effBossCloneWithGroups(EffBossWork *src) {
    EffBossWork *work;
    u32 groupIndex;

    work = effBossCloneWorkAndParameters(src);
    for (groupIndex = 0; groupIndex < work->groupCount; groupIndex++) {
        work->groups[groupIndex].rotationStartAge = src->groups[groupIndex].rotationStartAge;
        work->groups[groupIndex].rotationAngle = 0.0f;
        work->groups[groupIndex].angularSpeed = src->groups[groupIndex].angularSpeed;
    }
    return work;
}

void effBossReleaseWorkCallback(void *work) {
    effBossDestroy(work);
}

void effBossUpdateGeometryCallback(void *work) {
    effBossUpdateGeometry(work);
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
    PairedEffectResources *work = sdfChipAllocDirect(sizeof(PairedEffectResources));
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
