#include "common.h"
#include "dat_state.h"


extern void mdlFlagSet(s32);
extern void mdlFlagClear(s32);

typedef struct TimerWork {
    u8 pad00[0x74];
    s32 currentScore; /* 0x74: capped run score */
    s32 peakScore; /* 0x78: high-water mark seeded from the saved score */
    s32 pendingScore; /* 0x7C: score snapshot clamped/committed by the score callbacks */
    s32 progress;  /* 0x80 clamped to 100 */
    s32 step;      /* 0x84 quantised progress band */
    s16 completed; /* 0x88: remaining final-band ticks, decremented by the update */
    s16 countdown; /* 0x8A: interval before the next progress decrement */
    s32 updateCount; /* 0x8C */
} TimerWork;

void func_0031A830(u8 *work);

/* Advance progress for the selected record mode unless the final-band timer is active. */
void func_0031A638(u8 *record, s32 unused, TimerWork *timer) {
    if ((record[1] & 0xF) == 4 && timer->completed <= 0) {
        timer->progress++;
        timer->updateCount++;
        func_0031A830((u8 *)timer);
    }
}

extern void func_0035B6E0(const char *fmt, ...);
extern s32 mdlFlagTest(s32);

/* Seed the saved-score floor and score snapshots, then clear the current score. */
void mnuInitializeHighScoreState(TimerWork *work) {
    u32 minimum = mdlFlagTest(0x80E) == 0 ? 300000U : 600000U;
    if (datGameState->highScore < minimum) {
        datGameState->highScore = minimum;
    }
    work->peakScore = datGameState->highScore;
    work->pendingScore = datGameState->highScore;
    work->currentScore = 0;
    func_0035B6E0("************************Score Init!![%d]\n",
                  datGameState->highScore);
}


/* Clamp the pending score snapshot to the current high-water mark. */
void func_0031A730(TimerWork *work) {
    u32 peakScore = work->peakScore;
    u32 pendingScore = work->pendingScore;

    if (pendingScore < peakScore) {
        work->pendingScore = peakScore;
        pendingScore = peakScore;
    }
    func_0035B6E0("************************Score SetGlobal!![%d / %d]\n", (s32)pendingScore, (s32)peakScore);
}


/* Commit a new saved high score and record whether this snapshot beat the old value. */
void mnuUpdateHighScoreFlag(TimerWork *work) {
    u32 pendingScore = work->pendingScore;

    if (datGameState->highScore < pendingScore) {
        datGameState->highScore = pendingScore;
        mdlFlagSet(0x815);
    } else {
        mdlFlagClear(0x815);
    }
    func_0035B6E0("************************Score Update!![%d / %d]\n",
                  datGameState->highScore, work->pendingScore);
}


/* Reset current and peak scores to the pending snapshot. */
void func_0031A7F8(TimerWork *work) {
    s32 pendingScore = work->pendingScore;

    work->peakScore = pendingScore;
    work->currentScore = pendingScore;
    func_0035B6E0("************************Score Reset!![%d]\n", pendingScore);
}

/* Clamp/quantize progress; reaching the last band starts the timed completion state. */
void func_0031A830(u8 *work) {
    TimerWork *timer = (TimerWork *)work;
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
void mnuTickScoreProgressState(TimerWork *work) {
    u32 currentScore;

    func_0031A830((u8 *)work);
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

INCLUDE_ASM(const s32, "game/code_0031A638", func_0031AA10);
