#ifndef BTL_UNIT_COLOR_FADE_ARGS_H
#define BTL_UNIT_COLOR_FADE_ARGS_H

struct BtlUnit;

/* State shared by the unit color fade-in and fade-out task callbacks. */
typedef struct BtlUnitColorFadeArgs {
    struct BtlUnit *unit; /* 0x00 */
    s32 fadeIn;           /* 0x04 */
    s32 fadeOut;          /* 0x08 */
    u32 count;            /* 0x0C */
    u32 color;            /* 0x10 */
} BtlUnitColorFadeArgs;

typedef char BtlUnitColorFadeArgsSizeCheck[
    sizeof(BtlUnitColorFadeArgs) == 0x14 ? 1 : -1];
typedef char BtlUnitColorFadeArgsFadeInOffsetCheck[
    (u32)&((BtlUnitColorFadeArgs *)0)->fadeIn == 0x04 ? 1 : -1];
typedef char BtlUnitColorFadeArgsFadeOutOffsetCheck[
    (u32)&((BtlUnitColorFadeArgs *)0)->fadeOut == 0x08 ? 1 : -1];
typedef char BtlUnitColorFadeArgsCountOffsetCheck[
    (u32)&((BtlUnitColorFadeArgs *)0)->count == 0x0C ? 1 : -1];
typedef char BtlUnitColorFadeArgsColorOffsetCheck[
    (u32)&((BtlUnitColorFadeArgs *)0)->color == 0x10 ? 1 : -1];

#endif
