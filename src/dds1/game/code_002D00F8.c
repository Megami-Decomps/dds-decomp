#include "common.h"

extern u32 D_003E274C[];

INCLUDE_ASM(const s32, "game/code_002D00F8", func_002D00F8);

/* Size class of the chip heap: cells per block and bytes per cell. */
typedef struct SdfChipClass {
    u32 unk0; /* 0x0 */
    u32 unk4; /* 0x4 */
    s16 unitSize; /* 0x8: bytes per cell */
    s16 cellCount; /* 0xA: cells per block */
} SdfChipClass; /* 0xC */

/* One 0x18-byte heap block record. */
typedef struct SdfChipBlockRecord {
    u8 pad00[0xC];
    SdfChipClass *sizeClass; /* 0xC: NULL when the block is unassigned */
    u8 pad10[4];
    s16 usedCells; /* 0x14 */
    u8 pad16[2];
} SdfChipBlockRecord; /* 0x18 */

typedef struct SdfChipStats {
    u32 totalBytes; /* 0x00 */
    u32 freeBytes; /* 0x04 */
    u32 blockCount; /* 0x08 */
    u32 emptyBlocks; /* 0x0C: blocks without a size class */
    u32 partialBlocks; /* 0x10: blocks with free cells */
    u32 usedCells[7]; /* 0x14: used cells per size class */
} SdfChipStats;

extern SdfChipBlockRecord *D_003BD9A8;
extern s32 D_003BD9B8;
extern SdfChipClass D_003E26F0[];

/* Fill `stats` with the chip heap's block totals and per-size-class usage. */
void func_002D01F0(SdfChipStats *stats) {
    SdfChipBlockRecord *block;
    s32 remaining;
    s32 i;
    s32 partialBlocks;
    s32 emptyBlocks;
    s32 freeBytes;
    s32 delta;

    remaining = D_003BD9B8;
    stats->blockCount = remaining;
    stats->totalBytes = remaining << 12;
    for (i = 0; i != 7; i++) {
        stats->usedCells[i] = 0;
    }
    block = D_003BD9A8;
    emptyBlocks = 0;
    partialBlocks = 0;
    freeBytes = 0;
    do {
        if (block->sizeClass == NULL) {
            emptyBlocks++;
            freeBytes += 0x1000;
        } else {
            delta = block->sizeClass->cellCount - block->usedCells;
            if (delta != 0) {
                partialBlocks++;
                freeBytes += delta * block->sizeClass->unitSize;
            }
            stats->usedCells[block->sizeClass - D_003E26F0] += block->usedCells;
        }
        block++;
    } while (--remaining != 0);
    stats->freeBytes = freeBytes;
    stats->emptyBlocks = emptyBlocks;
    stats->partialBlocks = partialBlocks;
}

INCLUDE_ASM(const s32, "game/code_002D00F8", func_002D02C0);

typedef struct {
    u8 pad00[0xC];
    u16 state; /* 0x0C: 0 free, 1 used, 2 end marker */
} SdfMemBlockPrefix;

u16 sdfGetMemoryBlockState(SdfMemBlockPrefix *block) {
    return block->state;
}

u32 func_002D0380(void) {
    return D_003E274C[0];
}

INCLUDE_SDATA(const s32, "game/code_002D00F8", D_003BD2DC);

INCLUDE_SDATA(const s32, "game/code_002D00F8", D_003BD2E0);

INCLUDE_SDATA(const s32, "game/code_002D00F8", D_003BD2E1);

