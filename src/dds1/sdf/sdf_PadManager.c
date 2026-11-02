#include "common.h"
#include "ps2asm.h"
#include "sdf_lighting.h"
#include "sdf_resource.h"

struct sdfPadCb_t;

struct sdfTexPacket_t;

struct sdfGsMemSlice_t;

struct sdfTexHandle_t;

/* Serialized byte consumed by the GS texture-state packet builder. */
typedef u8 SdfTextureClampMode;

typedef struct sdfPadCb_t {
    u8 port;                 /* 0x00 */
    u8 slot;                 /* 0x01 */
    u8 stat;                /* 0x02 */
    u8 opaque03;
    u8 mode;                 /* 0x04 */
    u8 reqMode;        /* 0x05 */
    u16 padData;             /* 0x06 */
    u16 lastPadData;         /* 0x08 */
    u8 opaque0A[2];
    u32 repeatDeadline;      /* 0x0C */
    u8 stick[4];             /* 0x10 */
    u8 pressure[12];         /* 0x14 */
    s16 actData;          /* 0x20 */
    s16 actDataBack;          /* 0x22 */
    s16 lastSmallMotor;      /* 0x24 */
    s16 lastLargeMotor;      /* 0x26 */
} sdfPadCb_t;

#define SDF_PAD_ENTRY_COUNT 2

#define SDF_PAD_REPLY_BUFFER_BYTES 0x20

#define SDF_PAD_ACTUATOR_BYTES 6

#define SDF_PAD_REPLY_DIGITAL 0x41

#define SDF_PAD_REPLY_ANALOG 0x73

#define SDF_PAD_REPLY_PRESSURE 0x79

#define SDF_PAD_PRESSURE_COUNT 12

#define SDF_PAD_STICK_CENTER 0x80

#define SDF_PAD_PORT_BUFFER_BYTES 0x100

#define SDF_PAD_BUTTON_STATE_BYTES 0x20

#define SDF_PAD_STICK_STATE_BYTES 8

#define SDF_PAD_PRESSURE_STATE_BYTES 0x18

extern u64 sdf3DRefMapParam;

extern u8 sdfPadCb_0[];

#define D_005518D8 D_003BD3A0
#define D_005298A0 sdfPadButtonMasks
#define D_005298E8 sdfPadButtonPressure
#define D_005518E0 sdfPadAnalogSticks
#define D_0058F0C0 sdfPadPortBuffers

extern u8 D_005518E0[8];

extern s8 dds3PadBitTable[0x20];

extern u8 D_005298E8[0x18];

extern u16 D_005518D8[2];

extern u16 D_005298A0[0x10];

extern s32 sdfVblankFlag;

extern u8 sdfPadPortSlotPairs[4];

extern u8 D_0058F0C0[];

extern u8 sdfPadManagerMode;

extern sdfPadCb_t sdfPadCb[];

/* Per-texture reference count and non-owning clone source (0x8). */
/* Texture buffer GPU command words (0x40); DDS1/2 game/code_002D10B0/00329F60.c. */
typedef struct sdfTexPacket_t {
    s32 gifTagWord; /* Low GIFtag word; NLOOP occupies bits 0-14. */
    u32 gifTagControl; /* High GIFtag control word; one packed register. */
    u64 gifTagRegisters; /* Packed register list: A+D. */
    u64 samplingState; /* GS TEX1 data, including minification/magnification filters. */
    u64 samplingRegister; /* GS TEX1_1/TEX1_2 register selector. */
    u64 textureState; /* GS TEX0 data. */
    u64 textureRegister; /* GS TEX0_1/TEX0_2 register selector. */
    u64 clampState; /* GS CLAMP data. */
    u64 clampRegister; /* GS CLAMP_1/CLAMP_2 register selector. */
} sdfTexPacket_t;



extern sdfTexHandle_t *D_005518B8;

extern sdfTexHandle_t *sdf3DSpecularTex;

extern sdfTexHandle_t *D_005518C8;

extern s32 scePadGetState(s32 port, s32 slot);

extern s32 scePadRead(s32 port, s32 slot, void *data);

extern s32 scePadGetReqState(s32 port, s32 slot);

extern s32 scePadInfoAct(s32 port, s32 slot, s32 actNo, s32 term);

extern s32 scePadSetActDirect(s32 port, s32 slot, void *actData);

extern s32 scePadSetMainMode(s32 port, s32 slot, s32 offs, s32 lock);

extern s32 scePadSetActAlign(s32 port, s32 slot, void *data);

extern s32 scePadInit(s32);

extern s32 scePadPortOpen(s32 port, s32 slot, void *buffer);

extern void *memcpy(void *, const void *, unsigned int);

void sdfPadControl(sdfPadCb_t *entry);

void sdfPadManager(void);

void sdfPadSetAct0(s32 pad, u16 value);

void sdfPadSetAct1(s32 pad, u8 value);

void sdfPadInitManager(void);

void sdfPadControl(sdfPadCb_t *entry) {
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
    switch (entry->stat) {
    case 0:
        if (padState == 2 || padState == 6) {
            entry->lastSmallMotor = -1;
            entry->lastLargeMotor = -1;
            requestedMode = entry->mode = entry->reqMode;
            switch (requestedMode) {
            case 0:
                entry->stat = 3;
                break;
            case 1:
                if (padState == 6) {
                    if (scePadSetMainMode(port, slot, 0, 0) == 1) {
                        entry->stat = 1;
                    }
                } else {
                    entry->stat = 3;
                }
                break;
            case 2:
                if (padState == 6) {
                    if (scePadSetMainMode(port, slot, 0, 3) == 1) {
                        entry->stat = 1;
                    }
                } else {
                    entry->stat = 3;
                }
                break;
            case 3:
                if (padState == 6) {
                    if (scePadSetMainMode(port, slot, 1, 3) == 1) {
                        entry->stat = 1;
                    }
                } else {
                    entry->stat = 3;
                }
                break;
            }
        }
        break;
    case 1:
        if (padState == 6) {
            entry->stat = 3;
        } else if (padState != 5) {
            entry->stat = 0;
        }
        break;
    case 2:
        break;
    case 3:
        if (scePadInfoAct(port, slot, -1, 0) != 0) {
            if (scePadSetActAlign(port, slot, sdfPadCb_0) != 0) {
                entry->stat = 4;
            }
        } else {
            entry->stat = 5;
        }
        break;
    case 4:
        alignmentStatus = scePadGetReqState(port, slot);
        if (alignmentStatus != 0) {
            if (alignmentStatus == 1) {
                entry->stat = 3;
            }
        } else {
            entry->stat = 5;
        }
        break;
    case 5:
        if (padState != 2 && padState != 6) {
            entry->stat = 0;
        } else if (entry->mode != entry->reqMode) {
            entry->stat = 0;
        } else {
            scePadRead(port, slot, reply);
            smallMotor = entry->actData;
            largeMotor = entry->actDataBack;
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
        entry->padData = ~(reply[3] | (reply[2] << 8));
    } else {
        entry->padData = 0;
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

void sdfPadManager(void) {
    s32 padIndex;
    for (padIndex = 0; padIndex != SDF_PAD_ENTRY_COUNT; padIndex++) {
        sdfPadControl(&sdfPadCb[padIndex]);
    }
}

void sdfPadWork(void) {
    s32 padIndex;
    s32 bitIndex;
    s32 vblank = sdfVblankFlag;

    for (padIndex = 0; padIndex != SDF_PAD_ENTRY_COUNT; padIndex++) {
        sdfPadCb_t *entry = &sdfPadCb[padIndex];
        u16 current = entry->padData;
        u16 previous = entry->lastPadData;
        u16 changed = current ^ previous;
        u16 pressed;
        u16 repeat;

        D_005518D8[padIndex] = current;
        pressed = changed & current;
        entry->lastPadData = current;
        if (current != previous) {
            entry->repeatDeadline = vblank + 15;
            repeat = pressed;
        } else {
            repeat = 0;
            if (current != 0) {
                s32 elapsed = vblank - entry->repeatDeadline;

                if (elapsed >= 0) {
                    repeat = current;
                    entry->repeatDeadline = vblank + elapsed + 4;
                }
            }
        }
        for (bitIndex = 0; bitIndex != 0x10; bitIndex++) {
            u16 mask = D_005298A0[bitIndex];
            s32 state = (current & mask) != 0;

            if (repeat & mask) {
                state |= 2;
            }
            if (pressed & mask) {
                state |= 0x80;
            }
            dds3PadBitTable[padIndex * 0x10 + bitIndex] = state;
        }
        memcpy(&D_005518E0[padIndex * 4], entry->stick, 4);
        memcpy(&D_005298E8[padIndex * SDF_PAD_PRESSURE_COUNT], entry->pressure,
               SDF_PAD_PRESSURE_COUNT);
    }
}

void sdfPadChangeMode(s32 aPadNum, u8 mode) {
    sdfPadCb[aPadNum].reqMode = mode;
}

void sdfPadSetAct0(s32 pad, u16 value) {
    sdfPadCb[pad].actData = value;
}

void sdfPadSetAct1(s32 pad, u8 value) {
    sdfPadCb[pad].actDataBack = value;
}

void sdfPadSetAct(s32 pad, s32 value0, s32 value1) {
    sdfPadCb_t *entry = &sdfPadCb[pad];
    entry->actData = value0 & 0xFF;
    sdfPadCb[pad].actDataBack = value1 & 0xFF;
}

void sdfPadInitManager(void) {
    s32 padIndex;

    scePadInit(0);
    for (padIndex = 0; padIndex != SDF_PAD_ENTRY_COUNT; padIndex++) {
        s32 port = sdfPadPortSlotPairs[padIndex * 2];
        s32 slot = sdfPadPortSlotPairs[padIndex * 2 + 1];
        sdfPadCb_t *entry;

        scePadPortOpen(port, slot, &D_0058F0C0[padIndex * SDF_PAD_PORT_BUFFER_BYTES]);
        entry = &sdfPadCb[padIndex];
        entry->port = port;
        entry->slot = slot;
        entry->stat = 0;
        entry->mode = 0;
        entry->reqMode = 0;
        entry->padData = 0;
        entry->lastPadData = 0;
        entry->actData = 0;
        entry->actDataBack = 0;
    }
    memset(dds3PadBitTable, 0, SDF_PAD_BUTTON_STATE_BYTES);
    memset(D_005518E0, SDF_PAD_STICK_CENTER, SDF_PAD_STICK_STATE_BYTES);
    memset(D_005298E8, 0, SDF_PAD_PRESSURE_STATE_BYTES);
    sdfPadManagerMode = 0;
}

INCLUDE_RODATA(const s32, "sdf/sdf_PadManager", D_003B4548);

INCLUDE_SDATA(const s32, "sdf/sdf_PadManager", sdfPadManagerMode);

INCLUDE_SDATA(const s32, "sdf/sdf_PadManager", D_003BD3A0);

INCLUDE_SDATA(const s32, "sdf/sdf_PadManager", sdfPadAnalogSticks);

INCLUDE_SDATA(const s32, "sdf/sdf_PadManager", D_003BD3A9);
