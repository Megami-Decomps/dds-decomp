#include "common.h"
#include "effPCPData.h"
#include "ps2asm.h"

/* DDS's names for the same objects. */
#define D_0031F460 dds3NeedleModelDefaultPoints
#define D_0031F520 dds3NeedleModelDefaultColors
#define D_0031F550 dds3NeedleModelDrawSurfaces
#define D_0056AD88 dds3NeedleModelPrimParam
#define aViewEye sdfViewTargetVector
#define dds3EffectBase effDefaultRandomState
#define effBTLModelGetSphereCenter btlUnitGetMuzzlePosVU
#define effBTLSkillCheckRun effBTLFieldColorUsesActorSelectors
#define effBTLSkillGetInvokeFormation effBTLFieldColorGetOriginalSelector
#define D_0031F450 dds3NeedleModelUpVector


/* One needle instance: position plus the colour the draw loop stamps. */
typedef struct dds3NeedleModelPart_tag
{
    f32 position[3];
    u8 unk0C[4];
    u32 abgr;
} dds3NeedleModelPart_tag;

typedef struct dds3NeedleModel_tag
{
    f32 matrix[16];
    dds3NeedleModelPart_tag *pPart;
    u32 number;
    u32 blend;
    f32 scale[3];
    u32 point;
    f32 (*positions)[4];
    f32 (*normals)[4];
    u32 *pVC;
    void *hVNMem;
    void *pMat;
    void *hMem;
} dds3NeedleModel_tag;

/* Argument block effPkt3DPrimMakePoly reads. */
typedef struct effPrimParam_t
{
    u16 primCount;
    u16 vertexCount;
    u16 primitive;
    u16 mask;
    u32 color;
    void *parameters;
    f32 (*positions)[4];
    f32 (*normals)[4];
    u8 unk18[8];
    u32 *colors;
    u8 unk24[8];
} effPrimParam_t;

typedef struct effDrawSurface_t
{
    u8 unk00[0x10];
    void (*append)(struct effDrawSurface_t *surface, void *packet);
} effDrawSurface_t;

extern effPrimParam_t D_0056AD88;   /* effPkt3DPrimMakePoly parameter block */
extern f32 D_0031F460[][4];         /* default triangle points */
extern u32 D_0031F520[];            /* default triangle colours */
extern effDrawSurface_t *D_0031F550[];

extern void *sdfGrAllocPacketBuff(s32 aSize);
extern void sdfDrawTagInit(void *a);
extern void sdfDrawTagLink(void *aTag, u32 aPacket);
extern void sdf3DPrimLinkMat(void *aTag, void *aMat, s32 aAllocator);
extern void sdf3DPrimLinkTm(void *aTag, s32 aAllocator);
extern void *effPkt3DPrimMakePoly(effPrimParam_t *aParm);
extern void MultMatrixNoArg(void);
extern void sdfMatFreeDelayed(void *aMat);
extern void sdfMemFreeDirect(void *aHandle);
extern void dds3NeedleModelVNCListDelete(dds3NeedleModel_tag *hNModel);


/* The copied 0x5C-byte needle parameters precede runtime ownership state. */
typedef struct effPCPNeedleSpiralParam_t {
    f32 position[4];
    u32 number;
    s32 life;
    u32 mode;
    s32 fade_in;
    s32 fade_out;
    u32 timing;
    f32 speed;
    f32 accele;
    f32 radius;
    f32 radius_r;
    f32 roll_s;
    u32 n_split;
    f32 radiusScale;
    f32 viewOffset;
    u32 centerColor;
    u32 outerColor;
    f32 width;
    s32 t_length;
    u32 t_abgr;
} effPCPNeedleSpiralParam_t;

typedef struct effPCPNeedleSpiralPart_tag {
    f32 pos[4];
    u8 old_pos[0x10];
    s32 frame;
    f32 radius;
    f32 radian;
} effPCPNeedleSpiralPart_tag; /* 0x2C */

typedef struct effAllocation_t effAllocation_t;
typedef struct trackPolySystem_t trackPolySystem_t;
typedef struct effRandState_t { u32 words[4]; } effRandState_t;
typedef struct effAxis_t { f32 x, y, z; } effAxis_t;
struct BtlUnit;

typedef struct effPCPNeedleSpiral_tag {
    effPCPNeedleSpiralParam_t param;
    effPCPNeedleSpiralPart_tag *pPart;
    u32 abgr;
    u32 number;
    trackPolySystem_t *hTrackPoly;
    dds3NeedleModel_tag *hNModel;
    effAllocation_t *hMem;
} effPCPNeedleSpiral_tag; /* 0x74, followed by number pPart */

#define PAR_CELL_TOPOLOGY_PAIR 0
#define EFFECT_RING_FULL_TURN 6.2831853f
extern effRandState_t dds3EffectBase[];
extern f32 aViewTarget[4], aViewEye[4], D_0031F450[4];
extern void *sdfMemAllocDirect(u32 aSize);
extern void *sdfMemLock(void *aHandle);
extern void *sdfMatNew(void);
extern void sdfMatSetConstant(void *aMat, f32 aRatio);
extern void *memset(void *a, s32 b, u32 c);
extern u32 dds3GetRandA(effRandState_t *name);
extern f32 dds3GetRandFloatA(effRandState_t *name);
extern u32 effBTLSkillCheckRun(void);
extern u32 effBTLSkillGetInvokeFormation(void);
extern void effBTLModelGetSphereCenter(struct BtlUnit *pFormation);
extern void LookAtMatrix(f32 *aTarget, f32 *aEye, f32 *aDown);
extern void ReverseMatrix(void);
extern f32 sdfSin(f32 aRadius);
extern f32 sdfCos(f32 aRadius);
extern void SetRotateMatrixAxis(f32 aAngle, const effAxis_t *aAxis);
extern u32 effColorBlend(u32 abgr0, u32 abgr1, f32 rate);
extern trackPolySystem_t *dds3TrackPolyCreate(s32 number, s32 face, s32 rec, u32 type);
extern void dds3TrackPolyDelete(trackPolySystem_t *hTrackPoly);
extern void dds3TrackPolySetMode(trackPolySystem_t *hTrackPoly, u16 mode);
extern void dds3TrackPolyVCSet(u32 hTrackPoly, s32 type, s32 abgr_in, s32 abgr_out);
extern void dds3TrackPolySet(trackPolySystem_t *hTrackPoly, s32 number, const u128 *pV);
extern void dds3TrackPolyVCRefresh_ParType1(trackPolySystem_t *hTrackPoly, s32 index);
extern void dds3TrackPolyColorSet(u32 hTrackPoly, s32 number, u32 color);
extern void dds3TrackPolyListInit(trackPolySystem_t *hTrackPoly, s32 number);
extern void dds3TrackPolyDraw(trackPolySystem_t *system);
effPCPNeedleSpiral_tag *effPCPNeedleSpiralCreate(effPCPNeedleSpiralParam_t *pParam);
dds3NeedleModel_tag *dds3NeedleModelCreate(u32 number);
void dds3NeedleModelDelete(dds3NeedleModel_tag *hNModel);
void dds3NeedleModelDraw(dds3NeedleModel_tag *hNModel);
void dds3NeedleModelVNCListCreate(dds3NeedleModel_tag *hNModel, u32 split, u32 radius, u32 length, f32 abgr_in, f32 abgr_out);
void dds3NeedleModelSetPosition(dds3NeedleModel_tag *hNModel, s32 index, f32 *pPos);
void dds3NeedleModelGetPosition(dds3NeedleModel_tag *hNModel, s32 index, f32 *pPos);
void dds3NeedleModelSetColor(dds3NeedleModel_tag *hNModel, s32 index, u32 abgr);
void dds3NeedleModelSetMatrix(dds3NeedleModel_tag *hNModel, const f32 *pMatrix);


effPCPNeedleSpiral_tag *effPCPNeedleSpiralCreate(effPCPNeedleSpiralParam_t *pParam) {
    effAllocation_t *allocation = sdfMemAllocDirect(pParam->number * sizeof(effPCPNeedleSpiralPart_tag) + sizeof(effPCPNeedleSpiral_tag));
    effPCPNeedleSpiral_tag *work = (effPCPNeedleSpiral_tag *)sdfMemLock(allocation);
    u32 slotIndex;
    dds3NeedleModel_tag *resource;
    u32 count, delayRange;
    effPCPNeedleSpiralPart_tag *slot;
    f32 radiusBase, radiusJitter;

    work->pPart = (effPCPNeedleSpiralPart_tag *)(work + 1);
    work->param = *pParam;
    work->hMem = allocation;
    work->number = pParam->number;
    work->abgr = 0x80808080;
    work->hTrackPoly = 0;
    resource = dds3NeedleModelCreate(pParam->number);
    resource->blend = pParam->mode;
    work->hNModel = resource;
    dds3NeedleModelVNCListCreate(resource, pParam->n_split, pParam->centerColor,
                            pParam->outerColor, pParam->radiusScale, pParam->viewOffset);
    work->hTrackPoly = dds3TrackPolyCreate(work->number, work->param.t_length, 0, PAR_CELL_TOPOLOGY_PAIR);
    dds3TrackPolyVCSet((u32)work->hTrackPoly, 1, work->param.t_abgr, work->param.t_abgr);
    dds3TrackPolySetMode(work->hTrackPoly, (u16)work->param.mode);
    count = work->number;
    delayRange = pParam->timing;
    slot = work->pPart;
    if (delayRange == 0) delayRange = 1;
    radiusBase = work->param.radius;
    radiusJitter = work->param.radius_r;
    for (slotIndex = 0; slotIndex < count; slotIndex++, slot++) {
        slot->radian = dds3GetRandFloatA(dds3EffectBase) * EFFECT_RING_FULL_TURN;
        slot->radius = radiusBase * (dds3GetRandFloatA(dds3EffectBase) * radiusJitter + (1.0f - radiusJitter));
        slot->frame = -(dds3GetRandA((effRandState_t *)dds3EffectBase) % delayRange);
        dds3NeedleModelSetColor(work->hNModel, slotIndex, 0x808080);
    }
    return work;
}

void effPCPNeedleSpiralDelete(effPCPNeedleSpiral_tag *hData) {
    dds3TrackPolyDelete(hData->hTrackPoly);
    dds3NeedleModelDelete(hData->hNModel);
    sdfMemFreeDirect(hData->hMem);
}

effPCPNeedleSpiral_tag *effPCPNeedleSpiralCreatePCF(void *data) {
    effPCPNeedleSpiralParam_t *work;

    work = dds3EffectPCPDataGetData(data, 0);
    return effPCPNeedleSpiralCreate(work);
}

effPCPNeedleSpiral_tag *effPCPNeedleSpiralCopy(effPCPNeedleSpiralParam_t *work) {
    return effPCPNeedleSpiralCreate(work);
}

void effPCPNeedleSpiralDraw(effPCPNeedleSpiral_tag *hData) {
    f32 position[4];
    f32 axis[4];
    f32 vertices[2][4];
    f32 resourcePosition[4];
    f32 viewDirection[4];
    f32 width[4];
    f32 viewOffset[4];
    effPCPNeedleSpiralPart_tag *slot;
    s32 fadeIn;
    u32 count;
    u32 i;
    s32 age;
    u32 baseColor;
    u32 color;
    f32 t;
    f32 speed;
    f32 acceleration;
    f32 angleStep;
    f32 value;
    u32 radius;
    s32 duration;
    s32 fadeOut;

    value = hData->param.width;
    width[0] = value;
    width[1] = value;
    width[2] = value;
    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(viewDirection);
    value = -hData->param.viewOffset * 0.75f;
    viewOffset[0] = value;
    viewOffset[1] = value;
    viewOffset[2] = value;

    if (effBTLSkillCheckRun() != 0) {
        effBTLModelGetSphereCenter((struct BtlUnit *)effBTLSkillGetInvokeFormation());
        SDF_VU0_STOREVECTOR_NOMEM(position);
        LookAtMatrix(hData->param.position, position, D_0031F450);
        ReverseMatrix();
        SDF_VU0_STOREMATRIX_NOMEM(hData->hNModel->matrix);
    } else {
        SDF_VU0_LOADMATRIX(hData->hNModel->matrix);
    }

    axis[0] = 0.0f;
    axis[2] = 1.0f;
    axis[1] = 0.0f;
    SDF_VU0_LOADVECTOR(axis);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_STOREVECTOR_NOMEM(axis);
    SDF_VU0_LOADVECTOR_B(viewOffset);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(viewOffset);

    count = hData->number;
    i = 0;
    slot = hData->pPart;
    speed = hData->param.speed;
    acceleration = hData->param.accele;
    duration = hData->param.life;
    fadeIn = hData->param.fade_in;
    fadeOut = hData->param.fade_out;
    angleStep = hData->param.roll_s;
    baseColor = hData->abgr;
    if (count != 0) {
        do {
            age = slot->frame;
            if (age == 0) {
                SDF_VU0_LOADMATRIX(hData->hNModel->matrix);
                radius = (u32)slot->radius;
                position[0] = sdfCos(slot->radian) * radius;
                position[1] = sdfSin(slot->radian) * radius;
                position[2] = 0.0f;
                SDF_VU0_LOADVECTOR(position);
                SDF_VU0_APPLYMATRIX_XYZ();
                SDF_VU0_STOREVECTOR_NOMEM(position);
                slot->pos[0] = position[0];
                slot->pos[1] = position[1];
                slot->pos[2] = position[2];
                dds3NeedleModelSetPosition(hData->hNModel, i, position);
            }

            if (age >= 0 && age <= duration) {
                dds3NeedleModelGetPosition(hData->hNModel, i, resourcePosition);
                SDF_VU0_LOADVECTOR(viewOffset);
                SDF_VU0_LOADVECTOR_B(resourcePosition);
                SDF_VU0_ADDVECTOR();
                SDF_VU0_STOREVECTOR_NOMEM(resourcePosition);

                position[0] = slot->pos[0];
                position[1] = slot->pos[1];
                position[2] = slot->pos[2];
                SetRotateMatrixAxis(slot->radian, (const effAxis_t *)axis);
                slot->radian += angleStep;
                SDF_VU0_LOADVECTOR(position);
                SDF_VU0_APPLYMATRIX_XYZ();
                SDF_VU0_STOREVECTOR_NOMEM(position);

                {
                    f32 elapsed = (f32)slot->frame;
                    f32 halfAcceleration = acceleration * 0.5f;
                    f32 distance = speed * elapsed + halfAcceleration * elapsed * elapsed;

                    elapsed = (f32)duration;
                    distance -= speed * elapsed + halfAcceleration * elapsed * elapsed;
                    position[0] += axis[0] * distance;
                    position[1] += axis[1] * distance;
                    position[2] += axis[2] * distance;
                }
                SDF_VU0_LOADVECTOR(position);
                SDF_VU0_LOADVECTOR_B(hData->param.position);
                SDF_VU0_ADDVECTOR();
                SDF_VU0_STOREVECTOR_NOMEM(position);
                dds3NeedleModelSetPosition(hData->hNModel, i, position);

                if (age < fadeIn && fadeIn != 0) {
                    t = (f32)age / (f32)fadeIn;
                } else if (duration == age && fadeOut != 0) {
                    t = 0.0f;
                } else {
                    t = fadeOut < duration - age || fadeOut == 0
                        ? 1.0f : (f32)(duration - age) / (f32)fadeOut;
                }
                color = effColorBlend(baseColor & 0xFFFFFF, baseColor, t);
                dds3NeedleModelSetColor(hData->hNModel, i, color);

                if (age > 0) {
                    SDF_VU0_LOADVECTOR(viewOffset);
                    SDF_VU0_LOADVECTOR_B(position);
                    SDF_VU0_ADDVECTOR();
                    SDF_VU0_COPYVECTOR_A_TO_C();
                    SDF_VU0_LOADVECTOR(resourcePosition);
                    SDF_VU0_COPYVECTOR_C_TO_B();
                    SDF_VU0_SUBVECTOR();
                    SDF_VU0_LOADVECTOR_B(viewDirection);
                    SDF_VU0_OUTERPRODUCT();
                    SDF_VU0_NORMALIZE();
                    SDF_VU0_LOADVECTOR_B(width);
                    SDF_VU0_MULVECTOR();
                    SDF_VU0_SWAPVECTOR_A_C();
                    SDF_VU0_COPYVECTOR_A_TO_B();
                    SDF_VU0_ADDVECTOR_A_C();
                    SDF_VU0_STOREVECTOR_NOMEM(vertices[0]);
                    SDF_VU0_COPYVECTOR_C_TO_A();
                    SDF_VU0_SUBVECTOR_B_A();
                    SDF_VU0_STOREVECTOR_B_NOMEM(vertices[1]);
                    dds3TrackPolySet(hData->hTrackPoly, i, (const u128 *)vertices);
                    dds3TrackPolyVCRefresh_ParType1(hData->hTrackPoly, i);
                    dds3TrackPolyColorSet((u32)hData->hTrackPoly, i, (color & 0xFF000000) | 0x808080);
                }
            } else {
                dds3NeedleModelSetColor(hData->hNModel, i, 0);
                dds3TrackPolyColorSet((u32)hData->hTrackPoly, i, 0);
                dds3TrackPolyListInit(hData->hTrackPoly, i);
            }
            slot->frame++;
            i++;
            slot++;
        } while (i < count);
    }
    dds3TrackPolyDraw(hData->hTrackPoly);
    dds3NeedleModelDraw(hData->hNModel);
}

void effPCPNeedleSpiralSetPosition(void *hData, void *pPos) {
    SDF_VU0_COPYVECTOR(hData, pPos);
}

void effPCPNeedleSpiralSetRotateMatrix(effPCPNeedleSpiral_tag *hData, void *pMatrix) {
    dds3NeedleModelSetMatrix(hData->hNModel, pMatrix);
}

void effPCPNeedleSpiralSetColor(effPCPNeedleSpiral_tag *hData, u32 abgr) {
    hData->abgr = abgr;
}

dds3NeedleModel_tag *dds3NeedleModelCreate(u32 number)
{
    struct effAllocation_t *allocation = sdfMemAllocDirect(number * sizeof(dds3NeedleModelPart_tag) + sizeof(dds3NeedleModel_tag));
    dds3NeedleModel_tag *work = (void *)sdfMemLock(allocation);
    dds3NeedleModelPart_tag *entry;
    u32 entryIndex;

    work->blend = 2;
    work->pPart = (dds3NeedleModelPart_tag *)(work + 1);
    work->hMem = allocation;
    work->number = number;
    work->scale[0] = 1.0f;
    work->scale[1] = 1.0f;
    work->scale[2] = 1.0f;
    work->hVNMem = NULL;
    SDF_VU0_STOREIDENTITY(work->matrix);
    work->pMat = sdfMatNew();
    sdfMatSetConstant(work->pMat, 1.0f);
    entry = work->pPart;
    for (entryIndex = 0; entryIndex < number; entryIndex++, entry++) {
        entry->abgr = 0x80808080;
    }
    memset(&D_0056AD88, 0, sizeof(D_0056AD88));
    D_0056AD88.primitive = 0x4000;
    return work;
}

void dds3NeedleModelDelete(dds3NeedleModel_tag *hNModel)
{
    sdfMatFreeDelayed(hNModel->pMat);
    dds3NeedleModelVNCListDelete(hNModel);
    sdfMemFreeDirect(hNModel->hMem);
}

void dds3NeedleModelDraw(dds3NeedleModel_tag *hNModel)
{
    f32 matrix[16] __attribute__((aligned(16)));
    void *packet;
    dds3NeedleModelPart_tag *entry;
    u32 entryIndex;
    u32 remaining;
    u32 perPrim;

    packet = sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(packet);
    sdf3DPrimLinkMat(packet, hNModel->pMat, 0);
    SDF_VU0_STOREIDENTITY(matrix);
    matrix[0] = hNModel->scale[0];
    matrix[5] = hNModel->scale[1];
    matrix[10] = hNModel->scale[2];
    SDF_VU0_LOADMATRIX(matrix);
    SDF_VU0_LOADMATRIX_B(hNModel->matrix);
    MultMatrixNoArg();
    SDF_VU0_STOREMATRIX(matrix);
    D_0056AD88.parameters = NULL;
    entry = hNModel->pPart;
    for (entryIndex = 0; entryIndex < hNModel->number; entryIndex++, entry++) {
        perPrim = 3;
        matrix[12] = entry->position[0];
        matrix[13] = entry->position[1];
        matrix[14] = entry->position[2];
        SDF_VU0_LOADMATRIX(matrix);
        sdf3DPrimLinkTm(packet, 0);
        if (hNModel->hVNMem == NULL) {
            remaining = 12;
            D_0056AD88.colors = D_0031F520;
            D_0056AD88.positions = D_0031F460;
            D_0056AD88.normals = NULL;
        } else {
            D_0056AD88.positions = hNModel->positions;
            D_0056AD88.colors = hNModel->pVC;
            D_0056AD88.normals = hNModel->normals;
            remaining = hNModel->point;
        }
        D_0056AD88.color = entry->abgr;
        D_0056AD88.primCount = 16;
        D_0056AD88.vertexCount = 48;
        while (remaining >= 48) {
            sdfDrawTagLink(packet, (u32)effPkt3DPrimMakePoly(&D_0056AD88));
            if (hNModel->hVNMem == NULL) {
                D_0056AD88.colors += 48;
                D_0056AD88.positions += 48;
            } else {
                D_0056AD88.colors += 48;
                D_0056AD88.positions += 48;
                D_0056AD88.normals += 48;
            }
            remaining -= 48;
        }
        if (remaining >= 3) {
            D_0056AD88.primCount = remaining / perPrim;
            D_0056AD88.vertexCount = remaining;
            sdfDrawTagLink(packet, (u32)effPkt3DPrimMakePoly(&D_0056AD88));
        }
    }
    D_0031F550[hNModel->blend]->append(D_0031F550[hNModel->blend], packet);
}

#include "sdk_libc.h"

/* Build a closed triangle fan: one outer vertex, the axis centre and the next
 * outer vertex per segment. */
void dds3NeedleModelVNCListCreate(dds3NeedleModel_tag *hNModel, u32 split, u32 radius, u32 length, f32 abgr_in, f32 abgr_out)
{
    void *allocation;
    f32 (*position)[4];
    f32 (*normal)[4];
    u32 *color;
    u32 vertexCount;
    u32 i;
    f32 angle;
    f32 step;
    f32 mid;

    if (split < 3) {
        split = 3;
    }
    if (hNModel->hVNMem != NULL) {
        dds3NeedleModelVNCListDelete(hNModel);
    }
    angle = 0.0f;
    vertexCount = split * 3;
    allocation = sdfMemAllocDirect(vertexCount * 36);
    hNModel->hVNMem = allocation;
    position = (f32 (*)[4])sdfMemLock(allocation);
    normal = position + vertexCount;
    color = (u32 *)(normal + vertexCount);
    hNModel->point = vertexCount;
    hNModel->positions = position;
    hNModel->normals = normal;
    hNModel->pVC = color;
    step = EFFECT_RING_FULL_TURN / (f32)split;
    for (i = 0; i < split; i++) {
        mid = step * 0.5f;
        normal[0][0] = sdfCos(angle);
        normal[0][1] = sdfSin(angle);
        normal[0][2] = 0.0f;
        mid = angle + step * 0.5f;
        position[0][0] = normal[0][0] * abgr_in;
        angle += step;
        position[0][1] = normal[0][1] * abgr_in;
        position[0][2] = -abgr_out;
        normal++;
        position++;
        normal[0][0] = sdfCos(mid);
        normal[0][1] = sdfSin(mid);
        normal[0][2] = 0.0f;
        position[0][0] = 0.0f;
        position[0][1] = 0.0f;
        position[0][2] = 0.0f;
        normal++;
        position++;
        normal[0][0] = sdfCos(angle);
        normal[0][1] = sdfSin(angle);
        normal[0][2] = 0.0f;
        color[0] = length;
        color[1] = radius;
        color[2] = length;
        color += 3;
        position[0][0] = normal[0][0] * abgr_in;
        position[0][1] = normal[0][1] * abgr_in;
        position[0][2] = -abgr_out;
        normal++;
        position++;
    }
}

void dds3NeedleModelVNCListDelete(dds3NeedleModel_tag *hNModel)
{
    if (hNModel->hVNMem != NULL) {
        sdfMemFreeDirect(hNModel->hVNMem);
    }
}

void dds3NeedleModelSetPosition(dds3NeedleModel_tag *hNModel, s32 index, f32 *pPos)
{
    f32 *dst = (f32 *)(index * sizeof(dds3NeedleModelPart_tag) + (s32)hNModel->pPart);

    dst[0] = pPos[0];
    dst[1] = pPos[1];
    dst[2] = pPos[2];
}

void dds3NeedleModelGetPosition(dds3NeedleModel_tag *hNModel, s32 index, f32 *pPos)
{
    f32 *src = (f32 *)(index * sizeof(dds3NeedleModelPart_tag) + (s32)hNModel->pPart);

    pPos[0] = src[0];
    pPos[1] = src[1];
    pPos[2] = src[2];
}

void dds3NeedleModelSetColor(dds3NeedleModel_tag *hNModel, s32 index, u32 abgr)
{
    hNModel->pPart[index].abgr = abgr;
}

/* vu0 routine: copy the resource transform through the matrix bank. */
void dds3NeedleModelSetMatrix(dds3NeedleModel_tag *hNModel, const f32 *pMatrix) {
    VU0_COPY_MATRIX(hNModel->matrix, pMatrix);
}

