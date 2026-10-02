#include "common.h"

typedef struct ScoreGlobalState {
    u8 pad00[0x1E660];
    u32 highScore;
} ScoreGlobalState;

extern ScoreGlobalState *datGameState;
extern void mdlFlagSet(s32);
extern void mdlFlagClear(s32);

typedef struct TimerWork {
    u8 pad00[0x80];
    s32 progress;  /* 0x80 clamped to 100 */
    s32 step;      /* 0x84 quantised progress band */
    s16 completed; /* 0x88 non-zero once the final band is reached */
    u8 pad8A[2];
    s32 updateCount; /* 0x8C */
} TimerWork;

void func_0031A830(u8 *work);

void func_0031A638(u8 *record, s32 unused, TimerWork *timer) {
    if ((record[1] & 0xF) == 4 && timer->completed <= 0) {
        timer->progress++;
        timer->updateCount++;
        func_0031A830((u8 *)timer);
    }
}

typedef struct ScoreResetWork {
    u8 pad00[0x74];
    s32 unk74;
    s32 unk78;
    s32 unk7C;
} ScoreResetWork;
extern void func_0035B6E0(const char *fmt, ...);
extern s32 mdlFlagTest(s32);

void mnuInitializeHighScoreState(ScoreResetWork *work) {
    u32 minimum = mdlFlagTest(0x80E) == 0 ? 300000U : 600000U;
    if (datGameState->highScore < minimum) {
        datGameState->highScore = minimum;
    }
    work->unk78 = datGameState->highScore;
    work->unk7C = datGameState->highScore;
    work->unk74 = 0;
    func_0035B6E0("************************Score Init!![%d]\n",
                  datGameState->highScore);
}


void func_0031A730(ScoreResetWork *work) {
    u32 maximum = work->unk78;
    u32 score = work->unk7C;

    if (score < maximum) {
        work->unk7C = maximum;
        score = maximum;
    }
    func_0035B6E0("************************Score SetGlobal!![%d / %d]\n", (s32)score, (s32)maximum);
}


void mnuUpdateHighScoreFlag(ScoreResetWork *work) {
    u32 score = work->unk7C;

    if (datGameState->highScore < score) {
        datGameState->highScore = score;
        mdlFlagSet(0x815);
    } else {
        mdlFlagClear(0x815);
    }
    func_0035B6E0("************************Score Update!![%d / %d]\n",
                  datGameState->highScore, work->unk7C);
}


void func_0031A7F8(ScoreResetWork *work) {
    s32 score = work->unk7C;

    work->unk78 = score;
    work->unk74 = score;
    func_0035B6E0("************************Score Reset!![%d]\n", score);
}

void func_0031A830(u8 *work) {
    TimerWork *timer = (TimerWork *)work;
    s32 progress;
    f32 ratio;

    if (timer->progress >= 0x65) {
        timer->progress = 0x64;
    }
    progress = timer->progress;
    ratio = (f32)progress / 100.0f;
    if (timer->completed != 0) {
        return;
    }
    if (ratio < 0.25f) {
        timer->step = 1;
    } else if (ratio < 0.5f) {
        timer->step = 2;
    } else if (ratio < 0.75f) {
        timer->step = 4;
    } else if (ratio < 1.0f) {
        timer->step = 8;
    } else {
        timer->step = 0x14;
        timer->completed = 0x258;
        sndClaimFreeSoundSlot(0x1E00005, 0);
    }
}

INCLUDE_ASM(const s32, "game/code_0031A638", func_0031A920);

INCLUDE_ASM(const s32, "game/code_0031A638", func_0031AA10);
