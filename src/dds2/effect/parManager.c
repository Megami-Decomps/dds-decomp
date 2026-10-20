#include "common.h"
#include "eff_random.h"
#include "sdf_chip.h"
#include "sdf_resource.h"
#include "par_draw.h"
#include "par_kind_api.h"
#include "par_cell_api.h"
#include "bill_object_api.h"
#include "par_color_ramp.h"
#include "par_object.h"
#include "par_history.h"
#include "sdf_quaternion.h"

#include "ee_mmi.h"

#include "pcp_vu0.h"
#include "sdf_vu_matrix.h"

#include "eff.h"
#include "par_table.h"
/* Declared before first use (were implicit int). */
extern void effDestroyResources(EffEmitterHead *owner);

struct ParSystem;
struct EffTrackPolyList;

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
    SdfMemBlock *childHandle;     /* 0x174: additional owned allocation */
    f32 previousOrigin[3];       /* 0x178: native updater caches origin here */
} ParBurstEmitter; /* 0x184 */

/* Restart callbacks: each takes the object, which retail leaves in a0. */
extern void (*D_003AAC20[])(ParObj *);

/* Constructor, update and destructor operations share one 0xC-byte entry,
 * selected by the emitter's 16-bit dispatch index at +0x140. */

extern ParDispatch parKindConstructorEntries[];


/* Release the radial emitter's extra allocation, shared resources, and block. */
void parReleaseObject(ParBurstEmitter *obj) {
    if (obj->childHandle != 0) {
        sdfReleaseResourceAllocation(obj->childHandle);
    }
    effDestroyResources(obj);
    sdfReleaseChipBlock(obj);
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

extern EffRandState effDefaultRandomState[];
extern u8 effEmitterDelayRandomState[];

/* Initialize one 64-byte particle record and its kind-specific state. Negative
   ages delay activation; radial distance changes toward a jittered target over
   lifetimeFrames. RNG calls and the pre-transform length measurement stay in
   their original order. */
void parInitializeRadialParticle(ParBurstEmitter *effect, u32 particleIndex) {
    ParBurstPacket *packet = (ParBurstPacket *)effect->head.buffer->records;
    f32 direction[4];
    f32 initialRadius;
    f32 jitterFactor;
    f32 initialRadiusLength;

    packet += particleIndex;
    packet->color = 0;
    packet->age = -(effMiscRand((EffRandState *)effEmitterDelayRandomState) % (effect->spawnDelayFrames + 1));
    initialRadius = effect->initialRadius;
    jitterFactor = effect->initialRadiusJitter;
    direction[0] = (effMiscRandUnitFloat(effDefaultRandomState) - 0.5f) * 2.0f;
    direction[1] = (effMiscRandUnitFloat(effDefaultRandomState) - 0.5f) * 2.0f;
    direction[2] = (effMiscRandUnitFloat(effDefaultRandomState) - 0.5f) * 2.0f;
    VU0_LOAD_VF(vf10, direction);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, direction);
    /* Each component gets a separate jitter sample, not one shared radius draw. */
    packet->radialOffset[0] = initialRadius * (effMiscRandUnitFloat(effDefaultRandomState) * jitterFactor + (1.0f - jitterFactor)) * direction[0];
    packet->radialOffset[1] = initialRadius * (effMiscRandUnitFloat(effDefaultRandomState) * jitterFactor + (1.0f - jitterFactor)) * direction[1];
    packet->radialOffset[2] = initialRadius * (effMiscRandUnitFloat(effDefaultRandomState) * jitterFactor + (1.0f - jitterFactor)) * direction[2];
    VU0_LOAD_VF(vf10, packet->radialOffset);
    VU0_LENGTH_VF10(initialRadiusLength);
    VU0_LOAD_MATRIX(effect->head.matrix);
    VU0_LOAD_VF(vf10, packet->radialOffset);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF_UNCLOBBERED(vf10, packet->radialOffset);
    packet->position[0] = packet->radialOffset[0] + effect->head.origin[0];
    packet->position[1] = packet->radialOffset[1] + effect->head.origin[1];
    packet->position[2] = packet->radialOffset[2] + effect->head.origin[2];
    if (effect->axisMode == PAR_BURST_RANDOM_AXIS) {
        direction[0] = (effMiscRandUnitFloat(effDefaultRandomState) - 0.5f) * 2.0f;
        direction[1] = (effMiscRandUnitFloat(effDefaultRandomState) - 0.5f) * 2.0f;
        direction[2] = (effMiscRandUnitFloat(effDefaultRandomState) - 0.5f) * 2.0f;
        VU0_LOAD_VF(vf10, direction);
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF(vf10, direction);
        packet->rotationAxis[0] = direction[0];
        packet->rotationAxis[1] = direction[1];
        packet->rotationAxis[2] = direction[2];
    } else {
        packet->rotationAxis[0] = 0;
        packet->rotationAxis[1] = -1.0f;
        packet->rotationAxis[2] = 0;
    }
    jitterFactor = effect->targetRadiusJitter;
    packet->radiusStep = (effect->targetRadius * (effMiscRandUnitFloat(effDefaultRandomState) * jitterFactor +
                                    (1.0f - jitterFactor)) - initialRadiusLength) /
                   effect->head.lifetimeFrames;
    jitterFactor = effect->head.billboardScaleJitter;
    packet->billboardScale = effect->head.billboardScale * (effMiscRandUnitFloat(effDefaultRandomState) * jitterFactor +
                                         (1.0f - jitterFactor));
    jitterFactor = effect->head.initialPhaseJitter;
    if (jitterFactor != 0) {
        packet->initialPhaseRadians = (effMiscRandUnitFloat(effDefaultRandomState) * jitterFactor + (1.0f - jitterFactor)) *
                       (3.14159265f * 2.0f);
    } else {
        packet->initialPhaseRadians = 0;
    }
    parDispatchKindInit(&effect->head.kindState, particleIndex);
}

/* Age tag of a packet waiting for parInitializeRadialParticle. */
#define PAR_PACKET_INITIAL_TAG 0xF0000001

extern u32 effParModulateColors(u32 colorA, u32 colorB);
extern void parPopulateSlotFromHistory(ParTable *table, s32 slotIndex, const f32 *origin, u32 color,
                                       ParHistoryTable *source, f32 scale);
extern void parBuildHistoryRibbonStrip(struct ParSystem *system, s32 index, const f32 *origin, u32 color,
                                       ParHistoryTable *source);
extern void parDrawHistoryRibbonTriangles(struct ParSystem *system, s32 index, const f32 *origin, u32 color,
                                          ParHistoryTable *source);

/* Grow each radial offset toward its target radius, spin it about the particle axis,
 * rebuild position from the origin, and feed the kind's point history. */
void parUpdateBurstEmitter(ParBurstEmitter *effect) {
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
    parUpdateSharedScaleAndDelta(&effect->head.kindState);
    VU0_LOAD_MATRIX_B(effect->head.matrix);
    packetCount = effect->head.particleCount;
    lifetimeFrames = effect->head.lifetimeFrames;
    completedCount = 0;
    rotationStep = effect->rotationStepDegrees * (3.14159265f / 180.0f);
    repeatEnabled = effect->loop;
    PCP_COPY_VECTOR(origin, effect->head.origin);
    previousOrigin[0] = effect->previousOrigin[0];
    previousOrigin[1] = effect->previousOrigin[1];
    previousOrigin[2] = effect->previousOrigin[2];
    previousOrigin[3] = 0;
    for (packetIndex = 0; packetIndex < packetCount; packetIndex++, packet++, history++) {
        s32 age = packet->age;

        if (age == PAR_PACKET_INITIAL_TAG) {
            parInitializeRadialParticle(effect, packetIndex);
            age = packet->age;
        }
        if (age >= 0) {
            packet->color = parEvaluateColorRampAtAge(&effect->head.colorRamp, packet->color, age);
            packet->color = effParModulateColors(packet->color, effect->head.parameterWord);
            if (kind != 0) {
                VU0_LOAD_VF(vf12, packet->radialOffset);
            }
            VU0_LOAD_VF(vf10, packet->radialOffset);
            VU0_LENGTH_VF10(radius);
            radius += packet->radiusStep;
            scaleVector[0] = radius;
            scaleVector[1] = radius;
            scaleVector[2] = radius;
            VU0_NORMALIZE_VF10();
            VU0_LOAD_VF(vf11, scaleVector);
            VU0_MUL(vf10, vf10, vf11);
            VU0_STORE_VF(vf10, packet->radialOffset);
            VU0_MOVE_VF(vf28, vf24);
            VU0_MOVE_VF(vf29, vf25);
            VU0_MOVE_VF(vf30, vf26);
            VU0_MOVE_VF(vf31, vf27);
            rotationAxis[0] = packet->rotationAxis[0];
            rotationAxis[1] = packet->rotationAxis[1];
            rotationAxis[2] = packet->rotationAxis[2];
            rotationAxis[3] = 0;
            VU0_LOAD_VF(vf10, rotationAxis);
            VU0_ROTATE_VEC(vf10, vf10);
            VU0_STORE_VF_UNCLOBBERED(vf10, rotationAxis);
            sdfBuildVuRotationFromAxisAngle(rotationStep, (const RwV3d *)rotationAxis);
            VU0_LOAD_VF(vf10, packet->radialOffset);
            VU0_ROTATE_VEC(vf10, vf10);
            VU0_LOAD_VF(vf11, origin);
            VU0_STORE_VF(vf10, packet->radialOffset);
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF(vf10, packet->position);
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
                    parPopulateSlotFromHistory(effect->head.kindState.payload.history.table, packetIndex,
                                               previousOrigin, packet->color, &effect->history,
                                               packet->billboardScale);
                    break;
                case 2:
                    parBuildHistoryRibbonStrip(effect->head.kindState.payload.crossStrip.drawSystem,
                                               packetIndex, previousOrigin, color, &effect->history);
                    break;
                case 3:
                    parDrawHistoryRibbonTriangles(effect->head.kindState.payload.crossTriangle.drawSystem,
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


void parInitColorRamp(ParColorRamp *p, s32 frames) {
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
    if (p->mode == 1) {
        return;
    }
    if (p->mode == 2) {
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
    if (p->mode == 2) {
        return;
    }
    a = p->color[2];
    b = p->color[1];
    p->rStep[1] = ((f32)(a & 0xFF) - (f32)(b & 0xFF)) / span;
    p->gStep[1] = ((f32)((a >> 8) & 0xFF) - (f32)((b >> 8) & 0xFF)) / span;
    p->bStep[1] = ((f32)((a >> 16) & 0xFF) - (f32)((b >> 16) & 0xFF)) / span;
}

u32 parEvaluateColorRampAtAge(ParColorRamp *p, u32 color, s32 age) {
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
    if (p->mode == 1) {
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
ParTable *parCreateHistorySlotTable(u32 slotCount, u16 pointCapacity, BillObj **billboardRef) {
    u32 pointBytes = pointCapacity * sizeof(u128);
    u32 slabBytes = pointBytes + sizeof(ParSlot);
    u32 totalBytes = slabBytes * slotCount;
    SdfMemBlock *allocation = sdfAllocGeneralBlock(totalBytes + sizeof(ParTable));
    u8 *base = (u8 *)sdfResourceRetainAddress(allocation);
    ParTable *table = (ParTable *)(base + totalBytes);
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
void parReleaseHistorySlotTable(ParTable *table) {
    sdfReleaseResourceAllocation(table->allocation);
}

void parPrependHistorySample(ParTable *table, s32 slotIndex, u32 color, f32 billboardScale) {
    ParSlot *slot = &table->slots[slotIndex];
    s32 last = table->pointCapacity - 1;
    u128 *point = slot->points + last;
    s32 i;

    for (i = 0; i < last; i++) {
        point--;
        PCP_COPY_VECTOR(point + 1, point);
    }
    if (slot->pointCount <= last) {
        slot->pointCount++;
    }
    VU0_STORE_VF(vf12, point);
    slot->color = color;
    slot->billboardScale = billboardScale;
}

extern void billInvokeCallback(BillObj *billboard);

void parPopulateSlotFromHistory(ParTable *table, s32 slotIndex, const f32 *origin, u32 color,
                   ParHistoryTable *source, f32 scale) {
    f32 localOrigin[4];
    ParHistory *history;
    ParSlot *slot;
    f32 (*destination)[4];
    s32 pointCount;
    s32 pointCapacity;
    s32 pointIndex;

    PCP_COPY_VECTOR(localOrigin, origin);
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

void parDrawHistorySlots(ParTable *table) {
    BillObj *billboard;
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

            billSetChildScaleComponents(billboard, slot->billboardScale, slot->billboardScale);
            color &= 0xFFFFFF;
            if (pointCount != 0) {
                remainingPoints = pointCount;
                do {
                    PCP_COPY_VECTOR(billboard, point);
                    alpha -= alphaStep;
                    --remainingPoints;
                    billboard->childParam = color | (alpha & 0xFF000000);
                    ++point;
                    billInvokeCallback(billboard);
                } while (remainingPoints != 0);
            }
            --remainingSlots;
            ++slot;
        } while (remainingSlots != 0);
    }
}

/* Disable this slot's billboard points by clearing its count, not a bit flag. */
void parClearHistorySlotPoints(ParTable *table, s32 slotIndex) {
    table->slots[slotIndex].pointCount = 0;
}

extern f32 parSharedViewDelta[];
extern f32 parSharedCrossScale[];
extern void effBillSetEntryValue(ParSystem *system, s32 index, u32 color);

/* Rebuild one slot's two-vertex strip cells from its point history: each cell is
   the offset point +/- the cross product of the segment direction with the view axis,
   scaled by the strip width. */
void parBuildHistoryRibbonStrip(ParSystem *system, s32 index, const f32 *origin, u32 color, ParHistoryTable *source) {
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

        PCP_COPY_VECTOR(localOrigin, origin);
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
            VU0_LOAD_VF(vf10, point);
            ring = ring == 0 ? lastIndex : ring - 1;
            point[0] = localOrigin[0] + history->points[ring][0];
            point[1] = localOrigin[1] + history->points[ring][1];
            point[2] = localOrigin[2] + history->points[ring][2];
            VU0_LOAD_VF(vf12, point);
            VU0_MOVE_VF(vf11, vf12);
            VU0_SUB(vf10, vf10, vf11);
            VU0_LOAD_VF(vf11, parSharedViewDelta);
            VU0_CROSS_XYZ(vf10, vf10, vf11);
            VU0_NORMALIZE_VF10();
            VU0_LOAD_VF(vf11, parSharedCrossScale);
            VU0_MUL(vf10, vf10, vf11);
            VU0_MOVE_VF(vf2, vf10);
            VU0_MOVE_VF(vf10, vf12);
            VU0_MOVE_VF(vf12, vf2);
            VU0_MOVE_VF(vf11, vf10);
            VU0_ADD(vf10, vf10, vf12);
            VU0_STORE_VF_UNCLOBBERED(vf10, vertex);
            VU0_MOVE_VF(vf10, vf12);
            VU0_SUB(vf11, vf11, vf10);
            VU0_STORE_VF_UNCLOBBERED(vf11, vertex + 1);
            vertex += 2;
        }
        parFadeAlphaCell(system, index);
    }
    effBillSetEntryValue(system, index, color);
}

extern void parFadeTriangleCellAlpha(ParSystem *system, s32 index);

/* Triangle-cell variant of parBuildHistoryRibbonStrip: three vertices per history segment. */
void parDrawHistoryRibbonTriangles(ParSystem *system, s32 index, const f32 *origin, u32 color, ParHistoryTable *source) {
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

        PCP_COPY_VECTOR(localOrigin, origin);
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
            VU0_LOAD_VF(vf10, point);
            ring = ring == 0 ? lastIndex : ring - 1;
            point[0] = localOrigin[0] + history->points[ring][0];
            point[1] = localOrigin[1] + history->points[ring][1];
            point[2] = localOrigin[2] + history->points[ring][2];
            VU0_LOAD_VF(vf12, point);
            VU0_MOVE_VF(vf11, vf12);
            VU0_SUB(vf10, vf10, vf11);
            VU0_LOAD_VF(vf11, parSharedViewDelta);
            VU0_CROSS_XYZ(vf10, vf10, vf11);
            VU0_NORMALIZE_VF10();
            VU0_LOAD_VF(vf11, parSharedCrossScale);
            VU0_MUL(vf10, vf10, vf11);
            VU0_MOVE_VF(vf2, vf10);
            VU0_MOVE_VF(vf10, vf12);
            VU0_MOVE_VF(vf12, vf2);
            VU0_STORE_VF_UNCLOBBERED(vf10, vertex + 1);
            VU0_MOVE_VF(vf11, vf10);
            VU0_ADD(vf10, vf10, vf12);
            VU0_STORE_VF_UNCLOBBERED(vf10, vertex);
            VU0_MOVE_VF(vf10, vf12);
            VU0_SUB(vf11, vf11, vf10);
            VU0_STORE_VF_UNCLOBBERED(vf11, vertex + 2);
            vertex += 3;
        }
        parFadeTriangleCellAlpha(system, index);
    }
    effBillSetEntryValue(system, index, color);
}

/* vu0 routine: modulate two RGBA8888 colours, (a/128 * b/128) * 128 per channel */
u32 effParModulateColors(u32 colorA, u32 colorB) {
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit = 0x3C000000;
    color1[0] = colorA;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = colorB;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_MUL(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;
    return blended[0];
}

/* Create through the selected dispatch entry and record its index; returns void. */
void parCreateIndexed(s32 dispatchIndex, void *creationData) {
    ParObj *createdObject;

    createdObject = parKindConstructorEntries[dispatchIndex].func(creationData);
    createdObject->dispatchIndex = dispatchIndex;
}

/* Invoke the selected destructor with the existing empty argument list. */
void parDispatchByKind(ParObj *obj) {
    parKindConstructorEntries[obj->dispatchIndex].destroy(obj);
}

/* Draw live particles after applying the requested number of emitter updates. */
void parUpdateAndDrawObject(ParObj *effect) {
    EffectBufferRecord *record;
    BillObj *billboard;
    u32 step;
    s32 particleCount;
    s32 lifetime;

    if (effect->restartFlag == 0) {
        return;
    }
    if (effect->pendingRestartSteps == 0) {
        parKindConstructorEntries[effect->dispatchIndex].update(effect);
    } else {
        for (step = 0; step < effect->pendingRestartSteps; step++) {
            parKindConstructorEntries[effect->dispatchIndex].update(effect);
        }
        effect->pendingRestartSteps = 0;
    }
    billboard = effect->billboard;
    record = effect->buffer->records;
    particleCount = effect->particleCount;
    lifetime = effect->lifetimeFrames;
    billSetChildScaleComponents(billboard, effect->billboardScale, effect->billboardScaleY);
    if (particleCount > 0) {
        step = particleCount;
        do {
            s32 age = record->age;

            if (age < lifetime && age >= 0) {
                PCP_COPY_VECTOR(billboard, record->position);
                billboard->childParam = record->color;
                billInvokeCallback(billboard);
            }
            step--;
            record++;
        } while (step != 0);
    }
    parSubmitKindDrawing(&effect->kindState);
}

/* Create another object of the same dispatch index and return the new instance. */
ParObj *parCloneKind(ParObj *obj) {
    ParObj *createdObject;

    createdObject = parKindConstructorEntries[obj->dispatchIndex].func(obj);
    createdObject->dispatchIndex = obj->dispatchIndex;
    return createdObject;
}

/* Reissue the callback, reload the native updater's repeat count, and arm restart. */
void parRestartKind(ParObj *obj) {
    D_003AAC20[obj->dispatchIndex](obj);
    obj->pendingRestartSteps = obj->restartStepCount;
    obj->restartFlag = 1;
}

extern void (*D_003AAC58[])(ParObj *, f32);

/* Call the selected entry with factor, additionally scale kinds 2..4, then restart. */
void parScaleAndRestartKind(ParObj *obj, f32 factor) {
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
    parRestartKind(obj);
}

/* Return the full 16-bit restart state without converting it to a boolean. */
u16 parGetRestartFlag(ParObj *obj) {
    return obj->restartFlag;
}

/* Copy one 16-byte vector with the existing EE/VU copy primitive. */
void parCopyVectorB(void *destination, void *source) {
    PCP_COPY_VECTOR(destination, source);
}

/* vu0 routine: compose the supplied matrix with the emitter's source matrix. */
void parComposeEffectTransformMatrices(ParObj *effect, void *matrix) {
    VU0_LOAD_MATRIX(matrix);
    VU0_LOAD_MATRIX_B(effect->sourceMatrix);
    sdfComposeVuMatrixFromRegisters();
    VU0_STORE_MATRIX(effect->matrix);
}
