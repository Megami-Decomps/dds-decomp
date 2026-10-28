#ifndef EVT_STAGE_ANIME_CONTROL_API_H
#define EVT_STAGE_ANIME_CONTROL_API_H

#include "common.h"

/* Whether stage animation control found an active world to update. */
typedef s32 EvtStageAnimeControlResult;
enum {
    EVT_STAGE_ANIME_CONTROL_NO_ACTIVE_WORLD = 0,
    EVT_STAGE_ANIME_CONTROL_ACTIVE_WORLD = 1,
};

EvtStageAnimeControlResult evtStageAnimeControl(s32 modeSelector, s32 modeValue);

#endif /* EVT_STAGE_ANIME_CONTROL_API_H */
