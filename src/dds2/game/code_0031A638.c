#include "common.h"
#include "mnu_work.h"
#include "dat_state.h"
#include "mnu_shooting.h"
#include "mnu_shooting_round_phase_api.h"
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

#include "sdf_debug_api.h"

/* Seed the saved-score floor and score snapshots, then clear the current score. */
void mnuInitializeHighScoreState(MnuShootingWork *work) {
    u32 minimum = evtBitCheck(0x80E) == 0 ? 300000U : 600000U;
    if (DDS3_GBWK->highScore < minimum) {
        DDS3_GBWK->highScore = minimum;
    }
    work->peakScore = DDS3_GBWK->highScore;
    work->pendingScore = DDS3_GBWK->highScore;
    work->currentScore = 0;
    printf("************************Score Init!![%d]\n",
                  DDS3_GBWK->highScore);
}


/* Clamp the pending score snapshot to the current high-water mark. */
void mnuClampPendingScoreToPeak(MnuShootingWork *work) {
    u32 peakScore = work->peakScore;
    u32 pendingScore = work->pendingScore;

    if (pendingScore < peakScore) {
        work->pendingScore = peakScore;
        pendingScore = peakScore;
    }
    printf("************************Score SetGlobal!![%d / %d]\n", (s32)pendingScore, (s32)peakScore);
}


/* Commit a new saved high score and record whether this snapshot beat the old value. */
void mnuUpdateHighScoreFlag(MnuShootingWork *work) {
    u32 pendingScore = work->pendingScore;

    if (DDS3_GBWK->highScore < pendingScore) {
        DDS3_GBWK->highScore = pendingScore;
        evtBitOn(0x815);
    } else {
        evtBitOff(0x815);
    }
    printf("************************Score Update!![%d / %d]\n",
                  DDS3_GBWK->highScore, work->pendingScore);
}


/* Reset current and peak scores to the pending snapshot. */
void mnuResetCurrentAndPeakScoresToPending(MnuShootingWork *work) {
    s32 pendingScore = work->pendingScore;

    work->peakScore = pendingScore;
    work->currentScore = pendingScore;
    printf("************************Score Reset!![%d]\n", pendingScore);
}

struct SoundSlot;
extern struct SoundSlot * dds3ClaimSoundSlot(u32 sequence, u32 frames);

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
extern s8 dds3PadButtonState[];

MnuShootingRoundPhaseResult mnuAdvanceShootingRoundPhase(MnuShootingWork *work) {
    switch (work->phase) {
    case MNU_SHOOTING_PHASE_FADE_IN_START:
        work->phaseTicks = 0;
        work->phase = MNU_SHOOTING_PHASE_WAIT_FOR_PLAY;
        dds3FadeIn(0, 0, 0, 30);
        break;
    case MNU_SHOOTING_PHASE_WAIT_FOR_PLAY:
        if (++work->phaseTicks > 180) {
            work->phaseTicks = 0;
            work->phase = MNU_SHOOTING_PHASE_PLAY;
        }
        break;
    case MNU_SHOOTING_PHASE_ROUND_END_PENDING:
        work->phase = MNU_SHOOTING_PHASE_SCORE_FADE;
        work->phaseTicks = 0;
        itfSetFadeMode(&work->scoreFade, 1, 8);
        itfQueueFadeMode(&work->scoreFade, 0, 8, 120);
        break;
    case MNU_SHOOTING_PHASE_SCORE_FADE:
        if (++work->phaseTicks > 200) {
            work->phaseTicks = 0;
            work->phase = MNU_SHOOTING_PHASE_SCORE_HOLD;
        }
        break;
    case MNU_SHOOTING_PHASE_SCORE_HOLD:
        if (++work->phaseTicks > 180) {
            work->phaseTicks = 0;
            work->phase = MNU_SHOOTING_PHASE_FADE_OUT;
            dds3FadeOut(0, 0, 0, 30);
            mnuAdvanceTitleStateUnderSemaphore();
        }
        break;
    case MNU_SHOOTING_PHASE_FADE_OUT:
        if (++work->phaseTicks > 60) {
            work->phaseTicks = 0;
            work->phase = MNU_SHOOTING_PHASE_ROUND_COMPLETE;
        }
        break;
    case MNU_SHOOTING_PHASE_ROUND_COMPLETE:
        work->state = 0;
        work->phase = MNU_SHOOTING_PHASE_FADE_IN_START;
        work->round++;
        return MNU_SHOOTING_ROUND_RESULT_ROUND_COMPLETE;
    case MNU_SHOOTING_PHASE_SESSION_RESET_DELAY:
        work->phaseTicks = 0;
        work->phase = MNU_SHOOTING_PHASE_SESSION_RESET_WAIT;
        break;
    case MNU_SHOOTING_PHASE_SESSION_RESET_WAIT:
        if (++work->phaseTicks > 120) {
            work->phaseTicks = 0;
            work->phase = MNU_SHOOTING_PHASE_SESSION_CHOICE;
            work->choiceIndex = 0;
            itfSetFadeMode(&work->choiceFade.fade, 1, 8);
        }
        break;
    case MNU_SHOOTING_PHASE_SESSION_CHOICE:
        if (dds3PadButtonState[0x26] < 0) {
            work->choiceIndex--;
        }
        if (dds3PadButtonState[0x27] < 0) {
            work->choiceIndex++;
        }
        if (work->choiceIndex < 0) {
            work->choiceIndex = 0;
        }
        if (work->choiceIndex > 1) {
            work->choiceIndex = 1;
        }
        itfSetTwoChoiceFadeSelection(&work->choiceFade, work->choiceIndex);
        if (dds3PadButtonState[0x21] < 0) {
            work->phase = MNU_SHOOTING_PHASE_SESSION_EXIT;
            work->result = work->choiceIndex;
        }
        break;
    case MNU_SHOOTING_PHASE_SESSION_EXIT:
        work->phaseTicks = 0;
        work->phase = MNU_SHOOTING_PHASE_SESSION_EXIT_WAIT;
        itfSetFadeMode(&work->choiceFade.fade, 0, 8);
        dds3FadeOut(0, 0, 0, 120);
        mnuAdvanceTitleStateUnderSemaphore();
        break;
    case MNU_SHOOTING_PHASE_SESSION_EXIT_WAIT:
        if (++work->phaseTicks > 120) {
            work->phaseTicks = 0;
            switch (work->result) {
            case 0:
                work->phase = MNU_SHOOTING_PHASE_FADE_IN_START;
                work->state = 0;
                return MNU_SHOOTING_ROUND_RESULT_SESSION_RESET;
            case 1:
                work->phase = MNU_SHOOTING_PHASE_FADE_IN_START;
                work->state = 0;
                return MNU_SHOOTING_ROUND_RESULT_SESSION_STOP;
            }
        }
        break;
    }
    return MNU_SHOOTING_ROUND_RESULT_NO_ACTION;
}
