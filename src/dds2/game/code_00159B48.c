#include "sdk_ps2.h"
#include "eff_composite_gs_descriptor.h"
#include "sdf_gs_header.h"
#include "sdf_quaternion.h"
#include "common.h"
#include "sdf_vu0.h"
#include "sdf_vu_matrix.h"
#include "eff_generated_texture_descriptor.h"
#include "kwln_draw_buffer.h"
#include "kwln_frame_draw_buffer.h"
#include "sdf_trig.h"
#include "eff_random.h"
#include "sdf.h"
#include "sdf_asset_state.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "bill_object_api.h"
#include "bill_linked_value.h"
#include "par_cell_api.h"
#include "sdf_resource.h"
#include "sdf_gs_packet.h"
#include "pcp_vu0.h"
#include "sdf_vu_transform_api.h"
#include "ee_mmi.h"
#include "sdf_projection.h"

#include "sdf_asset_create.h"
extern s32 effEmitterDelayRandomState[];
#include "eff.h"
#include "eff_particle_emitter_variants.h"
#include "eff_template_scale_record.h"
#include "eff_particle_view.h"
#include "eff_look_at_ring.h"
#include "eff_track_poly_list.h"
#include "par_table.h"
#include "par_color_ramp.h"
#include "par_kind_api.h"
#include "eff_particle_record.h"
#include "eff_packet_params.h"
#include "fpu.h"

#define BILL_ENTRY_BYTES 0x14
#define BILL_VARIANT_MASK 0xFFFF
#define EFF_INSTANCE_BYTES 0x88
#define EFF_MATRIX_BYTES 0x40
#define EFF_PACKET_LIST_BYTES 0x20
#define EFF_TEMPLATE_TAIL_OFFSET 0x150
#define EFF_PACKET_INITIAL_TAG 0xF0000001

/* Header shared by the effect emitters that spawn a ring or spray of packets:
 * an origin, a sub-effect, a fade descriptor, jitter ranges and the packet
 * buffer. The kind-specific parameters follow at +0x150. */
extern void dds3Particle_BasicDelete(EffEmitterHead *owner);

/* Particle-style effect object and its per-record buffer entry. */

extern u32 dds3ParticleColorBlend(u32 color, u32 mask);

typedef struct EffInstance {
    f32 localMatrix[4][4]; /* 0x00: VU0 matrix initialized on clone. */
    f32 transform[4][4]; /* 0x40: VU0 matrix copied by dds3P2APolygonSetRotateMatrix. */
    billHandle_t *billboard; /* 0x80 */
    void *renderState; /* 0x84 */
} EffInstance;
typedef char EffInstance_size_check[(sizeof(EffInstance) == EFF_INSTANCE_BYTES) ? 1 : -1];
typedef char EffInstance_localMatrix_offset_check[
    ((u32)&((EffInstance *)0)->localMatrix == 0) ? 1 : -1];
typedef char EffInstance_transform_offset_check[
    ((u32)&((EffInstance *)0)->transform == EFF_MATRIX_BYTES) ? 1 : -1];
typedef char EffInstance_billboard_offset_check[
    ((u32)&((EffInstance *)0)->billboard == 0x80) ? 1 : -1];
typedef char EffInstance_renderState_offset_check[
    ((u32)&((EffInstance *)0)->renderState == 0x84) ? 1 : -1];

extern billHandle_t *hGeneralBillboard[];

#include "sdf_packet_alloc.h"


void effDrawGeneratedTextureQuad(sdfDrawTag_t *packet, EffGeneratedTextureDescriptor *source);

void effDrawCompositeTextureQuad(sdfDrawTag_t *packet, EffCompositeGsDescriptor *source);

void dds3Particle_BasicCreate(EffEmitterHead *effect);

void dds3Particle_SpiralAllBuffInit(EffTemplatePacketList *effect);




extern EffectConfig D_003AA884[];




void dds3Particle_SphereRingAllBuffInit(EffTemplatePacketList *effect);

/* Create a billboard sharing the indexed entry's resource. Word two of the
 * resource stores the reference count; the billHandle_t payload is not an emitter. */
billHandle_t *effCreateBillboardSharingIndexedResource(s32 index) {
    billHandle_t *effect = dds3BillboardCreate(D_003AA884[index].billboardKind, 0);
    BillChildPayload *resource = hGeneralBillboard[index]->child;
    s32 references = resource->refCount;

    effect->child = resource;
    resource->refCount = references + 1;
    return effect;
}

u32 dds3BillboardGeneralNumber(void) {
    return 0xf;
}

sdfTexHandle_t *dds3BillboardGetGeneralTex(s32 index) {
    return hGeneralBillboard[index]->child->texture;
}

void effCopyVector(void *dst, const void *src) {
    SDF_VU0_COPYVECTOR(dst, src);
}

void dds3BillboardSetRotate(billHandle_t *effect, float angle) {
    effect->rotationAngle = angle;
}

void dds3BillboardSetScale(billHandle_t *effect, float x, float y) {
    effect->childScaleX = x;
    effect->childScaleY = y;
}

void dds3BillboardSetRGBA(billHandle_t *effect, u32 value) {
    effect->childParam = value;
}

void billSetChildTextureQuad(billHandle_t *effect, const BillTextureQuad *textureQuad) {
    if (effect->kind == 0) {
        memcpy(&effect->child->uv, textureQuad, sizeof(*textureQuad));
    }
}

/* Narrow mode for the child kinds, or update each resolved animation entry. */
void dds3BillboardSetMode(billHandle_t *effect, s32 mode) {
    mode = (s16)mode;
    switch (effect->kind) {
    case 0:
    case 3:
        effect->requestedPacketListIndex = mode;
        break;
    case 1: {
        s32 entryCount = effect->entryCount;
        if (entryCount > 0) {
            s32 remaining = entryCount;
            BillOut *entries = effect->resolvedEntries;
            s32 index = 0;
            do {
                BillAnimationEntry *entry = entries[index].entry;
                u32 flags = entry->flags & ~BILL_ANIMATION_FLAG_PACKET_LIST_MASK;
                entry->flags = flags;
                if (mode == 2) {
                    entry->flags = flags | BILL_ANIMATION_FLAG_PACKET_LIST_2;
                } else if (mode == 3) {
                    entry->flags = flags | BILL_ANIMATION_FLAG_PACKET_LIST_3;
                }
                index++;
            } while (--remaining != 0);
        }
        break;
    }
    }
}



/* Store the variant bits in one child payload or every child of a list. */
void dds3BillboardSetCondition(billHandle_t *effect, s16 variant) {
    switch (effect->kind) {
    case 0:
        effect->child->variant = (u16)variant;
        break;
    case 1: {
        BillData *childList = effect->animationData;
        s32 remainingChildren = childList->childCount;
        BillChildPayload **childEntries = childList->children;
        BillChildPayload **childCursor;

        if (remainingChildren > 0) {
            childCursor = childEntries;
            do {
                (*childCursor)->variant = (u16)variant;
                childCursor++;
            } while (--remainingChildren != 0);
        }
        break;
    }
    }
}

sdfTexHandle_t *dds3BillboardGetTex(billHandle_t *effect) {
    if (effect->kind == 0) {
        return effect->child->texture;
    }
    return NULL;
}

u16 dds3BillboardGetType(billHandle_t *effect) {
    return effect->kind;
}

/* Store the low halfword through the kind-specific variant field. */
void dds3BillboardSetDrawType(billHandle_t *effect, s32 variantValue) {
    s32 maskedValue = variantValue & BILL_VARIANT_MASK;

    switch (effect->kind) {
    case 0:
        effect->child->variant = (u16)maskedValue;
        break;
    case 1:
        effect->pair.cameraFacingMode = maskedValue;
        break;
    }
}

u16 dds3BillboardGetDrawType(billHandle_t *effect) {
    switch (effect->kind) {
    case 0:
        return effect->child->variant;
    case 1:
        return effect->pair.cameraFacingMode;
    default:
        return 0;
    }
}

/* Replace the selected list entry only when its index changes. */
void dds3BillboardSetAnimNumber(billHandle_t *effect, u32 entryIndex) {
    if (effect->kind == 1 && effect->animationEntryIndex != entryIndex) {
        dds3Billboard_P2ASetNumber(effect, entryIndex);
    }
}

/* Read the selected entry for list billboards; other kinds have none. */
s32 dds3BillboardGetCurrentAnimNumber(billHandle_t *effect) {
    if (effect->kind == 1) {
        return effect->animationEntryIndex;
    }
    return 0;
}

s32 dds3BillboardGetAnimNumber(s32 billboard) {
    if (((billHandle_t *)billboard)->kind == 1) {
        return ((BillLinkedValue *)((billHandle_t *)billboard)->animationData->base)->value;
    }
    return 0;
}

/* Select the frame and make the next animation update advance immediately. */
void dds3BillboardSetAnimKey(billHandle_t *effect, u32 startFrame) {
    if (effect->kind == 1) {
        s32 entryCount = effect->entryCount;

        if (entryCount > 0) {
            BillOut *entry = effect->resolvedEntries;
            s32 remaining = entryCount;

            do {
                BillAnimationEntry *frameData = entry->entry;
                u32 wrappedFrame = startFrame % frameData->frameCount;
                remaining -= 1;
                entry->framesRemaining = 0;
                entry->frameIndex = wrappedFrame;
                entry++;
            } while (remaining != 0);
        }
    }
}

/* Select the frame and hold it for one animation update before advancing. */
void dds3BillboardSetAnimKey2(billHandle_t *effect, u32 startFrame) {
    if (effect->kind == 1) {
        s32 entryCount = effect->entryCount;

        if (entryCount > 0) {
            BillOut *entry = effect->resolvedEntries;
            s32 remaining = entryCount;

            do {
                BillAnimationEntry *frameData = entry->entry;
                u32 wrappedFrame = startFrame % frameData->frameCount;
                remaining -= 1;
                entry->framesRemaining = 1;
                entry->frameIndex = wrappedFrame;
                entry++;
            } while (remaining != 0);
        }
    }
}

/* Read the animation modulus of the first entry, if this is a list billboard. */
s32 dds3BillboardGetAnimFrame(const billHandle_t *effect) {
    if (effect->kind == 1) {
        const BillOut *entry = effect->resolvedEntries;
        const BillAnimationEntry *frame = entry->entry;
        return frame->frameCount;
    }
    return 0;
}

u16 dds3BillboardGetAnimState(const billHandle_t *effect) {
    if (effect->kind == 1) {
        return effect->animationActive;
    }
    return 0;
}


s32 dds3BillboardGetAnimType(s32 billboard) {
    if (((billHandle_t *)billboard)->kind == 1) {
        return ((billHandle_t *)billboard)->modeFlags;
    }
    return 0;
}

void dds3BillboardSetAnimColorAdvMode(billHandle_t *billboard) {
    if (billboard->kind == 1) {
        billboard->modeFlags |= 0x1000000;
    }
}

/* Kind 0 stores half the supplied width/height; other kinds remain unchanged. */
void dds3BillboardSetXYOffset(billHandle_t *billboard, float width, float height) {
    if (billboard->kind == 0) {
        BillChildPayload *child = billboard->child;
        child->halfWidth = width * 0.5f;
        child->halfHeight = height * 0.5f;
    }
}

extern u8 sdfViewMatrix[];
extern u8 sdfProjectionMatrix[];
extern u8 D_0037F650[];
extern u8 D_0037F660[];
extern u8 D_003AA9B0[];

/* vu0 routine: project position and position + scaled offset; return the reverse XY heading. */
f32 effComputeProjectedOffsetAngle(const void *position, const void *offset) {
    f32 projectedDelta[4];
    f32 projectedPosition[4];
    f32 projectedEndpoint[4];

    VU0_LOAD_MATRIX(sdfViewMatrix);
    sdfPostmultiplyVuMatrixFromMemory(sdfProjectionMatrix);
    SDF_VU0_LOADVECTOR(position);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    SDF_VU0_LOADVECTOR_B(D_0037F650);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(D_0037F660);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(projectedPosition);

    SDF_VU0_LOADVECTOR(offset);
    SDF_VU0_LOADVECTOR_B(D_003AA9B0);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_C_TO_B();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    SDF_VU0_LOADVECTOR_B(D_0037F650);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(D_0037F660);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(projectedEndpoint);

    projectedDelta[0] = projectedPosition[0] - projectedEndpoint[0];
    projectedDelta[1] = projectedPosition[1] - projectedEndpoint[1];
    projectedDelta[2] = 0.0f;
    SDF_VU0_LOADVECTOR(projectedDelta);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(projectedDelta);
    return sdfAtan2(projectedDelta[1], projectedDelta[0]);
}

extern u8 aViewTarget[];
extern u8 sdfViewTargetVector[];

/* vu0 routine: 1 - |dot(normalised eye - target, direction)|, how far the direction is from the view axis */
f32 dds3BillboardGetDirectionScale(void *unused, f32 *direction) {
    f32 dot;
    f32 alignment;

    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(sdfViewTargetVector);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(direction);
    SDF_VU0_INNERPRODUCT(dot);
    alignment = ffabsf(dot);
    return 1.0f - alignment;
}

/* Create the indexed billboard and render asset; only the second matrix is initialized. */
u8 *dds3P2APolygonCreate(s32 entryIndex) {
    EffInstance *instance = sdfChipAllocDirect(EFF_INSTANCE_BYTES);

    instance->billboard = dds3BillboardCreate(1, entryIndex);
    instance->renderState = sdfMatNew();
    sdfMatSetConstant(instance->renderState, 1.0f);
    EE_MMI_UNIT_MATRIX(instance->transform);
    return (u8 *)instance;
}

/* Retain a cloned billboard's shared data, create a fresh render asset, and reset both matrices. */
u8 *dds3P2APolygonCopy(EffInstance *source) {
    EffInstance *instance = sdfChipAllocDirect(EFF_INSTANCE_BYTES);

    instance->billboard = dds3BillboardCopy(source->billboard);
    instance->renderState = sdfMatNew();
    sdfMatSetConstant(instance->renderState, 1.0f);
    sdfMatSetDiffuse(instance->renderState, 0x80808080);
    sdfMatSetAmbient(instance->renderState, 0x80808080);
    sdfMatSetSpecular(instance->renderState, 0x80808080);
    EE_MMI_UNIT_MATRIX(instance->transform);
    EE_MMI_UNIT_MATRIX(instance->localMatrix);
    return (u8 *)instance;
}

void dds3P2APolygonDelete(EffInstance *instance) {
    sdfMatFreeDelayed((sdfMat_t *)instance->renderState);
    dds3BillboardDelete(instance->billboard);
    sdfChipFreeDirect(instance);
}

void dds3P2APolygonSetPosition(EffInstance *instance, void *position) {
    effCopyVector(instance->billboard, position);
}

void dds3P2APolygonSetScale(EffInstance *instance, f32 scale) {
    dds3BillboardSetScale(instance->billboard, scale, scale);
}

/* vu0 routine: copy the supplied 4x4 matrix to the instance's second matrix. */
void dds3P2APolygonSetRotateMatrix(EffInstance *instance, void *matrix) {
    VU0_LOAD_MATRIX(matrix);
    VU0_STORE_MATRIX(instance->transform);
}

void dds3P2APolygonSetRGBA(EffInstance *instance, u32 value) {
    dds3BillboardSetRGBA(instance->billboard, value);
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void dds3P2APolygonSetDirection(void *dst, void *src) {
    VU0_COPY_MATRIX(dst, src);
}

/* Kind 1 writes mode plus one/two entry values; untouched output words retain their contents. */
void dds3P2APolygonGetResource(EffInstance *instance, s32 *modeValues) {
    billHandle_t *billboard = instance->billboard;

    if (billboard->kind == 1) {
        u32 modeFlags = billboard->modeFlags;

        if (modeFlags & 0x40) {
            modeValues[0] = 2;
            modeValues[2] = (s32)dds3Billboard_P2AAnimDraw(billboard, billboard->resolvedEntries);
            modeValues[1] = (s32)dds3Billboard_P2AAnimDraw(billboard, billboard->resolvedEntries + 1);
        } else if (modeFlags & 0x80) {
            modeValues[0] = 3;
            modeValues[2] = (s32)dds3Billboard_P2AAnimDraw(billboard, billboard->resolvedEntries);
            modeValues[1] = (s32)dds3Billboard_P2AAnimDraw(billboard, billboard->resolvedEntries + 1);
        } else {
            modeValues[0] = 0;
            modeValues[1] = (s32)dds3Billboard_P2AAnimDraw(billboard, billboard->resolvedEntries);
        }
    }
}

extern u32 D_003AA9C0[],D_003AA9D0[];
extern SdfPoolNode *D_003AA9E0[];
extern void *effPkt3DPrimMakePoly(EffPacketParams *);
extern void sdf3DPrimLinkMat(sdfDrawTag_t *packetList, sdfMat_t *asset, s32 (*allocatePacket)(s32));
extern void sdf3DPrimLinkTm(sdfDrawTag_t *packetList, s32 (*allocatePacket)(s32));

/* vu0 routine: native instance matrix composition before billboard geometry. */

void dds3P2APolygonDraw(EffInstance *instance) {
    EffPacketParams params;
    s32 modeValues[3];
    f32 matrix[4][4];
    f32 positions[4][4];
    f32 uv[4][2];
    f32 secondaryUv[4][2];
    s32 list;
    BillChildPayload *child;
    sdfTexHandle_t *texture;
    SdfPoolNode *surface;
    f32 x, y;
    f32 scaleX, scaleY;
    f32 cornerX, cornerY;
    f32 cosine, sine;
    f32 width, height;

    if (!dds3BillboardGetAnimState(instance->billboard)) {
        return;
    }
    dds3P2APolygonGetResource(instance, modeValues);
    list = sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit((sdfDrawTag_t *)list);
    VU0_LOAD_MATRIX(instance->transform);
    SDF_VU0_LOADMATRIX_B(instance->localMatrix);
    sdfComposeVuMatrixFromRegisters();
    VU0_STORE_MATRIX_UNCLOBBERED(matrix);
    matrix[3][0] = instance->billboard->position[0];
    matrix[3][1] = instance->billboard->position[1];
    matrix[3][2] = instance->billboard->position[2];
    VU0_LOAD_MATRIX(matrix);
    sdf3DPrimLinkTm((sdfDrawTag_t *)list, 0);
    memset(&params, 0, sizeof(params));
    params.primitive = 0x4000;
    params.parameters = D_003AA9C0;
    params.positions = (u128 *)positions;
    params.colors = D_003AA9D0;
    params.texcoords = &uv[0][0];
    params.color = instance->billboard->childParam;
    params.parameterCount = 2;
    params.vertexCount = 4;
    child = (BillChildPayload *)modeValues[1];
    scaleX = instance->billboard->childScaleX;
    scaleY = instance->billboard->childScaleY;
    x = child->x * scaleX;
    y = child->y * scaleY;
    width = child->halfWidth * scaleX;
    height = child->halfHeight * scaleY;
    cosine = sdfCos(instance->billboard->rotationAngle);
    sine = sdfSin(instance->billboard->rotationAngle);
    cornerX = x - width;
    cornerY = y - height;
    positions[0][0] = cornerX * cosine - cornerY * sine;
    positions[0][1] = cornerX * sine + cornerY * cosine;
    positions[0][2] = 0.0f;
    cornerX = x + width;
    cornerY = y - height;
    positions[1][0] = cornerX * cosine - cornerY * sine;
    positions[1][1] = cornerX * sine + cornerY * cosine;
    positions[1][2] = 0.0f;
    cornerX = x + width;
    cornerY = y + height;
    positions[3][0] = cornerX * cosine - cornerY * sine;
    positions[3][1] = cornerX * sine + cornerY * cosine;
    positions[3][2] = 0.0f;
    cornerX = x - width;
    cornerY = y + height;
    positions[2][0] = cornerX * cosine - cornerY * sine;
    positions[2][1] = cornerX * sine + cornerY * cosine;
    positions[2][2] = 0.0f;
    width = 1.0f / (f32)(child->texture->width * 16);
    height = 1.0f / (f32)(child->texture->height * 16);
    texture = child->texture;
    uv[0][0] = child->uv.components[0] * width;
    uv[0][1] = child->uv.components[1] * height;
    uv[1][0] = child->uv.components[2] * width;
    uv[1][1] = child->uv.components[3] * height;
    uv[2][0] = child->uv.components[6] * width;
    uv[2][1] = child->uv.components[7] * height;
    uv[3][0] = child->uv.components[4] * width;
    uv[3][1] = child->uv.components[5] * height;
    sdfSetAssetPrimaryTextureAddress((sdfMat_t *)instance->renderState, texture);
    switch ((u32)modeValues[0]) {
    case 2:
    case 3:
        child = (BillChildPayload *)modeValues[2];
        scaleX = instance->billboard->childScaleX;
        scaleY = instance->billboard->childScaleY;
        x = child->x * scaleX;
        y = child->y * scaleY;
        width = child->halfWidth * scaleX;
        height = child->halfHeight * scaleY;
        params.primitive |= 0x1000;
        params.extraTexcoords = (u32 *)secondaryUv;
        cosine = sdfCos(instance->billboard->rotationAngle);
        sine = sdfSin(instance->billboard->rotationAngle);
        cornerX = x - width;
        cornerY = y - height;
        positions[0][0] = cornerX * cosine - cornerY * sine;
        positions[0][1] = cornerX * sine + cornerY * cosine;
        positions[0][2] = 0.0f;
        cornerX = x + width;
        cornerY = y - height;
        positions[1][0] = cornerX * cosine - cornerY * sine;
        positions[1][1] = cornerX * sine + cornerY * cosine;
        positions[1][2] = 0.0f;
        cornerX = x + width;
        cornerY = y + height;
        positions[3][0] = cornerX * cosine - cornerY * sine;
        positions[3][1] = cornerX * sine + cornerY * cosine;
        positions[3][2] = 0.0f;
        cornerX = x - width;
        cornerY = y + height;
        positions[2][0] = cornerX * cosine - cornerY * sine;
        positions[2][1] = cornerX * sine + cornerY * cosine;
        positions[2][2] = 0.0f;
        texture = child->texture;
        width = (f32)(texture->width * 16);
        height = (f32)(texture->height * 16);
        secondaryUv[0][0] = child->uv.components[0] / width;
        secondaryUv[0][1] = child->uv.components[1] / height;
        secondaryUv[1][0] = child->uv.components[2] / width;
        secondaryUv[1][1] = child->uv.components[3] / height;
        secondaryUv[2][0] = child->uv.components[6] / width;
        secondaryUv[2][1] = child->uv.components[7] / height;
        secondaryUv[3][0] = child->uv.components[4] / width;
        secondaryUv[3][1] = child->uv.components[5] / height;
        sdfMatSetMultiTex((sdfMat_t *)instance->renderState, texture);
        break;
    }
    sdf3DPrimLinkMat((sdfDrawTag_t *)list, (sdfMat_t *)instance->renderState, 0);
    sdfDrawTagLink((sdfDrawTag_t *)list, (u32)effPkt3DPrimMakePoly(&params));
    surface = D_003AA9E0[instance->billboard->requestedPacketListIndex];
    surface->append(surface, (sdfDrawTag_t *)list);
}

extern void sdfBillBoardMakeTMPacket(u8 *, const f32 *);
extern void sdfBillBoardMake1(u8 *, const f32 *, u32, const BillTextureQuad *, const f32 *);

void effDrawGeneratedTextureQuad(sdfDrawTag_t *list, EffGeneratedTextureDescriptor *source) {
    f32 matrix[16] __attribute__((aligned(16)));
    BillTextureQuad uv;
    f32 corners[4][2];
    SdfGsTexturePacket *texturePacket;
    u64 *packet;
    s16 width, height;
    f32 halfWidth, halfHeight;
    f32 cosine, sine;
    f32 cornerX, cornerY;

    texturePacket = (SdfGsTexturePacket *)sdfGrAllocPacketBuff(sizeof(SdfGsTexturePacket));
    texturePacket->header.dmaTag = 4;
    texturePacket->header.vifCommands = 0x5000000410000000ULL;
    texturePacket->header.gifTag = 0x1000000000008003ULL;
    texturePacket->header.gifRegisters = SDF_GIF_REGISTER_AD;
    texturePacket->sampling.value = sdfTexGetTex1Reg(source->texture);
    texturePacket->sampling.registerId = SDF_GS_TEX1_1;
    texturePacket->texture.value = sdfTexGetTex0Reg(source->texture);
    texturePacket->texture.registerId = SDF_GS_TEX0_1;
    texturePacket->clamp.value = sdfTexGetClampReg(source->texture);
    texturePacket->clamp.registerId = SDF_GS_CLAMP_1;
    sdfDrawTagLink(list, (u32)texturePacket);

    EE_MMI_UNIT_MATRIX(matrix);
    packet = (u64 *)sdfGrAllocPacketBuff(0x38);
    sdfBillBoardMakeTMPacket((u8 *)packet, matrix);
    sdfDrawTagLink(list, (u32)packet);

    width = (u16)source->texture->width << 4;
    height = (u16)source->texture->height << 4;
    halfWidth = (f32)(width >> 5) * 0.0625f * source->scaleX;
    halfHeight = (f32)(height >> 5) * 0.0625f * source->scaleY;
    uv.components[0] = 0;
    uv.components[1] = 0;
    uv.components[2] = width;
    uv.components[3] = 0;
    uv.components[4] = width;
    uv.components[5] = height;
    uv.components[6] = 0;
    uv.components[7] = height;
    if (source->angle == 0.0f) {
        corners[0][0] = -halfWidth;
        corners[0][1] = -halfHeight;
        corners[1][0] = halfWidth;
        corners[1][1] = -halfHeight;
        corners[2][0] = halfWidth;
        corners[2][1] = halfHeight;
        corners[3][0] = -halfWidth;
        corners[3][1] = halfHeight;
    } else {
        cosine = sdfCos(source->angle);
        sine = sdfSin(source->angle);
        cornerX = -halfWidth;
        cornerY = -halfHeight;
        corners[0][0] = cornerX * cosine - cornerY * sine;
        corners[0][1] = cornerX * sine + cornerY * cosine;
        cornerX = halfWidth;
        cornerY = -halfHeight;
        corners[1][0] = cornerX * cosine - cornerY * sine;
        corners[1][1] = cornerX * sine + cornerY * cosine;
        cornerX = halfWidth;
        cornerY = halfHeight;
        corners[2][0] = cornerX * cosine - cornerY * sine;
        corners[2][1] = cornerX * sine + cornerY * cosine;
        cornerX = -halfWidth;
        cornerY = halfHeight;
        corners[3][0] = cornerX * cosine - cornerY * sine;
        corners[3][1] = cornerX * sine + cornerY * cosine;
    }

    packet = (u64 *)sdfGrAllocPacketBuff(0x80);
    sdfBillBoardMake1((u8 *)packet, source->position, source->color, &uv, &corners[0][0]);
    sdfDrawTagLink(list, (u32)packet);
}

/* Allocate/init a packet list, generate its texture payload, then append it to the draw surface. */
void effSubmitGeneratedTexturePacket(SdfPoolNode *surface, EffGeneratedTextureDescriptor *source) {
    sdfDrawTag_t *packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EFF_PACKET_LIST_BYTES);

    sdfDrawTagInit(packetList);
    effDrawGeneratedTextureQuad(packetList, source);
    surface->append(surface, packetList);
}

extern KwlnFrameDrawBuffer kwlnFrameDrawPacketRecords[];
extern void sdfBuildMaskedFrameRegisterReference(const u8 *, SdfDmaReferenceChainPacket *, s32);
extern void sdfBuildFrameRegisterReference(const u8 *, SdfDmaReferenceChainPacket *, s32);
extern void sdfMultiBillBoardMake1(u8 *, const f32 *, u32, u32,
                        const BillTextureQuad *, const BillTextureQuad *, const f32 *);

void effDrawCompositeTextureQuad(sdfDrawTag_t *list, EffCompositeGsDescriptor *source) {
    f32 matrix[16] __attribute__((aligned(16)));
    f32 corners[4][2];
    sdfPrimTransPacket_t *state;
    SdfGsTexturePacket *textureState;
    u8 *geometry;
    SdfDmaReferenceChainPacket *framePacket;
    u64 restoreBlend;
    f32 halfWidth, halfHeight;
    f32 cosine, sine;
    f32 cornerX, cornerY;

    framePacket = (SdfDmaReferenceChainPacket *)sdfGrAllocPacketBuff(0x40);
    sdfBuildMaskedFrameRegisterReference(kwlnFrameDrawPacketRecords[dds3GetFrameID()].baseBytes, framePacket, 0);
    sdfDrawTagLinkRefGif(list, (u32)framePacket);

    state = (sdfPrimTransPacket_t *)sdfGrAllocPacketBuff(0x40);
    state->header.dmaTag = 3;
    state->header.vifCommands = 0x5000000310000000ULL;
    state->header.gifTag = 0x1000000000008002ULL;
    state->header.gifRegisters = SDF_GIF_REGISTER_AD;
    state->registers.test.value = 0x51001;
    state->registers.test.registerId = SDF_GS_TEST_1;
    state->registers.alpha.value = SDF_GS_ALPHA_INTERPOLATE_SOURCE_ALPHA;
    state->registers.alpha.registerId = SDF_GS_ALPHA_1;
    sdfDrawTagLink(list, (u32)state);

    state = (sdfPrimTransPacket_t *)sdfGrAllocPacketBuff(0x40);
    state->header.dmaTag = 3;
    state->header.vifCommands = 0x5000000310000000ULL;
    state->header.gifTag = 0x1000000000008002ULL;
    state->header.gifRegisters = SDF_GIF_REGISTER_AD;
    state->registers.test.value = 0x51001;
    state->registers.test.registerId = SDF_GS_TEST_2;
    switch (source->blendMode) {
    case 2:
        state->registers.alpha.value = SDF_GS_ALPHA_ADD_DESTINATION_ALPHA;
        restoreBlend = SDF_GS_ALPHA_ADD_SOURCE_ALPHA;
        break;
    case 3:
        state->registers.alpha.value = SDF_GS_ALPHA_SUBTRACT_DESTINATION_ALPHA;
        restoreBlend = SDF_GS_ALPHA_SUBTRACT_SOURCE_ALPHA;
        break;
    case 0:
    case 1:
        state->registers.alpha.value = SDF_GS_ALPHA_INTERPOLATE_DESTINATION_ALPHA;
        restoreBlend = SDF_GS_ALPHA_INTERPOLATE_SOURCE_ALPHA;
        break;
    default:
        restoreBlend = 0;
        break;
    }
    state->registers.alpha.registerId = SDF_GS_ALPHA_2;
    sdfDrawTagLink(list, (u32)state);

    textureState = (SdfGsTexturePacket *)sdfGrAllocPacketBuff(0x50);
    textureState->header.dmaTag = 4;
    textureState->header.vifCommands = 0x5000000410000000ULL;
    textureState->header.gifTag = 0x1000000000008003ULL;
    textureState->header.gifRegisters = SDF_GIF_REGISTER_AD;
    textureState->sampling.value = sdfTexGetTex1Reg(source->secondaryTexture);
    textureState->sampling.registerId = SDF_GS_TEX1_1;
    textureState->texture.value = sdfTexGetTex0Reg(source->secondaryTexture);
    textureState->texture.registerId = SDF_GS_TEX0_1;
    textureState->clamp.value = source->secondaryClamp;
    textureState->clamp.registerId = SDF_GS_CLAMP_1;
    sdfDrawTagLink(list, (u32)textureState);

    textureState = (SdfGsTexturePacket *)sdfGrAllocPacketBuff(0x50);
    textureState->header.dmaTag = 4;
    textureState->header.vifCommands = 0x5000000410000000ULL;
    textureState->header.gifTag = 0x1000000000008003ULL;
    textureState->header.gifRegisters = SDF_GIF_REGISTER_AD;
    textureState->sampling.value = sdfTexGetTex1Reg(source->primaryTexture);
    textureState->sampling.registerId = SDF_GS_TEX1_2;
    textureState->texture.value = sdfTexGetTex0Reg(source->primaryTexture);
    textureState->texture.registerId = SDF_GS_TEX0_2;
    textureState->clamp.value = source->primaryClamp;
    textureState->clamp.registerId = SDF_GS_CLAMP_2;
    sdfDrawTagLink(list, (u32)textureState);

    EE_MMI_UNIT_MATRIX(matrix);
    geometry = (u8 *)sdfGrAllocPacketBuff(0x38);
    sdfBillBoardMakeTMPacket((u8 *)geometry, matrix);
    sdfDrawTagLink(list, (u32)geometry);

    halfWidth = (f32)((s16)((u16)source->primaryTexture->width << 4) >> 5) * 0.0625f * source->scaleX;
    halfHeight = (f32)((s16)((u16)source->primaryTexture->height << 4) >> 5) * 0.0625f * source->scaleY;
    if (source->angle == 0.0f) {
        corners[0][0] = -halfWidth;
        corners[0][1] = -halfHeight;
        corners[1][0] = halfWidth;
        corners[1][1] = -halfHeight;
        corners[2][0] = halfWidth;
        corners[2][1] = halfHeight;
        corners[3][0] = -halfWidth;
        corners[3][1] = halfHeight;
    } else {
        cosine = sdfCos(source->angle);
        sine = sdfSin(source->angle);
        cornerX = -halfWidth;
        cornerY = -halfHeight;
        corners[0][0] = cornerX * cosine - cornerY * sine;
        corners[0][1] = cornerX * sine + cornerY * cosine;
        cornerX = halfWidth;
        cornerY = -halfHeight;
        corners[1][0] = cornerX * cosine - cornerY * sine;
        corners[1][1] = cornerX * sine + cornerY * cosine;
        cornerX = halfWidth;
        cornerY = halfHeight;
        corners[2][0] = cornerX * cosine - cornerY * sine;
        corners[2][1] = cornerX * sine + cornerY * cosine;
        cornerX = -halfWidth;
        cornerY = halfHeight;
        corners[3][0] = cornerX * cosine - cornerY * sine;
        corners[3][1] = cornerX * sine + cornerY * cosine;
    }

    geometry = (u8 *)sdfGrAllocPacketBuff(0x90);
    sdfMultiBillBoardMake1((u8 *)geometry, source->position, source->color, source->color,
                  &source->primaryUv, &source->secondaryUv, &corners[0][0]);
    sdfDrawTagLink(list, (u32)geometry);

    framePacket = (SdfDmaReferenceChainPacket *)sdfGrAllocPacketBuff(0x40);
    sdfBuildFrameRegisterReference(kwlnFrameDrawPacketRecords[dds3GetFrameID()].baseBytes, framePacket, 0);
    sdfDrawTagLinkRefGif(list, (u32)framePacket);

    state = (sdfPrimTransPacket_t *)sdfGrAllocPacketBuff(0x40);
    state->header.dmaTag = 3;
    state->header.vifCommands = 0x5000000310000000ULL;
    state->header.gifTag = 0x1000000000008002ULL;
    state->header.gifRegisters = SDF_GIF_REGISTER_AD;
    state->registers.test.value = 0x71801;
    state->registers.test.registerId = SDF_GS_TEST_1;
    state->registers.alpha.value = restoreBlend;
    state->registers.alpha.registerId = SDF_GS_ALPHA_1;
    sdfDrawTagLink(list, (u32)state);
}


/* Allocate/init a packet list, generate its composite GS payload, then append it to the draw surface. */
void effSubmitCompositeGsPacket(SdfPoolNode *surface, EffCompositeGsDescriptor *source) {
    sdfDrawTag_t *packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EFF_PACKET_LIST_BYTES);

    sdfDrawTagInit(packetList);
    effDrawCompositeTextureQuad(packetList, source);
    surface->append(surface, packetList);
}

/* Seed the generator used for per-particle launch delays. */
void dds3ParticleManagerInit(void) {
    sdfRandomizeByTime((EffRandState *)effEmitterDelayRandomState);
}

/* Effect-manager startup hook for emitter work; retail currently does no work. */
void dds3ParticleManagerExit(void) {
}

/* Allocate records followed by their owner tail; clear only two native state words per record. */
EffectBufferTail *dds3Particle_PhysicsCreate(s32 recordCount) {
    s32 recordBytes = recordCount * sizeof(EffectBufferRecord);
    sdfMemHandle_t *allocationHandle = sdfMemAllocDirect(recordBytes + sizeof(EffectBufferTail));
    EffectBufferRecord *recordCursor = (void *)sdfMemLock(allocationHandle);
    EffectBufferTail *bufferTail = (EffectBufferTail *)((u8 *)recordCursor + recordBytes);

    bufferTail->allocation = allocationHandle;
    bufferTail->records = recordCursor;
    if (recordCount > 0) {
        s32 remaining = recordCount;
        do {
            remaining--;
            recordCursor->age = 0;
            recordCursor->color = 0;
            recordCursor++;
        } while (remaining != 0);
    }
    return bufferTail;
}

void dds3Particle_PhysicsDelete(EffectBufferTail *buffer) {
    sdfMemFreeDirect(buffer->allocation);
}

struct ParColorRamp;
extern void dds3ParticleSetColorTable(struct ParColorRamp *, s32);
extern dds3Particle_BlurBuff_t *dds3Particle_BlurCreate(u32, u16, billHandle_t **);
extern u32 dds3Particle_TrackPolyGetRecTiming(u16);
extern void dds3TrackPolyVCSet(void *, s32, void *, void *);

/* Allocate drawing resources and reset the emitter's common runtime state. */
/* The subrecord bitfield keeps native int-alias dependencies; see
 * docs/idioms.md, "Trace-justified alias sets: emitter subrecord count". */
void dds3Particle_BasicCreate(EffEmitterHead *owner) {
    EffTrackPolyParams params;

    owner->templateSize = 0x150;
    owner->buffer = dds3Particle_PhysicsCreate(owner->particleCount);
    if (owner->billboardResource != -1) {
        owner->billboard = effCreateBillboardSharingIndexedResource(owner->billboardResource);
        dds3BillboardSetScale(owner->billboard, owner->scale[0], owner->scale[1]);
        dds3BillboardSetMode(owner->billboard, owner->drawKind);
    }
    dds3ParticleSetColorTable((struct ParColorRamp *)owner->fade, owner->lifetimeFrames);
    switch (owner->sub.kind) {
    case PAR_KIND_STATE_HISTORY:
        owner->sub.payload.history.table = dds3Particle_BlurCreate(owner->particleCount,
                                             owner->sub.templateEntryCount,
                                             &owner->billboard);
        break;
    case PAR_KIND_STATE_BILLBOARD_CROSS_STRIP:
        owner->sub.payload.crossStrip.drawSystem = dds3TrackPolyCreate(
            owner->particleCount, owner->sub.templateEntryCount,
            dds3Particle_TrackPolyGetRecTiming(owner->sub.templateEntryCount), 0);
        dds3TrackPolyVCSet(owner->sub.payload.crossStrip.drawSystem, 1,
                       owner->sub.payload.crossStrip.dispatchParameter, owner->sub.payload.crossStrip.dispatchParameter);
        dds3TrackPolySetMode(owner->sub.payload.crossStrip.drawSystem, (u16)owner->drawKind);
        break;
    case PAR_KIND_STATE_BILLBOARD_CROSS_TRIANGLE:
        owner->sub.payload.crossTriangle.drawSystem = dds3TrackPolyCreate(
            owner->particleCount, owner->sub.templateEntryCount,
            dds3Particle_TrackPolyGetRecTiming(owner->sub.templateEntryCount), 1);
        dds3TrackPolyVCSet(owner->sub.payload.crossTriangle.drawSystem, 1,
                       owner->sub.payload.crossTriangle.dispatchParameter, owner->sub.payload.crossStrip.drawSystem);
        dds3TrackPolySetMode(owner->sub.payload.crossTriangle.drawSystem, (u16)owner->drawKind);
        break;
    case PAR_KIND_STATE_TRACK_POLYGON_CROSS_AXES:
        /* These tracks receive endpoints directly; model sampling is unused. */
        params.sampleInterval = 1;
        params.historyLength = owner->sub.templateEntryCount;
        params.unk1C = 10;
        params.kind = (u16)owner->drawKind;
        params.gradientColors[0] = owner->sub.payload.trackPolygon.firstGradientColor;
        params.gradientColors[1] = owner->sub.payload.trackPolygon.firstGradientColor;
        params.gradientColors[2] = owner->sub.payload.trackPolygon.secondGradientColor;
        params.gradientColors[3] = owner->sub.payload.trackPolygon.secondGradientColor;
        owner->sub.payload.trackPolygon.modelList =
            Eff_ModelTrackPolyListCreate(&params, owner->particleCount);
        break;
    }
    owner->pendingRestartSteps = owner->restartStepCount;
    VU0_LOAD_MATRIX(owner->matrix);
    VU0_STORE_MATRIX(owner->backupMatrix);
    owner->active = 1;
    owner->colorMask = 0x80808080;
}

/* Release the selected sub-effect resource, billboard and packet-buffer
 * allocation described by the common emitter prefix. */
void dds3Particle_BasicDelete(EffEmitterHead *owner) {
    switch (owner->sub.kind) {
    case PAR_KIND_STATE_HISTORY:
        dds3Particle_BlurDelete(owner->sub.payload.history.table);
        break;
    case PAR_KIND_STATE_BILLBOARD_CROSS_STRIP:
        dds3TrackPolyDelete(owner->sub.payload.crossStrip.drawSystem);
        break;
    case PAR_KIND_STATE_BILLBOARD_CROSS_TRIANGLE:
        dds3TrackPolyDelete(owner->sub.payload.crossTriangle.drawSystem);
        break;
    case PAR_KIND_STATE_TRACK_POLYGON_CROSS_AXES:
        Eff_ModelTrackPolyListDelete(owner->sub.payload.trackPolygon.modelList);
        break;
    }
    dds3BillboardDelete(owner->billboard);
    dds3Particle_PhysicsDelete(owner->buffer);
}

/* Stagger native tags in period-sized groups; a nonempty buffer requires a nonzero period. */
void dds3Particle_SpiralAllBuffInit(EffTemplatePacketList *effect) {
    EffectBufferRecord *packetCursor;
    u32 packetIndex;
    u32 nextPacketIndex;
    s32 packetTag;

    packetTag = EFF_PACKET_INITIAL_TAG;
    packetIndex = 0;
    packetCursor = effect->head.buffer->records;
    if (effect->head.particleCount != 0) {
        do {
            packetCursor->age = packetTag;
            nextPacketIndex = packetIndex + 1;
            packetCursor = packetCursor + 1;
            if (nextPacketIndex % effect->tailWords[0] == 0) {
                packetTag = packetTag - effect->decayStep;
            }
            packetIndex = nextPacketIndex;
        } while (packetIndex < effect->head.particleCount);
    }
}

void dds3Particle_SpiralSetScale(float scale, EffTemplatePacketList *effect) {
    effect->head.scale[0] = effect->head.scale[0] * scale;
    effect->head.scale[1] = effect->head.scale[1] * scale;
    effect->head.scale[2] = effect->head.scale[2] * scale;
    effect->recordScale = effect->recordScale * scale;
    effect->tailValues[1] = effect->tailValues[1] * scale;
    effect->tailValues[3] = effect->tailValues[3] * scale;
}

/* Clone the template prefix and appended tail, then allocate its packet records. */
s32 dds3Particle_SpiralCreate(EffTemplatePacketList *source) {
    EffEmitterHead *clone = sdfChipAllocDirect(0x180);
    s32 tailBytes = 0x30;

    memset(clone, 0, 0x180);
    memcpy(clone, source, source->head.templateSize);
    memcpy((u8 *)clone + EFF_TEMPLATE_TAIL_OFFSET, (u8 *)source + source->head.templateSize, tailBytes);
    dds3Particle_BasicCreate((EffEmitterHead *)clone);
    dds3Particle_SpiralAllBuffInit((EffTemplatePacketList *)clone);
    return (s32)clone;
}

void dds3Particle_SpiralDelete(u32 effect) {
    dds3Particle_BasicDelete((EffEmitterHead *)effect);
    sdfChipFreeDirect(effect);
}


typedef struct EffPacket {
    f32 pos[4];   /* 0x00 */
    f32 vel[4];   /* 0x10 */
    s32 age;      /* 0x20 */
    u32 color;    /* 0x24 */
    f32 speed;    /* 0x28 */
    f32 spin;     /* 0x2C */
    f32 f30;
    f32 f34;
    f32 f38;
    f32 f3C;
} EffPacket;

extern EffRandState effDefaultRandomState[];

typedef struct EffRingEmitter {
    EffEmitterHead head;
    u8 flag150;
    u8 flag151;
    u8 pad152[6];
    f32 radius;      /* 0x158 */
    u32 period;      /* 0x15C */
    f32 f160;
    f32 f164;
    f32 f168;
    f32 f16C;
    f32 radiusRange; /* 0x170 */
    f32 speedRange;  /* 0x174 */
} EffRingEmitter;

/* Seed one ring packet's phase, radius and vertical motion, then its subeffect. */
void dds3Particle_SpiralBuffInit(EffRingEmitter *effect, u32 packetIndex) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 phaseIndex;
    f32 phase;
    f32 jitter;

    packet += packetIndex;
    packet->color = 0;
    packet->age = 0;
    phase = 0.0f;
    phaseIndex = packetIndex % effect->period;
    if (phaseIndex != 0) {
        phase = (3.14159265f * 2.0f) / effect->period * phaseIndex;
    }
    packet->f30 = phase;
    jitter = effect->radiusRange;
    packet->f34 = effect->radius * (dds3GetRandFloatA(effDefaultRandomState) * jitter + (1.0f - jitter));
    jitter = effect->speedRange;
    packet->vel[0] = effect->f168 * (dds3GetRandFloatA(effDefaultRandomState) * jitter + (1.0f - jitter));
    packet->f38 = effect->f160;
    packet->pos[0] = sdfCos(phase) * effect->radius;
    packet->pos[1] = 0;
    packet->pos[2] = sdfSin(phase) * effect->radius;
    VU0_LOAD_MATRIX(effect->head.matrix);
    SDF_VU0_LOADVECTOR(packet->pos);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR(packet->pos);
    packet->f3C = 0;
    packet->pos[0] += effect->head.origin[0];
    packet->pos[1] += effect->head.origin[1];
    packet->pos[2] += effect->head.origin[2];
    jitter = effect->head.speedJitter;
    packet->speed = effect->head.scale[0] * (dds3GetRandFloatA(effDefaultRandomState) * jitter + (1.0f - jitter));
    jitter = effect->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (dds3GetRandFloatA(effDefaultRandomState) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        packet->spin = 0;
    }
    parDispatchKindInit(&effect->head.sub, packetIndex);
}

/* Advance ring motion and ages; repeat by tagging packets for the next update.
 * completedCount counts expired nonrepeating packets in this update only. */
/* vf12 keeps the packet's previous position and vf10 the new one for
 * parDispatchKindUpdate, which reads them as implicit arguments. */
void dds3Particle_SpiralDraw(EffRingEmitter *effect) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 subeffectKind = effect->head.sub.kind;
    f32 origin[4];
    f32 offset[4];
    f32 verticalScale;
    f32 phaseStep;
    s32 completedCount;
    s32 repeatEnabled;
    s32 packetCount;
    s32 lifetimeFrames;
    s32 packetIndex;

    dds3Particle_EffectInit(&effect->head.sub);
    VU0_LOAD_MATRIX(effect->head.matrix);
    phaseStep = effect->f164 * (3.14159265f / 180.0f);
    verticalScale = effect->f16C / 100.0f + 1.0f;
    completedCount = 0;
    packetCount = effect->head.particleCount;
    lifetimeFrames = effect->head.lifetimeFrames;
    repeatEnabled = effect->flag151;
    SDF_VU0_COPYVECTOR(origin, effect->head.origin);
    for (packetIndex = 0; packetIndex < packetCount; packetIndex++, packet++) {
        s32 age = packet->age;

        if (age == EFF_PACKET_INITIAL_TAG) {
            dds3Particle_SpiralBuffInit(effect, packetIndex);
            age = packet->age;
        }
        if (age >= 0) {
            f32 theta;
            f32 radius;

            /* dds3Particle_BasicCreate initializes this opaque slot as a ParColorRamp. */
            packet->color = dds3ParticleColorAnim((ParColorRamp *)effect->head.fade, packet->color, age);
            packet->color = dds3ParticleColorBlend(packet->color, effect->head.colorMask);
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR_C(packet);
            }
            packet->f34 += (packet->vel[0] - effect->radius) / lifetimeFrames;
            packet->f30 += phaseStep;
            if (packet->f34 < 0) {
                packet->f34 = 0;
            }
            packet->f3C += packet->f38;
            theta = packet->f30;
            radius = packet->f34;
            offset[0] = sdfCos(theta) * radius;
            offset[1] = packet->f3C;
            offset[2] = sdfSin(theta) * radius;
            SDF_VU0_LOADVECTOR(offset);
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_STOREVECTOR(offset);
            packet->pos[0] = origin[0] + offset[0];
            packet->pos[1] = origin[1] + offset[1];
            packet->pos[2] = origin[2] + offset[2];
            packet->f38 *= verticalScale;
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR(packet);
                parDispatchKindUpdate(&effect->head.sub, packetIndex, packet->color, packet->speed);
            }
        }
        age++;
        if (age >= lifetimeFrames) {
            if (repeatEnabled) {
                age = EFF_PACKET_INITIAL_TAG;
            } else {
                completedCount++;
                parDispatchKindInit(&effect->head.sub, packetIndex);
                if (completedCount >= packetCount) {
                    effect->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

/* Initialize only the native age/tag word of each packet record. */
void dds3Particle_SmokeAllBuffInit(EffTemplatePacketList *effect) {
    EffectBufferRecord *packetCursor;
    u32 packetIndex;

    packetIndex = 0;
    packetCursor = effect->head.buffer->records;
    if (effect->head.particleCount != 0) {
        do {
            packetCursor->age = EFF_PACKET_INITIAL_TAG;
            packetIndex = packetIndex + 1;
            packetCursor = packetCursor + 1;
        } while (packetIndex < effect->head.particleCount);
    }
}

void dds3Particle_SmokeSetScale(float scale, EffTemplatePacketList *effect) {
    effect->head.scale[0] = effect->head.scale[0] * scale;
    effect->head.scale[1] = effect->head.scale[1] * scale;
    effect->head.scale[2] = effect->head.scale[2] * scale;
    effect->recordScale = effect->recordScale * scale;
    effect->tailValues[0] = effect->tailValues[0] * scale;
    effect->tailValues[2] = effect->tailValues[2] * scale;
}

/* Clone the disc prefix/tail, allocate packet records, and initialize their native age tags. */
s32 dds3Particle_SmokeCreate(EffTemplatePacketList *source) {
    EffEmitterHead *clone = sdfChipAllocDirect(0x180);
    s32 tailBytes = 0x30;

    memset(clone, 0, 0x180);
    memcpy(clone, source, source->head.templateSize);
    memcpy((u8 *)clone + EFF_TEMPLATE_TAIL_OFFSET, (u8 *)source + source->head.templateSize, tailBytes);
    dds3Particle_BasicCreate((EffEmitterHead *)clone);
    dds3Particle_SmokeAllBuffInit((EffTemplatePacketList *)clone);
    return (s32)clone;
}

void dds3Particle_SmokeDelete(u32 effect) {
    dds3Particle_BasicDelete((EffEmitterHead *)effect);
    sdfChipFreeDirect(effect);
}

/* Seed a random planar position and direction; radialDistance may be negative. */
void dds3Particle_SmokeBuffInit(EffDiscEmitter *effect, u32 packetIndex) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    f32 directionVector[4];
    f32 radialDistance;
    f32 jitter;

    packet += packetIndex;
    radialDistance = effect->radius * ((dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f);
    directionVector[0] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
    directionVector[1] = 0;
    directionVector[2] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
    SDF_VU0_LOADVECTOR(directionVector);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(directionVector);
    packet->pos[0] = directionVector[0] * radialDistance;
    packet->pos[1] = 0;
    packet->pos[2] = directionVector[2] * radialDistance;
    VU0_LOAD_MATRIX(effect->head.matrix);
    SDF_VU0_LOADVECTOR(packet->pos);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR(packet->pos);
    packet->pos[0] += effect->head.origin[0];
    packet->pos[1] += effect->head.origin[1];
    packet->pos[2] += effect->head.origin[2];
    directionVector[0] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
    directionVector[1] = 0;
    directionVector[2] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
    SDF_VU0_LOADVECTOR(directionVector);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(packet->vel);
    packet->f30 = effect->f164 * (dds3GetRandFloatA(effDefaultRandomState) * effect->jitterB + (1.0f - effect->jitterB));
    packet->f34 = 0;
    packet->f38 = 0;
    packet->f3C = effect->f15C * (dds3GetRandFloatA(effDefaultRandomState) * effect->jitterA + (1.0f - effect->jitterA));
    packet->age = -(dds3GetRandA((EffRandState *)effEmitterDelayRandomState) % (effect->spread + 1));
    packet->color = 0;
    jitter = effect->head.speedJitter;
    packet->speed = effect->head.scale[0] * (dds3GetRandFloatA(effDefaultRandomState) * jitter + (1.0f - jitter));
    jitter = effect->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (dds3GetRandFloatA(effDefaultRandomState) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        packet->spin = 0;
    }
    parDispatchKindInit(&effect->head.sub, packetIndex);
}

/* Advance sinusoidal planar displacement and scaled vertical steps, then ages. */
void dds3Particle_SmokeDraw(EffDiscEmitter *effect) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 subeffectKind = effect->head.sub.kind;
    f32 positionDelta[4];
    f32 verticalScale;
    f32 phaseStep;
    f32 waveRate;
    s32 completedCount;
    s32 lifetimeFrames;
    s32 packetCount;
    s32 packetIndex;
    u32 repeatEnabled;

    dds3Particle_EffectInit(&effect->head.sub);
    VU0_LOAD_MATRIX(effect->head.matrix);
    phaseStep = effect->f160 * (3.14159265f / 180.0f);
    verticalScale = effect->f16C / 100.0f + 1.0f;
    lifetimeFrames = effect->head.lifetimeFrames;
    packetCount = effect->head.particleCount;
    repeatEnabled = effect->flag150;
    waveRate = effect->f168;
    completedCount = 0;
    for (packetIndex = 0; packetIndex < packetCount; packetIndex++, packet++) {
        s32 age = packet->age;

        if (age == EFF_PACKET_INITIAL_TAG) {
            dds3Particle_SmokeBuffInit(effect, packetIndex);
            age = packet->age;
        }
        if (age >= 0) {
            f32 verticalStep;
            f32 previousWaveValue;
            f32 factor;

            packet->color = dds3ParticleColorAnim((ParColorRamp *)effect->head.fade, packet->color, age);
            packet->color = dds3ParticleColorBlend(packet->color, effect->head.colorMask);
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR_C(packet);
            }
            packet->f30 += waveRate;
            verticalStep = packet->f3C;
            packet->f34 += phaseStep;
            factor = sdfSin(packet->f34);
            previousWaveValue = packet->f38;
            packet->f38 = factor;
            factor = (factor - previousWaveValue) * packet->f30;
            positionDelta[0] = packet->vel[0] * factor;
            positionDelta[1] = verticalStep;
            positionDelta[2] = packet->vel[2] * factor;
            SDF_VU0_LOADVECTOR(positionDelta);
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_STOREVECTOR(positionDelta);
            packet->pos[0] += positionDelta[0];
            packet->pos[1] += positionDelta[1];
            packet->pos[2] += positionDelta[2];
            packet->f3C = verticalStep * verticalScale;
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR(packet);
                parDispatchKindUpdate(&effect->head.sub, packetIndex, packet->color, packet->speed);
            }
        }
        age++;
        if (age >= lifetimeFrames) {
            if (repeatEnabled) {
                age = EFF_PACKET_INITIAL_TAG;
            } else {
                parDispatchKindInit(&effect->head.sub, packetIndex);
                completedCount++;
                if (completedCount >= packetCount) {
                    effect->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

/* Initialize only the native age/tag word of each packet record. */
void dds3Particle_SparkAllBuffInit(EffTemplatePacketList *effect) {
    EffectBufferRecord *packetCursor;
    u32 packetIndex;

    packetIndex = 0;
    packetCursor = effect->head.buffer->records;
    if (effect->head.particleCount != 0) {
        do {
            packetCursor->age = EFF_PACKET_INITIAL_TAG;
            packetIndex = packetIndex + 1;
            packetCursor = packetCursor + 1;
        } while (packetIndex < effect->head.particleCount);
    }
}

void dds3Particle_SparkSetScale(float scale, EffTemplatePacketList *effect) {
    effect->head.scale[0] = effect->head.scale[0] * scale;
    effect->head.scale[1] = effect->head.scale[1] * scale;
    effect->head.scale[2] = effect->head.scale[2] * scale;
    effect->tailValues[0] = effect->tailValues[0] * scale;
    effect->tailValues[2] = effect->tailValues[2] * scale;
    effect->tailValues[3] = effect->tailValues[3] * scale;
}

/* Clone the ballistic prefix/tail, allocate packet records, and initialize their native age tags. */
s32 dds3Particle_SparkCreate(EffTemplatePacketList *source) {
    EffEmitterHead *clone = sdfChipAllocDirect(0x180);
    s32 tailBytes = 0x30;

    memset(clone, 0, 0x180);
    memcpy(clone, source, source->head.templateSize);
    memcpy((u8 *)clone + EFF_TEMPLATE_TAIL_OFFSET, (u8 *)source + source->head.templateSize, tailBytes);
    dds3Particle_BasicCreate((EffEmitterHead *)clone);
    dds3Particle_SparkAllBuffInit((EffTemplatePacketList *)clone);
    return (s32)clone;
}

void dds3Particle_SparkDelete(u32 effect) {
    dds3Particle_BasicDelete((EffEmitterHead *)effect);
    sdfChipFreeDirect(effect);
}

void dds3Particle_SparkBuffInit(EffBallisticEmitter *effect, s32 index);
/* Twelve authored ballistic launch directions, stored as four-float vectors. */
extern f32 D_003AA9F0[12][4];

/* Seed a ballistic packet; positions are emitter-relative and then offset by the origin. */
void dds3Particle_SparkBuffInit(EffBallisticEmitter *effect, s32 index) {
    f32 position[4];
    f32 direction[4];
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    f32 gravity = effect->gravity / 100.0f;
    f32 radius = effect->radius;
    f32 cone = effect->cone;
    f32 speed = effect->speed;
    f32 jitter;
    f32 angle, cosine, sine;
    f32 decay;
    s32 frame;

    packet += index;
    switch (effect->mode) {
    case 0:
        if (effect->randomDir) {
            direction[0] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
            direction[1] = 0;
            direction[2] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
            SDF_VU0_LOADVECTOR(direction);
            SDF_VU0_NORMALIZE();
            SDF_VU0_STOREVECTOR(direction);
            packet->pos[0] = direction[0] * radius;
            packet->pos[1] = 0;
            packet->pos[2] = direction[2] * radius;
            VU0_LOAD_MATRIX(effect->head.matrix);
            SDF_VU0_LOADVECTOR(packet->pos);
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_STOREVECTOR(packet->pos);
            packet->pos[0] += effect->head.origin[0];
            packet->pos[1] += effect->head.origin[1];
            packet->pos[2] += effect->head.origin[2];
            direction[0] = (dds3GetRandFloatA(effEmitterDelayRandomState) - 0.5f) * 2.0f * cone;
            direction[1] = -(1.0f - cone);
            direction[2] = (dds3GetRandFloatA(effEmitterDelayRandomState) - 0.5f) * 2.0f * cone;
            SDF_VU0_LOADVECTOR(direction);
            SDF_VU0_NORMALIZE();
            SDF_VU0_STOREVECTOR(direction);
            jitter = dds3GetRandFloatA(effDefaultRandomState) * effect->jitter + (1.0f - effect->jitter);
            packet->vel[0] = direction[0] * speed * jitter;
            packet->vel[1] = direction[1] * speed * jitter;
            packet->vel[2] = direction[2] * speed * jitter;
            packet->f38 = (dds3GetRandFloatA(effDefaultRandomState) * 0.5f + 0.5f) * gravity;
        } else {
            angle = (3.14159265f * 2.0f) / (u32)effect->head.particleCount * index;
            cosine = sdfCos(angle);
            sine = sdfSin(angle);
            packet->pos[0] = cosine * radius;
            packet->pos[1] = 0;
            packet->pos[2] = sine * radius;
            VU0_LOAD_MATRIX(effect->head.matrix);
            SDF_VU0_LOADVECTOR(packet->pos);
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_STOREVECTOR(packet->pos);
            packet->pos[0] += effect->head.origin[0];
            packet->pos[1] += effect->head.origin[1];
            packet->pos[2] += effect->head.origin[2];
            packet->vel[0] = cosine * cone * speed;
            packet->vel[1] = -speed * (1.0f - cone);
            packet->vel[2] = sine * cone * speed;
            packet->f38 = gravity;
        }
        break;
    case 1:
        if (effect->randomDir) {
            packet->pos[0] = effect->head.origin[0];
            packet->pos[1] = effect->head.origin[1];
            packet->pos[2] = effect->head.origin[2];
            direction[0] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
            direction[1] = -((dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f);
            direction[2] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
            SDF_VU0_LOADVECTOR(direction);
            SDF_VU0_NORMALIZE();
            SDF_VU0_STOREVECTOR(direction);
            jitter = dds3GetRandFloatA(effDefaultRandomState) * effect->jitter + (1.0f - effect->jitter);
            packet->vel[0] = direction[0] * speed * jitter;
            packet->vel[1] = direction[1] * speed * jitter;
            packet->vel[2] = direction[2] * speed * jitter;
            packet->f38 = gravity;
        } else {
            packet->pos[0] = effect->head.origin[0];
            packet->pos[1] = effect->head.origin[1];
            packet->pos[2] = effect->head.origin[2];
            EE_MMI_UNIT_MATRIX(effect->head.matrix);
            packet->vel[0] = D_003AA9F0[index % 12][0] * speed;
            packet->vel[1] = D_003AA9F0[index % 12][1] * speed;
            packet->vel[2] = D_003AA9F0[index % 12][2] * speed;
            packet->f38 = gravity;
        }
        break;
    case 2:
        if (effect->randomDir) {
            decay = effect->decayPct / 100.0f + 1.0f;
            EE_MMI_UNIT_MATRIX(effect->head.matrix);
            direction[0] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
            direction[1] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
            direction[2] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
            SDF_VU0_LOADVECTOR(direction);
            SDF_VU0_NORMALIZE();
            SDF_VU0_STOREVECTOR(direction);
            jitter = dds3GetRandFloatA(effDefaultRandomState) * effect->jitter + (1.0f - effect->jitter);
            direction[0] *= speed * jitter;
            direction[1] *= speed * jitter;
            direction[2] *= speed * jitter;
            packet->vel[0] = -direction[0];
            packet->vel[1] = -direction[1];
            packet->vel[2] = -direction[2];
            position[0] = effect->head.origin[0];
            position[1] = effect->head.origin[1];
            position[2] = effect->head.origin[2];
            frame = 0;
            /* Pre-integrate at least one step, even for a nonpositive lifetime. */
            do {
                position[0] += direction[0];
                position[1] += direction[1];
                position[2] += direction[2];
                direction[0] *= decay;
                direction[1] *= decay;
                direction[2] *= decay;
                direction[1] += gravity;
                frame++;
            } while (frame < effect->head.lifetimeFrames);
            packet->pos[0] = position[0];
            packet->pos[1] = position[1];
            packet->pos[2] = position[2];
            packet->f38 = -gravity;
        }
        break;
    }
    packet->age = -(dds3GetRandA((EffRandState *)effEmitterDelayRandomState) % (effect->spread + 1));
    packet->color = 0;
    jitter = effect->head.speedJitter;
    packet->speed = effect->head.scale[0] * (dds3GetRandFloatA(effDefaultRandomState) * jitter + (1.0f - jitter));
    jitter = effect->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (dds3GetRandFloatA(effDefaultRandomState) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        packet->spin = 0;
    }
    parDispatchKindInit(&effect->head.sub, index);
}
/* PARK (22/512 words, both titles): exact except a $20/$21 swap between the two PRE-hoisted
 * HIGH pseudos (RNG state D_0034DF38/effDefaultRandomState vs effEmitterDelayRandomState). Refs/live are
 * identical to retail (12/163*256 vs 10/138*256) -> global-alloc priority ties at 8; the tie
 * falls back to pseudo order, which gcse pre_delete assigns in expression-hash bucket order.
 * gcse hashes SYMBOL_REF by NAME characters (gcc 2.95 hash_expr_1), bucket = h % (max_cuid/2|1).
 * Ours: max_cuid 421 -> 211 buckets: DF38 50, eff 91 -> DF38 first (wrong). Retail used the
 * original symbol names. Levers: rename the placeholder RNG symbol (shared, ~20 units/title), or
 * a source with max_cuid 400-407/434-439 (both titles) — no natural form found. */

/* Scale velocity, add its stored vertical acceleration, and advance position.
 * Only mode zero transforms the displacement through the emitter matrix. */
void dds3Particle_SparkDraw(EffBallisticEmitter *effect) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 subeffectKind = effect->head.sub.kind;
    u32 mode;
    f32 positionDelta[4];
    f32 velocityScale;
    s32 completedCount;
    s32 lifetimeFrames;
    s32 packetCount;
    s32 repeatEnabled;
    s32 packetIndex;

    dds3Particle_EffectInit(&effect->head.sub);
    mode = effect->mode;
    if (mode == 0) {
        VU0_LOAD_MATRIX(effect->head.matrix);
    }
    velocityScale = effect->decayPct / 100.0f + 1.0f;
    completedCount = 0;
    lifetimeFrames = effect->head.lifetimeFrames;
    packetCount = effect->head.particleCount;
    repeatEnabled = effect->loop;
    for (packetIndex = 0; packetIndex < packetCount; packetIndex++, packet++) {
        s32 age = packet->age;

        if (age == EFF_PACKET_INITIAL_TAG) {
            dds3Particle_SparkBuffInit(effect, packetIndex);
            age = packet->age;
        }
        if (age >= 0) {
            packet->color = dds3ParticleColorAnim((ParColorRamp *)effect->head.fade, packet->color, age);
            packet->color = dds3ParticleColorBlend(packet->color, effect->head.colorMask);
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR_C(packet);
            }
            packet->vel[1] = packet->vel[1] * velocityScale + packet->f38;
            packet->vel[0] *= velocityScale;
            packet->vel[2] *= velocityScale;
            SDF_VU0_LOADVECTOR(packet->vel);
            if (mode == 0) {
                SDF_VU0_APPLYMATRIX();
            }
            SDF_VU0_STOREVECTOR(positionDelta);
            packet->pos[0] += positionDelta[0];
            packet->pos[1] += positionDelta[1];
            packet->pos[2] += positionDelta[2];
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR(packet);
                parDispatchKindUpdate(&effect->head.sub, packetIndex, packet->color, packet->speed);
            }
        }
        age++;
        if (age >= lifetimeFrames) {
            if (repeatEnabled) {
                age = EFF_PACKET_INITIAL_TAG;
            } else {
                parDispatchKindInit(&effect->head.sub, packetIndex);
                completedCount++;
                if (completedCount >= packetCount) {
                    effect->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

extern void dds3Particle_RadiateBuffInit(EffLookAtRingEmitter *effect, u32 index);

/* Spawn each packet, then overwrite its tag with a zero-based, period-grouped schedule. */
void dds3Particle_RadiateAllBuffInit(EffLookAtRingEmitter *effect) {
    EffectBufferRecord *packetCursor;
    u32 packetIndex;
    u32 nextPacketIndex;
    s32 packetTag;

    packetTag = 0;
    packetIndex = 0;
    packetCursor = effect->head.buffer->records;
    if (effect->head.particleCount != 0) {
        do {
            dds3Particle_RadiateBuffInit(effect, packetIndex);
            packetCursor->age = packetTag;
            nextPacketIndex = packetIndex + 1;
            packetCursor = packetCursor + 1;
            if (nextPacketIndex % effect->period == 0) {
                packetTag = packetTag - effect->decayStep;
            }
            packetIndex = nextPacketIndex;
        } while (packetIndex < effect->head.particleCount);
    }
}

void dds3Particle_RadiateSetScale(float scale, EffLookAtRingEmitter *effect) {
    effect->head.scale[0] = effect->head.scale[0] * scale;
    effect->head.scale[1] = effect->head.scale[1] * scale;
    effect->head.scale[2] = effect->head.scale[2] * scale;
    effect->f164 = effect->f164 * scale;
}

/* Clone the look-at ring prefix/tail and spawn its staggered packet schedule. */
EffLookAtRingEmitter *dds3Particle_RadiateCreate(EffLookAtRingEmitter *source) {
    EffLookAtRingEmitter *clone = sdfChipAllocDirect(sizeof(*clone));
    s32 tailBytes = sizeof(*clone) - sizeof(clone->head);

    memset(clone, 0, sizeof(*clone));
    memcpy(clone, source, source->head.templateSize);
    memcpy(&clone->mode, (u8 *)source + source->head.templateSize, tailBytes);
    dds3Particle_BasicCreate(&clone->head);
    dds3Particle_RadiateAllBuffInit(clone);
    return clone;
}

void dds3Particle_RadiateDelete(EffLookAtRingEmitter *effect) {
    dds3Particle_BasicDelete(&effect->head);
    sdfChipFreeDirect(effect);
}

extern u8 aViewTarget[];
extern u8 sdfViewTargetVector[];
extern u8 aViewDown[];

/* Seed a view-oriented ring packet and initialize its subeffect. */
void dds3Particle_RadiateBuffInit(EffLookAtRingEmitter *effect, u32 packetIndex) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 period = 0;
    f32 angle;
    f32 scale;
    f32 jitter;

    packet += packetIndex;
    LookAtMatrix(aViewTarget, sdfViewTargetVector, aViewDown);
    ReverseMatrix();
    if (effect->mode == 0) {
        SDF_VU0_COPYVECTOR(packet, effect->head.origin);
        period = effect->period;
        angle = (3.14159265f * 2.0f) / period * (packetIndex % period);
        packet->vel[0] = sdfCos(angle);
        packet->vel[1] = sdfSin(angle);
        packet->vel[2] = 0;
        SDF_VU0_LOADVECTOR(packet->vel);
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_STOREVECTOR(packet->vel);
        packet->f34 = angle;
        packet->age = 0;
        packet->color = 0;
    } else {
        f32 lifetimeFrames = effect->head.lifetimeFrames;

        scale = effect->f164;
        /* Native quirk: period was assigned only in mode zero and is still zero here. */
        angle = (3.14159265f * 2.0f) / period * (packetIndex % period);
        scale *= lifetimeFrames;
        angle += effect->f160 * (3.14159265f / 180.0f) * lifetimeFrames;
        packet->vel[0] = sdfCos(angle);
        packet->vel[1] = 0;
        packet->vel[2] = sdfSin(angle);
        packet->f34 = angle;
        packet->pos[0] = packet->vel[0] * scale;
        packet->pos[1] = packet->vel[1] * scale;
        packet->pos[2] = packet->vel[2] * scale;
        packet->age = 0;
        packet->color = 0;
    }
    jitter = effect->head.speedJitter;
    packet->speed = effect->head.scale[0] * (dds3GetRandFloatA(effDefaultRandomState) * jitter + (1.0f - jitter));
    jitter = effect->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (dds3GetRandFloatA(effDefaultRandomState) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        packet->spin = 0;
    }
    parDispatchKindInit(&effect->head.sub, packetIndex);
}

/* Advance view-oriented motion and ages; repeated packets respawn immediately. */
void dds3Particle_RadiateDraw(EffLookAtRingEmitter *effect) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 subeffectKind = effect->head.sub.kind;
    f32 phaseStep;
    f32 stepScale;
    s32 completedCount = 0;
    s32 repeatEnabled;
    s32 packetCount;
    s32 lifetimeFrames;
    s32 packetIndex;

    dds3Particle_EffectInit(&effect->head.sub);
    LookAtMatrix(aViewTarget, sdfViewTargetVector, aViewDown);
    ReverseMatrix();
    lifetimeFrames = effect->head.lifetimeFrames;
    phaseStep = effect->f160 * (3.14159265f / 180.0f);
    packetCount = effect->head.particleCount;
    repeatEnabled = effect->loop;
    stepScale = effect->f164;
    for (packetIndex = 0; packetIndex < packetCount; packetIndex++, packet++) {
        s32 age = packet->age;
        f32 angle;

        if (age >= 0) {
            packet->color = dds3ParticleColorAnim((ParColorRamp *)effect->head.fade, packet->color, age);
            packet->color = dds3ParticleColorBlend(packet->color, effect->head.colorMask);
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR_C(packet);
            }
            packet->pos[0] += packet->vel[0] * stepScale;
            packet->pos[1] += packet->vel[1] * stepScale;
            packet->pos[2] += packet->vel[2] * stepScale;
            angle = packet->f34;
            packet->vel[0] = sdfCos(angle);
            packet->vel[1] = sdfSin(angle);
            packet->vel[2] = 0;
            SDF_VU0_LOADVECTOR(packet->vel);
            SDF_VU0_APPLYMATRIX_XYZ();
            SDF_VU0_STOREVECTOR(packet->vel);
            packet->f34 += phaseStep;
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR(packet);
                parDispatchKindUpdate(&effect->head.sub, packetIndex, packet->color, packet->speed);
            }
        }
        age++;
        if (age >= lifetimeFrames) {
            if (repeatEnabled) {
                dds3Particle_RadiateBuffInit(effect, packetIndex);
                age = packet->age;
            } else {
                parDispatchKindInit(&effect->head.sub, packetIndex);
                completedCount++;
                if (completedCount >= packetCount) {
                    effect->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

/* Initialize only the native age/tag word of each packet record. */
void dds3Particle_SphereSpiralAllBuffInit(EffTemplatePacketList *effect) {
    EffectBufferRecord *packetCursor;
    u32 packetIndex;

    packetIndex = 0;
    packetCursor = effect->head.buffer->records;
    if (effect->head.particleCount != 0) {
        do {
            packetCursor->age = EFF_PACKET_INITIAL_TAG;
            packetIndex = packetIndex + 1;
            packetCursor = packetCursor + 1;
        } while (packetIndex < effect->head.particleCount);
    }
}

void dds3Particle_SphereSpiralSetScale(float scale, EffTemplatePacketList *effect) {
    effect->head.scale[0] = effect->head.scale[0] * scale;
    effect->head.scale[1] = effect->head.scale[1] * scale;
    effect->head.scale[2] = effect->head.scale[2] * scale;
    effect->recordScale = effect->recordScale * scale;
    effect->tailValues[1] = effect->tailValues[1] * scale;
}

/* Clone the burst prefix/tail, allocate packet records, and initialize their native age tags. */
s32 dds3Particle_SphereSpiralCreate(EffTemplatePacketList *source) {
    EffEmitterHead *clone = sdfChipAllocDirect(0x170);
    s32 tailBytes = 0x20;

    memset(clone, 0, 0x170);
    memcpy(clone, source, source->head.templateSize);
    memcpy((u8 *)clone + EFF_TEMPLATE_TAIL_OFFSET, (u8 *)source + source->head.templateSize, tailBytes);
    dds3Particle_BasicCreate((EffEmitterHead *)clone);
    dds3Particle_SphereSpiralAllBuffInit((EffTemplatePacketList *)clone);
    return (s32)clone;
}

void dds3Particle_SphereSpiralDelete(u32 effect) {
    dds3Particle_BasicDelete((EffEmitterHead *)effect);
    sdfChipFreeDirect(effect);
}

typedef struct EffBurstEmitter {
    EffEmitterHead head;
    u8 mode;         /* 0x150 */
    u8 loop;         /* 0x151 */
    u8 pad152[2];
    u32 spread;      /* 0x154 */
    f32 speed;       /* 0x158 */
    f32 spinRate;    /* 0x15C */
    f32 f160;
    f32 jitterA;     /* 0x164 */
    f32 jitterB;     /* 0x168 */
} EffBurstEmitter;

/* Seed burst velocity and an optional random axis; retain the native spawn delay. */
void dds3Particle_SphereSpiralBuffInit(EffBurstEmitter *effect, u32 packetIndex) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    f32 directionVector[4];
    f32 speed;
    f32 jitter;
    f32 velocityLength;

    packet += packetIndex;
    packet->color = 0;
    packet->age = -(dds3GetRandA((EffRandState *)effEmitterDelayRandomState) % (effect->spread + 1));
    speed = effect->speed;
    jitter = effect->jitterA;
    directionVector[0] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
    directionVector[1] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
    directionVector[2] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
    SDF_VU0_LOADVECTOR(directionVector);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(directionVector);
    packet->vel[0] = speed * (dds3GetRandFloatA(effDefaultRandomState) * jitter + (1.0f - jitter)) * directionVector[0];
    packet->vel[1] = speed * (dds3GetRandFloatA(effDefaultRandomState) * jitter + (1.0f - jitter)) * directionVector[1];
    packet->vel[2] = speed * (dds3GetRandFloatA(effDefaultRandomState) * jitter + (1.0f - jitter)) * directionVector[2];
    SDF_VU0_LOADVECTOR(packet->vel);
    SDF_VU0_GETLENGTH(velocityLength);
    VU0_LOAD_MATRIX(effect->head.matrix);
    SDF_VU0_LOADVECTOR(packet->vel);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_STOREVECTOR_NOMEM(packet->vel);
    packet->pos[0] = packet->vel[0] + effect->head.origin[0];
    packet->pos[1] = packet->vel[1] + effect->head.origin[1];
    packet->pos[2] = packet->vel[2] + effect->head.origin[2];
    if (effect->mode == 1) {
        directionVector[0] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
        directionVector[1] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
        directionVector[2] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
        SDF_VU0_LOADVECTOR(directionVector);
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR(directionVector);
        packet->f30 = directionVector[0];
        packet->f34 = directionVector[1];
        packet->f38 = directionVector[2];
    } else {
        packet->f30 = 0;
        packet->f34 = -1.0f;
        packet->f38 = 0;
    }
    jitter = effect->jitterB;
    packet->f3C = (effect->f160 * (dds3GetRandFloatA(effDefaultRandomState) * jitter + (1.0f - jitter)) - velocityLength) / effect->head.lifetimeFrames;
    jitter = effect->head.speedJitter;
    packet->speed = effect->head.scale[0] * (dds3GetRandFloatA(effDefaultRandomState) * jitter + (1.0f - jitter));
    jitter = effect->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (dds3GetRandFloatA(effDefaultRandomState) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        packet->spin = 0;
    }
    parDispatchKindInit(&effect->head.sub, packetIndex);
}


/* Adjust velocity length, rotate around the packet axis, and rebuild position. */
void dds3Particle_SphereSpiralDraw(EffBurstEmitter *effect) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 subeffectKind = effect->head.sub.kind;
    f32 origin[4];
    f32 rotationAxis[4];
    f32 scaleVector[4];
    f32 velocityLength;
    f32 rotationStep;
    s32 completedCount;
    s32 lifetimeFrames;
    s32 repeatEnabled;
    s32 packetCount;
    s32 packetIndex;

    dds3Particle_EffectInit(&effect->head.sub);
    SDF_VU0_LOADMATRIX_B(effect->head.matrix);
    packetCount = effect->head.particleCount;
    lifetimeFrames = effect->head.lifetimeFrames;
    completedCount = 0;
    rotationStep = effect->spinRate * (3.14159265f / 180.0f);
    repeatEnabled = effect->loop;
    SDF_VU0_COPYVECTOR(origin, effect->head.origin);
    for (packetIndex = 0; packetIndex < packetCount; packetIndex++, packet++) {
        s32 age = packet->age;

        if (age == EFF_PACKET_INITIAL_TAG) {
            dds3Particle_SphereSpiralBuffInit(effect, packetIndex);
            age = packet->age;
        }
        if (age >= 0) {
            packet->color = dds3ParticleColorAnim((ParColorRamp *)effect->head.fade, packet->color, age);
            packet->color = dds3ParticleColorBlend(packet->color, effect->head.colorMask);
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR_C(packet);
            }
            SDF_VU0_LOADVECTOR(packet->vel);
            SDF_VU0_GETLENGTH(velocityLength);
            velocityLength += packet->f3C;
            scaleVector[0] = velocityLength;
            scaleVector[1] = velocityLength;
            scaleVector[2] = velocityLength;
            SDF_VU0_NORMALIZE();
            SDF_VU0_LOADVECTOR_B(scaleVector);
            SDF_VU0_MULVECTOR();
            SDF_VU0_STOREVECTOR(packet->vel);
            VU0_MOVE_VF(vf28, vf24);
            VU0_MOVE_VF(vf29, vf25);
            VU0_MOVE_VF(vf30, vf26);
            VU0_MOVE_VF(vf31, vf27);
            rotationAxis[0] = packet->f30;
            rotationAxis[1] = packet->f34;
            rotationAxis[2] = packet->f38;
            rotationAxis[3] = 0;
            SDF_VU0_LOADVECTOR(rotationAxis);
            SDF_VU0_APPLYMATRIX_XYZ();
            SDF_VU0_STOREVECTOR_NOMEM(rotationAxis);
            SetRotateMatrixAxis(rotationStep, (const RwV3d *)rotationAxis);
            SDF_VU0_LOADVECTOR(packet->vel);
            SDF_VU0_APPLYMATRIX_XYZ();
            SDF_VU0_LOADVECTOR_B(origin);
            SDF_VU0_STOREVECTOR(packet->vel);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(packet);
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR(packet);
                parDispatchKindUpdate(&effect->head.sub, packetIndex, packet->color, packet->speed);
            }
        }
        age++;
        if (age >= lifetimeFrames) {
            if (repeatEnabled) {
                age = EFF_PACKET_INITIAL_TAG;
            } else {
                parDispatchKindInit(&effect->head.sub, packetIndex);
                completedCount++;
                if (completedCount >= packetCount) {
                    effect->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

/* Initialize only the native age/tag word of each packet record. */
void dds3Particle_SphereSmokeAllBuffInit(EffTemplatePacketList *effect) {
    EffectBufferRecord *packetCursor;
    u32 packetIndex;

    packetIndex = 0;
    packetCursor = effect->head.buffer->records;
    if (effect->head.particleCount != 0) {
        do {
            packetCursor->age = EFF_PACKET_INITIAL_TAG;
            packetIndex = packetIndex + 1;
            packetCursor = packetCursor + 1;
        } while (packetIndex < effect->head.particleCount);
    }
}

void dds3Particle_SphereSmokeSetScale(float scale, EffTemplatePacketList *effect) {
    effect->head.scale[0] = effect->head.scale[0] * scale;
    effect->head.scale[1] = effect->head.scale[1] * scale;
    effect->head.scale[2] = effect->head.scale[2] * scale;
    effect->recordScale = effect->recordScale * scale;
    effect->tailValues[0] = effect->tailValues[0] * scale;
    effect->tailValues[2] = effect->tailValues[2] * scale;
}

/* Clone the sphere prefix/tail, allocate packet records, and initialize their native age tags. */
s32 dds3Particle_SphereSmokeCreate(EffTemplatePacketList *source) {
    EffEmitterHead *clone = sdfChipAllocDirect(0x180);
    s32 tailBytes = 0x30;

    memset(clone, 0, 0x180);
    memcpy(clone, source, source->head.templateSize);
    memcpy((u8 *)clone + EFF_TEMPLATE_TAIL_OFFSET, (u8 *)source + source->head.templateSize, tailBytes);
    dds3Particle_BasicCreate((EffEmitterHead *)clone);
    dds3Particle_SphereSmokeAllBuffInit((EffTemplatePacketList *)clone);
    return (s32)clone;
}

void dds3Particle_SphereSmokeDelete(u32 effect) {
    dds3Particle_BasicDelete((EffEmitterHead *)effect);
    sdfChipFreeDirect(effect);
}

typedef struct EffSphereEmitter {
    EffEmitterHead head;
    u8 flag150;
    u8 pad151[3];
    u32 spread;      /* 0x154 */
    f32 radius;      /* 0x158 */
    f32 f15C;
    f32 f160;
    f32 f164;
    f32 f168;
    f32 f16C;
    f32 f170;
    f32 f174;
    f32 f178;
} EffSphereEmitter;

/* Seed a jittered spherical position and planar direction.
 * Native delay uses bitwise complement, not unary negation. */
void dds3Particle_SphereSmokeBuffInit(EffSphereEmitter *effect, u32 packetIndex) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    f32 directionVector[4];
    f32 radius;
    f32 jitter;

    packet += packetIndex;
    radius = effect->radius;
    jitter = effect->f178;
    directionVector[0] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
    directionVector[1] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
    directionVector[2] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
    SDF_VU0_LOADVECTOR(directionVector);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(directionVector);
    packet->pos[0] = radius * (dds3GetRandFloatA(effDefaultRandomState) * jitter + (1.0f - jitter)) * directionVector[0];
    packet->pos[1] = radius * (dds3GetRandFloatA(effDefaultRandomState) * jitter + (1.0f - jitter)) * directionVector[1];
    packet->pos[2] = radius * (dds3GetRandFloatA(effDefaultRandomState) * jitter + (1.0f - jitter)) * directionVector[2];
    VU0_LOAD_MATRIX(effect->head.matrix);
    SDF_VU0_LOADVECTOR(packet->pos);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR(packet->pos);
    packet->pos[0] += effect->head.origin[0];
    packet->pos[1] += effect->head.origin[1];
    packet->pos[2] += effect->head.origin[2];
    packet->vel[0] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
    packet->vel[1] = 0;
    packet->vel[2] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
    SDF_VU0_LOADVECTOR(packet->vel);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(packet->vel);
    packet->f30 = effect->f164 * (dds3GetRandFloatA(effDefaultRandomState) * effect->f174 + (1.0f - effect->f174));
    packet->f34 = 0;
    packet->f38 = 0;
    packet->f3C = effect->f15C * (dds3GetRandFloatA(effDefaultRandomState) * effect->f170 + (1.0f - effect->f170));
    packet->age = ~(dds3GetRandA((EffRandState *)effEmitterDelayRandomState) % (effect->spread + 1));
    packet->color = 0;
    jitter = effect->head.speedJitter;
    packet->speed = effect->head.scale[0] * (dds3GetRandFloatA(effDefaultRandomState) * jitter + (1.0f - jitter));
    jitter = effect->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (dds3GetRandFloatA(effDefaultRandomState) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        packet->spin = 0;
    }
    parDispatchKindInit(&effect->head.sub, packetIndex);
}

/* Advance sinusoidal planar displacement and scaled vertical steps, then ages. */
void dds3Particle_SphereSmokeDraw(EffSphereEmitter *effect) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 subeffectKind = effect->head.sub.kind;
    f32 positionDelta[4];
    f32 verticalScale;
    f32 phaseStep;
    f32 waveRate;
    s32 completedCount;
    s32 lifetimeFrames;
    s32 packetCount;
    s32 packetIndex;
    u32 repeatEnabled;

    dds3Particle_EffectInit(&effect->head.sub);
    VU0_LOAD_MATRIX(effect->head.matrix);
    phaseStep = effect->f160 * (3.14159265f / 180.0f);
    verticalScale = effect->f16C / 100.0f + 1.0f;
    lifetimeFrames = effect->head.lifetimeFrames;
    packetCount = effect->head.particleCount;
    repeatEnabled = effect->flag150;
    waveRate = effect->f168;
    completedCount = 0;
    for (packetIndex = 0; packetIndex < packetCount; packetIndex++, packet++) {
        s32 age = packet->age;

        if (age == EFF_PACKET_INITIAL_TAG) {
            dds3Particle_SphereSmokeBuffInit(effect, packetIndex);
            age = packet->age;
        }
        if (age >= 0) {
            f32 verticalStep;
            f32 previousWaveValue;
            f32 factor;

            packet->color = dds3ParticleColorAnim((ParColorRamp *)effect->head.fade, packet->color, age);
            packet->color = dds3ParticleColorBlend(packet->color, effect->head.colorMask);
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR_C(packet);
            }
            packet->f30 += waveRate;
            verticalStep = packet->f3C;
            packet->f34 += phaseStep;
            factor = sdfSin(packet->f34);
            previousWaveValue = packet->f38;
            packet->f38 = factor;
            factor = (factor - previousWaveValue) * packet->f30;
            positionDelta[0] = packet->vel[0] * factor;
            positionDelta[1] = verticalStep;
            positionDelta[2] = packet->vel[2] * factor;
            SDF_VU0_LOADVECTOR(positionDelta);
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_STOREVECTOR(positionDelta);
            packet->pos[0] += positionDelta[0];
            packet->pos[1] += positionDelta[1];
            packet->pos[2] += positionDelta[2];
            packet->f3C = verticalStep * verticalScale;
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR(packet);
                parDispatchKindUpdate(&effect->head.sub, packetIndex, packet->color, packet->speed);
            }
        }
        age++;
        if (age >= lifetimeFrames) {
            if (repeatEnabled) {
                age = EFF_PACKET_INITIAL_TAG;
            } else {
                parDispatchKindInit(&effect->head.sub, packetIndex);
                completedCount++;
                if (completedCount >= packetCount) {
                    effect->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

/* Stagger native tags using this variant's second tail word as the group period. */
void dds3Particle_SphereRingAllBuffInit(EffTemplatePacketList *effect) {
    EffectBufferRecord *packetCursor;
    u32 packetIndex;
    u32 nextPacketIndex;
    s32 packetTag;

    packetTag = EFF_PACKET_INITIAL_TAG;
    packetIndex = 0;
    packetCursor = effect->head.buffer->records;
    if (effect->head.particleCount != 0) {
        do {
            packetCursor->age = packetTag;
            nextPacketIndex = packetIndex + 1;
            packetCursor = packetCursor + 1;
            if (nextPacketIndex % effect->tailWords[1] == 0) {
                packetTag = packetTag - effect->decayStep;
            }
            packetIndex = nextPacketIndex;
        } while (packetIndex < effect->head.particleCount);
    }
}

void dds3Particle_SphereRingSetScale(float scale, EffTemplatePacketList *effect) {
    effect->head.scale[0] = effect->head.scale[0] * scale;
    effect->head.scale[1] = effect->head.scale[1] * scale;
    effect->head.scale[2] = effect->head.scale[2] * scale;
    effect->recordScale = effect->recordScale * scale;
}

/* Clone the expanding-ring prefix/tail and initialize its period-grouped packet tags. */
s32 dds3Particle_SphereRingCreate(EffTemplatePacketList *source) {
    EffEmitterHead *clone = sdfChipAllocDirect(0x170);
    s32 tailBytes = 0x20;

    memset(clone, 0, 0x170);
    memcpy(clone, source, source->head.templateSize);
    memcpy((u8 *)clone + EFF_TEMPLATE_TAIL_OFFSET, (u8 *)source + source->head.templateSize, tailBytes);
    dds3Particle_BasicCreate((EffEmitterHead *)clone);
    dds3Particle_SphereRingAllBuffInit((EffTemplatePacketList *)clone);
    return (s32)clone;
}

void dds3Particle_SphereRingDelete(u32 effect) {
    dds3Particle_BasicDelete((EffEmitterHead *)effect);
    sdfChipFreeDirect(effect);
}

typedef struct EffExpandingRingEmitter {
    EffEmitterHead head;
    u8 pad150;
    u8 loop;         /* 0x151 */
    u8 pad152[6];
    f32 radius;      /* 0x158 */
    f32 jitter;      /* 0x15C */
    u32 period;      /* 0x160 */
    f32 spinRate;    /* 0x164 */
} EffExpandingRingEmitter;

/* Seed radius and ring direction, placing the packet at its rotated vertical offset. */
void dds3Particle_SphereRingBuffInit(EffExpandingRingEmitter *effect, u32 packetIndex) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    f32 offsetVector[4];
    f32 jitter;
    f32 radius;
    f32 angle;

    packet += packetIndex;
    packet->age = -1;
    packet->color = 0;
    jitter = effect->jitter;
    radius = effect->radius * (dds3GetRandFloatA(effDefaultRandomState) * jitter + (1.0f - jitter));
    packet->f30 = radius;
    angle = (3.14159265f * 2.0f) / effect->period * (packetIndex % effect->period);
    packet->vel[0] = sdfCos(angle);
    packet->vel[1] = 0;
    packet->vel[2] = sdfSin(angle);
    VU0_LOAD_MATRIX(effect->head.matrix);
    offsetVector[1] = radius;
    offsetVector[0] = 0;
    offsetVector[2] = 0;
    SDF_VU0_LOADVECTOR(offsetVector);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_LOADVECTOR_B(effect->head.origin);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(packet);
    jitter = effect->head.speedJitter;
    packet->speed = effect->head.scale[0] * (dds3GetRandFloatA(effDefaultRandomState) * jitter + (1.0f - jitter));
    jitter = effect->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (dds3GetRandFloatA(effDefaultRandomState) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        packet->spin = 0;
    }
    parDispatchKindInit(&effect->head.sub, packetIndex);
}

/* Build age-phase ring positions and rotate the stored X/Z directions.
 * vector is reused for the position offset and old direction components. */
void dds3Particle_SphereRingDraw(EffExpandingRingEmitter *effect) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 subeffectKind = effect->head.sub.kind;
    f32 origin[4];
    f32 vector[4];
    f32 rotationStep;
    f32 rotationCos;
    f32 rotationSin;
    s32 completedCount;
    s32 repeatEnabled;
    s32 packetCount;
    s32 lifetimeFrames;
    s32 packetIndex;

    dds3Particle_EffectInit(&effect->head.sub);
    VU0_LOAD_MATRIX(effect->head.matrix);
    rotationStep = effect->spinRate * (3.14159265f / 180.0f);
    completedCount = 0;
    packetCount = effect->head.particleCount;
    lifetimeFrames = effect->head.lifetimeFrames;
    repeatEnabled = effect->loop;
    rotationCos = sdfCos(rotationStep);
    rotationSin = sdfSin(rotationStep);
    SDF_VU0_COPYVECTOR(origin, effect->head.origin);
    for (packetIndex = 0; packetIndex < packetCount; packetIndex++, packet++) {
        s32 age = packet->age;

        if (age == EFF_PACKET_INITIAL_TAG) {
            dds3Particle_SphereRingBuffInit(effect, packetIndex);
            age = packet->age;
        }
        if (age >= 0) {
            f32 angle;
            f32 radius;
            f32 phaseCos;
            f32 phaseSin;

            packet->color = dds3ParticleColorAnim((ParColorRamp *)effect->head.fade, packet->color, age);
            packet->color = dds3ParticleColorBlend(packet->color, effect->head.colorMask);
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR_C(packet);
            }
            angle = (f32)age / (f32)lifetimeFrames * 3.14159265f;
            radius = packet->f30;
            phaseCos = sdfCos(angle);
            phaseSin = sdfSin(angle);
            vector[0] = packet->vel[0] * (radius * phaseSin);
            vector[1] = radius * phaseCos;
            vector[2] = packet->vel[2] * (radius * phaseSin);
            SDF_VU0_LOADVECTOR(vector);
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_LOADVECTOR_B(origin);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(packet);
            vector[0] = packet->vel[0];
            vector[2] = packet->vel[2];
            packet->vel[0] = rotationCos * vector[0] + rotationSin * vector[2];
            packet->vel[2] = rotationCos * vector[2] - rotationSin * vector[0];
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR(packet);
                parDispatchKindUpdate(&effect->head.sub, packetIndex, packet->color, packet->speed);
            }
        }
        age++;
        if (age >= lifetimeFrames) {
            if (repeatEnabled) {
                age = EFF_PACKET_INITIAL_TAG;
            } else {
                parDispatchKindInit(&effect->head.sub, packetIndex);
                completedCount++;
                if (completedCount >= packetCount) {
                    effect->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

/* Initialize only the native age/tag word of each packet record. */
void dds3Particle_Spark2AllBuffInit(EffTemplatePacketList *effect) {
    EffectBufferRecord *packetCursor;
    u32 packetIndex;

    packetIndex = 0;
    packetCursor = effect->head.buffer->records;
    if (effect->head.particleCount != 0) {
        do {
            packetCursor->age = EFF_PACKET_INITIAL_TAG;
            packetIndex = packetIndex + 1;
            packetCursor = packetCursor + 1;
        } while (packetIndex < effect->head.particleCount);
    }
}

void dds3Particle_Spark2SetScale(float scale, EffTemplatePacketList *effect) {
    effect->head.scale[0] = effect->head.scale[0] * scale;
    effect->head.scale[1] = effect->head.scale[1] * scale;
    effect->head.scale[2] = effect->head.scale[2] * scale;
    effect->tailValues[0] = effect->tailValues[0] * scale;
    effect->tailValues[2] = effect->tailValues[2] * scale;
    effect->tailValues[3] = effect->tailValues[3] * scale;
}

/* Clone the cone prefix/tail, allocate packet records, and initialize their native age tags. */
s32 dds3Particle_Spark2Create(EffTemplatePacketList *source) {
    EffEmitterHead *clone = sdfChipAllocDirect(0x180);
    s32 tailBytes = 0x30;

    memset(clone, 0, 0x180);
    memcpy(clone, source, source->head.templateSize);
    memcpy((u8 *)clone + EFF_TEMPLATE_TAIL_OFFSET, (u8 *)source + source->head.templateSize, tailBytes);
    dds3Particle_BasicCreate((EffEmitterHead *)clone);
    dds3Particle_Spark2AllBuffInit((EffTemplatePacketList *)clone);
    return (s32)clone;
}

void dds3Particle_Spark2Delete(u32 effect) {
    dds3Particle_BasicDelete((EffEmitterHead *)effect);
    sdfChipFreeDirect(effect);
}

typedef struct EffConeEmitter {
    EffEmitterHead head;
    u8 mode;         /* 0x150 */
    u8 loop;         /* 0x151 */
    u8 pad152[2];
    u32 spread;      /* 0x154 */
    u8 pad158[4];
    f32 radius;      /* 0x15C */
    f32 cone;        /* 0x160 */
    f32 gravity;     /* 0x164 */
    f32 speed;       /* 0x168 */
    f32 decayPct;    /* 0x16C */
    u8 pad170[4];
    f32 degrees;     /* 0x174 */
} EffConeEmitter;

/* Seed mode-zero cone position/velocity; every mode initializes age and subeffect. */
void dds3Particle_Spark2BuffInit(EffConeEmitter *effect, s32 packetIndex) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    f32 gravity;
    f32 sweepRadians;
    f32 angle;
    f32 radius;
    f32 cone;
    f32 speed;
    f32 angleCos;
    f32 angleSin;
    f32 jitter;

    packet += packetIndex;
    gravity = effect->gravity / 100.0f;
    sweepRadians = effect->degrees * (3.14159265f / 180.0f);
    radius = effect->radius;
    cone = effect->cone;
    speed = effect->speed;
    if (effect->mode == 0) {
        angle = sweepRadians / (u32)effect->head.particleCount * packetIndex;
        angleCos = sdfCos(angle);
        angleSin = sdfSin(angle);
        packet->pos[0] = angleCos * radius;
        packet->pos[1] = 0;
        packet->pos[2] = angleSin * radius;
        VU0_LOAD_MATRIX(effect->head.matrix);
        SDF_VU0_LOADVECTOR(packet->pos);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR(packet->pos);
        packet->pos[0] += effect->head.origin[0];
        packet->pos[1] += effect->head.origin[1];
        packet->pos[2] += effect->head.origin[2];
        packet->vel[0] = angleCos * cone * speed;
        packet->vel[1] = -speed * (1.0f - cone);
        packet->vel[2] = angleSin * cone * speed;
        packet->f38 = gravity;
    }
    packet->age = -(dds3GetRandA((EffRandState *)effEmitterDelayRandomState) % (effect->spread + 1));
    packet->color = 0;
    jitter = effect->head.speedJitter;
    packet->speed = effect->head.scale[0] * (dds3GetRandFloatA(effDefaultRandomState) * jitter + (1.0f - jitter));
    jitter = effect->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (dds3GetRandFloatA(effDefaultRandomState) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        packet->spin = 0;
    }
    parDispatchKindInit(&effect->head.sub, packetIndex);
}

/* Scale velocity, apply stored gravity, and transform each motion step before aging. */
void dds3Particle_Spark2Draw(EffConeEmitter *effect) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 subeffectKind = effect->head.sub.kind;
    f32 positionDelta[4];
    f32 velocityScale;
    s32 completedCount;
    s32 lifetimeFrames;
    s32 packetCount;
    s32 repeatEnabled;
    s32 packetIndex;

    dds3Particle_EffectInit(&effect->head.sub);
    VU0_LOAD_MATRIX(effect->head.matrix);
    velocityScale = effect->decayPct / 100.0f + 1.0f;
    completedCount = 0;
    lifetimeFrames = effect->head.lifetimeFrames;
    packetCount = effect->head.particleCount;
    repeatEnabled = effect->loop;
    for (packetIndex = 0; packetIndex < packetCount; packetIndex++, packet++) {
        s32 age = packet->age;

        if (age == EFF_PACKET_INITIAL_TAG) {
            dds3Particle_Spark2BuffInit(effect, packetIndex);
            age = packet->age;
        }
        if (age >= 0) {
            packet->color = dds3ParticleColorAnim((ParColorRamp *)effect->head.fade, packet->color, age);
            packet->color = dds3ParticleColorBlend(packet->color, effect->head.colorMask);
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR_C(packet);
            }
            packet->vel[0] *= velocityScale;
            packet->vel[1] = packet->vel[1] * velocityScale + packet->f38;
            packet->vel[2] *= velocityScale;
            SDF_VU0_LOADVECTOR(packet->vel);
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_STOREVECTOR(positionDelta);
            packet->pos[0] += positionDelta[0];
            packet->pos[1] += positionDelta[1];
            packet->pos[2] += positionDelta[2];
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR(packet);
                parDispatchKindUpdate(&effect->head.sub, packetIndex, packet->color, packet->speed);
            }
        }
        age++;
        if (age >= lifetimeFrames) {
            if (repeatEnabled) {
                age = EFF_PACKET_INITIAL_TAG;
            } else {
                parDispatchKindInit(&effect->head.sub, packetIndex);
                completedCount++;
                if (completedCount >= packetCount) {
                    effect->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

/* Initialize each packet's native tag and its template-selected uniform scale. */
void dds3Particle_SphereAtomAllBuffInit(EffTemplatePacketList *effect) {
    s32 packetIndex = 0;
    EffTemplateScaleRecord *packetCursor = (EffTemplateScaleRecord *)effect->head.buffer->records;
    if (effect->head.particleCount != 0) {
        do {
            packetCursor->initialTag = EFF_PACKET_INITIAL_TAG;
            packetCursor->scale = effect->recordScale;
            packetIndex++;
            packetCursor++;
        } while ((u32)packetIndex < effect->head.particleCount);
    }
}

void dds3Particle_SphereAtomSetScale(float scale, EffTemplatePacketList *effect) {
    effect->head.scale[0] = effect->head.scale[0] * scale;
    effect->head.scale[1] = effect->head.scale[1] * scale;
    effect->head.scale[2] = effect->head.scale[2] * scale;
    effect->recordScale = effect->recordScale * scale;
    effect->tailValues[0] = effect->tailValues[0] * scale;
    effect->tailValues[1] = effect->tailValues[1] * scale;
}

/* Clone the prefix/tail and initialize each packet record with the template's uniform scale. */
s32 dds3Particle_SphereAtomCreate(EffTemplatePacketList *source) {
    EffEmitterHead *clone = sdfChipAllocDirect(0x170);
    s32 tailBytes = 0x20;

    memset(clone, 0, 0x170);
    memcpy(clone, source, source->head.templateSize);
    memcpy((u8 *)clone + EFF_TEMPLATE_TAIL_OFFSET, (u8 *)source + source->head.templateSize, tailBytes);
    dds3Particle_BasicCreate((EffEmitterHead *)clone);
    dds3Particle_SphereAtomAllBuffInit((EffTemplatePacketList *)clone);
    return (s32)clone;
}

void dds3Particle_SphereAtomDelete(u32 effect) {
    dds3Particle_BasicDelete((EffEmitterHead *)effect);
    sdfChipFreeDirect(effect);
}

/* vu0 routine: seed radial placement, normalized velocity and activation age. */
void dds3Particle_SphereAtomBuffInit(EffTemplatePacketList *effect, s32 index) {
    f32 direction[4];
    EffectBufferRecord *packet = effect->head.buffer->records;
    f32 radius;
    f32 value;

    packet += index;
    if (effect->randomSphere) {
        radius = effect->recordScale;
        direction[0] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
        direction[1] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
        direction[2] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
        SDF_VU0_LOADVECTOR(direction);
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR(direction);
        packet->position[0] = radius * dds3GetRandFloatA(effDefaultRandomState) * direction[0];
        packet->position[1] = radius * dds3GetRandFloatA(effDefaultRandomState) * direction[1];
        packet->position[2] = radius * dds3GetRandFloatA(effDefaultRandomState) * direction[2];
        VU0_LOAD_MATRIX(effect->head.matrix);
        SDF_VU0_LOADVECTOR(packet->position);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR(packet->position);
        packet->position[0] += effect->head.origin[0];
        packet->position[1] += effect->head.origin[1];
        packet->position[2] += effect->head.origin[2];
        direction[0] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
        direction[1] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
        direction[2] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
        SDF_VU0_LOADVECTOR(direction);
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR(packet->velocity);
        packet->age = -(dds3GetRandA((EffRandState *)effEmitterDelayRandomState) % (effect->delayRange + 1));
    } else {
        SDF_VU0_COPYVECTOR(packet->position, effect->head.origin);
        value = (3.14159265f * 2.0f) / effect->head.particleCount * index;
        direction[0] = sdfCos(value);
        direction[1] = sdfSin(value);
        direction[2] = 0;
        SDF_VU0_LOADVECTOR(direction);
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR(packet->velocity);
        packet->age = -(dds3GetRandA((EffRandState *)effEmitterDelayRandomState) % (effect->delayRange + 1));
    }
    packet->f30 = effect->motionMagnitude;
    packet->color = 0;
    value = effect->head.speedJitter;
    packet->scale = effect->head.scale[0] * (dds3GetRandFloatA(effDefaultRandomState) * value + (1.0f - value));
    value = effect->head.spinJitter;
    if (value != 0) {
        packet->spin = (dds3GetRandFloatA(effDefaultRandomState) * value + (1.0f - value)) * (3.14159265f * 2.0f);
    } else {
        packet->spin = 0;
    }
    parDispatchKindInit(&effect->head.sub, index);
}

extern f32 D_003AAAB0[4];

/* Reflect moving packets at the frame's entry radius. The first record's
 * updated radius becomes the boundary on the following frame. */
void dds3Particle_SphereAtomDraw(EffTemplatePacketList *effect) {
    f32 origin[4];
    f32 previousPosition[4];
    /* Retail leaves the fourth broadcast components uninitialized. The
     * XYZW operations propagate those W lanes into the packet stores. */
    f32 reflectionScale[4];
    f32 normal[4];
    f32 boundaryScale[4];
    s32 completedCount = 0;
    s32 repeatEnabled;
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 subeffectKind = effect->head.sub.kind;
    s32 lifetimeFrames;
    s32 packetCount;
    s32 index;
    f32 radiusStep;
    f32 targetRadius;
    f32 radius;
    f32 velocityScale;

    dds3Particle_EffectInit(&effect->head.sub);
    lifetimeFrames = (s32)effect->head.lifetimeFrames;
    targetRadius = effect->targetRadius;
    radiusStep = (targetRadius - effect->recordScale) / lifetimeFrames;
    velocityScale = effect->accelerationPct / 100.0f + 1.0f;
    packetCount = (s32)effect->head.particleCount;
    repeatEnabled = effect->restart;
    radius = packet->f34;
    if (radiusStep >= 0.0f) {
        if (radius < targetRadius) {
            packet->f34 = radius + radiusStep;
            if (packet->f34 > targetRadius) {
                packet->f34 = targetRadius;
            }
        }
    } else {
        if (radius > targetRadius) {
            packet->f34 = radius + radiusStep;
            if (packet->f34 < targetRadius) {
                packet->f34 = targetRadius;
            }
        }
    }
    SDF_VU0_COPYVECTOR(origin, effect->head.origin);
    for (index = 0; index < packetCount; index++, packet++) {
        s32 age = packet->age;
        f32 distance;
        f32 dot;
        f32 dx, dy, dz;
        f32 magnitude;

        if (age == EFF_PACKET_INITIAL_TAG) {
            dds3Particle_SphereAtomBuffInit(effect, index);
            age = packet->age;
        }
        if (age >= 0) {
            packet->color = dds3ParticleColorAnim((ParColorRamp *)effect->head.fade, packet->color, age);
            packet->color = dds3ParticleColorBlend(packet->color, effect->head.colorMask);
            if (subeffectKind != 0) {
                SDF_VU0_COPYVECTOR(previousPosition, packet->pos);
            }
            magnitude = packet->f30;
            dx = packet->vel[0] * magnitude;
            dy = packet->vel[1] * magnitude;
            dz = packet->vel[2] * magnitude;
            packet->pos[0] += dx;
            packet->pos[1] += dy;
            packet->pos[2] += dz;
            magnitude *= velocityScale;
            packet->f30 = magnitude;
            SDF_VU0_LOADVECTOR(packet->pos);
            SDF_VU0_LOADVECTOR_B(origin);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_COPYVECTOR_A_TO_C();
            SDF_VU0_GETLENGTH(distance);
            SDF_VU0_COPYVECTOR_C_TO_A();
            SDF_VU0_NORMALIZE();
            SDF_VU0_STOREVECTOR(normal);
            if (distance > radius) {
                SDF_VU0_LOADVECTOR(packet->vel);
                SDF_VU0_LOADVECTOR_B(D_003AAAB0);
                SDF_VU0_MULVECTOR();
                SDF_VU0_COPYVECTOR_A_TO_C();
                SDF_VU0_LOADVECTOR(origin);
                SDF_VU0_LOADVECTOR_B(packet->pos);
                SDF_VU0_SUBVECTOR();
                SDF_VU0_NORMALIZE();
                SDF_VU0_COPYVECTOR_C_TO_B();
                SDF_VU0_COPYVECTOR_A_TO_C();
                SDF_VU0_INNERPRODUCT(dot);
                reflectionScale[0] = dot * 2.0f;
                reflectionScale[1] = dot * 2.0f;
                reflectionScale[2] = dot * 2.0f;
                SDF_VU0_LOADVECTOR(reflectionScale);
                SDF_VU0_COPYVECTOR_C_TO_B();
                SDF_VU0_MULVECTOR();
                SDF_VU0_LOADVECTOR_B(packet->vel);
                SDF_VU0_ADDVECTOR();
                SDF_VU0_NORMALIZE();
                SDF_VU0_STOREVECTOR(packet->vel);
                boundaryScale[0] = radius;
                boundaryScale[1] = radius;
                boundaryScale[2] = radius;
                SDF_VU0_LOADVECTOR(normal);
                SDF_VU0_LOADVECTOR_B(boundaryScale);
                SDF_VU0_MULVECTOR();
                SDF_VU0_LOADVECTOR_B(origin);
                SDF_VU0_ADDVECTOR();
                SDF_VU0_STOREVECTOR(packet->pos);
            }
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR_C(previousPosition);
                SDF_VU0_LOADVECTOR(packet->pos);
                parDispatchKindUpdate(&effect->head.sub, index, packet->color, packet->speed);
            }
        }
        age++;
        if (age >= lifetimeFrames) {
            if (repeatEnabled) {
                age = EFF_PACKET_INITIAL_TAG;
            } else {
                parDispatchKindInit(&effect->head.sub, index);
                completedCount++;
                if (completedCount >= packetCount) {
                    effect->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

void effInitParticleRecord();

void dds3Particle_SpiralRandomize(EffParticleOverlay *effect) {
    effInitParticleRecord(effect);
}

void effScaleSingleParticleTemplate(float scale, EffTemplatePacketList *effect) {
    effect->head.scale[0] = effect->head.scale[0] * scale;
    effect->head.scale[1] = effect->head.scale[1] * scale;
    effect->head.scale[2] = effect->head.scale[2] * scale;
}

/* Clone a single-particle template: the fixed prefix plus an empty appended tail. */
void *effCloneSingleParticleTemplate(EffTemplatePacketList *source) {
    EffTemplatePacketList *clone = (EffTemplatePacketList *)sdfChipAllocDirect(0x150);
    s32 tailBytes = 0;

    memset(clone, 0, 0x150);
    memcpy(clone, source, source->head.templateSize);
    memcpy((u8 *)clone + EFF_TEMPLATE_TAIL_OFFSET, (u8 *)source + source->head.templateSize, tailBytes);
    clone->head.particleCount = 1;
    clone->head.lifetimeFrames = 0;
    dds3Particle_BasicCreate(&clone->head);
    return clone;
}

void effFreeSingleParticleTemplate(void *effect) {
    dds3Particle_BasicDelete((EffEmitterHead *)effect);
    sdfChipFreeDirect(effect);
}

extern EffRandState effDefaultRandomState[];

/* Initialize the first record at frame -1; speed draws RNG, angle draws it only for nonzero jitter. */
void effInitParticleRecord(effect)
    EffParticleOverlay *effect;
{
    EffParticleRecord *record = (EffParticleRecord *)effect->buffer->records;
    f32 jitter;
    u32 baseColor;

    baseColor = effect->color | (effect->alpha << 24);
    record->x = effect->x;
    record->frame = -1;
    record->y = effect->y;
    record->color = baseColor;
    record->z = effect->z;
    record->speed = 1.0f;
    record->angle = 0;
    effect->lastSpeed = effect->speed;
    jitter = effect->speedJitter;
    record->speed = effect->speed * (dds3GetRandFloatA(effDefaultRandomState) * jitter + (1.0f - jitter));
    jitter = effect->angleJitter;
    if (jitter != 0) {
        record->angle = (dds3GetRandFloatA(effDefaultRandomState) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        record->angle = 0;
    }
}

/* Frame zero reinitializes to -1 before incrementing; refresh position and modulated packed color. */
void dds3Particle_SpiralBlendColor(EffParticleOverlay *effect) {
    EffParticleRecord *record = (EffParticleRecord *)effect->buffer->records;
    u32 baseColor;

    if (record->frame == 0) {
        effInitParticleRecord(effect);
    }
    record->frame = record->frame + 1;
    record->x = effect->x;
    record->y = effect->y;
    baseColor = effect->color | (effect->alpha << 24);
    effect->unk24 = record->frame + 1;
    record->color = baseColor;
    record->z = effect->z;
    record->color = dds3ParticleColorBlend(baseColor, effect->colorMask);
}

/* Initialize only the native age/tag word of each packet record. */
void dds3Particle_SparkOffsetAllBuffInit(EffTemplatePacketList *effect) {
    EffectBufferRecord *packetCursor;
    u32 packetIndex;

    packetIndex = 0;
    packetCursor = effect->head.buffer->records;
    if (effect->head.particleCount != 0) {
        do {
            packetCursor->age = EFF_PACKET_INITIAL_TAG;
            packetIndex = packetIndex + 1;
            packetCursor = packetCursor + 1;
        } while (packetIndex < effect->head.particleCount);
    }
}

void dds3Particle_SparkOffsetSetScale(float scale, EffTemplatePacketList *effect) {
    effect->head.scale[0] = effect->head.scale[0] * scale;
    effect->head.scale[1] = effect->head.scale[1] * scale;
    effect->head.scale[2] = effect->head.scale[2] * scale;
    effect->tailValues[0] = effect->tailValues[0] * scale;
    effect->tailValues[2] = effect->tailValues[2] * scale;
    effect->tailValues[3] = effect->tailValues[3] * scale;
}

/* Clone the offset-gravity prefix/tail, allocate records, and initialize their native age tags. */
s32 dds3Particle_SparkOffsetCreate(EffTemplatePacketList *source) {
    EffEmitterHead *clone = sdfChipAllocDirect(0x190);
    s32 tailBytes = 0x40;

    memset(clone, 0, 0x190);
    memcpy(clone, source, source->head.templateSize);
    memcpy((u8 *)clone + EFF_TEMPLATE_TAIL_OFFSET, (u8 *)source + source->head.templateSize, tailBytes);
    dds3Particle_BasicCreate((EffEmitterHead *)clone);
    dds3Particle_SparkOffsetAllBuffInit((EffTemplatePacketList *)clone);
    return (s32)clone;
}

void dds3Particle_SparkOffsetDelete(u32 effect) {
    dds3Particle_BasicDelete((EffEmitterHead *)effect);
    sdfChipFreeDirect(effect);
}

typedef struct EffOffsetGravityEmitter {
    EffEmitterHead head;
    u8 mode;         /* 0x150 */
    u8 loop;         /* 0x151 */
    u8 pad152[2];
    u32 spread;      /* 0x154: random activation delay bound */
    u8 randomDir;    /* 0x158 */
    u8 pad159[3];
    f32 radius;      /* 0x15C */
    f32 cone;        /* 0x160: horizontal direction weight */
    f32 gravity;     /* 0x164: divided by 100 for acceleration */
    f32 speed;       /* 0x168: launch magnitude */
    f32 decayPct;    /* 0x16C */
    f32 jitter;      /* 0x170: launch magnitude jitter */
} EffOffsetGravityEmitter;

void dds3Particle_SparkOffsetBuffInit(EffOffsetGravityEmitter *effect, s32 index);
/* Twelve authored velocity directions, stored as four-float vectors. */
extern f32 D_003AAAC0[12][4];

/* Seed an offset-gravity packet; preserve each mode's native partial vector setup. */
void dds3Particle_SparkOffsetBuffInit(EffOffsetGravityEmitter *effect, s32 index) {
    f32 position[4];
    f32 direction[4];
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    f32 gravity = effect->gravity / 100.0f;
    f32 radius = effect->radius;
    f32 cone = effect->cone;
    f32 speed = effect->speed;
    f32 jitter;
    f32 angle, cosine, sine;
    f32 decay;
    s32 frame;

    packet += index;
    switch (effect->mode) {
    case 0:
        if (effect->randomDir) {
            direction[0] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
            direction[1] = 0;
            direction[2] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
            SDF_VU0_LOADVECTOR(direction);
            SDF_VU0_NORMALIZE();
            SDF_VU0_STOREVECTOR(direction);
            packet->f30 = direction[0] * radius;
            packet->f34 = 0;
            packet->f38 = direction[2] * radius;
            SDF_VU0_COPYVECTOR(packet->pos, effect->head.origin);
            VU0_LOAD_MATRIX(effect->head.matrix);
            SDF_VU0_LOADVECTOR(&packet->f30);
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_STOREVECTOR(&packet->f30);
            direction[0] = (dds3GetRandFloatA(effEmitterDelayRandomState) - 0.5f) * 2.0f * cone;
            direction[1] = -(1.0f - cone);
            direction[2] = (dds3GetRandFloatA(effEmitterDelayRandomState) - 0.5f) * 2.0f * cone;
            SDF_VU0_LOADVECTOR(direction);
            SDF_VU0_NORMALIZE();
            SDF_VU0_STOREVECTOR(direction);
            jitter = dds3GetRandFloatA(effDefaultRandomState) * effect->jitter + (1.0f - effect->jitter);
            packet->vel[0] = direction[0] * speed * jitter;
            packet->vel[1] = direction[1] * speed * jitter;
            packet->vel[2] = direction[2] * speed * jitter;
            packet->f3C = (dds3GetRandFloatA(effDefaultRandomState) * 0.5f + 0.5f) * gravity;
        } else {
            /* This native branch initializes position, leaving the stored offset intact. */
            angle = (3.14159265f * 2.0f) / (u32)effect->head.particleCount * index;
            cosine = sdfCos(angle);
            sine = sdfSin(angle);
            packet->pos[0] = cosine * radius;
            packet->pos[1] = 0;
            packet->pos[2] = sine * radius;
            VU0_LOAD_MATRIX(effect->head.matrix);
            SDF_VU0_LOADVECTOR(packet->pos);
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_STOREVECTOR(packet->pos);
            packet->vel[0] = cosine * cone * speed;
            packet->vel[1] = -speed * (1.0f - cone);
            packet->vel[2] = sine * cone * speed;
            packet->f3C = gravity;
        }
        break;
    case 1:
        if (effect->randomDir) {
            packet->f30 = 0;
            packet->f34 = 0;
            packet->f38 = 0;
            SDF_VU0_COPYVECTOR(packet->pos, effect->head.origin);
            direction[0] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
            direction[1] = -((dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f);
            direction[2] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
            SDF_VU0_LOADVECTOR(direction);
            SDF_VU0_NORMALIZE();
            SDF_VU0_STOREVECTOR(direction);
            jitter = dds3GetRandFloatA(effDefaultRandomState) * effect->jitter + (1.0f - effect->jitter);
            packet->vel[0] = direction[0] * speed * jitter;
            packet->vel[1] = direction[1] * speed * jitter;
            packet->vel[2] = direction[2] * speed * jitter;
            packet->f3C = gravity;
        } else {
            /* As in retail, the deterministic direction path leaves offset.xyz intact. */
            packet->pos[0] = 0;
            packet->pos[1] = 0;
            packet->pos[2] = 0;
            EE_MMI_UNIT_MATRIX(effect->head.matrix);
            packet->vel[0] = D_003AAAC0[index % 12][0] * speed;
            packet->vel[1] = D_003AAAC0[index % 12][1] * speed;
            packet->vel[2] = D_003AAAC0[index % 12][2] * speed;
            packet->f3C = gravity;
        }
        break;
    case 2:
        if (effect->randomDir) {
            decay = effect->decayPct / 100.0f + 1.0f;
            EE_MMI_UNIT_MATRIX(effect->head.matrix);
            direction[0] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
            direction[1] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
            direction[2] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
            SDF_VU0_LOADVECTOR(direction);
            SDF_VU0_NORMALIZE();
            SDF_VU0_STOREVECTOR(direction);
            jitter = dds3GetRandFloatA(effDefaultRandomState) * effect->jitter + (1.0f - effect->jitter);
            direction[0] *= speed * jitter;
            direction[1] *= speed * jitter;
            direction[2] *= speed * jitter;
            packet->vel[0] = -direction[0];
            packet->vel[1] = -direction[1];
            packet->vel[2] = -direction[2];
            position[0] = 0;
            position[1] = 0;
            position[2] = 0;
            frame = 0;
            /* Pre-integrate at least one step, even for a nonpositive lifetime. */
            do {
                position[0] += direction[0];
                position[1] += direction[1];
                position[2] += direction[2];
                direction[0] *= decay;
                direction[1] *= decay;
                direction[2] *= decay;
                direction[1] += gravity;
                frame++;
            } while (frame < effect->head.lifetimeFrames);
            packet->pos[0] = position[0];
            packet->pos[1] = position[1];
            packet->pos[2] = position[2];
            packet->f3C = -gravity;
        }
        break;
    }
    packet->age = -(dds3GetRandA((EffRandState *)effEmitterDelayRandomState) % (effect->spread + 1));
    packet->color = 0;
    jitter = effect->head.speedJitter;
    packet->speed = effect->head.scale[0] * (dds3GetRandFloatA(effDefaultRandomState) * jitter + (1.0f - jitter));
    jitter = effect->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (dds3GetRandFloatA(effDefaultRandomState) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        packet->spin = 0;
    }
    parDispatchKindInit(&effect->head.sub, index);
}

/* Accumulate motion in the stored offset, then add the emitter origin.
 * Only mode zero transforms the per-frame displacement through the matrix. */
void dds3Particle_SparkOffsetDraw(EffOffsetGravityEmitter *effect) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 subeffectKind = effect->head.sub.kind;
    u32 mode;
    f32 positionDelta[4];
    f32 origin[4];
    f32 velocityScale;
    s32 completedCount;
    s32 lifetimeFrames;
    s32 packetCount;
    s32 repeatEnabled;
    s32 packetIndex;

    dds3Particle_EffectInit(&effect->head.sub);
    mode = effect->mode;
    if (mode == 0) {
        VU0_LOAD_MATRIX(effect->head.matrix);
    }
    velocityScale = effect->decayPct / 100.0f + 1.0f;
    completedCount = 0;
    packetCount = effect->head.particleCount;
    lifetimeFrames = effect->head.lifetimeFrames;
    repeatEnabled = effect->loop;
    SDF_VU0_COPYVECTOR(origin, effect->head.origin);
    for (packetIndex = 0; packetIndex < packetCount; packetIndex++, packet++) {
        s32 age = packet->age;

        if (age == EFF_PACKET_INITIAL_TAG) {
            dds3Particle_SparkOffsetBuffInit(effect, packetIndex);
            age = packet->age;
        }
        if (age >= 0) {
            packet->color = dds3ParticleColorAnim((ParColorRamp *)effect->head.fade, packet->color, age);
            packet->color = dds3ParticleColorBlend(packet->color, effect->head.colorMask);
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR_C(packet);
            }
            packet->vel[1] = packet->vel[1] * velocityScale + packet->f3C;
            packet->vel[0] *= velocityScale;
            packet->vel[2] *= velocityScale;
            SDF_VU0_LOADVECTOR(packet->vel);
            if (mode == 0) {
                SDF_VU0_APPLYMATRIX();
            }
            SDF_VU0_STOREVECTOR(positionDelta);
            packet->f30 += positionDelta[0];
            packet->f34 += positionDelta[1];
            packet->f38 += positionDelta[2];
            SDF_VU0_LOADVECTOR(&packet->f30);
            SDF_VU0_LOADVECTOR_B(origin);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(packet);
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR(packet);
                parDispatchKindUpdate(&effect->head.sub, packetIndex, packet->color, packet->speed);
            }
        }
        age++;
        if (age >= lifetimeFrames) {
            if (repeatEnabled) {
                age = EFF_PACKET_INITIAL_TAG;
            } else {
                parDispatchKindInit(&effect->head.sub, packetIndex);
                completedCount++;
                if (completedCount >= packetCount) {
                    effect->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

/* Initialize only the native age/tag word of each packet record. */
void dds3Particle_SmokeOffsetAllBuffInit(EffTemplatePacketList *effect) {
    EffectBufferRecord *packetCursor;
    u32 packetIndex;

    packetIndex = 0;
    packetCursor = effect->head.buffer->records;
    if (effect->head.particleCount != 0) {
        do {
            packetCursor->age = EFF_PACKET_INITIAL_TAG;
            packetIndex = packetIndex + 1;
            packetCursor = packetCursor + 1;
        } while (packetIndex < effect->head.particleCount);
    }
}

void dds3Particle_SmokeOffsetSetScale(float scale, EffTemplatePacketList *effect) {
    effect->head.scale[0] = effect->head.scale[0] * scale;
    effect->head.scale[1] = effect->head.scale[1] * scale;
    effect->head.scale[2] = effect->head.scale[2] * scale;
    effect->recordScale = effect->recordScale * scale;
    effect->tailValues[0] = effect->tailValues[0] * scale;
    effect->tailValues[2] = effect->tailValues[2] * scale;
}

/* Clone the disc prefix/tail plus a separate 16-byte-per-packet auxiliary allocation. */
EffTemplatePacketList *dds3Particle_SmokeOffsetCreate(EffTemplatePacketList *source) {
    EffTemplatePacketList *clone = sdfChipAllocDirect(0x200);
    struct sdfMemHandle_t *auxAllocationHandle;
    s32 tailBytes = 0xB0;

    memset(clone, 0, 0x200);
    memcpy(clone, source, source->head.templateSize);
    memcpy((u8 *)clone + EFF_TEMPLATE_TAIL_OFFSET, (u8 *)source + source->head.templateSize, tailBytes);
    auxAllocationHandle = sdfMemAllocDirect(clone->head.particleCount * 0x10);
    clone->auxiliaryAllocation = auxAllocationHandle;
    clone->auxiliaryData = (void *)sdfMemLock(auxAllocationHandle);
    dds3Particle_BasicCreate(&clone->head);
    dds3Particle_SmokeOffsetAllBuffInit(clone);
    return clone;
}

void dds3Particle_SmokeOffsetDelete(EffTemplatePacketList *effect) {
    sdfMemFreeDirect(effect->auxiliaryAllocation);
    dds3Particle_BasicDelete(&effect->head);
    sdfChipFreeDirect(effect);
}

typedef struct EffDiscAuxEmitter {
    EffEmitterHead head;
    u8 flag150;
    u8 flag151;
    u8 pad152[2];
    u32 spread;      /* 0x154 */
    f32 radius;      /* 0x158 */
    f32 f15C;
    f32 f160;
    f32 f164;
    f32 f168;
    f32 f16C;
    f32 f170;
    f32 f174;
    f32 (*aux)[4];   /* 0x178: 16 bytes per packet */
} EffDiscAuxEmitter;

/* Seed the auxiliary position and packet direction separately, then the subeffect. */
void dds3Particle_SmokeOffsetBuffInit(EffDiscAuxEmitter *effect, u32 packetIndex) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    f32 (*auxiliaryCursor)[4] = effect->aux;
    f32 directionVector[4];
    f32 radialDistance;
    f32 jitter;

    packet += packetIndex;
    auxiliaryCursor += packetIndex;
    radialDistance = effect->radius * ((dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f);
    directionVector[0] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
    directionVector[1] = 0;
    directionVector[2] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
    SDF_VU0_LOADVECTOR(directionVector);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(directionVector);
    (*auxiliaryCursor)[0] = directionVector[0] * radialDistance;
    (*auxiliaryCursor)[1] = 0;
    (*auxiliaryCursor)[2] = directionVector[2] * radialDistance;
    VU0_LOAD_MATRIX(effect->head.matrix);
    SDF_VU0_LOADVECTOR(*auxiliaryCursor);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR(*auxiliaryCursor);
    directionVector[0] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
    directionVector[1] = 0;
    directionVector[2] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
    SDF_VU0_LOADVECTOR(directionVector);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(packet->vel);
    packet->f30 = effect->f164 * (dds3GetRandFloatA(effDefaultRandomState) * effect->f174 + (1.0f - effect->f174));
    packet->f34 = 0;
    packet->f38 = 0;
    packet->f3C = effect->f15C * (dds3GetRandFloatA(effDefaultRandomState) * effect->f170 + (1.0f - effect->f170));
    packet->age = -(dds3GetRandA((EffRandState *)effEmitterDelayRandomState) % (effect->spread + 1));
    packet->color = 0;
    SDF_VU0_COPYVECTOR(packet, effect->head.origin);
    jitter = effect->head.speedJitter;
    packet->speed = effect->head.scale[0] * (dds3GetRandFloatA(effDefaultRandomState) * jitter + (1.0f - jitter));
    jitter = effect->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (dds3GetRandFloatA(effDefaultRandomState) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        packet->spin = 0;
    }
    parDispatchKindInit(&effect->head.sub, packetIndex);
}

/* Advance auxiliary positions, then transform/store packet positions with origin.
 * Auxiliary accumulation remains separate from the transformed packet position. */
void dds3Particle_SmokeOffsetDraw(EffDiscAuxEmitter *effect) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 subeffectKind = effect->head.sub.kind;
    f32 (*auxiliaryCursor)[4] = effect->aux;
    f32 positionVector[4];
    f32 origin[4];
    f32 verticalScale;
    f32 phaseStep;
    f32 waveRate;
    s32 completedCount;
    s32 lifetimeFrames;
    s32 packetCount;
    s32 packetIndex;
    u32 repeatEnabled;

    dds3Particle_EffectInit(&effect->head.sub);
    VU0_LOAD_MATRIX(effect->head.matrix);
    phaseStep = effect->f160 * (3.14159265f / 180.0f);
    verticalScale = effect->f16C / 100.0f + 1.0f;
    completedCount = 0;
    packetCount = effect->head.particleCount;
    lifetimeFrames = effect->head.lifetimeFrames;
    repeatEnabled = effect->flag150;
    waveRate = effect->f168;
    SDF_VU0_COPYVECTOR(origin, effect->head.origin);
    for (packetIndex = 0; packetIndex < packetCount; packetIndex++, packet++, auxiliaryCursor++) {
        s32 age = packet->age;

        if (age == EFF_PACKET_INITIAL_TAG) {
            dds3Particle_SmokeOffsetBuffInit(effect, packetIndex);
            age = packet->age;
        }
        if (age >= 0) {
            f32 verticalStep;
            f32 previousWaveValue;
            f32 factor;

            packet->color = dds3ParticleColorAnim((ParColorRamp *)effect->head.fade, packet->color, age);
            packet->color = dds3ParticleColorBlend(packet->color, effect->head.colorMask);
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR_C(packet);
            }
            packet->f30 += waveRate;
            verticalStep = packet->f3C;
            packet->f34 += phaseStep;
            factor = sdfSin(packet->f34);
            previousWaveValue = packet->f38;
            packet->f38 = factor;
            factor = (factor - previousWaveValue) * packet->f30;
            positionVector[0] = (*auxiliaryCursor)[0] + packet->vel[0] * factor;
            positionVector[1] = (*auxiliaryCursor)[1] + verticalStep;
            positionVector[2] = (*auxiliaryCursor)[2] + packet->vel[2] * factor;
            SDF_VU0_LOADVECTOR(positionVector);
            SDF_VU0_STOREVECTOR(*auxiliaryCursor);
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_LOADVECTOR_B(origin);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(packet);
            packet->f3C = verticalStep * verticalScale;
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR(packet);
                parDispatchKindUpdate(&effect->head.sub, packetIndex, packet->color, packet->speed);
            }
        }
        age++;
        if (age >= lifetimeFrames) {
            if (repeatEnabled) {
                age = EFF_PACKET_INITIAL_TAG;
            } else {
                parDispatchKindInit(&effect->head.sub, packetIndex);
                completedCount++;
                if (completedCount >= packetCount) {
                    effect->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

/* Initialize only the native age/tag word of each packet record. */
void dds3Particle_SphereSpiral2AllBuffInit(EffTemplatePacketList *effect) {
    EffectBufferRecord *packetCursor;
    u32 packetIndex;

    packetIndex = 0;
    packetCursor = effect->head.buffer->records;
    if (effect->head.particleCount != 0) {
        do {
            packetCursor->age = EFF_PACKET_INITIAL_TAG;
            packetIndex = packetIndex + 1;
            packetCursor = packetCursor + 1;
        } while (packetIndex < effect->head.particleCount);
    }
}

void dds3Particle_SphereSpiral2SetScale(float scale, EffTemplatePacketList *effect) {
    effect->head.scale[0] = effect->head.scale[0] * scale;
    effect->head.scale[1] = effect->head.scale[1] * scale;
    effect->head.scale[2] = effect->head.scale[2] * scale;
    effect->recordScale = effect->recordScale * scale;
    effect->tailValues[1] = effect->tailValues[1] * scale;
}

/* Allocate one 12-byte descriptor per packet and its attached 16-byte records. */
s32 dds3Particle_SphereSpiral2Create(EffTemplatePacketList *source) {
    s32 copy = (s32)sdfChipAllocDirect(0x190);
    s32 tailLen = 0x40;
    s32 count;
    s32 perRecord;
    s32 listBytes;
    s32 *node;
    s32 *addr;
    s32 step;
    s32 base;
    s32 i;

    memset((void *)copy, 0, 0x190);
    memcpy((void *)copy, source, source->head.templateSize);
    memcpy((void *)(copy + 0x150), (u8 *)source + source->head.templateSize, tailLen);
    dds3Particle_BasicCreate((EffEmitterHead *)copy);
    perRecord = 0;
    if (((EffTemplatePacketList *)copy)->head.sub.kind != 0) {
        count = ((EffTemplatePacketList *)copy)->head.particleCount;
        /* All four supported resource kinds use the same subrecord count. */
        switch (((EffTemplatePacketList *)copy)->head.sub.kind) {
        case PAR_KIND_STATE_HISTORY:
            perRecord = ((EffTemplatePacketList *)copy)->head.sub.templateEntryCount;
            break;
        case PAR_KIND_STATE_BILLBOARD_CROSS_STRIP:
            perRecord = ((EffTemplatePacketList *)copy)->head.sub.templateEntryCount;
            break;
        case PAR_KIND_STATE_BILLBOARD_CROSS_TRIANGLE:
            perRecord = ((EffTemplatePacketList *)copy)->head.sub.templateEntryCount;
            break;
        case PAR_KIND_STATE_TRACK_POLYGON_CROSS_AXES:
            perRecord = ((EffTemplatePacketList *)copy)->head.sub.templateEntryCount;
            break;
        }
        listBytes = count * 12;
        ((EffTemplatePacketList *)copy)->recordsPerPacket = perRecord;
        ((EffTemplatePacketList *)copy)->listAllocation = sdfMemAllocDirect(listBytes + perRecord * count * 16);
        addr = (s32 *)sdfMemLock(((EffTemplatePacketList *)copy)->listAllocation);
        i = 0;
        base = (s32)addr;
        ((EffTemplatePacketList *)copy)->recordList = addr;
        base = base + listBytes;
        if (count > 0) {
            step = perRecord * 16;
            node = addr;
            do {
                i++;
                node[2] = base;
                node[0] = 0;
                node[1] = 0;
                node += 3;
                base += step;
            } while (i < count);
        }
    } else {
        ((EffTemplatePacketList *)copy)->listAllocation = 0;
    }
    dds3Particle_SphereSpiral2AllBuffInit((EffTemplatePacketList *)copy);
    return copy;
}
