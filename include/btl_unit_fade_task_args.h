#ifndef BTL_UNIT_FADE_TASK_ARGS_H
#define BTL_UNIT_FADE_TASK_ARGS_H

struct BtlUnit;

/* Shared payload for the unit defeat and exit fade callbacks (12 bytes). */
typedef struct BtlUnitFadeTaskArgs {
    struct BtlUnit *unit; /* 0x00 */
    s32 durationOrMode;   /* 0x04 */
    s32 elapsed;          /* 0x08 */
} BtlUnitFadeTaskArgs;

typedef char BtlUnitFadeTaskArgsSizeCheck[sizeof(BtlUnitFadeTaskArgs) == 0x0C ? 1 : -1];
typedef char BtlUnitFadeTaskArgsDurationOffsetCheck[(u32)&((BtlUnitFadeTaskArgs *)0)->durationOrMode == 0x04 ? 1 : -1];
typedef char BtlUnitFadeTaskArgsElapsedOffsetCheck[(u32)&((BtlUnitFadeTaskArgs *)0)->elapsed == 0x08 ? 1 : -1];

#endif
