#ifndef EFF_EMITTER_TYPE_H
#define EFF_EMITTER_TYPE_H

#include "common.h"

/* An emitter type (0x10), effEmitterTypeDescriptors[type]: its per-frame slot
 * update, its parameter scaling, and the sizes of its slots and data. An
 * emitter record starts with its u16 type. */
typedef struct EffEmitterType {
    void (*updateSlots)(void *record);
    void (*scaleParameters)(void *record, f32 scale);
    s32 slotBytes;
    s32 dataBytes;
} EffEmitterType;

extern EffEmitterType effEmitterTypeDescriptors[];

/* Scale an emitter record's parameters through its type. */
void dds3DispatchIndexedCallback(u16 *record, f32 scale);

#endif
