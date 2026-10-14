#ifndef BTL_AI_SCRATCH_H
#define BTL_AI_SCRATCH_H

#include "common.h"

/* One 0x10-byte AI query context. The actor pointer targets the title's
 * command-actor record, not its BtlUnit. DDS2's weighted-action producer
 * writes the actor and species ID; route selection writes the row and query
 * selector, which the conditional-action readers consume. DDS1 has matching
 * field reads and writes. */
#if defined(VERSION_DDS1)
struct BtlTask;
typedef struct BtlTask *BtlAiScratchActor;
#elif defined(VERSION_DDS2)
struct ActionStateLink;
typedef struct ActionStateLink *BtlAiScratchActor;
#else
#error "btl_ai_scratch.h requires a DDS title version"
#endif

typedef struct BtlAiScratchWork {
    BtlAiScratchActor actor; /* 0x00 */
    s32 speciesId; /* 0x04 */
    s32 rowIndex; /* 0x08: selected AiDecisionRow */
    s32 conditionKind; /* 0x0C: conditional-route selector */
} BtlAiScratchWork;

typedef char BtlAiScratchWork_size_must_be_0x10[
    (sizeof(BtlAiScratchWork) == 0x10) ? 1 : -1];
typedef char BtlAiScratchWork_species_offset_must_be_4[
    ((u32)&((BtlAiScratchWork *)0)->speciesId == 4) ? 1 : -1];
typedef char BtlAiScratchWork_row_offset_must_be_8[
    ((u32)&((BtlAiScratchWork *)0)->rowIndex == 8) ? 1 : -1];
typedef char BtlAiScratchWork_condition_offset_must_be_0xC[
    ((u32)&((BtlAiScratchWork *)0)->conditionKind == 0xC) ? 1 : -1];

#endif /* BTL_AI_SCRATCH_H */
