#ifndef BTL_TINT_TRANSITION_H
#define BTL_TINT_TRANSITION_H

#include "common.h"

/* Observed view of the battle tint state; bytes +0x04..+0x17 remain opaque. */
typedef struct BtlTintTransition {
    u32 currentColor;          /* 0x00 */
    u8 opaque04[0x14];         /* 0x04 */
    u32 sourceColor;           /* 0x18 */
    u32 targetColor;           /* 0x1C */
    u16 framesRemaining;       /* 0x20 */
    u16 durationFrames;        /* 0x22 */
} BtlTintTransition;

typedef char BtlTintTransition_source_at_0x18[
    ((u32)&((BtlTintTransition *)0)->sourceColor == 0x18) ? 1 : -1];
typedef char BtlTintTransition_target_at_0x1C[
    ((u32)&((BtlTintTransition *)0)->targetColor == 0x1C) ? 1 : -1];
typedef char BtlTintTransition_remaining_at_0x20[
    ((u32)&((BtlTintTransition *)0)->framesRemaining == 0x20) ? 1 : -1];
typedef char BtlTintTransition_duration_at_0x22[
    ((u32)&((BtlTintTransition *)0)->durationFrames == 0x22) ? 1 : -1];

#endif /* BTL_TINT_TRANSITION_H */
