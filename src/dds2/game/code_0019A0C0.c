#include "common.h"
#include "eff.h"

/* Small channel object (0x18 bytes, created by func_001936A8): float block
 * plus the channel-A cursor (count at +0x4, index at +0xC). */
typedef struct EffFloatRows {
    f32 primary[16];
    u8 pad40[0x18];
    f32 secondary[4];
} EffFloatRows;

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

/* Emitter handle for effFillRandRecords: target primitive at +0x8. */
typedef struct EffEmit {
    u8 unk0[8];    /* 0x0 */
    EffPrim *primitive; /* 0x8: target primitive */
} EffEmit;

extern u32 effMiscRand(void *state);

extern u8 D_003AA868[];

extern s32 effMathGetSlotAt(s32 *arg0, s32 arg1);

extern void func_0019A2E0(EffPrim *arg0, u32 arg1);

/* Interpolated vertex (x, y, z, w) written by func_00192ED0. */
typedef struct EffVert {
    f32 unk0; /* 0x0 */
    f32 unk4; /* 0x4 */
    f32 unk8; /* 0x8 */
    f32 unkC; /* 0xC */
} EffVert;

extern void func_0019AB08(EffVert *arg0, EffPrim *arg1, s32 arg2, f32 arg3);

extern void *func_003292A8(s32 arg0);

extern void *sdfResourceRetainAddress(void *arg0);

extern void func_0019B418(EffVert *arg0, EffChan *arg1, s32 arg2, f32 arg3);

/* List header defined in game/code_00193C08 (unsized: keeps absolute access). */
extern u8 frFontResourceList[];

/* Word at D_003D68C0+0x18 (list header defined in game/code_00193C08). */
extern s32 D_00452378[];

extern void func_003297C8(void *arg0);

extern void func_0019AE18(void *arg0, s32 arg1, u32 arg2);

extern void func_0019B120(EffPrim *arg0, void *arg1);

extern void func_0019B1F0(EffPrim *arg0, void *arg1);
extern void effMathReleaseWorkResource(void *work);
extern void effDispatchParameterDataAndFreeWork(void *handle);

/* Header block copied into every channel work (0x168 bytes): the random record count and modulus live inside it. */
typedef struct EffChanHead {
    u8 pad00[0x44];
    u32 count;      /* 0x44: number of random records */
    u8 pad48[4];
    s32 spread;     /* 0x4C: modulus of the start delay */
    u8 pad50[0x118];
} EffChanHead; /* 0x168 */

typedef struct EffChanRecord {
    s32 delay;      /* 0x00 */
    void *param;    /* 0x04 */
} EffChanRecord; /* 0x8 */

typedef struct EffChanSourceOwner {
    u8 pad00[4];
    void *param;    /* 0x04 */
} EffChanSourceOwner;

typedef struct EffChanSource {
    EffChanHead head;
    EffChanSourceOwner *owner; /* 0x168 */
} EffChanSource;

typedef struct EffChanWork {
    EffChanHead head;
    EffChanRecord *records; /* 0x168 */
    s32 *slots;             /* 0x16C */
    void *buffer;           /* 0x170 */
} EffChanWork;

extern void *effAllocSlotArray(u32 count);
extern void *effParamWorkDuplicate(void *param);

/* Create a channel work: clone the header, allocate the slot array, then give every record a duplicated parameter and a random negative start delay. */
EffChanWork *effChanWorkCreate(EffChanSource *src) {
    u32 count = src->head.count;
    void *handle = func_003292A8(count * sizeof(EffChanRecord) + sizeof(EffChanWork));
    EffChanWork *work = sdfResourceRetainAddress(handle);
    EffChanRecord *record = (EffChanRecord *)(work + 1);
    void *param;
    s32 spread;
    u32 i;

    work->head = src->head;
    work->buffer = handle;
    work->records = record;
    if (work->head.spread <= 0) {
        work->head.spread = 1;
    }
    work->slots = effAllocSlotArray(count);
    if (count != 0) {
        param = src->owner->param;
        spread = work->head.spread;
        for (i = 0; i < count; i++) {
            record->param = effParamWorkDuplicate(param);
            record->delay = -(effMiscRand(D_003AA868) % spread);
            record++;
        }
    }
    return work;
}

void func_0019A270(EffChanWork *work) {
    EffChanRecord *record;
    u32 i = 0;
    u32 count;

    effMathReleaseWorkResource(work->slots);
    count = work->head.count;
    record = work->records;
    if (count != 0) {
        do {
            effDispatchParameterDataAndFreeWork(record->param);
            record++;
            i++;
        } while (i < count);
    }
    func_003297C8(work->buffer);
}

INCLUDE_ASM(const s32, "game/code_0019A0C0", func_0019A2E0);

INCLUDE_ASM(const s32, "game/code_0019A0C0", func_0019A5D8);

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
        func_0019A2E0(primitive, index);
        record->randomIndex = effMiscRand(&D_003AA868) % modulus;
        keyframe = (EffRec38 *)effMathGetSlotAt(primitive->unk16C, index);
        index++;
        randomIndex = record->randomIndex;
        scale = keyframe->randomScale;
        record->randomIndex = randomIndex + 1;
        record++;
        keyframe->scaledRandomValue = scale * (f32)randomIndex;
    } while (index < count);
}

INCLUDE_ASM(const s32, "game/code_0019A0C0", func_0019A900);

/* Release the primitive's owned buffers, if a primitive was supplied. */
void effFreeBuffers(EffPrim *primitive) {
    if (primitive != NULL) {
        if (primitive->unk14 != NULL) {
            func_003297C8(primitive->unk4);
        }
        func_003297C8(primitive->unk0);
    }
}

/* Interpolate the current primitive record, then advance its wrapping cursor. */
s32 effAdvancePrimCursor(void *vertex, EffPrim *primitive) {
    s32 continuing = 1;
    f32 position = primitive->cursorPosition;
    u32 index = primitive->cursorIndex;

    func_0019AB08(vertex, primitive, index, position);
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

INCLUDE_ASM(const s32, "game/code_0019A0C0", func_0019AB08);

INCLUDE_ASM(const s32, "game/code_0019A0C0", func_0019AC38);

void effResetPrimitiveRecordCursor(EffPrim *primitive) {
    primitive->cursorIndex = 0;
    primitive->cursorPosition = 0.0f;
}

void effSetPrimitiveRecordCursorStep(EffPrim *primitive, f32 step) {
    primitive->cursorStep = step;
}

/* Build a temporary record array and dispatch it through the selected path. */
void effBuildAndDispatch(EffPrim *primitive, s32 variant) {
    void *allocation = func_003292A8(primitive->recordCount * 12);
    void *records = sdfResourceRetainAddress(allocation);

    func_0019AE18(records, primitive->unk10, primitive->recordCount);
    if (variant == 0) {
        func_0019B120(primitive, records);
    } else {
        func_0019B1F0(primitive, records);
    }
    func_003297C8(allocation);
}

INCLUDE_ASM(const s32, "game/code_0019A0C0", func_0019AE18);

INCLUDE_ASM(const s32, "game/code_0019A0C0", func_0019AFA0);

INCLUDE_ASM(const s32, "game/code_0019A0C0", func_0019B120);

INCLUDE_ASM(const s32, "game/code_0019A0C0", func_0019B1F0);

/* Create a channel only when there are enough records for interpolation. */
void *effCreateChannel(void *rows, u32 count) {
    void *channel = NULL;
    void *allocation;
    EffChan *cursor;

    if (count < 4) {
        return channel;
    }
    allocation = func_003292A8(0x18);
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

INCLUDE_ASM(const s32, "game/code_0019A0C0", effReleaseInterpolationChannel);

/* Interpolate the channel; advance three records when its position wraps. */
s32 effAdvanceChanCursor(void *vertex, EffChan *channel) {
    s32 continuing = 1;
    f32 position = channel->cursorPosition;
    u32 index = channel->cursorIndex;

    func_0019B418(vertex, channel, index, position);
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

INCLUDE_ASM(const s32, "game/code_0019A0C0", func_0019B418);

void effClearChanCursor(EffChan *chan) {
    chan->cursorIndex = 0;
    chan->cursorPosition = 0;
}

void effSetChanStep(EffChan *chan, f32 step) {
    chan->cursorStep = step;
}

void *effGetFontListHead(void) {
    return frFontResourceList;
}

s32 effGetFontListCount(void) {
    return D_00452378[0];
}

INCLUDE_ASM(const s32, "game/code_0019A0C0", func_0019B558);

INCLUDE_SDATA(const s32, "game/code_0019A0C0", D_00436540);

INCLUDE_SDATA(const s32, "game/code_0019A0C0", D_0043654C);

