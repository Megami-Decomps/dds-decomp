#include "common.h"
#include "mnu_work.h"
#include "dat_state.h"
#include "mnu_shooting.h"
#include "mdl_flag_api.h"
#include "kwln_fade.h"




void mnuUpdateShootingProgressTier(MnuShootingWork *timer);

/* Advance progress for the selected record mode unless the final-band timer is active. */
void mnuAdvanceShootingProgressFromRecord(MenuRuntimeRecord *record, MenuRuntimeRecord *unused, MnuShootingWork *timer) {
    if ((record->state.kind & 0xF) == 4 && timer->completed <= 0) {
        timer->progress++;
        timer->updateCount++;
        mnuUpdateShootingProgressTier(timer);
    }
}

extern void sdfDebugPrintf(const char *fmt, ...);

/* Seed the saved-score floor and score snapshots, then clear the current score. */
void mnuInitializeHighScoreState(MnuShootingWork *work) {
    u32 minimum = mdlFlagTest(0x80E) == 0 ? 300000U : 600000U;
    if (datGameState->highScore < minimum) {
        datGameState->highScore = minimum;
    }
    work->peakScore = datGameState->highScore;
    work->pendingScore = datGameState->highScore;
    work->currentScore = 0;
    sdfDebugPrintf("************************Score Init!![%d]\n",
                  datGameState->highScore);
}


/* Clamp the pending score snapshot to the current high-water mark. */
void mnuClampPendingScoreToPeak(MnuShootingWork *work) {
    u32 peakScore = work->peakScore;
    u32 pendingScore = work->pendingScore;

    if (pendingScore < peakScore) {
        work->pendingScore = peakScore;
        pendingScore = peakScore;
    }
    sdfDebugPrintf("************************Score SetGlobal!![%d / %d]\n", (s32)pendingScore, (s32)peakScore);
}


/* Commit a new saved high score and record whether this snapshot beat the old value. */
void mnuUpdateHighScoreFlag(MnuShootingWork *work) {
    u32 pendingScore = work->pendingScore;

    if (datGameState->highScore < pendingScore) {
        datGameState->highScore = pendingScore;
        mdlFlagSet(0x815);
    } else {
        mdlFlagClear(0x815);
    }
    sdfDebugPrintf("************************Score Update!![%d / %d]\n",
                  datGameState->highScore, work->pendingScore);
}


/* Reset current and peak scores to the pending snapshot. */
void mnuResetCurrentAndPeakScoresToPending(MnuShootingWork *work) {
    s32 pendingScore = work->pendingScore;

    work->peakScore = pendingScore;
    work->currentScore = pendingScore;
    sdfDebugPrintf("************************Score Reset!![%d]\n", pendingScore);
}

/* Clamp/quantize progress; reaching the last band starts the timed completion state. */
void mnuUpdateShootingProgressTier(MnuShootingWork *timer) {
    s32 progress;
    f32 progressRatio;

    if (timer->progress >= 0x65) {
        timer->progress = 0x64;
    }
    progress = timer->progress;
    progressRatio = (f32)progress / 100.0f;
    if (timer->completed != 0) {
        return;
    }
    if (progressRatio < 0.25f) {
        timer->step = 1;
    } else if (progressRatio < 0.5f) {
        timer->step = 2;
    } else if (progressRatio < 0.75f) {
        timer->step = 4;
    } else if (progressRatio < 1.0f) {
        timer->step = 8;
    } else {
        timer->step = 0x14;
        timer->completed = 0x258;
        dds3ClaimSoundSlot(0x1E00005, 0);
    }
}

/* Decay progress at the current interval and maintain the score high-water mark. */
void mnuTickScoreProgressState(MnuShootingWork *work) {
    u32 currentScore;

    mnuUpdateShootingProgressTier(work);
    if (work->completed > 0) {
        work->completed--;
    }
    if (work->updateCount > 0) {
        work->updateCount--;
    }
    if (--work->countdown <= 0) {
        if (work->progress > 0) {
            work->progress--;
        }
        if (work->completed > 0) {
            work->countdown = 6;
        } else if (work->updateCount > 0) {
            work->countdown = 24;
        } else {
            work->countdown = 12;
        }
    }
    if (work->progress < 0) {
        work->progress = 0;
    }
    currentScore = work->currentScore;
    if (currentScore > 99999999U) {
        work->currentScore = 99999999;
        currentScore = 99999999U;
    }
    if ((u32)work->peakScore < currentScore) {
        work->peakScore = currentScore;
    }
}



extern void mnuAdvanceTitleStateUnderSemaphore(void);
extern s8 D_0037F510[];

s32 mnuAdvanceShootingRoundPhase(MnuShootingWork *work) {
    switch (work->phase) {
    case 0:
        work->phaseTicks = 0;
        work->phase = 1;
        kwlnFadeOutStart(0, 0, 0, 30);
        break;
    case 1:
        if (++work->phaseTicks > 180) {
            work->phaseTicks = 0;
            work->phase = 2;
        }
        break;
    case 3:
        work->phase = 4;
        work->phaseTicks = 0;
        itfSetFadeMode(&work->scoreFade, 1, 8);
        itfQueueFadeMode(&work->scoreFade, 0, 8, 120);
        break;
    case 4:
        if (++work->phaseTicks > 200) {
            work->phaseTicks = 0;
            work->phase = 5;
        }
        break;
    case 5:
        if (++work->phaseTicks > 180) {
            work->phaseTicks = 0;
            work->phase = 6;
            kwlnFadeInStart(0, 0, 0, 30);
            mnuAdvanceTitleStateUnderSemaphore();
        }
        break;
    case 6:
        if (++work->phaseTicks > 60) {
            work->phaseTicks = 0;
            work->phase = 7;
        }
        break;
    case 7:
        work->state = 0;
        work->phase = 0;
        work->round++;
        return 1;
    case 8:
        work->phaseTicks = 0;
        work->phase = 9;
        break;
    case 9:
        if (++work->phaseTicks > 120) {
            work->phaseTicks = 0;
            work->phase = 10;
            work->choiceIndex = 0;
            itfSetFadeMode(&work->choiceFade.fade, 1, 8);
        }
        break;
    case 10:
        if (D_0037F510[0x26] < 0) {
            work->choiceIndex--;
        }
        if (D_0037F510[0x27] < 0) {
            work->choiceIndex++;
        }
        if (work->choiceIndex < 0) {
            work->choiceIndex = 0;
        }
        if (work->choiceIndex > 1) {
            work->choiceIndex = 1;
        }
        itfSetTwoChoiceFadeSelection(&work->choiceFade, work->choiceIndex);
        if (D_0037F510[0x21] < 0) {
            work->phase = 11;
            work->result = work->choiceIndex;
        }
        break;
    case 11:
        work->phaseTicks = 0;
        work->phase = 12;
        itfSetFadeMode(&work->choiceFade.fade, 0, 8);
        kwlnFadeInStart(0, 0, 0, 120);
        mnuAdvanceTitleStateUnderSemaphore();
        break;
    case 12:
        if (++work->phaseTicks > 120) {
            work->phaseTicks = 0;
            switch (work->result) {
            case 0:
                work->phase = 0;
                work->state = 0;
                return 3;
            case 1:
                work->phase = 0;
                work->state = 0;
                return 2;
            }
        }
        break;
    }
    return 0;
}
