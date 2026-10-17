#ifndef BTL_START_VOICE_ROW_H
#define BTL_START_VOICE_ROW_H

#include "common.h"

typedef struct BattleStartVoiceRow {
    s32 flagId;        /* Negative disables the flag gate; otherwise require this model flag. */
    s8 voice[2][3][3]; /* [alternate][state][choice]; -1 marks an empty choice. */
    u8 pad16[2];
} BattleStartVoiceRow;

typedef char BattleStartVoiceRow_size_must_be_0x18[
    (sizeof(BattleStartVoiceRow) == 0x18) ? 1 : -1];
typedef char BattleStartVoiceRow_voice_offset_must_be_4[
    ((u32)&((BattleStartVoiceRow *)0)->voice == 4) ? 1 : -1];

#endif
