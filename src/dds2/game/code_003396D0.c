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
extern void DiagMatrix(void);

#define SDF_PAD_REPLY_BUFFER_BYTES 0x20

#define SDF_PAD_ACTUATOR_BYTES 6

#define SDF_PAD_REPLY_DIGITAL 0x41

#define SDF_PAD_REPLY_ANALOG 0x73

#define SDF_PAD_REPLY_PRESSURE 0x79

#define SDF_PAD_STICK_COUNT 4

#define SDF_PAD_PRESSURE_COUNT 12

#define SDF_PAD_STICK_CENTER 0x80

#define SDF_PAD_BUTTON_COUNT 16

#define SDF_PAD_BUTTON_TRIGGER_BIT 2

#define SDF_PAD_BUTTON_NEW_PRESS_BIT 0x80

#define SDF_PAD_REPEAT_DELAY 15

#define SDF_PAD_REPEAT_STEP 4

#define SDF_PAD_MOTOR_VALUE_MASK 0xFF

#define SDF_PAD_PORT_BUFFER_BYTES 0x100

#define SDF_PAD_BUTTON_STATE_BYTES 0x20

#define SDF_PAD_STICK_STATE_BYTES 8

#define SDF_PAD_PRESSURE_STATE_BYTES 0x18

#define SDF_CONSOLE_CELL_BYTES 2

#define SDF_CONSOLE_NODE_BYTES 0x20

#define SDF_DMA_QWORD_SHIFT 4

#define SDF_DMA_CHCR_TTE 0x40

#define SDF_DMA_ADDRESS_MASK 0x0FFFFFFF

#define SDF_QWORD_ALIGNMENT_MASK 0xF

#define SDF_TEXTURE_DRAW_PACKET_BYTES 0x50

#define SDF_GIF_REGISTER_AD 0xE

#define SDF_GS_FOGCOL_REGISTER 0x3D

#define SDF_GIF_NREG_SHIFT 60

#define SDF_GIF_PRIM_SHIFT 47

#define SDF_GIF_PRE_EOP_FLAGS 0x400000008000LL

#define SDF_VIF_DIRECT_COMMAND 0x50000000

#define SDF_VIF_FLUSHA_COMMAND 0x13000000

#define SDF_VIF_ITOP_MATRIX 0x04000002

#define SDF_VIF_ITOP_COMPACT_VERTEX 0x04000002

#define SDF_VIF_ITOP_WIDE_VERTEX 0x04000008

#define SDF_VIF_ITOP_LIGHTING 0x04000010

#define SDF_VIF_MSCAL_COMMAND 0x14000000

#define SDF_VIF_MSCAL_VERTICES 0x14000008

#define SDF_CAMERA_USE_FOV_FLAG 2

#define SDF_CAMERA_HALF_HEIGHT_FLAG 1

extern const struct SdfDmaTagHeader *corePktCurrLight;

extern u32 D_0043918C;

extern const struct SdfDmaTagHeader *sdf3DLightDefault;

extern u32 sdf3DFogDefault;

extern struct sdfTexHandle_t *sdfDebugFontHandle;

extern f32 D_00439190;

extern SdfPadEntry sdfPadCb[];

extern void sdfPadControl(SdfPadEntry *);

extern u32 D_00438AB4;

extern u8 D_00370B80[];

extern u32 sdfDevConsFontUvTable[];

extern sdfDevConsCB_tag *D_00438AB0;


extern u8 D_0037B080[];

extern u8 D_0037B610[];

extern sdfMemHandle_t *D_00438A40;

extern s32 D_00439180;




#include "sdf_packet_alloc.h"

extern u8 D_0040B730[];

extern void sdf3DInitSpecularTex2(void);

extern u8 D_00372C00[];

extern u8 D_00376C40[];

extern s32 D_0037F1B8[];

extern s32 D_0037F270[];

extern s32 D_0037F1F4[];

extern sdfTexHandle_t *D_00438A6C;

extern sdfTexHandle_t *D_00438A70;

extern u64 sdf3DRefMapParam;

extern sdfTexHandle_t *D_00438A80;

extern vu8 sdfFrameId;


/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */ void MultMatrix(const f32 *matrix);

INCLUDE_ASM(const s32, "game/code_003396D0", sdfCorePacketGen);

void sdfCoreDrawScanOneSkinNodes(sdfModelRootNode_t *model, SdfObjectRefCommand *source) {
    sdfModelNode_t **objects = model->list->buffer;
    u16 *indices = &source->count;
    if ((source->flags & 0x800) != 0) {
        s32 count = *indices;
        if (count != 0) {
            indices++;
            do {
                sdfModelAllocTmPacket(objects[*indices++]);
            } while (--count != 0);
        }
    }
}

void sdfCoreDrawSetLight(const struct SdfDmaTagHeader *lightingPacket) {
    corePktCurrLight = lightingPacket;
}

extern u8 D_00476240[];

/* Cache the object's vector only when its address changes, not its contents. */
void sdfCoreDrawSetFog(const SdfLightingPacketStorage *sourcePacket) {
    if (D_0043918C != (u32)sourcePacket) {
        D_0043918C = (u32)sourcePacket;
        SDF_VU0_COPYVECTOR(D_00476240, sourcePacket->normal[0]);
    }
}

void sdfCoreDrawSetZBias(f32 scale) {
    D_00439190 = scale;
}

void sdfVuClearTransformCache(void) {
    corePktCurrLight = 0;
    D_0043918C = 0;
}

/* Upload the VIF0 program synchronously, then replace the ring workspace. */
void sdfCoreDrawInit(s32 workspaceBytes) {
    u32 *dmaChannel = sceDmaGetChan(0);
    HW_WRITE(dmaChannel, HW_READ(dmaChannel) & ~SDF_DMA_CHCR_TTE);
    sceDmaSendN(dmaChannel, D_0037B080, (D_0037B610 - D_0037B080) >> SDF_DMA_QWORD_SHIFT);
    sceDmaSync(dmaChannel, 0, 0);
    sdfMemFreeHandleDirect(&D_00438A40);
    D_00438A40 = sdfMemAllocDirect(workspaceBytes);
    D_00439180 = sdfMemLock(D_00438A40);
}

/* Fixed allocation size; the texture is deliberately unused. */
u32 sdf2DGetTexPacketSize(sdfTexHandle_t *texture) {
    return SDF_TEXTURE_DRAW_PACKET_BYTES;
}

/* Fill TEX1, TEX0 and CLAMP A+D writes for the selected GS context. */
SdfTextureDrawPacket *sdf2DMakeTexPacket(SdfTextureDrawPacket *drawPacket, sdfTexHandle_t *texture, s32 context) {
    drawPacket->header.quadwords = 4;
    drawPacket->header.gifTag = 0x1000000000008003ULL;
    drawPacket->header.command = 0x50000004;
    drawPacket->header.pad08 = 0;
    drawPacket->header.payloadHeader = SDF_GIF_REGISTER_AD;
    drawPacket->textureWordA = sdfTexGetTex1Reg(texture);
    drawPacket->registerAddressA = context + 0x14;
    drawPacket->textureWordB = sdfTexGetTex0Reg(texture);
    drawPacket->registerAddressB = context + 6;
    drawPacket->textureWordC = sdfTexGetClampReg(texture);
    drawPacket->registerAddressC = context + 8;
    return drawPacket;
}

/* Allocate and append texture state; return its packet address. */
s32 sdf2DLinkTexPacket(sdfDrawTag_t *packetList, sdfTexHandle_t *texture, s32 context) {
    SdfTextureDrawPacket *packet = sdf2DMakeTexPacket((SdfTextureDrawPacket *)sdfGrAllocPacketBuff(sdf2DGetTexPacketSize(texture)), texture, context);
    sdfDrawTagLink(packetList, (u32)packet);
    return (s32)packet;
}

/* Exclude the two header quadwords from the reference payload count. */
void *sdf2DMakeStdPacket(void *packet, s32 packetBytes) {
    sdfGrSetRegsPacketHeader((SdfGsPacketHeader *)packet, (packetBytes >> SDF_DMA_QWORD_SHIFT) - 2);
    return packet;
}

/* Packed GIF loops contain registerCount values, plus two header quadwords. */
s32 sdf2DGetPrimPacketSize(s32 registerCount, s32 loopCount) {
    return (registerCount * loopCount + 2) << SDF_DMA_QWORD_SHIFT;
}

/* Return the packet payload address after the DMA/GIF header. */
u8 *sdf2DGetPrimPtr(SdfDrawPacket *packet) {
    return (u8 *)packet + sizeof(*packet);
}

/* Build packed GIF NLOOP/NREG/PRIM fields and its VIF DIRECT command. */
SdfDrawPacket *sdf2DMakePrimPacketHeader(SdfDrawPacket *packet, s32 primitiveFlags, s32 registerCount, s64 registerList, s32 loopCount) {
    s32 packetQuadwords = registerCount * loopCount + 1;
    s64 gifTag = loopCount | ((s64)registerCount << SDF_GIF_NREG_SHIFT);

    gifTag |= (s64)primitiveFlags << SDF_GIF_PRIM_SHIFT;
    gifTag |= SDF_GIF_PRE_EOP_FLAGS;
    packet->gifTag = gifTag;
    packet->command = packetQuadwords | SDF_VIF_DIRECT_COMMAND;
    packet->payloadHeader = registerList;
    packet->pad08 = 0;
    packet->quadwords = packetQuadwords;
    return packet;
}

/* Allocate loopCount sprite loops with RGBAQ, UV, XYZ2, UV, XYZ2 registers. */
SdfDrawPacket *sdf2DAllocSprite(s32 loopCount) {
    SdfDrawPacket *packet = (SdfDrawPacket *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(5, loopCount));
    sdf2DMakePrimPacketHeader(packet, 0x156, 5, 0x53531, loopCount);
    return packet;
}

/* Emit two matrices and three vectors; also cache the inverse origin in node. */
void sdf3DMakeViewportPacket(SdfVuBonePacket *packet, SdfProjectionRecord *node, const Matrix4 *transformMatrix) {
    packet->quadwords = 0xC;
    packet->command = 0x6C0BC000;
    packet->reservedWord = 0;
    VU0_LOAD_MATRIX(transformMatrix);
    VU0_STORE_MATRIX(packet->matrixA);
    sdfPostmultiplyVuMatrixFromMemory((const struct Matrix4 *)node->camera.matrix);
    VU0_STORE_MATRIX(packet->matrixB);
    SDF_VU0_LOADVECTOR(&node->camera.halfWidth);
        SDF_VU0_STOREVECTOR_NOMEM(packet->vecA);
    SDF_VU0_LOADVECTOR(&node->camera.originX);
        SDF_VU0_STOREVECTOR_NOMEM(packet->vecB);
    VU0_LOAD_MATRIX(transformMatrix);
    ReverseMatrix();
    SDF_VU0_GET_ROW3();
        SDF_VU0_STOREVECTOR_NOMEM(packet->vecC);
        SDF_VU0_STOREVECTOR_NOMEM(node->inverseOrigin);
    packet->mscalCommand = SDF_VIF_MSCAL_COMMAND;
    packet->stmodCommand = SDF_VIF_ITOP_MATRIX;
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
        SDF_VU0_LOADVECTOR(node->inverseOrigin);
    VU0_STORE_MATRIX(sdf3DScreenTm);
    SDF_VU0_APPLYMATRIX_XYZ1();
        SDF_VU0_STOREVECTOR_NOMEM(sdf3DEyeOffset);
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

extern s8 D_00438A50;

extern f32 D_00438A54;

extern f32 D_00438A58;

extern f32 D_00438A5C;

extern f32 D_00438A60;


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
    if (D_00438A50 != 0) {
        camera->halfWidth *= D_00438A54;
        camera->halfHeight *= D_00438A58;
        camera->originX += D_00438A5C;
        camera->originY += D_00438A60;
    }
}

extern u8 D_0040B540[]; /* fallback up vector */

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
        SDF_VU0_LOADVECTOR_B(D_0040B540);
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

/* DMA/VIF prefix, projection range terms, then a one-register GIF write
 * and the VU execution command. */
typedef struct SdfProjPacket {
    u64 dmaTag;
    u64 vifUnpackCode;
    f32 rangeMax;
    f32 rangeMin;
    f32 offset;
    f32 scale;
    u64 gifTag;
    u64 gifRegister;
    u64 registerValue;
    u64 fogColorRegister;
    u32 mscalCommand;
    u32 reservedA;
    u32 reservedB;
    u32 reservedC;
} SdfProjPacket;

/* Emit projection depth coefficients and a GS FOGCOL A+D write. */
void sdf3DMakeFogPacket(SdfProjPacket *packet, KwlnDrawVectorParams *projectionParams) {
    f32 rangeMax = projectionParams->fogFar;
    f32 rangeMin = projectionParams->fogNear;
    f32 nearZ = projectionParams->depthNear;
    f32 farZ = projectionParams->depthFar;
    u32 fogColor = projectionParams->fogColor;
    packet->dmaTag = 0x20000004;
    packet->vifUnpackCode = 0x6C03C00013000000ULL;
    packet->rangeMax = rangeMax;
    packet->rangeMin = rangeMin;
    packet->offset = (((rangeMax - rangeMin) * (farZ + nearZ)) / (farZ - nearZ) + (rangeMax + rangeMin)) * 0.5f;
    packet->scale = ((farZ * nearZ) * (rangeMin - rangeMax)) / (farZ - nearZ);
    packet->gifTag = 0x1000000000008001ULL;
    packet->gifRegister = SDF_GIF_REGISTER_AD;
    packet->registerValue = fogColor;
    packet->fogColorRegister = SDF_GS_FOGCOL_REGISTER;
    packet->mscalCommand = 0x14000014;
    packet->reservedA = 0;
    packet->reservedB = 0;
    packet->reservedC = 0;
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

extern u8 D_0037F330[];

/* Append the second fixed program block as a DMA reference packet. */
void sdf3DLinkInitPacket(sdfDrawTag_t *packetList, DmaPacketHeader *packet) {
    packet->address = (u32)D_0037B610 & SDF_DMA_ADDRESS_MASK;
    packet->quadwords = (D_0037F330 - D_0037B610) >> SDF_DMA_QWORD_SHIFT;
    packet->tag = 0;
    packet->command = 0;
    packet->unknown10 = 0;
    packet->unknown18 = 0;
    packet->unknown1C = 0;
    sdfAppendReferencePacket(packetList, (u32)packet);
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
    D_00438A70 = sdfTexSet(&texture->header);
    sdfMemFreeDirect(allocation);
}

/* Acquire texture resources, split their TEX0 words, and initialize queues. */
void sdf3DInit(void) {
    u64 textureWord;
    sdf3DInitSpecularTex2();
    textureWord = sdfTexGetTex0Reg(D_00438A70);
    D_0037F1B8[0] = textureWord;
    sdf3DRefMapParam = textureWord;
    D_0037F1B8[1] = textureWord >> 32;
    D_00438A6C = sdfTexSetDirect((SdfTextureFileHeader *)D_00372C00);
    textureWord = sdfTexGetTex0Reg(D_00438A6C);
    D_0037F270[0] = textureWord;
    D_0037F270[1] = textureWord >> 32;
    D_00438A80 = sdfTexSetDirect((SdfTextureFileHeader *)D_00376C40);
    textureWord = sdfTexGetTex0Reg(D_00438A80);
    D_0037F1F4[0] = textureWord;
    D_0037F1F4[1] = textureWord >> 32;
    sdfModelInitSystem();
    sdfMatInitSystem();
}

/* Append the fixed clear block; NULL allocatePacket selects the packet allocator. */
void sdf3DPrimLinkResetTm(sdfDrawTag_t *packetList,
                              s32 (*allocatePacket)(s32)) {
    u64 *referencePacket;
    if (allocatePacket == NULL) {
        allocatePacket = sdfGrAllocPacketBuff;
    }
    referencePacket = (u64 *)allocatePacket(0x20);
    referencePacket[0] = ((u64)((u32)D_0040B730 & SDF_DMA_ADDRESS_MASK) << 32) | 0x30000008;
    referencePacket[1] = 0x6C07C000ULL << 32;
    *(u128 *)&referencePacket[2] = 0;
    sdfAppendReferencePacket(packetList, (u32)referencePacket);
}

/* vu0 routine: store the resident matrix and inverse-column-length scaled rows, then VIF ITOP/MSCAL. */
void sdf3DPrimSetTmPacket(VuLightingPacket *lightingPacket) {
    VU0_STORE_MATRIX_AND_UNIT_ROWS(lightingPacket);
    lightingPacket->tag[0] = SDF_VIF_ITOP_LIGHTING;
    lightingPacket->tag[1] = SDF_VIF_MSCAL_COMMAND;
    lightingPacket->tag[2] = 0;
    lightingPacket->tag[3] = 0;
}

/* Append inline matrix/lighting data using allocatePacket or the default allocator. */
void sdf3DPrimLinkTm(sdfDrawTag_t *packetList,
                           s32 (*allocatePacket)(s32)) {
    u64 *dmaPacket;
    if (allocatePacket == NULL) {
        allocatePacket = sdfGrAllocPacketBuff;
    }
    dmaPacket = (u64 *)allocatePacket(0x90);
    dmaPacket[0] = ((u64)((u32)(dmaPacket + 2) & SDF_DMA_ADDRESS_MASK) << 32) | 0x20000008;
    dmaPacket[1] = 0x6C07C000ULL << 32;
    sdf3DPrimSetTmPacket((VuLightingPacket *)(dmaPacket + 2));
    sdfDrawTagLink(packetList, (u32)dmaPacket);
}

/* Apply current-buffer changes and append the asset reference; keep both index reads. */
void sdf3DPrimLinkMat(sdfDrawTag_t *packetList, sdfMat_t *asset,
                              s32 (*allocatePacket)(s32)) {
    u64 *referencePacket;
    if (allocatePacket == NULL) {
        allocatePacket = sdfGrAllocPacketBuff;
    }
    sdfMatUpdatePacket(asset, (s8)sdfFrameId);
    referencePacket = (u64 *)allocatePacket(0x20);
    sdfMatMakePacket(asset, (struct SdfDmaTagHeader *)referencePacket, (s8)sdfFrameId);
    *(u128 *)&referencePacket[2] = 0;
    sdfAppendReferencePacket(packetList, (u32)referencePacket);
}

extern f32 sdfFaceDivSoften;

extern f32 sdfFaceDivBias;

extern u32 D_0040B6B0[];

/* Serialize the requested VIF attribute streams into one aligned DMA packet. */
u32 *sdf3DPrimMakePoly(SdfPrimitiveRequest *request) {
    s32 stripWords = request->stripWordCount;
    s32 count = request->vertexCount;
    s32 headerWords = 18;
    s32 wordsPerVertex = 3;
    u32 primitive = request->primitiveFlags | 0x68;
    u32 format = 0x60;
    s32 bytes;
    void *(*allocate)(s32);
    u32 *packet;
    u32 *cursor;
    const u128 *vector;
    const u32 *coordinates;
    const u32 *secondCoordinates;
    s32 i;
    u32 countCode;

    if (request->normals != NULL) {
        format = 0xE0;
        headerWords = 19;
        wordsPerVertex = 6;
    }
    if (request->coordinates != NULL) {
        format |= 0x100;
        primitive |= 0x10;
        headerWords++;
        wordsPerVertex += 2;
        if (request->secondCoordinates != NULL) {
            primitive |= 0x1000;
            wordsPerVertex += 2;
        }
    }
    if (request->vertexColors != NULL) {
        format |= 0x200;
        primitive |= 0x800;
        headerWords++;
        wordsPerVertex++;
    }
    bytes = (headerWords + stripWords + wordsPerVertex * count) * 4;
    bytes = (bytes + 15) & ~15;
    allocate = request->allocate;
    if (allocate == NULL) allocate = sdfGrAllocPacketBuff;
    packet = allocate(bytes);
    *(u64 *)packet = (u64)(u16)((bytes >> 4) - 1) | 0x20000000ULL;
    packet[2] = 0x6102C000;
    packet[3] = (~request->clipMask) & 0xFFFF;
    packet[4] = 0x6E01C002;
    packet[5] = request->color;
    packet[6] = 0x6003C003;
    cursor = packet + 7;
    *cursor++ = (u32)request->depth;
    *cursor++ = (u32)sdfFaceDivSoften;
    *cursor++ = (u32)sdfFaceDivBias;
    *cursor++ = 0x04000004;
    *cursor++ = 0x14000000;
    *cursor++ = 0x6D01C000;
    *cursor++ = (u16)stripWords | ((u32)(u16)count << 16);
    *cursor++ = (primitive & 0xFFFF) | (format << 16);
    *cursor++ = (stripWords << 16) | 0x6E00C001;
    if (request->strip == NULL) {
        memcpy(cursor, D_0040B6B0, stripWords * 4);
    } else {
        memcpy(cursor, request->strip, stripWords * 4);
    }
    cursor += stripWords;
    stripWords++;
    countCode = count << 16;
    *cursor++ = countCode | stripWords | 0x6800C000;
    vector = request->positions;
    i = 0;
    do {
        EE_MMI_STORE_VEC3_VALUE(cursor, *vector);
        cursor += 3;
        vector++;
        i++;
    } while (i != count);
    stripWords += count;
    vector = request->normals;
    if (vector != NULL) {
        *cursor++ = countCode | stripWords | 0x6800C000;
        i = 0;
        do {
            EE_MMI_STORE_VEC3_VALUE(cursor, *vector);
            cursor += 3;
            vector++;
            i++;
        } while (i != count);
        stripWords += count;
    }
    coordinates = request->coordinates;
    if (coordinates != NULL) {
        secondCoordinates = request->secondCoordinates;
        if (secondCoordinates == NULL) {
            *cursor++ = countCode | stripWords | 0x6400C000;
            memcpy(cursor, coordinates, count * 8);
            cursor += count * 2;
        } else {
            *cursor++ = countCode | stripWords | 0x6C00C000;
            i = 0;
            do {
                *cursor++ = *coordinates++;
                *cursor++ = *coordinates++;
                *cursor++ = *secondCoordinates++;
                *cursor++ = *secondCoordinates++;
                i++;
            } while (i != count);
        }
        stripWords += count;
    }
    if (request->vertexColors != NULL) {
        *cursor++ = countCode | stripWords | 0x6E00C000;
        memcpy(cursor, request->vertexColors, count * 4);
        cursor += count;
    }
    *cursor = 0x1400000C;
    while ((u32)++cursor & 15) {
        *cursor = 0;
    }
    return packet;
}

void sdfBillBoardMakeTMPacket(u8 *packet, const f32 *matrix) {
    *(u16 *)packet = 2;
    *(u32 *)(packet + 8) = 0x6403C000;
    *(f32 *)(packet + 12) = matrix[0];
    *(f32 *)(packet + 16) = matrix[1];
    *(f32 *)(packet + 20) = matrix[4];
    *(f32 *)(packet + 24) = matrix[5];
    *(f32 *)(packet + 28) = matrix[12];
    *(f32 *)(packet + 32) = matrix[13];
    *(u32 *)(packet + 36) = 0x04000000;
    *(u32 *)(packet + 40) = 0x14000008;
    memset(packet + 0x2C, 0, 12);
}

typedef struct SdfTexturedQuadVifPacket {
    u16 qwordCount;
    u8 pad02[6];
    u32 commandWords[8];
    f32 position[3];
    u32 commandWord34;
    u32 color;
    u32 commandWord3C;
    u8 uvPayload[0x10];
    u32 commandWord50;
    f32 quad[8];
    u32 trailer[3];
} SdfTexturedQuadVifPacket;

typedef char SdfTexturedQuadVifPacket_size_check[
    sizeof(SdfTexturedQuadVifPacket) == 0x80 ? 1 : -1];
typedef char SdfTexturedQuadVifPacket_position_offset_check[
    ((u32)&((SdfTexturedQuadVifPacket *)0)->position == 0x28) ? 1 : -1];
typedef char SdfTexturedQuadVifPacket_uv_offset_check[
    ((u32)&((SdfTexturedQuadVifPacket *)0)->uvPayload == 0x40) ? 1 : -1];
typedef char SdfTexturedQuadVifPacket_quad_offset_check[
    ((u32)&((SdfTexturedQuadVifPacket *)0)->quad == 0x54) ? 1 : -1];

/* Build a one-vertex textured-quad VIF packet: the position, color, the four
 * UV corners (copied as unaligned words) and the eight quad corner coordinates. */
struct BillTextureQuad;
void sdfBillBoardMake1(u8 *packet, const f32 *position, u32 color,
                                   const struct BillTextureQuad *uv,
                                   const f32 *quad) {
    SdfTexturedQuadVifPacket *view = (SdfTexturedQuadVifPacket *)packet;

    view->qwordCount = 7;
    view->commandWords[0] = 0x6C01C000;
    view->commandWords[1] = 1;
    view->commandWords[2] = 0xA0000000;
    view->commandWords[3] = 0x43434310;
    view->commandWords[4] = 0x43;
    view->commandWords[5] = 0x6001C001;
    view->commandWords[6] = 0x155;
    view->commandWords[7] = 0x6801C002;
    view->position[0] = position[0];
    view->position[1] = position[1];
    view->position[2] = position[2];
    view->commandWord34 = 0x6E01C003;
    view->color = color;
    view->commandWord3C = 0x6504C004;
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(view->uvPayload), (const u8 *)uv);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(view->uvPayload + 4), (const u8 *)uv + 4);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(view->uvPayload + 8), (const u8 *)uv + 8);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(view->uvPayload + 12), (const u8 *)uv + 12);
    view->commandWord50 = 0x6404C008;
    view->quad[0] = quad[0];
    view->quad[1] = quad[1];
    view->quad[2] = quad[2];
    view->quad[3] = quad[3];
    view->quad[4] = quad[4];
    view->quad[5] = quad[5];
    view->quad[6] = quad[6];
    view->quad[7] = quad[7];
    view->trailer[0] = 0x04000002;
    view->trailer[1] = 0x14000008;
    view->trailer[2] = 0;
}

s32 sdfBillBoardGetPacketSize(s32 count) {
    return count * 0x40 + 0x40;
}

/* Build XYZ and attribute VIF streams; inputs are copied into the allocated
 * packet, and vertexCount must be positive. */
/* vu0 routine: pack aligned positions into the VIF three-word stream. */
u32 *sdfBillBoardMakeN(const u128 *positions, const u32 *packedColors, const void *uvCorners, const f32 *cornerOffsets, s32 vertexCount, void *(*allocatePacket)(s32)) {
    s32 packetBytes;
    u32 *packet;
    u32 *cursor;
    u32 vuAddress;
    u32 unpackCode;
    s32 n;
    s32 vertexIndex;

    packetBytes = sdfBillBoardGetPacketSize(vertexCount);
    if (allocatePacket == NULL) {
        cursor = (u32 *)sdfGrAllocPacketBuff(packetBytes);
    } else {
        cursor = allocatePacket(packetBytes);
    }
    packet = cursor;
    packet[0] = (packetBytes >> SDF_DMA_QWORD_SHIFT) - 1;
    packet[1] = 0;
    packet[2] = 0x6C01C000;
    packet[3] = vertexCount;
    packet[4] = 0xA0000000;
    packet[5] = 0x43434310;
    packet[6] = 0x43;
    packet[7] = 0x6001C001;
    packet[8] = 0x155;
    packet[9] = (vertexCount << 16) | 0x6800C002;
    cursor = packet + 10;
    vertexIndex = 0;
    do {
        EE_MMI_STORE_VEC3_VALUE(cursor, positions[vertexIndex]);
        cursor += 3;
        vertexIndex++;
    } while (vertexIndex != vertexCount);
    n = vertexCount;
    unpackCode = n << 16;
    vuAddress = vertexCount + 2;
    *cursor++ = unpackCode | vuAddress | 0x6E00C000;
    memcpy(cursor, packedColors, n * 4);
    cursor += n;
    vuAddress += n;
    n = vertexCount * 4;
    unpackCode = n << 16;
    *cursor++ = unpackCode | vuAddress | 0x6500C000;
    vuAddress += n;
    unpackCode |= vuAddress;
    memcpy(cursor, uvCorners, n * 4);
    cursor += n;
    n *= 8;
    unpackCode |= 0x6400C000;
    *cursor++ = unpackCode;
    memcpy(cursor, cornerOffsets, n);
    cursor = (u32 *)((u8 *)cursor + n);
    cursor[0] = SDF_VIF_ITOP_COMPACT_VERTEX;
    cursor[1] = SDF_VIF_MSCAL_VERTICES;
    cursor += 2;
    while (((u32)cursor & SDF_QWORD_ALIGNMENT_MASK) != 0) {
        *cursor++ = 0;
    }
    return packet;
}

/* Build a one-vertex VIF packet for a two-texture quad: the position, both
 * colors, the interleaved primary/secondary UV corners (copied as unaligned
 * words) and the eight quad corner coordinates. */
void sdfMultiBillBoardMake1(u8 *packet, const f32 *position, u32 primaryColor, u32 secondaryColor,
                   const void *primaryUv, const void *secondaryUv, const f32 *quad) {
    *(u16 *)packet = 8;
    *(u32 *)(packet + 8) = 0x6C01C000;
    *(u32 *)(packet + 12) = 1;
    *(u32 *)(packet + 16) = 0xA0000000;
    *(u32 *)(packet + 20) = 0x43434310;
    *(u32 *)(packet + 24) = 0x43;
    *(u32 *)(packet + 28) = 0x6001C001;
    *(u32 *)(packet + 32) = 0x155;
    *(u32 *)(packet + 36) = 0x6801C002;
    *(f32 *)(packet + 40) = position[0];
    *(f32 *)(packet + 44) = position[1];
    *(f32 *)(packet + 48) = position[2];
    *(u32 *)(packet + 52) = 0x6E02C003;
    *(u32 *)(packet + 56) = primaryColor;
    *(u32 *)(packet + 60) = secondaryColor;
    *(u32 *)(packet + 64) = 0x6D04C005;
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 68), (const u8 *)primaryUv);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 72), (const u8 *)secondaryUv);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 76), (const u8 *)primaryUv + 4);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 80), (const u8 *)secondaryUv + 4);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 84), (const u8 *)primaryUv + 8);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 88), (const u8 *)secondaryUv + 8);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 92), (const u8 *)primaryUv + 12);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 96), (const u8 *)secondaryUv + 12);
    *(u32 *)(packet + 100) = 0x6404C009;
    *(f32 *)(packet + 104) = quad[0];
    *(f32 *)(packet + 108) = quad[1];
    *(f32 *)(packet + 112) = quad[2];
    *(f32 *)(packet + 116) = quad[3];
    *(f32 *)(packet + 120) = quad[4];
    *(f32 *)(packet + 124) = quad[5];
    *(f32 *)(packet + 128) = quad[6];
    *(f32 *)(packet + 132) = quad[7];
    *(u32 *)(packet + 136) = 0x04000004;
    *(u32 *)(packet + 140) = 0x14000008;
}

u32 sdfMultiBillBoardGetPacketSize(s32 count) {
    return (count * 0x54 + 0x4bU) & 0xfffffff0;
}

/* Build a quad packet from paired colors and per-corner UV sets; return its address. */
u32 *sdfMultiBillBoardMakeN(u128 *positions, void *attributes, void *halfAttributes, void *wordAttributes, s32 count, void *(*alloc)(s32)) {
    s32 bytes;
    u32 *packet;
    u32 *cursor;
    u32 index;
    u32 code;
    s32 n;
    s32 i;

    bytes = sdfMultiBillBoardGetPacketSize(count);
    if (alloc == NULL) {
        cursor = (u32 *)sdfGrAllocPacketBuff(bytes);
    } else {
        cursor = alloc(bytes);
    }
    packet = cursor;
    packet[0] = (bytes >> 4) - 1;
    packet[1] = 0;
    packet[2] = 0x6C01C000;
    packet[3] = count;
    packet[4] = 0xA0000000;
    packet[5] = 0x43434310;
    packet[6] = 0x43;
    packet[7] = 0x6001C001;
    packet[8] = 0x155;
    packet[9] = (count << 16) | 0x6800C002;
    cursor = packet + 10;
    i = 0;
    do {
        EE_MMI_STORE_VEC3_VALUE(cursor, positions[i]);
        cursor += 3;
        i++;
    } while (i != count);
    n = count * 2;
    index = count + 2;
    *cursor++ = (n << 16) | index | 0x6E00C000;
    memcpy(cursor, attributes, n * 4);
    cursor += n;
    index += n;
    n = count * 4;
    code = n << 16;
    *cursor++ = code | index | 0x6D00C000;
    memcpy(cursor, halfAttributes, n * 8);
    cursor += n * 2;
    index += n;
    code |= index;
    *cursor++ = code | 0x6400C000;
    memcpy(cursor, wordAttributes, n * 8);
    cursor += n * 2;
    cursor[0] = 0x04000004;
    cursor[1] = 0x14000008;
    cursor += 2;
    while (((u32)cursor & 0xF) != 0) {
        *cursor++ = 0;
    }
    return packet;
}

/* Build a one-vertex textured-quad VIF packet with four corner colors: the
 * position, colors, UV corners (copied as unaligned words) and the eight quad
 * corner coordinates. */
void sdfBillBoardVcMake1(u8 *packet, const f32 *position, const u32 *colors, const struct BillTextureQuad *uv, const f32 *quad) {
    *(u16 *)packet = 8;
    *(u32 *)(packet + 8) = 0x6C01C000;
    *(u32 *)(packet + 12) = 1;
    *(u32 *)(packet + 16) = 0xD0000000;
    *(u32 *)(packet + 20) = 0x34134130;
    *(u32 *)(packet + 24) = 0x41341;
    *(u32 *)(packet + 28) = 0x6001C001;
    *(u32 *)(packet + 32) = 0x155;
    *(u32 *)(packet + 36) = 0x6801C002;
    *(f32 *)(packet + 40) = position[0];
    *(f32 *)(packet + 44) = position[1];
    *(f32 *)(packet + 48) = position[2];
    *(u32 *)(packet + 52) = 0x6E04C003;
    *(u32 *)(packet + 56) = colors[0];
    *(u32 *)(packet + 60) = colors[1];
    *(u32 *)(packet + 64) = colors[2];
    *(u32 *)(packet + 68) = colors[3];
    *(u32 *)(packet + 72) = 0x6504C007;
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 76), (const u8 *)uv);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 80), (const u8 *)uv + 4);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 84), (const u8 *)uv + 8);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 88), (const u8 *)uv + 12);
    *(u32 *)(packet + 92) = 0x6404C00B;
    *(f32 *)(packet + 96) = quad[0];
    *(f32 *)(packet + 100) = quad[1];
    *(f32 *)(packet + 104) = quad[2];
    *(f32 *)(packet + 108) = quad[3];
    *(f32 *)(packet + 112) = quad[4];
    *(f32 *)(packet + 116) = quad[5];
    *(f32 *)(packet + 120) = quad[6];
    *(f32 *)(packet + 124) = quad[7];
    *(u32 *)(packet + 128) = 0x04000006;
    *(u32 *)(packet + 132) = 0x14000008;
    *(u32 *)(packet + 136) = 0;
    *(u32 *)(packet + 140) = 0;
}

u32 sdfBillBoardVcGetPacketSize(s32 count) {
    return (count * 0x4C + 0x4B) & ~0xF;
}

/* Build a quad packet with four corner colors, UV pairs and XY offset pairs. */
u32 sdfBillBoardVcMakeN(const u128 *positions, const void *attributes,
                  const void *halfAttributes, const void *wordAttributes,
                  s32 count, void *(*alloc)(s32)) {
    s32 bytes;
    u32 *packet;
    u32 *cursor;
    u32 index;
    u32 code;
    s32 n;
    s32 i;

    bytes = sdfBillBoardGetPacketSize(count);
    if (alloc == NULL) {
        cursor = (u32 *)sdfGrAllocPacketBuff(bytes);
    } else {
        cursor = alloc(bytes);
    }
    packet = cursor;
    packet[0] = (bytes >> 4) - 1;
    packet[1] = 0;
    packet[2] = 0x6C01C000;
    packet[3] = count;
    packet[4] = 0xD0000000;
    packet[5] = 0x34134130;
    packet[6] = 0x41341;
    packet[7] = 0x6001C001;
    packet[8] = 0x155;
    packet[9] = (count << 16) | 0x6800C002;

    cursor = packet + 10;
    i = 0;
    do {
        EE_MMI_STORE_VEC3_VALUE(cursor, positions[i]);
        cursor += 3;
        i++;
    } while (i != count);

    code = count << 18;
    index = count + 2;
    n = count * 4;
    *cursor++ = code | index | 0x6E00C000;
    memcpy(cursor, attributes, n * 4);
    cursor += n;
    index += n;
    *cursor++ = code | index | 0x6500C000;
    memcpy(cursor, halfAttributes, n * 4);
    cursor += n;
    index += n;
    n = count * 8;
    *cursor++ = code | index | 0x6400C000;
    memcpy(cursor, wordAttributes, n * 4);
    cursor += n;
    cursor[0] = 0x04000006;
    cursor[1] = 0x14000008;
    cursor += 2;
    while (((u32)cursor & 0xF) != 0) {
        *cursor++ = 0;
    }
    return (u32)packet;
}

/* Build a one-vertex VIF packet for a two-texture quad with eight colors: the
 * position, colors, interleaved primary/secondary UV corners (copied as
 * unaligned words) and the eight quad corner coordinates. */
void sdfMultiBillBoardVcMake1(u8 *packet, const f32 *position, const void *colors, const void *primaryUv,
                   const void *secondaryUv, const void *quad) {
    *(u16 *)packet = 10;
    *(u32 *)(packet + 8) = 0x6C01C000;
    *(u32 *)(packet + 12) = 1;
    *(u32 *)(packet + 16) = 0xD0000000;
    *(u32 *)(packet + 20) = 0x34134130;
    *(u32 *)(packet + 24) = 0x41341;
    *(u32 *)(packet + 28) = 0x6001C001;
    *(u32 *)(packet + 32) = 0x15D;
    *(u32 *)(packet + 36) = 0x6801C002;
    *(f32 *)(packet + 40) = position[0];
    *(f32 *)(packet + 44) = position[1];
    *(f32 *)(packet + 48) = position[2];
    *(u32 *)(packet + 52) = 0x6E08C003;
    memcpy(packet + 56, colors, 32);
    *(u32 *)(packet + 88) = 0x6D04C00B;
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 92), (const u8 *)primaryUv);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 96), (const u8 *)secondaryUv);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 100), (const u8 *)primaryUv + 4);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 104), (const u8 *)secondaryUv + 4);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 108), (const u8 *)primaryUv + 8);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 112), (const u8 *)secondaryUv + 8);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 116), (const u8 *)primaryUv + 12);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 120), (const u8 *)secondaryUv + 12);
    *(u32 *)(packet + 124) = 0x6404C00F;
    memcpy(packet + 128, quad, 32);
    *(u32 *)(packet + 160) = 0x04000008;
    *(u32 *)(packet + 164) = 0x14000008;
    *(u32 *)(packet + 168) = 0;
    *(u32 *)(packet + 172) = 0;
}

u32 sdfMultiBillBoardVcGetPacketSize(s32 count) {
    return (count * 0x6c + 0x4bU) & 0xfffffff0;
}

/* Build the wider attribute VIF stream; return the packet address.
 * vertexCount must be positive; preserve the native copy lengths and n reuse. */
/* vu0 routine: pack aligned positions into the VIF three-word stream. */
u32 sdfMultiBillBoardVcMakeN(u128 *positions, void *byteAttributes, void *halfAttributes, void *wordAttributes, s32 vertexCount, void *(*allocatePacket)(s32)) {
    s32 packetBytes;
    u32 *packet;
    u32 *cursor;
    u32 vuAddress;
    u32 unpackCode;
    s32 n;
    s32 vertexIndex;

    packetBytes = sdfMultiBillBoardVcGetPacketSize(vertexCount);
    if (allocatePacket == NULL) {
        cursor = (u32 *)sdfGrAllocPacketBuff(packetBytes);
    } else {
        cursor = allocatePacket(packetBytes);
    }
    packet = cursor;
    packet[0] = (packetBytes >> SDF_DMA_QWORD_SHIFT) - 1;
    packet[1] = 0;
    packet[2] = 0x6C01C000;
    packet[3] = vertexCount;
    packet[4] = 0xD0000000;
    packet[5] = 0x34134130;
    packet[6] = 0x41341;
    packet[7] = 0x6001C001;
    packet[8] = 0x15D;
    packet[9] = (vertexCount << 16) | 0x6800C002;
    cursor = packet + 10;
    vertexIndex = 0;
    do {
        EE_MMI_STORE_VEC3_VALUE(cursor, positions[vertexIndex]);
        cursor += 3;
        vertexIndex++;
    } while (vertexIndex != vertexCount);
    n = vertexCount * 8;
    vuAddress = vertexCount + 2;
    *cursor++ = (n << 16) | vuAddress | 0x6E00C000;
    memcpy(cursor, byteAttributes, n * 4);
    cursor += n;
    vuAddress += n;
    n = vertexCount * 4;
    unpackCode = n << 16;
    *cursor++ = unpackCode | vuAddress | 0x6D00C000;
    memcpy(cursor, halfAttributes, n * 8);
    cursor += n * 2;
    vuAddress += n;
    unpackCode |= vuAddress;
    *cursor++ = unpackCode | 0x6400C000;
    memcpy(cursor, wordAttributes, n * 8);
    cursor += n * 2;
    cursor[0] = SDF_VIF_ITOP_WIDE_VERTEX;
    cursor[1] = SDF_VIF_MSCAL_VERTICES;
    cursor += 2;
    while (((u32)cursor & SDF_QWORD_ALIGNMENT_MASK) != 0) {
        *cursor++ = 0;
    }
    return (u32)packet;
}

extern u8 sdfPadCb_0[];

extern s32 scePadGetState(s32 port, s32 slot);

extern s32 scePadRead(s32 port, s32 slot, void *data);

extern s32 scePadGetReqState(s32 port, s32 slot);

extern s32 scePadInfoAct(s32 port, s32 slot, s32 actNo, s32 term);

extern s32 scePadSetActDirect(s32 port, s32 slot, void *actData);



/* Drive mode/actuator setup and normalize this pad's latest reply.
 * Motor requests are sent only when their cached values change. */
void sdfPadControl(SdfPadEntry *entry) {
    u8 reply[SDF_PAD_REPLY_BUFFER_BYTES];
    u8 actuatorData[SDF_PAD_ACTUATOR_BYTES];
    s32 port = entry->port;
    s32 slot = entry->slot;
    s32 padState;
    s32 hasButtons;
    s32 hasAnalog;
    s32 hasPressure;
    s32 alignmentStatus;
    s32 requestedMode;
    s32 smallMotor;
    s32 largeMotor;
    reply[0] = -1;
    padState = scePadGetState(port, slot);
    switch (entry->state) {
    case SDF_PAD_CONTROL_APPLY_REQUESTED_MODE:
        if (padState == 2 || padState == 6) {
            entry->lastSmallMotor = -1;
            entry->lastLargeMotor = -1;
            requestedMode = entry->mode = entry->requestedMode;
            switch (requestedMode) {
            case SDF_PAD_MODE_KEEP_CURRENT:
                entry->state = SDF_PAD_CONTROL_PROBE_ACTUATORS;
                break;
            case SDF_PAD_MODE_DIGITAL_UNLOCKED:
                if (padState == 6) {
                    if (scePadSetMainMode(port, slot, 0, 0) == 1) {
                        entry->state = SDF_PAD_CONTROL_WAIT_FOR_MODE_CHANGE;
                    }
                } else {
                    entry->state = SDF_PAD_CONTROL_PROBE_ACTUATORS;
                }
                break;
            case SDF_PAD_MODE_DIGITAL_LOCKED:
                if (padState == 6) {
                    if (scePadSetMainMode(port, slot, 0, 3) == 1) {
                        entry->state = SDF_PAD_CONTROL_WAIT_FOR_MODE_CHANGE;
                    }
                } else {
                    entry->state = SDF_PAD_CONTROL_PROBE_ACTUATORS;
                }
                break;
            case SDF_PAD_MODE_DUALSHOCK_LOCKED:
                if (padState == 6) {
                    if (scePadSetMainMode(port, slot, 1, 3) == 1) {
                        entry->state = SDF_PAD_CONTROL_WAIT_FOR_MODE_CHANGE;
                    }
                } else {
                    entry->state = SDF_PAD_CONTROL_PROBE_ACTUATORS;
                }
                break;
            }
        }
        break;
    case SDF_PAD_CONTROL_WAIT_FOR_MODE_CHANGE:
        if (padState == 6) {
            entry->state = SDF_PAD_CONTROL_PROBE_ACTUATORS;
        } else if (padState != 5) {
            entry->state = SDF_PAD_CONTROL_APPLY_REQUESTED_MODE;
        }
        break;
    case SDF_PAD_CONTROL_UNKNOWN_2:
        break;
    case SDF_PAD_CONTROL_PROBE_ACTUATORS:
        if (scePadInfoAct(port, slot, -1, 0) != 0) {
            if (scePadSetActAlign(port, slot, sdfPadCb_0) != 0) {
                entry->state = SDF_PAD_CONTROL_WAIT_FOR_ACTUATOR_ALIGNMENT;
            }
        } else {
            entry->state = SDF_PAD_CONTROL_READ_INPUT_AND_UPDATE_MOTORS;
        }
        break;
    case SDF_PAD_CONTROL_WAIT_FOR_ACTUATOR_ALIGNMENT:
        alignmentStatus = scePadGetReqState(port, slot);
        if (alignmentStatus != 0) {
            if (alignmentStatus == 1) {
                entry->state = SDF_PAD_CONTROL_PROBE_ACTUATORS;
            }
        } else {
            entry->state = SDF_PAD_CONTROL_READ_INPUT_AND_UPDATE_MOTORS;
        }
        break;
    case SDF_PAD_CONTROL_READ_INPUT_AND_UPDATE_MOTORS:
        if (padState != 2 && padState != 6) {
            entry->state = SDF_PAD_CONTROL_APPLY_REQUESTED_MODE;
        } else if (entry->mode != entry->requestedMode) {
            entry->state = SDF_PAD_CONTROL_APPLY_REQUESTED_MODE;
        } else {
            scePadRead(port, slot, reply);
            smallMotor = entry->smallMotor;
            largeMotor = entry->largeMotor;
            if (smallMotor != entry->lastSmallMotor || largeMotor != entry->lastLargeMotor) {
                entry->lastSmallMotor = smallMotor;
                entry->lastLargeMotor = largeMotor;
                actuatorData[0] = smallMotor;
                actuatorData[1] = largeMotor;
                scePadSetActDirect(port, slot, actuatorData);
            }
        }
        break;
    }
    /* Pad reply IDs: digital, analog-stick, and pressure-sensitive modes.
     * Missing channels are normalized before consumers see this port. */
    hasAnalog = 0;
    hasPressure = 0;
    hasButtons = 0;
    if (reply[0] == 0) {
        switch (reply[1]) {
        case SDF_PAD_REPLY_DIGITAL:
            hasButtons = 1;
            break;
        case SDF_PAD_REPLY_ANALOG:
            hasButtons = 1;
            hasAnalog = 1;
            break;
        case SDF_PAD_REPLY_PRESSURE:
            hasButtons = 1;
            hasAnalog = 1;
            hasPressure = 1;
            break;
        }
    }
    if (hasButtons) {
        memset(entry->pressure, 0, SDF_PAD_PRESSURE_COUNT);
        entry->buttons = ~(reply[3] | (reply[2] << 8));
    } else {
        entry->buttons = 0;
    }
    if (hasAnalog) {
        entry->stick[0] = reply[4];
        entry->stick[1] = reply[5];
        entry->stick[2] = reply[6];
        entry->stick[3] = reply[7];
    } else {
        entry->stick[0] = SDF_PAD_STICK_CENTER;
        entry->stick[1] = SDF_PAD_STICK_CENTER;
        entry->stick[2] = SDF_PAD_STICK_CENTER;
        entry->stick[3] = SDF_PAD_STICK_CENTER;
    }
    if (hasPressure) {
        memcpy(entry->pressure, &reply[8], SDF_PAD_PRESSURE_COUNT);
    } else {
        memset(entry->pressure, 0, SDF_PAD_PRESSURE_COUNT);
    }
}

/* Update each configured logical pad entry. */
void sdfPadManager(void) {
    SdfPadIndex padIndex;
    for (padIndex = 0; padIndex != SDF_PAD_COUNT; padIndex++) {
        sdfPadControl(&sdfPadCb[padIndex]);
    }
}

extern s32 sdfVblankFlag;

extern u16 D_00438A90[4];

extern u8 sdfPadAnalogSticks[8];

extern u16 D_0040B7B0[16];

extern s8 dds3PadBitTable[0x20];

extern u8 sdfPadButtonPressure[0x18];

/* Emit held bit 0, press/repeat trigger bit 1, and new-press bit 7 per button.
 * Changes arm a 15-tick deadline; repeats set it to current tick + lateness + 4,
 * retaining the original extra delay when a deadline is missed. */
void sdfPadBuildButtonStates(void) {
    s32 currentTick = sdfVblankFlag;
    SdfPadIndex padIndex;
    s32 buttonIndex;
    for (padIndex = 0; padIndex != SDF_PAD_COUNT; padIndex++) {
        SdfPadEntry *entry = &sdfPadCb[padIndex];
        s32 heldButtons = entry->buttons;
        s32 previousButtons = entry->prevButtons;
        s32 newPressMask;
        s32 triggerMask;
        D_00438A90[padIndex] = heldButtons;
        entry->prevButtons = heldButtons;
        newPressMask = (heldButtons ^ previousButtons) & heldButtons;
        if (heldButtons != previousButtons) {
            entry->repeatDeadline = currentTick + SDF_PAD_REPEAT_DELAY;
            triggerMask = newPressMask;
        } else {
            triggerMask = 0;
            if (heldButtons != 0) {
                s32 ticksLate = currentTick - entry->repeatDeadline;
                if (ticksLate >= 0) {
                    triggerMask = heldButtons;
                    entry->repeatDeadline = currentTick + ticksLate + SDF_PAD_REPEAT_STEP;
                }
            }
        }
        for (buttonIndex = 0; buttonIndex != SDF_PAD_BUTTON_COUNT; buttonIndex++) {
            s32 buttonMask = D_0040B7B0[buttonIndex];
            s32 buttonState = (heldButtons & buttonMask) != 0;
            if (triggerMask & buttonMask) {
                buttonState |= SDF_PAD_BUTTON_TRIGGER_BIT;
            }
            if (newPressMask & buttonMask) {
                buttonState |= SDF_PAD_BUTTON_NEW_PRESS_BIT;
            }
            dds3PadBitTable[padIndex * SDF_PAD_BUTTON_COUNT + buttonIndex] = buttonState;
        }
        memcpy(&sdfPadAnalogSticks[padIndex * SDF_PAD_STICK_COUNT], entry->stick, SDF_PAD_STICK_COUNT);
        memcpy(&sdfPadButtonPressure[padIndex * SDF_PAD_PRESSURE_COUNT], entry->pressure, SDF_PAD_PRESSURE_COUNT);
    }
}

/* Store a mode request for the indexed logical pad; setup applies it later. */
void sdfPadChangeMode(SdfPadIndex padIndex, SdfPadMode requestedMode) {
    sdfPadCb[padIndex].requestedMode = requestedMode;
}

/* Store the small-motor request without validating the logical pad index. */
void sdfPadSetAct0(SdfPadIndex padIndex, u16 strength) {
    sdfPadCb[padIndex].smallMotor = strength;
}

/* Store the large-motor request without validating the logical pad index. */
void sdfPadSetAct1(SdfPadIndex padIndex, u8 strength) {
    sdfPadCb[padIndex].largeMotor = strength;
}

/* Despite the legacy console-prefixed name, set both pad motor low bytes. */
void sdfPadSetAct(SdfPadIndex padIndex, s32 smallMotor, s32 largeMotor) {
    SdfPadEntry *entry = &sdfPadCb[padIndex];
    entry->smallMotor = smallMotor & SDF_PAD_MOTOR_VALUE_MASK;
    sdfPadCb[padIndex].largeMotor = largeMotor & SDF_PAD_MOTOR_VALUE_MASK;
}

extern u8 D_00438A88[4];

extern u8 sdfPadPortBuffers[];

extern u8 D_00438A8C;

extern s32 scePadInit(s32);


/* Open both configured port/slot pairs and reset the public input arrays. */
void sdfPadInitManager(void) {
    SdfPadIndex padIndex;

    scePadInit(0);
    for (padIndex = 0; padIndex != SDF_PAD_COUNT; padIndex++) {
        s32 port = D_00438A88[padIndex * 2];
        s32 slot = D_00438A88[padIndex * 2 + 1];
        SdfPadEntry *entry;

        scePadPortOpen(port, slot, &sdfPadPortBuffers[padIndex * SDF_PAD_PORT_BUFFER_BYTES]);
        entry = &sdfPadCb[padIndex];
        entry->port = port;
        entry->slot = slot;
        entry->state = SDF_PAD_CONTROL_APPLY_REQUESTED_MODE;
        entry->mode = SDF_PAD_MODE_KEEP_CURRENT;
        entry->requestedMode = SDF_PAD_MODE_KEEP_CURRENT;
        entry->buttons = 0;
        entry->prevButtons = 0;
        entry->smallMotor = 0;
        entry->largeMotor = 0;
    }
    memset(dds3PadBitTable, 0, SDF_PAD_BUTTON_STATE_BYTES);
    memset(sdfPadAnalogSticks, SDF_PAD_STICK_CENTER, SDF_PAD_STICK_STATE_BYTES);
    memset(sdfPadButtonPressure, 0, SDF_PAD_PRESSURE_STATE_BYTES);
    D_00438A8C = 0;
}

/* Acquire and cache the shared console texture on first use. */
void sdfDevConsInit(void) {
    if (D_00438AB4 == 0) {
        D_00438AB4 = 1;
        sdfDebugFontHandle = sdfTexSet((SdfTextureFileHeader *)(D_00370B80));
    }
}

/* Ensure initialization and return the cached console texture. */
struct sdfTexHandle_t *sdfDevConsGetFontHandle(void) {
    sdfDevConsInit();
    return sdfDebugFontHandle;
}

/* Return the packed glyph-UV table indexed by character minus 0x20. */
u32 *sdfDevConsGetFontUvTable(void) {
    return sdfDevConsFontUvTable;
}

/* Append to the next-linked list and move its stored tail to this node. */
void sdfDevConsLinkToTop(sdfDevConsCB_tag *node) {
    sdfDevConsCB_tag *previousTail = D_00438AB0;

    node->next = NULL;
    node->previousNode = previousTail;
    if (previousTail != NULL) {
        previousTail->next = node;
    }
    D_00438AB0 = node;
}

/* Unlink this node, moving the stored tail when its next link is NULL. */
void sdfDevConsUnlink(node)
    sdfDevConsCB_tag *node;
{
    sdfDevConsCB_tag *nextNode = node->next;
    sdfDevConsCB_tag *previousNode = node->previousNode;
    if (nextNode != NULL) {
        nextNode->previousNode = previousNode;
    } else {
        D_00438AB0 = previousNode;
    }
    if (previousNode != NULL) {
        previousNode->next = nextNode;
    }
}

/* Unlink the console, release its cell-buffer allocation, then free the node. */
void sdfDevConsClose(sdfDevConsCB_tag *node) {
    sdfDevConsUnlink(node);
    sdfMemFreeDirect(node->bufferAllocation);
    sdfChipFreeDirect(node);
}

/* Reset both cursor coordinates and clear the two-byte character-cell grid. */
void sdfDevConsClearInside(sdfDevConsCB_tag *node) {
    node->cursorColumn = 0;
    node->cursorRow = 0;
    memset(node->cells, 0, node->columns * node->rows * SDF_CONSOLE_CELL_BYTES);
}

/* Reset a console through the existing clear operation. */
void sdfDevConsClear(sdfDevConsCB_tag *node) {
    sdfDevConsClearInside(node);
}

/* Allocate a two-byte character grid and link it for cleanup.
 * Allocation uses the supplied dimensions; clearing uses their stored s16
 * values. Dimensions and the two opaque halfword inputs are not validated. */
sdfDevConsCB_tag *sdfDevConsOpen(u32 screenOriginX, u32 screenOriginY, s32 columns, s32 rows) {
    sdfDevConsCB_tag *node;
    struct sdfMemHandle_t *bufferAllocation;

    sdfDevConsInit();
    node = sdfChipAllocDirect(SDF_CONSOLE_NODE_BYTES);
    node->screenOriginX = screenOriginX;
    node->screenOriginY = screenOriginY;
    node->columns = columns;
    node->rows = rows;
    node->tabWidth = 8;
    node->controlByte = 0;
    node->textAttribute = 0;
    bufferAllocation = sdfMemAllocDirect((columns * rows) * SDF_CONSOLE_CELL_BYTES);
    node->bufferAllocation = bufferAllocation;
    node->cells = (u8 *)sdfMemLock(bufferAllocation);
    sdfDevConsClearInside(node);
    sdfDevConsLinkToTop(node);
    return node;
}

/* Packed console glyph UVs for characters 0x20 through 0x7F. */
u32 sdfDevConsFontUvTable[96] __attribute__((aligned(8))) = {
    0x00000000, 0x000000C0, 0x00000180, 0x00000240, 0x00000300, 0x000003C0, 0x00000480, 0x00000540,
    0x00000600, 0x000006C0, 0x00C00000, 0x00C000C0, 0x00C00180, 0x00C00240, 0x00C00300, 0x00C003C0,
    0x00C00480, 0x00C00540, 0x00C00600, 0x00C006C0, 0x01800000, 0x018000C0, 0x01800180, 0x01800240,
    0x01800300, 0x018003C0, 0x01800480, 0x01800540, 0x01800600, 0x018006C0, 0x02400000, 0x024000C0,
    0x02400180, 0x02400240, 0x02400300, 0x024003C0, 0x02400480, 0x02400540, 0x02400600, 0x024006C0,
    0x03000000, 0x030000C0, 0x03000180, 0x03000240, 0x03000300, 0x030003C0, 0x03000480, 0x03000540,
    0x03000600, 0x030006C0, 0x03C00000, 0x03C000C0, 0x03C00180, 0x03C00240, 0x03C00300, 0x03C003C0,
    0x03C00480, 0x03C00540, 0x03C00600, 0x03C006C0, 0x04800000, 0x048000C0, 0x04800180, 0x04800240,
    0x04800300, 0x048003C0, 0x04800480, 0x04800540, 0x04800600, 0x048006C0, 0x05400000, 0x054000C0,
    0x05400180, 0x05400240, 0x05400300, 0x054003C0, 0x05400480, 0x05400540, 0x05400600, 0x054006C0,
    0x06000000, 0x060000C0, 0x06000180, 0x06000240, 0x06000300, 0x060003C0, 0x06000480, 0x06000540,
    0x06000600, 0x060006C0, 0x06C00000, 0x06C000C0, 0x06C00180, 0x06C00240, 0x06C00300, 0x06C003C0,
};

/* Mutable colors used by the device console and packet command setter. */
u32 sdfDevConsColorTable[16] = {
    0x80808080, 0x80800000, 0x80000080, 0x80800080,
    0x80008000, 0x80808000, 0x80008080, 0x80606060,
    0x80202020, 0x80400000, 0x80000040, 0x80400040,
    0x80004000, 0x80404000, 0x80004040, 0x00000000,
};

INCLUDE_RODATA(const s32, "game/code_003396D0", D_0042E258);

INCLUDE_SDATA(sdfMemHandle_t * const, "game/code_003396D0", D_00438A40);

INCLUDE_SDATA(const s32, "game/code_003396D0", sdfFaceDivSoften);

INCLUDE_SDATA(const s32, "game/code_003396D0", sdfFaceDivBias);

INCLUDE_SDATA(const s32, "game/code_003396D0", D_00438A50);

INCLUDE_SDATA(const s32, "game/code_003396D0", D_00438A54);

INCLUDE_SDATA(const s32, "game/code_003396D0", D_00438A58);

INCLUDE_SDATA(const s32, "game/code_003396D0", D_00438A5C);

INCLUDE_SDATA(const s32, "game/code_003396D0", D_00438A60);

INCLUDE_SDATA(const s32, "game/code_003396D0", sdf3DLightDefault);

INCLUDE_SDATA(const s32, "game/code_003396D0", sdf3DFogDefault);

INCLUDE_SDATA(const s32, "game/code_003396D0", D_00438A6C);

INCLUDE_SDATA(const s32, "game/code_003396D0", D_00438A70);

INCLUDE_SDATA(const s32, "game/code_003396D0", sdf3DRefMapParam);

INCLUDE_SDATA(const s32, "game/code_003396D0", D_00438A80);

INCLUDE_SDATA(const s32, "game/code_003396D0", D_00438A88);

INCLUDE_SDATA(const s32, "game/code_003396D0", D_00438A8C);

INCLUDE_SDATA(const s32, "game/code_003396D0", D_00438A90);

INCLUDE_SDATA(const s32, "game/code_003396D0", sdfPadAnalogSticks);

INCLUDE_SDATA(const s32, "game/code_003396D0", D_00438A99);

INCLUDE_SDATA(const s32, "game/code_003396D0", D_00438AB0);

INCLUDE_SDATA(const s32, "game/code_003396D0", D_00438AB4);
