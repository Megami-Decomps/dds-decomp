#include "sdk_ps2.h"
#include "common.h"
#include "sdfFrame.h"
#include "file_config_option_api.h"
#include "ps2asm.h"
#include "sdf_pad_motor_api.h"
#include "eff_random_mod.h"
#include "dds3_task_update_count.h"
#include "btl.h"

extern s8 dds3PadButtonState[64];

extern void dds3PadVibrationSet(u32, u8, s32);
extern s8 dds3PadBitTable[32];
extern u8 dds3PadVibrationMode;
extern u8 dds3PadVibrationState[2];
#define DDS3_PAD_SMALL_MOTOR 0
#define DDS3_PAD_LARGE_MOTOR 1
#define DDS3_PAD_MOTOR_COUNT 2
#define DDS3_DEBUG_RUMBLE_PERIOD 0x4650
#define DDS3_DEBUG_SMALL_PULSE_FRAMES 0xF
#define DDS3_DEBUG_LARGE_PULSE_FRAMES 30
#define DDS3_DEBUG_RUMBLE_VARIATION 150
#define DDS3_DEBUG_RUMBLE_BASE 100

/* Periodically pulses both pad motors, varying the large-motor level. */
s32 dds3PadVibrationTest(dds3ProcessID_t *task) {
    if (dds3GetProcessRunTime(task) % SDF_FRAMES(DDS3_DEBUG_RUMBLE_PERIOD) == 0) {
        dds3PadVibrationSet(DDS3_PAD_SMALL_MOTOR, 1, DDS3_DEBUG_SMALL_PULSE_FRAMES);
        dds3PadVibrationSet(DDS3_PAD_LARGE_MOTOR, (u8)(dds3GetRandIntA(0, DDS3_DEBUG_RUMBLE_VARIATION) + DDS3_DEBUG_RUMBLE_BASE), DDS3_DEBUG_LARGE_PULSE_FRAMES);
    }
    return 0;
}

extern u32 dds3PadButtonHoldCounters[4][16];
extern u32 dds3PadStickHoldCounters[2][2];
extern s8 dds3PadButtonPrev[2][2][16];
extern u8 D_00435C30[2][2][2];
extern s8 D_00435C38[2][2];
extern u8 D_00435C40[2][2][2];
extern u8 D_00435C48[2][2];
extern u8 D_00438D98[2];
extern u32 D_00438DA0[2];

void dds3PadInitialize(void) {
    s32 port;
    s32 bank;
    s32 i;
    u32 (*inputTimers)[16] = dds3PadButtonHoldCounters;

    for (port = 0; port < 2; port++) {
        for (bank = 0; bank < 2; bank++) {
            for (i = 0; i < 16; i++) {
                dds3PadButtonPrev[bank][port][i] = 0;
                dds3PadButtonState[bank * 32 + port * 16 + i] = 0;
                inputTimers[bank * 2 + port][i] = 0;
            }
        }
        for (i = 0; i < 2; i++) {
            dds3PadStickHoldCounters[port][i] = 0;
            D_00435C30[port][i][0] = 0;
            D_00435C30[port][i][1] = 0;
            D_00435C38[port][i] = 0;
            D_00435C40[port][i][0] = 0;
            D_00435C40[port][i][1] = 0;
            D_00435C48[port][i] = 0;
        }
    }
    dds3PadVibrationState[DDS3_PAD_SMALL_MOTOR] = 0;
    D_00438D98[DDS3_PAD_SMALL_MOTOR] = 0;
    D_00438DA0[DDS3_PAD_SMALL_MOTOR] = 0;
    dds3PadVibrationState[DDS3_PAD_LARGE_MOTOR] = 0;
    D_00438D98[DDS3_PAD_LARGE_MOTOR] = 0;
    D_00438DA0[DDS3_PAD_LARGE_MOTOR] = 0;
}

extern u8 sdfPadAnalogSticks[8];
extern void dds3UpdateVibrationSlots(void);

/* vu0 routine: native stick length, normalization and forward dot product. */

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", D_00435C30);

void dds3RunProcessList(void) {
    s8 buttons[2][16];
    u8 sticks[2][2];
    f32 v[4];
    f32 forward[4];
    f32 length;
    f32 dot;
    f32 angle;
    s32 pad;
    s32 i;
    s32 j;
    s32 n;

    memset(forward, 0, sizeof(forward));
    forward[2] = -1.0f;
    for (pad = 0; pad < 2; pad++) {
        for (i = 0; i < 16; i++) {
            if (dds3PadBitTable[pad * 16 + i] != 0) {
                buttons[0][i] = 1;
                buttons[1][i] = 1;
            } else {
                buttons[0][i] = 0;
                buttons[1][i] = 0;
            }
        }
        for (i = 0; i < 2; i++) {
            sticks[i][0] = sdfPadAnalogSticks[pad * 4 + i * 2];
            sticks[i][1] = sdfPadAnalogSticks[pad * 4 + i * 2 + 1];
        }
        for (i = 0; i < 2; i++) {
            D_00435C30[pad][i][0] = sticks[i][0];
            D_00435C30[pad][i][1] = sticks[i][1];
        }
        v[0] = sticks[1][0] - 0x80;
        v[1] = 0;
        v[2] = sticks[1][1] - 0x80;
        v[3] = 0;
        SDF_VU0_LOADVECTOR(v);
        SDF_VU0_GETLENGTH(length);
        if (length >= 64.0f) {
            SDF_VU0_NORMALIZE();
            SDF_VU0_LOADVECTOR_B(forward);
            SDF_VU0_INNERPRODUCT(dot);
            if (dot < -1.0f) {
                dot = -1.0f;
            }
            if (dot > 1.0f) {
                dot = 1.0f;
            }
            angle = acosf(dot);
            if (v[0] < 0.0f) {
                angle = 6.283185307179586f - angle;
            }
            n = (s32)(angle * 57.29577951308232f);
            n = (n + 22) / 45;
            n %= 8;
            switch (n) {
            case 0:
                buttons[1][6] = 1;
                break;
            case 1:
                buttons[1][6] = 1;
                buttons[1][5] = 1;
                break;
            case 2:
                buttons[1][5] = 1;
                break;
            case 3:
                buttons[1][5] = 1;
                buttons[1][7] = 1;
                break;
            case 4:
                buttons[1][7] = 1;
                break;
            case 5:
                buttons[1][7] = 1;
                buttons[1][4] = 1;
                break;
            case 6:
                buttons[1][4] = 1;
                break;
            case 7:
                buttons[1][4] = 1;
                buttons[1][6] = 1;
                break;
            }
        }
        for (j = 0; j < 2; j++) {
            for (i = 0; i < 16; i++) {
                if (buttons[j][i] != 0) {
                    dds3PadButtonPrev[j][pad][i] = 1;
                    if (++dds3PadButtonHoldCounters[j * 2 + pad][i] == 1) {
                        dds3PadButtonPrev[j][pad][i] = -0x7D;
                    }
                } else {
                    dds3PadButtonPrev[j][pad][i] = 0;
                    dds3PadButtonHoldCounters[j * 2 + pad][i] = 0;
                }
                if (dds3PadButtonHoldCounters[j * 2 + pad][i] >= 8 && !(dds3PadButtonHoldCounters[j * 2 + pad][i] & 1)) {
                    dds3PadButtonPrev[j][pad][i] |= 2;
                }
            }
        }
        for (i = 0; i < 2; i++) {
            f32 stickLength;

            v[0] = sticks[i][0] - 0x80;
            v[1] = 0;
            v[2] = sticks[i][1] - 0x80;
            v[3] = 0;
            SDF_VU0_LOADVECTOR(v);
            SDF_VU0_GETLENGTH(stickLength);
            if (stickLength >= 64.0f) {
                D_00435C38[pad][i] = 1;
                if (++dds3PadStickHoldCounters[pad][i] == 1) {
                    D_00435C38[pad][i] = -0x7D;
                }
            } else {
                D_00435C38[pad][i] = 0;
                dds3PadStickHoldCounters[pad][i] = 0;
            }
            if (dds3PadStickHoldCounters[pad][i] >= 8 && !(dds3PadStickHoldCounters[pad][i] & 1)) {
                D_00435C38[pad][i] |= 2;
            }
        }
    }
    dds3UpdateVibrationSlots();
}

void dds3GetVibrationLevel(void) {
    u8 target = dds3PadVibrationMode;

    if (dds3PadVibrationState[1] < target) {
        if (target - dds3PadVibrationState[1] > 0x20) {
            dds3PadVibrationState[1] = dds3PadVibrationState[1] + 0x20;
        } else {
            dds3PadVibrationState[1] = target;
        }
    } else if (target < dds3PadVibrationState[1]) {
        if (dds3PadVibrationState[1] - target > 0x20) {
            dds3PadVibrationState[1] = dds3PadVibrationState[1] - 0x20;
        } else {
            dds3PadVibrationState[1] = target;
        }
    } else {
        dds3PadVibrationState[1] = target;
    }
}

/* Honor the saved rumble option, normalize small-motor strength and clamp
 * motor indices to the large motor. Preserve target update before output. */
void dds3PadVibrationSet(u32 motor, u8 level, s32 duration) {
    if (fileTestSavedSlotFlags(FILE_CONFIG_OPTION_RUMBLE) == 0) {
        return;
    }
    if (motor == DDS3_PAD_SMALL_MOTOR) {
        if (level != 0) {
            level = 1;
        }
    } else if (motor >= DDS3_PAD_MOTOR_COUNT) {
        motor = DDS3_PAD_LARGE_MOTOR;
    }
    if (motor == DDS3_PAD_SMALL_MOTOR) {
        D_00438D98[DDS3_PAD_SMALL_MOTOR] = level;
        dds3PadVibrationState[DDS3_PAD_SMALL_MOTOR] = level;
    } else {
        D_00438D98[motor] = level;
        dds3GetVibrationLevel();
    }
    D_00438DA0[motor] = duration;
    if (motor == DDS3_PAD_SMALL_MOTOR) {
        sdfPadSetAct0(0, level);
    } else {
        sdfPadSetAct1(0, dds3PadVibrationState[motor]);
    }
}

/* Clear motor levels and countdowns, then send both zero strengths to pad 0. */
void dds3PadVibrationReset(void) {
    dds3PadVibrationState[DDS3_PAD_SMALL_MOTOR] = 0;
    dds3PadVibrationState[DDS3_PAD_LARGE_MOTOR] = 0;
    D_00438DA0[DDS3_PAD_SMALL_MOTOR] = 0;
    D_00438DA0[DDS3_PAD_LARGE_MOTOR] = 0;
    sdfPadSetAct(0, 0, 0);
}

void dds3UpdateVibrationSlots(void) {
    s32 largeMotor = DDS3_PAD_LARGE_MOTOR;
    u8 *levels = dds3PadVibrationState;
    u8 *targets = D_00438D98;
    u8 *largeLevel;
    s32 motor;

    for (motor = 0; motor < DDS3_PAD_MOTOR_COUNT; motor++) {
        if (D_00438DA0[motor] != 0) {
            D_00438DA0[motor]--;
            if (D_00438DA0[motor] == 0) {
                levels[motor] = 0;
                if (motor == DDS3_PAD_SMALL_MOTOR) {
                    sdfPadSetAct0(0, 0);
                } else {
                    sdfPadSetAct1(0, 0);
                }
            } else if (motor == largeMotor) {
                largeLevel = levels + DDS3_PAD_LARGE_MOTOR;
                if (*largeLevel != targets[DDS3_PAD_LARGE_MOTOR]) {
                    dds3GetVibrationLevel();
                    sdfPadSetAct1(0, *largeLevel);
                }
            }
        }
    }
}

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", D_00435C38);

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", D_00435C3F);

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", D_00435C40);

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", D_00435C41);

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", D_00435C42);

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", D_00435C48);

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", D_00435C49);

