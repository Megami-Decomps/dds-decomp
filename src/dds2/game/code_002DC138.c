#include "common.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"

typedef struct EffModelOwner {
    f32 scale;
    s32 model;
    void *ownedBuffer;
} EffModelOwner;

extern void func_003343E8(s32, f32);

extern u32 sdfResourceRetainAddress();

extern u32 D_00437E08;

extern void *fileResolvePrimaryBuffer();

extern s32 fileResolveSecondaryBuffer(void *);

extern void *sdfAllocAndClearQuadwords(s32);

extern void sdfReleaseChipBlock();

extern void sdfTexReleaseReference();

extern void effReleaseSharedReference();

extern s64 btlIsRuntimeAllocated(void);

extern s64 btlIsCurrentActorFullyMarked(void);

extern s32 func_002DC1D0(u32, u32);

extern s32 btlFindGroupedEntity(s32, u16);

extern void mdlLoadViewerPackage(s32, u16, s32, u32, u32);

extern u16 D_00437E2C;

extern EffModelOwner *effCreateModelOwner();

extern void effRecreateModelFromSource(EffModelOwner *, EffModelOwner *);

extern void func_0035B6E0();

extern u8 D_00380828[];

extern void func_00232390(void *, const void *);

typedef struct EffectObjectFlag {
    u8 pad00[0xC];
    u32 flags;
} EffectObjectFlag;

typedef struct EffectObjectNode {
    EffectObjectFlag *object;
    struct EffectObjectNode *prev;
    struct EffectObjectNode *next;
} EffectObjectNode;

extern EffectObjectNode *D_00437E30;

extern u32 D_00437E38;

extern u32 D_00437E3C;

extern u32 func_002DDF48(u32);

extern u32 effCloneSharedReferenceWithValue(u32, u32);

extern s32 func_001AA6F8(void);

extern void mdlStoreTertiaryVectorVU(void *);

extern void effFloorModelListRemove(EffectObjectNode *);

extern void *func_002DDAA8(void *);

/* Reference-counted object header (layout inferred from field accesses). */
typedef struct RefObj {
    u8 pad_0x00[0x14]; // 0x00
    s32 refCount;      // 0x14 incremented with the global reference count
    s32 unk18;         // 0x18
    s32 cnt1C;         // 0x1C
} RefObj; // 0x20

extern u32 D_00437E34;

extern void *func_00232198(s32 group, s32 id);

extern s32 func_00232EE8(void *model);

extern s32 func_00232EF8(void *model);

/* VU0 model helpers consume vf10 directly, as in the DDS1 counterpart. */
extern void *func_003292A8(u32);

void effInitModelVUState(void *model) {
VU0_MOVE_VF(vf10, vf0);
    mdlStorePrimaryVectorVU(model);
    VU0_MOVE_VF(vf10, vf0);
    mdlUpdateContextRotationBasisFromQuaternion(model);
    VU0_SET_ONES_XYZ(vf10);
    mdlStoreTertiaryVectorVU(model);
    mdlBroadcastMasked(model, 0x80808080);
    if (*(void **)(model + 0x1C) != NULL) {
        mdlAddEntryPlain(model, 0, 0);
        *(float *)(*(u8 **)(model + 0x1C) + 0x20) = 1.0f;
    }
    *(u32 *)model &= ~1;
}

s32 func_002DC1D0(u32 kind, u32 flags) {
    s32 result;
    while (btlFindGroupedEntity(6, D_00437E2C) != 0) {
        D_00437E2C++;
    }
    mdlLoadViewerPackage(6, D_00437E2C, 0x101, kind, flags);
    result = (s32)func_00232198(6, D_00437E2C);
    effInitModelVUState((void *)result);
    D_00437E2C++;
    return result;
}

void effDestroyModelContext(s32 owner) {
    *(u32 *)(*(s32 *)(owner + 0x18) + 0x80) = 0;
    mdlDestroyContext();
}

void *effCloneModelWithVUState(void *sourceModel) {
    s32 group;
    s32 id;
    void *model;

    group = func_00232EE8(sourceModel);
    id = func_00232EF8(sourceModel);
    model = func_00232198(group, id);
    effInitModelVUState(model);
    return model;
}

EffModelOwner *effCreateModelOwner(u8 *source) {
    EffModelOwner *owner = sdfAllocAndClearQuadwords(0x10);
    owner->ownedBuffer = sdfAllocAndClearQuadwords(0xE0);
    if (source != NULL) {
        s32 data;
        *(u32 *)owner = *(u32 *)fileResolvePrimaryBuffer(source);
        data = fileResolveSecondaryBuffer(source);
        if (data != 0) {
            owner->model = func_002DC1D0(data, *(u32 *)(source + 0x24));
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

EffModelOwner *effDuplicateEffectHeader(EffModelOwner *source) {
    EffModelOwner *owner = effCreateModelOwner(0);
    *(u32 *)owner = *(u32 *)source;
    effRecreateModelFromSource(owner, source);
    return owner;
}

void effRecreateModelFromSource(EffModelOwner *owner, EffModelOwner *source) {
    s32 group;
    s32 id;
    void *model;

    if (owner->model != 0) {
        effDestroyModelContext(owner->model);
    }
    group = func_00232EE8((void *)source->model);
    id = func_00232EF8((void *)source->model);
    model = func_00232198(group, id);
    effInitModelVUState(model);
    VU0_SET_ONES_XYZ(vf10);
    VU0_SCALAR_OP_CLOBBER(owner->scale, "vmulx.xyzw vf10, vf10, vf2x");
    mdlStoreTertiaryVectorVU(model);
    owner->model = (s32)model;
}

void effModelAnimationStop(EffModelOwner *owner) {
    func_003343E8(*(s32 *)(owner->model + 0x1C), 0.0f);
}

void effRefreshModelLighting(void **obj) {
    if (effComputeLightDirectionVU(obj[1], obj[2])) {
        *(u32 *)(*(u8 **)((u8 *)obj[1] + 0x18) + 0x80) = (u32)obj[2];
    }
    func_00232390(obj[1], D_00380828);
}

void effApplyModelPrimaryVector(EffModelOwner *owner, u8 *vec) {
VU0_LOAD_VF_MEMORY(vf10, vec);
    mdlStorePrimaryVectorVU((void *)owner->model);
}

void effApplyModelVecB(EffModelOwner *owner, u8 *vec) {
VU0_LOAD_VF_MEMORY(vf10, vec);
    mdlUpdateContextRotationBasisFromQuaternion((void *)owner->model);
}

void effBroadcastModelMask(EffModelOwner *owner) {
    mdlBroadcastMasked(owner->model);
}

void effApplyScaledModelTertiaryVector(s32 model, float scale) {
    float v[3];
    float t;

    t = *(float *)model * scale;
    v[0] = v[1] = v[2] = t;
    VU0_LOAD_VF(vf10, v);
    mdlStoreTertiaryVectorVU(*(void **)(model + 4));
}

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
    u8 pad_000[0x5B0];
    u8 *slot1; // 0x5B0
    u8 *slot2; // 0x5B4
} EffBattleTexHeaders;

extern s32 func_0032B240();

extern u8 *sdfTexSubmitPixelsForFormat(void *, u32, u8 *, s32);

extern s32 func_0032B3F8(u32, s16, s16, u8, u8 *, s32);

extern u32 sdfTexGetPrimaryResourceWord(void *);

extern u32 sdfTexGetSecondaryResourceWord(void *);

void effUploadModelTextures(EffModelOwner *owner) {
    s32 i = 0;
    EffBattleTexHeaders *battle = (EffBattleTexHeaders *)func_001AA6F8();
    EffTexTable *table = *(EffTexTable **)(*(s32 *)(owner->model + 0x18) + 8);

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
        if (func_0032B240(tex) != 0) {
            pixels = sdfTexSubmitPixelsForFormat(tex, sdfTexGetSecondaryResourceWord(tex), pixels, 1);
        }
        width = tex->width;
        height = tex->height;
        format = tex->format;
        func_0032B3F8(sdfTexGetPrimaryResourceWord(tex), width, height, format, pixels, 1);
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
        effFloorModelListPush(owner);
    }
    return owner;
}

void effMarkFloorModelForDestruction(u32 *p) {
    p[3] |= 2;
    if (!(p[3] & 4)) {
        effDestroyModelOwner(p);
    }
}

EffModelOwner *effDuplicateFloorModelOwner(EffModelOwner *source) {
    EffModelOwner *owner = effCreateModelOwner(0);
    s64 marked;

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
    p[3] |= 1;
    if (!(p[3] & 4)) {
        if (!(D_00437E08 & 1)) {
            func_002DC808(p);
        }
    } else if (D_00437E08 & 1) {
        p[3] |= 0x30;
    }
}

void effFloorModelListPush(EffectObjectFlag *obj) {
    EffectObjectNode *entry = sdfAllocAndClearQuadwords(sizeof(EffectObjectNode));

    entry->object = obj;
    entry->prev = NULL;
    if (D_00437E30 != NULL) {
        D_00437E30->prev = entry;
        entry->next = D_00437E30;
    } else {
        entry->next = NULL;
    }
    D_00437E30 = entry;
    entry->object->flags |= 4;
}

void effFloorModelListRemove(EffectObjectNode *node) {
    if (node->object != NULL) {
        func_0035B6E0("eff:floor model delete[%p]\n", node->object);
        effDestroyModelOwner((EffModelOwner *)node->object);
        node->object = NULL;
    }
    if (node->next != NULL) {
        node->next->prev = node->prev;
    }
    if (node->prev != NULL) {
        node->prev->next = node->next;
    } else {
        D_00437E30 = node->next;
    }
    sdfReleaseChipBlock(node);
}

void effSweepFloorModelList(void) {
    EffectObjectNode *node = D_00437E30;
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
    EffectObjectNode *node = D_00437E30;
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

    node = D_00437E30;
    while (node != NULL) {
        object = node->object;
        node = node->next;
        object->flags = object->flags & 0xffffffef;
    }
}

void mdlSetListedObjectFlag(void) {
    EffectObjectFlag *object;
    EffectObjectNode *node;

    node = D_00437E30;
    while (node != NULL) {
        object = node->object;
        node = node->next;
        object->flags = object->flags | 0x10;
    }
}

void mdlMarkAndProcessObjectNodes(void) {
    EffectObjectNode *node = D_00437E30;
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

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DCCE8);

typedef struct EffResourceOwner {
    u32 count;
    u8 pad_04[0x38];
    u32 plainEntry; // 0x3C: add the model entry plain instead of flagged
    u8 pad_40[0x78];
    void **entries;
    void *buffer;
    u32 model;
} EffResourceOwner;

extern void fileQueueDestroy(s32);

void effDestroyResourceOwner(EffResourceOwner *owner) {
    u32 i;

    if (owner->model != 0) {
        effDestroyModelContext(owner->model);
    }
    if (owner->buffer != 0) {
        for (i = 0; i < owner->count; i++) {
            fileQueueDestroy((s32)owner->entries[i]);
        }
        func_003297C8((u32)owner->buffer);
    }
    sdfReleaseChipBlock(owner);
}

typedef struct { u32 word[0xF]; } EffectBlob3C;

typedef struct { u32 word[0x1D]; } EffectBlob74;

extern u8 *func_002DCCE8(s32);

extern void effCopyResourceOwner(EffResourceOwner *, EffResourceOwner *);

s32 effDuplicateModelOwner(u8 *src) {
    u8 *dst = func_002DCCE8(0);

    *(EffectBlob3C *)(dst + 8) = *(EffectBlob3C *)(src + 8);
    *(EffectBlob74 *)(dst + 0x44) = *(EffectBlob74 *)(src + 0x44);
    effCopyResourceOwner((EffResourceOwner *)dst, (EffResourceOwner *)src);
    return (s32)dst;
}

extern u32 sdfCountMapPositionRecords(u32);

extern void *fileQueueClone(void *);

void effCopyResourceOwner(EffResourceOwner *dst, EffResourceOwner *src) {
    u32 i;

    if (dst->model != 0) {
        effDestroyModelContext(dst->model);
    }
    dst->model = (u32)func_00232198(func_00232EE8((void *)src->model), func_00232EF8((void *)src->model));
    effInitModelVUState((void *)dst->model);
    if (*(s32 *)(dst->model + 0x1C) != 0) {
        if (dst->plainEntry != 0) {
            mdlAddEntryPlain(dst->model, 0, 0);
        } else {
            mdlAddEntryFlagged(dst->model, 0, 0);
        }
    }
    dst->count = sdfCountMapPositionRecords(*(u32 *)(dst->model + 0x18));
    if (src->buffer != 0) {
        if (dst->buffer != 0) {
            for (i = 0; i < dst->count; i++) {
                fileQueueDestroy((s32)dst->entries[i]);
            }
            func_003297C8((u32)dst->buffer);
        }
        dst->buffer = func_003292A8(dst->count * 4);
        dst->entries = (void **)sdfResourceRetainAddress((u32)dst->buffer);
        for (i = 0; i < dst->count; i++) {
            dst->entries[i] = fileQueueClone(*src->entries);
        }
    }
}

extern void fileQueueNotifyAllJobsComplete(s32);

extern void func_003343E8(s32, f32);

void func_002DD3C0(s32 *work) {
    if (work[0xBC / 4] != 0) {
        u32 count = work[0];
        u32 i = 0;
        s32 *entries = (s32 *)work[0xB8 / 4];
        if (count != 0) {
            do {
                fileQueueNotifyAllJobsComplete(*entries);
                entries++;
                i++;
            } while (i < count);
        }
    }
    func_003343E8(*(s32 *)(work[0xC0 / 4] + 0x1C), 0.0f);
    work[1] = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DD448);

void effLoadModelPrimaryVector(u8 *obj, u8 *vec) {
VU0_LOAD_VF_MEMORY(vf10, vec);
    mdlStorePrimaryVectorVU(*(void **)(obj + 0xC0));
}

void func_002DDA30(u8 *obj, u8 *vec) {
VU0_LOAD_VF_MEMORY(vf10, vec);
    mdlUpdateContextRotationBasisFromQuaternion(*(void **)(obj + 0xC0));
}

void func_002DDA50(s32 model) {
    mdlBroadcastMasked(*(u32 *)(model + 0xc0));
}

void effScaleModelVec(u8 *work, float scale) {
    u32 bits;
    VU0_SET_ONES_XYZ(vf10);
    VU0_SCALAR_OP_TMP_MEMORY(bits, scale, "vmulx.xyzw vf10, vf10, vf2x");
    mdlStoreTertiaryVectorVU(*(void **)(work + 0xC0));
}

void effObjectListCountersReset(void) {
    D_00437E3C = 0;
    D_00437E38 = 0xffffffff;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DDAA8);

u32 effCloneSharedReferenceWithValue(u32 source, u32 value) {
    RefObj *copy;

    copy = (RefObj *)func_002DDAA8((void *)source);
    copy->unk18 = value;
    return (u32)copy;
}

extern u8 *D_00437E40;

void effReleaseSharedReference(RefObj *obj) {
    if (--D_00437E34 == 0) {
        u8 *texture = D_00437E40;

        *(u16 *)(texture + 0xC) = 0x100;
        *(u16 *)(texture + 0xE) = 0x100;
        D_00437E38 = 0xffffffff;
        sdfTexReleaseReference(texture);
    }
    if (--obj->refCount == 0) {
        func_003297C8(obj->cnt1C);
    }
}

RefObj *effRetainSharedReference(RefObj *obj) {
    obj->refCount++;
    D_00437E34++;
    return obj;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DDD60);

/* Each 0x10-byte entry contributes itself plus the number stored in its first word. */
typedef struct EffExpandedList {
    u8 pad0[4];
    u32 count;
    u8 pad8[8];
    u8 *entries;
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

    if (--holder[0x1C / 4] == 0) {
        for (i = 0; i < holder[1]; i++) {
            effReleaseSharedReference(((u32 *)holder[0x14 / 4])[i]);
        }
        func_003297C8(holder[0x24 / 4]);
    }
}

RefObj *effReferenceObjectRetain(RefObj *obj) {
    obj->cnt1C++;
    return obj;
}

/* One animation track: segments of `length` frames each (plus one), looping if flags & 1. */
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

void func_002DE218(s32 owner, u32 target, s32 indexSource) {
    func_002DDD60(target, *(u32 *)(*(s32 *)(indexSource + 0xc) * 4 + *(s32 *)(owner + 0x14)));
}

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E2C);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E30);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E34);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E38);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E3C);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E40);

