#ifndef PAR_TABLE_H
#define PAR_TABLE_H

#include "eff.h"
#include "sdf_resource.h"

typedef struct ParSlot {
    u128 *points;
    u16 pointCount;
    u8 pad06[2];
    u32 color;
    f32 billboardScale;
} ParSlot;

typedef struct dds3Particle_BlurBuff_t {
    u16 slotCount;
    u16 pointCapacity;
    ParSlot *slots;
    billHandle_t **billboardRef;
    struct sdfMemHandle_t *allocation;
} dds3Particle_BlurBuff_t;

typedef char ParSlot_size_must_be_0x10[(sizeof(ParSlot) == 0x10) ? 1 : -1];
typedef char ParTable_size_must_be_0x10[(sizeof(dds3Particle_BlurBuff_t) == 0x10) ? 1 : -1];

void dds3Particle_BlurDelete(dds3Particle_BlurBuff_t *table);
/* Prepends the current particle vector to the selected slot's point history. */
void dds3Particle_BlurSet(dds3Particle_BlurBuff_t *table, s32 slotIndex, u32 color, f32 billboardScale);

#endif
