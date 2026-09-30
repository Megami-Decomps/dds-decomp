#include "common.h"
#include "eff.h"

/* The channel copies three coordinates from each four-float source row,
 * then copies the four trailing values to a second block. */
typedef struct EffFloatRows {
    f32 primary[16];
    u8 pad40[0x18];
    f32 secondary[4];
} EffFloatRows;

/* Small channel object (0x18 bytes, created by effCreateChannel): float block
 * plus the channel-A cursor (count at +0x4, index at +0xC). */
typedef struct EffChan {
    void *unk0; /* 0x0: mem handle */
    u32 recordCount; /* 0x4: number of keyframe records */
    EffFloatRows *rows; /* 0x8: interpolation rows owned by the channel */
    u32 cursorIndex; /* 0xC: channel-A record index */
    f32 cursorPosition; /* 0x10: channel-A interpolation position */
    f32 cursorStep; /* 0x14: channel-A position increment */
} EffChan;

/* 0x38-byte keyframe record addressed by effMathGetSlotAt. */
typedef struct EffRec38 {
    u8 unk0[0x30]; /* 0x0 */
    f32 scaledRandomValue; /* 0x30 */
    f32 randomScale;       /* 0x34 */
} EffRec38;

/* Interpolated vertex (x, y, z, w) written by func_00192ED0. */
typedef struct EffVert {
    f32 unk0; /* 0x0 */
    f32 unk4; /* 0x4 */
    f32 unk8; /* 0x8 */
    f32 unkC; /* 0xC */
} EffVert;

/* Emitter handle for effFillRandRecords: target primitive at +0x8. */
typedef struct EffEmit {
    u8 unk0[8];    /* 0x0 */
    EffPrim *primitive; /* 0x8: target primitive */
} EffEmit;

/* Word at D_003D68C0+0x18 (list header defined in game/code_00193C08). */
extern s32 D_003D68D8[];
/* List header defined in game/code_00193C08 (unsized: keeps absolute access). */
extern u8 D_003D68C0[];
extern void func_002D0918(void *arg0);
extern void *func_002D03F8(s32 arg0);
extern void *sdfResourceRetainAddress(void *arg0);
extern void func_00192ED0(EffVert *arg0, EffPrim *arg1, s32 arg2, f32 arg3);
extern void func_001937E0(EffVert *arg0, EffChan *arg1, s32 arg2, f32 arg3);
extern u32 effMiscRand(void *state);
extern u8 D_0034DF38[];
extern s32 effMathGetSlotAt(s32 *arg0, s32 arg1);
extern void func_001926A8(EffPrim *arg0, u32 arg1);
extern void func_001931E0(void *arg0, s32 arg1, u32 arg2);
extern void func_001934E8(EffPrim *arg0, void *arg1);
extern void func_001935B8(EffPrim *arg0, void *arg1);

INCLUDE_ASM(const s32, "game/code_00192488", func_00192488);

INCLUDE_ASM(const s32, "game/code_00192488", func_00192638);

INCLUDE_ASM(const s32, "game/code_00192488", func_001926A8);

INCLUDE_ASM(const s32, "game/code_00192488", func_001929A0);

/* Copy four rows of three coordinates and their separate scalar values. */
void effCopyVertRows(EffChan *channel, f32 *source) {
    f32 *row;
    u32 index;
    f32 *secondary;
    f32 *primary;

    row = source;
    index = 0;
    source += 16;
    secondary = channel->rows->secondary;
    primary = channel->rows->primary;
    do {
        index++;
        primary[0] = row[0];
        primary[1] = row[1];
        primary[2] = row[2];
        row += 4;
        primary += 4;
        *secondary = *source++;
        secondary++;
    } while (index < 4);
}

void effFillRandRecords(EffEmit *emitter) {
    EffPrim *primitive = emitter->primitive;
    u32 modulus = primitive->randomModulus;
    u32 count = primitive->randomCount;
    EffCntRec *record = primitive->counterRecords;
    u32 index = 0;
    EffRec38 *keyframe;
    s32 randomIndex;
    f32 scale;

    if (count == 0) {
        return;
    }
    do {
        func_001926A8(primitive, index);
        record->randomIndex = effMiscRand(&D_0034DF38) % modulus;
        keyframe = (EffRec38 *)effMathGetSlotAt(primitive->unk16C, index);
        index++;
        randomIndex = record->randomIndex;
        scale = keyframe->randomScale;
        record->randomIndex = randomIndex + 1;
        record++;
        keyframe->scaledRandomValue = scale * (f32)randomIndex;
    } while (index < count);
}

INCLUDE_ASM(const s32, "game/code_00192488", func_00192CC8);

void effFreeBuffers(EffPrim *primitive) {
    if (primitive != NULL) {
        if (primitive->unk14 != NULL) {
            func_002D0918(primitive->unk4);
        }
        func_002D0918(primitive->unk0);
    }
}

/* Interpolate the current primitive record, then advance its wrapping cursor. */
s32 effAdvancePrimCursor(void *vertex, EffPrim *primitive) {
    s32 continuing = 1;
    f32 position = primitive->cursorPosition;
    u32 index = primitive->cursorIndex;

    func_00192ED0(vertex, primitive, index, position);
    position += primitive->cursorStep;
    if (position > 1.0f) {
        position -= 1.0f;
        index += 1;
    }
    if (index >= primitive->recordCount - 1) {
        position = 0.0f;
        index = 0;
        continuing = 0;
    }
    primitive->cursorIndex = index;
    primitive->cursorPosition = position;
    return continuing;
}

INCLUDE_ASM(const s32, "game/code_00192488", func_00192ED0);

INCLUDE_ASM(const s32, "game/code_00192488", func_00193000);

void func_00193130(EffPrim *primitive) {
    primitive->cursorIndex = 0;
    primitive->cursorPosition = 0;
}

void func_00193140(EffPrim *primitive, f32 step) {
    primitive->cursorStep = step;
}

/* Build a temporary record array and dispatch it through the selected path. */
void effBuildAndDispatch(EffPrim *primitive, s32 variant) {
    void *allocation = func_002D03F8(primitive->recordCount * 12);
    void *records = sdfResourceRetainAddress(allocation);

    func_001931E0(records, primitive->unk10, primitive->recordCount);
    if (variant == 0) {
        func_001934E8(primitive, records);
    } else {
        func_001935B8(primitive, records);
    }
    func_002D0918(allocation);
}


INCLUDE_ASM(const s32, "game/code_00192488", func_001931E0);

INCLUDE_ASM(const s32, "game/code_00192488", func_00193368);

INCLUDE_ASM(const s32, "game/code_00192488", func_001934E8);

INCLUDE_ASM(const s32, "game/code_00192488", func_001935B8);

/* Create a channel only when there are enough records for interpolation. */
void *effCreateChannel(void *rows, u32 count) {
    void *channel = NULL;
    void *allocation;
    EffChan *cursor;

    if (count < 4) {
        return channel;
    }
    allocation = func_002D03F8(0x18);
    channel = sdfResourceRetainAddress(allocation);
    cursor = channel;
    cursor->unk0 = allocation;
    cursor->cursorStep = 0.05f;
    cursor->recordCount = count;
    cursor->rows = rows;
    cursor->cursorPosition = 0;
    cursor->cursorIndex = 0;
    return channel;
}

s64 func_00193720(u32 *p) {
    if (p != NULL) {
        func_002D0918((void *)*p);
    }
}

/* Interpolate the channel; advance three records when its position wraps. */
s32 effAdvanceChanCursor(void *vertex, EffChan *channel) {
    s32 continuing = 1;
    f32 position = channel->cursorPosition;
    u32 index = channel->cursorIndex;

    func_001937E0(vertex, channel, index, position);
    position += channel->cursorStep;
    if (position > 1.0f) {
        position -= 1.0f;
        index += 3;
    }
    if (index >= channel->recordCount - 1) {
        position = 0.0f;
        index = 0;
        continuing = 0;
    }
    channel->cursorIndex = index;
    channel->cursorPosition = position;
    return continuing;
}

INCLUDE_ASM(const s32, "game/code_00192488", func_001937E0);

void effClearChanCursor(EffChan *arg0) {
    arg0->cursorIndex = 0;
    arg0->cursorPosition = 0;
}

void effSetChanStep(EffChan *arg0, f32 arg1) {
    arg0->cursorStep = arg1;
}

void *effGetFontListHead(void) {
    return D_003D68C0;
}

s32 effGetFontListCount(void) {
    return D_003D68D8[0];
}

INCLUDE_ASM(const s32, "game/code_00192488", func_00193920);

INCLUDE_SDATA(const s32, "game/code_00192488", D_003BB150);

INCLUDE_SDATA(const s32, "game/code_00192488", D_003BB15C);

