#ifndef EFF_TRACK_POLY_LIST_H
#define EFF_TRACK_POLY_LIST_H

#include "eff.h"

/* Retained pointer array used by particle kind 4. */
typedef struct EffTrackPolyList {
    EffTrackPolyWork **items;
    u32 count;
    SdfMemBlock *handle;
} EffTrackPolyList;

typedef char EffTrackPolyListSizeCheck[
    (sizeof(EffTrackPolyList) == 0x0C) ? 1 : -1];
typedef char EffTrackPolyListCountOffsetCheck[
    ((u32)&((EffTrackPolyList *)0)->count == 0x04) ? 1 : -1];
typedef char EffTrackPolyListHandleOffsetCheck[
    ((u32)&((EffTrackPolyList *)0)->handle == 0x08) ? 1 : -1];

EffTrackPolyList *Eff_ModelTrackPolyListCreate(EffTrackPolyParams *params, u32 count);
void Eff_ModelTrackPolyListDelete(EffTrackPolyList *list);
/* Read two caller-owned 128-bit endpoint vectors. */
void Eff_ModelTrackPolyListVertexSet(EffTrackPolyList *list, s32 index, const u128 *endpoints);
void Eff_ModelTrackPolyListReset(EffTrackPolyList *list, s32 index);
void effTrackPolySetIndexedColor(EffTrackPolyList *list, s32 index, u32 color);
void Eff_ModelTrackPolyListDrawOnly(EffTrackPolyList *list);
void dds3Particle_Track3Set(EffTrackPolyList *list, s32 index, u32 color);

#endif
