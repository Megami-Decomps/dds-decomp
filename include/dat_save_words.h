#ifndef DAT_SAVE_WORDS_H
#define DAT_SAVE_WORDS_H

#include "common.h"

/* Three opaque words copied together between the game state and save preview. */
typedef struct DatSaveWords {
    u32 words[3];
} DatSaveWords;

typedef char DatSaveWordsSizeCheck[
    sizeof(DatSaveWords) == 0x0C ? 1 : -1];

#endif
