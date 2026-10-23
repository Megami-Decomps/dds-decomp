#ifndef BTL_ACTOR_ENTRY_CODE_DEFAULTS_H
#define BTL_ACTOR_ENTRY_CODE_DEFAULTS_H

#include "common.h"

typedef struct BtlActorEntryCodeDefault {
    s16 maxCode;
    s16 minCode;
    s16 initialValue;
    s16 countdownFrames;
} BtlActorEntryCodeDefault;

typedef char BtlActorEntryCodeDefaultSizeCheck[
    sizeof(BtlActorEntryCodeDefault) == 8 ? 1 : -1];
typedef char BtlActorEntryCodeDefaultMaxCodeOffsetCheck[
    ((u32)&((BtlActorEntryCodeDefault *)0)->maxCode == 0) ? 1 : -1];
typedef char BtlActorEntryCodeDefaultMinCodeOffsetCheck[
    ((u32)&((BtlActorEntryCodeDefault *)0)->minCode == 2) ? 1 : -1];
typedef char BtlActorEntryCodeDefaultInitialValueOffsetCheck[
    ((u32)&((BtlActorEntryCodeDefault *)0)->initialValue == 4) ? 1 : -1];
typedef char BtlActorEntryCodeDefaultCountdownOffsetCheck[
    ((u32)&((BtlActorEntryCodeDefault *)0)->countdownFrames == 6) ? 1 : -1];

extern BtlActorEntryCodeDefault btlActorEntryCodeDefaults[];

#endif
