#ifndef PAR_CELL_API_H
#define PAR_CELL_API_H

#include "common.h"

struct ParSystem;

struct ParSystem *parAllocateCellSystem(s32 count, s32 perCell,
                                      s32 groupDivisor, u32 kind);
void parReleaseCellSystem(struct ParSystem *system);
void parCellInit(struct ParSystem *system, s32 index);
void parPrependCellNode(struct ParSystem *system);
void parSetCellDrawBucket(struct ParSystem *system, u16 value);
void parDrawPendingCellSystems(void);
void parDecreaseStripCellAlpha(struct ParSystem *system, u32 centerWord,
                               u32 middleWord, u32 edgeWord);
void parRiseFallSymmetricCellAlpha(struct ParSystem *system, u32 centerWord,
                                   u32 middleWord, u32 edgeWord);
void parUpdateCellVertexPair(struct ParSystem *system, s32 index,
                             const u128 *vertices);
void parFadeAlphaCell(struct ParSystem *system, s32 index);

#endif
