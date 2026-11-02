#include "common.h"
#include "libsdf.h"

#define func_002BFF98 sdfDevResizeBufferedRequest
#define sdfTexFreeDelayed sdfTexReleaseReferenceViaHandler
#define sdfMatSetRefMap sdfSetPrimaryStateWordFourth
#define sdfMatSetTex sdfSetAssetPrimaryTextureAddress
#define D_00529438 D_00398198

extern void *sdfChipAllocDirect(s32 aSize);
extern void *sdfChipAllocZeroDirect(s32 aSize);
extern void sdfChipFreeDirect(void *aPtr);

extern sdfItemArray_t *sdfItemArrayNew(s32 aInitCapa, s32 aElemSize, s32 aGrowStep);
extern void sdfItemArrayFree(sdfItemArray_t *aArray);
extern void sdfItemArrayGrow(sdfItemArray_t *aArray);
extern void func_002BFF98(sdfItemArray_t *aArray, s32 aCapacity);

extern void sdfDelayedResrcInitCB(sdfDelayedResrcCB_t *aCB, sdfDelayedResrcFreeFunc aFreeFunc);
extern void sdfDelayedResrcEntry(sdfDelayedResrcCB_t *aCB, void *aItem);

extern sdfDelayedResrcCB_t sdfMatDelayedResrcCB;
extern sdfDelayedResrcCB_t sdfMatDelayedResrcCB2;
extern s32 sdfMatAllocCount;

/* The GS ALPHA_2 value per multi-texture method, indexed by sdfMat_t.multiTexMethod. */
extern u16 D_00529438[];

extern void sdfTexFreeDelayed(sdfTexHandle_t *aHandle);
extern sdfTexHandle_t *sdfTexDup(sdfTexHandle_t *aHandle);
extern void sdfTexSetClutColor(sdfTexHandle_t *aHandle, s32 aColor, f32 aRatio);
extern u64 sdfTexGetTex0Reg(sdfTexHandle_t *aHandle);
extern u64 sdfTexGetTex1Reg(sdfTexHandle_t *aHandle);
extern u64 sdfTexGetClampReg(sdfTexHandle_t *aHandle);

extern f32 sdfSin(f32 aRadius);
extern f32 sdfCos(f32 aRadius);

sdfItemArray_t *sdfTexListNew(s32 aInitCapa);
sdfItemArray_t *sdfMatListNew(s32 aInitCapa);
void sdfMatListGrow(sdfItemArray_t *aList);
void sdfMatListFree(sdfItemArray_t *aList);
void sdfMatFree(sdfMat_t *aMat);
sdfMatUvwParm_t *sdfMatAllocTexUvwParm(void);
sdfMatUvwParm_t *sdfMatGetTexUvwParm(sdfMat_t *aMat);
sdfMatUvwParm_t *sdfMatGetMultiTexUvwParm(sdfMat_t *aMat);
void sdfMatCopyStat(sdfMat_t *aDstMat, sdfMat_t *aSrcMat);
void sdfMatListCopyStat(sdfItemArray_t *aDst, sdfItemArray_t *aSrc);
void sdfMatSetUvwParm(sdfMatTexTm_t *aUvwPacket, const sdfMatUvwParm_t *aParm);
void sdfMatUpdatePacketBasic(sdfMat_t *aMat, sdfMatPacket_t *aPacket);
void sdfMatUpdatePacketUvwParm(sdfMat_t *aMat, sdfMatPacket_t *aPacket);
void sdfMatUpdatePacketMultiTex(sdfMat_t *aMat, sdfMatPacket_t *aPacket);
void sdfMatUpdatePacketFaceDiv(sdfMat_t *aMat, sdfMatPacket_t *aPacket);
void sdfMatSetAmbient(sdfMat_t *aMat, u32 aColor);
void sdfMatSetDiffuse(sdfMat_t *aMat, u32 aColor);
void sdfMatSetSpecular(sdfMat_t *aMat, u32 aColor);
void sdfMatSetRefMap(sdfMat_t *aMat, u32 aColor);
void sdfMatSetConstant(sdfMat_t *aMat, f32 aRatio);
void sdfMatSetTex(sdfMat_t *aMat, sdfTexHandle_t *aTex);
void sdfMatSetTexUvw(sdfMat_t *aMat, f32 aOffsetX, f32 aOffsetY, f32 aTileX, f32 aTileY,
                     f32 aAngle);
void sdfMatSetMultiDiffuse(sdfMat_t *aMat, u32 aColor);
void sdfMatSetMultiMethod(sdfMat_t *aMat, u32 aMethod);
void sdfMatSetMultiTex(sdfMat_t *aMat, sdfTexHandle_t *aTex);
void sdfMatSetMultiTexUvw(sdfMat_t *aMat, f32 aOffsetX, f32 aOffsetY, f32 aTileX, f32 aTileY,
                          f32 aAngle);
void sdfMatSetFaceDivParm(sdfMat_t *aMat, f32 aBias, f32 aSoften);
sdfMat_t *sdfMatNew(void);
u8 *sdfMatSet(sdfMat_t *aMatPtr, sdfItemArray_t *aTexList, u8 *aDef);
void sdfMatListAdd(sdfItemArray_t *aList, sdfMat_t *aMat);

/* 0 normal; 1 and 2 make sdfMatListUpdatePacket refresh the basic packet even when
 * it is clean, and 1 also zeroes its specular color. */
extern s32 sdfForcedAssetTextureMode;

void sdfApplyAssetEntryChangesWithForcedTexture(sdfMat_t *aMat, s32 aId);

void sdfSetForcedAssetTextureMode(s32 aMode)
{
    sdfForcedAssetTextureMode = aMode;
}

sdfItemArray_t *sdfTexListNew(s32 aInitCapa)
{
    return sdfItemArrayNew(aInitCapa, 4, 4);
}

void sdfTexListFree(sdfItemArray_t *aList, s32 aReleaseItems)
{
    s32 i;
    s32 count;

    if (aList == NULL) {
        return;
    }
    if (aReleaseItems != 0) {
        count = aList->count;
        for (i = 0; i < count; i++) {
            sdfTexFreeDelayed((sdfTexHandle_t *)((u32 *)aList->items)[i]);
        }
    }
    sdfItemArrayFree(aList);
}

void sdfTexListAdd(sdfItemArray_t *aList, sdfTexHandle_t *aTexHandle)
{
    if (aList->count >= (s16)aList->capacity) {
        sdfItemArrayGrow(aList);
    }
    ((u32 *)aList->items)[aList->count] = (u32)aTexHandle;
    aList->count++;
}

void sdfTexListSetCapacity(sdfItemArray_t *aList, s32 aCapa, s32 aReleaseItems)
{
    s32 i;
    s32 count;

    count = aList->count;
    if (aCapa < count && aReleaseItems != 0) {
        /* Retail only counts down to the new capacity here; the loop body is
         * empty and ee-gcc pads it to the R5900 short-loop minimum. */
        i = aCapa;
        do {
            i++;
        } while (i != count);
        aList->count = aCapa;
    }
    func_002BFF98(aList, aCapa);
}

sdfItemArray_t *sdfTexListDup(sdfItemArray_t *aList)
{
    s32 count;
    sdfItemArray_t *dup;
    s32 i;

    if (aList == NULL) {
        return NULL;
    }
    count = aList->count;
    dup = sdfTexListNew(count);
    for (i = 0; i < count; i++) {
        ((u32 *)dup->items)[i] = (u32)sdfTexDup((sdfTexHandle_t *)((u32 *)aList->items)[i]);
    }
    dup->count = count;
    return dup;
}

void sdfTexListSetClutColor(sdfItemArray_t *aList, s32 aColor, f32 aRatio)
{
    s32 i;
    s32 count;

    if (aList == NULL) {
        return;
    }
    count = aList->count;
    for (i = 0; i < count; i++) {
        sdfTexHandle_t *handle = ((sdfTexHandle_t **)aList->items)[i];

        if (handle->numOfCLUT != 0) {
            sdfTexSetClutColor(handle, aColor, aRatio);
        }
    }
}

void sdfMatInitSystem(void)
{
    sdfDelayedResrcInitCB(&sdfMatDelayedResrcCB, (sdfDelayedResrcFreeFunc)sdfMatListFree);
    sdfDelayedResrcInitCB(&sdfMatDelayedResrcCB2, (sdfDelayedResrcFreeFunc)sdfMatFree);
}

sdfItemArray_t *sdfMatListNew(s32 aInitCapa)
{
    return sdfItemArrayNew(aInitCapa, 4, 8);
}

void sdfMatListFree(sdfItemArray_t *aList)
{
    s32 i;

    for (i = 0; i < aList->count; i++) {
        sdfMatFree(((sdfMat_t **)aList->items)[i]);
    }
    sdfItemArrayFree(aList);
}

void sdfMatListFreeResrc(void *aList, s32 aDelayed)
{
    if (aList == NULL) {
        return;
    }
    if (aDelayed != 0) {
        sdfDelayedResrcEntry(&sdfMatDelayedResrcCB, aList);
    } else {
        sdfItemArrayFree(aList);
    }
}

void sdfMatListGrow(sdfItemArray_t *aList)
{
    sdfItemArrayGrow(aList);
}

void sdfMatListAdd(sdfItemArray_t *aList, sdfMat_t *aMat)
{
    if (aList->count >= (s16)aList->capacity) {
        sdfMatListGrow(aList);
    }
    ((sdfMat_t **)aList->items)[aList->count] = aMat;
    aList->count++;
}

void sdfMatSetAmbient(sdfMat_t *aMat, u32 aColor)
{
    aMat->ambient = aColor;
    aMat->duty |= 3;
}

void sdfMatSetDiffuse(sdfMat_t *aMat, u32 aColor)
{
    aMat->diffuse = aColor;
    aMat->duty |= 3;
}

void sdfMatSetSpecular(sdfMat_t *aMat, u32 aColor)
{
    aMat->specular = aColor;
    aMat->duty |= 3;
}

void sdfMatSetRefMap(sdfMat_t *aMat, u32 aColor)
{
    aMat->refmap = aColor;
    aMat->duty |= 3;
}

void sdfMatSetConstant(sdfMat_t *aMat, f32 aRatio)
{
    aMat->constant = aRatio;
    aMat->duty = aMat->duty | 3;
}

void sdfMatSetTex(sdfMat_t *aMat, sdfTexHandle_t *aTex)
{
    aMat->texHandle = aTex;
    aMat->duty |= 3;
}

sdfMatUvwParm_t *sdfMatAllocTexUvwParm(void)
{
    sdfMatUvwParm_t *parm;

    parm = sdfChipAllocDirect(sizeof(sdfMatUvwParm_t));
    /* Tile (1, 1), offset (0, 0), angle 0, written as the three quadword halves. */
    *(u64 *)&parm->tile[0] = 0x3F8000003F800000ULL;
    *(u64 *)&parm->offset[0] = 0;
    *(u32 *)&parm->angle = 0;
    return parm;
}

sdfMatUvwParm_t *sdfMatGetTexUvwParm(sdfMat_t *aMat)
{
    sdfMatUvwParm_t *parm = aMat->uvwParm;

    if (parm == NULL) {
        parm = sdfMatAllocTexUvwParm();
        aMat->uvwParm = parm;
    }
    return parm;
}

void sdfMatSetTexUvw(sdfMat_t *aMat, f32 aOffsetX, f32 aOffsetY, f32 aTileX, f32 aTileY,
                     f32 aAngle)
{
    f32 *values = sdfMatGetTexUvwParm(aMat)->offset;

    values[0] = aOffsetX;
    values[1] = aOffsetY;
    values[2] = aTileX;
    values[3] = aTileY;
    values[4] = aAngle;
    aMat->duty |= 0xC;
}

void sdfMatSetTexUvwByPtr(sdfMat_t *aMat, const f32 *aParm)
{
    f32 *values = sdfMatGetTexUvwParm(aMat)->offset;

    values[0] = aParm[0];
    values[1] = aParm[1];
    values[2] = aParm[2];
    values[3] = aParm[3];
    values[4] = aParm[4];
    aMat->duty |= 0xC;
}

void sdfMatSetMultiDiffuse(sdfMat_t *aMat, u32 aColor)
{
    aMat->multiTexDiffuse = aColor;
    aMat->duty |= 0x30;
}

void sdfMatSetMultiMethod(sdfMat_t *aMat, u32 aMethod)
{
    aMat->multiTexMethod = aMethod;
    aMat->duty |= 0x30;
}

void sdfMatSetMultiTex(sdfMat_t *aMat, sdfTexHandle_t *aTex)
{
    aMat->multiTexHandle = aTex;
    aMat->duty |= 0x30;
}

sdfMatUvwParm_t *sdfMatGetMultiTexUvwParm(sdfMat_t *aMat)
{
    sdfMatUvwParm_t *parm = aMat->multiTexUvwParm;

    if (parm == NULL) {
        parm = sdfMatAllocTexUvwParm();
        aMat->multiTexUvwParm = parm;
    }
    return parm;
}

void sdfMatSetMultiTexUvw(sdfMat_t *aMat, f32 aOffsetX, f32 aOffsetY, f32 aTileX, f32 aTileY,
                          f32 aAngle)
{
    f32 *values = sdfMatGetMultiTexUvwParm(aMat)->offset;

    values[0] = aOffsetX;
    values[1] = aOffsetY;
    values[2] = aTileX;
    values[3] = aTileY;
    values[4] = aAngle;
    aMat->duty |= 0x30;
}

void sdfMatSetMultiTexUvwByPtr(sdfMat_t *aMat, const f32 *aParm)
{
    f32 *values = sdfMatGetMultiTexUvwParm(aMat)->offset;

    values[0] = aParm[0];
    values[1] = aParm[1];
    values[2] = aParm[2];
    values[3] = aParm[3];
    values[4] = aParm[4];
    aMat->duty |= 0x30;
}

void sdfMatSetFaceDivParm(sdfMat_t *aMat, f32 aBias, f32 aSoften)
{
    aMat->fdBias = aBias;
    aMat->fdSoften = aSoften;
    aMat->duty = aMat->duty | 0xC0;
}

sdfMat_t *sdfMatNew(void)
{
    sdfMat_t *mat;
    u32 *packet;
    s32 i;

    sdfMatAllocCount++;
    mat = sdfChipAllocZeroDirect(sizeof(sdfMat_t));
    mat->duty = 0xFF;
    for (i = 0; i != 2; i++) {
        packet = sdfChipAllocDirect(sizeof(sdfMatPacket_t));
        mat->packet[i] = (sdfMatPacket_t *)packet;
        packet[0] = 0x6E05C000;
        packet[0x18 / 4] = 0x6005C005;
        packet[0x30 / 4] = 0;
        packet[0x34 / 4] = 0x640CC00A;
        ((sdfMatPacket_t *)packet)->cmd_call_spec = 0x0400000C;
        ((sdfMatPacket_t *)packet)->cmd_call_kind = 0x14000000;
    }
    mat->fdBias = 0;
    mat->fdSoften = 0;
    mat->ambient = 0x80808080;
    mat->diffuse = 0x80808080;
    mat->multiTexDiffuse = 0x80808080;
    return mat;
}

u8 *sdfMatSet(sdfMat_t *aMatPtr, sdfItemArray_t *aTexList, u8 *aDef)
{
    u32 flags;
    u8 *cursor;
    u32 packed;

    aMatPtr->id = *(u32 *)aDef;
    aMatPtr->flags = *(u16 *)(aDef + 4);
    flags = *(u16 *)(aDef + 6);
    cursor = aDef + 8;
    if (flags & 0x1) {
        sdfMatSetAmbient(aMatPtr, *(u32 *)cursor);
        cursor += 4;
    }
    if (flags & 0x2) {
        sdfMatSetDiffuse(aMatPtr, *(u32 *)cursor);
        cursor += 4;
    }
    if (flags & 0x4) {
        sdfMatSetTex(aMatPtr, (sdfTexHandle_t *)((u32 *)aTexList->items)[*(u16 *)cursor]);
        cursor += 4;
    }
    if (flags & 0x8) {
        sdfMatSetTexUvw(aMatPtr, ((f32 *)cursor)[0], ((f32 *)cursor)[1], ((f32 *)cursor)[2],
                        ((f32 *)cursor)[3], ((f32 *)cursor)[4]);
        cursor += 0x14;
    }
    if (flags & 0x10) {
        sdfMatSetMultiDiffuse(aMatPtr, *(u32 *)cursor);
        cursor += 4;
    }
    if (flags & 0x20) {
        packed = *(u32 *)cursor;
        cursor += 4;
        sdfMatSetMultiTex(aMatPtr, (sdfTexHandle_t *)((u32 *)aTexList->items)[packed & 0xFFFF]);
        sdfMatSetMultiMethod(aMatPtr, packed >> 16);
    }
    if (flags & 0x40) {
        sdfMatSetMultiTexUvw(aMatPtr, ((f32 *)cursor)[0], ((f32 *)cursor)[1], ((f32 *)cursor)[2],
                             ((f32 *)cursor)[3], ((f32 *)cursor)[4]);
        cursor += 0x14;
    }
    if (flags & 0x80) {
        sdfMatSetSpecular(aMatPtr, *(u32 *)cursor);
        cursor += 4;
    }
    if (flags & 0x100) {
        sdfMatSetRefMap(aMatPtr, *(u32 *)cursor);
        cursor += 4;
    }
    if (flags & 0x200) {
        sdfMatSetConstant(aMatPtr, *(f32 *)cursor);
        cursor += 4;
    }
    if (flags & 0x400) {
        sdfMatSetFaceDivParm(aMatPtr, ((f32 *)cursor)[0], ((f32 *)cursor)[1]);
        cursor += 8;
    }
    return cursor;
}

void sdfMatFree(sdfMat_t *aMat)
{
    if (aMat == NULL) {
        return;
    }
    sdfMatAllocCount--;
    sdfChipFreeDirect(aMat->packet[0]);
    sdfChipFreeDirect(aMat->packet[1]);
    sdfChipFreeDirect(aMat->uvwParm);
    sdfChipFreeDirect(aMat->multiTexUvwParm);
    sdfChipFreeDirect(aMat);
}

void sdfMatFreeDelayed(sdfMat_t *aMat)
{
    if (aMat != NULL) {
        sdfDelayedResrcEntry(&sdfMatDelayedResrcCB2, aMat);
    }
}

sdfDmaTag_t *sdfMatMakePacket(sdfMat_t *aMat, sdfDmaTag_t *aBuffer, s32 aId)
{
    aBuffer->id = 0x30;
    aBuffer->addr = (void *)((u32)aMat->packet[aId] & 0x0FFFFFFF);
    aBuffer->qwc = 0xA;
    aBuffer->dat0 = 0;
    aBuffer->dat1 = 0;
    return (sdfDmaTag_t *)((u8 *)aBuffer + 0x10);
}

void sdfMatUpdatePacketBasic(sdfMat_t *aMat, sdfMatPacket_t *aPacket)
{
    sdfTexHandle_t *handle;

    aPacket->ambient = aMat->ambient;
    aPacket->constant = aMat->constant;
    aPacket->diffuse = aMat->diffuse;
    if (sdfForcedAssetTextureMode == 1) {
        aPacket->specularColor = 0;
    } else {
        aPacket->specularColor = aMat->specular;
    }
    aPacket->refmapColor = aMat->refmap;
    handle = aMat->texHandle;
    if (handle != NULL) {
        aPacket->tex1Data = sdfTexGetTex1Reg(handle);
        aPacket->tex0Data = sdfTexGetTex0Reg(handle);
        aPacket->clampData = sdfTexGetClampReg(handle);
    }
}

void sdfMatSetUvwParm(sdfMatTexTm_t *aUvwPacket, const sdfMatUvwParm_t *aParm)
{
    f32 offsetU;
    f32 offsetV;
    f32 tileU;
    f32 tileV;
    f32 angle;
    f32 s;
    f32 c;
    f32 m0;
    f32 m1;
    f32 m2;
    f32 m3;

    if (aParm == NULL) {
        aUvwPacket->q[0] = 0x000000003F800000ULL;
        aUvwPacket->q[1] = 0x3F80000000000000ULL;
        aUvwPacket->q[2] = 0;
        return;
    }

    angle = -aParm->angle;
    s = sdfSin(angle);
    c = sdfCos(angle);
    offsetU = aParm->offset[0];
    offsetV = aParm->offset[1];
    tileU = aParm->tile[0];
    tileV = aParm->tile[1];

    m0 = c * tileU;
    m1 = s * tileV;
    m2 = -s * tileU;
    m3 = c * tileV;
    aUvwPacket->m[0] = m0;
    aUvwPacket->m[1] = m1;
    aUvwPacket->m[2] = m2;
    aUvwPacket->m[3] = m3;
    aUvwPacket->m[4] = 0.5f - m0 * (offsetU + 0.5f) - m2 * (0.5f - offsetV);
    aUvwPacket->m[5] = 0.5f - m1 * (offsetU + 0.5f) - m3 * (0.5f - offsetV);
}

void sdfMatUpdatePacketUvwParm(sdfMat_t *aMat, sdfMatPacket_t *aPacket)
{
    sdfMatSetUvwParm(&aPacket->texTm, aMat->uvwParm);
}

void sdfMatUpdatePacketMultiTex(sdfMat_t *aMat, sdfMatPacket_t *aPacket)
{
    sdfTexHandle_t *handle = aMat->multiTexHandle;
    u32 method;

    aPacket->mtexDiffuse = aMat->multiTexDiffuse;
    method = aMat->multiTexMethod;
    aPacket->mtexAlgo = method;
    aPacket->mtexAlpha = D_00529438[method];
    if (handle != NULL) {
        aPacket->mtexTex1Data = sdfTexGetTex1Reg(handle);
        aPacket->mtexTex0Data = sdfTexGetTex0Reg(handle);
        aPacket->mtexClampData = sdfTexGetClampReg(handle);
    }
    sdfMatSetUvwParm(&aPacket->mtexTexTm, aMat->multiTexUvwParm);
}

void sdfMatUpdatePacketFaceDiv(sdfMat_t *aMat, sdfMatPacket_t *aPacket)
{
    aPacket->fdBias = aMat->fdBias;
    aPacket->fdSoften = aMat->fdSoften;
}

void sdfMatUpdatePacket(sdfMat_t *aMat, s32 aId)
{
    u8 duty = aMat->duty;
    sdfMatPacket_t *packet = aMat->packet[aId];

    if ((duty >> aId) & 1) {
        sdfMatUpdatePacketBasic(aMat, packet);
    }
    if (duty & (4 << aId)) {
        sdfMatUpdatePacketUvwParm(aMat, packet);
    }
    if (duty & (0x10 << aId)) {
        sdfMatUpdatePacketMultiTex(aMat, packet);
    }
    if (duty & (0x40 << aId)) {
        sdfMatUpdatePacketFaceDiv(aMat, packet);
    }
    aMat->duty = duty & (0x55 << (aId ^ 1));
}

void sdfApplyAssetEntryChangesWithForcedTexture(sdfMat_t *aMat, s32 aId)
{
    u8 duty = aMat->duty;
    sdfMatPacket_t *packet = aMat->packet[aId];

    if ((duty >> aId) & 1) {
        sdfMatUpdatePacketBasic(aMat, packet);
    } else if (sdfForcedAssetTextureMode != 0) {
        sdfMatUpdatePacketBasic(aMat, packet);
    }
    if (duty & (4 << aId)) {
        sdfMatUpdatePacketUvwParm(aMat, packet);
    }
    if (duty & (0x10 << aId)) {
        sdfMatUpdatePacketMultiTex(aMat, packet);
    }
    if (duty & (0x40 << aId)) {
        sdfMatUpdatePacketFaceDiv(aMat, packet);
    }
    aMat->duty = duty & (0x55 << (aId ^ 1));
}

sdfItemArray_t *sdfMatListMake(sdfItemArray_t *aTexList, u32 *aData)
{
    u32 count = *aData;
    u8 *cursor = (u8 *)(aData + 1);
    sdfItemArray_t *list = sdfMatListNew(count >= 0x20 ? count : 0x20);

    while (count != 0) {
        sdfMat_t *mat = sdfMatNew();

        cursor = sdfMatSet(mat, aTexList, cursor);
        sdfMatListAdd(list, mat);
        count--;
    }
    return list;
}

void sdfResourceListApplyEntryChanges(sdfItemArray_t *aMatList, s32 aId)
{
    s32 i;
    s32 count = aMatList->count;
    sdfMat_t **items = (sdfMat_t **)aMatList->items;

    for (i = 0; i < count; i++) {
        sdfMatUpdatePacket(items[i], aId);
    }
}

void sdfMatListUpdatePacket(sdfItemArray_t *aMatList, s32 aId)
{
    s32 i;
    s32 count = aMatList->count;
    sdfMat_t **items = (sdfMat_t **)aMatList->items;

    for (i = 0; i < count; i++) {
        sdfApplyAssetEntryChangesWithForcedTexture(items[i], aId);
    }
}

void sdfMatCopyStat(sdfMat_t *aDstMat, sdfMat_t *aSrcMat)
{
    sdfMatUvwParm_t *parm;

    aDstMat->duty = 0xFF;
    aDstMat->ambient = aSrcMat->ambient;
    aDstMat->diffuse = aSrcMat->diffuse;
    aDstMat->constant = aSrcMat->constant;
    aDstMat->multiTexDiffuse = aSrcMat->multiTexDiffuse;
    aDstMat->flags = aSrcMat->flags;
    aDstMat->texHandle = aSrcMat->texHandle;
    aDstMat->specular = aSrcMat->specular;
    aDstMat->refmap = aSrcMat->refmap;
    parm = aSrcMat->uvwParm;
    if (parm != NULL) {
        *sdfMatGetTexUvwParm(aDstMat) = *parm;
    }
    parm = aSrcMat->multiTexUvwParm;
    if (parm != NULL) {
        *sdfMatGetMultiTexUvwParm(aDstMat) = *parm;
    }
    aDstMat->fdBias = aSrcMat->fdBias;
    aDstMat->fdSoften = aSrcMat->fdSoften;
}

void sdfMatListCopyStat(sdfItemArray_t *aDst, sdfItemArray_t *aSrc)
{
    s32 i;
    s32 count = aDst->count;
    u32 *src = aSrc->items;
    u32 *dst = aDst->items;

    for (i = 0; i < count; i++) {
        sdfMatCopyStat((sdfMat_t *)dst[i], (sdfMat_t *)src[i]);
    }
}

INCLUDE_SDATA(const s32, "sdf/sdfMat", sdfMatAllocCount);

INCLUDE_SDATA(const s32, "sdf/sdfMat", sdfForcedAssetTextureMode);
