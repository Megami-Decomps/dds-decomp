#ifndef BTL_TARGET_RESULT_H
#define BTL_TARGET_RESULT_H

#include "common.h"

/* Per-hit amount in a target's fixed 32-entry result array. Unused bytes stay
 * opaque.
 */
typedef struct BtlHitResult {
    s32 amount;
    u8 pad04[0x28];
} BtlHitResult;

/* Result slot consumed by battle action amount and target-count routines. */
typedef struct BtlTargetResult {
    u8 hitCount;
    u8 pad01[7];
    u32 kind;
    u8 pad0C[4];
    u8 skipped;
    u8 pad11[3];
    u8 blocked;
    u8 pad15[7];
    BtlHitResult hits[32];
} BtlTargetResult;

typedef char BtlHitResult_size_must_be_0x2C[
    (sizeof(BtlHitResult) == 0x2C) ? 1 : -1];
typedef char BtlHitResult_amount_must_be_at_0x00[
    ((u32)&((BtlHitResult *)0)->amount == 0x00) ? 1 : -1];
typedef char BtlTargetResult_size_must_be_0x59C[
    (sizeof(BtlTargetResult) == 0x59C) ? 1 : -1];
typedef char BtlTargetResult_kind_must_be_at_0x08[
    ((u32)&((BtlTargetResult *)0)->kind == 0x08) ? 1 : -1];
typedef char BtlTargetResult_skipped_must_be_at_0x10[
    ((u32)&((BtlTargetResult *)0)->skipped == 0x10) ? 1 : -1];
typedef char BtlTargetResult_blocked_must_be_at_0x14[
    ((u32)&((BtlTargetResult *)0)->blocked == 0x14) ? 1 : -1];
typedef char BtlTargetResult_hits_must_be_at_0x1C[
    ((u32)&((BtlTargetResult *)0)->hits == 0x1C) ? 1 : -1];

#endif /* BTL_TARGET_RESULT_H */
