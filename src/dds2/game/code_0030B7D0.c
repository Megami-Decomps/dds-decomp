#include "common.h"

extern s32 D_00435E50;

extern s32 D_00435DD0;

extern u64 func_0019F460(s32, s32, u64, u64, u64, u64);

extern u64 func_0019D958(u64);

extern u64 func_0019F448(u64, u64, u64, u64, u64, u64);

extern u32 D_004388B8;

extern s32 D_004388C4;

typedef struct {
    u32 *word;         /* 0x00 */
    u8 pad04[4];
    s16 value;         /* 0x08 */
} SdfCounterDisplay;

typedef struct {
    s32 value;         /* 0x00 */
    s16 countdown;     /* 0x04 */
    s16 mode;          /* 0x06 */
} SdfCounterTimer;

typedef struct {
    u8 pad00[0x70];
    SdfCounterDisplay *display; /* 0x70 */
} SdfCounterChannel;

typedef struct {
    u8 pad00[0x1C];
    SdfCounterChannel *channel; /* 0x1C */
    u8 pad20[0x10];
    SdfCounterTimer *timer;     /* 0x30 */
} SdfCounterRuntime;

extern u32 D_00439098;

extern s32 D_0043909C;

extern u32 D_004390A0;

extern s32 func_0030C9B8(void);

extern s32 func_00101740(u32);

extern u32 func_00314690(u16);

extern u32 func_00314B80(u32, u16);

extern s32 D_004388BC;

extern void func_0030BBA8(void);

extern s8 D_004388C0;

extern u32 D_004390A4;

extern u32 D_0045C7A0[];

extern u32 D_0045C7B0[];

typedef struct ShortPair2C {
    u8 pad_0x00[0x2C]; // 0x00
    s16 h2C;           // 0x2C
    s16 h2E;           // 0x2E
} ShortPair2C; // 0x30

/* 24-byte table entries (full layout unknown; stride inferred from index math). */
typedef struct Entry24B {
    u8 v0;            // 0x00
    u8 pad_0x01[0x17]; // 0x01
} Entry24B; // 0x18

extern Entry24B D_00404A90[];

typedef struct Entry24W {
    u32 v0;            // 0x00
    u8 pad_0x04[0x14]; // 0x04
} Entry24W; // 0x18

extern Entry24W D_00404AA4[];

extern u8 D_00405CA8[];

/* Operand block used by the script VM helpers near func_002CD730 (layout inferred from field accesses). */
typedef struct ScrVmOperand {
    u8 pad_0x00[0x04]; // 0x00
    u8 b04;            // 0x04
    u8 pad_0x05[0x0F]; // 0x05
    u16 h14;           // 0x14
    u8 pad_0x16[0x3A]; // 0x16
    float f50;         // 0x50
    u8 unk54;          // 0x54
    s8 s55;            // 0x55
} ScrVmOperand; // 0x56

typedef struct ScriptFlagEntry {
    u32 unknown;
    u32 flags;
} ScriptFlagEntry;

extern void sdfCounterTickCountdown(void);

extern void mnuTickMapTimers(void);

extern void func_0030D438(void);

extern u32 func_00312A48(u32 *);

extern u32 func_00312188(u32, u32, u32);

extern void func_003123D0(void *, void *);

extern void kwlnTaskDestroyWithHierarchyByName(char *, s32);

extern void func_0030BD10();

extern void func_003139D8();

void func_003298C0(u32 sprite);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030B7D0);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030B838);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030B880);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030BA98);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030BBA8);

void func_0030BC30(u32 arg0) {
    func_0030BBA8();
    D_004388B8 = arg0;
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030BC58);

void func_0030BCA8(void) {
    if (D_004388B8 != 0) {
        func_0030BBA8();
        D_004388B8 = D_004388B8 - 1;
    } else {
        func_0030BBA8();
        D_004388B8 = D_004388BC - 1;
    }
}

s8 func_0030BCE8(void) {
    return D_004388C0;
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030BCF0);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030BD10);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030BED8);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030C0C0);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030C250);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030C378);

extern u8 D_00400BB0[];

u8 *func_0030C568(s32 scene) {
    if (scene == 4) {
        if (mdlFlagTest(0x13)) {
            scene = 10;
        }
        if (mdlFlagTest(0x29)) {
            scene = 11;
        }
    }
    return D_00400BB0 + scene * 52 - 0x34;
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030C5D8);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030C640);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030C660);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030C690);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030C8E8);

u32 func_0030C9A0(void) {
    return (u32)((SdfCounterRuntime *)D_004388C4)->channel->display->word;
}

s32 func_0030C9B8(void) {
    return ((SdfCounterRuntime *)D_004388C4)->channel->display->value;
}

s16 func_0030C9D0(s32 remaining) {
    s32 task = *(s32 *)(D_004388C4 + 0x10);
    if (remaining > 0) {
        do {
            remaining--;
            task = *(s32 *)(task + 0x58);
        } while (remaining != 0);
    }
    return *(s16 *)(*(s32 *)(task + 0x70) + 8);
}

float sdfCounterGetScaledValue(void) {
    SdfCounterTimer *timer;

    timer = ((SdfCounterRuntime *)D_004388C4)->timer;
    return (float)timer->value / 10.0f;
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030CA38);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030CC68);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030CEF0);

void sdfCounterIncrease(void) {
    s32 currentValue;

    currentValue = ((SdfCounterRuntime *)D_004388C4)->timer->value;
    if (currentValue < 10) {
        ((SdfCounterRuntime *)D_004388C4)->timer->value = currentValue + 1;
    }
}

void sdfCounterDecrease(void) {
    s32 currentValue;

    currentValue = ((SdfCounterRuntime *)D_004388C4)->timer->value;
    if (currentValue != 0) {
        ((SdfCounterRuntime *)D_004388C4)->timer->value = currentValue - 1;
    }
}

void sdfCounterSetMode(s32 mode) {
    SdfCounterTimer *timer;

    timer = ((SdfCounterRuntime *)D_004388C4)->timer;
    timer->mode = mode;
    timer->countdown = 8;
}

void sdfCounterTickCountdown(void) {
    SdfCounterTimer *timer;

    timer = ((SdfCounterRuntime *)D_004388C4)->timer;
    if (0 < timer->countdown) {
        timer->countdown = timer->countdown - 1;
    }
}

void mnuSetMapTimerFlags(s32 flags) {
    s16 *timers = *(s16 **)(D_004388C4 + 0x30);
    if ((flags & 1) != 0) {
        if (timers[6] == 0) {
            timers[6] = 1;
        }
    } else {
        timers[6] = 0;
    }
    if ((flags & 2) != 0) {
        if (timers[7] == 0) {
            timers[7] = 1;
        }
    } else {
        timers[7] = 0;
    }
}

void mnuTickMapTimers(void) {
    s16 *timers = *(s16 **)(D_004388C4 + 0x30);
    if (timers[6] > 0) {
        timers[6]--;
        if (timers[6] == 0) {
            timers[6] = 60;
        }
    }
    if (timers[7] > 0) {
        timers[7]--;
        if (timers[7] == 0) {
            timers[7] = 60;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030D3A8);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030D3C0);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030D408);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030D438);

void func_0030D4F8(void) {
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030D500);

u64 func_0030D780(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0019F448(0, 0, 0, 0, arg0, 0);
    temp_v1 = func_0019D958(temp_v0);
    func_0019C5B0(temp_v0);
    return temp_v1;
}

s32 func_0030D7E0(s32 mask, s32 ordinal) {
    s32 bitIndex = 0;
    s32 count = 0;
    s32 nextIndex;

    do {
        nextIndex = bitIndex + 1;
        if (ordinal == nextIndex) {
            break;
        }
        count += (mask >> bitIndex) & 1;
        bitIndex = nextIndex;
    } while (bitIndex < 0x1F);
    return count;
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030D818);

typedef struct {
    u8 pad00[0x0C];
    s32 selectedCount; /* 0x0C */
    u8 pad10[0x10];
    s32 maxCount;      /* 0x20 */
} MapSelection;

void fldSetMapSelectedCount(MapSelection *selection, s32 count) {
    if ((count <= selection->maxCount) && (count != 0)) {
        selection->selectedCount = count;
    }
}

void fldIncreaseMapSelectedCount(MapSelection *selection) {
    if (selection->selectedCount < 10) {
        selection->selectedCount = selection->selectedCount + 1;
    }
}

void fldDecreaseMapSelectedCount(MapSelection *selection) {
    if (1 < selection->selectedCount) {
        selection->selectedCount = selection->selectedCount - 1;
    }
}

u32 func_0030D900(s32 context) {
    s32 task = *(s32 *)(context + 0x10);
    u32 mask = 0;
    do {
        mask |= 1 << (*(s16 *)(*(s32 *)(task + 0x70) + 8) - 1);
        task = *(s32 *)(task + 0x58);
    } while (task != 0);
    return mask;
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030D938);

extern u32 D_0045C7C0[];

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030DAA0);

void func_0030DAE8(void) {
    s32 remaining = 24;
    u32 *slot = D_0045C7C0;
    do {
        if (*slot != 0) {
            func_003054E8(*slot);
        }
        *slot++ = 0;
    } while (--remaining >= 0);
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030DB40);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030DBF0);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030DE08);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030E010);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030E030);

INCLUDE_ASM(const s32, "game/code_0030B7D0", fldReleaseLocalMapResources);

void func_0030E130(void) {
    s32 temp_v0;

    D_00439098 = 0;
    temp_v0 = func_0030C9B8();
    D_0043909C = temp_v0 - 1;
    D_004390A0 = 0x3c;
}

void func_0030E160(void) {
    if ((s32)D_00439098 < 0x3C) {
        D_00439098++;
    }
}

void func_0030E180(void) {
    if ((s32)D_00439098 > 0) {
        D_00439098 -= 2;
    } else {
        D_00439098 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030E1A0);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030E390);

void func_0030E878(void) {
}

extern s32 func_0030EE40(s32, s32);

extern void fldSetMapRequestInterval(s32, u16);

extern void func_0030E940(void);

extern void func_0030E958(void);

extern s32 D_004390AC;

extern s32 D_004390B0;

extern u32 D_004390A8;

void func_0030E880(void) {
    s32 handler;
    handler = func_0030EE40(0x14, 0xC);
    D_004390AC = handler;
    *(s32 *)(D_004390AC + 0x18) = (s32)func_0030E940;
    fldSetMapRequestInterval(handler, 0);
    D_004390B0 = func_0030EE40(0x14, 0x18);
    *(s32 *)(D_004390B0 + 0x18) = (s32)func_0030E958;
    D_004390A8 = 5;
    D_004390A4 = 0;
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030E8E8);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030E910);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030E940);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030E958);

void func_0030EA70(void) {
    D_0045C7B0[0] = D_0045C7A0[0];
    D_0045C7B0[1] = D_0045C7A0[1];
    D_0045C7B0[2] = D_0045C7A0[2];
    D_004390A4 = 1;
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030EAA8);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030ECC0);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030EE40);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030EF18);

void fldAdvanceMapRequest(s32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    u32 *puVar1;

    puVar1 = *(u32 **)(arg0 + 8);
    if (*(s16 *)(arg0 + 0x16) == *(s16 *)(arg0 + 0x14)) {
        if (puVar1[3] == 0) {
            *puVar1 = arg1;
            puVar1[1] = arg2;
            puVar1[2] = arg3;
            *(u32 *)(arg0 + 8) = puVar1[4];
            puVar1[3] = 1;
        }
        *(u16 *)(arg0 + 0x16) = 0;
        return;
    }
    *(s16 *)(arg0 + 0x16) = *(s16 *)(arg0 + 0x16) + 1;
}

void fldSetMapRequestInterval(s32 arg0, u16 arg1) {
    *(u16 *)(arg0 + 0x14) = arg1;
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030EF90);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030F038);

INCLUDE_ASM(const s32, "game/code_0030B7D0", fldLoadMapResource);

u32 fldReleaseMapResource(s32 *arg0) {
    if (*arg0 != 0) {
        func_0032BBB0(*arg0);
        *arg0 = 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030F1A0);

void func_0030F2A8(s32 arg0, s32 arg1, u64 arg2, u64 arg3) {
    u64 temp_v0;

    temp_v0 = func_0019F460(arg0 << 4, arg1 << 3, 0, arg2, arg3, 0);
    func_0019D530(temp_v0, 1);
    func_0019C5B0(temp_v0);
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030F2F8);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030F390);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030F420);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030F4B8);

void sdfVec3AddInPlace(float *vector, float *delta) {
    *vector = *vector + *delta;
    vector[1] = vector[1] + delta[1];
    vector[2] = vector[2] + delta[2];
}

void sdfVec3SubtractInPlace(float *vector, float *delta) {
    *vector = *vector - *delta;
    vector[1] = vector[1] - delta[1];
    vector[2] = vector[2] - delta[2];
}

void sdfVec3AddComponents(float x, float y, float z, float *vector) {
    *vector = *vector + x;
    vector[1] = vector[1] + y;
    vector[2] = vector[2] + z;
}

void func_0030F898(f32 x, f32 y, f32 z, f32 *out) {
    out[0] = x;
    out[1] = y;
    out[2] = z;
}

void sdfVec3ScaleInPlace(float factor, float *vector) {
    *vector = *vector * factor;
    vector[1] = vector[1] * factor;
    vector[2] = vector[2] * factor;
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030F8D0);

INCLUDE_ASM(const s32, "game/code_0030B7D0", fldVectorLength);

INCLUDE_ASM(const s32, "game/code_0030B7D0", fldNormalizedVectorDot);

INCLUDE_ASM(const s32, "game/code_0030B7D0", fldVec3AngleBetween);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030FA28);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030FAF0);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030FD50);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_0030FFB0);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00310210);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00310320);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_003103C8);

float func_00310608(float x, float y) {
    float p = 1.0f;
    s32 i = 1;

    if (y >= 1.0f) {
        do {
            i++;
            p *= x;
        } while ((float)i <= y);
    }
    return p;
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00310648);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00310888);

void sdfVec4Add(float *arg0, float *arg1, float *arg2) {
    *arg0 = *arg1 + *arg2;
    arg0[1] = arg1[1] + arg2[1];
    arg0[2] = arg1[2] + arg2[2];
    arg0[3] = arg1[3] + arg2[3];
}

void sdfQuatMultiply(float *arg0, float *arg1, float *arg2) {
    *arg0 = (arg1[3] * *arg2 + *arg1 * arg2[3] + arg1[1] * arg2[2]) -
                          arg1[2] * arg2[1];
    arg0[1] = (arg1[3] * arg2[1] + arg1[1] * arg2[3] + arg1[2] * *arg2) -
                              *arg1 * arg2[2];
    arg0[2] = (arg1[3] * arg2[2] + arg1[2] * arg2[3] + *arg1 * arg2[1]) -
                              arg1[1] * *arg2;
    arg0[3] = ((arg1[3] * arg2[3] - *arg1 * *arg2) - arg1[1] * arg2[1]) -
                              arg1[2] * arg2[2];
}

float sdfQuatDot(float *arg0, float *arg1) {
    return *arg0 * *arg1 + arg0[1] * arg1[1] + arg0[2] * arg1[2] +
                  arg0[3] * arg1[3];
}

float func_00310B60(float *arg0, float *arg1) {
    return (arg0[1] * arg1[2] - arg0[2] * arg1[1]) +
                  (arg0[2] * *arg1 - *arg0 * arg1[2]) +
                  (*arg0 * arg1[1] - arg0[1] * *arg1);
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", fldVec4ArcCosDot);

float mdlQuaternionLengthSquared(float *quaternion) {
    return *quaternion * *quaternion + quaternion[1] * quaternion[1] +
                  quaternion[2] * quaternion[2] + quaternion[3] * quaternion[3];
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", quaternionMagnitude);

INCLUDE_ASM(const s32, "game/code_0030B7D0", quaternionInverse);

INCLUDE_ASM(const s32, "game/code_0030B7D0", quaternionNormalize);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00310D28);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00310DE8);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00310EB0);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00310FD0);

INCLUDE_ASM(const s32, "game/code_0030B7D0", quaternionBlendNormalize);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00311178);

INCLUDE_ASM(const s32, "game/code_0030B7D0", sdfQuatBlendAngular);

INCLUDE_ASM(const s32, "game/code_0030B7D0", sdfQuatSquad);

INCLUDE_ASM(const s32, "game/code_0030B7D0", sdfQuatLog);

INCLUDE_ASM(const s32, "game/code_0030B7D0", sdfQuatExp);

INCLUDE_ASM(const s32, "game/code_0030B7D0", sdfQuatSquadControl);

INCLUDE_ASM(const s32, "game/code_0030B7D0", sdfQuatForwardVector);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00311888);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_003118E8);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00311978);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00311A90);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00311AE8);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00311B58);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00311C50);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00311D00);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00311DB0);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00311E60);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00311F20);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_003120B8);

INCLUDE_ASM(const s32, "game/code_0030B7D0", sdfDestroyTaskWork);

void func_00312178(s32 arg0, s32 arg1) {
    if (arg1 != 0) {
        *(s32 *)(arg0 + 0x18) = (s32)arg1;
    }
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00312188);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00312228);

void func_00312310(s32 arg0, s32 arg1) {
    if (arg1 != 0) {
        *(s32 *)(arg0 + 0x14) = (s32)arg1;
    }
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00312320);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_003123D0);

INCLUDE_ASM(const s32, "game/code_0030B7D0", sdfClearTaskList);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_003124B0);

typedef struct TaskListNode {
    u32 handle; /* 0x00 */
    s32 key;    /* 0x04 */
    struct TaskListNode *next; /* 0x08 */
    u8 pad0C[4];
    u32 value;  /* 0x10 */
} TaskListNode;

typedef struct {
    u32 pad00;
    u32 tail;  /* 0x04 */
    TaskListNode *head; /* 0x08 */
    u32 count; /* 0x0C */
    u32 pad10;
    void (*onRemove)(u32, u32); /* 0x14 */
} TaskList;

typedef struct TaskWork {
    u32 handle;
    char *primaryTaskName;
    char *secondaryTaskName;
    TaskList *list;
    u32 firstItemHandle;
} TaskWork;

void *sdfFindTaskListNodeByKey(TaskList *list, s32 key) {
    TaskListNode *node;

    node = list->head;
    while (node->key != key) {
        node = node->next;
        if (node == NULL) {
            break;
        }
    }
    return node;
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00312578);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00312620);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_003126D0);

u8 func_00312710(TaskWork *work) {
    u8 exists;
    s64 task;

    exists = 0;
    if (work != NULL) {
        task = func_00101740((u32)work->primaryTaskName);
        exists = task != 0;
    }
    return exists;
}

s32 kwlnTaskExists(u32 name) {
    return func_00101740(name) != 0;
}

void sdfAttachTaskItem(TaskWork *work, u32 *item) {
    u32 result = func_00312188((u32)work->list, *item, func_00312A48(item));
    if (work->firstItemHandle == 0) {
        work->firstItemHandle = result;
    }
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", sdfRemoveTaskItem);

s32 func_003127E8(TaskWork *work, s32 key) {
    TaskListNode *item;

    item = sdfFindTaskListNodeByKey(work->list, key);
    if (item != NULL) {
        return item->value;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00312810);

INCLUDE_RODATA(const s32, "game/code_0030B7D0", D_0042D418);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00312850);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00312910);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00312A00);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00312A48);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00312B10);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00312B50);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00312B70);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00312C78);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00312D48);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00312DA0);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00312DF8);

void func_00312E20(void) {
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00312E28);

void func_00312F58(ShortPair2C *p, s32 a, s32 b) {
    p->h2C = a;
    p->h2E = b;
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", sdfDestroyGridWork);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00312FB0);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00312FD0);

void func_00313008(void *p, u32 *mod, u32 *div) {
    u32 *t = *(u32 **)((s32)p + 8);
    *mod = *t % *(u32 *)((s32)p + 0x14);
    *div = *t / *(u32 *)((s32)p + 0x14);
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00313040);

void func_00313078(s32 arg0, s32 arg1, s32 arg2, u32 arg3) {
    u32 temp_v0;

    temp_v0 = arg2 * *(s32 *)(arg0 + 0x14) + arg1;
    if (temp_v0 < *(u32 *)(arg0 + 0x10)) {
        *(u32 *)(temp_v0 * 8 + *(s32 *)(arg0 + 4) + 4) = arg3;
    }
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_003130B0);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00313100);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00313158);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_003131B0);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00313210);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00313280);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_003133C8);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00313538);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_003136A0);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00313810);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00313898);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_003139D8);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00313A58);

float func_00313B90(float arg0, float arg1, float arg2) {
    return arg0 + arg1 * arg2;
}

u32 func_00313BA0(void) {
    return 0;
}

void func_00313BA8(void) {
}

u32 func_00313BB0(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00313BB8);

void func_00313C40(void) {
    memset(D_00435DD0 + 0x17210, 0, 0x5800);
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00313C70);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00313D80);

extern u16 D_004052E8[][80];

void func_00313F88(u8 *work) {
    u16 *source = D_004052E8[*(u16 *)(work + 4)];
    u16 *slots = (u16 *)(work + 0x22);
    u32 index;
    index = 0;
    do {
        u16 id = *source++;
        if (id != 0) {
            scrSetFlag(work, id);
            *slots = id;
        }
        slots++;
        index++;
    } while (index < 8);
}

extern u16 D_004052F8[][80];

extern void func_0011CA88(u8 *);

void func_00314020(u8 *work) {
    u16 *source = D_004052F8[*(u16 *)(work + 4)];
    u32 index = 0;
    do {
        u16 id = *source++;
        if (id != 0) {
            scrSetFlag(work, id);
        }
        index++;
    } while (index < 40);
    if (mdlFlagTest(0xBA0)) {
        func_0011CA88(work);
    }
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_003140C8);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00314200);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00314298);

void func_003144E8(u32 arg0) {
    func_00314298(arg0, 0);
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00314500);

u32 func_00314668(u32 arg0, s32 *arg1) {
    *arg1 = D_00435E50 + (arg0 & 0xffff) * 0x13;
    return 1;
}

extern u32 D_00401328[][9];

u32 func_00314690(u16 scriptId) {
    return D_00401328[scriptId][0];
}

void func_003146B8(u32 arg0, u16 arg1, u32 arg2) {
    u32 *puVar1;

    puVar1 = (u32 *)func_00314B80(arg0, arg1);
    *puVar1 = arg2;
}

void func_003146E8(u32 arg0, u16 arg1) {
    u32 *puVar1;
    u32 temp_v0;

    puVar1 = (u32 *)func_00314B80(arg0, arg1);
    temp_v0 = func_00314690(arg1);
    *puVar1 = temp_v0;
}

extern u8 func_00314C10(s32);

extern u32 *func_00314BE0(s32);

u32 func_00314728(u8 *work, u32 amount) {
    u32 *total;
    u32 limit;
    u8 scriptId;
    if (func_00314C10((s32)work) == 0) return 0;
    total = func_00314BE0((s32)work);
    scriptId = work[0x55];
    *total += amount;
    limit = func_00314690(scriptId);
    if (limit < *total) {
        *total = limit;
    }
    return *total;
}

void func_003147A8(u32 v, u32 *a, u32 *b) {
    u32 lo;

    v &= 0xFFFF;
    lo = v & 0xF;
    v >>= 4;
    *a = v;
    *b = lo << 1;
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_003147C8);

void func_00314838(void) {
    memset(D_00435DD0 + 0x16f10, 0, 0x300);
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00314868);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00314990);

extern u32 func_00315FC8(u16);

extern s32 func_00314990(u8 *, u16);

s32 func_00314A08(u8 *work) {
    u32 index;
    for (index = 0; index < 0xB0; index++) {
        u16 id = index;
        if (!(func_00315FC8(id) & 1) && !func_00314990(work, id)) {
            return 0;
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00314A80);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00314B00);

u8 func_00314B78(s32 arg0) {
    return *(u8 *)(arg0 + 0x55);
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00314B80);

u32 func_00314BC0(u32 arg0, u16 arg1) {
    u32 *puVar1;

    puVar1 = (u32 *)func_00314B80(arg0, arg1);
    return *puVar1;
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00314BE0);

u8 func_00314C10(s32 arg0) {
    return *(u8 *)(arg0 + 0x55);
}

u8 func_00314C18(u8 *context, u32 entryId) {
    context[0x55] = entryId;
    func_00314A80(context, (u16)entryId);
    return context[0x55];
}

void scrDecodePackedFlagIndex(s32 unused, u32 v, u32 *a, u32 *b) {
    u32 lo;

    v &= 0xFFFF;
    lo = v & 7;
    v >>= 3;
    *a = v;
    *b = lo << 2;
}

void func_00314C68(s32 arg0) {
    memset(arg0 + 0x58, 0, 0x154);
}

s32 scrSetFlag(u8 *work, u16 index) {
    u32 word, shift;
    scrDecodePackedFlagIndex((s32)work, index, &word, &shift);
    *(u32 *)(work + 0x58 + word * 4) |= 1U << shift;
    return 1;
}

void func_00314CE8(u8 *work, u16 id) {
    u32 word, shift;
    u32 status = func_00315030(work, id);
    if (status == 1) {
        scrDecodePackedFlagIndex((s32)work, id, &word, &shift);
        *(u32 *)(work + 0x58 + word * 4) &= ~(status << shift);
        if (scrFindSlot(work, id) >= 0) {
            scrRemoveSlot(work, id);
        }
    }
}

void func_00314D90(void) {
    u32 temp_v0;
    s32 temp_v1;

    memset(D_00435DD0 + 0x16ef0, 0, 0x10);
    temp_v0 = mnuPickPairedTableValue(0x10, 0);
    temp_v1 = mnuPickPairedTableValue(0x10, 1);
    for (; (s32)temp_v0 < temp_v1; temp_v0 = temp_v0 + 1) {
        func_001B7940(temp_v0 & 0xffff, 1);
    }
}

void scrSetGlobalBitFlag(u32 id) {
    u16 bit;
    u32 *word;
    s32 offset;
    id &= 0xffff;
    if (id < 0x1ab) return;
    if (id >= 0x220) return;
    bit = id + 0xfe55;
    offset = 0x16ef0 + (bit >> 5) * 4;
    word = (u32 *)(D_00435DD0 + offset);
    *word |= 1U << (bit & 31);
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00314E80);

void scrSetSecondaryScriptFlag(u8 *work, u16 index) {
    u32 word, shift;
    scrDecodePackedFlagIndex((s32)work, index, &word, &shift);
    *(u32 *)(work + 0x58 + word * 4) |= 4U << shift;
}

void scrClearSecondaryScriptFlag(u8 *work, u16 index) {
    u32 word, shift;
    scrDecodePackedFlagIndex((s32)work, index, &word, &shift);
    *(u32 *)(work + 0x58 + word * 4) &= ~(4U << shift);
}

void func_00314F90(u8 *work) {
    s32 index = 0;
    do {
        scrClearSecondaryScriptFlag(work, (u16)index);
        index++;
    } while (index < 0x2A0);
}

u32 scrGetSecondaryScriptFlag(u8 *work, u16 index) {
    u32 word, shift;
    scrDecodePackedFlagIndex((s32)work, index, &word, &shift);
    return *(u32 *)(work + 0x58 + word * 4) & (4U << shift);
}

u32 func_00315030(u8 *work, u16 index) {
    u32 word, shift;
    u32 mask;
    scrDecodePackedFlagIndex((s32)work, index, &word, &shift);
    mask = *(u32 *)(work + 0x58 + word * 4);
    if (mask & (2U << shift)) {
        return 2;
    }
    return (mask & (1U << shift)) != 0;
}

s32 func_00315098(s32 arg0, s32 arg1) {
    u32 key = arg1 & 0xFFFF;
    u16 *p = (u16 *)(arg0 + 0x22);
    u32 i = 0;

    do {
        if (*p++ == key) {
            return 1;
        }
        i++;
    } while (i < 0x18);
    return 0;
}

s32 scrFindSlot(u8 *work, u16 key) {
    u32 index;
    u16 *entries = (u16 *)(work + 0x22);
    for (index = 0; index < 24; index++) {
        if (entries[index] == key) {
            return index;
        }
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00315118);

u32 func_00315138(u8 *work) {
    u16 *entries = (u16 *)(work + 0x22);
    u32 count = 0;
    u32 index;
    for (index = 0; index < 24; index++) {
        if (entries[index] != 0) {
            count++;
        }
    }
    return count;
}

u16 func_00315170(s32 arg0, s32 arg1, u16 arg2) {
    u16 temp_v0;
    u16 *puVar2;

    puVar2 = (u16 *)(arg1 * 2 + arg0 + 0x22);
    temp_v0 = *puVar2;
    *puVar2 = arg2;
    return temp_v0;
}

s32 scrRemoveSlot(u8 *work, u16 key) {
    s32 index = scrFindSlot(work, key);
    if (index >= 0) {
        *(u16 *)(work + 0x22 + index * 2) = 0;
        return 1;
    }
    return 0;
}

extern u8 D_00401324[];

extern u8 D_00401325[];

extern u16 D_00401326[];

extern u8 D_0040132C[][36];

u8 func_003151D0(u16 scriptId) {
    return D_00401324[scriptId * 36];
}

u16 func_003151F8(u16 scriptId) {
    return *(u16 *)((u8 *)D_00401326 + scriptId * 36);
}

u8 func_00315220(u16 scriptId) {
    return D_00401325[scriptId * 36];
}

u8 func_00315248(u16 scriptId, s32 index) {
    return D_0040132C[scriptId][index];
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00315270);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_003152D8);

u32 func_00315318(void) {
    return 0;
}

s32 func_00315320(u8 *work, u16 scriptId) {
    s32 current = *(u16 *)(work + 0x14);
    if (current < (s32)func_003151F8(scriptId)) return 0;
    return 1;
}

void func_00315350(u32 arg0, u32 arg1, u32 arg2) {
    memset(arg2, 0, 8);
}

u8 func_00315370(s32 arg0, u16 arg1) {
    return *(u8 *)(arg0 + 0x55) == arg1;
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00315388);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_003154A0);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00315628);

u32 func_00315640(u8 *work) {
    u8 *flags = work + 0xC;
    u32 index = 0;
    while (index < 8) {
        if (flags[index] == 0) return index;
        index++;
    }
    return 8;
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00315680);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00315700);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_003157A0);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00315838);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00315920);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00315938);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00315950);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00315A50);

extern u8 D_0045C828[];

extern u32 *func_0026CF70(s16);

s32 func_00315BF8(s16 id) {
    u32 *info = func_0026CF70(id);
    if (info == 0) {
        return 0;
    }
    return D_0045C828[(s32)(*info << 24) >> 28] != 0;
}

s32 func_00315C40(u32 index) {
    if (index >= 17) {
        return 0;
    }
    return D_0045C828[index] != 0;
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00315C68);

void func_00315FA0(u32 arg0, u32 arg1, u16 arg2) {
    func_00315C68(arg0, 0, arg1, arg2, 0);
}

extern u8 D_00401320[][36];

u32 func_00315FC8(u16 index) {
    return *(u32 *)D_00401320[index];
}

typedef struct ScriptEntry44 {
    u32 state;
    u8 unknown[40];
} ScriptEntry44;

extern ScriptEntry44 D_00402BE0[];

u32 func_00315FF0(u16 index) {
    return D_00402BE0[index].state;
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00316020);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00316118);

u8 func_003161E8(s32 i) {
    return D_00404A90[i].v0;
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00316208);

u32 func_00316260(s32 i) {
    return D_00404AA4[i].v0;
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00316280);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_003162D8);

void scrSetEntryFlag(u32 context, u16 entryId, u32 bit) {
    ScriptFlagEntry *entry;
    if (bit < 16) {
        entry = (ScriptFlagEntry *)func_00314B80(context, entryId);
        entry->flags |= 1 << bit;
    }
}

void scrClearEntryFlag(u32 context, u16 entryId, u32 bit) {
    ScriptFlagEntry *entry;
    if (bit < 16) {
        entry = (ScriptFlagEntry *)func_00314B80(context, entryId);
        entry->flags &= ~(1 << bit);
    }
}

s32 scrTestEntryFlag(u32 context, u16 entryId, u32 bit) {
    if (bit >= 16) {
        return 0;
    }
    return (((ScriptFlagEntry *)func_00314B80(context, entryId))->flags & (1 << bit)) != 0;
}

void scrSetEntryLowFlags(u32 context, u16 entryId, u16 lowFlags) {
    ScriptFlagEntry *entry = (ScriptFlagEntry *)func_00314B80(context, entryId);
    entry->flags = (entry->flags & 0xFFFF0000) | lowFlags;
}

u16 scrGetEntryLowFlags(u32 context, u16 entryId) {
    return *(u16 *)&((ScriptFlagEntry *)func_00314B80(context, entryId))->flags;
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00316450);

u8 *func_003164C0(void) {
    return D_00405CA8;
}

void func_003164D0(s32 arg0) {
    u32 temp_v0;
    u32 *puVar2;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *(u32 *)(arg0 + 0x4c);
    puVar2 = *(u32 **)(arg0 + 8);
    if (temp_v0 != 0) {
        do {
            temp_v1 = temp_v1 + 1;
            *puVar2 = 0xffffffff;
            puVar2 = puVar2 + 2;
        } while (temp_v1 < temp_v0);
    }
    memset(*(u32 *)(arg0 + 0x10), 0, temp_v0 << 3);
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00316528);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00316648);

void func_00316668(s32 arg0) {
    func_003297C8(*(u32 *)(arg0 + 0x54));
}

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00316680);

INCLUDE_ASM(const s32, "game/code_0030B7D0", func_00316C88);

float func_00316DD0(ScrVmOperand *op) {
    return op->f50;
}

void func_00316DD8(ScrVmOperand *op, float value) {
    op->f50 = value;
}

void func_00316DE0(s32 arg0) {
    func_002D7458(arg0 + 0x14, arg0 + 0x38, 0, 0);
}

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_004388B4);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_004388B8);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_004388BC);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_004388C0);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_004388C1);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_004388C4);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_004388D0);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_004388D1);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_004388D8);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_004388E0);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_004388E8);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_004388F0);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_004388F8);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_00438900);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_00438908);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_00438910);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_00438918);

