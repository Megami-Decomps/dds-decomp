#ifndef SND_BATTLE_SOUND_FILE_REQUEST_STATUS_H
#define SND_BATTLE_SOUND_FILE_REQUEST_STATUS_H

#include "common.h"

typedef s32 SndBattleSoundFileRequestStatus;
enum {
    SND_BATTLE_SOUND_FILE_REQUESTS_CLEAR = 0,
    SND_BATTLE_SOUND_FILE_REQUESTS_PENDING = 1
};

/* Pending means a not-yet-ready sound-slot owner still retains a file request. */
SndBattleSoundFileRequestStatus sndHasOccupiedNodeSlots(void);

#endif /* SND_BATTLE_SOUND_FILE_REQUEST_STATUS_H */
