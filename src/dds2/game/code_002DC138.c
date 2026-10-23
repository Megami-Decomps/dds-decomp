#include "sdk_ps2.h"
#include "common.h"
#include "eff_texture_table.h"
#include "sdf_vu0.h"
#include "eff_object_node.h"
#include "eff_model_owner.h"
#include "sdf_motion.h"
#include "sdf_packet_list.h"
#include "sdf_chip.h"
#include "eff_ref_obj.h"
#include "sdf_resource.h"
#include "eff_anim.h"
#include "file.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "mdl.h"
#include "mdl_broadcast_callback.h"
#include "btl_grouped_entity_api.h"
#include "sdf.h"
#include "sdf_texture_submit.h"
#include "sdf_texture_file.h"
#include "sdf_tex_packet.h"
#include "sdf_tex_create.h"
#include "sdf_chunk.h"
#include "eff_expanded_source.h"
#include "eff_update_flags.h"
#include "btl_runtime.h"

extern u32 effModelUpdateControlFlags;
extern u8 btlIsRuntimeAllocated(void);

extern s32 btlIsCurrentActorFullyMarked(void);

extern dds3ModelHandle_t *effLoadViewerModelWithVUState(void *, u32);

extern void mdlPackDecode(s32, u16, s32, void *, u32);

extern u16 D_00437E2C;

extern EffModelOwner *effCreateModelOwner(void *);

extern void effRecreateModelFromSource(EffModelOwner *, EffModelOwner *);

#include "sdf_debug_api.h"




extern EffectObjectNode *effFloorModelListHead;

extern u32 D_00437E38;

extern SdfPoolNode *D_00437E3C;

extern struct EffExpandedList *effCreateTextureAnimationSet(u32);
extern s32 effComputeLightDirectionVU(dds3ModelHandle_t *, SdfLightingPacketStorage *);

extern void effFloorModelListRemove(EffectObjectNode *);

#include "sdf_packet_alloc.h"
extern SdfPoolNode *kwlnSceneSurfaceSets[13][4];
extern u64 D_003E9640[];

extern u32 effSharedTextureReferenceCount;
extern sdfTexHandle_t *D_00437E40;

/* VU0 model helpers consume vf10 directly, as in the DDS1 counterpart. */


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

/* Load under an unused group-6 viewer ID and initialize the model's VU state. */
dds3ModelHandle_t *effLoadViewerModelWithVUState(void *modelData, u32 parameter) {
    dds3ModelHandle_t *result;
    while (mdlSearchResrc(6, D_00437E2C) != 0) {
        D_00437E2C++;
    }
    mdlPackDecode(6, D_00437E2C, 0x101, modelData, parameter);
    result = mdlLoad(6, D_00437E2C);
    effInitModelVUState(result);
    D_00437E2C++;
    return result;
}

/* Clear the lighting attachment before destroying its model context. */
void effDestroyModelContext(dds3ModelHandle_t *owner) {
    owner->inner->lighting = NULL;
    mdlFree(owner);
}

dds3ModelHandle_t *effCloneModelWithVUState(dds3ModelHandle_t *sourceModel) {
    s32 group;
    s32 id;
    dds3ModelHandle_t *model;

    group = mdlGetMajorNum(sourceModel);
    id = mdlGetMinorNum(sourceModel);
    model = mdlLoad(group, id);
    effInitModelVUState(model);
    return model;
}


EffModelOwner *effCreateModelOwner(void *input) {
    FileJobPayload *source = input;
    EffModelOwner *owner = sdfAllocAndClearQuadwords(sizeof(*owner));
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

EffModelOwner *effDuplicateEffectHeader(EffModelOwner *source) {
    EffModelOwner *owner = effCreateModelOwner(0);
    memcpy(&owner->scale, &source->scale, sizeof(owner->scale));
    effRecreateModelFromSource(owner, source);
    return owner;
}

void effRecreateModelFromSource(EffModelOwner *owner, EffModelOwner *source) {
    s32 group;
    s32 id;
    dds3ModelHandle_t *model;

    if (owner->model != 0) {
        effDestroyModelContext(owner->model);
    }
    group = mdlGetMajorNum(source->model);
    id = mdlGetMinorNum(source->model);
    model = mdlLoad(group, id);
    effInitModelVUState(model);
    SDF_VU0_SETSCALE1VECTOR();
    VU0_SCALAR_OP_CLOBBER(owner->scale, "vmulx.xyzw vf10, vf10, vf2x");
    mdlSetScale(model);
    owner->model = model;
}

void effModelAnimationStop(EffModelOwner *owner) {
    sdfMotionSeek(owner->model->first, 0.0f);
}

/* Attach the owner's lighting buffer when the direction update succeeds. */
void effRefreshModelLighting(EffModelOwner *work) {
    dds3ModelHandle_t *model;
    if (effComputeLightDirectionVU(work->model, work->ownedBuffer)) {
        model = work->model;
        model->inner->lighting = &work->ownedBuffer->dmaTag;
    } else {
        model = work->model;
    }
    mdlLink(model, kwlnSceneSurfaceSets[10]);
}

void effApplyModelPrimaryVector(EffModelOwner *owner, u8 *vec) {
SDF_VU0_LOADVECTOR_BARRIER(vec);
    mdlSetPosition(owner->model);
}

void effApplyModelVecB(EffModelOwner *owner, u8 *vec) {
SDF_VU0_LOADVECTOR_BARRIER(vec);
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


/* Battle state: resource headers for texture slots 1 and 2. */
typedef struct EffBattleTexHeaders {
    u8 pad_000[0x5B0];
    u8 *slot1; // 0x5B0
    u8 *slot2; // 0x5B4
} EffBattleTexHeaders;

void effUploadModelTextures(EffModelOwner *owner) {
    s32 i = 0;
    EffBattleTexHeaders *battle = (EffBattleTexHeaders *)btlGetRuntime();
    EffTexTable *table = owner->model->inner->assetData;

    do {
        sdfTexHandle_t *tex = table->entries[i++];
        u8 *header;
        u8 *pixels;
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
            pixels = sdfTexTransCLUT(tex, sdfTexGetCLUTAddr(tex), pixels, 1);
        }
        width = tex->width;
        height = tex->height;
        format = tex->pixelFormat;
        sdfTexTransData(sdfTexGetPixelAddr(tex), width, height, format, pixels, 1);
    } while (i < 2);
}

extern void effFloorModelListPush(EffModelOwner *);

/* Track floor models only while battle is active and the current actor is not fully marked. */
EffModelOwner *effCreateFloorModelOwner(void *source) {
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

void effMarkFloorModelForDestruction(EffModelOwner *p) {
    p->flags |= 2;
    if (!(p->flags & 4)) {
        effDestroyModelOwner(p);
    }
}

EffModelOwner *effDuplicateFloorModelOwner(EffModelOwner *source) {
    EffModelOwner *owner = effCreateModelOwner(0);
    s32 marked;

    memcpy(&owner->scale, &source->scale, sizeof(owner->scale));
    effRecreateModelFromSource(owner, source);
    effUploadModelTextures(owner);
    marked = btlIsRuntimeAllocated();
    if ((marked != 0) && (marked = btlIsCurrentActorFullyMarked(), marked == 0)) {
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

    mdlApplyBroadcastVia(mdlSetColor, owner->model,
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
        packet[4] = D_003E9640[index];
        packet[5] = 0x47;
        sdfDrawTagLink(list, (u32)packet);
        surface = kwlnSceneSurfaceSets[1][index];
        surface->append(surface, list);
    }
}

void effMarkFloorModelForUpdate(EffModelOwner *p) {
    p->flags |= 1;
    if (!(p->flags & 4)) {
        if (!(effModelUpdateControlFlags & EFF_MODEL_UPDATE_DEFER_FLOOR_REFRESH)) {
            effRenderModelOwnerWithGsTests(p);
        }
    } else if (effModelUpdateControlFlags & EFF_MODEL_UPDATE_DEFER_FLOOR_REFRESH) {
        p->flags |= 0x30;
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
    ((EffModelOwner *)entry->object)->flags |= 4;
}

void effFloorModelListRemove(EffectObjectNode *node) {
    if (node->object != NULL) {
        printf("eff:floor model delete[%p]\n", node->object);
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
            if (!(((EffModelOwner *)node->object)->flags & 8)) {
                if ((((EffModelOwner *)node->object)->flags & 0x31) == 1) {
                    effRenderModelOwnerWithGsTests(node->object);
                }
                ((EffModelOwner *)node->object)->flags &= ~0x20;
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


#include "eff_resource_owner.h"

typedef char EffResourceOwner_buffer_check[(u32)&((EffResourceOwner *)0)->buffer == 0xBC ? 1 : -1];
typedef char EffResourceOwner_model_check[(u32)&((EffResourceOwner *)0)->model == 0xC0 ? 1 : -1];

extern void *effCreateResourceOwner(void *);

/* Decode the type-6 configuration, load its model and clone per-position queues. */
void *effCreateResourceOwner(void *input) {
    FileJobPayload *job = input;
    EffResourceOwner *owner;
    void *resource;
    u32 modelOffset;
    u32 i;

    owner = sdfChipAllocDirect(sizeof(*owner));
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
        owner->model = effLoadViewerModelWithVUState((u8 *)resource + modelOffset, job->primary.size - modelOffset);
        if (owner->model->first != NULL) {
            if (owner->base.plainEntry != 0) {
                mdlSingleMotion(owner->model, 0, 0);
            } else {
                mdlLoopMotion(owner->model, 0, 0);
            }
        }
        owner->count = sdfModelGetMarkPosCount(owner->model->inner);
        if (owner->count == 0) {
            return owner;
        }
        resource = fileResolveSecondaryBuffer(job);
        if (resource != NULL) {
            owner->buffer = sdfMemAllocDirect(owner->count * sizeof(*owner->entries));
            owner->entries = (FileQueue **)sdfMemLock(owner->buffer);
            owner->entries[0] = fileCloneQueueEntries(resource);
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
    if (owner->buffer != 0) {
        for (i = 0; i < owner->count; i++) {
            fileQueueDestroy(owner->entries[i]);
        }
        sdfMemFreeDirect(owner->buffer);
    }
    sdfChipFreeDirect(owner);
}


extern void effCopyResourceOwner(EffResourceOwner *, EffResourceOwner *);

void *effDuplicateModelOwner(void *source, u16 unusedType) {
    EffResourceOwner *src = source;
    EffResourceOwner *dst = effCreateResourceOwner(0);

    dst->base = src->base;
    dst->extended = src->extended;
    effCopyResourceOwner(dst, src);
    return dst;
}



void effCopyResourceOwner(EffResourceOwner *dst, EffResourceOwner *src) {
    u32 i;

    if (dst->model != 0) {
        effDestroyModelContext(dst->model);
    }
    dst->model = mdlLoad(mdlGetMajorNum(src->model), mdlGetMinorNum(src->model));
    effInitModelVUState(dst->model);
    if (dst->model->first != NULL) {
        if (dst->base.plainEntry != 0) {
            mdlSingleMotion(dst->model, 0, 0);
        } else {
            mdlLoopMotion(dst->model, 0, 0);
        }
    }
    dst->count = sdfModelGetMarkPosCount(dst->model->inner);
    if (src->buffer != 0) {
        if (dst->buffer != 0) {
            for (i = 0; i < dst->count; i++) {
                fileQueueDestroy(dst->entries[i]);
            }
            sdfMemFreeDirect(dst->buffer);
        }
        dst->buffer = sdfMemAllocDirect(dst->count * 4);
        dst->entries = (FileQueue **)sdfMemLock(dst->buffer);
        for (i = 0; i < dst->count; i++) {
            dst->entries[i] = fileQueueClone(*src->entries);
        }
    }
}


/* Complete queued jobs, sample the first model motion at zero, and clear the owner word. */
void effResetModelMotionAndOwnerFlag(EffResourceOwner *owner) {
    if (owner->buffer != NULL) {
        u32 count = owner->count;
        u32 i = 0;
        FileQueue **entries = owner->entries;
        if (count != 0) {
            do {
                fileQueueNotifyAllJobsComplete(*entries);
                entries++;
                i++;
            } while (i < count);
        }
    }
    sdfMotionSeek(owner->model->first, 0.0f);
    owner->currentFrame = 0;
}

#include "eff_curve.h"
#include "sdf_projection.h"
#include "sdf_vu_transform_api.h"
#include "sdf_vu_rotation_axes.h"
#include "sdf_vu_matrix.h"
extern void effGetWorldVector(u32);
extern s32 effResolveRequestPositionIntoVu(EffectVectorRequest *, s16);
extern s32 sdfModelGetMarkPosMatrix(sdfModelRootNode_t *, s32);
extern void sdfVuMatrixToQuaternion(f32 [4][4]);
extern void Normalize(void);
extern void btlDebugDrawSegment(f32 *, f32 *, u32);
extern void btlDebugDrawPoint(f32 *);
extern f32 D_003E9660[4];
extern f32 D_003E9130[4];
extern f32 effWorldUpVector[4];
extern f32 D_003E9120[4];

/* Resolve attachment endpoints, advance the model, and update retained effect queues. */
void func_002DD448(EffResourceOwner *owner) {
    f32 matrix[4][4];
    f32 position[4];
    f32 endpoint[4];
    f32 rotation[4];
    f32 scale;
    f32 distanceScale = 1.0f;
    f32 frameStep = owner->base.frameStep;
    f32 distance;
    s32 applyDistanceScale = 1;
    s32 haveTarget;
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
    if (btlIsRuntimeAllocated() != 0) {
        haveTarget = 0;
        if (owner->extended.enableExplicitEndpoints != 0) {
            if (effResolveRequestPositionIntoVu(&owner->extended.start, owner->extended.startIndex) != 0) {
                mdlSetPosition(owner->model);
            }
            if (effResolveRequestPositionIntoVu(&owner->extended.end, owner->extended.endIndex) == 0) {
                SDF_VU0_LOADIDENTITYVECTOR();
            }
            SDF_VU0_STOREVECTOR_NOMEM(position);
            if (owner->extended.end.kind == 5) {
                position[1] -= 400.0f;
            }
            haveTarget = 1;
            if (owner->extended.preserveBaseScale == 1) {
                applyDistanceScale = 0;
            }
        } else if ((owner->base.alphaTrack.surfaceIndex & 0xFF0000) != 0) {
            effGetWorldVector(owner->base.alphaTrack.surfaceIndex >> 16);
            SDF_VU0_STOREVECTOR_NOMEM(position);
            targetKind = owner->base.alphaTrack.surfaceIndex >> 16;
            if (targetKind == 5) {
                position[1] -= 400.0f;
            }
            haveTarget = 1;
        }
        if (haveTarget != 0) {
            mdlGetPosition(owner->model);
            SDF_VU0_STOREVECTOR_NOMEM(endpoint);
            LookAtMatrix(position, endpoint, D_003E9660);
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
    }
    if (!(effModelUpdateControlFlags & EFF_MODEL_UPDATE_PAUSE_EFFECT_FRAME_ADVANCE)) {
        owner->model->first->frameStep = frameStep;
        mdlLink(owner->model, kwlnSceneSurfaceSets[10]);
    }
    frame = owner->currentFrame;
    count = owner->count;
    limit = owner->base.plainEntry;
    model = owner->model->inner;
    queue = owner->entries;
    if (frame < limit || limit == 0) {
        if (owner->buffer != NULL) {
            sampledColor = effSampleColorAlphaTracks(&owner->base.colorTrack,
                &owner->base.alphaTrack, frame, limit);
            broadcastColor = mdlGetColor(owner->model);
            byteUnit = 0x3C000000;
            eeUnpackRGBAWord(broadcastColor, byteUnit);
            SDF_VU0_COPYVECTOR_A_TO_B();
            eeUnpackRGBAWord(sampledColor, byteUnit);
            SDF_VU0_MULVECTOR();
            color = eePackRGBAWord(128.0f);
            for (index = 0; index < count; index++, queue++) {
                if (!(effModelUpdateControlFlags & EFF_MODEL_UPDATE_PAUSE_EFFECT_FRAME_ADVANCE)) {
                    sdfModelGetMarkPosMatrix(model, index);
                    VU0_STORE_VF_UNCLOBBERED(vf31, position);
                    if (scale != 0.0f) {
                        SDF_VU0_SETSCALE1VECTOR();
                        VU0_SCALE_VF(vf10, 1.0f / (scale * distanceScale));
                        SDF_VU0_SCALE_XYZ();
                    }
                    sdfVuLoadSecondaryRotationMatrixY(3.14159265f);
                    sdfComposeVuMatrixFromRegisters();
                    VU0_STORE_MATRIX(matrix);
                    fileQueueSetPosition(*queue, position);
                    sdfVuMatrixToQuaternion(matrix);
                    Normalize();
                    SDF_VU0_STOREVECTOR_NOMEM(rotation);
                    fileQueueSetRotation(*queue, rotation);
                    if (applyDistanceScale != 0) {
                        fileQueueSetScale(*queue, scale * distanceScale);
                    } else {
                        fileQueueSetScale(*queue, scale);
                    }
                    fileQueueSetColor(*queue, color);
                }
                fileQueueUpdate(*queue);
            }
        } else {
            for (index = 0; index < count; index++) {
                sdfModelGetMarkPosMatrix(model, index);
                VU0_STORE_VF_UNCLOBBERED(vf31, position);
                VU0_STORE_MATRIX(matrix);
                SDF_VU0_LOADVECTOR(D_003E9130);
                VU0_SCALE_VF(vf10, 5000.0f);
                SDF_VU0_APPLYMATRIX_XYZ1();
                SDF_VU0_STOREVECTOR_NOMEM(endpoint);
                btlDebugDrawSegment(position, endpoint, 0x80006000);
                VU0_LOAD_MATRIX_MEMORY(matrix);
                SDF_VU0_LOADVECTOR(effWorldUpVector);
                VU0_SCALE_VF(vf10, 5000.0f);
                SDF_VU0_APPLYMATRIX_XYZ1();
                SDF_VU0_STOREVECTOR_NOMEM(endpoint);
                btlDebugDrawSegment(position, endpoint, 0x80600000);
                VU0_LOAD_MATRIX_MEMORY(matrix);
                SDF_VU0_LOADVECTOR(D_003E9120);
                VU0_SCALE_VF(vf10, 5000.0f);
                SDF_VU0_APPLYMATRIX_XYZ1();
                SDF_VU0_STOREVECTOR_NOMEM(endpoint);
                btlDebugDrawSegment(position, endpoint, 0x80000060);
                btlDebugDrawPoint(position);
            }
        }
    }
    if (!(effModelUpdateControlFlags & EFF_MODEL_UPDATE_PAUSE_EFFECT_FRAME_ADVANCE)) {
        owner->currentFrame++;
    }
}



void effLoadModelPrimaryVector(EffResourceOwner *owner, void *vec) {
SDF_VU0_LOADVECTOR_BARRIER(vec);
    mdlSetPosition(owner->model);
}

void effSetModelRotationQuaternion(EffResourceOwner *owner, void *vec) {
SDF_VU0_LOADVECTOR_BARRIER(vec);
    mdlSetRotate(owner->model);
}

void effPropagateResourceModelMask(EffResourceOwner *owner, u32 color) {
    mdlSetColor(owner->model, color);
}

void effScaleModelVec(EffResourceOwner *owner, float scale) {
    u32 bits;
    SDF_VU0_SETSCALE1VECTOR();
    VU0_SCALAR_OP_TMP_MEMORY(bits, scale, "vmulx.xyzw vf10, vf10, vf2x");
    mdlSetScale(owner->model);
}

void effObjectListCountersReset(void) {
    D_00437E3C = 0;
    D_00437E38 = 0xffffffff;
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
        D_00437E40 = sdfTexAlloc(0x100, 0x100, 0x13, 0, 0, 1);
        sdfTexMakeGIFPacket(D_00437E40);
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
    if (--effSharedTextureReferenceCount == 0) {
        sdfTexHandle_t *texture = D_00437E40;

        texture->width = 0x100;
        texture->height = 0x100;
        D_00437E38 = 0xffffffff;
        sdfTexReleaseReference((struct sdfTexHandle_t *)texture);
    }
    if (--obj->refCount == 0) {
        sdfMemFreeDirect(obj->allocationHandle);
    }
}

RefObj *effRetainSharedReference(RefObj *obj) {
    obj->refCount++;
    effSharedTextureReferenceCount++;
    return obj;
}

extern sdfTexHandle_t *sdfGetOrCreateTextureForReference(SdfPoolNode *, RefObj *);
#include "sdf_packet_builders.h"

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

    if (source->index != -1 && D_00437E38 == source->index &&
        D_00437E3C == surface) {
        return D_00437E40;
    }
    D_00437E38 = source->index;
    D_00437E3C = surface;
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    sdfLinkLoadImage(list, D_00437E40->secondaryResource, 0, 0,
        source->paletteWidth, source->paletteHeight, (s32)source->palette, 0);
    sdfLinkLoadImage(list, D_00437E40->primaryResource, 0, 0,
        ((SdfTextureFileHeader *)source->base)->width,
        ((SdfTextureFileHeader *)source->base)->height, (s32)source->pixels, 0);
    surface->append(surface, list);
    texture = D_00437E40;
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
    return D_00437E40;
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

    if (--holder->refCount == 0) {
        for (i = 0; i < holder->count; i++) {
            effReleaseSharedReference(holder->handles[i]);
        }
        sdfMemFreeDirect(holder->allocation);
    }
}

EffExpandedList *effReferenceObjectRetain(EffExpandedList *obj) {
    obj->refCount++;
    return obj;
}

/* One animation track: segments contribute their length plus a boundary frame;
 * the loop flag wraps the frame by the total track length. */
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

sdfTexHandle_t *effAssignSampledSegmentReference(EffExpandedList *set, SdfPoolNode *target,
                                                 const EffAnimSample *sample) {
    return sdfGetOrCreateTextureForReference(target, set->handles[sample->segment]);
}

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E2C);

INCLUDE_SDATA(const s32, "game/code_002DC138", effFloorModelListHead);

INCLUDE_SDATA(const s32, "game/code_002DC138", effSharedTextureReferenceCount);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E38);

INCLUDE_SDATA(SdfPoolNode * const, "game/code_002DC138", D_00437E3C);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E40);
