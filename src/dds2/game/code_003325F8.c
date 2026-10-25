#include "sdk_ps2.h"
#include "sdf_request.h"
#include "sdf_gs_blend.h"
#include "common.h"
#include "sdf_vu0.h"
#include "sdf_resource_list_release_api.h"
#include "sdf_vu_matrix.h"
#include "sdf_trig.h"
#include "sdf_asset_packets.h"
#include "sdf_dma_tag.h"
#include "sdf_model_scalars.h"
#include "sdf_unique_chunk_value_api.h"
#include "sdf_packet_list.h"
#include "sdf_chip.h"
#include "pcp_vu0.h"

#include "sdf.h"
#include "sdf_model_copy.h"
#include "sdf_asset_state.h"
#include "sdf_pending.h"
#include "sdf_queued_resource_api.h"
#include "sdf_draw.h"
#include "sdf_resource_list_item.h"
#include "sdf_chunk.h"
#include "sdf_draw_packet_groups.h"
#include "sdf_motion.h"
#include "sdf_motion_node_api.h"
#include "sdf_material_asset_list.h"

extern u32 sdfForcedAssetTextureMode;


#define SDF_CHUNK_NAMED_IDS 0x4D4E444E
#define SDF_ASSET_LIST_MIN_CAPACITY 0x20

/* Four change groups, with adjacent bits for the two draw entries. */
#define SDF_ASSET_PRIMARY_STATE_BIT 1
#define SDF_ASSET_PRIMARY_SCALARS_BIT 4
#define SDF_ASSET_SECONDARY_STATE_BIT 16
#define SDF_ASSET_PAIR_STATE_BIT 64
#define SDF_ASSET_ENTRY_CHANGE_BITS 0x55
#define SDF_ASSET_PRIMARY_STATE_DIRTY 3
#define SDF_ASSET_PRIMARY_SCALARS_DIRTY 0xC
#define SDF_ASSET_SECONDARY_STATE_DIRTY 0x30
#define SDF_ASSET_PAIR_STATE_DIRTY 0xC0
#define SDF_ASSET_ALL_STATE_DIRTY 0xFF
#define SDF_ASSET_ENTRY_COUNT 2
#define SDF_TEXT_PAIR_OVERRIDE_CLEAR_MASK 0xFD
#define SDF_SUBPARAM_BYTES 0x18
#define SDF_ASSET_BYTES 0x48
#define SDF_ASSET_DRAW_ENTRY_BYTES 0xA0
#define SDF_RESOURCE_LIST_ITEM_BYTES 4
#define SDF_RESOURCE_LIST_GROWTH 4
#define SDF_FORCED_TEXTURE_ZERO_STATE_MODE 1
#define SDF_NODE_SOURCE_WORD_MASK 0x0FFFFFFF
#define SDF_NODE_HEADER_BYTES 0x10

/* Serialized presence flags are distinct from the draw-entry change bits. */
#define SDF_PARAM_PRIMARY_WORD_FIRST_PRESENT 0x1
#define SDF_PARAM_PRIMARY_WORD_SECOND_PRESENT 0x2
#define SDF_PARAM_PRIMARY_TEXTURE_PRESENT 0x4
#define SDF_PARAM_PRIMARY_SCALARS_PRESENT 0x8
#define SDF_PARAM_SECONDARY_COLOR_PRESENT 0x10
#define SDF_PARAM_SECONDARY_TEXTURE_STATE_PRESENT 0x20
#define SDF_PARAM_SECONDARY_SCALARS_PRESENT 0x40
#define SDF_PARAM_PRIMARY_WORD_THIRD_PRESENT 0x80
#define SDF_PARAM_PRIMARY_WORD_FOURTH_PRESENT 0x100
#define SDF_PARAM_PRIMARY_FLOAT_PRESENT 0x200
#define SDF_PARAM_SCALAR_PAIR_PRESENT 0x400
#define SDF_PARAM_HEADER_BYTES 8
#define SDF_PARAM_WORD_BYTES 4
#define SDF_PARAM_SCALAR_BLOCK_BYTES 0x14
#define SDF_PARAM_SCALAR_PAIR_BYTES 8
#define SDF_PARAM_TEXTURE_INDEX_MASK 0xFFFF
#define SDF_PARAM_PACKET_MODE_SHIFT 16


extern f32 sdfFaceDivSoften;

extern f32 sdfFaceDivBias;

extern SdfPendingRequest sdfMatDelayedResrcCB;

extern SdfPendingRequest sdfMatDelayedResrcCB2;


void sdfMatListFree(DevRequest *);

extern SdfSubParam *sdfMatAllocTexUvwParm(void);




extern s32 sdfMatAllocCount;


void sdfMatFree(sdfMat_t *);


void sdfAssetCopyTextureState(sdfMat_t *, SdfAssetEntry *);

void sdfMatUpdatePacketMultiTex(sdfMat_t *, SdfAssetEntry *);

void sdfTexBlendPaletteToTintedIntensityVU(sdfTexHandle_t *texture, s32 tint, f32 amount);

void sdfTexReleaseReferenceViaHandler(sdfTexHandle_t *texture);

extern vu8 sdfFrameId;
extern void sdfMatListCopyStat(DevRequest *, DevRequest *);


void sdfModelCopyStat(sdfModelRootNode_t *destination, sdfModelRootNode_t *source) {
    sdfModelNode_t **destinationNodes;
    sdfModelNode_t **sourceNodes;
    s32 count;
    s32 buffer;
    s32 i;

    sdfMatListCopyStat(destination->resources, source->resources);
    sdfModelMorphListCopyStat(destination->slotPairs, source->slotPairs);
    destination->flags = source->flags;
    destination->drawFlags = source->drawFlags;
    destination->color = source->color;
    VU0_LOAD_MATRIX(source->matrix);
    VU0_STORE_MATRIX(destination->matrix);
    SDF_VU0_COPYVECTOR(destination->rotationQuaternion, source->rotationQuaternion);
    SDF_VU0_COPYVECTOR(destination->scaleVector, source->scaleVector);
    buffer = (s8)sdfFrameId;
    destination->lighting = source->lighting;
    destination->scalarOverrideFirst = source->scalarOverrideFirst;
    destination->secondaryLighting = source->secondaryLighting;
    destination->scalarOverrideSecond = source->scalarOverrideSecond;
    count = destination->list->usedCount;
    destinationNodes = destination->list->buffer;
    sourceNodes = source->list->buffer;
    i = 0;
    if (count > 0) {
        s32 bufferOffset = buffer * 0x80;
        for (; i < count; i++) {
            sdfModelNode_t *destinationNode = destinationNodes[i];
            sdfModelNode_t *sourceNode = sourceNodes[i];

            destinationNode->flags = sourceNode->flags;
            destinationNode->color = sourceNode->color;
            destinationNode->drawFlagMask = sourceNode->drawFlagMask;
            SDF_VU0_COPYVECTOR(destinationNode->quaternion, sourceNode->quaternion);
            SDF_VU0_COPYVECTOR(destinationNode->translation, sourceNode->translation);
            SDF_VU0_COPYVECTOR(destinationNode->scale, sourceNode->scale);
            VU0_LOAD_MATRIX(sourceNode->localMatrix);
            VU0_STORE_MATRIX(destinationNode->localMatrix);
            VU0_LOAD_MATRIX(sourceNode->worldMatrix);
            VU0_STORE_MATRIX(destinationNode->worldMatrix);
            if (destinationNode->workspace != 0) {
                memcpy((u8 *)destinationNode->workspace + bufferOffset,
                       (u8 *)sourceNode->workspace + bufferOffset, 0x80);
            }
        }
    }
}

/* Initialize one draw-group record of a frame draw bank. */
void sdfModelSetDrawEnv(SdfDrawPacketGroups *ctx) {
    SdfDrawPacketGroup *packet = ctx->groups;
    s32 i;

    sdfPrimSetOpaquePacket(&ctx->groups[0].packet.blend);
    sdfPrimSetSemiTransPacket(&ctx->groups[1].packet.blend);
    sdfPrimSetAddTransPacket(&ctx->groups[2].packet.blend);
    sdfPrimSetSubTransPacket(&ctx->groups[3].packet.blend);
    for (i = 0; i != 4; i++) {
        /* Replace only the first VIF word; preserve the builder's DIRECT word. */
        packet->packet.dma.firstVifCode = 0x11000000;
        sdfDrawTagInit(&packet->list);
        sdfDrawTagLink(&packet->list, (u32)&packet->packet.blend);
        packet++;
    }
    sdfDrawTagInit(&ctx->syncList);
    ctx->syncPacketWords[0] = 0;
    ctx->syncPacketWords[1] = 0x13000000;
    sdfDrawTagLink(&ctx->syncList, (u32)&ctx->syncPacketWords[0]);
}

/* Submit the four draw lists, then the trailing list to the fourth owner. */
void sdfModelDrawEnvLinkOt(SdfPoolNode **owners, SdfDrawPacketGroups *packets) {
    s32 i;

    for (i = 0; i != 4; i++) {
        owners[i]->prepend(owners[i], 1, &packets->groups[i].list);
    }
    owners[3]->append(owners[3], &packets->syncList);
}

/* Follow chunk byte extents until the requested ID or the zero-ID terminator is reached. */
SdfChunkHeader *sdfChunkFindById(SdfChunkHeader *chunk, s32 chunkId) {
    u32 currentId;

    if (chunk == NULL) {
        return NULL;
    }
    currentId = chunk->id;
    while (currentId != 0) {
        if (currentId == chunkId) {
            return chunk;
        }
        chunk = (SdfChunkHeader *)((u8 *)chunk + chunk->size);
        currentId = chunk->id;
    }
    return NULL;
}

SdfChunkHeader *sdfChunkFindByTag(const sdfModelRootNode_t *model, s32 tag) {
    return sdfChunkFindById(model->chunkTable, tag);
}


/* Names are followed by a four-byte-aligned ID word; missing chunks/names return -1. */
s32 sdfModelGetNodeIndexByName(sdfModelRootNode_t *model, const char *name) {
    SdfChunkHeader *namesChunk = sdfChunkFindByTag(model, SDF_CHUNK_NAMED_IDS);
    u8 *entryName;
    u8 *chunkEnd;
    u32 nameLength;
    if (namesChunk == NULL) {
        return -1;
    }
    entryName = (u8 *)namesChunk + 8;
    chunkEnd = (u8 *)namesChunk + namesChunk->size;
    nameLength = strlen(name);
    do {
        u32 entryNameLength = strlen((char *)entryName);
        u8 *idWord = (u8 *)(((u32)(entryName + entryNameLength + 4)) & ~3U);
        if (entryNameLength == nameLength && memcmp(entryName, name, nameLength) == 0) {
            return *(s32 *)idWord;
        }
        entryName = idWord + 4;
    } while (entryName < chunkEnd);
    return -1;
}

extern const char D_0040B338[];

const char *sdfModelGetNodeName(sdfModelRootNode_t *model, s32 nameId) {
    SdfChunkHeader *chunk = sdfChunkFindByTag(model, SDF_CHUNK_NAMED_IDS);
    const u8 *name;
    const u8 *end;
    if (chunk == NULL) {
        return D_0040B338;
    }
    name = (const u8 *)(chunk + 1);
    end = (const u8 *)chunk + chunk->size;
    do {
        const u8 *cursor = name;
        const s32 *id;
        while (*cursor++ != 0) {
        }
        id = (const s32 *)(((u32)cursor + 3) & ~3U);
        if (*id == nameId) {
            return (const char *)name;
        }
        name = (const u8 *)(id + 1);
        if (name >= end) {
            return D_0040B338;
        }
    } while (1);
}

u32 sdfModelGetMarkPosCount(const sdfModelRootNode_t *model) {
    const SdfChunkHeader *chunk = sdfChunkFindByTag(model, SDF_CHUNK_MAP_POSITIONS);
    if (chunk != NULL) {
        return (chunk->size - 0x10) >> 6;
    }
    return 0;
}

/* vu0 routine: build basis rows in vf28-vf31 from the map record, then
 * postmultiply using its draw-node matrix. */
void sdfModelMakeMarkPosMatrix(sdfModelRootNode_t *model, SdfMapPositionRecord *record) {
    sdfModelNode_t *drawNode = sdfModelSearchNode(model, record->nodeId);
    u8 *vector;

    vector = (u8 *)&record->up;
    SDF_VU0_LOADVECTOR(vector);
    SDF_VU0_SET_ROW2();
    SDF_VU0_COPYVECTOR_A_TO_B();
    vector = (u8 *)&record->direction;
    SDF_VU0_LOADVECTOR(vector);
    SDF_VU0_NEGVECTOR_XYZ();
    SDF_VU0_SET_ROW1();
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_SET_ROW0();
    SDF_VU0_LOAD_ROW3(&record->position);
    SDF_VU0_MATRIX_ROW3_W_SET_ONE();
    sdfPostmultiplyVuMatrixFromMemory((const struct Matrix4 *)drawNode->worldMatrix);
}

/* vu0 routine: transform the map-record position by its draw-node matrix;
 * the transformed position is left in vf10. */
void sdfModelMakeMarkPos(sdfModelRootNode_t *model, SdfMapPositionRecord *record) {
    sdfModelNode_t *drawNode = sdfModelSearchNode(model, record->nodeId);

    VU0_LOAD_MATRIX(drawNode->worldMatrix);
    SDF_VU0_LOADVECTOR(&record->position);
    SDF_VU0_APPLYMATRIX_XYZ1();
}

/* Search the fixed-size map-position records within the chunk's declared byte extent. */
SdfMapPositionRecord *sdfModelSearchMarkPosData(sdfModelRootNode_t *model, s32 recordId) {
    SdfChunkHeader *positionsChunk = sdfChunkFindByTag(model, SDF_CHUNK_MAP_POSITIONS);
    SdfMapPositionChunkPrefix *positions;
    SdfMapPositionRecord *record;
    u8 *chunkEnd;
    if (positionsChunk == NULL) {
        return NULL;
    }
    positions = (SdfMapPositionChunkPrefix *)positionsChunk;
    record = (SdfMapPositionRecord *)(positions + 1);
    chunkEnd = (u8 *)positionsChunk + positionsChunk->size;
    while ((u8 *)record < chunkEnd) {
        if (record->id == recordId) {
            return record;
        }
        record++;
    }
    return NULL;
}

s32 sdfModelGetMarkPosMatrix(sdfModelRootNode_t *model, s32 mapRecordId) {
    SdfMapPositionRecord *resource = sdfModelSearchMarkPosData(model, mapRecordId);
    if (resource != NULL) {
        sdfModelMakeMarkPosMatrix(model, resource);
        return 1;
    }
    return 0;
}

s32 sdfModelGetMarkPos(sdfModelRootNode_t *model, s32 mapRecordId) {
    SdfMapPositionRecord *resource = sdfModelSearchMarkPosData(model, mapRecordId);
    if (resource != NULL) {
        sdfModelMakeMarkPos(model, resource);
        return 1;
    }
    return 0;
}

u32 sdfGetUniqueChunkValue(const sdfModelRootNode_t *model) {
    const SdfChunkHeader *chunk = sdfChunkFindByTag(model, SDF_CHUNK_UNIQUE_VALUE);
    if (chunk != NULL) {
        return *(const u32 *)((const u8 *)chunk + sizeof(*chunk));
    }
    return 0;
}

u32 sdfGetLodChunkValue(sdfModelRootNode_t *model) {
    SdfChunkHeader *chunk = sdfChunkFindByTag(model, SDF_CHUNK_LOD_VALUE);
    if (chunk != NULL) {
        return *(u32 *)((u8 *)chunk + sizeof(*chunk));
    }
    return 0;
}

/* Store the override pair and select it instead of the shared defaults. */
void sdfModelSetFaceDivParm(sdfModelRootNode_t *model, f32 first, f32 second) {
    model->scalarOverrideFirst = first;
    model->scalarOverrideSecond = second;
    model->flags = model->flags | SDF_MODEL_USE_SCALAR_OVERRIDES;
}

/* Clear only the pair-override selection bit. */
void sdfModelResetFaceDivParm(sdfModelRootNode_t *model) {
    model->flags = model->flags & SDF_TEXT_PAIR_OVERRIDE_CLEAR_MASK;
}

/* Return the first override value when selected, otherwise its shared default. */
f32 sdfModelGetFaceDivSoften(sdfModelRootNode_t *model) {
    if ((model->flags & SDF_MODEL_USE_SCALAR_OVERRIDES) != 0) {
        return model->scalarOverrideFirst;
    }
    return sdfFaceDivSoften;
}

/* Return the second override value when selected, otherwise its shared default. */
f32 sdfModelGetFaceDivBias(sdfModelRootNode_t *model) {
    if ((model->flags & SDF_MODEL_USE_SCALAR_OVERRIDES) != 0) {
        return model->scalarOverrideSecond;
    }
    return sdfFaceDivBias;
}

/* Set the native mode word; its consumers distinguish mode one from other values. */
void sdfSetForcedAssetTextureMode(u32 mode) {
    sdfForcedAssetTextureMode = mode;
}

/* Create a four-byte-item buffer with growth in groups of four slots. */
DevRequest *sdfTexListNew(s32 capacity) {
    return sdfDevCreateBufferedRequest(capacity, SDF_RESOURCE_LIST_ITEM_BYTES, SDF_RESOURCE_LIST_GROWTH);
}

/* Optionally release item references before destroying the buffered list; null lists are ignored. */
void sdfTexListFree(DevRequest *list, SdfTexListItemReferencePolicy itemReferencePolicy) {
    s32 itemIndex;
    s32 itemCount;
    if (list == NULL) {
        return;
    }
    if (itemReferencePolicy != SDF_TEX_LIST_KEEP_REFERENCES) {
        itemCount = list->usedCount;
        for (itemIndex = 0; itemIndex < itemCount; itemIndex++) {
            sdfTexReleaseReferenceViaHandler(((sdfTexHandle_t **)list->buffer)[itemIndex]);
        }
    }
    sdfItemArrayFree(list);
}

void sdfTexListAdd(DevRequest *list, sdfTexHandle_t *texture) {
    if (list->usedCount >= (s16)list->capacity) {
        sdfItemArrayGrow(list);
    }
    ((sdfTexHandle_t **)list->buffer)[list->usedCount] = texture;
    list->usedCount++;
}

void sdfTexListSetCapacity(DevRequest *list, s32 newCount, s32 applyReduction) {
    s32 countCursor;

    if ((newCount < list->usedCount) && (applyReduction != 0)) {
        /* Retail retains the counted NOP-body loop at +0x28..+0x40. */
        countCursor = newCount;
        do {
            countCursor++;
        } while (countCursor != list->usedCount);
        list->usedCount = newCount;
    }
    sdfDevResizeBufferedRequest(list, newCount);
}

extern sdfTexHandle_t *sdfTexClone(sdfTexHandle_t *);
/* Clone every texture reference into a new buffered list; null sources return null. */
DevRequest *sdfTexListDup(DevRequest *source) {
    s32 itemCount;
    DevRequest *clone;
    s32 itemIndex;

    if (source == NULL) {
        return NULL;
    }
    itemCount = source->usedCount;
    clone = sdfTexListNew(itemCount);
    for (itemIndex = 0; itemIndex < itemCount; itemIndex++) {
        ((sdfTexHandle_t **)clone->buffer)[itemIndex] =
            sdfTexClone(((sdfTexHandle_t **)source->buffer)[itemIndex]);
    }
    clone->usedCount = itemCount;
    return clone;
}

/* Apply the palette update only to textures with a nonempty color lookup table. */
void sdfTexListSetClutColor(DevRequest *list, s32 tint, f32 amount) {
    s32 itemIndex;
    s32 itemCount;

    if (list == NULL) {
        return;
    }
    itemCount = list->usedCount;
    for (itemIndex = 0; itemIndex < itemCount; itemIndex++) {
        sdfTexHandle_t *texture = ((sdfTexHandle_t **)list->buffer)[itemIndex];

        if (texture->paletteCount != 0) {
            sdfTexBlendPaletteToTintedIntensityVU(texture, tint, amount);
        }
    }
}

void sdfMatInitSystem(void) {
    sdfDelayedResrcInitCB(&sdfMatDelayedResrcCB, sdfMatListFree);
    sdfDelayedResrcInitCB(&sdfMatDelayedResrcCB2, sdfMatFree);
}

DevRequest *sdfMatListNew(s32 capacity) {
    return sdfDevCreateBufferedRequest(capacity, 4, 8);
}

/* Release every asset, then destroy its buffered list. */
void sdfMatListFree(DevRequest *list) {
    s32 itemIndex;

    for (itemIndex = 0; itemIndex < list->usedCount; itemIndex++) {
        sdfMatFree(((sdfMat_t **)list->buffer)[itemIndex]);
    }
    sdfItemArrayFree(list);
}

void sdfMatListFreeResrc(DevRequest *resource, s32 retained) {
    if (resource == NULL) {
        return;
    }
    if (retained != 0) {
        sdfDelayedResrcEntry(&sdfMatDelayedResrcCB, resource);
    } else {
        sdfItemArrayFree(resource);
    }
}

void sdfMatListGrow(DevRequest *list) {
    sdfItemArrayGrow(list);
}

void sdfMatListAdd(DevRequest *list, sdfMat_t *asset) {
    if (list->usedCount >= (s16)list->capacity) {
        sdfMatListGrow(list);
    }
    ((sdfMat_t **)list->buffer)[list->usedCount] = asset;
    list->usedCount++;
}

/* Store the first primary-state word and dirty both draw entries. */
void sdfMatSetAmbient(sdfMat_t *asset, u32 stateWord) {
    asset->primaryStateWordFirst = stateWord;
    asset->dirtyFlags = asset->dirtyFlags | SDF_ASSET_PRIMARY_STATE_DIRTY;
}

/* Store the second primary-state word and dirty both draw entries. */
void sdfMatSetDiffuse(sdfMat_t *asset, u32 stateWord) {
    asset->primaryStateWordSecond = stateWord;
    asset->dirtyFlags = asset->dirtyFlags | SDF_ASSET_PRIMARY_STATE_DIRTY;
}

/* Store the third primary-state word and dirty both draw entries. */
void sdfMatSetSpecular(sdfMat_t *asset, u32 stateWord) {
    asset->primaryStateWordThird = stateWord;
    asset->dirtyFlags = asset->dirtyFlags | SDF_ASSET_PRIMARY_STATE_DIRTY;
}

/* Store raw bits in the fourth primary-state word, without float conversion. */
void sdfSetPrimaryStateWordFourth(sdfMat_t *asset, u32 bits) {
    asset->primaryStateWordFourth = bits;
    asset->dirtyFlags = asset->dirtyFlags | SDF_ASSET_PRIMARY_STATE_DIRTY;
}

/* Store the primary-state float and dirty both draw entries. */
void sdfMatSetConstant(sdfMat_t *asset, f32 value) {
    asset->primaryStateFloat = value;
    asset->dirtyFlags = asset->dirtyFlags | SDF_ASSET_PRIMARY_STATE_DIRTY;
}

/* Store the primary texture address as raw bits and dirty both draw entries. */
void sdfSetAssetPrimaryTextureAddress(sdfMat_t *asset, sdfTexHandle_t *texture) {
    asset->texture = texture;
    asset->dirtyFlags = asset->dirtyFlags | SDF_ASSET_PRIMARY_STATE_DIRTY;
}

/* Initialize packed scalar values 0,0,1,1,0; the final opaque word is untouched. */
SdfSubParam *sdfMatAllocTexUvwParm(void) {
    SdfSubParam *subParameter;

    subParameter = sdfChipAllocDirect(SDF_SUBPARAM_BYTES);
    /* Packed defaults: center offsets (0, 0), basis scales (1, 1), rotation 0. */
    subParameter->packed.unk8 = 0x3F8000003F800000ULL;
    subParameter->packed.unk0 = 0;
    subParameter->packed.unk10 = 0;
    return subParameter;
}

/* Lazily create the primary scalar block; this does not itself dirty an entry. */
SdfSubParam *sdfMatGetTexUvwParm(sdfMat_t *asset) {
    SdfSubParam *subParameter = asset->primarySubParam;
    if (subParameter == NULL) {
        subParameter = sdfMatAllocTexUvwParm();
        asset->primarySubParam = subParameter;
    }
    return subParameter;
}

/* Store the five primary scalars in order and dirty both draw entries. */
void sdfSetPrimaryTextScalars(sdfMat_t *asset, f32 centerOffsetX, f32 centerOffsetY,
                              f32 basisScaleX, f32 basisScaleY, f32 rotationInput) {
    f32 *scalarValues = sdfMatGetTexUvwParm(asset)->scalar.values;
    scalarValues[0] = centerOffsetX;
    scalarValues[1] = centerOffsetY;
    scalarValues[2] = basisScaleX;
    scalarValues[3] = basisScaleY;
    scalarValues[4] = rotationInput;
    asset->dirtyFlags |= SDF_ASSET_PRIMARY_SCALARS_DIRTY;
}

/* Copy five scalar values, leaving the block's opaque final word unchanged. */
void sdfMatSetTexUvwByPtr(sdfMat_t *asset, const f32 *sourceScalars) {
    f32 *scalarValues = sdfMatGetTexUvwParm(asset)->scalar.values;
    scalarValues[0] = sourceScalars[0];
    scalarValues[1] = sourceScalars[1];
    scalarValues[2] = sourceScalars[2];
    scalarValues[3] = sourceScalars[3];
    scalarValues[4] = sourceScalars[4];
    asset->dirtyFlags |= SDF_ASSET_PRIMARY_SCALARS_DIRTY;
}

/* Write the full packed secondary-color word and dirty both draw entries. */
void sdfMatSetMultiDiffuse(sdfMat_t *asset, u32 packedColor) {
    asset->secondaryColor = packedColor;
    asset->dirtyFlags = asset->dirtyFlags | SDF_ASSET_SECONDARY_STATE_DIRTY;
}

/* Store the mode used by the secondary packet's mode and palette lookup. */
void sdfMatSetMultiMethod(sdfMat_t *asset, u32 packetMode) {
    asset->secondaryMode = packetMode;
    asset->dirtyFlags = asset->dirtyFlags | SDF_ASSET_SECONDARY_STATE_DIRTY;
}

/* Store the secondary texture address and dirty both draw entries. */
void sdfMatSetMultiTex(sdfMat_t *asset, sdfTexHandle_t *texture) {
    asset->secondaryTexture = texture;
    asset->dirtyFlags = asset->dirtyFlags | SDF_ASSET_SECONDARY_STATE_DIRTY;
}

/* Lazily create the secondary scalar block; this does not itself dirty an entry. */
SdfSubParam *sdfMatGetMultiTexUvwParm(sdfMat_t *asset) {
    SdfSubParam *subParameter = asset->secondarySubParam;
    if (subParameter == NULL) {
        subParameter = sdfMatAllocTexUvwParm();
        asset->secondarySubParam = subParameter;
    }
    return subParameter;
}

/* Store the five secondary scalars in order and dirty both draw entries. */
void sdfMatSetMultiTexUvw(sdfMat_t *asset, f32 centerOffsetX, f32 centerOffsetY,
                                f32 basisScaleX, f32 basisScaleY, f32 rotationInput) {
    f32 *scalarValues = sdfMatGetMultiTexUvwParm(asset)->scalar.values;
    scalarValues[0] = centerOffsetX;
    scalarValues[1] = centerOffsetY;
    scalarValues[2] = basisScaleX;
    scalarValues[3] = basisScaleY;
    scalarValues[4] = rotationInput;
    asset->dirtyFlags |= SDF_ASSET_SECONDARY_STATE_DIRTY;
}

/* Copy five scalar values, leaving the block's opaque final word unchanged. */
void sdfMatSetMultiTexUvwByPtr(sdfMat_t *asset, const f32 *sourceScalars) {
    f32 *scalarValues = sdfMatGetMultiTexUvwParm(asset)->scalar.values;
    scalarValues[0] = sourceScalars[0];
    scalarValues[1] = sourceScalars[1];
    scalarValues[2] = sourceScalars[2];
    scalarValues[3] = sourceScalars[3];
    scalarValues[4] = sourceScalars[4];
    asset->dirtyFlags |= SDF_ASSET_SECONDARY_STATE_DIRTY;
}

/* Store the scalar pair and mark its corresponding group dirty for both entries. */
void sdfMatSetFaceDivParm(sdfMat_t *asset, f32 first, f32 second) {
    asset->scalarPairFirst = first;
    asset->scalarPairSecond = second;
    asset->dirtyFlags = asset->dirtyFlags | SDF_ASSET_PAIR_STATE_DIRTY;
}

/* Allocate both native draw-entry command blocks and mark every change group dirty. */
sdfMat_t *sdfMatNew(void) {
    sdfMat_t *asset;
    u32 *entryWords;
    s32 entryIndex;

    sdfMatAllocCount++;
    asset = sdfAllocAndClearQuadwords(SDF_ASSET_BYTES);
    asset->dirtyFlags = SDF_ASSET_ALL_STATE_DIRTY;
    for (entryIndex = 0; entryIndex != SDF_ASSET_ENTRY_COUNT; entryIndex++) {
        entryWords = sdfChipAllocDirect(SDF_ASSET_DRAW_ENTRY_BYTES);
        asset->drawEntries[entryIndex] = (SdfAssetEntry *)entryWords;
        entryWords[0] = 0x6E05C000;
        entryWords[0x18 / 4] = 0x6005C005;
        entryWords[0x30 / 4] = 0;
        entryWords[0x34 / 4] = 0x640CC00A;
        ((SdfAssetEntry *)entryWords)->vifItopCommand = 0x0400000C;
        ((SdfAssetEntry *)entryWords)->vifMscalCommand = 0x14000000;
    }
    asset->scalarPairFirst = 0;
    asset->scalarPairSecond = 0;
    asset->primaryStateWordFirst = 0x80808080;
    asset->primaryStateWordSecond = 0x80808080;
    asset->secondaryColor = 0x80808080;
    return asset;
}

/* Consume supported presence bits in their native order and return the next byte.
 * Texture indices and packet modes are unchecked; unknown bits consume no payload. */
u8 *sdfMatSet(sdfMat_t *asset, DevRequest *resourceLookup, u8 *serializedData) {
    u32 parameterFlags;
    u8 *parameterCursor;
    u32 packedTextureMode;

    asset->unk00 = *(u32 *)serializedData;
    asset->unk04 = *(u16 *)(serializedData + 4);
    parameterFlags = *(u16 *)(serializedData + 6);
    parameterCursor = serializedData + SDF_PARAM_HEADER_BYTES;
    if (parameterFlags & SDF_PARAM_PRIMARY_WORD_FIRST_PRESENT) {
        sdfMatSetAmbient(asset, *(u32 *)parameterCursor);
        parameterCursor += SDF_PARAM_WORD_BYTES;
    }
    if (parameterFlags & SDF_PARAM_PRIMARY_WORD_SECOND_PRESENT) {
        sdfMatSetDiffuse(asset, *(u32 *)parameterCursor);
        parameterCursor += SDF_PARAM_WORD_BYTES;
    }
    if (parameterFlags & SDF_PARAM_PRIMARY_TEXTURE_PRESENT) {
        sdfSetAssetPrimaryTextureAddress(asset, (sdfTexHandle_t *)((u32 *)resourceLookup->buffer)[*(u16 *)parameterCursor]);
        parameterCursor += SDF_PARAM_WORD_BYTES;
    }
    if (parameterFlags & SDF_PARAM_PRIMARY_SCALARS_PRESENT) {
        sdfSetPrimaryTextScalars(asset, ((f32 *)parameterCursor)[0], ((f32 *)parameterCursor)[1], ((f32 *)parameterCursor)[2], ((f32 *)parameterCursor)[3], ((f32 *)parameterCursor)[4]);
        parameterCursor += SDF_PARAM_SCALAR_BLOCK_BYTES;
    }
    if (parameterFlags & SDF_PARAM_SECONDARY_COLOR_PRESENT) {
        sdfMatSetMultiDiffuse(asset, *(u32 *)parameterCursor);
        parameterCursor += SDF_PARAM_WORD_BYTES;
    }
    if (parameterFlags & SDF_PARAM_SECONDARY_TEXTURE_STATE_PRESENT) {
        packedTextureMode = *(u32 *)parameterCursor;
        parameterCursor += SDF_PARAM_WORD_BYTES;
        sdfMatSetMultiTex(asset, (sdfTexHandle_t *)((u32 *)resourceLookup->buffer)[packedTextureMode & SDF_PARAM_TEXTURE_INDEX_MASK]);
        sdfMatSetMultiMethod(asset, packedTextureMode >> SDF_PARAM_PACKET_MODE_SHIFT);
    }
    if (parameterFlags & SDF_PARAM_SECONDARY_SCALARS_PRESENT) {
        sdfMatSetMultiTexUvw(asset, ((f32 *)parameterCursor)[0], ((f32 *)parameterCursor)[1], ((f32 *)parameterCursor)[2], ((f32 *)parameterCursor)[3], ((f32 *)parameterCursor)[4]);
        parameterCursor += SDF_PARAM_SCALAR_BLOCK_BYTES;
    }
    if (parameterFlags & SDF_PARAM_PRIMARY_WORD_THIRD_PRESENT) {
        sdfMatSetSpecular(asset, *(u32 *)parameterCursor);
        parameterCursor += SDF_PARAM_WORD_BYTES;
    }
    if (parameterFlags & SDF_PARAM_PRIMARY_WORD_FOURTH_PRESENT) {
        sdfSetPrimaryStateWordFourth(asset, *(u32 *)parameterCursor);
        parameterCursor += SDF_PARAM_WORD_BYTES;
    }
    if (parameterFlags & SDF_PARAM_PRIMARY_FLOAT_PRESENT) {
        sdfMatSetConstant(asset, *(f32 *)parameterCursor);
        parameterCursor += SDF_PARAM_WORD_BYTES;
    }
    if (parameterFlags & SDF_PARAM_SCALAR_PAIR_PRESENT) {
        sdfMatSetFaceDivParm(asset, ((f32 *)parameterCursor)[0], ((f32 *)parameterCursor)[1]);
        parameterCursor += SDF_PARAM_SCALAR_PAIR_BYTES;
    }
    return parameterCursor;
}

/* Release both draw entries and the parameter blocks, then the asset; null is ignored. */
void sdfMatFree(sdfMat_t *asset) {
    if (asset == NULL) {
        return;
    }
    sdfMatAllocCount--;
    sdfChipFreeDirect(asset->drawEntries[0]);
    sdfChipFreeDirect(asset->drawEntries[1]);
    sdfChipFreeDirect(asset->primarySubParam);
    sdfChipFreeDirect(asset->secondarySubParam);
    sdfChipFreeDirect(asset);
}

/* Queue the non-null asset address for deferred release. */
void sdfMatFreeDelayed(sdfMat_t *asset) {
    if (asset != NULL) {
        sdfDelayedResrcEntry(&sdfMatDelayedResrcCB2, asset);
    }
}

/* Reference the selected buffered asset draw entry and return the packet payload. */
SdfDmaTagHeader *sdfMatMakePacket(sdfMat_t *asset, SdfDmaTagHeader *tag, s32 frame) {
    tag->control = 0x30;
    tag->address = (u32)asset->drawEntries[frame] & SDF_NODE_SOURCE_WORD_MASK;
    tag->quadwordCount = 0xA;
    tag->firstVifCode = 0;
    tag->secondVifCode = 0;
    return (SdfDmaTagHeader *)((u8 *)tag + SDF_NODE_HEADER_BYTES);
}

/* Copy primary words and optional GS texture state; only mode one zeros its state word. */
void sdfAssetCopyTextureState(sdfMat_t *asset, SdfAssetEntry *entry) {
    sdfTexHandle_t *texture;
    entry->unk04 = asset->primaryStateWordFirst;
    entry->unk1C = asset->primaryStateFloat;
    entry->unk08 = asset->primaryStateWordSecond;
    if (sdfForcedAssetTextureMode == SDF_FORCED_TEXTURE_ZERO_STATE_MODE) {
        entry->primaryStateWordThird = 0;
    } else {
        entry->primaryStateWordThird = asset->primaryStateWordThird;
    }
    entry->unk14 = asset->primaryStateWordFourth;
    texture = asset->texture;
    if (texture != NULL) {
        entry->primaryTextureState.sampling = sdfTexGetTex1Reg(texture);
        entry->primaryTextureState.texture = sdfTexGetTex0Reg(texture);
        entry->primaryTextureState.clamp = sdfTexGetClampReg(texture);
    }
}



/* Build the 3x2 scalar-block transform from the sub-parameter's five floats
 * and rotation, or the identity layout when the block is absent. */
void sdfMatSetUvwParm(SdfDrawTransform *transform, const SdfSubParam *subParam) {
    f32 v0;
    f32 v1;
    f32 v2;
    f32 v3;
    f32 angle;
    f32 s;
    f32 c;
    f32 m0;
    f32 m1;
    f32 m2;
    f32 m3;

    if (subParam == NULL) {
        /* Identity {1,0}/{0,1}/{0,0}; the constant 24-byte block is written
         * through the packed view, the form that emits three 64-bit stores. */
        transform->words[0] = 0x000000003F800000;
        transform->words[1] = 0x3F80000000000000;
        transform->words[2] = 0;
        return;
    }

    angle = -subParam->transformInputs.rotationInput;
    s = sdfSin(angle);
    c = sdfCos(angle);
    v0 = subParam->transformInputs.centerOffsetX;
    v1 = subParam->transformInputs.centerOffsetY;
    v2 = subParam->transformInputs.basisScaleX;
    v3 = subParam->transformInputs.basisScaleY;

    m0 = c * v2;
    m1 = s * v3;
    m2 = -s * v2;
    m3 = c * v3;
    transform->m[0] = m0;
    transform->m[1] = m1;
    transform->m[2] = m2;
    transform->m[3] = m3;
    transform->m[4] = 0.5f - m0 * (v0 + 0.5f) - m2 * (0.5f - v1);
    transform->m[5] = 0.5f - m1 * (v0 + 0.5f) - m3 * (0.5f - v1);
}

void sdfMatUpdatePacketUvwParm(sdfMat_t *asset, SdfAssetEntry *entry) {
    sdfMatSetUvwParm(&entry->transforms[0], asset->primarySubParam);
}

extern u16 D_0040B348[];
/* Copy color, unchecked mode/palette state and optional GS texture words,
 * then apply the secondary scalar block to the entry's native location. */
void sdfMatUpdatePacketMultiTex(sdfMat_t *asset, SdfAssetEntry *drawEntry) {
    sdfTexHandle_t *texture = asset->secondaryTexture;
    u32 packetMode;

    drawEntry->secondaryColor = asset->secondaryColor;
    packetMode = asset->secondaryMode;
    drawEntry->secondaryMode = packetMode;
    drawEntry->alphaState = D_0040B348[packetMode];
    if (texture != NULL) {
        drawEntry->secondaryTextureState.sampling = sdfTexGetTex1Reg(texture);
        drawEntry->secondaryTextureState.texture = sdfTexGetTex0Reg(texture);
        drawEntry->secondaryTextureState.clamp = sdfTexGetClampReg(texture);
    }
    sdfMatSetUvwParm(&drawEntry->transforms[1], asset->secondarySubParam);
}

/* Copy the source pair into the destination entry's two scalar slots. */
void sdfMatUpdatePacketFaceDiv(sdfMat_t *source, SdfAssetEntry *destination) {
    destination->y = source->scalarPairFirst;
    destination->x = source->scalarPairSecond;
}

/* Apply the selected entry's groups and retain the other entry's captured bits.
 * entryIndex is unchecked and expected to be zero or one. */
void sdfMatUpdatePacket(sdfMat_t *asset, s32 entryIndex) {
    u8 dirtyFlags = asset->dirtyFlags;
    SdfAssetEntry *drawEntry = asset->drawEntries[entryIndex];
    if ((dirtyFlags >> entryIndex) & SDF_ASSET_PRIMARY_STATE_BIT) {
        sdfAssetCopyTextureState(asset, drawEntry);
    }
    if (dirtyFlags & (SDF_ASSET_PRIMARY_SCALARS_BIT << entryIndex)) {
        sdfMatUpdatePacketUvwParm(asset, drawEntry);
    }
    if (dirtyFlags & (SDF_ASSET_SECONDARY_STATE_BIT << entryIndex)) {
        sdfMatUpdatePacketMultiTex(asset, drawEntry);
    }
    if (dirtyFlags & (SDF_ASSET_PAIR_STATE_BIT << entryIndex)) {
        sdfMatUpdatePacketFaceDiv(asset, drawEntry);
    }
    asset->dirtyFlags = dirtyFlags & (SDF_ASSET_ENTRY_CHANGE_BITS << (entryIndex ^ 1));
}

/* Nonzero forced mode refreshes primary state even when its change bit is clear.
 * Retain the other entry's captured bits; entryIndex remains unchecked. */
void sdfApplyAssetEntryChangesWithForcedTexture(sdfMat_t *asset, s32 entryIndex) {
    u8 dirtyFlags = asset->dirtyFlags;
    SdfAssetEntry *drawEntry = asset->drawEntries[entryIndex];
    if ((dirtyFlags >> entryIndex) & SDF_ASSET_PRIMARY_STATE_BIT) {
        sdfAssetCopyTextureState(asset, drawEntry);
    } else if (sdfForcedAssetTextureMode != 0) {
        sdfAssetCopyTextureState(asset, drawEntry);
    }
    if (dirtyFlags & (SDF_ASSET_PRIMARY_SCALARS_BIT << entryIndex)) {
        sdfMatUpdatePacketUvwParm(asset, drawEntry);
    }
    if (dirtyFlags & (SDF_ASSET_SECONDARY_STATE_BIT << entryIndex)) {
        sdfMatUpdatePacketMultiTex(asset, drawEntry);
    }
    if (dirtyFlags & (SDF_ASSET_PAIR_STATE_BIT << entryIndex)) {
        sdfMatUpdatePacketFaceDiv(asset, drawEntry);
    }
    asset->dirtyFlags = dirtyFlags & (SDF_ASSET_ENTRY_CHANGE_BITS << (entryIndex ^ 1));
}

#include "sdf_asset_create.h"
u8 *sdfMatSet(sdfMat_t *, DevRequest *, u8 *);
/* Parse the declared number of variable-sized assets, reserving at least the minimum capacity. */
DevRequest *sdfMatListMake(DevRequest *lookup, SdfMaterialAssetList *serializedAssets) {
    u32 remainingAssets = serializedAssets->assetCount;
    u8 *parameterCursor = serializedAssets->records;
    DevRequest *list = sdfMatListNew(remainingAssets >= SDF_ASSET_LIST_MIN_CAPACITY ? remainingAssets : SDF_ASSET_LIST_MIN_CAPACITY);
    while (remainingAssets != 0) {
        sdfMat_t *asset = sdfMatNew();
        parameterCursor = sdfMatSet(asset, lookup, parameterCursor);
        sdfMatListAdd(list, asset);
        remainingAssets--;
    }
    return list;
}

/* Apply one entry index to every asset in the list. */
void sdfResourceListApplyEntryChanges(DevRequest *list, s32 entryIndex) {
    s32 itemIndex;
    s32 itemCount = list->usedCount;
    u32 *assetItems = list->buffer;

    for (itemIndex = 0; itemIndex < itemCount; itemIndex++) {
        sdfMatUpdatePacket((sdfMat_t *)assetItems[itemIndex], entryIndex);
    }
}

/* Apply the selected entry to each asset using the forced-texture variant. */
void sdfMatListUpdatePacket(DevRequest *list, s32 entryIndex) {
    s32 itemIndex;
    s32 itemCount = list->usedCount;
    u32 *assetItems = list->buffer;

    for (itemIndex = 0; itemIndex < itemCount; itemIndex++) {
        sdfApplyAssetEntryChangesWithForcedTexture((sdfMat_t *)assetItems[itemIndex], entryIndex);
    }
}


/* Copy base state; only present source subparameters overwrite destination
 * blocks. Each whole scalar aggregate includes its final opaque word. */
void sdfMatCopyStat(sdfMat_t *destination, sdfMat_t *source) {
    SdfSubParam *subParameters;

    destination->dirtyFlags = SDF_ASSET_ALL_STATE_DIRTY;
    destination->primaryStateWordFirst = source->primaryStateWordFirst;
    destination->primaryStateWordSecond = source->primaryStateWordSecond;
    destination->primaryStateFloat = source->primaryStateFloat;
    destination->secondaryColor = source->secondaryColor;
    destination->unk04 = source->unk04;
    destination->texture = source->texture;
    destination->primaryStateWordThird = source->primaryStateWordThird;
    destination->primaryStateWordFourth = source->primaryStateWordFourth;
    subParameters = source->primarySubParam;
    if (subParameters != NULL) {
        sdfMatGetTexUvwParm(destination)->scalar = subParameters->scalar;
    }
    subParameters = source->secondarySubParam;
    if (subParameters != NULL) {
        sdfMatGetMultiTexUvwParm(destination)->scalar = subParameters->scalar;
    }
    destination->scalarPairFirst = source->scalarPairFirst;
    destination->scalarPairSecond = source->scalarPairSecond;
}

void sdfMatCopyStat(sdfMat_t *, sdfMat_t *);
/* Copy paired items using the destination count; source must contain at least that many items. */
void sdfMatListCopyStat(DevRequest *destination, DevRequest *source) {
    s32 itemCount = destination->usedCount;
    u32 *sourceItems = source->buffer;
    u32 *destinationItems = destination->buffer;
    s32 itemIndex;

    for (itemIndex = 0; itemIndex < itemCount; itemIndex++) {
        sdfMatCopyStat((sdfMat_t *)destinationItems[itemIndex], (sdfMat_t *)sourceItems[itemIndex]);
    }
}

extern s32 (*D_0040B358[])(u32, u32);
/* The high halfword selects the handler, which receives the complete command word unchanged. */
struct SdfMotionBindingHead *sdfMotionNodeNew(sdfMotionCtrlr_t *motion, u32 commandWord, s32 targetIndex) {
    return (struct SdfMotionBindingHead *)D_0040B358[commandWord >> 16]((u32)motion, commandWord);
}

INCLUDE_SDATA(const s32, "game/code_003325F8", sdfMatAllocCount);

INCLUDE_SDATA(const s32, "game/code_003325F8", sdfForcedAssetTextureMode);
