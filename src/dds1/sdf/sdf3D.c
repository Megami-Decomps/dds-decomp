#include "sdk_ps2.h"
#include "sdf_gs_header.h"
#include "eeregs.h"
#include "common.h"
#include "sdf_vu0.h"
#include "sdf_vu_matrix.h"
#include "sdf_pad_entry.h"
#include "sdf_pad_index.h"
#include "sdf_pad_mode_api.h"
#include "sdf_pad_motor_api.h"
#include "sdf_cons_matrix_packet.h"
#include "sdf_dma_packet_header.h"
#include "kwln.h"
#include "sdf_asset_packets.h"
#include "sdf_asset_state.h"
#include "sdf_vu_lighting.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "sdf_packet_header.h"
#include "sdf_packet_append.h"
#include "sdf_resource.h"
#include "sdf_primitive_packet_api.h"
#include "sdf.h"
#include "sdf_draw.h"
#include "sdf_request.h"
#include "sdf_model.h"
#include "sdf_projection.h"
#include "pcp_vu0.h"
#include "sdf_vu_transform_api.h"
#include "ee_mmi.h"
#include "fpu.h"
#include "sdf_texture_file.h"
#include "sdf_column_packet_api.h"
#include "sdf_lookup_texture.h"
#include "sdf_compact_vertex_vif_packet.h"
#include "sdf_dev_console_node.h"
#include "btl.h"
#include "sdf_packet_alloc.h"

extern void DiagMatrix(void);

#define SDF_DMA_QWORD_SHIFT 4

#define SDF_DMA_ADDRESS_MASK 0x0FFFFFFF

#define SDF_GIF_REGISTER_AD 0xE

#define SDF_GS_FOGCOL_REGISTER 0x3D

#define SDF_VIF_DIRECT_COMMAND 0x50000000

#define SDF_VIF_FLUSHA_COMMAND 0x13000000

#define SDF_VIF_ITOP_MATRIX 0x04000002

#define SDF_VIF_MSCAL_COMMAND 0x14000000

#define SDF_CAMERA_USE_FOV_FLAG 2

#define SDF_CAMERA_HALF_HEIGHT_FLAG 1

extern u32 sdf3DFogDefault;

extern const struct SdfDmaTagHeader *sdf3DLightDefault;

extern sdfTexHandle_t *D_003BD37C;

extern sdfTexHandle_t *D_003BD380;

extern sdfTexHandle_t *D_003BD390;

/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */ void MultMatrix(const f32 *matrix);

extern u8 D_00320630[];

/* Emit two matrices and three vectors; also cache the inverse origin in node. */
void sdf3DMakeViewportPacket(ConsMatrixPacket *packet, SdfProjectionRecord *node, const Matrix4 *transformMatrix) {
    packet->quadwords = 0xC;
    packet->command = 0x6C0BC000;
    packet->reservedWord = 0;
    VU0_LOAD_MATRIX(transformMatrix);
    VU0_STORE_MATRIX(packet->matrixA);
    sdfPostmultiplyVuMatrixFromMemory((const struct Matrix4 *)node->camera.matrix);
    VU0_STORE_MATRIX(packet->matrixB);
    SDF_VU0_LOADVECTOR_BARRIER(&node->camera.halfWidth);
    SDF_VU0_STOREVECTOR(packet->vecA);
    SDF_VU0_LOADVECTOR_BARRIER(&node->camera.originX);
    SDF_VU0_STOREVECTOR(packet->vecB);
    VU0_LOAD_MATRIX(transformMatrix);
    ReverseMatrix();
    SDF_VU0_GET_ROW3();
    SDF_VU0_STOREVECTOR(packet->vecC);
    SDF_VU0_STOREVECTOR(node->inverseOrigin);
    packet->stmodCommand = SDF_VIF_ITOP_MATRIX;
    packet->mscalCommand = SDF_VIF_MSCAL_COMMAND;
    packet->unknownC8 = 0;
    packet->unknownCC = 0;
}

extern u8 sdf3DScreenTm[];

extern u8 sdfCameraTm[];

extern u8 sdf3DEyeOffset[];

/* Cache node data, its composed transform, and the transformed cached origin. */
void sdf3DSetCamera(const SdfProjectionRecord *node, const Matrix4 *transformMatrix) {
    VU0_LOAD_MATRIX(transformMatrix);
    VU0_STORE_MATRIX(sdfCameraTm);
    sdfPostmultiplyVuMatrixFromMemory((const struct Matrix4 *)node->camera.matrix);
    SDF_VU0_LOADVECTOR_BARRIER(node->inverseOrigin);
    VU0_STORE_MATRIX(sdf3DScreenTm);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_STOREVECTOR(sdf3DEyeOffset);
    sdf3DScreenParam = *node;
}

/* Set the first DMA reference fallback used when a model has no override. */
void sdf3DSetDefaultLight(const struct SdfDmaTagHeader *lightingPacket) {
    sdf3DLightDefault = lightingPacket;
}

/* Set the second DMA reference fallback used when a model has no override. */
void sdf3DSetDefaultFog(u32 referenceSource) {
    sdf3DFogDefault = referenceSource;
}

extern s8 D_003BD360;

extern f32 D_003BD364;

extern f32 D_003BD368;

extern f32 D_003BD36C;

extern f32 D_003BD370;

/* Compose camera projection and screen parameters without reassociating floats. */
void sdfCameraBuildProjection(SdfCamera *camera) {
    f32 projectionMatrix[16];
    f32 farZ = camera->farZ;
    f32 nearZ = camera->nearZ;
    f32 depthRange = farZ - nearZ;
    f32 halfWidth = camera->width * 0.5f;
    f32 halfHeight = camera->height * 0.5f;
    f32 projectionScale;
    f32 centerY;

    EE_MMI_UNIT_MATRIX(projectionMatrix);
    projectionMatrix[0] = 1.0f / (halfWidth * camera->aspect);
    projectionMatrix[5] = 1.0f / halfHeight;
    projectionMatrix[10] = farZ * nearZ * 2.0f / depthRange;
    projectionMatrix[14] = -(nearZ + farZ) / depthRange;
    VU0_LOAD_MATRIX(projectionMatrix);
    EE_MMI_UNIT_MATRIX(projectionMatrix);
    if (camera->flags & SDF_CAMERA_USE_FOV_FLAG) {
        projectionScale = camera->height / (tanf(camera->fov * 0.5f) * 2.0f);
    } else {
        projectionScale = camera->scale;
    }
    projectionMatrix[5] = projectionMatrix[0] = projectionScale;
    projectionMatrix[10] = 0;
    projectionMatrix[15] = 0;
    projectionMatrix[14] = projectionMatrix[11] = 1.0f;
    MultMatrix(projectionMatrix);
    VU0_STORE_MATRIX(camera->matrix);
    camera->halfWidth = halfWidth;
    centerY = (camera->bottom - camera->top) * 0.5f;
    if (camera->flags & SDF_CAMERA_HALF_HEIGHT_FLAG) {
        camera->halfHeight = halfHeight * 0.5f;
    } else {
        camera->halfHeight = halfHeight;
    }
    camera->centerY = centerY;
    camera->one = 1.0f;
    camera->originX = camera->offsetX;
    camera->originY = camera->offsetY;
    camera->bottomY = centerY + camera->top;
    camera->zero = 0;
    if (D_003BD360 != 0) {
        camera->halfWidth *= D_003BD364;
        camera->halfHeight *= D_003BD368;
        camera->originX += D_003BD36C;
        camera->originY += D_003BD370;
    }
}

extern u8 D_00398390[]; /* fallback up vector */

/* Pack up to three directional lights and the ambient color for VU1: light
 * directions as columns (negated, unit length), colors and their sum, and a
 * basis built from the first light's direction. The axis scratch is volatile:
 * retail keeps its stores in source order. */
void sdf3DMakeLightParallelPacket(SdfLightingPacketStorage *packet,
                                  SdfLightSources sources,
                                  const f32 *ambient) {
    f32 direction[4] __attribute__((aligned(16)));
    volatile f32 axis[4] __attribute__((aligned(16)));
    const f32 (*first)[4];
    s32 i;

    packet->quadwordCount = 13;
    packet->flushCode = 0x10000000;
    packet->unpackCode = 0x6C0D4398;
    SDF_VU0_LOADVECTOR_B(ambient);
    SDF_VU0_STOREVECTOR_B(packet->ambient);
    first = NULL;
    for (i = 0; i < 3; i++) {
        const f32 (*light)[4] = sources[i];

        if (light == NULL) {
            packet->normal[0][i] = 0.0f;
            packet->normal[1][i] = 0.0f;
            packet->normal[2][i] = 0.0f;
            packet->color[i][0] = 0.0f;
            packet->color[i][1] = 0.0f;
            packet->color[i][2] = 0.0f;
            packet->color[i][3] = 0.0f;
        } else {
            if (first == NULL) {
                first = light;
            }
            SDF_VU0_LOADVECTOR(light[1]);
            SDF_VU0_NORMALIZE();
            VU0_SCALE_VF(vf10, -1.0f);
            SDF_VU0_STOREVECTOR(direction);
            packet->normal[0][i] = direction[0];
            packet->normal[1][i] = direction[1];
            packet->normal[2][i] = direction[2];
            SDF_VU0_LOADVECTOR(light[0]);
            VU0_ADD(vf11, vf11, vf10);
            SDF_VU0_STOREVECTOR(packet->color[i]);
        }
    }
    SDF_VU0_COPYVECTOR(packet->normal[3], packet->color[0]);
    SDF_VU0_STOREVECTOR_B(packet->combinedColor);
    if (first == NULL) {
        EE_MMI_UNIT_MATRIX(packet->basis);
        return;
    }
    SDF_VU0_LOADVECTOR(first[1]);
    SDF_VU0_NORMALIZE();
    SDF_VU0_VECTOR_W_SET_ZERO();
    SDF_VU0_SET_ROW2();
    SDF_VU0_STOREVECTOR(axis);
    if (ffabsf(axis[0]) < 0.000001f && ffabsf(axis[2]) < 0.000001f) {
        axis[0] = -axis[1];
        axis[1] = 0.0f;
        axis[2] = 0.0f;
        axis[3] = 0.0f;
        SDF_VU0_LOADVECTOR_B(axis);
    } else {
        SDF_VU0_LOADVECTOR_B(D_00398390);
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
    SDF_VU0_LOADVECTOR(first[1]);
    SDF_VU0_NORMALIZE();
    EE_MMI_LOAD_MATRIX_COLUMN(vf11, sdfCameraTm + 8);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_NEGVECTOR_XYZ();
    SDF_VU0_NORMALIZE();
    SDF_VU0_SET_ROW3();
    VU0_STORE_MATRIX(packet->basis);
}

/* DMA/VIF prefix, left/right projection terms, then a one-register GIF write
 * and the VU execution command. */
typedef struct ConsFrustumPacket {
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
} ConsFrustumPacket;

/* Emit projection depth coefficients and a GS FOGCOL A+D write. */
void sdf3DMakeFogPacket(ConsFrustumPacket *packet, KwlnDrawVectorParams *projectionParams) {
    f32 rangeMax = projectionParams->fogFar;
    f32 rangeMin = projectionParams->fogNear;
    f32 nearZ = projectionParams->depthNear;
    f32 farZ = projectionParams->depthFar;
    f32 depthRange = farZ - nearZ;
    s32 fogColor = projectionParams->fogColor;
    packet->dmaTag = 0x20000004;
    packet->vifUnpackCode = 0x6C03C00013000000ULL;
    packet->gifTag = 0x1000000000008001ULL;
    packet->gifRegister = SDF_GIF_REGISTER_AD;
    packet->registerValue = (u32)fogColor;
    packet->fogColorRegister = SDF_GS_FOGCOL_REGISTER;
    packet->mscalCommand = 0x14000014;
    packet->reservedC = 0;
    packet->right = rangeMax;
    packet->left = rangeMin;
    packet->reservedA = 0;
    packet->reservedB = 0;
    packet->mid = (((rangeMax - rangeMin) * (farZ + nearZ)) / depthRange + (rangeMax + rangeMin)) * 0.5f;
    packet->scale = ((farZ * nearZ) * (rangeMin - rangeMax)) / depthRange;
}

/* Round payloadBytes up to quadwords and pair FLUSHA with VIF DIRECT. */
void sdf3DMakeDirectRefPacket(DmaPacketHeader *packet, u32 sourceAddress, s32 payloadBytes) {
    s32 quadwordCount = (payloadBytes + 15) >> SDF_DMA_QWORD_SHIFT;
    packet->quadwords = quadwordCount;
    packet->address = sourceAddress & SDF_DMA_ADDRESS_MASK;
    packet->tag = SDF_VIF_FLUSHA_COMMAND;
    packet->command = quadwordCount | SDF_VIF_DIRECT_COMMAND;
    packet->unknown10 = 0;
    packet->unknown18 = 0;
    packet->unknown1C = 0;
}

extern u8 D_00324350[];

/* Append the second fixed program block as a DMA reference packet. */
void sdf3DLinkInitPacket(sdfDrawTag_t *packetList, DmaPacketHeader *packet) {
    packet->address = (u32)D_00320630 & SDF_DMA_ADDRESS_MASK;
    packet->quadwords = (D_00324350 - D_00320630) >> SDF_DMA_QWORD_SHIFT;
    packet->tag = 0;
    packet->command = 0;
    packet->unknown10 = 0;
    packet->unknown18 = 0;
    packet->unknown1C = 0;
    sdfDrawTagLinkRef(packetList, (u32)packet);
}

extern void sdfTmxSetHeader(SdfTextureFileHeader *, s32, s32, s32, s32, s32);

/* Acquire a 128x64 grayscale power lookup texture; release its temporary image. */
void sdf3DInitSpecularTex2(void) {
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect(0x2440);
    SdfGeneratedLookupTexture *texture = sdfMemLock(allocation);
    s32 i;
    s32 j;

    sdfTmxSetHeader(&texture->header, 0x80, 0x40, 0x13, 1, 0);
    texture->header.byteCount = 0x2440;
    texture->header.clampMode = 5;
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
    D_003BD380 = sdfTexSet(&texture->header);
    sdfMemFreeDirect(allocation);
}

extern u64 sdf3DRefMapParam;

extern s32 D_003241D8[];

extern s32 D_00324290[];

extern s32 D_00324214[];

extern u8 D_00317C20[];

extern u8 D_0031BC60[];

extern void sdf3DInitSpecularTex2(void);

/* Acquire texture resources, split their TEX0 words, and initialize queues. */
void sdf3DInit(void) {
    s64 textureWord;
    sdf3DInitSpecularTex2();
    textureWord = sdfTexGetTex0Reg(D_003BD380);
    sdf3DRefMapParam = textureWord;
    D_003241D8[0] = textureWord;
    D_003241D8[1] = textureWord >> 32;
    D_003BD37C = sdfTexSetDirect((SdfTextureFileHeader *)D_00317C20);
    textureWord = sdfTexGetTex0Reg(D_003BD37C);
    D_00324290[0] = textureWord;
    D_00324290[1] = textureWord >> 32;
    D_003BD390 = sdfTexSetDirect((SdfTextureFileHeader *)D_0031BC60);
    textureWord = sdfTexGetTex0Reg(D_003BD390);
    D_00324214[0] = textureWord;
    D_00324214[1] = textureWord >> 32;
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

INCLUDE_SDATA(const s32, "sdf/sdf3D", D_003BD37C);

INCLUDE_SDATA(const s32, "sdf/sdf3D", D_003BD380);

INCLUDE_SDATA(const s32, "sdf/sdf3D", sdf3DRefMapParam);

INCLUDE_SDATA(const s32, "sdf/sdf3D", D_003BD390);

INCLUDE_SDATA(const s32, "sdf/sdf3D", sdfPadPortSlotPairs);

