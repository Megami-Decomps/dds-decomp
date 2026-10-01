#include "common.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"

typedef struct EffParamWork EffParamWork;

/* Packed effect parameter-set accessor (see game/code_001624D0). */
extern void *effParamTableGetBlock(void *data, s32 index);
extern void func_00184BC8(void *work);
extern void func_001855B8();
extern void func_00185BD8(void *work);

extern void *memcpy(void *dst, const void *src, u32 size);
extern void *func_002CFEB8(s32 size);
extern EffParamWork *effParamWorkCreate(s32 kind, void *params);
extern EffParamWork *effParamWorkDuplicate(EffParamWork *param);
extern void func_001629F0(EffParamWork *handle);
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
    EffParamWork *resource28;  /* 0x28 released by func_001629F0 */
    EffParamWork *resource2C;  /* 0x2C released by func_001629F0 */
} EffPCPBossWork;

/* Trail effect built from a 0x8C-byte parameter head (copied verbatim on spawn),
   `groupCount` groups of `cellCount` cells each. */
typedef struct {
    u8 pad00[0x14];
    f32 modelScale;   /* 0x14 */
    f32 scale;        /* 0x18 applied to every random cell offset */
    u8 pad1C[0x1C];
    u16 systemParam;  /* 0x38 */
    u8 pad3A[2];
    u8 hasCells;      /* 0x3C */
    u8 pad3D[7];
    u32 spread;       /* 0x44 modulus of the cell delay */
    u8 pad48[0x10];
    f32 xRange;       /* 0x58 */
    f32 xBlend;       /* 0x5C */
    f32 yRange;       /* 0x60 */
    f32 zRange;       /* 0x64 */
    f32 yBlend;       /* 0x68 */
    u8 pad6C[0x18];
    u32 incrementBits; /* 0x84 */
    u8 forward;       /* 0x88 */
    u8 pad89[3];
} EffBossHead; /* 0x8C */

/* Parameter block as read by effBossCreateWithGroups: two words follow the head. */
typedef struct {
    EffBossHead head;
    u32 groupValue;   /* 0x8C */
    f32 groupFloat;   /* 0x90 */
} EffBossParams;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    s32 delay;        /* 0x0C */
    u8 flip;          /* 0x10 */
    u8 pad11[3];
} EffBossCell; /* 0x14 */

typedef struct EffBossRecords EffBossRecords;

typedef struct {
    EffBossRecords *records; /* 0x00 */
    u32 unk04;
    f32 direction;    /* 0x08 -1.0 or 1.0 */
    u32 unk0C;
    u8 pad10[4];
    u32 unk14;
    u32 unk18;
    f32 unk1C;
    EffBossCell *cells; /* 0x20 */
} EffBossGroup; /* 0x24 */

typedef struct {
    EffBossHead head;
    EffBossGroup *groups; /* 0x8C */
    u32 groupsHandle; /* 0x90 */
    u32 groupCount;   /* 0x94 */
    u16 cellCount;    /* 0x98 */
    u8 pad9A[2];
    u32 unk9C;
    u32 color;        /* 0xA0 */
    u32 system;       /* 0xA4 */
    EffParamWork *paramWork; /* 0xA8 */
} EffBossWork;

typedef struct {
    u32 w0;
    u32 w1;
    u32 w2;
    u32 w3;
    u32 w4;
} EffBossIndex; /* 0x14 */

typedef struct {
    u8 pad00[0x20];
    f32 scale;        /* 0x20 */
    u8 pad24[0xA];
    u16 cellCount;    /* 0x2E */
} EffBossModelHeader;

typedef struct {
    u8 pad00[0x18];
    void *chunk;      /* 0x18 */
    EffBossModelHeader *model; /* 0x1C */
} EffBossModelData;

extern void *effParamWorkGetData(EffParamWork *handle);
extern void mdlAddEntryPlain(void *work, s32 arg1, s32 arg2);
extern u32 sdfCountMapPositionRecords(void *chunk);
extern u32 parAllocateCellSystem(s32 count, s32 perCell, s32 groupDivisor, u32 kind);
extern void func_0015D078(u32 system, u32 value);
extern u32 func_002D03F8(s32 size);
extern u8 *sdfResourceRetainAddress(u32 handle);
extern EffBossRecords *func_0016FB08(u32 cellCount);
extern u32 effGetIndexedEffectGroupRecord(EffBossRecords *records, s32 index);
extern EffBossIndex *effGetIndexedEffectGroupIndexEntry(EffBossRecords *records, s32 index);
extern void effSetVectorIncrementBits(EffBossRecords *records, u32 bits);
extern u32 effMiscRand(void *state);
extern f32 effMiscRandUnitFloat(void *state);
extern u8 D_0034DF38[];
extern f32 D_003B9308;
extern void func_00184630(EffBossWork *work);
extern EffBossWork *effBossCloneWorkAndParameters(EffBossWork *src);
extern void effReleaseRecordGroupAssetAndHandle(EffBossRecords *records);
extern void func_002D0918(u32 handle);
extern void parReleaseCellSystem(u32 system);

void effBossCellRandomize(EffBossWork *work, EffBossCell *cell) {
    f32 blend = work->head.xBlend;
    f32 scale = work->head.scale;
    f32 t;

    cell->x = work->head.xRange * (effMiscRandUnitFloat(D_0034DF38) * blend + (1.0f - blend)) * scale;
    blend = work->head.yBlend;
    t = effMiscRandUnitFloat(D_0034DF38) * blend + (1.0f - blend);
    cell->y = work->head.yRange * t * scale;
    cell->z = work->head.zRange * t * scale;
    cell->flip = effMiscRand(D_0034DF38) & 1;
    cell->delay = -(effMiscRand(D_0034DF38) % work->head.spread);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_00184630);

EffBossWork *effBossCreate(EffBossParams *src, void *param1) {
    EffBossWork *work;

    work = func_002CFEB8(sizeof(EffBossWork));
    work->head = src->head;
    work->unk9C = 0;
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

EffBossWork *effBossCloneWorkAndParameters(EffBossWork *src) {
    EffBossWork *work;

    work = func_002CFEB8(sizeof(EffBossWork));
    memcpy(&work->head, &src->head, sizeof(EffBossHead));
    work->unk9C = 0;
    work->color = 0x80808080;
    work->paramWork = effParamWorkDuplicate(src->paramWork);
    func_00184630(work);
    return work;
}

void effBossDestroy(EffBossWork *work) {
    u32 i;

    if (work->groupsHandle != 0) {
        for (i = 0; i < work->groupCount; i++) {
            effReleaseRecordGroupAssetAndHandle(work->groups[i].records);
        }
        func_002D0918(work->groupsHandle);
    }
    parReleaseCellSystem(work->system);
    func_001629F0(work->paramWork);
    sdfReleaseChipBlock(work);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_00184BC8);

void func_001855B8(dst, src)
void *dst;
void *src;
{
    PCP_COPY_VECTOR(dst, src);
}

void func_001855C8(u8 *work, s32 value) {
    *(s32 *)(work + 0xA0) = value;
}

EffPCPBossWork *effBossBeamCreate(void *vector, void *paramA, void *paramB) {
    EffPCPBossWork *work;

    work = func_002CFEB8(sizeof(EffPCPBossWork));
    memcpy(work->parameterVector, vector, 0x10);
    work->color = 0x80808080;
    work->frame = 0;
    work->resource28 = effParamWorkCreate(3, paramA);
    work->resource2C = effParamWorkCreate(6, paramB);
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

EffPCPBossWork *effBossBeamClone(EffPCPBossWork *src) {
    EffPCPBossWork *work;

    work = func_002CFEB8(sizeof(EffPCPBossWork));
    memcpy(work->parameterVector, src->parameterVector, 0x10);
    work->color = 0x80808080;
    work->frame = 0;
    work->resource28 = effParamWorkDuplicate(src->resource28);
    work->resource2C = effParamWorkDuplicate(src->resource2C);
    return work;
}

void effPCPBossFree(EffPCPBossWork *work) {
    func_001629F0(work->resource2C);
    func_001629F0(work->resource28);
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
    effParamWorkCallback2(work->resource28, matrix);
    effParamWorkCallback2(work->resource2C, matrix);
    effParamWorkCallback0(work->resource28, direction);
    effParamWorkCallback0(work->resource2C, direction);
    effParamWorkCallback3(work->resource28, work->color);
    effParamWorkCallback3(work->resource2C, work->color);
    effParamWorkInvokeCallback(work->resource28);
    effParamWorkInvokeCallback(work->resource2C);
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

EffBossWork *effBossCreateWithGroups(EffBossParams *src, void *param1) {
    EffBossWork *work;
    u32 i;

    work = effBossCreate(src, param1);
    for (i = 0; i < work->groupCount; i++) {
        work->groups[i].unk1C = src->groupFloat;
        work->groups[i].unk18 = src->groupValue;
        work->groups[i].unk14 = 0;
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

EffBossWork *effBossCloneWithGroups(EffBossWork *src) {
    EffBossWork *work;
    u32 i;

    work = effBossCloneWorkAndParameters(src);
    for (i = 0; i < work->groupCount; i++) {
        work->groups[i].unk1C = src->groups[i].unk1C;
        work->groups[i].unk18 = src->groups[i].unk18;
        work->groups[i].unk14 = 0;
    }
    return work;
}

void func_00185B78(void *work) {
    effBossDestroy(work);
}

void func_00185B90(void *work) {
    func_00184BC8(work);
}

void func_00185BA8(void *work) {
    func_001855B8(work);
}

void func_00185BC0(u8 *work, s32 value) {
    func_001855C8(work, value);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_00185BD8);

void func_00185DE0(void *data) {
    void *work;

    work = effParamTableGetBlock(data, 0);
    func_00185BD8(work);
}

void func_00185E00(void *work) {
    func_00185BD8(work);
}
