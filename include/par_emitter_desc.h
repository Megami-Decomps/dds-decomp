#ifndef PAR_EMITTER_DESC_H
#define PAR_EMITTER_DESC_H

#include "common.h"

struct ParSystem;

/* Serialized particle cell state copied from the variable emitter header. */
typedef struct ParEmitCellState {
    u8 pad00[8];
    u16 verticesPerCell; /* 0x08 */
    u8 pad0A[0xA];
    u8 pad14[8];          /* 0x14: forwarded as two opaque callback arguments */
    struct ParSystem *cellSystem; /* 0x1C: allocated cell system */
    u32 *cellWords;       /* 0x20: inline per-cell word array */
    u8 pad24[0xC];
} ParEmitCellState;

/* Fixed 0xF0 descriptor prefix; variable per-cell words begin at +0xF0. */
typedef struct ParEmitDesc {
    u8 pad00[0x10];
    s32 count;            /* 0x10 */
    u8 pad14[4];
    s32 headerSize;       /* 0x18: serialized-header byte count */
    u8 pad1C[0xA4];
    ParEmitCellState cells; /* 0xC0 */
} ParEmitDesc;

typedef char ParEmitCellState_size_must_be_0x30[
    sizeof(ParEmitCellState) == 0x30 ? 1 : -1];
typedef char ParEmitCellState_vertices_at_0x08[
    ((u32)&((ParEmitCellState *)0)->verticesPerCell == 0x08) ? 1 : -1];
typedef char ParEmitCellState_system_at_0x1C[
    ((u32)&((ParEmitCellState *)0)->cellSystem == 0x1C) ? 1 : -1];
typedef char ParEmitCellState_words_at_0x20[
    ((u32)&((ParEmitCellState *)0)->cellWords == 0x20) ? 1 : -1];
typedef char ParEmitDesc_cells_at_0xC0[
    ((u32)&((ParEmitDesc *)0)->cells == 0xC0) ? 1 : -1];
typedef char ParEmitDesc_size_must_be_0xF0[
    sizeof(ParEmitDesc) == 0xF0 ? 1 : -1];

#endif
