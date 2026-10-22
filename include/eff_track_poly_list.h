#ifndef EFF_TRACK_POLY_LIST_H
#define EFF_TRACK_POLY_LIST_H

#include "eff.h"

/* Retained pointer array used by particle kind 4. */
typedef struct effMTPolyListHandle_t {
    EffTrackPolyWork **items;
    u32 count;
    sdfMemHandle_t *handle;
} effMTPolyListHandle_t;

typedef char EffTrackPolyListSizeCheck[
    (sizeof(effMTPolyListHandle_t) == 0x0C) ? 1 : -1];
typedef char EffTrackPolyListCountOffsetCheck[
    ((u32)&((effMTPolyListHandle_t *)0)->count == 0x04) ? 1 : -1];
typedef char EffTrackPolyListHandleOffsetCheck[
    ((u32)&((effMTPolyListHandle_t *)0)->handle == 0x08) ? 1 : -1];

effMTPolyListHandle_t *Eff_ModelTrackPolyListCreate(EffTrackPolyParams *params, u32 count);
void Eff_ModelTrackPolyListDelete(effMTPolyListHandle_t *list);
/* Read two caller-owned 128-bit endpoint vectors. */
void Eff_ModelTrackPolyListVertexSet(effMTPolyListHandle_t *list, s32 index, const u128 *endpoints);
void Eff_ModelTrackPolyListReset(effMTPolyListHandle_t *list, s32 index);
void effTrackPolySetIndexedColor(effMTPolyListHandle_t *list, s32 index, u32 color);
void Eff_ModelTrackPolyListDrawOnly(effMTPolyListHandle_t *list);
void dds3Particle_Track3Set(effMTPolyListHandle_t *list, s32 index, u32 color);

#endif
