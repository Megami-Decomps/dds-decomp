#include "sdk_ps2.h"
#include "sdf_gs_header.h"
#include "common.h"
#include "sdf_vu0.h"
#include "kwln_draw_buffer.h"
#include "kwln_frame_draw_buffer.h"
#include "bill_state_packet.h"
#include "sdf_trig.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "sdf_compact_vertex_vif_packet.h"
#include "bill_object_api.h"
#include "sdf_resource.h"
#include "sdf_texture_file.h"
#include "eff.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"
#include "sdf.h"
#include "sdf_texture_primary_buffer_api.h"

extern BillDispatch D_0034E060[];

void dds3BillboardTagLink(billHandle_t *obj, BillChildPayload *child);
BillData *billCreateAnimationDataFromResource(void *resourceData);

#include "sdf_packet_alloc.h"
typedef struct DmaPacketHeader DmaPacketHeader;
extern void sdf3DMakeDirectRefPacket(DmaPacketHeader *packet, u32 source, s32 bytes);
extern void sdfBillBoardMakeTMPacket(u8 *packet, const f32 *matrix);
extern f32 aViewTarget[4];
extern f32 sdfViewTargetVector[4];
extern f32 billChildFacingReferenceAxis[4];
extern f32 dds3BillboardTagParam[4];
extern f64 fabs(f64 value);
extern BillChildPayload *dds3BillboardTag;

/* Keep each child's pending list synchronized with the instance's draw mode,
 * then append one quad to its fifteen-record streams. */
void dds3BillboardTagLink(billHandle_t *obj, BillChildPayload *child) {
    f32 matrix[16];
    f32 direction[4];
    f32 dot;
    BillPacketWork *work;
    s32 selected;
    u32 packet;
    u32 *vertexPacket;
    u8 *geometry;
    s32 index;
    f32 x, y, halfWidth, halfHeight;
    f32 cosine, sine;
    f32 cornerX, cornerY;

    selected = child->packetListIndex;
    if (selected != obj->requestedPacketListIndex) {
        work = child->work;
        if (work->count > 0) {
            vertexPacket = sdfBillBoardMakeN((const u128 *)work->positions, work->colors,
                work->uv, work->offsets[0], work->count, 0);
            sdfDrawTagLink(child->pendingLists[selected], (u32)vertexPacket);
            work->count = 0;
        }
        selected = obj->requestedPacketListIndex;
        child->packetListIndex = selected;
    }
    if (child->pendingLists[selected] == NULL) {
        child->pendingLists[selected] = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(child->pendingLists[selected]);
        packet = sdfGrAllocPacketBuff(0x20);
        sdf3DMakeDirectRefPacket((DmaPacketHeader *)packet,
            (u32)sdfTexGetPacket(child->texture),
            sdfTexGetPacketLength(child->texture));
        sdfAppendReferencePacket(child->pendingLists[selected], packet);
        if ((u16)(child->variant & 1) != 0) {
            SDF_VU0_LOADVECTOR(aViewTarget);
            SDF_VU0_LOADVECTOR_B(sdfViewTargetVector);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_NORMALIZE();
            SDF_VU0_LOADVECTOR_B(billChildFacingReferenceAxis);
            SDF_VU0_INNERPRODUCT(dot);
            dds3BillboardTagParam[1] = 1.0 - fabs(dot);
            SDF_VU0_STOREVECTOR(direction);
            SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
            SDF_VU0_LOADIDENTITY();
            SDF_VU0_LOADVECTOR_BARRIER(dds3BillboardTagParam);
            SDF_VU0_SCALE_XYZ();
            VU0_STORE_MATRIX(matrix);
        } else {
            EE_MMI_UNIT_MATRIX(matrix);
        }
        geometry = (u8 *)sdfGrAllocPacketBuff(0x38);
        sdfBillBoardMakeTMPacket(geometry, matrix);
        sdfDrawTagLink(child->pendingLists[selected], (u32)geometry);
        if (child->next == child) {
            child->next = dds3BillboardTag;
            dds3BillboardTag = child;
        }
    }
    work = child->work;
    index = work->count;
    SDF_VU0_COPYVECTOR(work->positions[index], obj->position);
    work->colors[index] = obj->childParam;
    memcpy(&work->uv[index], &child->uv, sizeof(child->uv));
    x = child->x * obj->childScaleX;
    y = child->y * obj->childScaleY;
    halfWidth = child->halfWidth * obj->childScaleX;
    halfHeight = child->halfHeight * obj->childScaleY;
    if (obj->rotationAngle == 0.0f) {
        work->offsets[index][0] = x - halfWidth;
        work->offsets[index][1] = y - halfHeight;
        work->offsets[index][2] = x + halfWidth;
        work->offsets[index][3] = y - halfHeight;
        work->offsets[index][4] = x + halfWidth;
        work->offsets[index][5] = y + halfHeight;
        work->offsets[index][6] = x - halfWidth;
        work->offsets[index][7] = y + halfHeight;
    } else {
        cosine = sdfCos(obj->rotationAngle);
        sine = sdfSin(obj->rotationAngle);
        cornerX = x - halfWidth;
        cornerY = y - halfHeight;
        work->offsets[index][0] = cornerX * cosine - cornerY * sine;
        work->offsets[index][1] = cornerX * sine + cornerY * cosine;
        cornerX = x + halfWidth;
        cornerY = y - halfHeight;
        work->offsets[index][2] = cornerX * cosine - cornerY * sine;
        work->offsets[index][3] = cornerX * sine + cornerY * cosine;
        cornerX = x + halfWidth;
        cornerY = y + halfHeight;
        work->offsets[index][4] = cornerX * cosine - cornerY * sine;
        work->offsets[index][5] = cornerX * sine + cornerY * cosine;
        cornerX = x - halfWidth;
        cornerY = y + halfHeight;
        work->offsets[index][6] = cornerX * cosine - cornerY * sine;
        work->offsets[index][7] = cornerX * sine + cornerY * cosine;
    }
    work->count++;
    if (work->count == 15) {
        vertexPacket = sdfBillBoardMakeN((const u128 *)work->positions, work->colors,
            work->uv, work->offsets[0], 15, 0);
        sdfDrawTagLink(child->pendingLists[selected], (u32)vertexPacket);
        work->count = 0;
    }
}




extern BillChildPayload *dds3BillboardTag;
extern SdfPoolNode *D_0034E030[5];

void billFlushPendingChildPackets(void) {
    BillChildPayload *node;

    node = dds3BillboardTag;
    if (node != NULL) {
        do {
            BillPacketWork *work = node->work;
            s32 count = work->count;
            s32 listIndex;

            if (count > 0) {
                u32 *vertexPacket = sdfBillBoardMakeN((const u128 *)work->positions, work->colors, work->uv,
                                           work->offsets[0], count, 0);
                sdfDrawTagLink(node->pendingLists[node->packetListIndex], (u32)vertexPacket);
                work->count = 0;
            }

            for (listIndex = 0; listIndex < 5; listIndex++) {
                sdfDrawTag_t *value = node->pendingLists[listIndex];

                if (value != NULL) {
                    D_0034E030[listIndex]->append(D_0034E030[listIndex], value);
                    node->pendingLists[listIndex] = 0;
                }
            }

            {
                BillChildPayload *next = node->next;
                node->next = node;
                node = next;
            }
        } while (node != NULL);
    }
    dds3BillboardTag = NULL;
}

extern KwlnFrameDrawBuffer kwlnFrameDrawPacketRecords[];
extern void sdfBuildMaskedFrameRegisterReference(const u8 *source, SdfDmaReferenceChainPacket *packet, s32 gsContext);
extern f32 billPairFacingReferenceAxis[4];
extern f32 billPairFacingScale[4];
extern BillRenderPair *billPendingRenderPairList;
extern void dds3MultiBillboardPktLink(BillRenderPair *node);

/* Initialize the paired texture/state list, then append one quad to each
 * child stream. The second child owns the geometry and common batch count. */
void billAppendPairedQuadEntry(billHandle_t *obj, BillRenderPair *node) {
    f32 matrix[16];
    f32 direction[4];
    f32 dot;
    BillStatePacket *state;
    SdfDmaReferenceChainPacket *framePacket;
    u32 packet;
    u8 *geometry;
    BillChildPayload *child;
    BillPacketWork *work;
    s32 index;
    u32 primaryColor, secondaryColor;
    f32 x, y, halfWidth, halfHeight;
    f32 cosine, sine;
    f32 cornerX, cornerY;

    if (node->packetList == NULL) {
        node->packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(node->packetList);
        framePacket = (SdfDmaReferenceChainPacket *)sdfGrAllocPacketBuff(0x40);
        sdfBuildMaskedFrameRegisterReference(kwlnFrameDrawPacketRecords[dds3GetFrameID()].baseBytes, framePacket, 0);
        sdfDrawTagLinkRefGif(node->packetList, (u32)framePacket);
        state = (BillStatePacket *)sdfGrAllocPacketBuff(0x40);
        state->dmaTag = 3;
        state->vifCommands = 0x5000000310000000ULL;
        state->gifTag = 0x1000000000008002ULL;
        state->gifRegisters = 0xE;
        state->test = 0x51001;
        state->testRegister = 0x47;
        state->alpha = 0x44;
        state->alphaRegister = 0x42;
        sdfDrawTagLink(node->packetList, (u32)state);
        state = (BillStatePacket *)sdfGrAllocPacketBuff(0x40);
        state->dmaTag = 3;
        state->vifCommands = 0x5000000310000000ULL;
        state->gifTag = 0x1000000000008002ULL;
        state->gifRegisters = 0xE;
        state->test = 0x51001;
        state->testRegister = 0x48;
        switch (node->kind) {
        case 2:
            state->alpha = 0x58;
            break;
        case 3:
            state->alpha = 0x52;
            break;
        case 1:
            state->alpha = 0x54;
            break;
        }
        state->alphaRegister = 0x43;
        sdfDrawTagLink(node->packetList, (u32)state);
        packet = sdfGrAllocPacketBuff(0x20);
        sdf3DMakeDirectRefPacket((DmaPacketHeader *)packet,
            (u32)sdfTexGetPacket(node->children[0]->texture),
            sdfTexGetPacketLength(node->children[0]->texture));
        sdfAppendReferencePacket(node->packetList, packet);
        packet = sdfGrAllocPacketBuff(0x20);
        sdf3DMakeDirectRefPacket((DmaPacketHeader *)packet,
            (u32)sdfTexGetPacketCtx2(node->children[1]->texture),
            sdfTexGetPacketCtx2Length(node->children[1]->texture));
        sdfAppendReferencePacket(node->packetList, packet);
        geometry = (u8 *)sdfGrAllocPacketBuff(0x38);
        if (node->cameraFacingMode == 1) {
            SDF_VU0_LOADVECTOR(aViewTarget);
            SDF_VU0_LOADVECTOR_B(sdfViewTargetVector);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_NORMALIZE();
            SDF_VU0_LOADVECTOR_B(billPairFacingReferenceAxis);
            SDF_VU0_INNERPRODUCT(dot);
            billPairFacingScale[1] = 1.0 - fabs(dot);
            SDF_VU0_STOREVECTOR(direction);
            SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
            SDF_VU0_LOADIDENTITY();
            SDF_VU0_LOADVECTOR_BARRIER(billPairFacingScale);
            SDF_VU0_SCALE_XYZ();
            VU0_STORE_MATRIX(matrix);
        } else {
            EE_MMI_UNIT_MATRIX(matrix);
        }
        sdfBillBoardMakeTMPacket(geometry, matrix);
        sdfDrawTagLink(node->packetList, (u32)geometry);
        node->next = billPendingRenderPairList;
        billPendingRenderPairList = node;
    }
    child = node->children[1];
    work = child->work;
    index = work->count;
    primaryColor = node->colors[0];
    child = node->children[0];
    work = child->work;
    work->colors[index] = primaryColor;
    work->uv[index].components[0] = child->uv.components[0];
    work->uv[index].components[1] = child->uv.components[1];
    work->uv[index].components[2] = child->uv.components[2];
    work->uv[index].components[3] = child->uv.components[3];
    work->uv[index].components[4] = child->uv.components[4];
    work->uv[index].components[5] = child->uv.components[5];
    work->uv[index].components[6] = child->uv.components[6];
    work->uv[index].components[7] = child->uv.components[7];
    secondaryColor = node->colors[1];
    child = node->children[1];
    work = child->work;
    work->colors[index] = secondaryColor;
    work->uv[index].components[0] = child->uv.components[0];
    work->uv[index].components[1] = child->uv.components[1];
    work->uv[index].components[2] = child->uv.components[2];
    work->uv[index].components[3] = child->uv.components[3];
    work->uv[index].components[4] = child->uv.components[4];
    work->uv[index].components[5] = child->uv.components[5];
    work->uv[index].components[6] = child->uv.components[6];
    work->uv[index].components[7] = child->uv.components[7];
    x = child->x * obj->childScaleX;
    y = child->y * obj->childScaleY;
    halfWidth = child->halfWidth * obj->childScaleX;
    halfHeight = child->halfHeight * obj->childScaleY;
    if (obj->rotationAngle == 0.0f) {
        work->offsets[index][0] = x - halfWidth;
        work->offsets[index][1] = y - halfHeight;
        work->offsets[index][2] = x + halfWidth;
        work->offsets[index][3] = y - halfHeight;
        work->offsets[index][4] = x + halfWidth;
        work->offsets[index][5] = y + halfHeight;
        work->offsets[index][6] = x - halfWidth;
        work->offsets[index][7] = y + halfHeight;
    } else {
        cosine = sdfCos(obj->rotationAngle);
        sine = sdfSin(obj->rotationAngle);
        cornerX = x - halfWidth;
        cornerY = y - halfHeight;
        work->offsets[index][0] = cornerX * cosine - cornerY * sine;
        work->offsets[index][1] = cornerX * sine + cornerY * cosine;
        cornerX = x + halfWidth;
        cornerY = y - halfHeight;
        work->offsets[index][2] = cornerX * cosine - cornerY * sine;
        work->offsets[index][3] = cornerX * sine + cornerY * cosine;
        cornerX = x + halfWidth;
        cornerY = y + halfHeight;
        work->offsets[index][4] = cornerX * cosine - cornerY * sine;
        work->offsets[index][5] = cornerX * sine + cornerY * cosine;
        cornerX = x - halfWidth;
        cornerY = y + halfHeight;
        work->offsets[index][6] = cornerX * cosine - cornerY * sine;
        work->offsets[index][7] = cornerX * sine + cornerY * cosine;
    }
    SDF_VU0_COPYVECTOR(work->positions[index], obj->position);
    work->count++;
    if (work->count == 7) {
        dds3MultiBillboardPktLink(node);
    }
}

extern u32 *sdfMultiBillBoardMakeN(u128 *positions, void *attributes, void *halfAttributes,
                         void *wordAttributes, s32 count, void *(*alloc)(s32));

void dds3MultiBillboardPktLink(BillRenderPair *pair) {
    u32 colors[7][2];
    u16 uv[7][4][4];
    BillPacketWork *data[2];
    s32 count;
    s32 quadIndex;

    data[0] = pair->children[0]->work;
    data[1] = pair->children[1]->work;
    count = data[1]->count;
    for (quadIndex = 0; quadIndex < count; quadIndex++) {
        colors[quadIndex][0] = data[0]->colors[quadIndex];
        colors[quadIndex][1] = data[1]->colors[quadIndex];
        uv[quadIndex][0][0] = data[0]->uv[quadIndex].components[0];
        uv[quadIndex][0][1] = data[0]->uv[quadIndex].components[1];
        uv[quadIndex][1][0] = data[0]->uv[quadIndex].components[2];
        uv[quadIndex][1][1] = data[0]->uv[quadIndex].components[3];
        uv[quadIndex][2][0] = data[0]->uv[quadIndex].components[4];
        uv[quadIndex][2][1] = data[0]->uv[quadIndex].components[5];
        uv[quadIndex][3][0] = data[0]->uv[quadIndex].components[6];
        uv[quadIndex][3][1] = data[0]->uv[quadIndex].components[7];
        uv[quadIndex][0][2] = data[1]->uv[quadIndex].components[0];
        uv[quadIndex][0][3] = data[1]->uv[quadIndex].components[1];
        uv[quadIndex][1][2] = data[1]->uv[quadIndex].components[2];
        uv[quadIndex][1][3] = data[1]->uv[quadIndex].components[3];
        uv[quadIndex][2][2] = data[1]->uv[quadIndex].components[4];
        uv[quadIndex][2][3] = data[1]->uv[quadIndex].components[5];
        uv[quadIndex][3][2] = data[1]->uv[quadIndex].components[6];
        uv[quadIndex][3][3] = data[1]->uv[quadIndex].components[7];
    }
    sdfDrawTagLink(pair->packetList,
        (u32)sdfMultiBillBoardMakeN((u128 *)data[1]->positions, colors, uv,
                            data[1]->offsets, count, NULL));
    data[0]->count = 0;
    data[1]->count = 0;
}

extern SdfPoolNode D_00325228;
extern void sdfBuildFrameRegisterReference(const u8 *source, SdfDmaReferenceChainPacket *packet, s32 gsContext);

void billFlushPendingRenderPairs(void) {
    BillRenderPair *node = billPendingRenderPairList;
    sdfDrawTag_t *list;
    SdfDmaReferenceChainPacket *texture;
    BillStatePacket *packet;

    if (node != NULL) {
        do {
            BillPacketWork *work = node->children[1]->work;
            if (work->count != 0) {
                dds3MultiBillboardPktLink(node);
            }
            D_00325228.append(&D_00325228, node->packetList);
            node->packetList = NULL;
            node = node->next;
        } while (node != NULL);
    }
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    texture = (SdfDmaReferenceChainPacket *)sdfGrAllocPacketBuff(0x40);
    sdfBuildFrameRegisterReference(kwlnFrameDrawPacketRecords[dds3GetFrameID()].baseBytes, texture, 0);
    sdfDrawTagLinkRefGif(list, (u32)texture);
    packet = (BillStatePacket *)sdfGrAllocPacketBuff(0x40);
    packet->dmaTag = 3;
    packet->vifCommands = 0x5000000310000000ULL;
    packet->gifTag = 0x1000000000008002ULL;
    packet->gifRegisters = 0xE;
    packet->test = 0x71801;
    packet->testRegister = 0x47;
    packet->alpha = 0x48;
    packet->alphaRegister = 0x42;
    sdfDrawTagLink(list, (u32)packet);
    D_00325228.append(&D_00325228, list);
    billPendingRenderPairList = NULL;
}

void dds3Billboard_BasicInit(billHandle_t *billboard) {
    f32 zero = 0.0f;
    f32 one = 1.0f;
    billboard->requestedPacketListIndex = 0;
    billboard->position[3] = zero;
    billboard->position[2] = zero;
    billboard->position[1] = zero;
    billboard->position[0] = zero;
    billboard->unk18 = one;
    billboard->childScaleY = one;
    billboard->unk1C = zero;
    billboard->childScaleX = one;
    billboard->childParam = 0x80808080;
    billboard->rotationAngle = zero;
}

billHandle_t *billAllocChild(SdfTextureFileHeader *resourceData) {
    billHandle_t *obj;

    obj = sdfChipAllocDirect(0x34);
    obj->child = NULL;
    if (resourceData != NULL) {
        obj->child = BillboardLoadTex(resourceData);
    }
    return obj;
}

void billReleaseChild(billHandle_t *obj) {
    if (obj->child != NULL) {
        effReleaseSharedTextureRecord(obj->child);
    }
    sdfChipFreeDirect(obj);
}

void dds3Billboard_NormalDraw(billHandle_t *obj) {
    dds3BillboardTagLink(obj, obj->child);
}

billHandle_t *dds3Billboard_P2ACreate(void *resourceData) {
    BillData *data;
    billHandle_t *newobj;
    s32 n;

    data = NULL;
    if (resourceData != NULL) {
        data = billCreateAnimationDataFromResource(resourceData);
    }
    n = data->entryCount;
    newobj = sdfChipAllocDirect(n * 20 + 0x6C);
    newobj->animationData = data;
    newobj->resolvedEntries = (BillOut *)((u8 *)newobj + 0x6C);
    newobj->animationActive = 1;
    newobj->pair.packetList = 0;
    newobj->pair.next = 0;
    newobj->pair.cameraFacingMode = 0;
    dds3Billboard_P2ASetNumber(newobj, 0);
    return newobj;
}

billHandle_t *dds3Billboard_P2ACopy(const billHandle_t *obj) {
    BillData *data;
    s32 n;
    billHandle_t *newobj;

    data = obj->animationData;
    n = data->entryCount;
    data->listRefCount = data->listRefCount + 1;
    newobj = sdfChipAllocDirect(n * 20 + 0x6C);
    newobj->animationData = data;
    newobj->resolvedEntries = (BillOut *)((u8 *)newobj + 0x6C);
    newobj->animationActive = 1;
    newobj->pair.packetList = 0;
    newobj->pair.next = 0;
    dds3Billboard_P2ASetNumber(newobj, 0);
    return newobj;
}

void dds3Billboard_P2ADelete(billHandle_t *obj) {
    billReleaseSharedEntryBlock(obj->animationData);
    sdfChipFreeDirect(obj);
}

/* Advance an entry's frame timer and apply its current record to the shared
 * child payload. Plural descriptors are handled by the dispatcher instead. */
BillChildPayload *dds3Billboard_P2AAnimDraw(billHandle_t *obj, BillOut *out) {
    BillAnimationEntry *entry = out->entry;
    BillData *data = obj->animationData;
    BillRecord *record;
    BillChildPayload *child;
    s32 index;
    u32 color;

    if (entry->flags & BILL_ANIMATION_FLAG_PLURAL_ENTRIES) {
        return NULL;
    }
    if (out->framesRemaining <= 0) {
        out->frameIndex++;
        if ((u32)out->frameIndex >= entry->frameCount) {
            if (entry->flags & BILL_ANIMATION_FLAG_STOP_AT_END) {
                obj->animationActive = 0;
                out->frameIndex = entry->frameCount - 1;
            } else {
                out->frameIndex = 0;
            }
        }
        out->framesRemaining = out->record[out->frameIndex].frameDelay;
    } else {
        out->framesRemaining--;
    }
    index = out->frameIndex;
    record = out->record + index;
    if (entry->flags & BILL_ANIMATION_FLAG_FRAME_COLORS) {
        color = ((u32 *)(data->base + entry->colorOffset))[index];
    } else {
        color = 0x80808080;
    }
    obj->childParam = color;
    child = data->children[record->childIndex];
    if (entry->flags & BILL_ANIMATION_FLAG_PACKET_LIST_2) {
        obj->requestedPacketListIndex = 2;
    } else if (entry->flags & BILL_ANIMATION_FLAG_PACKET_LIST_3) {
        obj->requestedPacketListIndex = 3;
    } else {
        obj->requestedPacketListIndex = 1;
    }
    obj->rotationAngle = record->rotationAngle;
    child->uv.components[0] = record->u0;
    child->uv.components[1] = record->v0;
    child->uv.components[2] = record->u1;
    child->uv.components[3] = record->v0;
    child->uv.components[4] = record->u1;
    child->uv.components[5] = record->v1;
    child->uv.components[6] = record->u0;
    child->uv.components[7] = record->v1;
    {
        f32 halfHeight = record->height * 0.5f;
        f32 halfWidth = record->width * 0.5f;
        f32 y = halfHeight - record->y;
        f32 x = halfWidth - record->x;

        child->halfHeight = halfHeight;
        child->halfWidth = halfWidth;
        child->y = y;
        child->x = x;
    }
    return child;
}


/* vu0 routine: modulate two RGBA8888 colours, (a/128 * b/128) * 128 per channel */
u32 dds3Billboard_P2AColorMul(u32 colorA, u32 colorB) {
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit = 0x3C000000;
    color1[0] = colorA;
    EE_MMI_RGBA_UNPACK(color1, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    color2[0] = colorB;
    EE_MMI_RGBA_UNPACK(color2, unit);
    SDF_VU0_MULVECTOR();
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;
    return blended[0];
}

/* Indexed entries and restore order preserve the native PLUS and lifetimes;
 * see docs/idioms.md, "Trace-justified indexing: billboard update". */
void dds3Billboard_P2ADraw(billHandle_t *obj) {
    BillChildPayload *child;
    u32 savedColor;
    f32 savedAngle;
    if (obj->animationActive == 0) {
        return;
    }
    if (obj->modeFlags & BILL_ANIMATION_FLAG_PLURAL_ENTRIES) {
        BillData *data = obj->animationData;
        u8 *base = data->base;
        BillAnimationEntry *animation = data->entries + obj->animationEntryIndex;
        BillPluralRecord *records = (BillPluralRecord *)(base + animation->offset);
        u32 i = 0;
        savedColor = obj->childParam;
        savedAngle = obj->rotationAngle;

        if (obj->entryCount != 0) {
            BillPluralRecord *record = records;
            do {
                if (obj->resolvedEntries[i].frameIndex >= 0) {
                    f32 savedX;
                    f32 savedY;
                    child = dds3Billboard_P2AAnimDraw(obj, &obj->resolvedEntries[i]);
                    savedX = child->x;
                    savedY = child->y;
                    child->x = record->x;
                    child->y = -record->y;
                    obj->childParam = dds3Billboard_P2AColorMul(savedColor, obj->childParam);
                    obj->rotationAngle += savedAngle;
                    dds3BillboardTagLink(obj, child);
                    child->x = savedX;
                    child->y = savedY;
                } else {
                    obj->resolvedEntries[i].frameIndex++;
                }
                i++;
                record++;
            } while (i < (u32)obj->entryCount);
        }
        obj->rotationAngle = savedAngle;
        obj->childParam = savedColor;
    } else {
        if (obj->modeFlags & 0x40) {
            savedColor = obj->childParam;
            savedAngle = obj->rotationAngle;
            child = dds3Billboard_P2AAnimDraw(obj, obj->resolvedEntries);
            obj->childParam = dds3Billboard_P2AColorMul(savedColor, obj->childParam);
            obj->rotationAngle += savedAngle;
            dds3BillboardTagLink(obj, child);
            child = dds3Billboard_P2AAnimDraw(obj, obj->resolvedEntries + 1);
            obj->childParam = dds3Billboard_P2AColorMul(savedColor, obj->childParam);
            obj->rotationAngle += savedAngle;
            dds3BillboardTagLink(obj, child);
            obj->rotationAngle = savedAngle;
            obj->childParam = savedColor;
        } else if (obj->modeFlags & 0x80) {
            u32 flags;
            savedColor = obj->childParam;
            savedAngle = obj->rotationAngle;
            obj->pair.children[0] = dds3Billboard_P2AAnimDraw(obj, obj->resolvedEntries);
            obj->pair.colors[0] = dds3Billboard_P2AColorMul(savedColor, obj->childParam);
            obj->pair.children[1] = dds3Billboard_P2AAnimDraw(obj, obj->resolvedEntries + 1);
            obj->pair.colors[1] = dds3Billboard_P2AColorMul(savedColor, obj->childParam);
            flags = obj->resolvedEntries[1].entry->flags;
            if (flags & BILL_ANIMATION_FLAG_PACKET_LIST_2) {
                obj->pair.kind = 2;
            } else if (flags & BILL_ANIMATION_FLAG_PACKET_LIST_3) {
                obj->pair.kind = 3;
            } else {
                obj->pair.kind = 1;
            }
            obj->rotationAngle += savedAngle;
            billAppendPairedQuadEntry(obj, &obj->pair);
            obj->rotationAngle = savedAngle;
            obj->childParam = savedColor;
        } else {
            savedColor = obj->childParam;
            savedAngle = obj->rotationAngle;
            child = dds3Billboard_P2AAnimDraw(obj, obj->resolvedEntries);
            obj->childParam = dds3Billboard_P2AColorMul(savedColor, obj->childParam);
            obj->rotationAngle += savedAngle;
            dds3BillboardTagLink(obj, child);
            obj->rotationAngle = savedAngle;
            obj->childParam = savedColor;
        }
    }
}

/* Resolve an animation entry and initialize its signed frame-delay countdown. */
void dds3Billboard_P2ASetAnim(BillData *animationData, s32 entryIndex, BillOut *resolvedEntry) {
    u8 *base;
    BillAnimationEntry *entry;
    s32 offset;
    s16 value;

    base = animationData->base;
    entry = animationData->entries + entryIndex;
    offset = entry->offset;
    resolvedEntry->entry = entry;
    base = base + offset;
    resolvedEntry->frameIndex = 0;
    value = ((BillRecord *)base)->frameDelay;
    resolvedEntry->record = (BillRecord *)base;
    resolvedEntry->framesRemaining = value;
}

#include "sdf_debug_api.h"

enum {
    BILL_ANIMATION_ENTRY_MODE_MTEX = 0x00000040,
    BILL_ANIMATION_ENTRY_MODE_AMTEX = 0x00000080,
    BILL_ANIMATION_ENTRY_MODE_TWO_ENTRY_MASK = 0x000000C0,
    BILL_ANIMATION_ENTRY_MODE_P2A_POLYGON_LOG = 0x00000100
};

/* Select an animation and initialize the plural records' signed start delays. */
void dds3Billboard_P2ASetNumber(billHandle_t *obj, s32 animationEntryIndex) {
    BillData *data = obj->animationData;
    BillAnimationEntry *entry = data->entries + animationEntryIndex;

    if (entry->frameCount == 0) {
        obj->animationActive = 0;
        return;
    }
    if (entry->flags & BILL_ANIMATION_FLAG_PLURAL_ENTRIES) {
        BillPluralRecord *records;
        u32 i = 0;

        printf("billAnim..PLURAL SET\n");
        obj->modeFlags = 0x10000000;
        obj->animationEntryIndex = animationEntryIndex;
        obj->entryCount = entry->frameCount;
        records = (BillPluralRecord *)(data->base + entry->offset);
        for (; i < entry->frameCount; i++) {
            dds3Billboard_P2ASetAnim(data, records[i].entryIndex, obj->resolvedEntries + i);
            (obj->resolvedEntries)[i].frameIndex = -records[i].delay;
        }
    } else if (entry->entryModeFlags & BILL_ANIMATION_ENTRY_MODE_TWO_ENTRY_MASK) {
        printf("billAnim..(A)MTEX SET\n");
        obj->modeFlags = entry->entryModeFlags;
        obj->animationEntryIndex = animationEntryIndex;
        obj->entryCount = 2;
        dds3Billboard_P2ASetAnim(data, animationEntryIndex, obj->resolvedEntries);
        dds3Billboard_P2ASetAnim(data, animationEntryIndex + 1, obj->resolvedEntries + 1);
    } else {
        obj->modeFlags = 0;
        obj->entryCount = 1;
        obj->animationEntryIndex = animationEntryIndex;
        dds3Billboard_P2ASetAnim(data, animationEntryIndex, obj->resolvedEntries);
    }
    if (entry->entryModeFlags & BILL_ANIMATION_ENTRY_MODE_P2A_POLYGON_LOG) {
        printf("billAnim..P2A POLYGON\n");
    }
    obj->animationActive = 1;
}


BillData *billCreateAnimationDataFromResource(void *resourceData) {
    u8 *sourceBytes = resourceData;
    s32 *resourceHeader = resourceData;
    s32 *childOffsetCursor = (s32 *)(sourceBytes + resourceHeader[0]);
    s32 childCount = *childOffsetCursor++;
    sdfMemHandle_t *allocation;
    BillData *data;
    u8 *copiedBase;
    s32 entryCount;
    s32 resourceIndex;

    allocation = sdfMemAllocDirect(resourceHeader[0] + childCount * 0x50 + sizeof(*data));
    data = (BillData *)sdfMemLock(allocation);
    copiedBase = (u8 *)(data + 1);
    data->allocation = allocation;
    memcpy(copiedBase, sourceBytes, resourceHeader[0]);
    data->base = copiedBase;
    data->childCount = childCount;
    data->entries = (BillAnimationEntry *)(copiedBase + 8);
    data->children = (BillChildPayload **)(copiedBase + *(s32 *)copiedBase + 8);
    for (resourceIndex = 0; resourceIndex < childCount; resourceIndex++) {
        data->children[resourceIndex] = BillboardLoadTex(
            (SdfTextureFileHeader *)(sourceBytes + *childOffsetCursor++));
    }
    data->entryCount = data->listRefCount = 1;
    entryCount = ((s32 *)data->base)[1];
    for (resourceIndex = 0; resourceIndex < entryCount; resourceIndex++) {
        BillAnimationEntry *entry = &data->entries[resourceIndex];
        if (entry->flags & BILL_ANIMATION_FLAG_PLURAL_ENTRIES) {
            data->entryCount = entry->frameCount;
            printf("billAnim no[%d][%d]...PLURAL\n", resourceIndex, data->entryCount);
        } else {
            if (entry->entryModeFlags & BILL_ANIMATION_ENTRY_MODE_MTEX) {
                data->entryCount = 2;
                printf("billAnim no[%d][%d]...MTEX\n", resourceIndex, 2);
            } else if (entry->entryModeFlags & BILL_ANIMATION_ENTRY_MODE_AMTEX) {
                data->entryCount = 2;
                printf("billAnim no[%d][%d]...AMTEX\n", resourceIndex, 2);
            }
        }
    }
    return data;
}


/* Drop one reference; the last one releases every entry and the block itself. */
void billReleaseSharedEntryBlock(BillData *block) {
    s32 childIndex;

    block->listRefCount--;
    if (block->listRefCount == 0) {
        childIndex = 0;
        while (childIndex < block->childCount) {
            effReleaseSharedTextureRecord(block->children[childIndex]);
            childIndex++;
        }
        sdfMemFreeDirect(block->allocation);
    }
}



/* Copy the billboard's current source record (by kind) into a snapshot. */
void billCopyCurrentRecordToSnapshot(billHandle_t *obj, BillSnapshot *snapshot) {
    BillChildPayload *record;

    if (obj->kind == 1) {
        record = dds3Billboard_P2AAnimDraw(obj, obj->resolvedEntries);
    } else if (obj->kind == 0 || obj->kind == 3) {
        record = obj->child;
    } else {
        return;
    }
    snapshot->x = record->x;
    snapshot->texture = record->texture;
    snapshot->y = record->y;
    snapshot->halfWidth = record->halfWidth;
    snapshot->halfHeight = record->halfHeight;
    memcpy(&snapshot->uv, &record->uv, sizeof(snapshot->uv));
}

billHandle_t *dds3BillboardCreate(s32 index, u32 data) {
    billHandle_t *newobj;

    newobj = D_0034E060[index].func(data);
    dds3Billboard_BasicInit(newobj);
    newobj->kind = index;
    newobj->callback = D_0034E060[index].callback;
    return newobj;
}

billHandle_t *billCreateFromResource(s32 kind, const char *path) {
    struct sdfMemHandle_t *allocation;
    billHandle_t *billboard;
    u32 header[4];

    allocation = sdfDevLoadFileEx(path, header, 0);
    billboard = dds3BillboardCreate(kind, header[0]);
    sdfMemFreeDirect(allocation);
    return billboard;
}

/* Duplicate a billboard object: an entry list is cloned, a child shares (and refs) the source's data block. */
billHandle_t *dds3BillboardCopy(const billHandle_t *source) {
    billHandle_t *copy;
    BillChildPayload *data;
    u16 sourceKind;
    void (*sourceCallback)();

    if (source->kind == 1) {
        copy = dds3Billboard_P2ACopy(source);
        dds3Billboard_BasicInit(copy);
        copy->kind = source->kind;
        copy->callback = source->callback;
    } else {
        copy = billAllocChild(NULL);
        dds3Billboard_BasicInit(copy);
        sourceKind = source->kind;
        data = source->child;
        sourceCallback = source->callback;
        copy->kind = sourceKind;
        copy->callback = sourceCallback;
        data->refCount = data->refCount + 1;
        copy->child = data;
    }
    return copy;
}

void dds3BillboardDelete(billHandle_t *obj) {
    D_0034E060[obj->kind].destroy(obj);
}

void dds3BillboardDraw(billHandle_t *obj) {
    obj->callback(obj);
}
