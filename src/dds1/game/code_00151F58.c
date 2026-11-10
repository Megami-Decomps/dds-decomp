#include "sdk_ps2.h"
#include "eff_composite_gs_descriptor.h"
#include "eff_unit_object.h"
#include "sdf_projection.h"
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
#include "sdf_heap_api.h"
#include "sdf_gs_packet.h"
#include "pcp_vu0.h"
#include "sdf_vu_transform_api.h"
#include "ee_mmi.h"
#include "sdf_asset_create.h"
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
#include "fpu.h"
#include "sdf_packet_alloc.h"
#include "eff_packet_params.h"

#define BILL_VARIANT_MASK 0xFFFF

#define EFF_PACKET_LIST_BYTES 0x20

extern EffectConfig D_0034DF54[];

extern billHandle_t *hGeneralBillboard[];

void effDrawGeneratedTextureQuad(sdfDrawTag_t *packet, EffGeneratedTextureDescriptor *source);

void effDrawCompositeTextureQuad(sdfDrawTag_t *packet, EffCompositeGsDescriptor *source);

/* Create a billboard sharing the indexed entry's resource. Word two of the
 * resource stores the reference count; the billHandle_t payload is not an emitter. */
billHandle_t *dds3BillboardGeneral(s32 index) {
    billHandle_t *effect = dds3BillboardCreate(D_0034DF54[index].billboardKind, 0);
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

void effCopyVector(f32 destination[4], const f32 source[4]) {
    SDF_VU0_COPYVECTOR(destination, source);
}

/* Set the radian rotation applied to the billboard quad corners. */
void dds3BillboardSetRotate(billHandle_t *effect, float angle) {
    effect->rotationAngle = angle;
}

/* Set the two child scale components stored at +0x10 and +0x14. */
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
    s32 remainingChildren;
    BillChildPayload **childEntries;
    BillChildPayload **childCursor;

    switch (effect->kind) {
    case 0:
        effect->child->variant = (u16)variant;
        break;
    case 1:
        remainingChildren = effect->animationData->childCount;
        childEntries = effect->animationData->children;

        if (remainingChildren > 0) {
            childCursor = childEntries;
            do {
                (*childCursor)->variant = (u16)variant;
                childCursor++;
                remainingChildren--;
            } while (remainingChildren != 0);
        }
        break;
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

s32 dds3BillboardGetAnimNumber(billHandle_t *billboard) {
    if (billboard->kind == 1) {
        return ((BillLinkedValue *)billboard->animationData->base)->value;
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

/* Read the animation-active gate for list billboards. */
u16 dds3BillboardGetAnimState(const billHandle_t *effect) {
    if (effect->kind == 1) {
        return effect->animationActive;
    }
    return 0;
}

s32 dds3BillboardGetAnimType(billHandle_t *billboard) {
    if (billboard->kind == 1) {
        return billboard->modeFlags;
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

extern u8 D_00324650[];

extern u8 D_00324660[];

extern u8 D_0034E080[];

/* vu0 routine: project position and position + scaled offset; return the reverse XY heading. */
f32 effComputeProjectedOffsetAngle(const void *position, const void *offset) {
    f32 projectedDelta[4];
    f32 projectedPosition[4];
    f32 projectedEndpoint[4];

    VU0_LOAD_MATRIX(sdfViewMatrix);
    MultMatrixRev((const struct Matrix4 *)sdfProjectionMatrix);
    SDF_VU0_LOADVECTOR(position);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    SDF_VU0_LOADVECTOR_B(D_00324650);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(D_00324660);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(projectedPosition);

    SDF_VU0_LOADVECTOR(offset);
    SDF_VU0_LOADVECTOR_B(D_0034E080);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_C_TO_B();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    SDF_VU0_LOADVECTOR_B(D_00324650);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(D_00324660);
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

extern f32 aViewTarget[4], aViewEye[4];

/* vu0 routine: 1 - |dot(normalised eye - target, axis)|, how far the axis is from the view direction */
f32 dds3BillboardGetDirectionScale(void *self, f32 *axis) {
    f32 dot;

    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(axis);
    SDF_VU0_INNERPRODUCT(dot);
    dot = ffabsf(dot);
    return 1.0f - dot;
}

/* Create the indexed billboard and render asset; only the second matrix is initialized. */
u8 *dds3P2APolygonCreate(s32 entryIndex) {
    EffUnitObject *instance = sdfChipAllocDirect(sizeof(EffUnitObject));

    instance->billboard = dds3BillboardCreate(1, entryIndex);
    instance->material = sdfMatNew();
    sdfMatSetConstant(instance->material, 1.0f);
    EE_MMI_UNIT_MATRIX(instance->transform);
    return (u8 *)instance;
}

/* Retain a cloned billboard's shared data, create a fresh render asset, and reset both matrices. */
u8 *dds3P2APolygonCopy(EffUnitObject *source) {
    EffUnitObject *instance = sdfChipAllocDirect(sizeof(EffUnitObject));

    instance->billboard = dds3BillboardCopy(source->billboard);
    instance->material = sdfMatNew();
    sdfMatSetConstant(instance->material, 1.0f);
    sdfMatSetDiffuse(instance->material, 0x80808080);
    sdfMatSetAmbient(instance->material, 0x80808080);
    sdfMatSetSpecular(instance->material, 0x80808080);
    EE_MMI_UNIT_MATRIX(instance->transform);
    EE_MMI_UNIT_MATRIX(instance->localMatrix);
    return (u8 *)instance;
}

void dds3P2APolygonDelete(EffUnitObject *instance) {
    sdfMatFreeDelayed(instance->material);
    dds3BillboardDelete(instance->billboard);
    sdfChipFreeDirect(instance);
}

void dds3P2APolygonSetPosition(EffUnitObject *instance, const f32 position[4]) {
    effCopyVector(instance->billboard->position, position);
}

void dds3P2APolygonSetScale(EffUnitObject *instance, float scale) {
    dds3BillboardSetScale(instance->billboard, scale, scale);
}

/* vu0 routine: copy the supplied 4x4 matrix to the instance's second matrix. */
void dds3P2APolygonSetRotateMatrix(EffUnitObject *instance, void *matrix) {
    VU0_LOAD_MATRIX(matrix);
    VU0_STORE_MATRIX(instance->transform);
}

void dds3P2APolygonSetRGBA(EffUnitObject *instance, u32 value) {
    dds3BillboardSetRGBA(instance->billboard, value);
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void dds3P2APolygonSetDirection(void *dst, void *src) {
    VU0_COPY_MATRIX(dst, src);
}

/* Kind 1 writes mode plus one/two entry values; untouched output words retain their contents. */
void dds3P2APolygonGetResource(EffUnitObject *instance, s32 *modeValues) {
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

extern u32 D_0034E090[],D_0034E0A0[];

extern sdfGrOt_t *D_0034E0B0[];

extern void *effPkt3DPrimMakePoly(sdf3DPrimPolyParm_t *);

extern void sdf3DPrimLinkMat(sdfDrawTag_t *packetList, sdfMat_t *asset, s32 (*allocatePacket)(s32));

extern void sdf3DPrimLinkTm(sdfDrawTag_t *packetList, s32 (*allocatePacket)(s32));

void dds3P2APolygonDraw(EffUnitObject *instance) {
    sdf3DPrimPolyParm_t params;
    s32 modeValues[3];
    f32 matrix[4][4];
    f32 positions[4][4];
    f32 uv[4][2];
    f32 secondaryUv[4][2];
    s32 list;
    BillChildPayload *child;
    sdfTexHandle_t *texture;
    sdfGrOt_t *surface;
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
    MultMatrixNoArg();
    VU0_STORE_MATRIX_UNCLOBBERED(matrix);
    matrix[3][0] = instance->billboard->position[0];
    matrix[3][1] = instance->billboard->position[1];
    matrix[3][2] = instance->billboard->position[2];
    VU0_LOAD_MATRIX(matrix);
    sdf3DPrimLinkTm((sdfDrawTag_t *)list, 0);
    memset(&params, 0, sizeof(params));
    params.attr = 0x4000;
    params.faceIndex = D_0034E090;
    params.vertex = (u128 *)positions;
    params.vertexColor = D_0034E0A0;
    params.uv = &uv[0][0];
    params.color = instance->billboard->childParam;
    params.faceCount = 2;
    params.pointCount = 4;
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
    sdfMatSetTex(instance->material, texture);
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
        params.attr |= 0x1000;
        params.multiUv = (u32 *)secondaryUv;
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
        sdfMatSetMultiTex(instance->material, texture);
        break;
    }
    sdf3DPrimLinkMat((sdfDrawTag_t *)list, instance->material, 0);
    sdfDrawTagLink((sdfDrawTag_t *)list, (u32)effPkt3DPrimMakePoly(&params));
    surface = D_0034E0B0[instance->billboard->requestedPacketListIndex];
    surface->regFunc(surface, (sdfDrawTag_t *)list);
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
void effSubmitGeneratedTexturePacket(sdfGrOt_t *surface, EffGeneratedTextureDescriptor *source) {
    sdfDrawTag_t *packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EFF_PACKET_LIST_BYTES);

    sdfDrawTagInit(packetList);
    effDrawGeneratedTextureQuad(packetList, source);
    surface->regFunc(surface, packetList);
}

extern KwlnFrameDrawBuffer kwlnFrameDrawPacketRecords[];

extern void sdfGrSetFrameMaskRGB(const u8 *, SdfDmaReferenceChainPacket *, s32);

extern void sdfGrSetFrameMaskReset(const u8 *, SdfDmaReferenceChainPacket *, s32);

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
    sdfGrSetFrameMaskRGB(kwlnFrameDrawPacketRecords[dds3GetFrameID()].baseBytes, framePacket, 0);
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
    sdfGrSetFrameMaskReset(kwlnFrameDrawPacketRecords[dds3GetFrameID()].baseBytes, framePacket, 0);
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
void effSubmitCompositeGsPacket(sdfGrOt_t *surface, EffCompositeGsDescriptor *source) {
    sdfDrawTag_t *packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EFF_PACKET_LIST_BYTES);

    sdfDrawTagInit(packetList);
    effDrawCompositeTextureQuad(packetList, source);
    surface->regFunc(surface, packetList);
}
