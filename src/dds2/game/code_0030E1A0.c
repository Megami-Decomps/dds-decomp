#include "common.h"
#include "fpu.h"
#include "pcp_vu0.h"

typedef struct FieldTransitionStyle {
    s32 unk0;
    s32 unk4;
    s32 kind;
} FieldTransitionStyle;

extern u32 sdfCounterAnimationValue;
extern u32 D_004390A0;
extern FieldTransitionStyle *D_0043888C;
extern void func_0030E390(f32);
extern void func_0030DE08(f32, f32, s32, s32, s32, s32, s32, s32, s32);

void func_0030E1A0(s32 opening) {
    f32 progress;
    f32 glow;
    s32 timer;

    if (opening != 0) {
        timer = sdfCounterAnimationValue;
        if (timer < 20) {
            progress = timer / 20.0f;
        } else {
            progress = 1.0f;
        }
        if (timer < 14) {
            glow = 0.0f;
        } else if (timer < 26) {
            glow = 1.0f - (timer - 14) / 12.0f;
        } else {
            glow = 0.0f;
        }
        D_004390A0 = 0;
    } else {
        if (D_004390A0 == 0) {
            D_004390A0 = sdfCounterAnimationValue;
        }
        timer = D_004390A0 - sdfCounterAnimationValue;
        if (timer < 20) {
            progress = timer / 20.0f;
        } else {
            progress = 1.0f;
        }
        progress = 1.0f - progress;
        if (progress == 0.0f) {
            return;
        }
        glow = 0.0f;
    }
    func_0030E390(progress);
    if (D_0043888C != 0) {
        switch (D_0043888C->kind) {
        case 1:
            func_0030DE08((1.0f - glow) * 10.0f, 1.0f,
                         385, 303, 0, (s32)(glow * 104.0f), 19, 0, 84);
            return;
        case 2:
        case 3:
            func_0030DE08((1.0f - glow) * 10.0f, 1.0f,
                         385, 303, 0, (s32)(glow * 104.0f), 19, 0, 84);
            break;
        }
    }
}
