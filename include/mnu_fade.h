#ifndef MNU_FADE_H
#define MNU_FADE_H

#include "common.h"

typedef struct FadeEntry {
    s32 mode;
    s32 step;
    s32 queuedMode; /* Zero means no transition; otherwise mode + 1. */
    s32 queuedStep;
    s32 delay;
    s32 extent;
} FadeEntry;
typedef struct FadeNumber {
    FadeEntry fade;
    s32 displayValue;
} FadeNumber;
typedef struct FadeGauge {
    FadeEntry fade;
    s32 displayValue;
    u32 parameter;
    u32 secondaryParameter;
    s32 phase;
    s32 filled[4];
    s32 decay[4];
} FadeGauge;
typedef struct FadeOffset {
    u8 glyph;
    u8 pad01[3];
    s32 x;
    s32 y;
} FadeOffset;
typedef char FadeLayoutAssert[
    (sizeof(FadeEntry) == 0x18 && sizeof(FadeNumber) == 0x1C &&
     sizeof(FadeGauge) == 0x48 && sizeof(FadeOffset) == 0xC &&
     (unsigned long)&((FadeGauge *)0)->phase == 0x24 &&
     (unsigned long)&((FadeGauge *)0)->filled == 0x28 &&
     (unsigned long)&((FadeGauge *)0)->decay == 0x38) ? 1 : -1];

struct EffectSlotSet;
extern struct EffectSlotSet *itfFadeTint;
extern const FadeOffset D_0040B088[];
extern u8 mnuGetIndexedFadeTexture(s32);
extern void itfFadeSetTint(struct EffectSlotSet *);
extern void itfSetFadeMode(FadeEntry *, s32, s32);
extern void itfQueueFadeMode(FadeEntry *, u32, u32, u32);
extern void itfDrawFadeGlyphStrip(FadeEntry *);
extern void itfDrawFadeGlyphTriplet(FadeEntry *);
extern void itfDrawLowerFadeGlyphPair(FadeNumber *);
extern void itfDrawUpperFadeGlyphPair(FadeNumber *);
extern void itfDrawFadeGlyphForFrame(FadeNumber *);
extern void mnuDrawFadeSequenceThree(FadeNumber *);
extern void mnuDrawFadeSequenceTwo(FadeNumber *);
extern void mnuDrawFadeSequenceOffset(FadeNumber *);
extern void func_0031E7C8(FadeNumber *, u32);
extern void func_0031E850(FadeNumber *, u32);
extern void func_0031ED68(FadeNumber *, u32);
extern void func_0031EE28(FadeNumber *, u32);
extern void func_0031EEE8(FadeNumber *, u32);
extern void func_0031F040(FadeNumber *, u32);
extern void itfSetFadeParameter(FadeGauge *, u32);
extern void itfSetFadeSecondaryParameter(FadeGauge *, u32);
extern void func_0031E8E8(FadeGauge *, u32);
extern void func_0031E8F0(FadeGauge *);
#endif
