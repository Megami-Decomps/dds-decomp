#ifndef PAR_CELL_API_H
#define PAR_CELL_API_H

#include "common.h"

struct ParSystem;

struct ParSystem *dds3TrackPolyCreate(s32 count, s32 perCell,
                                      s32 groupDivisor, u32 kind);
void dds3TrackPolyDelete(struct ParSystem *system);
void dds3TrackPolyListInit(struct ParSystem *system, s32 cellIndex);
void dds3TrackPolyDraw(struct ParSystem *system);
void dds3TrackPolySetMode(struct ParSystem *system, u16 value);
void dds3TrackPolyColorSet(struct ParSystem *system, s32 cellIndex, u32 color);
/* Dispatch the selected color setter with two raw packed-color words. */
void dds3TrackPolyVCSet(struct ParSystem *system, s32 sub,
                        u32 colorWordA, u32 colorWordB);
void dds3TrackPolyLinkOt(void);
void dds3TrackPolyVC3Set_Acute(struct ParSystem *system, u32 centerWord,
                               u32 middleWord, u32 edgeWord);
void dds3TrackPolyVC5Set_Normal(struct ParSystem *system, s32 centerWord,
                                 s32 middleWord, s32 edgeWord);
void dds3TrackPolyVC5Set_Acute(struct ParSystem *system, u32 centerWord,
                                   u32 middleWord, u32 edgeWord);
void dds3TrackPolyVC5Set_AcuteR(struct ParSystem *system, u32 centerWord,
                                   u32 middleWord, u32 edgeWord);
void dds3TrackPolyVC5Set_AcuteW(struct ParSystem *system, u32 centerWord,
                                   u32 middleWord, u32 edgeWord);
void dds3TrackPolySet(struct ParSystem *system, s32 cellIndex,
                             const u128 *vertices);
void dds3TrackPolyVCRefresh_ParType1(struct ParSystem *system, s32 cellIndex);

#endif
