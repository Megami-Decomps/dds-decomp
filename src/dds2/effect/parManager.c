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
#include "sdf_heap_api.h"
#include "sdf_gs_packet.h"
#include "pcp_vu0.h"
#include "sdf_vu_transform_api.h"
#include "ee_mmi.h"
#include "sdf_projection.h"
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
#include "eff_packet_params.h"
#include "fpu.h"
#include "sdf_packet_alloc.h"
#include "par_draw.h"
#include "par_object.h"
#include "par_history.h"
#include "eff_matrix4.h"
#include "par_dispatch_api.h"
#include "par_release_record.h"
#include "par_vertex_colors.h"
#include "sdf_packet_append.h"
#include "par_kind_resource.h"
#include "par_draw_block.h"
#include "par_emitter_desc.h"
#include "par_object_client_view.h"

extern s32 effEmitterDelayRandomState[];

#define EFF_TEMPLATE_TAIL_OFFSET 0x150

#define EFF_PACKET_INITIAL_TAG 0xF0000001

/* Header shared by the effect emitters that spawn a ring or spray of packets:
 * an origin, a sub-effect, a fade descriptor, jitter ranges and the packet
 * buffer. The kind-specific parameters follow at +0x150. */
extern void dds3Particle_BasicDelete(EffEmitterHead *owner);

extern u32 dds3ParticleColorBlend(u32 color, u32 mask);

void dds3Particle_BasicCreate(EffEmitterHead *effect);

void dds3Particle_SpiralAllBuffInit(EffTemplatePacketList *effect);

void dds3Particle_SphereRingAllBuffInit(EffTemplatePacketList *effect);

extern f32 aViewTarget[4];

extern f32 sdfViewTargetVector[4];

/* Narrow mode for the child kinds, or update each resolved animation entry. */ void dds3BillboardSetMode(billHandle_t *effect, s32 mode);

void dds3BillboardSetScale(billHandle_t *effect, float x, float y);

/* Create a billboard sharing the indexed entry's resource. Word two of the * resource stores the reference count; the billHandle_t payload is not an emitter. */ billHandle_t *effCreateBillboardSharingIndexedResource(s32 index);

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
                       (u32)owner->sub.payload.crossStrip.dispatchParameter,
                       (u32)owner->sub.payload.crossStrip.dispatchParameter);
        dds3TrackPolySetMode(owner->sub.payload.crossStrip.drawSystem, (u16)owner->drawKind);
        break;
    case PAR_KIND_STATE_BILLBOARD_CROSS_TRIANGLE:
        owner->sub.payload.crossTriangle.drawSystem = dds3TrackPolyCreate(
            owner->particleCount, owner->sub.templateEntryCount,
            dds3Particle_TrackPolyGetRecTiming(owner->sub.templateEntryCount), 1);
        dds3TrackPolyVCSet(owner->sub.payload.crossTriangle.drawSystem, 1,
                       (u32)owner->sub.payload.crossTriangle.dispatchParameter,
                       *(u32 *)&owner->sub.payload.crossTriangle.opaque10[0]);
        dds3TrackPolySetMode(owner->sub.payload.crossTriangle.drawSystem, (u16)owner->drawKind);
        break;
    case PAR_KIND_STATE_TRACK_POLYGON_CROSS_AXES:
        /* These tracks receive endpoints directly; model sampling is unused. */
        params.sampleInterval = 1;
        params.historyLength = owner->sub.templateEntryCount;
        params.unk1C = 10;
        params.drawKind = (u16)owner->drawKind;
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
    packet->f30 = effect->initialWaveAmplitude * (dds3GetRandFloatA(effDefaultRandomState) * effect->jitterB + (1.0f - effect->jitterB));
    packet->f34 = 0;
    packet->f38 = 0;
    packet->f3C = effect->initialVerticalSpeed * (dds3GetRandFloatA(effDefaultRandomState) * effect->jitterA + (1.0f - effect->jitterA));
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
    phaseStep = effect->phaseStepDegreesPerFrame * (3.14159265f / 180.0f);
    verticalScale = effect->verticalSpeedScalePercent / 100.0f + 1.0f;
    lifetimeFrames = effect->head.lifetimeFrames;
    packetCount = effect->head.particleCount;
    repeatEnabled = effect->flag150;
    waveRate = effect->waveAmplitudeStep;
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
    switch (effect->launchMode) {
    case EFF_PARTICLE_LAUNCH_MODE_RING:
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
    case EFF_PARTICLE_LAUNCH_MODE_ORIGIN:
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
    case EFF_PARTICLE_LAUNCH_MODE_REVERSE_TRAJECTORY:
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
 * Only ring launches transform the displacement through the emitter matrix. */
void dds3Particle_SparkDraw(EffBallisticEmitter *effect) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 subeffectKind = effect->head.sub.kind;
    EffParticleLaunchMode launchMode;
    f32 positionDelta[4];
    f32 velocityScale;
    s32 completedCount;
    s32 lifetimeFrames;
    s32 packetCount;
    s32 repeatEnabled;
    s32 packetIndex;

    dds3Particle_EffectInit(&effect->head.sub);
    launchMode = effect->launchMode;
    if (launchMode == EFF_PARTICLE_LAUNCH_MODE_RING) {
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
            if (launchMode == EFF_PARTICLE_LAUNCH_MODE_RING) {
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

extern f32 aViewDown[4];

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
EffEmitterHead *effCloneSingleParticleTemplate(EffTemplatePacketList *source) {
    EffEmitterHead *clone = sdfChipAllocDirect(0x150);
    s32 tailBytes = 0;

    memset(clone, 0, 0x150);
    memcpy(clone, source, source->head.templateSize);
    memcpy((u8 *)clone + EFF_TEMPLATE_TAIL_OFFSET, (u8 *)source + source->head.templateSize, tailBytes);
    clone->particleCount = 1;
    clone->lifetimeFrames = 0;
    dds3Particle_BasicCreate(clone);
    return clone;
}

void effFreeSingleParticleTemplate(EffEmitterHead *effect) {
    dds3Particle_BasicDelete(effect);
    sdfChipFreeDirect(effect);
}

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
    EffParticleLaunchMode launchMode; /* 0x150 */
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
    switch (effect->launchMode) {
    case EFF_PARTICLE_LAUNCH_MODE_RING:
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
    case EFF_PARTICLE_LAUNCH_MODE_ORIGIN:
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
    case EFF_PARTICLE_LAUNCH_MODE_REVERSE_TRAJECTORY:
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
 * Only ring launches transform the per-frame displacement through the matrix. */
void dds3Particle_SparkOffsetDraw(EffOffsetGravityEmitter *effect) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 subeffectKind = effect->head.sub.kind;
    EffParticleLaunchMode launchMode;
    f32 positionDelta[4];
    f32 origin[4];
    f32 velocityScale;
    s32 completedCount;
    s32 lifetimeFrames;
    s32 packetCount;
    s32 repeatEnabled;
    s32 packetIndex;

    dds3Particle_EffectInit(&effect->head.sub);
    launchMode = effect->launchMode;
    if (launchMode == EFF_PARTICLE_LAUNCH_MODE_RING) {
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
            if (launchMode == EFF_PARTICLE_LAUNCH_MODE_RING) {
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
    clone->auxiliaryVectors = sdfMemLock(auxAllocationHandle);
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

/* Declared before first use (were implicit int). */
extern void dds3Particle_BasicDelete(EffEmitterHead *owner);

struct ParSystem;

struct effMTPolyListHandle_t;

enum {
    PAR_BURST_RANDOM_AXIS = 1
};

typedef struct {
    ParObj head;
    u8 axisMode;                 /* 0x150 */
    u8 loop;                     /* 0x151: rearm expired particles */
    u8 pad152[2];
    u32 spawnDelayFrames;        /* 0x154 */
    f32 initialRadius;           /* 0x158 */
    f32 rotationStepDegrees;     /* 0x15C: native updater converts to radians */
    f32 targetRadius;
    f32 initialRadiusJitter;
    f32 targetRadiusJitter;
    ParHistoryTable history;
    sdfMemHandle_t *childHandle;     /* 0x174: additional owned allocation */
    f32 previousOrigin[3];       /* 0x178: native updater caches origin here */
} ParBurstEmitter; /* 0x184 */

/* Restart callbacks: each takes the object, which retail leaves in a0. */
extern void (*D_003AAC20[])(ParObj *);

/* Creation/copy, update and release callbacks share one 0xC-byte entry,
 * selected by the emitter's 16-bit dispatch index at +0x140. */

/* Release the radial emitter's extra allocation, shared resources, and block. */
void dds3Particle_SphereSpiral2Delete(ParBurstEmitter *obj) {
    if (obj->childHandle != 0) {
        sdfMemFreeDirect(obj->childHandle);
    }
    dds3Particle_BasicDelete(obj);
    sdfChipFreeDirect(obj);
}

/* The native updater rotates radialOffset and rebuilds position = origin +
   radialOffset; it does not integrate this vector as a velocity. */
typedef struct {
    f32 position[4];
    f32 radialOffset[3];
    u8 pad1C[4];
    s32 age;
    u32 color;
    f32 billboardScale;
    f32 initialPhaseRadians; /* initialized as a full-turn phase; later use is not established */
    f32 rotationAxis[3];
    f32 radiusStep;
} ParBurstPacket;


/* Initialize one 64-byte particle record and its kind-specific state. Negative
   ages delay activation; radial distance changes toward a jittered target over
   lifetimeFrames. RNG calls and the pre-transform length measurement stay in
   their original order. */
void dds3Particle_SphereSpiral2BuffInit(ParBurstEmitter *effect, u32 particleIndex) {
    ParBurstPacket *packet = (ParBurstPacket *)effect->head.buffer->records;
    f32 direction[4];
    f32 initialRadius;
    f32 jitterFactor;
    f32 initialRadiusLength;

    packet += particleIndex;
    packet->color = 0;
    packet->age = -(dds3GetRandA((EffRandState *)effEmitterDelayRandomState) % (effect->spawnDelayFrames + 1));
    initialRadius = effect->initialRadius;
    jitterFactor = effect->initialRadiusJitter;
    direction[0] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
    direction[1] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
    direction[2] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
    SDF_VU0_LOADVECTOR(direction);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(direction);
    /* Each component gets a separate jitter sample, not one shared radius draw. */
    packet->radialOffset[0] = initialRadius * (dds3GetRandFloatA(effDefaultRandomState) * jitterFactor + (1.0f - jitterFactor)) * direction[0];
    packet->radialOffset[1] = initialRadius * (dds3GetRandFloatA(effDefaultRandomState) * jitterFactor + (1.0f - jitterFactor)) * direction[1];
    packet->radialOffset[2] = initialRadius * (dds3GetRandFloatA(effDefaultRandomState) * jitterFactor + (1.0f - jitterFactor)) * direction[2];
    SDF_VU0_LOADVECTOR(packet->radialOffset);
    SDF_VU0_GETLENGTH(initialRadiusLength);
    VU0_LOAD_MATRIX(effect->head.matrix);
    SDF_VU0_LOADVECTOR(packet->radialOffset);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_STOREVECTOR_NOMEM(packet->radialOffset);
    packet->position[0] = packet->radialOffset[0] + effect->head.origin[0];
    packet->position[1] = packet->radialOffset[1] + effect->head.origin[1];
    packet->position[2] = packet->radialOffset[2] + effect->head.origin[2];
    if (effect->axisMode == PAR_BURST_RANDOM_AXIS) {
        direction[0] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
        direction[1] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
        direction[2] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
        SDF_VU0_LOADVECTOR(direction);
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR(direction);
        packet->rotationAxis[0] = direction[0];
        packet->rotationAxis[1] = direction[1];
        packet->rotationAxis[2] = direction[2];
    } else {
        packet->rotationAxis[0] = 0;
        packet->rotationAxis[1] = -1.0f;
        packet->rotationAxis[2] = 0;
    }
    jitterFactor = effect->targetRadiusJitter;
    packet->radiusStep = (effect->targetRadius * (dds3GetRandFloatA(effDefaultRandomState) * jitterFactor +
                                    (1.0f - jitterFactor)) - initialRadiusLength) /
                   effect->head.lifetimeFrames;
    jitterFactor = effect->head.billboardScaleJitter;
    packet->billboardScale = effect->head.billboardScale * (dds3GetRandFloatA(effDefaultRandomState) * jitterFactor +
                                         (1.0f - jitterFactor));
    jitterFactor = effect->head.initialPhaseJitter;
    if (jitterFactor != 0) {
        packet->initialPhaseRadians = (dds3GetRandFloatA(effDefaultRandomState) * jitterFactor + (1.0f - jitterFactor)) *
                       (3.14159265f * 2.0f);
    } else {
        packet->initialPhaseRadians = 0;
    }
    parDispatchKindInit(&effect->head.kindState, particleIndex);
}

/* Age tag of a packet waiting for dds3Particle_SphereSpiral2BuffInit. */

#define PAR_PACKET_INITIAL_TAG 0xF0000001

extern u32 dds3ParticleColorBlend(u32 colorA, u32 colorB);

extern void dds3Particle_BlurSetExp(dds3Particle_BlurBuff_t *table, s32 slotIndex, const f32 *origin, u32 color,
                                       ParHistoryTable *source, f32 scale);

extern void dds3Particle_TrackSetExp(struct ParSystem *system, s32 index, const f32 *origin, u32 color,
                                       ParHistoryTable *source);

extern void dds3Particle_Track2SetExp(struct ParSystem *system, s32 index, const f32 *origin, u32 color,
                                          ParHistoryTable *source);

/* Grow each radial offset toward its target radius, spin it about the particle axis,
 * rebuild position from the origin, and feed the kind's point history. */
void dds3Particle_SphereSpiral2Draw(ParBurstEmitter *effect) {
    ParBurstPacket *packet = (ParBurstPacket *)effect->head.buffer->records;
    u32 kind;
    ParHistory *history = effect->history.records;
    f32 origin[4];
    f32 previousOrigin[4];
    f32 rotationAxis[4];
    f32 scaleVector[4];
    f32 radius;
    f32 rotationStep;
    s32 completedCount;
    s32 lifetimeFrames;
    u32 capacity;
    s32 repeatEnabled;
    s32 packetCount;
    s32 packetIndex;

    capacity = effect->history.capacity;
    kind = effect->head.kindState.kind;
    dds3Particle_EffectInit(&effect->head.kindState);
    SDF_VU0_LOADMATRIX_B(effect->head.matrix);
    packetCount = effect->head.particleCount;
    lifetimeFrames = effect->head.lifetimeFrames;
    completedCount = 0;
    rotationStep = effect->rotationStepDegrees * (3.14159265f / 180.0f);
    repeatEnabled = effect->loop;
    SDF_VU0_COPYVECTOR(origin, effect->head.origin);
    previousOrigin[0] = effect->previousOrigin[0];
    previousOrigin[1] = effect->previousOrigin[1];
    previousOrigin[2] = effect->previousOrigin[2];
    previousOrigin[3] = 0;
    for (packetIndex = 0; packetIndex < packetCount; packetIndex++, packet++, history++) {
        s32 age = packet->age;

        if (age == PAR_PACKET_INITIAL_TAG) {
            dds3Particle_SphereSpiral2BuffInit(effect, packetIndex);
            age = packet->age;
        }
        if (age >= 0) {
            packet->color = dds3ParticleColorAnim(&effect->head.colorRamp, packet->color, age);
            packet->color = dds3ParticleColorBlend(packet->color, effect->head.parameterWord);
            if (kind != 0) {
                SDF_VU0_LOADVECTOR_C(packet->radialOffset);
            }
            SDF_VU0_LOADVECTOR(packet->radialOffset);
            SDF_VU0_GETLENGTH(radius);
            radius += packet->radiusStep;
            scaleVector[0] = radius;
            scaleVector[1] = radius;
            scaleVector[2] = radius;
            SDF_VU0_NORMALIZE();
            SDF_VU0_LOADVECTOR_B(scaleVector);
            SDF_VU0_MULVECTOR();
            SDF_VU0_STOREVECTOR(packet->radialOffset);
            VU0_MOVE_VF(vf28, vf24);
            VU0_MOVE_VF(vf29, vf25);
            VU0_MOVE_VF(vf30, vf26);
            VU0_MOVE_VF(vf31, vf27);
            rotationAxis[0] = packet->rotationAxis[0];
            rotationAxis[1] = packet->rotationAxis[1];
            rotationAxis[2] = packet->rotationAxis[2];
            rotationAxis[3] = 0;
            SDF_VU0_LOADVECTOR(rotationAxis);
            SDF_VU0_APPLYMATRIX_XYZ();
            SDF_VU0_STOREVECTOR_NOMEM(rotationAxis);
            SetRotateMatrixAxis(rotationStep, (const RwV3d *)rotationAxis);
            SDF_VU0_LOADVECTOR(packet->radialOffset);
            SDF_VU0_APPLYMATRIX_XYZ();
            SDF_VU0_LOADVECTOR_B(origin);
            SDF_VU0_STOREVECTOR(packet->radialOffset);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(packet->position);
            if (kind != 0) {
                u32 color;

                history->points[history->nextIndex][0] = packet->radialOffset[0];
                history->points[history->nextIndex][1] = packet->radialOffset[1];
                history->points[history->nextIndex][2] = packet->radialOffset[2];
                color = (packet->color & 0xFF000000) | (effect->head.parameterWord & 0xFFFFFF);
                history->nextIndex++;
                if ((u32)history->nextIndex >= capacity) {
                    history->nextIndex = 0;
                }
                if ((u32)history->count < capacity) {
                    history->count++;
                }
                switch (effect->head.kindState.kind) {
                case 1:
                    dds3Particle_BlurSetExp(effect->head.kindState.payload.history.table, packetIndex,
                                               previousOrigin, packet->color, &effect->history,
                                               packet->billboardScale);
                    break;
                case 2:
                    dds3Particle_TrackSetExp(effect->head.kindState.payload.crossStrip.drawSystem,
                                               packetIndex, previousOrigin, color, &effect->history);
                    break;
                case 3:
                    dds3Particle_Track2SetExp(effect->head.kindState.payload.crossTriangle.drawSystem,
                                                  packetIndex, previousOrigin, color, &effect->history);
                    break;
                }
            }
        }
        age++;
        if (age >= lifetimeFrames) {
            if (repeatEnabled) {
                age = PAR_PACKET_INITIAL_TAG;
            } else {
                parDispatchKindInit(&effect->head.kindState, packetIndex);
                completedCount++;
                if (completedCount >= packetCount) {
                    effect->head.restartFlag = 0;
                }
            }
            if (kind != 0) {
                history->nextIndex = 0;
                history->count = 0;
            }
        }
        packet->age = age;
    }
    effect->previousOrigin[0] = origin[0];
    effect->previousOrigin[1] = origin[1];
    effect->previousOrigin[2] = origin[2];
}

void dds3ParticleSetColorTable(ParColorRamp *p, s32 frames) {
    u32 alpha;
    f32 span;
    u32 a, b;

    p->color[0] &= 0xFFFFFF;
    p->color[1] &= 0xFFFFFF;
    p->color[2] &= 0xFFFFFF;
    alpha = p->alpha << 24;
    p->fadeIn = alpha;
    if (p->fadeInFrames > 0) {
        p->fadeIn = alpha / p->fadeInFrames;
    }
    p->fadeOut = alpha;
    if (p->fadeOutFrames > 0) {
        p->fadeOut = alpha / p->fadeOutFrames;
    }
    p->frames = frames;
    if (p->mode == PAR_COLOR_RAMP_MODE_CONSTANT) {
        return;
    }
    if (p->mode == PAR_COLOR_RAMP_MODE_SINGLE_SEGMENT) {
        p->rampFrames = frames;
    } else {
        p->rampFrames = frames >> 1;
    }
    span = p->rampFrames;
    if (span == 0) {
        span = 1.0f;
    }
    a = p->color[1];
    b = p->color[0];
    p->rStep[0] = ((f32)(a & 0xFF) - (f32)(b & 0xFF)) / span;
    p->gStep[0] = ((f32)((a >> 8) & 0xFF) - (f32)((b >> 8) & 0xFF)) / span;
    p->bStep[0] = ((f32)((a >> 16) & 0xFF) - (f32)((b >> 16) & 0xFF)) / span;
    if (p->mode == PAR_COLOR_RAMP_MODE_SINGLE_SEGMENT) {
        return;
    }
    a = p->color[2];
    b = p->color[1];
    p->rStep[1] = ((f32)(a & 0xFF) - (f32)(b & 0xFF)) / span;
    p->gStep[1] = ((f32)((a >> 8) & 0xFF) - (f32)((b >> 8) & 0xFF)) / span;
    p->bStep[1] = ((f32)((a >> 16) & 0xFF) - (f32)((b >> 16) & 0xFF)) / span;
}

u32 dds3ParticleColorAnim(ParColorRamp *p, u32 color, s32 age) {
    s32 channel[3];
    u32 alpha;
    u32 sum;
    f32 t;

    if (age >= p->frames - p->fadeOutFrames) {
        if (p->frames < age) {
            return 0;
        }
        alpha = p->fadeOut * (p->frames - age);
        alpha &= 0xFF000000;
    } else if (age <= p->fadeInFrames) {
        alpha = p->fadeIn * age;
        alpha &= 0xFF000000;
    } else {
        alpha = p->alpha << 24;
    }
    if (p->mode == PAR_COLOR_RAMP_MODE_CONSTANT) {
        return p->color[0] | alpha;
    }
    t = age;
    if (age < p->rampFrames) {
        s32 red = p->rStep[0] * t;
        s32 green = p->gStep[0] * t;
        s32 blue = p->bStep[0] * t;
        sum = p->color[0];
        channel[0] = red;
        channel[1] = green;
        channel[2] = blue;
        sum += channel[0];
        sum += (u32)channel[1] << 8;
        sum += (u32)channel[2] << 16;
        return (sum & 0xFFFFFF) | alpha;
    } else {
        s32 red, green, blue;
        t -= (f32)p->rampFrames;
        red = p->rStep[1] * t;
        green = p->gStep[1] * t;
        blue = p->bStep[1] * t;
        sum = p->color[1];
        channel[0] = red;
        channel[1] = green;
        channel[2] = blue;
        sum += channel[0];
        sum += (u32)channel[1] << 8;
        sum += (u32)channel[2] << 16;
        return (sum & 0xFFFFFF) | alpha;
    }
}

/* Allocate contiguous point slabs, slot records and the trailing table header. */
dds3Particle_BlurBuff_t *dds3Particle_BlurCreate(u32 slotCount, u16 pointCapacity, billHandle_t **billboardRef) {
    u32 pointBytes = pointCapacity * sizeof(u128);
    u32 slabBytes = pointBytes + sizeof(ParSlot);
    u32 totalBytes = slabBytes * slotCount;
    sdfMemHandle_t *allocation = sdfMemAllocDirect(totalBytes + sizeof(dds3Particle_BlurBuff_t));
    u8 *base = (u8 *)sdfMemLock(allocation);
    dds3Particle_BlurBuff_t *table = (dds3Particle_BlurBuff_t *)(base + totalBytes);
    u128 *points = (u128 *)base;
    ParSlot *slotBase = (ParSlot *)(points + pointCapacity * slotCount);
    ParSlot *slots;
    u32 index = 0;

    table->pointCapacity = pointCapacity;
    table->billboardRef = billboardRef;
    table->allocation = allocation;
    table->slotCount = slotCount;
    table->slots = slotBase;
    if (slotCount != 0) {
        slots = table->slots;
        do {
            slots[index].points = points;
            slots[index].pointCount = 0;
            slots[index].color = 0;
            points += pointCapacity;
            index++;
        } while (index < slotCount);
    }
    return table;
}

/* Release the slot table's allocation handle, not a separate node object. */
void dds3Particle_BlurDelete(dds3Particle_BlurBuff_t *table) {
    sdfMemFreeDirect(table->allocation);
}

void dds3Particle_BlurSet(dds3Particle_BlurBuff_t *table, s32 slotIndex, u32 color, f32 billboardScale) {
    ParSlot *slot = &table->slots[slotIndex];
    s32 last = table->pointCapacity - 1;
    u128 *point = slot->points + last;
    s32 i;

    for (i = 0; i < last; i++) {
        point--;
        SDF_VU0_COPYVECTOR(point + 1, point);
    }
    if (slot->pointCount <= last) {
        slot->pointCount++;
    }
    SDF_VU0_STOREVECTOR_C(point);
    slot->color = color;
    slot->billboardScale = billboardScale;
}

extern void dds3BillboardDraw(billHandle_t *billboard);

void dds3Particle_BlurSetExp(dds3Particle_BlurBuff_t *table, s32 slotIndex, const f32 *origin, u32 color,
                   ParHistoryTable *source, f32 scale) {
    f32 localOrigin[4];
    ParHistory *history;
    ParSlot *slot;
    f32 (*destination)[4];
    s32 pointCount;
    s32 pointCapacity;
    s32 pointIndex;

    SDF_VU0_COPYVECTOR(localOrigin, origin);
    history = &source->records[slotIndex];
    slot = &table->slots[slotIndex];
    pointCount = history->count;
    pointCapacity = source->capacity;
    slot->pointCount = pointCount;
    destination = (f32 (*)[4])slot->points;
    pointIndex = history->nextIndex;
    if (pointCount > 0) {
        f32 (*points)[3] = history->points;
        s32 lastIndex = pointCapacity - 1;
        s32 remaining = pointCount;

        do {
            --remaining;
            pointIndex = pointIndex == 0 ? lastIndex : pointIndex - 1;
            (*destination)[0] = localOrigin[0] + points[pointIndex][0];
            (*destination)[1] = localOrigin[1] + points[pointIndex][1];
            (*destination)[2] = localOrigin[2] + points[pointIndex][2];
            ++destination;
        } while (remaining != 0);
    }
    slot->color = color;
    slot->billboardScale = scale;
}

void dds3Particle_BlurDraw(dds3Particle_BlurBuff_t *table) {
    billHandle_t *billboard;
    ParSlot *slot;
    s32 remainingSlots;
    s32 pointCount;
    s32 remainingPoints;
    u32 color;
    u32 alpha;
    u32 alphaStep;
    u128 *point;

    billboard = *table->billboardRef;
    slot = table->slots;

    if (table->slotCount != 0) {
        remainingSlots = table->slotCount;

        do {
            pointCount = slot->pointCount;
            color = slot->color;
            point = slot->points;
            alpha = color & 0xFF000000;
            alphaStep = alpha / (pointCount + 1);

            dds3BillboardSetScale(billboard, slot->billboardScale, slot->billboardScale);
            color &= 0xFFFFFF;
            if (pointCount != 0) {
                remainingPoints = pointCount;
                do {
                    SDF_VU0_COPYVECTOR(billboard, point);
                    alpha -= alphaStep;
                    --remainingPoints;
                    billboard->childParam = color | (alpha & 0xFF000000);
                    ++point;
                    dds3BillboardDraw(billboard);
                } while (remainingPoints != 0);
            }
            --remainingSlots;
            ++slot;
        } while (remainingSlots != 0);
    }
}

/* Disable this slot's billboard points by clearing its count, not a bit flag. */
void dds3Particle_BlurClearItem(dds3Particle_BlurBuff_t *table, s32 slotIndex) {
    table->slots[slotIndex].pointCount = 0;
}

extern f32 parLook[];

extern f32 parTrack_w[];

/* Rebuild one slot's two-vertex strip cells from its point history: each cell is
   the offset point +/- the cross product of the segment direction with the view axis,
   scaled by the strip width. */
void dds3Particle_TrackSetExp(ParSystem *system, s32 index, const f32 *origin, u32 color, ParHistoryTable *source) {
    ParHistory *history = &source->records[index];
    ParCell *cell = &system->cells[index];
    u128 *vertex = cell->history;
    u32 count = history->count;
    f32 point[4];
    f32 localOrigin[4];

    if (count >= 2) {
        s32 segments;
        s32 segmentIndex;
        s32 lastIndex;
        s32 ring;

        SDF_VU0_COPYVECTOR(localOrigin, origin);
        segments = count - 1;
        cell->vertexCount = segments * 2;
        ring = history->nextIndex;
        lastIndex = source->capacity - 1;
        ring = ring == 0 ? lastIndex : ring - 1;
        point[0] = localOrigin[0] + history->points[ring][0];
        point[1] = localOrigin[1] + history->points[ring][1];
        point[2] = localOrigin[2] + history->points[ring][2];
        point[3] = 0;
        for (segmentIndex = 0; segmentIndex < segments; segmentIndex++) {
            SDF_VU0_LOADVECTOR(point);
            ring = ring == 0 ? lastIndex : ring - 1;
            point[0] = localOrigin[0] + history->points[ring][0];
            point[1] = localOrigin[1] + history->points[ring][1];
            point[2] = localOrigin[2] + history->points[ring][2];
            SDF_VU0_LOADVECTOR_C(point);
            SDF_VU0_COPYVECTOR_C_TO_B();
            SDF_VU0_SUBVECTOR();
            SDF_VU0_LOADVECTOR_B(parLook);
            SDF_VU0_OUTERPRODUCT();
            SDF_VU0_NORMALIZE();
            SDF_VU0_LOADVECTOR_B(parTrack_w);
            SDF_VU0_MULVECTOR();
            VU0_MOVE_VF(vf2, vf10);
            SDF_VU0_COPYVECTOR_C_TO_A();
            VU0_MOVE_VF(vf12, vf2);
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_ADDVECTOR_A_C();
            SDF_VU0_STOREVECTOR_NOMEM(vertex);
            SDF_VU0_COPYVECTOR_C_TO_A();
            SDF_VU0_SUBVECTOR_B_A();
            SDF_VU0_STOREVECTOR_B_NOMEM(vertex + 1);
            vertex += 2;
        }
        dds3TrackPolyVCRefresh_ParType1(system, index);
    }
    dds3TrackPolyColorSet(system, index, color);
}

extern void dds3TrackPolyVCRefresh_ParType2(ParSystem *system, s32 index);

/* Triangle-cell variant of dds3Particle_TrackSetExp: three vertices per history segment. */
void dds3Particle_Track2SetExp(ParSystem *system, s32 index, const f32 *origin, u32 color, ParHistoryTable *source) {
    ParHistory *history = &source->records[index];
    ParCell *cell = &system->cells[index];
    u128 *vertex = cell->history;
    u32 count = history->count;
    f32 point[4];
    f32 localOrigin[4];

    if (count >= 2) {
        s32 segments;
        s32 segmentIndex;
        s32 lastIndex;
        s32 ring;

        SDF_VU0_COPYVECTOR(localOrigin, origin);
        segments = count - 1;
        cell->vertexCount = segments * 3;
        ring = history->nextIndex;
        lastIndex = source->capacity - 1;
        ring = ring == 0 ? lastIndex : ring - 1;
        point[0] = localOrigin[0] + history->points[ring][0];
        point[1] = localOrigin[1] + history->points[ring][1];
        point[2] = localOrigin[2] + history->points[ring][2];
        point[3] = 0;
        for (segmentIndex = 0; segmentIndex < segments; segmentIndex++) {
            SDF_VU0_LOADVECTOR(point);
            ring = ring == 0 ? lastIndex : ring - 1;
            point[0] = localOrigin[0] + history->points[ring][0];
            point[1] = localOrigin[1] + history->points[ring][1];
            point[2] = localOrigin[2] + history->points[ring][2];
            SDF_VU0_LOADVECTOR_C(point);
            SDF_VU0_COPYVECTOR_C_TO_B();
            SDF_VU0_SUBVECTOR();
            SDF_VU0_LOADVECTOR_B(parLook);
            SDF_VU0_OUTERPRODUCT();
            SDF_VU0_NORMALIZE();
            SDF_VU0_LOADVECTOR_B(parTrack_w);
            SDF_VU0_MULVECTOR();
            VU0_MOVE_VF(vf2, vf10);
            SDF_VU0_COPYVECTOR_C_TO_A();
            VU0_MOVE_VF(vf12, vf2);
            SDF_VU0_STOREVECTOR_NOMEM(vertex + 1);
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_ADDVECTOR_A_C();
            SDF_VU0_STOREVECTOR_NOMEM(vertex);
            SDF_VU0_COPYVECTOR_C_TO_A();
            SDF_VU0_SUBVECTOR_B_A();
            SDF_VU0_STOREVECTOR_B_NOMEM(vertex + 2);
            vertex += 3;
        }
        dds3TrackPolyVCRefresh_ParType2(system, index);
    }
    dds3TrackPolyColorSet(system, index, color);
}

/* vu0 routine: modulate two RGBA8888 colours, (a/128 * b/128) * 128 per channel */
u32 dds3ParticleColorBlend(u32 colorA, u32 colorB) {
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

/* Create through the selected dispatch entry and record its index. The object is the
 * result of the effect table's create slot, which dds3EffectCreate keeps as its
 * node's instance. */
ParObj *dds3ParticleCreate(s32 dispatchIndex, void *creationData) {
    ParObj *createdObject;

    createdObject = dds3ParticleFuncTable[dispatchIndex].createOrCopy(creationData);
    createdObject->dispatchIndex = dispatchIndex;
    return createdObject;
}

/* Invoke the selected destructor with the existing empty argument list. */
void dds3ParticleDelete(ParObj *obj) {
    dds3ParticleFuncTable[obj->dispatchIndex].destroy(obj);
}

/* Draw live particles after applying the requested number of emitter updates. */
void dds3ParticleDraw(ParObj *effect) {
    EffectBufferRecord *record;
    billHandle_t *billboard;
    u32 step;
    s32 particleCount;
    s32 lifetime;

    if (effect->restartFlag == 0) {
        return;
    }
    if (effect->pendingRestartSteps == 0) {
        dds3ParticleFuncTable[effect->dispatchIndex].update(effect);
    } else {
        for (step = 0; step < effect->pendingRestartSteps; step++) {
            dds3ParticleFuncTable[effect->dispatchIndex].update(effect);
        }
        effect->pendingRestartSteps = 0;
    }
    billboard = effect->billboard;
    record = effect->buffer->records;
    particleCount = effect->particleCount;
    lifetime = effect->lifetimeFrames;
    dds3BillboardSetScale(billboard, effect->billboardScale, effect->billboardScaleY);
    if (particleCount > 0) {
        step = particleCount;
        do {
            s32 age = record->age;

            if (age < lifetime && age >= 0) {
                SDF_VU0_COPYVECTOR(billboard, record->position);
                billboard->childParam = record->color;
                dds3BillboardDraw(billboard);
            }
            step--;
            record++;
        } while (step != 0);
    }
    parSubmitKindDrawing(&effect->kindState);
}

/* Create another object of the same dispatch index and return the new instance. */
ParObj *dds3ParticleCopy(ParObj *obj) {
    ParObj *createdObject;

    createdObject = dds3ParticleFuncTable[obj->dispatchIndex].createOrCopy(obj);
    createdObject->dispatchIndex = obj->dispatchIndex;
    return createdObject;
}

/* Reissue the callback, reload the native updater's repeat count, and arm restart. */
void dds3ParticleReset(ParObj *obj) {
    D_003AAC20[obj->dispatchIndex](obj);
    obj->pendingRestartSteps = obj->restartStepCount;
    obj->restartFlag = 1;
}

extern void (*D_003AAC58[])(ParObj *, f32);

/* Call the selected entry with factor, additionally scale kinds 2..4, then restart. */
void dds3ParticleSetScale(ParObj *obj, f32 factor) {
    D_003AAC58[obj->dispatchIndex](obj, factor);
    switch (obj->kindState.kind) {
    case PAR_KIND_STATE_BILLBOARD_CROSS_STRIP:
        obj->kindState.payload.crossStrip.scale *= factor;
        break;
    case PAR_KIND_STATE_BILLBOARD_CROSS_TRIANGLE:
        obj->kindState.payload.crossTriangle.scale *= factor;
        break;
    case PAR_KIND_STATE_TRACK_POLYGON_CROSS_AXES:
        obj->kindState.payload.trackPolygon.scale *= factor;
        break;
    }
    dds3ParticleReset(obj);
}

/* Return the full 16-bit restart state without converting it to a boolean. */
u16 dds3ParticleGetState(ParObj *obj) {
    return obj->restartFlag;
}

/* Copy one 16-byte vector with the existing EE/VU copy primitive. */
void dds3ParticleSetPosition(void *destination, void *source) {
    SDF_VU0_COPYVECTOR(destination, source);
}

/* vu0 routine: compose the supplied matrix with the emitter's source matrix. */
void dds3ParticleSetRotateMatrix(ParObj *effect, const Matrix4 *matrix) {
    VU0_LOAD_MATRIX(matrix);
    SDF_VU0_LOADMATRIX_B(effect->sourceMatrix);
    MultMatrixNoArg();
    VU0_STORE_MATRIX(effect->matrix);
}



extern void (*VCSetFunc[])(ParSystem *, u32, u32);

extern u16 dds3ParticleGetState(ParObj *obj);

extern void dds3TrackPolySet2(ParSystem *, s32, const u128 *);

extern u8 dds3ParticleGetLoop(ParObj *object);

extern void dds3Particle_TrackSet(ParSystem *system, s32 index, u32 color);

extern void dds3Particle_Track2Set(ParSystem *system, s32 index, u32 color);

extern void dds3TrackPolyVCRefresh_ParType2(ParSystem *system, s32 cellIndex);


void dds3ParticleSetRGBA(ParObj *work, u32 parameterWord) {
    work->parameterWord = parameterWord;
}

void dds3ParticleSetTypeFlag(ParObj *object, u8 mode) {
    switch (object->dispatchIndex) {
    case 1:
    case 5:
    case 11:
        ((ParObjClientView *)object)->mode150 = mode;
        break;
    case 0:
    case 2:
    case 3:
    case 4:
    case 6:
    case 7:
    case 8:
    case 10:
    case 12:
        ((ParObjClientView *)object)->mode151 = mode;
        break;
    }
    object->restartFlag = 1;
}

u8 dds3ParticleGetLoop(ParObj *object) {
    switch (object->dispatchIndex) {
    case 1:
    case 5:
    case 11:
        return ((ParObjClientView *)object)->mode150;
    case 0:
    case 2:
    case 3:
    case 4:
    case 6:
    case 7:
    case 8:
    case 10:
    case 12:
        return ((ParObjClientView *)object)->mode151;
    default:
        return 0;
    }
}

ParObj *dds3ParticleExpCreate(s32 kind, ParKindResource *resource) {
    ParObj *object = (ParObj *)((u8 *)resource + resource->offset + 0x10);
    billHandle_t *billboard;

    if (resource->type != 3 || resource->offset != 0) {
        object->billboardCloneMarker = PAR_BILLBOARD_CLONE_FROM_RESOURCE;
        object = dds3ParticleFuncTable[kind].createOrCopy(object);
        billboard = dds3BillboardCreate(resource->type, (u32)(resource + 1));
        dds3BillboardSetScale(billboard, object->billboardScale, object->billboardScaleY);
        dds3BillboardSetMode(billboard, object->billboardMode);
        dds3BillboardSetAnimColorAdvMode(billboard);
        object->billboard = billboard;
    } else {
        object = dds3ParticleFuncTable[kind].createOrCopy(object);
    }
    object->dispatchIndex = kind;
    return object;
}

ParObj *dds3ParticleExpCopy(ParObj *source) {
    ParObj *particle;
    billHandle_t *billboard;

    particle = dds3ParticleFuncTable[source->dispatchIndex].createOrCopy(source);
    particle->dispatchIndex = source->dispatchIndex;
    if (source->billboardCloneMarker == PAR_BILLBOARD_CLONE_FROM_RESOURCE) {
        billboard = dds3BillboardCopy(source->billboard);
        dds3BillboardSetScale(billboard, particle->billboardScale, particle->billboardScaleY);
        dds3BillboardSetMode(billboard, particle->billboardMode);
        dds3BillboardSetAnimColorAdvMode(billboard);
        particle->billboard = billboard;
    }
    return particle;
}

void dds3ParticleExpDelete(ParObj *object) {
    dds3ParticleFuncTable[object->dispatchIndex].destroy(object);
}

extern void dds3BillboardDraw(billHandle_t *);

/* Apply queued updates, then draw live records and submit the kind's resource. */
void dds3ParticleExpDraw(ParObj *effect) {
    EffectBufferRecord *record;
    billHandle_t *billboard;
    u32 step;
    s32 particleCount;
    s32 lifetime;
    f32 scaleStep;
    f32 spinStep;
    f32 spinDirection;
    f32 alternate;

    if (effect->restartFlag == 0) {
        return;
    }
    if (effect->pendingRestartSteps == 0) {
        dds3ParticleFuncTable[effect->dispatchIndex].update(effect);
    } else {
        for (step = 0; step < effect->pendingRestartSteps; step++) {
            dds3ParticleFuncTable[effect->dispatchIndex].update(effect);
        }
        particleCount = effect->particleCount;
        scaleStep = (effect->scaleEnd - effect->billboardScale) / effect->lifetimeFrames;
        spinStep = effect->spinDegrees * (3.14159265f / 180.0f);
        alternate = effect->alternateSpin ? -1.0f : 1.0f;
        for (step = 0; step < effect->pendingRestartSteps; step++) {
            s32 recordIndex;

            record = effect->buffer->records;
            spinDirection = 1.0f;
            for (recordIndex = 0; recordIndex < particleCount; recordIndex++, record++) {
                if (record->age >= 0) {
                    record->scale += scaleStep;
                    record->spin += spinStep * spinDirection;
                }
                spinDirection *= alternate;
            }
        }
        effect->pendingRestartSteps = 0;
    }
    /* Capture the draw owners before deriving this frame's scale/spin steps. */
    record = effect->buffer->records;
    billboard = effect->billboard;
    particleCount = effect->particleCount;
    scaleStep = (effect->scaleEnd - effect->billboardScale) / effect->lifetimeFrames;
    lifetime = effect->lifetimeFrames;
    spinStep = effect->spinDegrees * (3.14159265f / 180.0f);
    alternate = effect->alternateSpin ? -1.0f : 1.0f;
    spinDirection = 1.0f;
    if (particleCount > 0) {
        step = particleCount;
        do {
            s32 age = record->age;

            if (age < lifetime && age >= 0) {
                SDF_VU0_COPYVECTOR(billboard, record->position);
                billboard->rotationAngle = record->spin;
                billboard->childParam = record->color;
                billboard->childScaleX = billboard->childScaleY = record->scale;
                record->scale += scaleStep;
                if (record->scale < 0) {
                    record->scale = 0;
                }
                record->spin += spinStep * spinDirection;
                dds3BillboardSetAnimKey(billboard, age);
                dds3BillboardDraw(billboard);
            }
            spinDirection *= alternate;
            --step;
            ++record;
        } while (step != 0);
    }
    parSubmitKindDrawing(&effect->kindState);
}

extern void dds3ParticleReset(ParObj *obj);

extern void dds3ParticleSetScale(ParObj *obj, f32 factor);

void dds3ParticleExpReset(ParObj *obj) {
    dds3ParticleReset(obj);
}

void dds3ParticleExpSetScale(float scale, ParObj *work) {
    dds3ParticleSetScale(work, scale);
    work->scaleEnd = work->scaleEnd * scale;
}

/* Descriptor query: return the complete 16-bit restart state. */
u16 dds3ParticleExpGetState(ParObj *obj) {
    return dds3ParticleGetState(obj);
}

void dds3ParticleExpSetPosition(void *dst, void *src) {
    SDF_VU0_COPYVECTOR(dst, src);
}

extern void dds3ParticleSetRotateMatrix(ParObj *effect, const Matrix4 *matrix);

void dds3ParticleExpSetRotateMatrix(ParObj *effect, const Matrix4 *matrix) {
    dds3ParticleSetRotateMatrix(effect, matrix);
}

void dds3ParticleExpSetRGBA(ParObj *work, u32 parameterWord) {
    work->parameterWord = parameterWord;
}

void parChangeInstanceMode(ParObj *work, u8 mode) {
    dds3ParticleSetTypeFlag(work, mode);
}

/* Descriptor query: select the kind's current mode byte. */
u8 dds3ParticleExpGetLoop(ParObj *work) {
    return dds3ParticleGetLoop(work);
}

/* Kinds 2-4 keep the scale at +8 of their own record; copy it into the
 * shared vector and store the (vf10 - vf11) difference. */
void dds3Particle_EffectInit(ParKindState *obj) {
    f32 scale;

    switch (obj->kind) {
    case 0:
    case PAR_KIND_STATE_HISTORY:
        return;
    case PAR_KIND_STATE_BILLBOARD_CROSS_STRIP:
        scale = obj->payload.crossStrip.scale;
        parTrack_w[0] = parTrack_w[1] = parTrack_w[2] = scale;
        break;
    case PAR_KIND_STATE_BILLBOARD_CROSS_TRIANGLE:
        scale = obj->payload.crossTriangle.scale;
        parTrack_w[0] = parTrack_w[1] = parTrack_w[2] = scale;
        break;
    case PAR_KIND_STATE_TRACK_POLYGON_CROSS_AXES:
        scale = obj->payload.trackPolygon.scale;
        parTrack_w[0] = parTrack_w[1] = parTrack_w[2] = scale;
        break;
    default:
        return;
    }
    VU0_LOAD_VF($vf10, aViewTarget);
    VU0_LOAD_VF($vf11, sdfViewTargetVector);
    SDF_VU0_SUBVECTOR();
    VU0_STORE_VF($vf10, parLook);
}

void parDispatchKindUpdate(ParKindState *work, s32 index, u32 color, f32 speed) {
    switch ((u16)work->kind) {
    case 1:
        dds3Particle_BlurSet(work->payload.history.table, index, color, speed);
        return;
    case 2:
        dds3Particle_TrackSet(work->payload.crossStrip.drawSystem, index, color);
        return;
    case 3:
        dds3Particle_Track2Set(work->payload.crossTriangle.drawSystem, index, color);
        return;
    case 4:
        dds3Particle_Track3Set(work->payload.trackPolygon.modelList, index, color);
        break;
    }
}

void parDispatchKindInit(ParKindState *work, s32 index) {
    switch ((u16)work->kind) {
    case 1:
        dds3Particle_BlurClearItem((s32)work->payload.history.table, index);
        return;
    case 2:
        dds3TrackPolyListInit(work->payload.crossStrip.drawSystem, index);
        return;
    case 3:
        dds3TrackPolyListInit(work->payload.crossTriangle.drawSystem, index);
        return;
    case 4:
        Eff_ModelTrackPolyListReset(work->payload.trackPolygon.modelList, index);
        break;
    }
}

void dds3Particle_TrackSet(ParSystem *system, s32 index, u32 color) {
    u128 axis[2];
    SDF_VU0_COPYVECTOR_C_TO_B();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(parLook);
;
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B(parTrack_w);
;
    SDF_VU0_MULVECTOR();
    VU0_MOVE_VF(vf2, vf10);
    SDF_VU0_COPYVECTOR_C_TO_A();
    VU0_MOVE_VF(vf12, vf2);
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_STOREVECTOR(&axis[0]);
;
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_SUBVECTOR_B_A();
    SDF_VU0_STOREVECTOR_B(&axis[1]);
;
    dds3TrackPolySet(system, index, axis);
    dds3TrackPolyVCRefresh_ParType1(system, index);
    dds3TrackPolyColorSet(system, index, (color & 0xFF000000) | 0x808080);
}

void dds3Particle_Track2Set(ParSystem *system, s32 index, u32 color) {
    u128 axis[3];
    SDF_VU0_COPYVECTOR_C_TO_B();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(parLook);
;
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B(parTrack_w);
;
    SDF_VU0_MULVECTOR();
    VU0_MOVE_VF(vf2, vf10);
    SDF_VU0_COPYVECTOR_C_TO_A();
    VU0_MOVE_VF(vf12, vf2);
    SDF_VU0_STOREVECTOR(&axis[1]);
;
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_STOREVECTOR(&axis[0]);
;
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_SUBVECTOR_B_A();
    SDF_VU0_STOREVECTOR_B(&axis[2]);
;
    dds3TrackPolySet2(system, index, axis);
    dds3TrackPolyVCRefresh_ParType2(system, index);
    dds3TrackPolyColorSet(system, index, (color & 0xFF000000) | 0x808080);
}

void dds3Particle_Track3Set(effMTPolyListHandle_t *list, s32 index, u32 color) {
    u128 axis[2];
    SDF_VU0_COPYVECTOR_C_TO_B();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(parLook);
;
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B(parTrack_w);
;
    SDF_VU0_MULVECTOR();
    VU0_MOVE_VF(vf2, vf10);
    SDF_VU0_COPYVECTOR_C_TO_A();
    VU0_MOVE_VF(vf12, vf2);
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_STOREVECTOR(&axis[0]);
;
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_SUBVECTOR_B_A();
    SDF_VU0_STOREVECTOR_B(&axis[1]);
;
    Eff_ModelTrackPolyListVertexSet(list, index, axis);
    effTrackPolySetIndexedColor(list, index, (color & 0xFF000000) | 0x808080);
}

u32 dds3Particle_TrackPolyGetRecTiming(u16 pointCapacity) {
    return 0;
}
