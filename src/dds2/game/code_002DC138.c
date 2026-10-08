#include "common.h"
#include "eff_anim.h"
#include "file.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "mdl.h"
#include "sdf.h"

typedef struct EffModelOwner {
    f32 scale;
    MdlCtx *model;
    SdfLightingPacketStorage *ownedBuffer;
    u32 flags;
} EffModelOwner;

extern void sdfMotionSampleAtFrame(Motion *, f32);

extern u32 sdfResourceRetainAddress(SdfMemBlock *);
extern void sdfReleaseResourceAllocation(SdfMemBlock *);

extern u32 effModelUpdateControlFlags;

extern void *fileResolvePrimaryBuffer(FileJobPayload *);

extern void *fileResolveSecondaryBuffer(FileJobPayload *);

extern void *sdfAllocAndClearQuadwords(s32);

extern void sdfReleaseChipBlock();

extern void sdfTexReleaseReference();

extern void effReleaseSharedReference();

extern u8 btlIsRuntimeAllocated(void);

extern s32 btlIsCurrentActorFullyMarked(void);

extern MdlCtx *func_002DC1D0(void *, u32);

extern BattleGroupNode *btlFindGroupedEntity(s32, s32);

extern void mdlLoadViewerPackage(s32, u16, s32, void *, u32);

extern u16 D_00437E2C;

extern void *effCreateModelOwner(void *);

extern void effRecreateModelFromSource(EffModelOwner *, EffModelOwner *);

extern void func_0035B6E0(const char *fmt, ...);

extern u8 D_00380828[];

extern void mdlProcessContextNodesAndTransforms(MdlCtx *, s32);


typedef struct EffectObjectNode {
    EffModelOwner *object;
    struct EffectObjectNode *prev;
    struct EffectObjectNode *next;
} EffectObjectNode;

extern EffectObjectNode *effFloorModelListHead;

extern u32 D_00437E38;

extern u32 D_00437E3C;

extern u32 func_002DDF48(u32);

extern u32 effCloneSharedReferenceWithValue(u32, u32);

extern s32 btlGetRuntime(void);

extern void mdlStoreTertiaryVectorVU(MdlCtx *);
extern void mdlStorePrimaryVectorVU(MdlCtx *);
extern void mdlUpdateContextRotationBasisFromQuaternion(MdlCtx *);
extern void mdlBroadcastMasked(MdlCtx *, u32);
extern void mdlAddEntryPlain(MdlCtx *, s32, s32);
extern void mdlAddEntryFlagged(MdlCtx *, s32, s32);
extern void mdlDestroyContext(MdlCtx *);
extern s32 effComputeLightDirectionVU(MdlCtx *, SdfLightingPacketStorage *);

extern void effFloorModelListRemove(EffectObjectNode *);

/* Reference-counted texture object at the end of its combined allocation. */
typedef struct RefObj {
    u8 *base;                 // 0x00: retained base of the combined allocation
    u8 *pixels;               // 0x04: image data after the palette
    u8 *palette;              // 0x08: palette data after the copied header
    s32 paletteWidth;         // 0x0C
    s32 paletteHeight;        // 0x10
    s32 refCount;             // 0x14
    s32 index;                // 0x18
    u32 allocationHandle;     // 0x1C: handle released with the final reference
} RefObj; // 0x20

typedef struct SdfTextureFileHeader {
    u8 unk00;
    u8 flags;
    u8 pad02[0xE];
    u8 unk10;
    u8 unk11;
    s16 width;
    s16 height;
    u8 pixelFormat;
    u8 clutFormat;
    u16 lodParameters;
    u8 unk1A;
    u8 clampMode;
    s32 resourceKey;
    s32 unk20;
    u8 pad24[0x1C];
} SdfTextureFileHeader;

typedef char SdfTextureFileHeader_size_must_be_0x40[
    (sizeof(SdfTextureFileHeader) == 0x40) ? 1 : -1];
typedef char RefObj_size_must_be_0x20[(sizeof(RefObj) == 0x20) ? 1 : -1];

extern RefObj *func_002DDAA8(SdfTextureFileHeader *);

extern u32 effSharedTextureReferenceCount;
extern SdfTex *D_00437E40;

extern MdlCtx *func_00232198(s32 group, s32 id);

extern u16 mdlGetContextResourceGroup(MdlCtx *);

extern u16 mdlGetContextResourceId(MdlCtx *);

/* VU0 model helpers consume vf10 directly, as in the DDS1 counterpart. */
extern SdfMemBlock *sdfAllocGeneralBlock(s32);


/* Initialize the VU transforms and the first node's float slot, if present. */
void effInitModelVUState(MdlCtx *model) {
    VU0_MOVE_VF(vf10, vf0);
    mdlStorePrimaryVectorVU(model);
    VU0_MOVE_VF(vf10, vf0);
    mdlUpdateContextRotationBasisFromQuaternion(model);
    VU0_SET_ONES_XYZ(vf10);
    mdlStoreTertiaryVectorVU(model);
    mdlBroadcastMasked(model, 0x80808080);
    if (model->first != NULL) {
        mdlAddEntryPlain(model, 0, 0);
        model->first->frameStep = 1.0f;
    }
    model->flags &= ~MDL_SKIP_TRANSFORMS;
}

MdlCtx *func_002DC1D0(void *kind, u32 flags) {
    MdlCtx *result;
    while (btlFindGroupedEntity(6, D_00437E2C) != 0) {
        D_00437E2C++;
    }
    mdlLoadViewerPackage(6, D_00437E2C, 0x101, kind, flags);
    result = func_00232198(6, D_00437E2C);
    effInitModelVUState(result);
    D_00437E2C++;
    return result;
}

/* Clear the lighting attachment before destroying its model context. */
void effDestroyModelContext(MdlCtx *owner) {
    owner->inner->lighting = NULL;
    mdlDestroyContext(owner);
}

MdlCtx *effCloneModelWithVUState(MdlCtx *sourceModel) {
    s32 group;
    s32 id;
    MdlCtx *model;

    group = mdlGetContextResourceGroup(sourceModel);
    id = mdlGetContextResourceId(sourceModel);
    model = func_00232198(group, id);
    effInitModelVUState(model);
    return model;
}


void *effCreateModelOwner(void *input) {
    FileJobPayload *source = input;
    EffModelOwner *owner = sdfAllocAndClearQuadwords(0x10);
    owner->ownedBuffer = sdfAllocAndClearQuadwords(sizeof(*owner->ownedBuffer));
    if (source != NULL) {
        void *data;
        *(u32 *)owner = *(u32 *)fileResolvePrimaryBuffer(source);
        data = fileResolveSecondaryBuffer(source);
        if (data != 0) {
            owner->model = func_002DC1D0(data, source->secondary.size);
            VU0_SET_ONES_XYZ(vf10);
            VU0_SCALE_VF_MFC1(vf10, owner->scale);
            mdlStoreTertiaryVectorVU(owner->model);
        }
    }
    return owner;
}

void effDestroyModelOwner(EffModelOwner *owner) {
    SdfLightingPacketStorage *buffer = owner->ownedBuffer;
    if (buffer != NULL) {
        sdfReleaseChipBlock(buffer);
    }
    if (owner->model != 0) {
        effDestroyModelContext(owner->model);
    }
    sdfReleaseChipBlock(owner);
}

EffModelOwner *effDuplicateEffectHeader(EffModelOwner *source) {
    EffModelOwner *owner = effCreateModelOwner(0);
    *(u32 *)owner = *(u32 *)source;
    effRecreateModelFromSource(owner, source);
    return owner;
}

void effRecreateModelFromSource(EffModelOwner *owner, EffModelOwner *source) {
    s32 group;
    s32 id;
    MdlCtx *model;

    if (owner->model != 0) {
        effDestroyModelContext(owner->model);
    }
    group = mdlGetContextResourceGroup(source->model);
    id = mdlGetContextResourceId(source->model);
    model = func_00232198(group, id);
    effInitModelVUState(model);
    VU0_SET_ONES_XYZ(vf10);
    VU0_SCALAR_OP_CLOBBER(owner->scale, "vmulx.xyzw vf10, vf10, vf2x");
    mdlStoreTertiaryVectorVU(model);
    owner->model = model;
}

void effModelAnimationStop(EffModelOwner *owner) {
    sdfMotionSampleAtFrame(owner->model->first, 0.0f);
}

/* Attach the owner's lighting buffer when the direction update succeeds. */
void effRefreshModelLighting(EffModelOwner *work) {
    MdlCtx *model;
    if (effComputeLightDirectionVU(work->model, work->ownedBuffer)) {
        model = work->model;
        model->inner->lighting = work->ownedBuffer;
    } else {
        model = work->model;
    }
    mdlProcessContextNodesAndTransforms(model, (s32)D_00380828);
}

void effApplyModelPrimaryVector(EffModelOwner *owner, u8 *vec) {
VU0_LOAD_VF_MEMORY(vf10, vec);
    mdlStorePrimaryVectorVU(owner->model);
}

void effApplyModelVecB(EffModelOwner *owner, u8 *vec) {
VU0_LOAD_VF_MEMORY(vf10, vec);
    mdlUpdateContextRotationBasisFromQuaternion(owner->model);
}

void effBroadcastModelMask(EffModelOwner *owner, u32 color) {
    mdlBroadcastMasked(owner->model, color);
}

void effApplyScaledModelTertiaryVector(EffModelOwner *owner, float scale) {
    float v[3];
    float t;

    t = owner->scale * scale;
    v[0] = v[1] = v[2] = t;
    VU0_LOAD_VF(vf10, v);
    mdlStoreTertiaryVectorVU(owner->model);
}


typedef struct EffTexTable {
    u8 pad_00[0xC];
    SdfTex **entries; // 0x0C
} EffTexTable;

/* Battle state: resource headers for texture slots 1 and 2. */
typedef struct EffBattleTexHeaders {
    u8 pad_000[0x5B0];
    u8 *slot1; // 0x5B0
    u8 *slot2; // 0x5B4
} EffBattleTexHeaders;

extern s32 func_0032B240();

extern u8 *sdfTexSubmitPixelsForFormat(void *, u32, u8 *, s32);

extern s32 sdfTexSubmitImageCopy(u32, s16, s16, u8, u8 *, s32);

extern u32 sdfTexGetPrimaryResourceWord(void *);

extern u32 sdfTexGetSecondaryResourceWord(void *);

void effUploadModelTextures(EffModelOwner *owner) {
    s32 i = 0;
    EffBattleTexHeaders *battle = (EffBattleTexHeaders *)btlGetRuntime();
    EffTexTable *table = owner->model->inner->assetData;

    do {
        SdfTex *tex = table->entries[i++];
        u8 *header;
        u8 *pixels;
        s16 width;
        s16 height;
        u8 format;

        switch (tex->unk24) {
        case 1:
            header = battle->slot1;
            break;
        case 2:
            header = battle->slot2;
            break;
        default:
            header = NULL;
            break;
        }
        pixels = header + (header[1] & 0xF0) + 0x40;
        if (func_0032B240(tex) != 0) {
            pixels = sdfTexSubmitPixelsForFormat(tex, sdfTexGetSecondaryResourceWord(tex), pixels, 1);
        }
        width = tex->width;
        height = tex->height;
        format = tex->pixelFormat;
        sdfTexSubmitImageCopy(sdfTexGetPrimaryResourceWord(tex), width, height, format, pixels, 1);
    } while (i < 2);
}

extern void effFloorModelListPush(EffModelOwner *);

/* Track floor models only while battle is active and the current actor is not fully marked. */
void *effCreateFloorModelOwner(void *source) {
    EffModelOwner *owner;
    s32 battleActive;

    owner = effCreateModelOwner(source);
    effUploadModelTextures(owner);
    battleActive = btlIsRuntimeAllocated();
    if ((battleActive != 0) && (battleActive = btlIsCurrentActorFullyMarked(), battleActive == 0)) {
        effFloorModelListPush(owner);
    }
    return owner;
}

void effMarkFloorModelForDestruction(u32 *p) {
    ((EffModelOwner *)p)->flags |= 2;
    if (!(((EffModelOwner *)p)->flags & 4)) {
        effDestroyModelOwner((EffModelOwner *)p);
    }
}

EffModelOwner *effDuplicateFloorModelOwner(EffModelOwner *source) {
    EffModelOwner *owner = effCreateModelOwner(0);
    s32 marked;

    *(u32 *)owner = *(u32 *)source;
    effRecreateModelFromSource(owner, source);
    effUploadModelTextures(owner);
    marked = btlIsRuntimeAllocated();
    if ((marked != 0) && (marked = btlIsCurrentActorFullyMarked(), marked == 0)) {
        effFloorModelListPush(owner);
    }
    return owner;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DC808);

void effMarkFloorModelForUpdate(u32 *p) {
    ((EffModelOwner *)p)->flags |= 1;
    if (!(((EffModelOwner *)p)->flags & 4)) {
        if (!(effModelUpdateControlFlags & 1)) {
            func_002DC808(p);
        }
    } else if (effModelUpdateControlFlags & 1) {
        ((EffModelOwner *)p)->flags |= 0x30;
    }
}

void effFloorModelListPush(EffModelOwner *obj) {
    EffectObjectNode *entry = sdfAllocAndClearQuadwords(sizeof(EffectObjectNode));

    entry->object = obj;
    entry->prev = NULL;
    if (effFloorModelListHead != NULL) {
        effFloorModelListHead->prev = entry;
        entry->next = effFloorModelListHead;
    } else {
        entry->next = NULL;
    }
    effFloorModelListHead = entry;
    entry->object->flags |= 4;
}

void effFloorModelListRemove(EffectObjectNode *node) {
    if (node->object != NULL) {
        func_0035B6E0("eff:floor model delete[%p]\n", node->object);
        effDestroyModelOwner(node->object);
        node->object = NULL;
    }
    if (node->next != NULL) {
        node->next->prev = node->prev;
    }
    if (node->prev != NULL) {
        node->prev->next = node->next;
    } else {
        effFloorModelListHead = node->next;
    }
    sdfReleaseChipBlock(node);
}

void effSweepFloorModelList(void) {
    EffectObjectNode *node = effFloorModelListHead;
    EffectObjectNode *next;

    if (node != NULL) {
        do {
            next = node->next;
            if (!(node->object->flags & 8)) {
                if ((node->object->flags & 0x31) == 1) {
                    func_002DC808(node->object);
                }
                node->object->flags &= ~0x20;
            } else {
                effFloorModelListRemove(node);
            }
            node = next;
        } while (node != NULL);
    }
}

void mdlPropagateObjectFlag(void) {
    EffectObjectNode *node = effFloorModelListHead;
    EffModelOwner *object;
    s32 flags;

    if (node != NULL) {
        do {
            object = node->object;
            flags = object->flags;
            if ((flags & 2) != 0) {
                object->flags = flags | 8;
            }
            node = node->next;
        } while (node != NULL);
    }
}

void mdlClearListedObjectFlag(void) {
    EffModelOwner *object;
    EffectObjectNode *node;

    node = effFloorModelListHead;
    while (node != NULL) {
        object = node->object;
        node = node->next;
        object->flags = object->flags & 0xffffffef;
    }
}

void mdlSetListedObjectFlag(void) {
    EffModelOwner *object;
    EffectObjectNode *node;

    node = effFloorModelListHead;
    while (node != NULL) {
        object = node->object;
        node = node->next;
        object->flags = object->flags | 0x10;
    }
}

void mdlMarkAndProcessObjectNodes(void) {
    EffectObjectNode *node = effFloorModelListHead;
    EffModelOwner *object;
    EffectObjectNode *next;
    s32 flags;

    if (node == NULL) {
        return;
    }
    do {
        object = node->object;
        next = node->next;
        flags = object->flags | 0xA;
        object->flags = flags;
        effFloorModelListRemove(node);
        node = next;
    } while (node != NULL);
}

typedef struct FileQueue FileQueue;

typedef struct EffResourceConfig {
    u8 unknown00[0x34];
    s32 plainEntry; /* Duration, also selects plain versus flagged entry. */
    f32 frameStep;
} EffResourceConfig;

typedef struct EffResourceExtendedConfig {
    EffResourceConfig base;
    u8 unknown3C[0x38];
} EffResourceExtendedConfig;

typedef struct EffResourceOwner {
    u32 count;
    s32 unk04;
    EffResourceConfig base;
    EffResourceExtendedConfig extended;
    FileQueue **entries;
    SdfMemBlock *buffer;
    MdlCtx *model;
} EffResourceOwner;

typedef char EffResourceConfig_size_check[sizeof(EffResourceConfig) == 0x3C ? 1 : -1];
typedef char EffResourceExtendedConfig_size_check[sizeof(EffResourceExtendedConfig) == 0x74 ? 1 : -1];
typedef char EffResourceOwner_size_check[sizeof(EffResourceOwner) == 0xC4 ? 1 : -1];
typedef char EffResourceOwner_base_check[(u32)&((EffResourceOwner *)0)->base == 8 ? 1 : -1];
typedef char EffResourceOwner_extended_check[(u32)&((EffResourceOwner *)0)->extended == 0x44 ? 1 : -1];
typedef char EffResourceOwner_entries_check[(u32)&((EffResourceOwner *)0)->entries == 0xB8 ? 1 : -1];
typedef char EffResourceOwner_buffer_check[(u32)&((EffResourceOwner *)0)->buffer == 0xBC ? 1 : -1];
typedef char EffResourceOwner_model_check[(u32)&((EffResourceOwner *)0)->model == 0xC0 ? 1 : -1];

extern u32 sdfCountMapPositionRecords(SdfModel *model);
extern FileQueue *fileQueueClone(FileQueue *);
extern FileQueue *fileCloneQueueEntries(FileQueue *);
extern void *func_002DCCE8(void *);
extern void *sdfAllocSizeClassBlock(s32);
extern void *memset(void *, s32, u32);
extern void *memcpy(void *, const void *, u32);

/* Decode the type-6 configuration, load its model and clone per-position queues. */
void *func_002DCCE8(void *input) {
    FileJobPayload *job = input;
    EffResourceOwner *owner;
    void *resource;
    u32 modelOffset;
    u32 i;

    owner = sdfAllocSizeClassBlock(sizeof(*owner));
    memset(owner, 0, sizeof(*owner));
    if (job != NULL) {
        resource = fileResolvePrimaryBuffer(job);
        switch (job->option) {
        case 0:
            modelOffset = 0x40;
            memcpy(&owner->base, resource, sizeof(owner->base));
            break;
        case 1:
            modelOffset = 0x80;
            memcpy(&owner->extended, resource, sizeof(owner->extended));
            owner->base = owner->extended.base;
            break;
        default:
            modelOffset = 0;
            break;
        }
        owner->model = func_002DC1D0((u8 *)resource + modelOffset, job->primary.size - modelOffset);
        if (owner->model->first != NULL) {
            if (owner->base.plainEntry != 0) {
                mdlAddEntryPlain(owner->model, 0, 0);
            } else {
                mdlAddEntryFlagged(owner->model, 0, 0);
            }
        }
        owner->count = sdfCountMapPositionRecords(owner->model->inner);
        if (owner->count == 0) {
            return owner;
        }
        resource = fileResolveSecondaryBuffer(job);
        if (resource != NULL) {
            owner->buffer = sdfAllocGeneralBlock(owner->count * sizeof(*owner->entries));
            owner->entries = (FileQueue **)sdfResourceRetainAddress(owner->buffer);
            owner->entries[0] = fileCloneQueueEntries(resource);
            for (i = 1; i < owner->count; i++) {
                owner->entries[i] = fileQueueClone(owner->entries[0]);
            }
        }
    }
    return owner;
}

extern void fileQueueDestroy(FileQueue *);

void effDestroyResourceOwner(void *work) {
    EffResourceOwner *owner = work;
    u32 i;

    if (owner->model != 0) {
        effDestroyModelContext(owner->model);
    }
    if (owner->buffer != 0) {
        for (i = 0; i < owner->count; i++) {
            fileQueueDestroy(owner->entries[i]);
        }
        sdfReleaseResourceAllocation(owner->buffer);
    }
    sdfReleaseChipBlock(owner);
}


extern void effCopyResourceOwner(void *, void *);

void *effDuplicateModelOwner(void *source, u16 unusedType) {
    EffResourceOwner *src = source;
    EffResourceOwner *dst = func_002DCCE8(0);

    dst->base = src->base;
    dst->extended = src->extended;
    effCopyResourceOwner(dst, src);
    return dst;
}



void effCopyResourceOwner(void *destination, void *source) {
    EffResourceOwner *dst = destination;
    EffResourceOwner *src = source;
    u32 i;

    if (dst->model != 0) {
        effDestroyModelContext(dst->model);
    }
    dst->model = func_00232198(mdlGetContextResourceGroup(src->model), mdlGetContextResourceId(src->model));
    effInitModelVUState(dst->model);
    if (dst->model->first != NULL) {
        if (dst->base.plainEntry != 0) {
            mdlAddEntryPlain(dst->model, 0, 0);
        } else {
            mdlAddEntryFlagged(dst->model, 0, 0);
        }
    }
    dst->count = sdfCountMapPositionRecords(dst->model->inner);
    if (src->buffer != 0) {
        if (dst->buffer != 0) {
            for (i = 0; i < dst->count; i++) {
                fileQueueDestroy(dst->entries[i]);
            }
            sdfReleaseResourceAllocation(dst->buffer);
        }
        dst->buffer = sdfAllocGeneralBlock(dst->count * 4);
        dst->entries = (FileQueue **)sdfResourceRetainAddress(dst->buffer);
        for (i = 0; i < dst->count; i++) {
            dst->entries[i] = fileQueueClone(*src->entries);
        }
    }
}

extern void fileQueueNotifyAllJobsComplete(u8 *);

extern void sdfMotionSampleAtFrame(Motion *, f32);

void func_002DD3C0(void *work) {
    EffResourceOwner *owner = work;
    if (owner->buffer != NULL) {
        u32 count = owner->count;
        u32 i = 0;
        FileQueue **entries = owner->entries;
        if (count != 0) {
            do {
                fileQueueNotifyAllJobsComplete((u8 *)*entries);
                entries++;
                i++;
            } while (i < count);
        }
    }
    sdfMotionSampleAtFrame(owner->model->first, 0.0f);
    owner->unk04 = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DD448);

void effLoadModelPrimaryVector(void *obj, void *vec) {
    EffResourceOwner *owner = obj;
VU0_LOAD_VF_MEMORY(vf10, vec);
    mdlStorePrimaryVectorVU(owner->model);
}

void effSetModelRotationQuaternion(void *obj, void *vec) {
    EffResourceOwner *owner = obj;
VU0_LOAD_VF_MEMORY(vf10, vec);
    mdlUpdateContextRotationBasisFromQuaternion(owner->model);
}

void effPropagateResourceModelMask(void *work, u32 color) {
    EffResourceOwner *model = work;
    mdlBroadcastMasked(model->model, color);
}

void effScaleModelVec(void *work, float scale) {
    EffResourceOwner *owner = work;
    u32 bits;
    VU0_SET_ONES_XYZ(vf10);
    VU0_SCALAR_OP_TMP_MEMORY(bits, scale, "vmulx.xyzw vf10, vf10, vf2x");
    mdlStoreTertiaryVectorVU(owner->model);
}

void effObjectListCountersReset(void) {
    D_00437E3C = 0;
    D_00437E38 = 0xffffffff;
}

extern s32 sdfFormatImageSize(u32 format, s32 width, s32 height);
extern SdfTex *func_0032B968(s32, s32, u32, u32, u32, u32);
extern void sdfTexCreateFirstPacket(SdfTex *texture);

RefObj *func_002DDAA8(SdfTextureFileHeader *source) {
    u32 paletteWidth;
    u32 paletteHeight;
    s32 paletteBytes;
    s32 imageBytes;
    s32 payloadBytes;
    s32 textureOffset;
    u32 allocationHandle;
    u8 *cursor;
    RefObj *texture;

    if ((source->pixelFormat == 0x13) || (source->pixelFormat == 0x1B)) {
        paletteWidth = 0x10;
        paletteHeight = 0x10;
    } else {
        paletteWidth = 8;
        paletteHeight = 2;
    }

    paletteBytes = sdfFormatImageSize(source->unk11, paletteWidth, paletteHeight) << 4;
    imageBytes = sdfFormatImageSize(source->pixelFormat, source->width, source->height) << 4;
    payloadBytes = imageBytes + paletteBytes;
    textureOffset = payloadBytes + 0x40;
    allocationHandle = (u32)sdfAllocGeneralBlock(payloadBytes + 0x60);
    cursor = (u8 *)sdfResourceRetainAddress((SdfMemBlock *)allocationHandle);
    texture = (RefObj *)(cursor + textureOffset);
    texture->base = cursor;
    cursor += 0x40;
    texture->palette = cursor;
    cursor += paletteBytes;
    texture->pixels = cursor;
    texture->paletteWidth = paletteWidth;
    texture->paletteHeight = paletteHeight;
    texture->refCount = 0;
    texture->index = -1;
    texture->allocationHandle = allocationHandle;

    memcpy(texture->base, source, 0x40);
    cursor = (u8 *)source + 0x40 + (source->flags & 0xF0);
    memcpy(texture->palette, cursor, paletteBytes);
    cursor += paletteBytes;
    memcpy(texture->pixels, cursor, imageBytes);

    if (effSharedTextureReferenceCount == 0) {
        D_00437E40 = func_0032B968(0x100, 0x100, 0x13, 0, 0, 1);
        sdfTexCreateFirstPacket(D_00437E40);
    }
    effSharedTextureReferenceCount++;
    texture->refCount++;
    return texture;
}

u32 effCloneSharedReferenceWithValue(u32 source, u32 value) {
    RefObj *copy;

    copy = (RefObj *)func_002DDAA8((void *)source);
    copy->index = value;
    return (u32)copy;
}

void effReleaseSharedReference(RefObj *obj) {
    if (--effSharedTextureReferenceCount == 0) {
        SdfTex *texture = D_00437E40;

        texture->width = 0x100;
        texture->height = 0x100;
        D_00437E38 = 0xffffffff;
        sdfTexReleaseReference(texture);
    }
    if (--obj->refCount == 0) {
        sdfReleaseResourceAllocation((SdfMemBlock *)obj->allocationHandle);
    }
}

RefObj *effRetainSharedReference(RefObj *obj) {
    obj->refCount++;
    effSharedTextureReferenceCount++;
    return obj;
}

extern SdfTex *func_002DDD60(void *, RefObj *);
INCLUDE_ASM(const s32, "game/code_002DC138", func_002DDD60);

/* Each 0x10-byte entry contributes itself plus the number stored in its first word. */
typedef struct EffExpandedList {
    u8 pad0[4];
    u32 count;
    u8 pad8[8];
    u8 *entries;
    RefObj **handles;
    s32 total;
    u32 refCount;
    s32 unk20;
    u32 buffer;
} EffExpandedList;

s32 effCountExpandedEntries(void *work) {
    EffExpandedList *list = work;
    u32 count = list->count;
    u32 i = 0;
    s32 total = 0;

    if (count != 0) {
        u8 *entries = list->entries;
        do {
            s32 additionalCount = *(s32 *)entries;
            entries += 0x10;
            i++;
            total++;
            total += additionalCount;
        } while (i < count);
    }
    return total;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DDF48);

void effReleaseReferenceHolder(u32 *holder) {
    u32 i;

    if (--((EffExpandedList *)holder)->refCount == 0) {
        for (i = 0; i < ((EffExpandedList *)holder)->count; i++) {
            effReleaseSharedReference(((EffExpandedList *)holder)->handles[i]);
        }
        sdfReleaseResourceAllocation((SdfMemBlock *)((EffExpandedList *)holder)->buffer);
    }
}

EffExpandedList *effReferenceObjectRetain(EffExpandedList *obj) {
    obj->refCount++;
    return obj;
}

/* One animation track: segments of `length` frames each (plus one), looping if flags & 1. */
void effSampleAnimSet(EffAnimSet *set, u32 frame, EffAnimSample *out) {
    u32 count = set->count;
    u32 local = 0;
    s32 segment = -1;
    EffAnimSegment *seg;
    u32 acc;
    u32 i;

    if (count == 1) {
        segment = 0;
    } else {
        if (set->flags & 1) {
            local = frame % set->length;
        } else if (frame >= set->length) {
            segment = count - 1;
        } else {
            local = frame;
        }
        if (segment == -1) {
            seg = set->segments;
            acc = 0;
            for (i = 0; i < count; i++) {
                acc += seg->length;
                if (acc >= local) {
                    segment = i;
                    break;
                }
                acc++;
                seg++;
            }
        }
    }
    out->scaleX = 1.0f;
    {
        f32 scaleY = 2.0f;

        if (!(set->flags & 4)) {
            scaleY = 1.0f;
        }
        out->segment = segment;
        out->scaleY = scaleY;
    }
    out->angle = 0.0f;
}

SdfTex *effAssignSampledSegmentReference(EffAnimSet *set, void *target, const EffAnimSample *sample) {
    return func_002DDD60(target, set->handles[sample->segment]);
}

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E2C);

INCLUDE_SDATA(const s32, "game/code_002DC138", effFloorModelListHead);

INCLUDE_SDATA(const s32, "game/code_002DC138", effSharedTextureReferenceCount);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E38);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E3C);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E40);

