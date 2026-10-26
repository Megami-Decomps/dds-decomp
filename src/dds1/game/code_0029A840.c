#include "sdk_ps2.h"
#include "common.h"
#include "eff_texture_table.h"
#include "sdf_vu0.h"
#include "eff_object_node.h"
#include "eff_model_owner.h"
#include "sdf_motion.h"
#include "sdf_packet_list.h"
#include "sdf_packet_builders.h"
#include "sdf_chip.h"
#include "eff_ref_obj.h"
#include "sdf_resource.h"
#include "eff_anim.h"
#include "file.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"
#include "sdf_projection.h"
#include "mdl.h"
#include "sdf.h"
#include "sdf_texture_submit.h"
#include "sdf_texture_file.h"
#include "sdf_tex_packet.h"
#include "sdf_tex_create.h"
#include "sdf_chunk.h"
#include "eff_expanded_source.h"
#include "eff_update_flags.h"
#include "btl_runtime.h"
#include "btl.h"
extern struct EffExpandedList *effCreateTextureAnimationSet(u32);

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

extern u32 D_003BC950;

extern u32 D_003BC954;


extern EffectObjectNode *effFloorModelListHead;

extern u8 btlIsRuntimeAllocated(void);

extern u8 btlIsCurrentActorFullyMarked(void);

extern EffModelOwner *effCreateModelOwner(FileJobPayload *);

extern u32 effSharedTextureReferenceCount;
extern sdfTexHandle_t *D_003BC958;

extern void effFloorModelListRemove(EffectObjectNode *);



/* Initialize the VU transforms and the first node's float slot, if present. */
void effInitModelVUState(dds3ModelHandle_t *model) {
    SDF_VU0_LOADIDENTITYVECTOR();
    mdlSetPosition(model);
    SDF_VU0_LOADIDENTITYVECTOR();
    mdlSetRotate(model);
    SDF_VU0_SETSCALE1VECTOR();
    mdlSetScale(model);
    mdlSetColor(model, 0x80808080);
    if (model->first != NULL) {
        mdlSingleMotion(model, 0, 0);
        model->first->frameStep = 1.0f;
    }
    model->flags &= ~MDL_SKIP_TRANSFORMS;
}

extern u16 D_003BC944;

extern void mdlPackDecode(s32, u16, s32, const u8 *, s32);

extern void effInitModelVUState(dds3ModelHandle_t *);

dds3ModelHandle_t *effLoadViewerModelWithVUState(void *modelData, u32 modelDataSize) {
    dds3ModelHandle_t *model;
    mdlPackDecode(6, D_003BC944, 0x101, modelData, modelDataSize);
    model = mdlLoad(6, D_003BC944);
    effInitModelVUState(model);
    D_003BC944++;
    return model;
}

/* Clear the lighting attachment before destroying its model context. */
void effDestroyModelContext(dds3ModelHandle_t *model) {
    model->inner->lighting = NULL;
    mdlFree(model);
}

EffModelOwner *effCreateModelOwner(FileJobPayload *source) {
    EffModelOwner *owner = sdfAllocAndClearQuadwords(sizeof(EffModelOwner));
    owner->ownedBuffer = sdfAllocAndClearQuadwords(sizeof(*owner->ownedBuffer));
    if (source != NULL) {
        void *data;
        u32 scaleBits;
        scaleBits = *(u32 *)fileResolvePrimaryBuffer(source);
        memcpy(&owner->scale, &scaleBits, sizeof(owner->scale));
        data = fileResolveSecondaryBuffer(source);
        if (data != 0) {
            owner->model = effLoadViewerModelWithVUState(data, source->secondary.size);
            SDF_VU0_SETSCALE1VECTOR();
            VU0_SCALE_VF_MFC1(vf10, owner->scale);
            mdlSetScale(owner->model);
        }
    }
    return owner;
}

void effDestroyModelOwner(EffModelOwner *owner) {
    SdfLightingPacketStorage *buffer = owner->ownedBuffer;
    if (buffer != NULL) {
        sdfChipFreeDirect(buffer);
    }
    if (owner->model != 0) {
        effDestroyModelContext(owner->model);
    }
    sdfChipFreeDirect(owner);
}

extern void effRecreateModelFromSource(EffModelOwner *, EffModelOwner *);

EffModelOwner *effDuplicateEffectHeader(EffModelOwner *source) {
    EffModelOwner *effect = effCreateModelOwner(0);
    memcpy(&effect->scale, &source->scale, sizeof(effect->scale));
    effRecreateModelFromSource(effect, source);
    return effect;
}

void effRecreateModelFromSource(EffModelOwner *work, EffModelOwner *source) {
    EffModelOwner *owner = work;
    EffModelOwner *original = source;
    dds3ModelHandle_t *model;

    if (owner->model != 0) {
        effDestroyModelContext(owner->model);
    }
    model = mdlLoad(mdlGetMajorNum(original->model), mdlGetMinorNum(original->model));
    effInitModelVUState(model);
    SDF_VU0_SETSCALE1VECTOR();
    VU0_SCALE_VF_MFC1(vf10, owner->scale);
    mdlSetScale(model);
    owner->model = model;
}

void effModelAnimationStop(EffModelOwner *owner) {
    sdfMotionSeek(owner->model->first, 0.0f);
}


extern s32 effComputeLightDirectionVU(dds3ModelHandle_t *, SdfLightingPacketStorage *);

#include "sdf_packet_alloc.h"
extern SdfPoolNode *kwlnSceneSurfaceSets[13][4];
extern u64 D_0037E5B0[];

/* Attach the owner's lighting buffer when the direction update succeeds. */
void effRefreshModelLighting(EffModelOwner *work) {
    dds3ModelHandle_t *model;
    if (effComputeLightDirectionVU(work->model, work->ownedBuffer) != 0) {
        model = work->model;
        model->inner->lighting = &work->ownedBuffer->dmaTag;
    } else {
        model = work->model;
    }
    mdlLink(model, kwlnSceneSurfaceSets[10]);
}

/* Pass a vector to the VU0 model helpers via vf10 (gcc cannot do this from plain C). */
void effApplyModelPrimaryVector(EffModelOwner *owner, const f32 *position) {
    SDF_VU0_LOADVECTOR(position);
    mdlSetPosition(owner->model);
}

void effApplyModelVecB(EffModelOwner *owner, const f32 *rotation) {
    SDF_VU0_LOADVECTOR(rotation);
    mdlSetRotate(owner->model);
}

void effBroadcastModelMask(EffModelOwner *owner, u32 color) {
    mdlSetColor(owner->model, color);
}

void effApplyScaledModelTertiaryVector(EffModelOwner *owner, float scale) {
    float v[4];
    float t;

    t = owner->scale * scale;
    v[0] = v[1] = v[2] = t;
    SDF_VU0_LOADVECTOR(v);
    mdlSetScale(owner->model);
}

void effUploadModelTextures(EffModelOwner *owner) {
    s32 i = 0;
    EffBattleTexHeaders *battle = (EffBattleTexHeaders *)btlGetRuntime();
    EffTexTable *table = owner->model->inner->assetData;

    do {
        sdfTexHandle_t *tex = table->entries[i++];
        u8 *header;
        const u8 *pixels;
        s16 width;
        s16 height;
        u8 format;

        switch (tex->battleTextureSlot) {
        case SDF_TEXTURE_BATTLE_SLOT_ONE:
            header = battle->slot1;
            break;
        case SDF_TEXTURE_BATTLE_SLOT_TWO:
            header = battle->slot2;
            break;
        default:
            header = NULL;
            break;
        }
        pixels = header + (header[1] & 0xF0) + 0x40;
        if (sdfTexGetCLUTCount(tex) != 0) {
            pixels = sdfTexTransCLUT(tex, sdfTexGetCLUTAddr(tex), pixels,
                                     SDF_TEXTURE_UPLOAD_BORROW);
        }
        width = tex->width;
        height = tex->height;
        format = tex->pixelFormat;
        sdfTexTransData(sdfTexGetPixelAddr(tex), width, height, format, pixels,
                        SDF_TEXTURE_UPLOAD_BORROW);
    } while (i < 2);
}

void effFloorModelListPush(EffModelOwner *obj);

/* Track floor models only while battle is active and the current actor is not fully marked. */
EffModelOwner *effCreateFloorModelOwner(FileJobPayload *source) {
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

void effMarkFloorModelForDestruction(EffModelOwner *work) {
    u32 flags = work->flags | EFF_MODEL_OWNER_FLAG_DESTROY_REQUESTED;
    work->flags = flags;
    if ((flags & EFF_MODEL_OWNER_FLAG_FLOOR_LISTED) == 0) {
        effDestroyModelOwner(work);
    }
}

extern void effRecreateModelFromSource(EffModelOwner *, EffModelOwner *);

EffModelOwner *effDuplicateFloorModelOwner(EffModelOwner *source) {
    EffModelOwner *owner = effCreateModelOwner(0);

    memcpy(&owner->scale, &source->scale, sizeof(owner->scale));
    effRecreateModelFromSource(owner, source);
    effUploadModelTextures(owner);
    if (btlIsRuntimeAllocated() != 0 && btlIsCurrentActorFullyMarked() == 0) {
        effFloorModelListPush(owner);
    }
    return owner;
}

/* Render the model owner with staged GS TEST_2 packets, then restore the tests. */
void effRenderModelOwnerWithGsTests(EffModelOwner *owner) {
    sdfDrawTag_t *list;
    u64 *packet;
    SdfPoolNode *surface;
    s32 index;

    mdlSetColor(owner->model,
                       (mdlGetColor(owner->model) & 0xFFFFFF) | 0x80000000);

    for (index = 0; index != 4; index++) {
        list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(list);
        packet = (u64 *)sdfGrAllocPacketBuff(0x30);
        packet[0] = 2;
        packet[1] = 0x5000000210000000ULL;
        packet[2] = 0x1000000000008001ULL;
        packet[3] = 0xE;
        if (index == 0) {
            packet[4] = 0x7000D;
        } else {
            packet[4] = 0x71007;
        }
        packet[5] = 0x47;
        sdfDrawTagLink(list, (u32)packet);
        surface = kwlnSceneSurfaceSets[1][index];
        surface->append(surface, list);
    }

    if (effComputeLightDirectionVU(owner->model, owner->ownedBuffer)) {
        owner->model->inner->lighting = &owner->ownedBuffer->dmaTag;
    }

    mdlLink(owner->model, kwlnSceneSurfaceSets[1]);

    for (index = 1; index != 4; index++) {
        list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(list);
        packet = (u64 *)sdfGrAllocPacketBuff(0x30);
        packet[0] = 2;
        packet[1] = 0x5000000210000000ULL;
        packet[2] = 0x1000000000008001ULL;
        packet[3] = 0xE;
        packet[4] = D_0037E5B0[index];
        packet[5] = 0x47;
        sdfDrawTagLink(list, (u32)packet);
        surface = kwlnSceneSurfaceSets[1][index];
        surface->append(surface, list);
    }
}

extern u32 effModelUpdateControlFlags;

void effMarkFloorModelForUpdate(EffModelOwner *work) {
    u32 previous = work->flags;
    u32 flags = previous | EFF_MODEL_OWNER_FLAG_UPDATE_PENDING;
    work->flags = flags;
    if ((flags & EFF_MODEL_OWNER_FLAG_FLOOR_LISTED) == 0) {
        if ((effModelUpdateControlFlags & EFF_MODEL_UPDATE_DEFER_FLOOR_REFRESH) == 0) {
            effRenderModelOwnerWithGsTests(work);
        }
    } else if ((effModelUpdateControlFlags & EFF_MODEL_UPDATE_DEFER_FLOOR_REFRESH) != 0) {
        work->flags = previous | EFF_MODEL_OWNER_FLAG_UPDATE_PENDING |
                      EFF_MODEL_OWNER_FLAG_LISTED_OBJECT | EFF_MODEL_OWNER_FLAG_UPDATE_DEFERRED;
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
    ((EffModelOwner *)entry->object)->flags |= EFF_MODEL_OWNER_FLAG_FLOOR_LISTED;
}

extern char D_003B2AA0[];

#include "sdf_debug_api.h"

void effFloorModelListRemove(EffectObjectNode *node) {
    if (node->object != NULL) {
        printf(D_003B2AA0, node->object);
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
    sdfChipFreeDirect(node);
}

void effSweepFloorModelList(void) {
    EffectObjectNode *node = effFloorModelListHead;
    EffectObjectNode *next;

    if (node != NULL) {
        do {
            next = node->next;
            if (!(((EffModelOwner *)node->object)->flags & EFF_MODEL_OWNER_FLAG_DESTROY_READY)) {
                if ((((EffModelOwner *)node->object)->flags &
                     (EFF_MODEL_OWNER_FLAG_UPDATE_PENDING | EFF_MODEL_OWNER_FLAG_LISTED_OBJECT |
                      EFF_MODEL_OWNER_FLAG_UPDATE_DEFERRED)) == EFF_MODEL_OWNER_FLAG_UPDATE_PENDING) {
                    effRenderModelOwnerWithGsTests(node->object);
                }
                ((EffModelOwner *)node->object)->flags &= ~EFF_MODEL_OWNER_FLAG_UPDATE_DEFERRED;
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
            if ((flags & EFF_MODEL_OWNER_FLAG_DESTROY_REQUESTED) != 0) {
                object->flags = flags | EFF_MODEL_OWNER_FLAG_DESTROY_READY;
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
        object->flags = object->flags & ~EFF_MODEL_OWNER_FLAG_LISTED_OBJECT;
    }
}

void mdlSetListedObjectFlag(void) {
    EffModelOwner *object;
    EffectObjectNode *node;

    node = effFloorModelListHead;
    while (node != NULL) {
        object = node->object;
        node = node->next;
        object->flags = object->flags | EFF_MODEL_OWNER_FLAG_LISTED_OBJECT;
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
        flags = object->flags | EFF_MODEL_OWNER_FLAG_DESTROY_REQUESTED |
                EFF_MODEL_OWNER_FLAG_DESTROY_READY;
        object->flags = flags;
        effFloorModelListRemove(node);
        node = next;
    } while (node != NULL);
}

#include "eff_resource_owner.h"

typedef struct EffResourceModelConfig {
    SdfColorTrack colorTrack;
    SdfAlphaTrack alphaTrack;
    u32 plainEntry;
    f32 frameStep;
} EffResourceModelConfig;

typedef char EffResourceModelConfig_size_check[
    sizeof(EffResourceModelConfig) == 0x3C ? 1 : -1];


/* Create an empty owner, or load its model and per-record queues from a file job. */
EffResourceOwner *effCreateResourceOwner(void *input) {
    FileJobPayload *source = input;
    EffResourceOwner *owner = sdfChipAllocDirect(sizeof(EffResourceOwner));
    u8 *data;
    u32 i;

    memset(owner, 0, sizeof(EffResourceOwner));
    if (source != NULL) {
        data = fileResolvePrimaryBuffer(source);
        memcpy((u8 *)owner + 8, data, sizeof(EffResourceModelConfig));
        owner->model = effLoadViewerModelWithVUState(data + 0x40,
                                                   source->primary.size - 0x40);
        if (owner->model->first != NULL) {
            if (owner->plainEntry != 0) {
                mdlSingleMotion(owner->model, 0, 0);
            } else {
                mdlLoopMotion(owner->model, 0, 0);
            }
        }
        owner->count = sdfModelGetMarkPosCount(owner->model->inner);
        if (owner->count == 0) {
            return owner;
        }
        data = (u8 *)fileResolveSecondaryBuffer(source);
        if (data != NULL) {
            owner->queueAllocation = sdfMemAllocDirect(owner->count * 4);
            owner->entries = sdfMemLock(owner->queueAllocation);
            owner->entries[0] = fileCloneQueueEntries((struct FileQueue *)data);
            for (i = 1; i < owner->count; i++) {
                owner->entries[i] = fileQueueClone(owner->entries[0]);
            }
        }
    }
    return owner;
}



void effDestroyResourceOwner(EffResourceOwner *owner) {
    u32 i;

    if (owner->model != 0) {
        effDestroyModelContext(owner->model);
    }
    if (owner->queueAllocation != 0) {
        for (i = 0; i < owner->count; i++) {
            fileQueueDestroy(owner->entries[i]);
        }
        sdfMemFreeDirect(owner->queueAllocation);
    }
    sdfChipFreeDirect(owner);
}


extern EffResourceOwner *effCreateResourceOwner(void *);

extern void effCopyResourceOwner(EffResourceOwner *, EffResourceOwner *);

EffResourceOwner *effDuplicateModelOwner(EffResourceOwner *source) {
    EffResourceOwner *effect = effCreateResourceOwner(0);

    *(EffResourceModelConfig *)((u8 *)effect + 8) =
        *(EffResourceModelConfig *)((u8 *)source + 8);
    effCopyResourceOwner(effect, source);
    return effect;
}

void effCopyResourceOwner(EffResourceOwner *dst, EffResourceOwner *src) {
    u32 i;

    if (dst->model != 0) {
        effDestroyModelContext(dst->model);
    }
    dst->model = mdlLoad(mdlGetMajorNum(src->model), mdlGetMinorNum(src->model));
    effInitModelVUState(dst->model);
    if (dst->model->first != NULL) {
        if (dst->plainEntry != 0) {
            mdlSingleMotion(dst->model, 0, 0);
        } else {
            mdlLoopMotion(dst->model, 0, 0);
        }
    }
    dst->count = sdfModelGetMarkPosCount(dst->model->inner);
    if (src->queueAllocation != 0) {
        if (dst->queueAllocation != 0) {
            for (i = 0; i < dst->count; i++) {
                fileQueueDestroy(dst->entries[i]);
            }
            sdfMemFreeDirect(dst->queueAllocation);
        }
        dst->queueAllocation = sdfMemAllocDirect(dst->count * 4);
        dst->entries = sdfMemLock(dst->queueAllocation);
        for (i = 0; i < dst->count; i++) {
            dst->entries[i] = fileQueueClone(*src->entries);
        }
    }
}

void effResetModelMotionAndOwnerFlag(EffResourceOwner *work) {
    sdfMotionSeek(work->model->first, 0.0f);
    work->currentFrame = 0;
}

extern void effGetWorldVector(u32);
extern s32 sdfModelGetMarkPosMatrix(sdfModelRootNode_t *, s32);
extern void sdfVuMatrixToQuaternion(const f32 [4][4]);
extern void btlDebugDrawSegment(f32 *, f32 *, u32);
extern void btlDebugDrawPoint(f32 *);
extern f32 effWorldUpVector[4];
extern f32 D_0037E110[4];
extern f32 D_0037E100[4];
extern f32 D_003B2AC0[4];

void func_0029B818(EffResourceOwner *owner) {
    f32 matrix[4][4];
    f32 position[4];
    f32 endpoint[4];
    f32 rotation[4];
    f32 scale;
    f32 distanceScale = 1.0f;
    f32 frameStep = owner->frameStep;
    f32 distance;
    s16 targetKind;
    u32 count;
    sdfModelRootNode_t *model;
    FileQueue **queue;
    u32 index;
    u32 sampledColor;
    u32 color;
    s32 frame;
    s32 limit;
    u32 broadcastColor;
    u32 byteUnit;

    mdlGetScale(owner->model);
    SDF_VU0_GETVECTOR_X(scale);
    if (btlIsRuntimeAllocated() != 0 &&
        (owner->alphaTrack.surfaceIndex & 0xFF0000) != 0) {
        effGetWorldVector(owner->alphaTrack.surfaceIndex >> 16);
        SDF_VU0_STOREVECTOR_NOMEM(position);
        targetKind = owner->alphaTrack.surfaceIndex >> 16;
        if (targetKind == 5) {
            position[1] -= 400.0f;
        }
        mdlGetPosition(owner->model);
        SDF_VU0_STOREVECTOR_NOMEM(endpoint);
        LookAtMatrix(position, endpoint, effWorldUpVector);
        ReverseMatrix();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_LOADVECTOR_B(endpoint);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_GETLENGTH(distance);
        distanceScale = distance / 800.0f;
        distanceScale /= scale;
        SDF_VU0_SETSCALE1VECTOR();
        VU0_SCALAR_OP(distanceScale, "vmulx.xyzw vf10, vf10, vf2x");
        SDF_VU0_SCALE_XYZ();
        SDF_VU0_STORE_ROW0_NOMEM(owner->model->inner->matrix[0]);
        SDF_VU0_STORE_ROW1_NOMEM(owner->model->inner->matrix[1]);
        SDF_VU0_STORE_ROW2_NOMEM(owner->model->inner->matrix[2]);
    }
    owner->model->first->frameStep = frameStep;
    mdlLink(owner->model, kwlnSceneSurfaceSets[10]);
    frame = owner->currentFrame;
    count = owner->count;
    limit = owner->plainEntry;
    model = owner->model->inner;
    queue = owner->entries;
    if (frame < limit || limit == 0) {
        if (owner->queueAllocation != NULL) {
            sampledColor = effSampleColorAlphaTracks(&owner->colorTrack,
                &owner->alphaTrack, owner->currentFrame, owner->plainEntry);
            broadcastColor = mdlGetColor(owner->model);
            byteUnit = 0x3C000000;
            eeUnpackRGBAWord(broadcastColor, byteUnit);
            SDF_VU0_COPYVECTOR_A_TO_B();
            eeUnpackRGBAWord(sampledColor, byteUnit);
            SDF_VU0_MULVECTOR();
            color = eePackRGBAWord(128.0f);
            for (index = 0; index < count; index++, queue++) {
                sdfModelGetMarkPosMatrix(model, index);
                VU0_STORE_VF_UNCLOBBERED(vf31, position);
                if (scale != 0.0f) {
                    SDF_VU0_SETSCALE1VECTOR();
                    VU0_SCALE_VF(vf10, 1.0f / scale);
                    SDF_VU0_SCALE_XYZ();
                }
                VU0_STORE_MATRIX(matrix);
                fileQueueSetPosition(*queue, position);
                sdfVuMatrixToQuaternion(matrix);
                SDF_VU0_LOADVECTOR_B(D_003B2AC0);
                effMiscQuatMultiplyVU();
                SDF_VU0_STOREVECTOR_NOMEM(rotation);
                fileQueueSetRotation(*queue, rotation);
                fileQueueSetScale(*queue, scale * distanceScale);
                fileQueueSetColor(*queue, color);
                fileQueueUpdate(*queue);
            }
        } else {
            for (index = 0; index < count; index++) {
                sdfModelGetMarkPosMatrix(model, index);
                VU0_STORE_VF_UNCLOBBERED(vf31, position);
                VU0_STORE_MATRIX(matrix);
                SDF_VU0_LOADVECTOR(D_0037E110);
                VU0_SCALE_VF(vf10, 50.0f);
                SDF_VU0_APPLYMATRIX_XYZ1();
                SDF_VU0_STOREVECTOR_NOMEM(endpoint);
                btlDebugDrawSegment(position, endpoint, 0x80006000);
                VU0_LOAD_MATRIX_MEMORY(matrix);
                SDF_VU0_LOADVECTOR(effWorldUpVector);
                VU0_SCALE_VF(vf10, 50.0f);
                SDF_VU0_APPLYMATRIX_XYZ1();
                SDF_VU0_STOREVECTOR_NOMEM(endpoint);
                btlDebugDrawSegment(position, endpoint, 0x80600000);
                VU0_LOAD_MATRIX_MEMORY(matrix);
                SDF_VU0_LOADVECTOR(D_0037E100);
                VU0_SCALE_VF(vf10, 50.0f);
                SDF_VU0_APPLYMATRIX_XYZ1();
                SDF_VU0_STOREVECTOR_NOMEM(endpoint);
                btlDebugDrawSegment(position, endpoint, 0x80000060);
                btlDebugDrawPoint(position);
            }
        }
    }
    owner->currentFrame++;
}

/* Pass a vector to the VU0 model helpers via vf10 (gcc cannot do this from plain C). */
void effLoadModelPrimaryVector(EffResourceOwner *work, const f32 *position) {
    SDF_VU0_LOADVECTOR(position);
    mdlSetPosition(work->model);
}

void effSetModelRotationQuaternion(EffResourceOwner *work, const f32 *rotationQuaternion) {
    SDF_VU0_LOADVECTOR(rotationQuaternion);
    mdlSetRotate(work->model);
}

void effPropagateResourceModelMask(EffResourceOwner *work, u32 color) {
    mdlSetColor(work->model, color);
}

void effScaleModelVec(EffResourceOwner *work, float scale) {
    u32 bits;

    SDF_VU0_SETSCALE1VECTOR();
    VU0_SCALAR_OP_TMP(bits, scale, "vmulx.xyzw vf10, vf10, vf2x");
    mdlSetScale(work->model);
}

void effObjectListCountersReset(void) {
    D_003BC954 = 0;
    D_003BC950 = 0xffffffff;
}

extern s32 sdfImageDataSize(u32 format, s32 width, s32 height);

RefObj *effCreateSharedTextureReference(SdfTextureFileHeader *source) {
    u32 paletteWidth;
    u32 paletteHeight;
    s32 paletteBytes;
    s32 imageBytes;
    s32 payloadBytes;
    s32 textureOffset;
    struct sdfMemHandle_t *allocationHandle;
    u8 *cursor;
    RefObj *texture;

    if ((source->pixelFormat == 0x13) || (source->pixelFormat == 0x1B)) {
        paletteWidth = 0x10;
        paletteHeight = 0x10;
    } else {
        paletteWidth = 8;
        paletteHeight = 2;
    }

    paletteBytes = sdfImageDataSize(source->clutFormat, paletteWidth, paletteHeight) << 4;
    imageBytes = sdfImageDataSize(source->pixelFormat, source->width, source->height) << 4;
    payloadBytes = imageBytes + paletteBytes;
    textureOffset = payloadBytes + 0x40;
    allocationHandle = sdfMemAllocDirect(payloadBytes + 0x60);
    cursor = (u8 *)sdfMemLock(allocationHandle);
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
    cursor = (u8 *)source + 0x40 + (source->payloadOffsetFlags & SDF_TEXTURE_PAYLOAD_EXTRA_OFFSET_MASK);
    memcpy(texture->palette, cursor, paletteBytes);
    cursor += paletteBytes;
    memcpy(texture->pixels, cursor, imageBytes);

    if (effSharedTextureReferenceCount == 0) {
        D_003BC958 = sdfTexAlloc(0x100, 0x100, 0x13, 0, 0, 1);
        sdfTexMakeGIFPacket(D_003BC958);
    }
    effSharedTextureReferenceCount++;
    texture->refCount++;
    return texture;
}

RefObj *effCloneSharedReferenceWithValue(SdfTextureFileHeader *source, u32 textureIndex) {
    RefObj *copy;

    copy = effCreateSharedTextureReference(source);
    copy->index = textureIndex;
    return copy;
}
void effReleaseSharedReference(RefObj *obj) {
    effSharedTextureReferenceCount--;
    if (effSharedTextureReferenceCount == 0) {
        sdfTexHandle_t *graphics = D_003BC958;
        graphics->width = 0x100;
        graphics->height = 0x100;
        D_003BC950 = -1;
        sdfTexReleaseReference(graphics);
    }
    obj->refCount--;
    if (obj->refCount == 0) {
        sdfMemFreeDirect(obj->allocationHandle);
    }
}

RefObj *effRetainSharedReference(RefObj *obj) {
    obj->refCount++;
    effSharedTextureReferenceCount++;
    return obj;
}

/* Native GS dimension SDK calls: 0029C16C/0029C17C.
 * Address normalization before TEX0 packing is also traced in
 * texture_renew_secondary_epoch_probe (parity OK, local-alloc q14/q16). */
sdfTexHandle_t *sdfGetOrCreateTextureForReference(SdfPoolNode *surface, RefObj *source) {
    sdfDrawTag_t *list;
    u16 width;
    u16 height;
    s32 widthLog2;
    s32 heightLog2;
    sdfTexHandle_t *texture;
    u32 primary;
    u32 secondary;
    u8 clutFormat;
    u8 pixelFormat;

    if (source->index != -1 && D_003BC950 == source->index && D_003BC954 == (u32)surface) {
        return D_003BC958;
    }
    D_003BC950 = source->index;
    D_003BC954 = (u32)surface;
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    sdfLinkLoadImage(list, D_003BC958->secondaryResource, 0, 0, source->paletteWidth, source->paletteHeight, (s32)source->palette, 0);
    sdfLinkLoadImage(list, D_003BC958->primaryResource, 0, 0, ((SdfTextureFileHeader *)source->base)->width, ((SdfTextureFileHeader *)source->base)->height, (s32)source->pixels, 0);
    surface->append(surface, list);
    texture = D_003BC958;
    width = ((SdfTextureFileHeader *)source->base)->width;
    texture->width = width;
    widthLog2 = (s16)width - 1;
    primary = texture->primaryResource->word >> 6;
    height = ((SdfTextureFileHeader *)source->base)->height;
    texture->height = height;
    clutFormat = texture->clutFormat;
    secondary = texture->secondaryResource->word;
    pixelFormat = texture->pixelFormat;
    heightLog2 = (s16)height - 1;
    EE_MMI_BIT_LENGTH(heightLog2);
    EE_MMI_BIT_LENGTH(widthLog2);
    secondary >>= 6;
    texture->primaryBuffer->textureState =
        ((u64)2 << 61) |
        ((u64)clutFormat << 51) |
        ((u64)secondary << 37) |
        ((u64)1 << 34) |
        ((u64)heightLog2 << 30) |
        (widthLog2 << 26) |
        ((u64)pixelFormat << 20) |
        (4 << 14) |
        (u64)primary;
    return D_003BC958;
}

s32 effCountExpandedEntries(const EffExpandedList *list) {
    u32 count = list->count;
    u32 i = 0;
    s32 total = 0;

    if (count != 0) {
        const EffExpandedEntry *entries = list->entries;
        do {
            s32 additionalCount = entries->additionalCount;
            entries++;
            i++;
            total++;
            total += additionalCount;
        } while (i < count);
    }
    return total;
}

/* Serialized expanded list: 0x10-byte header, the entry table, then each entry's payload. */
/* Copy a serialized list into one allocation and retain a shared texture reference per entry. */
struct EffExpandedList *effCreateTextureAnimationSet(u32 sourceAddress) {
    const EffExpandedSource *source = (const EffExpandedSource *)sourceAddress;
    u32 count = source->count;
    const EffExpandedEntry *sourceEntry = source->entries;
    u32 i = 0;
    struct sdfMemHandle_t *allocation;
    EffExpandedList *list;
    u8 *payload;

    allocation = sdfMemAllocDirect(count * (sizeof(EffExpandedEntry) + sizeof(RefObj *)) + sizeof(EffExpandedList));
    list = (EffExpandedList *)sdfMemLock(allocation);
    list->entries = (EffExpandedEntry *)(list + 1);
    list->handles = (RefObj **)(list->entries + count);
    list->unk20 = -1;
    list->allocation = allocation;
    list->refCount = 0;
    memcpy(list, source, 0x10);
    payload = (u8 *)(sourceEntry + count);
    if (count != 0) {
        do {
            list->entries[i] = *sourceEntry;
            list->handles[i] = effCreateSharedTextureReference((SdfTextureFileHeader *)payload);
            payload += sourceEntry->payloadSize;
            sourceEntry++;
            i++;
        } while (i < count);
    }
    list->totalEntryCount = effCountExpandedEntries(list);
    list->refCount++;
    return list;
}

void effReleaseReferenceHolder(EffExpandedList *holder) {
    u32 i;
    if (--holder->refCount != 0) {
        return;
    }
    for (i = 0; i < holder->count; i++) {
        effReleaseSharedReference(holder->handles[i]);
    }
    sdfMemFreeDirect(holder->allocation);
}

EffExpandedList *effReferenceObjectRetain(EffExpandedList *obj) {
    obj->refCount++;
    return obj;
}

void effSampleAnimSet(const EffExpandedList *set, u32 frame, EffAnimSample *out) {
    u32 count = set->count;
    u32 local = 0;
    s32 segment = -1;
    const EffExpandedEntry *seg;
    u32 acc;
    u32 i;

    if (count == 1) {
        segment = 0;
    } else {
        if (set->flags & EFF_ANIM_SET_LOOP) {
            local = frame % set->totalEntryCount;
        } else if (frame >= set->totalEntryCount) {
            segment = count - 1;
        } else {
            local = frame;
        }
        if (segment == -1) {
            seg = set->entries;
            acc = 0;
            for (i = 0; i < count; i++) {
                acc += seg->additionalCount;
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

        if (!(set->flags & EFF_ANIM_SET_DOUBLE_Y_SCALE)) {
            scaleY = 1.0f;
        }
        out->segment = segment;
        out->scaleY = scaleY;
    }
    out->angle = 0.0f;
}

sdfTexHandle_t *effAssignSampledSegmentReference(const EffExpandedList *set, SdfPoolNode *target,
                                                 const EffAnimSample *sample) {
    return sdfGetOrCreateTextureForReference(target, set->handles[sample->segment]);
}

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2AA0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2AC0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC944);

INCLUDE_SDATA(const s32, "game/code_0029A840", effFloorModelListHead);

INCLUDE_SDATA(const s32, "game/code_0029A840", effSharedTextureReferenceCount);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC950);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC954);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC958);
