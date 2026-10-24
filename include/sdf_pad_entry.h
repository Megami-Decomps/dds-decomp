#ifndef SDF_PAD_ENTRY_H
#define SDF_PAD_ENTRY_H

#include "sdf_pad_mode.h"

enum {
    SDF_PAD_CONTROL_APPLY_REQUESTED_MODE = 0,
    SDF_PAD_CONTROL_WAIT_FOR_MODE_CHANGE = 1,
    SDF_PAD_CONTROL_UNKNOWN_2 = 2,
    SDF_PAD_CONTROL_PROBE_ACTUATORS = 3,
    SDF_PAD_CONTROL_WAIT_FOR_ACTUATOR_ALIGNMENT = 4,
    SDF_PAD_CONTROL_READ_INPUT_AND_UPDATE_MOTORS = 5
};

typedef struct SdfPadEntry {
    u8 port;                 /* 0x00 */
    u8 slot;                 /* 0x01 */
    u8 state;                /* 0x02 */
    u8 opaque03;
    SdfPadMode mode;          /* 0x04 */
    SdfPadMode requestedMode; /* 0x05 */
    u16 buttons;             /* 0x06 */
    u16 prevButtons;         /* 0x08 */
    u8 opaque0A[2];
    u32 repeatDeadline;      /* 0x0C */
    u8 stick[4];             /* 0x10 */
    u8 pressure[12];         /* 0x14 */
    s16 smallMotor;          /* 0x20 */
    s16 largeMotor;          /* 0x22 */
    s16 lastSmallMotor;      /* 0x24 */
    s16 lastLargeMotor;      /* 0x26 */
} SdfPadEntry;

typedef char SdfPadEntry_size[(sizeof(SdfPadEntry) == 0x28) ? 1 : -1];

#endif
