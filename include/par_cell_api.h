#ifndef PAR_CELL_API_H
#define PAR_CELL_API_H

#include "common.h"

struct ParSystem;

struct ParSystem *parAllocateCellSystem(s32 count, s32 perCell,
                                      s32 groupDivisor, u32 kind);
void parReleaseCellSystem(struct ParSystem *system);
void parCellInit(struct ParSystem *system, s32 index);
void parPrependCellNode(struct ParSystem *system);

#endif
