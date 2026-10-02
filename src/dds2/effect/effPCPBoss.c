#include "common.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"

typedef struct EffParamWork EffParamWork;

/* Packed effect parameter-set accessor (see game/code_001624D0). */
extern void *effParamTableGetBlock(void *data, s32 index);
extern void func_0018C820(void *work);
extern void effBossSetPosition();
extern void func_0018D830(void *work);

extern void *memcpy(void *dst, const void *src, u32 size);
extern void *func_00328D68(s32 size);
extern EffParamWork *effParamWorkCreate(s32 kind, void *params);
extern EffParamWork *effParamWorkDuplicate(EffParamWork *param);
extern void effDispatchParameterDataAndFreeWork(EffParamWork *handle);
extern void sdfReleaseChipBlock(void *work);
extern u32 func_001695C8(void);
extern u32 effBTLFieldColorGetVariantSelector(void);
extern void btlUnitGetMuzzlePosVU(u32 unit);
extern void sdfVuBuildLookAtBasis(void *origin, void *direction, void *up);
extern void sdfInvertRigidVuTransform(void);
extern void effParamWorkCallback0(EffParamWork *handle, void *vec);
extern void effParamWorkCallback2(EffParamWork *handle, void *matrix);
extern void effParamWorkCallback3(EffParamWork *handle, u32 value);
extern void effParamWorkInvokeCallback(EffParamWork *handle);
extern u8 D_003B1FF0[];

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
    u32 rotationStartAge;
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

typedef struct EffBossDrawPool EffBossDrawPool;

typedef struct {
    EffBossDrawPool *drawPool;
    f32 direction[3]; /* Unit Y initially; mode 2 refreshes it from the model. */
    u8 pad10[4];
    u32 rotationAngleBits; /* Initialized as a word; native update uses float bits. */
    u32 rotationStartAge;
    f32 angularSpeed;
    EffBossCell *cells; /* 0x20 */
} EffBossGroup; /* 0x24 */

typedef struct {
    EffBossHead head;
    EffBossGroup *groups; /* 0x8C */
    u32 groupsHandle; /* 0x90 */
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

typedef struct {
    u8 pad00[0x20];
    f32 unk20;
    u8 pad24[0xA];
    u16 frameCount; /* Copied from the motion clip; also sizes the trail history. */
} EffBossMotionNode;

typedef struct {
    u8 pad00[0x18];
    void *inner;
    EffBossMotionNode *motion;
} EffBossModelContext;

extern void *effParamWorkGetData(EffParamWork *handle);
extern void mdlAddEntryPlain(void *work, s32 arg1, s32 arg2);
extern u32 sdfCountMapPositionRecords(void *chunk);
extern u32 parAllocateCellSystem(s32 count, s32 perCell, s32 groupDivisor, u32 kind);
extern void func_00164C68(u32 system, u32 value);
extern u32 sdfAllocGeneralBlock(s32 size);
extern u8 *sdfResourceRetainAddress(u32 handle);
extern EffBossDrawPool *func_00177760(u32 cellCount);
extern u32 effGetIndexedEffectGroupRecord(EffBossDrawPool *pool, s32 index);
extern EffBossColorSlot *effGetIndexedEffectGroupIndexEntry(EffBossDrawPool *pool, s32 index);
extern void effSetVectorIncrementBits(EffBossDrawPool *pool, u32 bits);
extern u32 effMiscRand(void *state);
extern f32 effMiscRandUnitFloat(void *state);
extern u8 D_003AA868[];
extern f32 D_004334C4;
extern void func_0018C288(EffBossWork *work);
extern EffBossWork *effBossCloneWorkAndParameters(EffBossWork *src);
extern void effReleaseRecordGroupAssetAndHandle(EffBossDrawPool *pool);
extern void sdfReleaseResourceAllocation(u32 handle);
extern void parReleaseCellSystem(u32 system);

/* Randomize geometry and initial age; the two extents remain proportional. */
void effBossCellRandomize(EffBossWork *work, EffBossCell *cell) {
    f32 blend = work->head.offsetRandomness;
    f32 scale = work->head.scale;
    f32 t;

    cell->offsetDistance = work->head.offsetDistance * (effMiscRandUnitFloat(D_003AA868) * blend + (1.0f - blend)) * scale;
    blend = work->head.extentRandomness;
    t = effMiscRandUnitFloat(D_003AA868) * blend + (1.0f - blend);
    cell->baseExtent = work->head.baseExtent * t * scale;
    cell->tipExtent = work->head.tipExtent * t * scale;
    cell->flip = effMiscRand(D_003AA868) & 1;
    cell->age = -(effMiscRand(D_003AA868) % work->head.delaySpread);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_0018C288);

/* Copy the trail head and create its model, cell system and per-map groups. */
EffBossWork *effBossCreate(EffBossParams *src, void *param1) {
    EffBossWork *work;

    work = func_00328D68(sizeof(EffBossWork));
    work->head = src->head;
    work->frame = 0;
    work->color = 0x80808080;
    work->paramWork = effParamWorkCreate(3, param1);
    func_0018C288(work);
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

    work = func_00328D68(sizeof(EffBossWork));
    memcpy(&work->head, &src->head, sizeof(EffBossHead));
    work->frame = 0;
    work->color = 0x80808080;
    work->paramWork = effParamWorkDuplicate(src->paramWork);
    func_0018C288(work);
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

void effBossSetPosition(dst, src)
void *dst;
void *src;
{
    PCP_COPY_VECTOR(dst, src);
}

void func_0018D220(u8 *work, s32 value) {
    *(s32 *)(work + 0xA0) = value;
}

/* Copy the beam vector and create its model and auxiliary parameter works. */
EffPCPBossWork *effBossBeamCreate(void *vector, void *paramA, void *paramB) {
    EffPCPBossWork *work;

    work = func_00328D68(sizeof(EffPCPBossWork));
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

    work = func_00328D68(sizeof(EffPCPBossWork));
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
        work->groups[i].angularSpeed = src->angularSpeed;
        work->groups[i].rotationStartAge = src->rotationStartAge;
        work->groups[i].rotationAngleBits = 0;
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
        work->groups[i].angularSpeed = src->groups[i].angularSpeed;
        work->groups[i].rotationStartAge = src->groups[i].rotationStartAge;
        work->groups[i].rotationAngleBits = 0;
    }
    return work;
}

void effBossReleaseWorkCallback(void *work) {
    effBossDestroy(work);
}

void effBossUpdateGeometryCallback(void *work) {
    func_0018C820(work);
}

void func_0018D800(void *work) {
    effBossSetPosition(work);
}

void func_0018D818(u8 *work, s32 value) {
    func_0018D220(work, value);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_0018D830);

void func_0018DA38(void *data) {
    void *work;

    work = effParamTableGetBlock(data, 0);
    func_0018D830(work);
}

void func_0018DA58(void *work) {
    func_0018D830(work);
}
