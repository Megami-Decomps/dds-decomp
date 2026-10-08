#include "common.h"
#include "eff_anim.h"
#include "file.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"
#include "mdl.h"
#include "sdf.h"

extern u32 sdfResourceRetainAddress(u32);

extern void *sdfAllocGeneralBlock(u32);

extern s32 btlGetRuntime(void);

extern u32 effCloneSharedReferenceWithValue(u32, u32);

extern u32 func_0029C230(u32);

extern void *fileResolvePrimaryBuffer();

extern u32 *fileResolveSecondaryBuffer(void *);


typedef struct EffTexTable {
    u8 pad_00[0xC];
    SdfTex **entries; // 0x0C
} EffTexTable;

/* Battle state: resource headers for texture slots 1 and 2. */
typedef struct EffBattleTexHeaders {
    u8 pad_000[0x1F4];
    u32 statusFlags; /* 0x1F4 */
    u8 pad_1F8[0x30];
    struct EffBattleUnit *units; /* 0x228 */
    u8 pad_22C[0x350];
    u8 *slot1; // 0x57C
    u8 *slot2; // 0x580
} EffBattleTexHeaders;

extern s32 func_002D2390();

extern u8 *sdfTexSubmitPixelsForFormat(void *, u32, u8 *, s32);

extern s32 sdfTexSubmitImageCopy(u32, s16, s16, u8, u8 *, s32);

extern u32 sdfTexGetPrimaryResourceWord(void *);

extern u32 sdfTexGetSecondaryResourceWord(void *);

extern u32 D_003BC950;

extern u32 D_003BC954;


typedef struct EffModelOwner {
    f32 scale;
    MdlCtx *model;
    SdfLightingPacketStorage *ownedBuffer;
    u32 flags;
} EffModelOwner;


typedef struct EffectObjectNode {
    EffModelOwner *object;
    struct EffectObjectNode *prev;
    struct EffectObjectNode *next;
} EffectObjectNode;

extern EffectObjectNode *effFloorModelListHead;

extern u8 btlIsRuntimeAllocated(void);

extern s32 btlIsCurrentActorFullyMarked(void);

extern EffModelOwner *effCreateModelOwner();

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

extern RefObj *func_0029BD90(SdfTextureFileHeader *);

extern void sdfMotionSampleAtFrame(Motion *, f32);

extern u32 effSharedTextureReferenceCount;
extern SdfTex *D_003BC958;

extern void effFloorModelListRemove(EffectObjectNode *);

extern void mdlStoreTertiaryVectorVU(MdlCtx *);

extern void mdlStorePrimaryVectorVU(MdlCtx *);

extern void mdlUpdateContextRotationBasisFromQuaternion(MdlCtx *);
extern void mdlBroadcastMasked(MdlCtx *, u32);
extern void mdlAddEntryPlain(MdlCtx *, s32, s32);
extern void mdlAddEntryFlagged(MdlCtx *, s32, s32);
extern void mdlDestroyContext(MdlCtx *);
extern u16 mdlGetContextResourceGroup(MdlCtx *);
extern u16 mdlGetContextResourceId(MdlCtx *);


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
    model->flags &= ~1;
}

extern u16 D_003BC944;

extern void mdlLoadViewerPackage(s32, u16, s32, void *, u32);

extern MdlCtx *func_00217680(s32, s32);

extern void effInitModelVUState(MdlCtx *);

MdlCtx *effLoadViewerModelWithVUState(void *first, u32 second) {
    MdlCtx *model;
    mdlLoadViewerPackage(6, D_003BC944, 0x101, first, second);
    model = func_00217680(6, D_003BC944);
    effInitModelVUState(model);
    D_003BC944++;
    return model;
}

/* Clear the lighting attachment before destroying its model context. */
void effDestroyModelContext(MdlCtx *model) {
    model->inner->lighting = NULL;
    mdlDestroyContext(model);
}

EffModelOwner *effCreateModelOwner(u8 *source) {
    EffModelOwner *owner = sdfAllocAndClearQuadwords(0x10);
    owner->ownedBuffer = sdfAllocAndClearQuadwords(sizeof(*owner->ownedBuffer));
    if (source != NULL) {
        void *data;
        *(u32 *)owner = *(u32 *)fileResolvePrimaryBuffer(source);
        data = fileResolveSecondaryBuffer(source);
        if (data != 0) {
            owner->model = effLoadViewerModelWithVUState(data, ((FileJob *)source)->slots[1].size);
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

u32 *effDuplicateEffectHeader(u32 *source) {
    u32 *effect = (u32 *)effCreateModelOwner(0);
    effect[0] = source[0];
    effRecreateModelFromSource(effect, (u8 *)source);
    return effect;
}

void effRecreateModelFromSource(u32 *work, u8 *source) {
    EffModelOwner *owner = (EffModelOwner *)work;
    EffModelOwner *original = (EffModelOwner *)source;
    MdlCtx *model;

    if (owner->model != 0) {
        effDestroyModelContext(owner->model);
    }
    model = func_00217680(mdlGetContextResourceGroup(original->model), mdlGetContextResourceId(original->model));
    effInitModelVUState(model);
    VU0_SET_ONES_XYZ(vf10);
    VU0_SCALE_VF_MFC1(vf10, owner->scale);
    mdlStoreTertiaryVectorVU(model);
    owner->model = model;
}

void effModelAnimationStop(EffModelOwner *owner) {
    sdfMotionSampleAtFrame(owner->model->first, 0.0f);
}

extern u8 D_00325828[];

extern s32 effComputeLightDirectionVU(MdlCtx *, void *);

extern void mdlProcessContextNodesAndTransforms(MdlCtx *, s32);

/* Attach the owner's lighting buffer when the direction update succeeds. */
void effRefreshModelLighting(EffModelOwner *work) {
    MdlCtx *model;
    if (effComputeLightDirectionVU(work->model, work->ownedBuffer) != 0) {
        model = work->model;
        model->inner->lighting = work->ownedBuffer;
    } else {
        model = work->model;
    }
    mdlProcessContextNodesAndTransforms(model, (s32)D_00325828);
}

/* Pass a vector to the VU0 model helpers via vf10 (gcc cannot do this from plain C). */
void effApplyModelPrimaryVector(EffModelOwner *owner, void *vec) {
    VU0_LOAD_VF(vf10, vec);
    mdlStorePrimaryVectorVU(owner->model);
}

void effApplyModelVecB(EffModelOwner *owner, void *vec) {
    VU0_LOAD_VF(vf10, vec);
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
        if (func_002D2390(tex) != 0) {
            pixels = sdfTexSubmitPixelsForFormat(tex, sdfTexGetSecondaryResourceWord(tex), pixels, 1);
        }
        width = tex->width;
        height = tex->height;
        format = tex->pixelFormat;
        sdfTexSubmitImageCopy(sdfTexGetPrimaryResourceWord(tex), width, height, format, pixels, 1);
    } while (i < 2);
}

/* Track floor models only while battle is active and the current actor is not fully marked. */
EffModelOwner *effCreateFloorModelOwner(u8 *source) {
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

void effMarkFloorModelForDestruction(u8 *work) {
    u32 flags = ((EffModelOwner *)work)->flags | 2;
    ((EffModelOwner *)work)->flags = flags;
    if ((flags & 4) == 0) {
        effDestroyModelOwner(work);
    }
}

extern void effRecreateModelFromSource(u32 *, u8 *);

u32 *effDuplicateFloorModelOwner(u8 *source) {
    u32 *owner = (u32 *)effCreateModelOwner(0);

    owner[0] = *(u32 *)source;
    effRecreateModelFromSource(owner, source);
    effUploadModelTextures(owner);
    if (btlIsRuntimeAllocated() != 0 && btlIsCurrentActorFullyMarked() == 0) {
        effFloorModelListPush(owner);
    }
    return owner;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029AE88);

extern u32 effModelUpdateControlFlags;

void effMarkFloorModelForUpdate(u8 *work) {
    u32 previous = ((EffModelOwner *)work)->flags;
    u32 flags = previous | 1;
    ((EffModelOwner *)work)->flags = flags;
    if ((flags & 4) == 0) {
        if ((effModelUpdateControlFlags & 1) == 0) {
            func_0029AE88(work);
        }
    } else if ((effModelUpdateControlFlags & 1) != 0) {
        ((EffModelOwner *)work)->flags = previous | 0x31;
    }
}

extern void *sdfAllocAndClearQuadwords(u32);


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

extern char D_003B2AA0[];

extern void func_003003F0(const char *, void *);

void effFloorModelListRemove(EffectObjectNode *node) {
    if (node->object != NULL) {
        func_003003F0(D_003B2AA0, node->object);
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
                    func_0029AE88(node->object);
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

/* DDS1 resource owner: same lifetime fields as DDS2, with a shorter payload. */
typedef struct EffResourceOwner {
    u32 count;
    u32 unk04;
    u8 pad_08[0x34];
    u32 plainEntry; // 0x3C: add the model entry plain instead of flagged
    u8 pad_40[4];
    void **entries;
    void *buffer;
    MdlCtx *model;
} EffResourceOwner;

typedef struct {
    u8 bytes[0x38];
    u32 last;
} EffectModelHeaderCopy;

struct FileQueue;
extern void *sdfAllocSizeClassBlock(s32);
extern u32 sdfCountMapPositionRecords(void *);
extern struct FileQueue *fileCloneQueueEntries(struct FileQueue *);
extern struct FileQueue *fileQueueClone(struct FileQueue *);

u8 *func_0029B368(void *source) {
    EffResourceOwner *owner = sdfAllocSizeClassBlock(sizeof(EffResourceOwner));
    u8 *data;
    u32 i;

    memset(owner, 0, sizeof(EffResourceOwner));
    if (source != NULL) {
        data = fileResolvePrimaryBuffer(source);
        memcpy((u8 *)owner + 8, data, sizeof(EffectModelHeaderCopy));
        owner->model = effLoadViewerModelWithVUState(data + 0x40,
                                                   ((FileJob *)source)->slots[0].size - 0x40);
        if (owner->model->first != NULL) {
            if (owner->plainEntry != 0) {
                mdlAddEntryPlain(owner->model, 0, 0);
            } else {
                mdlAddEntryFlagged(owner->model, 0, 0);
            }
        }
        owner->count = sdfCountMapPositionRecords(owner->model->inner);
        if (owner->count == 0) {
            return (u8 *)owner;
        }
        data = (u8 *)fileResolveSecondaryBuffer(source);
        if (data != NULL) {
            owner->buffer = sdfAllocGeneralBlock(owner->count * 4);
            owner->entries = (void **)sdfResourceRetainAddress((u32)owner->buffer);
            owner->entries[0] = fileCloneQueueEntries((struct FileQueue *)data);
            for (i = 1; i < owner->count; i++) {
                owner->entries[i] = fileQueueClone(owner->entries[0]);
            }
        }
    }
    return (u8 *)owner;
}

extern void fileQueueDestroy(u32);


void effDestroyResourceOwner(EffResourceOwner *owner) {
    u32 i;

    if (owner->model != 0) {
        effDestroyModelContext(owner->model);
    }
    if (owner->buffer != 0) {
        for (i = 0; i < owner->count; i++) {
            fileQueueDestroy((u32)owner->entries[i]);
        }
        sdfReleaseResourceAllocation(owner->buffer);
    }
    sdfReleaseChipBlock(owner);
}


extern u8 *func_0029B368(void *);

extern void effCopyResourceOwner(EffResourceOwner *, EffResourceOwner *);

u8 *effDuplicateModelOwner(u8 *source) {
    u8 *effect = func_0029B368(0);

    *(EffectModelHeaderCopy *)(effect + 8) = *(EffectModelHeaderCopy *)(source + 8);
    effCopyResourceOwner((EffResourceOwner *)effect, (EffResourceOwner *)source);
    return effect;
}

void effCopyResourceOwner(EffResourceOwner *dst, EffResourceOwner *src) {
    u32 i;

    if (dst->model != 0) {
        effDestroyModelContext(dst->model);
    }
    dst->model = func_00217680(mdlGetContextResourceGroup(src->model), mdlGetContextResourceId(src->model));
    effInitModelVUState(dst->model);
    if (dst->model->first != NULL) {
        if (dst->plainEntry != 0) {
            mdlAddEntryPlain(dst->model, 0, 0);
        } else {
            mdlAddEntryFlagged(dst->model, 0, 0);
        }
    }
    dst->count = sdfCountMapPositionRecords(dst->model->inner);
    if (src->buffer != 0) {
        if (dst->buffer != 0) {
            for (i = 0; i < dst->count; i++) {
                fileQueueDestroy((u32)dst->entries[i]);
            }
            sdfReleaseResourceAllocation(dst->buffer);
        }
        dst->buffer = sdfAllocGeneralBlock(dst->count * 4);
        dst->entries = (void **)sdfResourceRetainAddress((u32)dst->buffer);
        for (i = 0; i < dst->count; i++) {
            dst->entries[i] = fileQueueClone(*src->entries);
        }
    }
}

void effResetModelMotionAndOwnerFlag(EffResourceOwner *work) {
    sdfMotionSampleAtFrame(work->model->first, 0.0f);
    work->unk04 = 0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029B818);

/* Pass a vector to the VU0 model helpers via vf10 (gcc cannot do this from plain C). */
void effLoadModelPrimaryVector(EffResourceOwner *work, void *vec) {
    VU0_LOAD_VF(vf10, vec);
    mdlStorePrimaryVectorVU(work->model);
}

void effSetModelRotationQuaternion(EffResourceOwner *work, void *vec) {
    VU0_LOAD_VF(vf10, vec);
    mdlUpdateContextRotationBasisFromQuaternion(work->model);
}

void effPropagateResourceModelMask(EffResourceOwner *work, u32 color) {
    mdlBroadcastMasked(work->model, color);
}

void effScaleModelVec(EffResourceOwner *work, float scale) {
    u32 bits;

    VU0_SET_ONES_XYZ(vf10);
    VU0_SCALAR_OP_TMP(bits, scale, "vmulx.xyzw vf10, vf10, vf2x");
    mdlStoreTertiaryVectorVU(work->model);
}

void effObjectListCountersReset(void) {
    D_003BC954 = 0;
    D_003BC950 = 0xffffffff;
}

extern s32 sdfFormatImageSize(u32 format, s32 width, s32 height);
extern SdfTex *func_002D2AB8(s32, s32, u32, u32, u32, u32);
extern void sdfTexCreateFirstPacket(SdfTex *texture);

RefObj *func_0029BD90(SdfTextureFileHeader *source) {
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
    cursor = (u8 *)sdfResourceRetainAddress(allocationHandle);
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
        D_003BC958 = func_002D2AB8(0x100, 0x100, 0x13, 0, 0, 1);
        sdfTexCreateFirstPacket(D_003BC958);
    }
    effSharedTextureReferenceCount++;
    texture->refCount++;
    return texture;
}

u32 effCloneSharedReferenceWithValue(u32 source, u32 value) {
    void *copy;

    copy = func_0029BD90((void *)source);
    ((RefObj *)copy)->index = value;
    return (u32)copy;
}

extern void sdfTexReleaseReference(void *);

void effReleaseSharedReference(RefObj *obj) {
    effSharedTextureReferenceCount--;
    if (effSharedTextureReferenceCount == 0) {
        SdfTex *graphics = D_003BC958;
        graphics->width = 0x100;
        graphics->height = 0x100;
        D_003BC950 = -1;
        sdfTexReleaseReference(graphics);
    }
    obj->refCount--;
    if (obj->refCount == 0) {
        sdfReleaseResourceAllocation(obj->allocationHandle);
    }
}

RefObj *effRetainSharedReference(RefObj *obj) {
    obj->refCount++;
    effSharedTextureReferenceCount++;
    return obj;
}

extern SdfTex *func_0029C048(void *, RefObj *);
INCLUDE_ASM(const s32, "game/code_0029A840", func_0029C048);

/* Each 0x10-byte entry contributes itself plus the number stored in its first word. */
typedef struct EffExpandedList {
    u8 pad0[4];
    u32 count;
    u8 pad8[8];
    u8 *entries;
    RefObj **handles; // 0x14
    s32 total;      // 0x18
    u32 refCount;   // 0x1C
    s32 unk20;
    u32 buffer;     // 0x24
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

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029C230);

void effReleaseReferenceHolder(u8 *holder) {
    u32 i;
    if (--((EffExpandedList *)holder)->refCount != 0) {
        return;
    }
    for (i = 0; i < ((EffExpandedList *)holder)->count; i++) {
        effReleaseSharedReference(((EffExpandedList *)holder)->handles[i]);
    }
    sdfReleaseResourceAllocation(((EffExpandedList *)holder)->buffer);
}

EffExpandedList *effReferenceObjectRetain(EffExpandedList *obj) {
    obj->refCount++;
    return obj;
}

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
    return func_0029C048(target, set->handles[sample->segment]);
}

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2AA0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2AC0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC944);

INCLUDE_SDATA(const s32, "game/code_0029A840", effFloorModelListHead);

INCLUDE_SDATA(const s32, "game/code_0029A840", effSharedTextureReferenceCount);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC950);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC954);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC958);

