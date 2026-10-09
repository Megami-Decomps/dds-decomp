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

EffTrackPolyList *effTrackPolyCreateModelWorkList(EffTrackPolyParams *params, u32 count);
void effTrackPolyDestroyModelWorkList(EffTrackPolyList *list);
void effTrackPolyPushIndexedWorkEndpoints(EffTrackPolyList *list, s32 index, void *data);
void effTrackPolyResetIndexedWork(EffTrackPolyList *list, s32 index);
void effTrackPolySetIndexedColor(EffTrackPolyList *list, s32 index, u32 color);
void effTrackPolyDrawModelWorkList(EffTrackPolyList *list);
void parUpdateTrackPolygonCrossAxes(EffTrackPolyList *list, s32 index, u32 color);

#endif
