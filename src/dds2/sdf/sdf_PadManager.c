#include "sdk_ps2.h"
#include "sdf_gs_header.h"
#include "eeregs.h"
#include "common.h"
#include "sdf_vu0.h"
#include "sdf_vu_matrix.h"
#include "sdf_pad_entry.h"
#include "sdf_pad_index.h"
#include "sdf_pad_mode_api.h"
#include "sdf_pad_motor_api.h"
#include "sdf_cons_matrix_packet.h"
#include "sdf_dma_packet_header.h"
#include "kwln.h"
#include "sdf_asset_packets.h"
#include "sdf_asset_state.h"
#include "sdf_vu_lighting.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "sdf_packet_header.h"
#include "sdf_packet_append.h"
#include "sdf_resource.h"
#include "sdf_primitive_packet_api.h"
#include "sdf.h"
#include "sdf_draw.h"
#include "sdf_request.h"
#include "sdf_model.h"
#include "sdf_projection.h"
#include "pcp_vu0.h"
#include "sdf_vu_transform_api.h"
#include "ee_mmi.h"
#include "fpu.h"
#include "sdf_texture_file.h"
#include "sdf_column_packet_api.h"
#include "sdf_lookup_texture.h"
#include "sdf_compact_vertex_vif_packet.h"
#include "sdf_dev_console_node.h"
#include "btl.h"
#include "sdf_packet_alloc.h"

#define SDF_PAD_REPLY_BUFFER_BYTES 0x20

#define SDF_PAD_ACTUATOR_BYTES 6

#define SDF_PAD_REPLY_DIGITAL 0x41

#define SDF_PAD_REPLY_ANALOG 0x73

#define SDF_PAD_REPLY_PRESSURE 0x79

#define SDF_PAD_STICK_COUNT 4

#define SDF_PAD_PRESSURE_COUNT 12

#define SDF_PAD_STICK_CENTER 0x80

#define SDF_PAD_BUTTON_COUNT 16

#define SDF_PAD_BUTTON_TRIGGER_BIT 2

#define SDF_PAD_BUTTON_NEW_PRESS_BIT 0x80

#define SDF_PAD_REPEAT_DELAY 15

#define SDF_PAD_REPEAT_STEP 4

#define SDF_PAD_MOTOR_VALUE_MASK 0xFF

#define SDF_PAD_PORT_BUFFER_BYTES 0x100

#define SDF_PAD_BUTTON_STATE_BYTES 0x20

#define SDF_PAD_STICK_STATE_BYTES 8

#define SDF_PAD_PRESSURE_STATE_BYTES 0x18

extern SdfPadEntry sdfPadCb[];

extern void sdfPadControl(SdfPadEntry *);

extern u8 sdfPadCb_0[];

extern s32 scePadGetState(s32 port, s32 slot);

extern s32 scePadRead(s32 port, s32 slot, void *data);

extern s32 scePadGetReqState(s32 port, s32 slot);

extern s32 scePadInfoAct(s32 port, s32 slot, s32 actNo, s32 term);

extern s32 scePadSetActDirect(s32 port, s32 slot, void *actData);

/* Drive mode/actuator setup and normalize this pad's latest reply.
 * Motor requests are sent only when their cached values change. */
void sdfPadControl(SdfPadEntry *entry) {
    u8 reply[SDF_PAD_REPLY_BUFFER_BYTES];
    u8 actuatorData[SDF_PAD_ACTUATOR_BYTES];
    s32 port = entry->port;
    s32 slot = entry->slot;
    s32 padState;
    s32 hasButtons;
    s32 hasAnalog;
    s32 hasPressure;
    s32 alignmentStatus;
    s32 requestedMode;
    s32 smallMotor;
    s32 largeMotor;
    reply[0] = -1;
    padState = scePadGetState(port, slot);
    switch (entry->state) {
    case SDF_PAD_CONTROL_APPLY_REQUESTED_MODE:
        if (padState == 2 || padState == 6) {
            entry->lastSmallMotor = -1;
            entry->lastLargeMotor = -1;
            requestedMode = entry->mode = entry->requestedMode;
            switch (requestedMode) {
            case SDF_PAD_MODE_KEEP_CURRENT:
                entry->state = SDF_PAD_CONTROL_PROBE_ACTUATORS;
                break;
            case SDF_PAD_MODE_DIGITAL_UNLOCKED:
                if (padState == 6) {
                    if (scePadSetMainMode(port, slot, 0, 0) == 1) {
                        entry->state = SDF_PAD_CONTROL_WAIT_FOR_MODE_CHANGE;
                    }
                } else {
                    entry->state = SDF_PAD_CONTROL_PROBE_ACTUATORS;
                }
                break;
            case SDF_PAD_MODE_DIGITAL_LOCKED:
                if (padState == 6) {
                    if (scePadSetMainMode(port, slot, 0, 3) == 1) {
                        entry->state = SDF_PAD_CONTROL_WAIT_FOR_MODE_CHANGE;
                    }
                } else {
                    entry->state = SDF_PAD_CONTROL_PROBE_ACTUATORS;
                }
                break;
            case SDF_PAD_MODE_DUALSHOCK_LOCKED:
                if (padState == 6) {
                    if (scePadSetMainMode(port, slot, 1, 3) == 1) {
                        entry->state = SDF_PAD_CONTROL_WAIT_FOR_MODE_CHANGE;
                    }
                } else {
                    entry->state = SDF_PAD_CONTROL_PROBE_ACTUATORS;
                }
                break;
            }
        }
        break;
    case SDF_PAD_CONTROL_WAIT_FOR_MODE_CHANGE:
        if (padState == 6) {
            entry->state = SDF_PAD_CONTROL_PROBE_ACTUATORS;
        } else if (padState != 5) {
            entry->state = SDF_PAD_CONTROL_APPLY_REQUESTED_MODE;
        }
        break;
    case SDF_PAD_CONTROL_UNKNOWN_2:
        break;
    case SDF_PAD_CONTROL_PROBE_ACTUATORS:
        if (scePadInfoAct(port, slot, -1, 0) != 0) {
            if (scePadSetActAlign(port, slot, sdfPadCb_0) != 0) {
                entry->state = SDF_PAD_CONTROL_WAIT_FOR_ACTUATOR_ALIGNMENT;
            }
        } else {
            entry->state = SDF_PAD_CONTROL_READ_INPUT_AND_UPDATE_MOTORS;
        }
        break;
    case SDF_PAD_CONTROL_WAIT_FOR_ACTUATOR_ALIGNMENT:
        alignmentStatus = scePadGetReqState(port, slot);
        if (alignmentStatus != 0) {
            if (alignmentStatus == 1) {
                entry->state = SDF_PAD_CONTROL_PROBE_ACTUATORS;
            }
        } else {
            entry->state = SDF_PAD_CONTROL_READ_INPUT_AND_UPDATE_MOTORS;
        }
        break;
    case SDF_PAD_CONTROL_READ_INPUT_AND_UPDATE_MOTORS:
        if (padState != 2 && padState != 6) {
            entry->state = SDF_PAD_CONTROL_APPLY_REQUESTED_MODE;
        } else if (entry->mode != entry->requestedMode) {
            entry->state = SDF_PAD_CONTROL_APPLY_REQUESTED_MODE;
        } else {
            scePadRead(port, slot, reply);
            smallMotor = entry->smallMotor;
            largeMotor = entry->largeMotor;
            if (smallMotor != entry->lastSmallMotor || largeMotor != entry->lastLargeMotor) {
                entry->lastSmallMotor = smallMotor;
                entry->lastLargeMotor = largeMotor;
                actuatorData[0] = smallMotor;
                actuatorData[1] = largeMotor;
                scePadSetActDirect(port, slot, actuatorData);
            }
        }
        break;
    }
    /* Pad reply IDs: digital, analog-stick, and pressure-sensitive modes.
     * Missing channels are normalized before consumers see this port. */
    hasAnalog = 0;
    hasPressure = 0;
    hasButtons = 0;
    if (reply[0] == 0) {
        switch (reply[1]) {
        case SDF_PAD_REPLY_DIGITAL:
            hasButtons = 1;
            break;
        case SDF_PAD_REPLY_ANALOG:
            hasButtons = 1;
            hasAnalog = 1;
            break;
        case SDF_PAD_REPLY_PRESSURE:
            hasButtons = 1;
            hasAnalog = 1;
            hasPressure = 1;
            break;
        }
    }
    if (hasButtons) {
        memset(entry->pressure, 0, SDF_PAD_PRESSURE_COUNT);
        entry->buttons = ~(reply[3] | (reply[2] << 8));
    } else {
        entry->buttons = 0;
    }
    if (hasAnalog) {
        entry->stick[0] = reply[4];
        entry->stick[1] = reply[5];
        entry->stick[2] = reply[6];
        entry->stick[3] = reply[7];
    } else {
        entry->stick[0] = SDF_PAD_STICK_CENTER;
        entry->stick[1] = SDF_PAD_STICK_CENTER;
        entry->stick[2] = SDF_PAD_STICK_CENTER;
        entry->stick[3] = SDF_PAD_STICK_CENTER;
    }
    if (hasPressure) {
        memcpy(entry->pressure, &reply[8], SDF_PAD_PRESSURE_COUNT);
    } else {
        memset(entry->pressure, 0, SDF_PAD_PRESSURE_COUNT);
    }
}

/* Update each configured logical pad entry. */
void sdfPadManager(void) {
    SdfPadIndex padIndex;
    for (padIndex = 0; padIndex != SDF_PAD_COUNT; padIndex++) {
        sdfPadControl(&sdfPadCb[padIndex]);
    }
}

extern s32 sdfVblankFlag;

extern u16 D_00438A90[4];

extern u8 sdfPadAnalogSticks[8];

extern u16 D_0040B7B0[16];

extern s8 dds3PadBitTable[0x20];

extern u8 sdfPadButtonPressure[0x18];

/* Emit held bit 0, press/repeat trigger bit 1, and new-press bit 7 per button.
 * Changes arm a 15-tick deadline; repeats set it to current tick + lateness + 4,
 * retaining the original extra delay when a deadline is missed. */
void sdfPadBuildButtonStates(void) {
    s32 currentTick = sdfVblankFlag;
    SdfPadIndex padIndex;
    s32 buttonIndex;
    for (padIndex = 0; padIndex != SDF_PAD_COUNT; padIndex++) {
        SdfPadEntry *entry = &sdfPadCb[padIndex];
        s32 heldButtons = entry->buttons;
        s32 previousButtons = entry->prevButtons;
        s32 newPressMask;
        s32 triggerMask;
        D_00438A90[padIndex] = heldButtons;
        entry->prevButtons = heldButtons;
        newPressMask = (heldButtons ^ previousButtons) & heldButtons;
        if (heldButtons != previousButtons) {
            entry->repeatDeadline = currentTick + SDF_PAD_REPEAT_DELAY;
            triggerMask = newPressMask;
        } else {
            triggerMask = 0;
            if (heldButtons != 0) {
                s32 ticksLate = currentTick - entry->repeatDeadline;
                if (ticksLate >= 0) {
                    triggerMask = heldButtons;
                    entry->repeatDeadline = currentTick + ticksLate + SDF_PAD_REPEAT_STEP;
                }
            }
        }
        for (buttonIndex = 0; buttonIndex != SDF_PAD_BUTTON_COUNT; buttonIndex++) {
            s32 buttonMask = D_0040B7B0[buttonIndex];
            s32 buttonState = (heldButtons & buttonMask) != 0;
            if (triggerMask & buttonMask) {
                buttonState |= SDF_PAD_BUTTON_TRIGGER_BIT;
            }
            if (newPressMask & buttonMask) {
                buttonState |= SDF_PAD_BUTTON_NEW_PRESS_BIT;
            }
            dds3PadBitTable[padIndex * SDF_PAD_BUTTON_COUNT + buttonIndex] = buttonState;
        }
        memcpy(&sdfPadAnalogSticks[padIndex * SDF_PAD_STICK_COUNT], entry->stick, SDF_PAD_STICK_COUNT);
        memcpy(&sdfPadButtonPressure[padIndex * SDF_PAD_PRESSURE_COUNT], entry->pressure, SDF_PAD_PRESSURE_COUNT);
    }
}

/* Store a mode request for the indexed logical pad; setup applies it later. */
void sdfPadChangeMode(SdfPadIndex padIndex, SdfPadMode requestedMode) {
    sdfPadCb[padIndex].requestedMode = requestedMode;
}

/* Store the small-motor request without validating the logical pad index. */
void sdfPadSetAct0(SdfPadIndex padIndex, u16 strength) {
    sdfPadCb[padIndex].smallMotor = strength;
}

/* Store the large-motor request without validating the logical pad index. */
void sdfPadSetAct1(SdfPadIndex padIndex, u8 strength) {
    sdfPadCb[padIndex].largeMotor = strength;
}

/* Despite the legacy console-prefixed name, set both pad motor low bytes. */
void sdfPadSetAct(SdfPadIndex padIndex, s32 smallMotor, s32 largeMotor) {
    SdfPadEntry *entry = &sdfPadCb[padIndex];
    entry->smallMotor = smallMotor & SDF_PAD_MOTOR_VALUE_MASK;
    sdfPadCb[padIndex].largeMotor = largeMotor & SDF_PAD_MOTOR_VALUE_MASK;
}

extern u8 D_00438A88[4];

extern u8 sdfPadPortBuffers[];

extern u8 D_00438A8C;

extern s32 scePadInit(s32);

/* Open both configured port/slot pairs and reset the public input arrays. */
void sdfPadInitManager(void) {
    SdfPadIndex padIndex;

    scePadInit(0);
    for (padIndex = 0; padIndex != SDF_PAD_COUNT; padIndex++) {
        s32 port = D_00438A88[padIndex * 2];
        s32 slot = D_00438A88[padIndex * 2 + 1];
        SdfPadEntry *entry;

        scePadPortOpen(port, slot, &sdfPadPortBuffers[padIndex * SDF_PAD_PORT_BUFFER_BYTES]);
        entry = &sdfPadCb[padIndex];
        entry->port = port;
        entry->slot = slot;
        entry->state = SDF_PAD_CONTROL_APPLY_REQUESTED_MODE;
        entry->mode = SDF_PAD_MODE_KEEP_CURRENT;
        entry->requestedMode = SDF_PAD_MODE_KEEP_CURRENT;
        entry->buttons = 0;
        entry->prevButtons = 0;
        entry->smallMotor = 0;
        entry->largeMotor = 0;
    }
    memset(dds3PadBitTable, 0, SDF_PAD_BUTTON_STATE_BYTES);
    memset(sdfPadAnalogSticks, SDF_PAD_STICK_CENTER, SDF_PAD_STICK_STATE_BYTES);
    memset(sdfPadButtonPressure, 0, SDF_PAD_PRESSURE_STATE_BYTES);
    D_00438A8C = 0;
}

INCLUDE_RODATA(const s32, "sdf/sdf_PadManager", D_0042E258);

INCLUDE_SDATA(const s32, "sdf/sdf_PadManager", D_00438A8C);

INCLUDE_SDATA(const s32, "sdf/sdf_PadManager", D_00438A90);

INCLUDE_SDATA(const s32, "sdf/sdf_PadManager", sdfPadAnalogSticks);

INCLUDE_SDATA(const s32, "sdf/sdf_PadManager", D_00438A99);

