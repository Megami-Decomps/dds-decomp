#include "sdk_ps2.h"
#include "sdf_projection.h"
#include "common.h"
#include "sdf_vu0.h"
#include "btl_effect_position.h"
#include "eff_random.h"
#include "sdf_chip.h"
#include "par_cell_api.h"
#include "sdf_resource.h"
#include "eff.h"
#include "eff_param.h"
#include "eff_pcp_boss.h"
#include "eff_flash_fan_color_slot.h"
#include "pcp_vu0.h"
#include "sdf_vu_transform_api.h"
#include "ee_mmi.h"
#include "mdl.h"
#include "sdf_chunk.h"

extern void effPCPCrackDraw(effPCPCrack_t *work);
extern void effPCPCrackSetPosition(f32 *dst, const f32 *src);
extern PairedEffectResources *effPCPBossMETATOBeamCreate(PairedEffectParams *src);
extern EffThunderGroup *effPCP_ThunderPolyListCreate(EffThunderGroupParams *src);

extern u32 effBTLFieldColorUsesActorSelectors(void);
extern u32 effBTLFieldColorGetVariantSelector(void);
extern f32 effBossLookAtReferenceAxis[4];

extern dds3FlashPolyList_t *dds3FlashPolyListCreate(u32 cellCount);
extern f32 *dds3FlashPolyListGetVertex(dds3FlashPolyList_t *pool, s32 index);
extern void dds3FlashPolyListSetBlend(dds3FlashPolyList_t *pool, u32 bits);
extern EffRandState effDefaultRandomState[];
extern f32 D_003B9308;
extern void effPCPCrackInit(effPCPCrack_t *work);
extern effPCPCrack_t *effPCPCrackCopy(effPCPCrack_t *src);
extern void dds3FlashPolyListDelete(dds3FlashPolyList_t *pool);

/* Randomize geometry and initial age; the two extents remain proportional. */
void effPCPCrackSizeInit(effPCPCrack_t *work, EffBossCell *cell) {
    f32 blend = work->head.offsetRandomness;
    f32 scale = work->head.scale;
    f32 t;

    cell->offsetDistance = work->head.offsetDistance * (dds3GetRandFloatA(effDefaultRandomState) * blend + (1.0f - blend)) * scale;
    blend = work->head.extentRandomness;
    t = dds3GetRandFloatA(effDefaultRandomState) * blend + (1.0f - blend);
    cell->baseExtent = work->head.baseExtent * t * scale;
    cell->tipExtent = work->head.tipExtent * t * scale;
    cell->flip = dds3GetRandA((EffRandState *)effDefaultRandomState) & 1;
    cell->age = -(dds3GetRandA((EffRandState *)effDefaultRandomState) % work->head.delaySpread);
}

/* Create a draw pool and delayed cells for each model map-position group. */
void effPCPCrackInit(effPCPCrack_t *work)
{
    dds3ModelHandle_t *model = effParamWorkGetData(work->paramWork);
    EffBossGroup *group;
    EffBossCell *nextCells;
    u32 groupIndex;
    u32 cellIndex;

    if ((s32)work->head.delaySpread <= 0) {
        work->head.delaySpread = 1;
    }
    model->first->frameStep = work->head.frameStep;
    mdlSingleMotion(model, 0, 0);
    work->cellCount = model->first->frameCount;
    work->groupCount = sdfModelGetMarkPosCount(model->inner);
    work->system = dds3TrackPolyCreate(work->groupCount, work->cellCount, 1, PAR_CELL_TOPOLOGY_TRIANGLE);
    dds3TrackPolySetMode(work->system, work->head.drawBucket);
    work->groupsHandle = NULL;
    if (work->head.hasCells) {
        work->groupsHandle = sdfMemAllocDirect(work->groupCount * sizeof(EffBossGroup)
            + work->cellCount * work->groupCount * sizeof(EffBossCell));
        nextCells = (EffBossCell *)sdfMemLock(work->groupsHandle);
        work->groups = (EffBossGroup *)nextCells;
        nextCells = (EffBossCell *)((u8 *)nextCells + work->groupCount * sizeof(EffBossGroup));
        for (groupIndex = 0, group = work->groups; groupIndex < work->groupCount; groupIndex++, group++) {
            EffBossCell *cell;

            group->drawPool = dds3FlashPolyListCreate(work->cellCount);
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
                f32 *vertices = dds3FlashPolyListGetVertex(group->drawPool, cellIndex);
                EffFlashFanColorSlot *colors = dds3FlashPolyListGetVC(group->drawPool, cellIndex);

                colors->color[0] = 0;
                colors->color[1] = 0;
                colors->color[2] = 0;
                colors->color[3] = 0;
                colors->color[4] = 0;
                SDF_VU0_LOADIDENTITYVECTOR();
                SDF_VU0_STOREVECTOR(vertices);
                SDF_VU0_STOREVECTOR(vertices + 4);
                SDF_VU0_STOREVECTOR(vertices + 8);
                SDF_VU0_STOREVECTOR(vertices + 12);
                SDF_VU0_STOREVECTOR(vertices + 16);
            }
            dds3FlashPolyListSetBlend(group->drawPool, work->head.incrementBits);
            group->cells = nextCells;
            nextCells += work->cellCount;
            for (cellIndex = 0, cell = group->cells; cellIndex < work->cellCount; cellIndex++, cell++) {
                effPCPCrackSizeInit(work, cell);
            }
        }
    }
}

/* Copy the trail head and create its model, cell system and per-map groups. */
effPCPCrack_t *effPCPCrackCreate(EffBossParams *bossParams, void *effectParams) {
    effPCPCrack_t *work;

    work = sdfChipAllocDirect(sizeof(effPCPCrack_t));
    work->head = bossParams->head;
    work->frame = 0;
    work->color = 0x80808080;
    work->paramWork = dds3EffectPartCreate(EFF_PARAM_WORK_KIND_VIEWER_CONTEXT, effectParams);
    effPCPCrackInit(work);
    return work;
}

/* Apply the first two packed parameter blocks to the boss effect. */
effPCPCrack_t *effPCPCrackCreatePCF(const EffParamTable *parameterTable) {
    EffBossParams *bossParams;
    void *effectParams;

    bossParams = dds3EffectPCPDataGetData(parameterTable, 0);
    effectParams = dds3EffectPCPDataGetData(parameterTable, 1);
    return effPCPCrackCreate(bossParams, effectParams);
}

/* Duplicate model parameters and rebuild groups with a fresh frame counter. */
effPCPCrack_t *effPCPCrackCopy(effPCPCrack_t *src) {
    effPCPCrack_t *work;

    work = sdfChipAllocDirect(sizeof(effPCPCrack_t));
    memcpy(&work->head, &src->head, sizeof(EffBossHead));
    work->frame = 0;
    work->color = 0x80808080;
    work->paramWork = dds3EffectPartCopy(src->paramWork);
    effPCPCrackInit(work);
    return work;
}

/* Release group draw pools, the shared cell system, model resource and owner. */
void effPCPCrackDelete(effPCPCrack_t *work) {
    u32 groupIndex;

    if (work->groupsHandle != 0) {
        for (groupIndex = 0; groupIndex < work->groupCount; groupIndex++) {
            dds3FlashPolyListDelete(work->groups[groupIndex].drawPool);
        }
        sdfMemFreeDirect(work->groupsHandle);
    }
    dds3TrackPolyDelete(work->system);
    dds3EffectPartDelete(work->paramWork);
    sdfChipFreeDirect(work);
}

#include "eff_math.h"
#include "sdf_quaternion.h"
extern u32 effMagaCylinderCalcColor(u32, u32);
extern struct SdfPoolNode *kwlnSceneSurfaceSets[13][4];
extern f32 D_003556B0[4];
extern s32 sdfModelGetMarkPosMatrix(struct sdfModelRootNode_t *, s32);
extern void dds3FlashPolyListSetABGR(dds3FlashPolyList_t *, u32);
extern void dds3FlashPolyListDraw(dds3FlashPolyList_t *);

/* Independent native reconstruction; prior KiB/Opal/Brass algorithms credited. */
void effPCPCrackDraw(effPCPCrack_t *work) {
    f32 perpendicular[4];
    f32 direction[4];
    f32 axis[4];
    f32 scaleVector[4];
    f32 widthVector[4];
    f32 displacement[4];
    f32 baseVector[4];
    f32 tipVector[4];
    dds3ModelHandle_t *model;
    struct sdfModelRootNode_t *map;
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
    SDF_VU0_LOADVECTOR(work->head.position);
    mdlSetPosition(model);
    VEC3_SPLAT(scaleVector, work->head.scale);
    scaleVector[3] = 0.0f;
    SDF_VU0_LOADVECTOR(scaleVector);
    mdlSetScale(model);
    mdlLink(model, kwlnSceneSurfaceSets[10]);
    if (work->head.drawTrail) {
        colorStart = work->head.trailColorStartFrame;
        if (frame > colorStart) {
            if (work->head.trailColorTransitionFrames > 0) {
                blend = (f32)(frame - colorStart) / work->head.trailColorTransitionFrames;
                center = effColorBlend(work->head.trailStartCenterColor, work->head.trailEndCenterColor, blend);
                edge = effColorBlend(work->head.trailStartEdgeColor, work->head.trailEndEdgeColor, blend);
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
                sdfModelGetMarkPosMatrix(map, i);
                SDF_VU0_LOADVECTOR(direction);
                SDF_VU0_APPLYMATRIX_XYZ();
                SDF_VU0_STOREVECTOR(direction);
                work->groups[i].direction[0] = direction[0];
                work->groups[i].direction[1] = direction[1];
                work->groups[i].direction[2] = direction[2];
            }
            sdfModelGetMarkPos(map, i);
            if (frame < work->cellCount) {
                historyCount = historyCell->vertexCount;
                SDF_VU0_STOREVECTOR_NOMEM(positions + historyCount);
                SDF_VU0_STOREVECTOR_NOMEM(positions + historyCount + 1);
                SDF_VU0_STOREVECTOR_NOMEM(positions + historyCount + 2);
                historyCell->vertexCount += 3;
                taperStep = taperRange / frame;
                blend = 1.0f;
                SDF_VU0_LOADVECTOR_C(D_003556B0);
                if (frame > 0) {
                    const f32 *trailWidth = widthVector;
                    j = frame;
                    do {
                        SDF_VU0_LOADVECTOR(positions + 4);
                        SDF_VU0_LOADVECTOR_B(positions + 1);
                        SDF_VU0_SUBVECTOR();
                        SDF_VU0_GETLENGTH(trailLength);
                        if (trailLength > 1.0f) {
                            SDF_VU0_COPYVECTOR_C_TO_B();
                            SDF_VU0_OUTERPRODUCT();
                            SDF_VU0_NORMALIZE();
                            SDF_VU0_STOREVECTOR(perpendicular);
                        } else {
                            SDF_VU0_LOADVECTOR(perpendicular);
                        }
                        SDF_VU0_LOADVECTOR_B(trailWidth);
                        VU0_SCALE_VF(vf11, blend);
                        SDF_VU0_MULVECTOR();
                        SDF_VU0_COPYVECTOR_A_TO_B();
                        SDF_VU0_LOADVECTOR(positions + 1);
                        SDF_VU0_ADDVECTOR();
                        SDF_VU0_STOREVECTOR(positions);
                        SDF_VU0_SUBVECTOR();
                        SDF_VU0_SUBVECTOR();
                        SDF_VU0_STOREVECTOR(positions + 2);
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
        dds3TrackPolyDraw(work->system);
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
                dds3FlashPolyListSetABGR(group->drawPool, tint);
                for (j = 0; j < count - 1; j++, cell++, historyStart += 3) {
                    positions = dds3FlashPolyListGetVertex(group->drawPool, j);
                    colorWords = dds3FlashPolyListGetVC(group->drawPool, j)->color;
                    age = cell->age;
                    if (age > colorStart) {
                        if (work->head.cellColorTransitionFrames > 0 && age < colorStart + colorFrames) {
                            blend = (f32)(age - colorStart) / colorFrames;
                            center = effColorBlend(work->head.cellStartCenterColor, work->head.cellEndCenterColor, blend);
                            edge = effColorBlend(work->head.cellStartEdgeColor, work->head.cellEndEdgeColor, blend);
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
                            SDF_VU0_LOADVECTOR(historyStart + 4);
                            SDF_VU0_LOADVECTOR_B(historyStart + 1);
                            SDF_VU0_SUBVECTOR();
                            SDF_VU0_NORMALIZE();
                            SDF_VU0_STOREVECTOR_NOMEM(axis);
                            SetRotateMatrixAxis(group->rotationAngle, (const struct RwV3d *)axis);
                            SDF_VU0_LOADVECTOR(displacement);
                            SDF_VU0_APPLYMATRIX_XYZ();
                            SDF_VU0_STOREVECTOR(displacement);
                            group->rotationAngle += group->angularSpeed;
                        }
                        baseExtent = cell->baseExtent;
                        VEC3_SPLAT(baseVector, baseExtent);
                        baseVector[3] = 0.0f;
                        tipExtent = cell->tipExtent;
                        VEC3_SPLAT(tipVector, tipExtent);
                        tipVector[3] = 0.0f;
                        SDF_VU0_LOADVECTOR(historyStart + 4);
                        SDF_VU0_LOADVECTOR_B(historyStart + 1);
                        SDF_VU0_SUBVECTOR();
                        SDF_VU0_GETLENGTH(cellLength);
                        if (cellLength > 1.0f) {
                            SDF_VU0_NORMALIZE();
                            SDF_VU0_COPYVECTOR_A_TO_B();
                            SDF_VU0_LOADVECTOR(tipVector);
                            SDF_VU0_MULVECTOR();
                            SDF_VU0_STOREVECTOR(tipVector);
                            SDF_VU0_LOADVECTOR(baseVector);
                            SDF_VU0_MULVECTOR();
                            SDF_VU0_STOREVECTOR(baseVector);
                            SDF_VU0_LOADVECTOR_C(displacement);
                            SDF_VU0_LOADVECTOR(historyStart + 4);
                            SDF_VU0_LOADVECTOR_B(baseVector);
                            SDF_VU0_STOREVECTOR(positions + 3);
                            SDF_VU0_ADDVECTOR();
                            SDF_VU0_STOREVECTOR(positions + 2);
                            SDF_VU0_SUBVECTOR();
                            SDF_VU0_SUBVECTOR();
                            SDF_VU0_STOREVECTOR(positions + 4);
                            SDF_VU0_LOADVECTOR(historyStart + 4);
                            SDF_VU0_LOADVECTOR_B(tipVector);
                            SDF_VU0_ADDVECTOR_A_C();
                            SDF_VU0_ADDVECTOR();
                            SDF_VU0_STOREVECTOR(positions);
                            SDF_VU0_SUBVECTOR();
                            SDF_VU0_SUBVECTOR();
                            SDF_VU0_STOREVECTOR(positions + 1);
                        } else {
                            SDF_VU0_LOADIDENTITYVECTOR();
                            SDF_VU0_STOREVECTOR(positions);
                            SDF_VU0_STOREVECTOR(positions + 1);
                            SDF_VU0_STOREVECTOR(positions + 2);
                            SDF_VU0_STOREVECTOR(positions + 3);
                            SDF_VU0_STOREVECTOR(positions + 4);
                        }
                        alphaColor = 0;
                        if (age <= duration) {
                            if (age < fadeIn && fadeIn != 0) {
                                blend = (f32)age / fadeIn;
                                alphaColor = effMagaCylinderCalcColor(((u32)(blend * 2147483648.0f) & 0xFF000000) | 0x808080, tint);
                            } else if (duration - age <= fadeOut && fadeOut != 0) {
                                blend = (f32)(duration - age) / fadeOut;
                                alphaColor = effMagaCylinderCalcColor(((u32)(blend * 2147483648.0f) & 0xFF000000) | 0x808080, tint);
                            } else alphaColor = tint;
                        }
                        colorWords[0] = effMagaCylinderCalcColor(edge, alphaColor);
                        colorWords[1] = effMagaCylinderCalcColor(edge, alphaColor);
                        if (cell->flip) {
                            colorWords[2] = effMagaCylinderCalcColor(0x80000000, alphaColor);
                            colorWords[3] = effMagaCylinderCalcColor(center | 0xFF000000, alphaColor);
                            colorWords[4] = colorWords[2];
                        } else {
                            colorWords[2] = effMagaCylinderCalcColor(0xFF000000, alphaColor);
                            colorWords[3] = effMagaCylinderCalcColor(center | 0x40000000, alphaColor);
                            colorWords[4] = colorWords[2];
                        }
                    }
                    cell->age++;
                }
                dds3FlashPolyListDraw(group->drawPool);
            }
        }
    }
    work->frame++;
}


void effPCPCrackSetPosition(f32 *dst, const f32 *src)
{
    PCP_COPY_VECTOR_F32(dst, src);
}

void effBossTrailSetColor(effPCPCrack_t *work, s32 value) {
    work->color = value;
}

/* Copy the beam vector and create its model and auxiliary parameter works. */
EffPCPBossWork *effPCPBossBaelCreate(const f32 vector[4], void *paramA, void *paramB) {
    EffPCPBossWork *work;

    work = sdfChipAllocDirect(sizeof(EffPCPBossWork));
    memcpy(work->parameterVector, vector, 0x10);
    work->color = 0x80808080;
    work->frame = 0;
    work->modelResource = dds3EffectPartCreate(EFF_PARAM_WORK_KIND_VIEWER_CONTEXT, paramA);
    work->auxiliaryResource = dds3EffectPartCreate(EFF_PARAM_WORK_KIND_EXTENDED_WORK_WITH_MATRIX_CALLBACK, paramB);
    return work;
}

/* Apply three packed parameter blocks to the boss effect. */
EffPCPBossWork *effPCPBossBaelCreatePCF(const EffParamTable *parameterTable) {
    void *firstBlock;
    void *secondBlock;
    void *thirdBlock;

    firstBlock = dds3EffectPCPDataGetData(parameterTable, 0);
    secondBlock = dds3EffectPCPDataGetData(parameterTable, 1);
    thirdBlock = dds3EffectPCPDataGetData(parameterTable, 2);
    return effPCPBossBaelCreate(firstBlock, secondBlock, thirdBlock);
}

/* Duplicate both parameter works; the clone begins at frame zero. */
EffPCPBossWork *effPCPBossBaelCopy(EffPCPBossWork *src) {
    EffPCPBossWork *work;

    work = sdfChipAllocDirect(sizeof(EffPCPBossWork));
    memcpy(work->parameterVector, src->parameterVector, 0x10);
    work->color = 0x80808080;
    work->frame = 0;
    work->modelResource = dds3EffectPartCopy(src->modelResource);
    work->auxiliaryResource = dds3EffectPartCopy(src->auxiliaryResource);
    return work;
}

/* Release both beam parameter works before freeing the owner. */
void effPCPBossBaelDelete(EffPCPBossWork *work) {
    dds3EffectPartDelete(work->auxiliaryResource);
    dds3EffectPartDelete(work->modelResource);
    sdfChipFreeDirect(work);
}

/* Per-frame update: basis from the muzzle position (or identity) fed to both parameter works. */
void effBossBeamUpdate(EffPCPBossWork *work) {
    u128 matrix[4];
    f32 *direction;

    if (effBTLFieldColorUsesActorSelectors() != 0) {
        if (work->frame == 0) {
            btlUnitGetMuzzlePosVU((struct BtlUnit *)effBTLFieldColorGetVariantSelector());
            SDF_VU0_STOREVECTOR(work->position);
        }
        direction = work->parameterVector;
        LookAtMatrix(work->position, direction, effBossLookAtReferenceAxis);
        ReverseMatrix();
        SDF_VU0_RESET_ROW3();
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
    dds3EffectPartSetRotateMatrix(work->modelResource, matrix);
    dds3EffectPartSetRotateMatrix(work->auxiliaryResource, matrix);
    dds3EffectPartSetPosition(work->modelResource, direction);
    dds3EffectPartSetPosition(work->auxiliaryResource, direction);
    dds3EffectPartSetColor(work->modelResource, work->color);
    dds3EffectPartSetColor(work->auxiliaryResource, work->color);
    dds3EffectPartDraw(work->modelResource);
    dds3EffectPartDraw(work->auxiliaryResource);
    work->frame++;
}

void effPCPBossBaelSetPosition(EffPCPBossWork *work, void *src) {
    SDF_VU0_COPYVECTOR(&work->parameterVector, src);
}

void effPCPBossBaelSetColor(EffPCPBossWork *work, u32 value) {
    work->color = value;
}

u32 func_001858E8(void) {
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

u32 func_00185918(void) {
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

void func_00185950(void) {
}

/* Kind 90 factory: DDS1 does not write v0; unlike DDS2, this is not a zero return. */
void effPcpKind90CreateNoOp(void) {
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

void func_00185980(void) {
}

/* Kind 91 factory: DDS1 does not write v0; unlike DDS2, this is not a zero return. */
void effPcpKind91CreateNoOp(void) {
}

/* Kind 91 duplicate: DDS1 does not write v0; unlike DDS2, this is not a zero return. */
void effPcpKind91DuplicateNoOp(void) {
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

u32 func_001859B0(void) {
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

u32 func_001859E8(void) {
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
effPCPCrack_t *effPCPCrack2Create(EffBossParams *bossParams, void *effectParams) {
    effPCPCrack_t *work;
    u32 groupIndex;

    work = effPCPCrackCreate(bossParams, effectParams);
    for (groupIndex = 0; groupIndex < work->groupCount; groupIndex++) {
        work->groups[groupIndex].rotationStartAge = bossParams->rotationStartAge;
        work->groups[groupIndex].rotationAngle = 0.0f;
        work->groups[groupIndex].angularSpeed = bossParams->angularSpeed;
    }
    return work;
}

effPCPCrack_t *effPCPCrack2CreatePCF(const EffParamTable *data) {
    EffBossParams *bossParams;
    void *effectParams;

    bossParams = dds3EffectPCPDataGetData(data, 0);
    effectParams = dds3EffectPCPDataGetData(data, 1);
    return effPCPCrack2Create(bossParams, effectParams);
}

/* Preserve rotation settings, but restart every group's accumulated angle. */
effPCPCrack_t *effPCPCrack2Copy(effPCPCrack_t *src) {
    effPCPCrack_t *work;
    u32 groupIndex;

    work = effPCPCrackCopy(src);
    for (groupIndex = 0; groupIndex < work->groupCount; groupIndex++) {
        work->groups[groupIndex].rotationStartAge = src->groups[groupIndex].rotationStartAge;
        work->groups[groupIndex].rotationAngle = 0.0f;
        work->groups[groupIndex].angularSpeed = src->groups[groupIndex].angularSpeed;
    }
    return work;
}

void effPCPCrack2Delete(effPCPCrack_t *work) {
    effPCPCrackDelete(work);
}

void effPCPCrack2Draw(effPCPCrack_t *work) {
    effPCPCrackDraw(work);
}

void effBossSetPositionCallback(f32 *destination, const f32 position[4]) {
    effPCPCrackSetPosition(destination, position);
}

void effBossApplyGroupTint(effPCPCrack_t *work, s32 value) {
    effBossTrailSetColor(work, value);
}

/* Create paired four-point chains; the update fills their point vectors. */
PairedEffectResources *effPCPBossMETATOBeamCreate(PairedEffectParams *src)
{
    PairedEffectResources *work = sdfChipAllocDirect(sizeof(PairedEffectResources));
    EffThunderGroupParams chain;

    memcpy(work, src, sizeof(PairedEffectParams));
    work->colorWithAlpha = 0x80808080;
    work->frame = 0;
    chain.params = work->fragment;
    chain.count = 4;
    work->resource[0] = effPCP_ThunderPolyListCreate(&chain);
    work->resource[1] = effPCP_ThunderPolyListCreate(&chain);
    return work;
}

PairedEffectResources *effPCPBossMETATOBeamCreatePCF(const EffParamTable *data) {
    void *work;

    work = dds3EffectPCPDataGetData(data, 0);
    return effPCPBossMETATOBeamCreate(work);
}

PairedEffectResources *effPCPBossMETATOBeamCopy(void *work) {
    return effPCPBossMETATOBeamCreate(work);
}
