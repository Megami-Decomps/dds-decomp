#ifndef EVT_VIEWER_PLAYBACK_H
#define EVT_VIEWER_PLAYBACK_H

#include "common.h"

struct ObjBase;
struct EvtScaledValue;

/* Client view used by playback-mode iteration over world slots 6 and 3. */
typedef struct EvtViewerPlaybackData {
    struct ObjBase *object;
    struct EvtScaledValue *counter;
} EvtViewerPlaybackData;

typedef char EvtViewerPlaybackData_size_must_be_0x8[(sizeof(EvtViewerPlaybackData) == 0x8) ? 1 : -1];

#endif
