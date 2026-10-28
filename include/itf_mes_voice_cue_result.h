#ifndef ITF_MES_VOICE_CUE_RESULT_H
#define ITF_MES_VOICE_CUE_RESULT_H

#include "common.h"
#include "itf_mes_entry_index.h"
#include "itf_mes_window_index.h"

/* The selected text scan reports whether it contains the F413 voice cue. */
typedef s32 ItfMesVoiceCueResult;
enum {
    ITF_MES_VOICE_CUE_NOT_FOUND = 0,
    ITF_MES_VOICE_CUE_FOUND = 1
};

ItfMesVoiceCueResult itfMesSelectTableItemText(ItfMesWindowIndex window,
    ItfMesEntryIndex entryIndex, s32 itemIndex);

#endif /* ITF_MES_VOICE_CUE_RESULT_H */
