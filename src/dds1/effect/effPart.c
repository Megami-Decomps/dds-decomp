#include "common.h"
#include "motion.h"
#include "model.h"
#include "effPCPData.h"
#include "ps2asm.h"

extern sdfVector4_t effPartModelPosInit;

extern sdfVector4_t effPartModelRotInit;

extern sdfVector4_t effPartModelScaleInit;

typedef struct
{
    u16 type;
    void *pHandle;
} effPartHandle_t;

typedef struct
{
    u32 kateg;
    u32 param;
    void *pHandle;
} dds3EffectPCP_t;



typedef struct
{
    void *(*create)(void *);
    void (*draw)(void *);
    void (*destroy)(void *);
    void *(*copy)(void *);
    void (*setPosition)(void *, void *);
    void (*setScale)(void *, f32);
    void (*setRotateMatrix)(void *, void *);
    void (*setColor)(void *, u32);
    void (*setDirection)(void *, void *);
    void (*setArea)(void *, f32);
} dds3EffectFunc_t;

void mdlSetPosition(mdlObj_t *aHandle);

void mdlSetRotate(mdlObj_t *aHandle);

void mdlSetScale(mdlObj_t *aHandle);

void mdlSetColor(mdlObj_t *aHandle, u32 aColor);

void mdlLoopMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber);

extern dds3EffectFunc_t dds3EffectFuncTable[7];

extern void *kwlnSceneSurfaceSets[13][4];
#define D_002F69B8 kwlnSceneSurfaceSets[10]

extern u16 effPartModelSerial;

extern s32 effBTLCutCount;

/* Header of a packed model an effect part carries, followed by the pack data. */
typedef struct
{
    u32 size;
    u8 reserve[0xC];
    u8 data[1];
} dds3EffectPartModelData_t;

void *dds3BillboardCreate(s32 kind, addr_t data);

void mdlPackDecode(s32 major, s32 minor, s32 regFlags, const u8 *buff, s32 leng);

void mdlLink(mdlObj_t *aHandle, void **aModelOt);

void mdlFree(mdlObj_t *aHandle);

extern dds3EffectFunc_t EffectPCPFuncTable[];

extern void **effPartPCPDataTable[];

void *sdfChipAllocDirect(s32 aSize);

void sdfChipFreeDirect(void *aPtr);

void dds3BillboardSetScale(void *hBillboard, f32 x, f32 y);

s32 mdlGetMajorNum(mdlObj_t *aHandle);

s32 mdlGetMinorNum(mdlObj_t *aHandle);

mdlObj_t *mdlLoad(u32 aMajor, u32 aMinor);

void dds3EffectPartModelInit(mdlObj_t *hModel);

effPartHandle_t *dds3EffectPartCreate(u16 type, void *pData);





extern u32 effBTLPointLightProcess;

extern s8 effBTLFieldColorOn;

extern s8 D_0054F0F1;

extern const char *D_0054F0F4;

extern u8 effBTLDebugCameraAllEdit;

extern u8 effBTLDebugCameraAllTable;

extern u8 D_0054F1C3;

extern s32 effBTLPartyIndex;

extern s8 effBTLUnitLightOn;

extern char effBedDirName[];

void *dds3EffectPartGetHandle(effPartHandle_t *hEffPart);

u16 dds3EffectPartGetType(effPartHandle_t *hEffPart);


void dds3EffectPartDelete(effPartHandle_t *hEffPart);

effPartHandle_t *dds3EffectPartCopy(effPartHandle_t *hEffPartSrc);

void dds3EffectPartDraw(effPartHandle_t *hEffPart);

void dds3EffectPartSetPosition(effPartHandle_t *hEffPart, sdfVector4_t *pPos);

void dds3EffectPartSetScale(effPartHandle_t *hEffPart, f32 scale);

void dds3EffectPartSetColor(effPartHandle_t *hEffPart, u32 abgr);

void *dds3EffectPartGetHandle(effPartHandle_t *hEffPart)
{
    return hEffPart->pHandle;
}

u16 dds3EffectPartGetType(effPartHandle_t *hEffPart)
{
    return hEffPart->type;
}

effPartHandle_t *dds3EffectPartCreate(u16 type, void *pData)
{
    effPartHandle_t *hEffPart;

    hEffPart = sdfChipAllocDirect(sizeof(effPartHandle_t));
    hEffPart->type = type;
    hEffPart->pHandle = dds3EffectFuncTable[type].create(pData);
    return hEffPart;
}

void dds3EffectPartDelete(effPartHandle_t *hEffPart)
{
    dds3EffectFuncTable[hEffPart->type].destroy(hEffPart->pHandle);
    sdfChipFreeDirect(hEffPart);
}

void dds3EffectPartDraw(effPartHandle_t *hEffPart)
{
    dds3EffectFuncTable[hEffPart->type].draw(hEffPart->pHandle);
}

effPartHandle_t *dds3EffectPartCopy(effPartHandle_t *hEffPartSrc)
{
    effPartHandle_t *hEffPart;

    hEffPart = sdfChipAllocDirect(sizeof(effPartHandle_t));
    hEffPart->type = hEffPartSrc->type;
    hEffPart->pHandle = dds3EffectFuncTable[hEffPartSrc->type].copy(hEffPartSrc->pHandle);
    return hEffPart;
}

void dds3EffectPartSetPosition(effPartHandle_t *hEffPart, sdfVector4_t *pPos)
{
    if (dds3EffectFuncTable[hEffPart->type].setPosition != NULL) {
        dds3EffectFuncTable[hEffPart->type].setPosition(hEffPart->pHandle, pPos);
    }
}

void dds3EffectPartSetScale(effPartHandle_t *hEffPart, f32 scale)
{
    if (dds3EffectFuncTable[hEffPart->type].setScale != NULL) {
        dds3EffectFuncTable[hEffPart->type].setScale(hEffPart->pHandle, scale);
    }
}

void dds3EffectPartSetRotateMatrix(effPartHandle_t *hEffPart, void *pMatrix)
{
    if (dds3EffectFuncTable[hEffPart->type].setRotateMatrix != NULL) {
        dds3EffectFuncTable[hEffPart->type].setRotateMatrix(hEffPart->pHandle, pMatrix);
    }
}

void dds3EffectPartSetColor(effPartHandle_t *hEffPart, u32 abgr)
{
    if (dds3EffectFuncTable[hEffPart->type].setColor != NULL) {
        dds3EffectFuncTable[hEffPart->type].setColor(hEffPart->pHandle, abgr);
    }
}

void dds3EffectPartSetDirection(effPartHandle_t *hEffPart, void *pMatrix)
{
    if (dds3EffectFuncTable[hEffPart->type].setDirection != NULL) {
        dds3EffectFuncTable[hEffPart->type].setDirection(hEffPart->pHandle, pMatrix);
    }
}

void dds3EffectPartSetArea(effPartHandle_t *hEffPart, f32 area)
{
    if (dds3EffectFuncTable[hEffPart->type].setArea != NULL) {
        dds3EffectFuncTable[hEffPart->type].setArea(hEffPart->pHandle, area);
    }
}

void *dds3EffectPartTMXBillboardCreate(u32 data)
{
    return dds3BillboardCreate(0, data);
}

void *dds3EffectPartP2ABillboardCreate(u32 data)
{
    return dds3BillboardCreate(1, data);
}

void dds3EffectPartBillboardScale(void *hBillboard, f32 scale)
{
    dds3BillboardSetScale(hBillboard, scale, scale);
}

void dds3EffectPartModelInit(mdlObj_t *hModel)
{
    SDF_VU0_LOADVECTOR(&effPartModelPosInit);
    mdlSetPosition(hModel);
    SDF_VU0_LOADVECTOR(&effPartModelRotInit);
    mdlSetRotate(hModel);
    SDF_VU0_LOADVECTOR(&effPartModelScaleInit);
    mdlSetScale(hModel);
    mdlSetColor(hModel, 0x80808080);
    if (hModel->first != NULL) {
        mdlLoopMotion(hModel, 0, 0);
        hModel->first->speed = 1.0f;
    }
    hModel->flags &= ~1;
}

mdlObj_t *dds3EffectPartModelCreate(dds3EffectPartModelData_t *pData)
{
    mdlObj_t *hModel;

    mdlPackDecode(7, effPartModelSerial, 0x101, pData->data, pData->size);
    hModel = mdlLoad(7, effPartModelSerial);
    dds3EffectPartModelInit(hModel);
    effPartModelSerial++;
    return hModel;
}

void dds3EffectPartModelDraw(mdlObj_t *hData)
{
    mdlLink(hData, D_002F69B8);
}

void dds3EffectPartModelDelete(mdlObj_t *hData)
{
    mdlFree(hData);
}

mdlObj_t *dds3EffectPartModelCopy(void *hData)
{
    s32 major;
    s32 minor;
    mdlObj_t *hModel;

    major = mdlGetMajorNum(hData);
    minor = mdlGetMinorNum(hData);
    hModel = mdlLoad(major, minor);
    dds3EffectPartModelInit(hModel);
    return hModel;
}

void dds3EffectPartModelSetPosition(mdlObj_t *hData, sdfVector4_t *pPos)
{
    SDF_VU0_LOADVECTOR(pPos);
    mdlSetPosition(hData);
}

void dds3EffectPartModelSetScale(void *hData, f32 scale)
{
    sdfVector4_t v;

    v.x = v.y = v.z = scale;
#ifdef NON_MATCHING
    v.w = 1.0f;
#endif
    SDF_VU0_LOADVECTOR_MEMOPS(v);
    mdlSetScale(hData);
}

void dds3EffectPartModelSetRotateMatrix(mdlObj_t *hData, sdfMatrix4_t *pMatrix)
{
    SDF_VU0_LOADMATRIX(pMatrix);
    SDF_VU0_STORE_ROW0(&hData->inner->tm.m[0]);
    SDF_VU0_STORE_ROW1(&hData->inner->tm.m[4]);
    SDF_VU0_STORE_ROW2(&hData->inner->tm.m[8]);
}

void dds3EffectPartModelSetColor(mdlObj_t *hData, u32 abgr)
{
    mdlSetColor(hData, abgr);
}

dds3EffectPCP_t *dds3EffectPCPCreate(dds3EffectPCPData_t *hPCPData)
{
    dds3EffectPCP_t *hPCP;

    hPCP = sdfChipAllocDirect(sizeof(dds3EffectPCP_t));
    hPCP->kateg = dds3EffectPCPDataGetKateg(hPCPData);
    hPCP->param = dds3EffectPCPDataGetParam(hPCPData);
    if (EffectPCPFuncTable[hPCP->kateg].copy == NULL) {
        hPCP->pHandle = EffectPCPFuncTable[hPCP->kateg].create(effPartPCPDataTable[hPCP->kateg][hPCP->param]);
    } else {
        hPCP->pHandle = EffectPCPFuncTable[hPCP->kateg].create(hPCPData);
    }
    return hPCP;
}

void dds3EffectPCPDelete(dds3EffectPCP_t *hData)
{
    EffectPCPFuncTable[hData->kateg].destroy(hData->pHandle);
    sdfChipFreeDirect(hData);
}

void dds3EffectPCPDraw(dds3EffectPCP_t *hData)
{
    EffectPCPFuncTable[hData->kateg].draw(hData->pHandle);
}

dds3EffectPCP_t *dds3EffectPCPCopy(dds3EffectPCP_t *hPCPSrc)
{
    dds3EffectPCP_t *hPCP;

    hPCP = sdfChipAllocDirect(sizeof(dds3EffectPCP_t));
    hPCP->kateg = hPCPSrc->kateg;
    hPCP->param = hPCPSrc->param;
    if (EffectPCPFuncTable[hPCP->kateg].copy == NULL) {
        hPCP->pHandle = EffectPCPFuncTable[hPCP->kateg].create(effPartPCPDataTable[hPCP->kateg][hPCP->param]);
    } else {
        hPCP->pHandle = EffectPCPFuncTable[hPCP->kateg].copy(hPCPSrc->pHandle);
    }
    return hPCP;
}

void dds3EffectPCPSetPosition(dds3EffectPCP_t *hData, void *pPos)
{
    if (EffectPCPFuncTable[hData->kateg].setPosition != NULL) {
        EffectPCPFuncTable[hData->kateg].setPosition(hData->pHandle, pPos);
    }
}

void dds3EffectPCPSetScale(dds3EffectPCP_t *hData, f32 scale)
{
    if (EffectPCPFuncTable[hData->kateg].setScale != NULL) {
        EffectPCPFuncTable[hData->kateg].setScale(hData->pHandle, scale);
    }
}

void dds3EffectPCPSetRotateMatrix(dds3EffectPCP_t *hData, void *pMatrix)
{
    if (EffectPCPFuncTable[hData->kateg].setRotateMatrix != NULL) {
        EffectPCPFuncTable[hData->kateg].setRotateMatrix(hData->pHandle, pMatrix);
    }
}

void dds3EffectPCPSetDirection(dds3EffectPCP_t *hData, void *pMatrix)
{
    if (EffectPCPFuncTable[hData->kateg].setDirection != NULL) {
        EffectPCPFuncTable[hData->kateg].setDirection(hData->pHandle, pMatrix);
    }
}

void dds3EffectPCPSetColor(dds3EffectPCP_t *hData, u32 abgr)
{
    if (EffectPCPFuncTable[hData->kateg].setColor != NULL) {
        EffectPCPFuncTable[hData->kateg].setColor(hData->pHandle, abgr);
    }
}

void dds3EffectPCPSetArea(dds3EffectPCP_t *hData, f32 area)
{
    if (EffectPCPFuncTable[hData->kateg].setArea != NULL) {
        EffectPCPFuncTable[hData->kateg].setArea(hData->pHandle, area);
    }
}

u32 dds3EffectPCPDataGetKateg(dds3EffectPCPData_t *hPCPData)
{
    return hPCPData->kateg;
}

u32 dds3EffectPCPDataGetParam(dds3EffectPCPData_t *hPCPData)
{
    return hPCPData->param;
}

void *dds3EffectPCPDataGetData(dds3EffectPCPData_t *hPCPData, u32 number)
{
    dds3EffectPCPData_Part_t *part = &hPCPData->pcpData[number];

    return (u8 *)hPCPData + part->offset;
}

u32 dds3EffectPCPDataGetSize(dds3EffectPCPData_t *hPCPData, u32 number)
{
    dds3EffectPCPData_Part_t *part = &hPCPData->pcpData[number];

    return part->size;
}

u32 dds3EffectPCPDataGetType(dds3EffectPCPData_t *hPCPData, u32 number)
{
    dds3EffectPCPData_Part_t *part = &hPCPData->pcpData[number];

    return part->type;
}

effPartHandle_t *dds3EffectPCPDataPartCreate(dds3EffectPCPData_t *hPCPData, u32 number)
{
    u16 type;
    void *pData;

    type = (u16)dds3EffectPCPDataGetType(hPCPData, number);
    pData = dds3EffectPCPDataGetData(hPCPData, number);
    return dds3EffectPartCreate(type, pData);
}

INCLUDE_SDATA(const s32, "effect/effPart", effPartModelSerial);

