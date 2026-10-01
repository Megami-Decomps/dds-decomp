#include "common.h"

INCLUDE_ASM(const s32, "game/code_0031A638", func_0031A638);

INCLUDE_ASM(const s32, "game/code_0031A638", func_0031A690);

INCLUDE_ASM(const s32, "game/code_0031A638", func_0031A730);

INCLUDE_ASM(const s32, "game/code_0031A638", func_0031A770);

INCLUDE_ASM(const s32, "game/code_0031A638", func_0031A7F8);

typedef struct TimerWork {
    u8 pad00[0x80];
    s32 progress;  /* 0x80 clamped to 100 */
    s32 step;      /* 0x84 quantised progress band */
    s16 completed; /* 0x88 non-zero once the final band is reached */
} TimerWork;

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
