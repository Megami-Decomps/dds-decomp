#include "common.h"
#include "libsdf.h"
#include "fpu.h"
#include "ps2asm.h"
#include "sdf_lighting.h"
#include "libsdf.h"
#include "sdf_resource.h"

#include "eeregs.h"

/* Register selectors carried by GIF A+D writes in the two GS contexts. */
enum {
    SDF_GS_PRIM = 0x00,
    SDF_GS_RGBAQ = 0x01,
    SDF_GS_XYZ2 = 0x05,
    SDF_GS_TEX0_1 = 0x06,
    SDF_GS_TEX0_2 = 0x07,
    SDF_GS_CLAMP_1 = 0x08,
    SDF_GS_CLAMP_2 = 0x09,
    SDF_GIF_REGISTER_AD = 0x0E,
    SDF_GS_TEX1_1 = 0x14,
    SDF_GS_TEX1_2 = 0x15,
    SDF_GS_XYOFFSET_1 = 0x18,
    SDF_GS_XYOFFSET_2 = 0x19,
    SDF_GS_PRMODECONT = 0x1A,
    SDF_GS_TEXA = 0x3B,
    SDF_GS_TEXFLUSH = 0x3F,
    SDF_GS_SCISSOR_1 = 0x40,
    SDF_GS_SCISSOR_2 = 0x41,
    SDF_GS_ALPHA_1 = 0x42,
    SDF_GS_ALPHA_2 = 0x43,
    SDF_GS_DTHE = 0x45,
    SDF_GS_COLCLAMP = 0x46,
    SDF_GS_TEST_1 = 0x47,
    SDF_GS_TEST_2 = 0x48,
    SDF_GS_FRAME_1 = 0x4C,
    SDF_GS_FRAME_2 = 0x4D,
    SDF_GS_ZBUF_1 = 0x4E,
    SDF_GS_ZBUF_2 = 0x4F,
    SDF_GS_BITBLTBUF = 0x50,
    SDF_GS_TRXPOS = 0x51,
    SDF_GS_TRXREG = 0x52,
    SDF_GS_TRXDIR = 0x53
};


struct sdfDrawTag_t;

struct sdf3DFogParam_t;


struct sdf3DViewportPacket_t;

struct DmaPacketHeader;

struct sdf3DFogPacket_t;

struct sdf3DScreenParam_t;

struct sdfTexPacket_t;

struct sdfGsMemSlice_t;

struct sdfTexHandle_t;

struct sdfMemHandle_t;

struct TMX_HEADER_t;


/* Serialized byte consumed by the GS texture-state packet builder. */
typedef u8 SdfTextureClampMode;


/* Shared fog/depth state consumed by sdf3DMakeFogPacket (0x14).
 * The viewer config serializes fogMax, fogNear, fogMin, fogFar, color;
 * timeline payloads store fogMin before fogMax instead. */
typedef struct sdf3DFogParam_t {
    f32 fogMax;   /* 0x00: minimum fog range. */
    f32 fogMin;   /* 0x04: maximum fog range. */
    f32 fogNear;  /* 0x08: near depth used in the projection coefficients. */
    f32 fogFar;   /* 0x0C: far depth used in the projection coefficients. */
    u32 color;    /* 0x10: packed GS FOGCOL value. */
} sdf3DFogParam_t;

/* Native 0x90-byte camera/viewport owner shared by both projection builders. */
/* Shared 0xD0 VIF packet: two stored matrices, three vector slots, and trailer. */
typedef struct sdf3DViewportPacket_t {
    u16 quadwords;        /* 0x00 */
    u8 unknown02[6];      /* 0x02 */
    u32 reservedWord;     /* 0x08 */
    u32 command;          /* 0x0C */
    u8 matrixA[0x40];     /* 0x10 */
    u8 matrixB[0x40];     /* 0x50 */
    u8 vecA[0x10];        /* 0x90 */
    u8 vecB[0x10];        /* 0xA0 */
    u8 vecC[0x10];        /* 0xB0 */
    u32 stmodCommand;     /* 0xC0 */
    u32 mscalCommand;     /* 0xC4 */
    u32 unknownC8;        /* 0xC8 */
    u32 unknownCC;        /* 0xCC */
} QWORD_ALIGNED sdf3DViewportPacket_t;

/* Shared 0x20-byte DMA/VIF reference header. Unrecovered bytes stay opaque. */
typedef struct DmaPacketHeader {
    u16 quadwords; /* 0x00 */
    u8 pad02[2]; /* 0x02 */
    u32 address; /* 0x04 */
    u32 tag; /* 0x08 */
    u32 command; /* 0x0C */
    u16 unknown10; /* 0x10 */
    u8 opaque12[6]; /* 0x12 */
    u32 unknown18; /* 0x18 */
    u32 unknown1C; /* 0x1C */
} QWORD_ALIGNED DmaPacketHeader;

/* Bind resource and asset release queues to their matching destruction callbacks. */
void sdfMatInitSystem(void);

/* Initialize the buffered object-list release queue with its object destructor. */
void sdfModelInitSystem(void);

extern addr_t sdf3DLightDefault;

extern addr_t sdf3DFogDefault;

extern f32 sdf3DEyeOffset[4];

/* Shared byte-storage view: the local screen structs are not compatible C types. */
extern u8 sdf3DScreenParam[];

#define SDF_DMA_QWORD_SHIFT 4

#define SDF_GS_FOGCOL_REGISTER 0x3D

#define SDF_VIF_DIRECT_COMMAND 0x50000000

#define SDF_VIF_FLUSHA_COMMAND 0x13000000

#define SDF_VIF_ITOP_MATRIX 0x04000002

#define SDF_VIF_MSCAL_COMMAND 0x14000000

extern u8 vu1MicroPacket[];

/* One past the last byte of the vu1MicroPacket VIF packet, so the packet's
 * length is the distance between the two. */
SYMBOL_AT_REGION(u8, vu1MicroPacketEnd, vu1MicroPacket, 0x3D20);

extern u8 sdf3DScreenTm[];

extern u8 sdfCameraTm[];

/* DMA/VIF prefix, left/right projection terms, then a one-register GIF write
 * and the VU execution command. */
typedef struct sdf3DFogPacket_t {
    u64 dmaTag;
    u64 vifUnpackCode;
    f32 right;
    f32 left;
    f32 mid;
    f32 scale;
    u64 gifTag;
    u64 gifRegister;
    u64 registerValue;
    u64 fogColorRegister;
    u32 mscalCommand;
    u32 reservedA;
    u32 reservedB;
    u32 reservedC;
} sdf3DFogPacket_t;

extern u64 sdf3DRefMapParam;

extern u8 sdf3DSlot2TmxData[];

extern u8 sdf3DSlot1TmxData[];

/* The camera, as sdf3DMakeScreenParam derives it: twelve inputs, then the
 * matrix and the clip-to-screen mapping it computes. cameraPos is the inverted
 * camera origin sdf3DMakeViewportPacket caches here. Same record as
 * draw3d.h's. */
typedef struct sdf3DScreenParam_t {
    u32 flags; /* sdf_3d_screen_flag */
    f32 aspect;
    f32 projDist;
    f32 fovy;
    f32 ox;
    f32 oy;
    f32 width;
    f32 height;
    f32 zMin;
    f32 zMax;
    f32 nearClip;
    f32 farClip;
    u8 TM[0x40];   /* 0x30 */
    f32 scale[4];  /* 0x70 */
    f32 trans[4];  /* 0x80 */
    f32 cameraPos[4]; /* 0x90 */
} sdf3DScreenParam_t;

void sdfDrawTagLinkRef(struct sdfDrawTag_t *aTag, addr_t aPacket);





/* Return packed 64-bit TEX0, TEX1, and CLAMP states from the primary GS packet. */
u64 sdfTexGetTex0Reg(sdfTexHandle_t *aHandle);


u32 sdfMemLock(struct sdfMemHandle_t *aHandle);

void sdfMemFreeDirect(struct sdfMemHandle_t *aHandle);


/* Acquire a texture from a serialized texture-file header. */
struct sdfTexHandle_t *sdfTexSet(const TMX_HEADER_t *aTex);

struct sdfTexHandle_t *sdfTexSetDirect(const TMX_HEADER_t *aTex);

/* Generated 128x64 grayscale power lookup texture: a serialized texture
 * header, 256 RGBA palette words, then the full 64x128 index image. */
/* The specular falloff table sdf3DInitSpecularTex2 generates and hands to
 * sdfTexSet: a TMX header, the 256-entry CLUT, and 64 rows of 128 exponents. */
typedef struct sdf3DSpecularTexImage_t {
    TMX_HEADER_t header; /* 0x0000 */
    u32 palette[256];            /* 0x0040 */
    u8 indices[64][128];         /* 0x0440 */
} sdf3DSpecularTexImage_t;

extern sdfTexHandle_t *sdf3DSlot2Tex;

extern sdfTexHandle_t *sdf3DSpecularTex;

extern sdfTexHandle_t *sdf3DSlot1Tex;

extern void sdfTmxSetHeader(TMX_HEADER_t *, s32, s32, s32, s32, s32);

extern s32 vu1Tex0Slot0[];

extern s32 vu1Tex0Slot2[];

extern s32 vu1Tex0Slot1[];

extern void *memcpy(void *, const void *, size_t);

void ReverseMatrix(void);

void MultMatrixRev(const void *aMatrix);

void sdf3DMakeViewportPacket(sdf3DViewportPacket_t *aPacket, sdf3DScreenParam_t *aParam, void *aCamera);

void sdf3DSetCamera(sdf3DScreenParam_t *aParam, void *aCamera);

void sdf3DMakeFogPacket(sdf3DFogPacket_t *aPacket, sdf3DFogParam_t *aParam);

void sdf3DMakeDirectRefPacket(DmaPacketHeader *aPacket, addr_t aData, s32 aLeng);

void sdf3DLinkInitPacket(sdfDrawTag_t *aTag, DmaPacketHeader *aPacket);

void sdf3DInitSpecularTex2(void);

void sdf3DInit(void);

void sdf3DMakeViewportPacket(sdf3DViewportPacket_t *aPacket, sdf3DScreenParam_t *aParam, void *aCamera) {
    aPacket->quadwords = 0xC;
    aPacket->command = 0x6C0BC000;
    aPacket->reservedWord = 0;
    SDF_VU0_LOADMATRIX(aCamera);
    SDF_VU0_STOREMATRIX(aPacket->matrixA);
    MultMatrixRev(aParam->TM);
    SDF_VU0_STOREMATRIX(aPacket->matrixB);
    SDF_VU0_LOADVECTOR(aParam->scale);
    SDF_VU0_STOREVECTOR(aPacket->vecA);
    SDF_VU0_LOADVECTOR(aParam->trans);
    SDF_VU0_STOREVECTOR(aPacket->vecB);
    SDF_VU0_LOADMATRIX(aCamera);
    ReverseMatrix();
    SDF_VU0_GET_ROW3();
    SDF_VU0_STOREVECTOR(aPacket->vecC);
    SDF_VU0_STOREVECTOR(aParam->cameraPos);
    aPacket->stmodCommand = SDF_VIF_ITOP_MATRIX;
    aPacket->mscalCommand = SDF_VIF_MSCAL_COMMAND;
    aPacket->unknownC8 = 0;
    aPacket->unknownCC = 0;
}

void sdf3DSetCamera(sdf3DScreenParam_t *aParam, void *aCamera) {
    SDF_VU0_LOADMATRIX(aCamera);
    SDF_VU0_STOREMATRIX(sdfCameraTm);
    MultMatrixRev(aParam->TM);
    SDF_VU0_LOADVECTOR(aParam->cameraPos);
    SDF_VU0_STOREMATRIX(sdf3DScreenTm);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_STOREVECTOR(sdf3DEyeOffset);
    memcpy(sdf3DScreenParam, aParam, sizeof(*aParam));
}

void sdf3DSetDefaultLight(u8 *aLight) {
    sdf3DLightDefault = EE_ADDR(aLight);
}

void sdf3DSetDefaultFog(u8 *aFog) {
    sdf3DFogDefault = EE_ADDR(aFog);
}

extern f32 tanf(f32 angle);
extern void MultMatrix(const void *matrix);

#define sdf3DScreenAdjust D_003BD360
#define sdf3DScreenAdjustScaleX D_003BD364
#define sdf3DScreenAdjustScaleY D_003BD368
#define sdf3DScreenAdjustOffsetX D_003BD36C
#define sdf3DScreenAdjustOffsetY D_003BD370
/* Nonzero scales and offsets every screen sdf3DMakeScreenParam builds. */
extern s8 sdf3DScreenAdjust;
extern f32 sdf3DScreenAdjustScaleX;
extern f32 sdf3DScreenAdjustScaleY;
extern f32 sdf3DScreenAdjustOffsetX;
extern f32 sdf3DScreenAdjustOffsetY;

/* vu0 routine: compose the perspective matrix and clip-to-screen mapping. */
void sdf3DMakeScreenParam(sdf3DScreenParam_t *camera)
{
    f32 projectionMatrix[16] __attribute__((aligned(16)));
    f32 farZ = camera->farClip;
    f32 nearZ = camera->nearClip;
    f32 depthRange = farZ - nearZ;
    f32 halfWidth = camera->width * 0.5f;
    f32 halfHeight = camera->height * 0.5f;
    f32 projectionScale;
    f32 centerY;

    SDF_MMI_STORE_UNIT_MATRIX(projectionMatrix);
    projectionMatrix[0] = 1.0f / (halfWidth * camera->aspect);
    projectionMatrix[5] = 1.0f / halfHeight;
    projectionMatrix[10] = farZ * nearZ * 2.0f / depthRange;
    projectionMatrix[14] = -(nearZ + farZ) / depthRange;
    SDF_VU0_LOADMATRIX(projectionMatrix);
    SDF_MMI_STORE_UNIT_MATRIX(projectionMatrix);
    if (camera->flags & 2) {
        projectionScale = camera->height / (tanf(camera->fovy * 0.5f) * 2.0f);
    } else {
        projectionScale = camera->projDist;
    }
    projectionMatrix[5] = projectionMatrix[0] = projectionScale;
    projectionMatrix[10] = 0;
    projectionMatrix[15] = 0;
    projectionMatrix[14] = projectionMatrix[11] = 1.0f;
    MultMatrix(projectionMatrix);
    SDF_VU0_STOREMATRIX(camera->TM);
    camera->scale[0] = halfWidth;
    centerY = (camera->zMax - camera->zMin) * 0.5f;
    if (camera->flags & 1) {
        camera->scale[1] = halfHeight * 0.5f;
    } else {
        camera->scale[1] = halfHeight;
    }
    camera->scale[2] = centerY;
    camera->scale[3] = 1.0f;
    camera->trans[0] = camera->ox;
    camera->trans[1] = camera->oy;
    camera->trans[2] = centerY + camera->zMin;
    camera->trans[3] = 0;
    if (sdf3DScreenAdjust != 0) {
        camera->scale[0] *= sdf3DScreenAdjustScaleX;
        camera->scale[1] *= sdf3DScreenAdjustScaleY;
        camera->trans[0] += sdf3DScreenAdjustOffsetX;
        camera->trans[1] += sdf3DScreenAdjustOffsetY;
    }
}


extern f32 sdf3DUpVector[];
extern void DiagMatrix(void);
extern sdfMemHandle_t *sdfMemAllocDirect(s32 size);
extern void *memset(void *dst, s32 value, size_t size);

/* vu0 routine: parallel-light packet and lighting basis construction */
void sdf3DMakeLightParallelPacket(sdf3DLightParallelPacket_t *aPacket, sdf3DLight_t **aLights, sdfVector4_t *aAmbient) {
    f32 direction[4] __attribute__((aligned(16)));
    /* VU0 lqc2 reads this scratch through its address, like the DDS rotation
     * loaders and Euler-to-quaternion routines; volatile preserves its stores. */
    volatile f32 axis[4] __attribute__((aligned(16)));
    sdf3DLight_t *first;
    s32 i;

    aPacket->dmatag.qwc = 13;
    aPacket->dmatag.dat0 = 0x10000000;
    aPacket->dmatag.dat1 = 0x6C0D4398;
    SDF_VU0_LOADVECTOR_B(aAmbient);
    SDF_VU0_STOREVECTOR_B(&aPacket->color[3]);
    first = NULL;
    for (i = 0; i < 3; i++) {
        sdf3DLight_t *light = aLights[i];

        if (light == NULL) {
            aPacket->normalComponents[0][i] = 0.0f;
            aPacket->normalComponents[1][i] = 0.0f;
            aPacket->normalComponents[2][i] = 0.0f;
            aPacket->color[i].x = 0.0f;
            aPacket->color[i].y = 0.0f;
            aPacket->color[i].z = 0.0f;
            aPacket->color[i].w = 0.0f;
        } else {
            if (first == NULL) {
                first = light;
            }
            SDF_VU0_LOADVECTOR(&light->v);
            SDF_VU0_NORMALIZE();
            SDF_VU0_SCALEVECTOR_BITS(-1.0f);
            SDF_VU0_STOREVECTOR(direction);
            aPacket->normalComponents[0][i] = direction[0];
            aPacket->normalComponents[1][i] = direction[1];
            aPacket->normalComponents[2][i] = direction[2];
            SDF_VU0_LOADVECTOR(&light->color);
            SDF_VU0_ADDVECTOR_B_A();
            SDF_VU0_STOREVECTOR(&aPacket->color[i]);
        }
    }
    SDF_VU0_COPYVECTOR_F32(&aPacket->colorBase, &aPacket->color[0]);
    SDF_VU0_STOREVECTOR_B(&aPacket->color[4]);
    if (first == NULL) {
        SDF_MMI_STORE_UNIT_MATRIX(&aPacket->TM);
        return;
    }
    SDF_VU0_LOADVECTOR(&first->v);
    SDF_VU0_NORMALIZE();
    SDF_VU0_VECTOR_W_SET_ZERO();
    SDF_VU0_SET_ROW2();
    SDF_VU0_STOREVECTOR_F32(axis);
    if (ffabsf(axis[0]) < 0.000001f && ffabsf(axis[2]) < 0.000001f) {
        axis[0] = -axis[1];
        axis[1] = 0.0f;
        axis[2] = 0.0f;
        axis[3] = 0.0f;
        SDF_VU0_LOADVECTOR_B_F32(axis);
    } else {
        SDF_VU0_LOADVECTOR_B(sdf3DUpVector);
    }
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_VECTOR_W_SET_ZERO();
    SDF_VU0_SET_ROW0();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_GET_ROW2();
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_VECTOR_W_SET_ZERO();
    SDF_VU0_SET_ROW1();
    SDF_VU0_RESET_ROW3();
    DiagMatrix();
    SDF_VU0_LOADVECTOR(&first->v);
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B_MTXCOL(sdfCameraTm + 8);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_NEGVECTOR_XYZ();
    SDF_VU0_NORMALIZE();
    SDF_VU0_SET_ROW3();
    SDF_VU0_STOREMATRIX(&aPacket->TM);
}

void sdf3DMakeFogPacket(sdf3DFogPacket_t *aPacket, sdf3DFogParam_t *aParam) {
    f32 rangeMax = aParam->fogMin;
    f32 rangeMin = aParam->fogMax;
    f32 nearZ = aParam->fogNear;
    f32 farZ = aParam->fogFar;
    f32 depthRange = farZ - nearZ;
    s32 fogColor = aParam->color;
    aPacket->dmaTag = 0x20000004;
    aPacket->vifUnpackCode = 0x6C03C00013000000ULL;
    aPacket->gifTag = 0x1000000000008001ULL;
    aPacket->gifRegister = SDF_GIF_REGISTER_AD;
    aPacket->registerValue = (u32)fogColor;
    aPacket->fogColorRegister = SDF_GS_FOGCOL_REGISTER;
    aPacket->mscalCommand = 0x14000014;
    aPacket->reservedC = 0;
    aPacket->right = rangeMax;
    aPacket->left = rangeMin;
    aPacket->reservedA = 0;
    aPacket->reservedB = 0;
    aPacket->mid = (((rangeMax - rangeMin) * (farZ + nearZ)) / depthRange + (rangeMax + rangeMin)) * 0.5f;
    aPacket->scale = ((farZ * nearZ) * (rangeMin - rangeMax)) / depthRange;
}

void sdf3DMakeDirectRefPacket(DmaPacketHeader *aPacket, addr_t aData, s32 aLeng) {
    s32 quadwordCount = (aLeng + 15) >> SDF_DMA_QWORD_SHIFT;
    aPacket->quadwords = quadwordCount;
    aPacket->address = EE_PHYSICAL(aData);
    aPacket->tag = SDF_VIF_FLUSHA_COMMAND;
    aPacket->command = quadwordCount | SDF_VIF_DIRECT_COMMAND;
    aPacket->unknown10 = 0;
    aPacket->unknown18 = 0;
    aPacket->unknown1C = 0;
}

void sdf3DLinkInitPacket(sdfDrawTag_t *aTag, DmaPacketHeader *aPacket) {
    aPacket->address = EE_PHYSICAL(EE_ADDR(vu1MicroPacket));
    aPacket->quadwords = (vu1MicroPacketEnd - vu1MicroPacket) >> SDF_DMA_QWORD_SHIFT;
    aPacket->tag = 0;
    aPacket->command = 0;
    aPacket->unknown10 = 0;
    aPacket->unknown18 = 0;
    aPacket->unknown1C = 0;
    sdfDrawTagLinkRef(aTag, EE_ADDR(aPacket));
}

extern sdfMemHandle_t *sdfMemAllocDirect(s32 size);
extern void *memset(void *dst, s32 value, size_t size);

void sdf3DInitSpecularTex2(void) {
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect(0x2440);
    sdf3DSpecularTexImage_t *texture = (sdf3DSpecularTexImage_t *)EE_ADDR_PTR(sdfMemLock(allocation));
    s32 i;
    s32 j;

    sdfTmxSetHeader(&texture->header, 0x80, 0x40, 0x13, 1, 0);
    texture->header.fileSize = 0x2440;
    texture->header.texWrapMode = 5;
    /* The physical CLUT slot j decodes to the logical intensity i. */
    for (j = 0; j != 0x100; j++) {
        i = ((j & 8) << 1) | ((j & 0x10) >> 1) | (j & ~0x18);
        texture->palette[j] = i | (i << 16) | (i << 8) | 0x80000000;
    }
    memset(&texture->indices[0][1], 0xFF, 0x7F);
    texture->indices[0][0] = 0;
    for (i = 0; i != 0x80; i++) {
        f32 power = i * (1.0f / 127.0f);
        f32 value = power;
        for (j = 1; j != 0x40; j++) {
            texture->indices[j][i] = (u8)(s32)(value * 255.0f + 0.5f);
            value *= power;
        }
    }
    sdf3DSpecularTex = sdfTexSet(&texture->header);
    sdfMemFreeDirect(allocation);
}

void sdf3DInit(void) {
    s64 textureWord;
    sdf3DInitSpecularTex2();
    textureWord = sdfTexGetTex0Reg(sdf3DSpecularTex);
    sdf3DRefMapParam = textureWord;
    vu1Tex0Slot0[0] = textureWord;
    vu1Tex0Slot0[1] = textureWord >> 32;
    sdf3DSlot2Tex = sdfTexSetDirect((TMX_HEADER_t *)sdf3DSlot2TmxData);
    textureWord = sdfTexGetTex0Reg(sdf3DSlot2Tex);
    vu1Tex0Slot2[0] = textureWord;
    vu1Tex0Slot2[1] = textureWord >> 32;
    sdf3DSlot1Tex = sdfTexSetDirect((TMX_HEADER_t *)sdf3DSlot1TmxData);
    textureWord = sdfTexGetTex0Reg(sdf3DSlot1Tex);
    vu1Tex0Slot1[0] = textureWord;
    vu1Tex0Slot1[1] = textureWord >> 32;
    sdfModelInitSystem();
    sdfMatInitSystem();
}

INCLUDE_SDATA(const s32, "sdf/sdf3D", D_003BD360);

INCLUDE_SDATA(const s32, "sdf/sdf3D", D_003BD364);

INCLUDE_SDATA(const s32, "sdf/sdf3D", D_003BD368);

INCLUDE_SDATA(const s32, "sdf/sdf3D", D_003BD36C);

INCLUDE_SDATA(const s32, "sdf/sdf3D", D_003BD370);

INCLUDE_SDATA(const s32, "sdf/sdf3D", sdf3DLightDefault);

INCLUDE_SDATA(const s32, "sdf/sdf3D", sdf3DFogDefault);

INCLUDE_SDATA(const s32, "sdf/sdf3D", sdf3DSlot2Tex);

INCLUDE_SDATA(const s32, "sdf/sdf3D", sdf3DSpecularTex);

INCLUDE_SDATA(const s32, "sdf/sdf3D", sdf3DRefMapParam);

INCLUDE_SDATA(const s32, "sdf/sdf3D", sdf3DSlot1Tex);

INCLUDE_SDATA(const s32, "sdf/sdf3D", sdfPadPortSlotPairs);

