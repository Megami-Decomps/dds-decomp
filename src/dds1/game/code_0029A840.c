#include "common.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"
#include "sdf_draw.h"

extern u32 sdfResourceRetainAddress(u32);

extern void *sdfAllocGeneralBlock(u32);

extern s32 btlGetRuntime(void);

extern u32 effCloneSharedReferenceWithValue(u32, u32);

extern u32 func_0029C230(u32);

extern void *fileResolvePrimaryBuffer();

extern u32 *fileResolveSecondaryBuffer(void *);

/* Texture record read by the floor-model setup (see SdfTex). */
typedef struct EffTexture {
    u8 pad_00[0xC];
    s16 width;   // 0x0C
    s16 height;  // 0x0E
    u8 pad_10[0xA];
    u8 format;   // 0x1A
    u8 pad_1B[9];
    s32 slot;    // 0x24
} EffTexture;

typedef struct EffTexTable {
    u8 pad_00[0xC];
    EffTexture **entries; // 0x0C
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

/* Effect file request: kind, secondary mode, and a parameter forwarded to instance creation. */
typedef struct EffFileRequest {
    u8 pad_00[0xC];     // 0x00
    u16 kind;           // 0x0C
    u8 pad_0E[0xE];     // 0x0E
    u16 secondaryMode;  // 0x1C
    u8 pad_1E[6];
    u32 resourceParam;  // 0x24
} EffFileRequest;

typedef struct EffModelOwner {
    f32 scale;
    s32 model;
    void *ownedBuffer;
} EffModelOwner;

typedef struct EffectObjectFlag {
    u8 pad00[0xC];
    u32 flags;
} EffectObjectFlag;

typedef struct EffectObjectNode {
    EffectObjectFlag *object;
    struct EffectObjectNode *prev;
    struct EffectObjectNode *next;
} EffectObjectNode;

extern EffectObjectNode *effFloorModelListHead;

extern s64 btlIsRuntimeAllocated(void);

extern s32 btlIsCurrentActorFullyMarked(void);

extern EffModelOwner *effCreateModelOwner();

/* Reference-counted object header (layout inferred from field accesses). */
typedef struct RefObj {
    u8 pad_0x00[0x14]; // 0x00
    s32 refCount;      // 0x14 incremented with the global reference count
    s32 unk18;         // 0x18
    s32 cnt1C;         // 0x1C
} RefObj; // 0x20

extern void sdfMotionSampleAtFrame(void *, float);

extern u32 effSharedTextureReferenceCount;

extern void effFloorModelListRemove(EffectObjectNode *);

extern void mdlStoreTertiaryVectorVU(void *);

extern void *func_0029BD90(void *);

extern void mdlStorePrimaryVectorVU(void *);

extern void mdlUpdateContextRotationBasisFromQuaternion(void *);

/* Model context prefix and the effect owner's lighting attachment. */

typedef struct MdlInner {
    u8 pad00[8];
    u32 resourceHandle;
    u8 pad0C[0x74];
    void *lighting;
} MdlInner;

typedef struct MdlCtx {
    u32 flags;
    u8 pad04[0x14];
    MdlInner *inner;
    Motion *first;
} MdlCtx;

/* Initialize the VU transforms and the first node's float slot, if present. */
void effInitModelVUState(void *model) {
    VU0_MOVE_VF(vf10, vf0);
    mdlStorePrimaryVectorVU(model);
    VU0_MOVE_VF(vf10, vf0);
    mdlUpdateContextRotationBasisFromQuaternion(model);
    VU0_SET_ONES_XYZ(vf10);
    mdlStoreTertiaryVectorVU(model);
    mdlBroadcastMasked(model, 0x80808080);
    if (((MdlCtx *)model)->first != NULL) {
        mdlAddEntryPlain(model, 0, 0);
        ((MdlCtx *)model)->first->frameStep = 1.0f;
    }
    ((MdlCtx *)model)->flags &= ~1;
}

extern u16 D_003BC944;

extern void mdlLoadViewerPackage(u32, u16, u32, u32, u32);

extern void *func_00217680(u32, u32);

extern void effInitModelVUState(void *);

void *effLoadViewerModelWithVUState(u32 first, u32 second) {
    void *model;
    mdlLoadViewerPackage(6, D_003BC944, 0x101, first, second);
    model = func_00217680(6, D_003BC944);
    effInitModelVUState(model);
    D_003BC944++;
    return model;
}

/* Clear the lighting attachment before destroying its model context. */
void effDestroyModelContext(s32 model) {
    ((MdlCtx *)model)->inner->lighting = NULL;
    mdlDestroyContext();
}

EffModelOwner *effCreateModelOwner(u8 *source) {
    EffModelOwner *owner = sdfAllocAndClearQuadwords(0x10);
    owner->ownedBuffer = sdfAllocAndClearQuadwords(0xE0);
    if (source != NULL) {
        s32 data;
        *(u32 *)owner = *(u32 *)fileResolvePrimaryBuffer(source);
        data = fileResolveSecondaryBuffer(source);
        if (data != 0) {
            owner->model = effLoadViewerModelWithVUState(data, ((EffFileRequest *)source)->resourceParam);
            VU0_SET_ONES_XYZ(vf10);
            VU0_SCALE_VF_MFC1(vf10, owner->scale);
            mdlStoreTertiaryVectorVU((void *)owner->model);
        }
    }
    return owner;
}

void effDestroyModelOwner(EffModelOwner *owner) {
    void *buffer = owner->ownedBuffer;
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
    void *model;

    if (owner->model != 0) {
        effDestroyModelContext(owner->model);
    }
    model = func_00217680(mdlGetContextResourceGroup(original->model), mdlGetContextResourceId(original->model));
    effInitModelVUState(model);
    VU0_SET_ONES_XYZ(vf10);
    VU0_SCALE_VF_MFC1(vf10, owner->scale);
    mdlStoreTertiaryVectorVU(model);
    owner->model = (u32)model;
}

void effModelAnimationStop(s32 owner) {
    sdfMotionSampleAtFrame(((MdlCtx *)((EffModelOwner *)owner)->model)->first, 0.0f);
}

extern u8 D_00325828[];

extern s64 effComputeLightDirectionVU(void *, void *);

extern void mdlProcessContextNodesAndTransforms(void *, void *);

/* Attach the owner's lighting buffer when the direction update succeeds. */
void effRefreshModelLighting(u8 *work) {
    void *model;
    if (effComputeLightDirectionVU((void *)((EffModelOwner *)work)->model, ((EffModelOwner *)work)->ownedBuffer) != 0) {
        model = (void *)((EffModelOwner *)work)->model;
        ((MdlCtx *)model)->inner->lighting = ((EffModelOwner *)work)->ownedBuffer;
    } else {
        model = (void *)((EffModelOwner *)work)->model;
    }
    mdlProcessContextNodesAndTransforms(model, D_00325828);
}

/* Pass a vector to the VU0 model helpers via vf10 (gcc cannot do this from plain C). */
void effApplyModelPrimaryVector(s32 owner, void *vec) {
    VU0_LOAD_VF(vf10, vec);
    mdlStorePrimaryVectorVU((void *)((EffModelOwner *)owner)->model);
}

void effApplyModelVecB(s32 owner, void *vec) {
    VU0_LOAD_VF(vf10, vec);
    mdlUpdateContextRotationBasisFromQuaternion((void *)((EffModelOwner *)owner)->model);
}

void effBroadcastModelMask(s32 owner) {
    mdlBroadcastMasked(((EffModelOwner *)owner)->model);
}

void effApplyScaledModelTertiaryVector(s32 model, float scale) {
    float v[3];
    float t;

    t = *(float *)model * scale;
    v[0] = v[1] = v[2] = t;
    VU0_LOAD_VF(vf10, v);
    mdlStoreTertiaryVectorVU((void *)((EffModelOwner *)model)->model);
}

void effUploadModelTextures(EffModelOwner *owner) {
    s32 i = 0;
    EffBattleTexHeaders *battle = (EffBattleTexHeaders *)btlGetRuntime();
    EffTexTable *table = (EffTexTable *)((MdlCtx *)owner->model)->inner->resourceHandle;

    do {
        EffTexture *tex = table->entries[i++];
        u8 *header;
        u8 *pixels;
        s16 width;
        s16 height;
        u8 format;

        switch (tex->slot) {
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
        format = tex->format;
        sdfTexSubmitImageCopy(sdfTexGetPrimaryResourceWord(tex), width, height, format, pixels, 1);
    } while (i < 2);
}

/* Track floor models only while battle is active and the current actor is not fully marked. */
EffModelOwner *effCreateFloorModelOwner(u8 *source) {
    EffModelOwner *owner;
    s64 battleActive;

    owner = effCreateModelOwner(source);
    effUploadModelTextures(owner);
    battleActive = btlIsRuntimeAllocated();
    if ((battleActive != 0) && (battleActive = btlIsCurrentActorFullyMarked(), battleActive == 0)) {
        effFloorModelListPush((EffectObjectFlag *)owner);
    }
    return owner;
}

void effMarkFloorModelForDestruction(u8 *work) {
    u32 flags = ((EffectObjectFlag *)work)->flags | 2;
    ((EffectObjectFlag *)work)->flags = flags;
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
    u32 previous = ((EffectObjectFlag *)work)->flags;
    u32 flags = previous | 1;
    ((EffectObjectFlag *)work)->flags = flags;
    if ((flags & 4) == 0) {
        if ((effModelUpdateControlFlags & 1) == 0) {
            func_0029AE88(work);
        }
    } else if ((effModelUpdateControlFlags & 1) != 0) {
        ((EffectObjectFlag *)work)->flags = previous | 0x31;
    }
}

extern void *sdfAllocAndClearQuadwords(u32);

typedef struct TrackedEffectObject {
    u8 unk_00[0xC];
    u32 flags;
} TrackedEffectObject;

void effFloorModelListPush(EffectObjectFlag *obj) {
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
        effDestroyModelOwner((EffModelOwner *)node->object);
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
    EffectObjectFlag *object;
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
    EffectObjectFlag *object;
    EffectObjectNode *node;

    node = effFloorModelListHead;
    while (node != NULL) {
        object = node->object;
        node = node->next;
        object->flags = object->flags & 0xffffffef;
    }
}

void mdlSetListedObjectFlag(void) {
    EffectObjectFlag *object;
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
    EffectObjectFlag *object;
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

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029B368);

extern void fileQueueDestroy(u32);

/* DDS1 resource owner: same lifetime fields as DDS2, with a shorter payload. */
typedef struct EffResourceOwner {
    u32 count;
    u32 unk04;
    u8 pad_08[0x34];
    u32 plainEntry; // 0x3C: add the model entry plain instead of flagged
    u8 pad_40[4];
    void **entries;
    void *buffer;
    u32 model;
} EffResourceOwner;

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

typedef struct {
    u8 bytes[0x38];
    u32 last;
} EffectModelHeaderCopy;

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
    dst->model = (u32)func_00217680(mdlGetContextResourceGroup(src->model), mdlGetContextResourceId(src->model));
    effInitModelVUState((void *)dst->model);
    if (((MdlCtx *)dst->model)->first != NULL) {
        if (dst->plainEntry != 0) {
            mdlAddEntryPlain(dst->model, 0, 0);
        } else {
            mdlAddEntryFlagged(dst->model, 0, 0);
        }
    }
    dst->count = sdfCountMapPositionRecords((u32)((MdlCtx *)dst->model)->inner);
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

void effResetModelMotionAndOwnerFlag(s32 work) {
    sdfMotionSampleAtFrame(((MdlCtx *)((EffResourceOwner *)work)->model)->first, 0.0f);
    ((EffResourceOwner *)work)->unk04 = 0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029B818);

/* Pass a vector to the VU0 model helpers via vf10 (gcc cannot do this from plain C). */
void effLoadModelPrimaryVector(s32 work, void *vec) {
    VU0_LOAD_VF(vf10, vec);
    mdlStorePrimaryVectorVU((void *)((EffResourceOwner *)work)->model);
}

void effSetModelRotationQuaternion(s32 work, void *vec) {
    VU0_LOAD_VF(vf10, vec);
    mdlUpdateContextRotationBasisFromQuaternion((void *)((EffResourceOwner *)work)->model);
}

void effPropagateResourceModelMask(s32 work) {
    mdlBroadcastMasked(((EffResourceOwner *)work)->model);
}

void effScaleModelVec(s32 work, float scale) {
    u32 bits;

    VU0_SET_ONES_XYZ(vf10);
    VU0_SCALAR_OP_TMP(bits, scale, "vmulx.xyzw vf10, vf10, vf2x");
    mdlStoreTertiaryVectorVU((void *)((EffResourceOwner *)work)->model);
}

void effObjectListCountersReset(void) {
    D_003BC954 = 0;
    D_003BC950 = 0xffffffff;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029BD90);

u32 effCloneSharedReferenceWithValue(u32 source, u32 value) {
    void *copy;

    copy = func_0029BD90((void *)source);
    ((RefObj *)copy)->unk18 = value;
    return (u32)copy;
}

extern u8 *D_003BC958;

extern void sdfTexReleaseReference(void *);

void effReleaseSharedReference(RefObj *obj) {
    effSharedTextureReferenceCount--;
    if (effSharedTextureReferenceCount == 0) {
        u8 *graphics = D_003BC958;
        ((EffTexture *)graphics)->width = 0x100;
        ((EffTexture *)graphics)->height = 0x100;
        D_003BC950 = -1;
        sdfTexReleaseReference(graphics);
    }
    obj->refCount--;
    if (obj->refCount == 0) {
        sdfReleaseResourceAllocation(obj->cnt1C);
    }
}

RefObj *effRetainSharedReference(RefObj *obj) {
    obj->refCount++;
    effSharedTextureReferenceCount++;
    return obj;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029C048);

/* Each 0x10-byte entry contributes itself plus the number stored in its first word. */
typedef struct EffExpandedList {
    u8 pad0[4];
    u32 count;
    u8 pad8[8];
    u8 *entries;
    void **handles; // 0x14
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
        effReleaseSharedReference(((RefObj **)((EffExpandedList *)holder)->handles)[i]);
    }
    sdfReleaseResourceAllocation(((EffExpandedList *)holder)->buffer);
}

RefObj *effReferenceObjectRetain(RefObj *obj) {
    obj->cnt1C++;
    return obj;
}

typedef struct EffAnimSegment {
    u32 length;     // 0x00
    u8 pad_04[0xC];
} EffAnimSegment;

typedef struct EffAnimSet {
    u8 pad_00[4];
    u32 count;                 // 0x04
    u32 flags;                 // 0x08: 1 loop, 4 double speed
    u8 pad_0C[4];
    EffAnimSegment *segments;  // 0x10
    u8 pad_14[4];
    u32 length;                // 0x18
} EffAnimSet;

typedef struct EffAnimSample {
    f32 weight;     // 0x00
    f32 speed;      // 0x04
    u32 pad_08;
    s32 segment;    // 0x0C
} EffAnimSample;

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
    out->weight = 1.0f;
    {
        f32 speed = 2.0f;

        if (!(set->flags & 4)) {
            speed = 1.0f;
        }
        out->segment = segment;
        out->speed = speed;
    }
    out->pad_08 = 0;
}

void effAssignSampledSegmentReference(s32 owner, u32 target, s32 indexSource) {
    func_0029C048(target, (u32)((EffExpandedList *)owner)->handles[((EffAnimSample *)indexSource)->segment]);
}

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2AA0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2AC0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC944);

INCLUDE_SDATA(const s32, "game/code_0029A840", effFloorModelListHead);

INCLUDE_SDATA(const s32, "game/code_0029A840", effSharedTextureReferenceCount);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC950);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC954);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC958);

