#include "common.h"

/* Primary work object (created by func_00192CC8): owned buffers plus the
 * channel-B cursor (count at +0x8, index at +0x20). Extends past 0x2C with
 * the rand/emit control words used by effFillRandRecords. */
typedef struct EffPrim {
    void *unk0;       /* 0x0: buffer freed by effFreeBuffers */
    void *unk4;       /* 0x4: buffer freed by effFreeBuffers */
    u32 recordCount; /* 0x8: number of keyframe records */
    u16 unkC;         /* 0xC: flag set by func_00192CC8 */
    u8 unkE[2];       /* 0xE */
    s32 unk10;        /* 0x10 */
    void *unk14;      /* 0x14: buffer or NULL, tested by effFreeBuffers */
    void *unk18;      /* 0x18 */
    void *unk1C;      /* 0x1C */
    u32 cursorIndex;  /* 0x20: channel-B record index */
    f32 cursorPosition; /* 0x24: channel-B interpolation position */
    f32 cursorStep;  /* 0x28: channel-B position increment */
    u8 unk2C[0x18];   /* 0x2C */
    u32 randomCount;   /* 0x44: number of random records */
    u32 randomModulus; /* 0x48: modulus for each random slot */
    u8 unk4C[0x11C];  /* 0x4C */
    struct EffCntRec *counterRecords; /* 0x168 */
    s32 *unk16C;      /* 0x16C: base for effMathGetSlotAt */
} EffPrim;

/* Small channel object (0x18 bytes, created by effCreateChannel): float block
 * plus the channel-A cursor (count at +0x4, index at +0xC). */
typedef struct EffChan {
    void *unk0; /* 0x0: mem handle */
    u32 recordCount; /* 0x4: number of keyframe records */
    void *unk8; /* 0x8: float block copied by effCopyVertRows */
    u32 cursorIndex; /* 0xC: channel-A record index */
    f32 cursorPosition; /* 0x10: channel-A interpolation position */
    f32 cursorStep; /* 0x14: channel-A position increment */
} EffChan;

/* 8-byte counter record at EffPrim.counterRecords. */
typedef struct EffCntRec {
    s32 randomIndex; /* 0x0: advanced by effFillRandRecords */
    u32 unk4; /* 0x4: id released by func_00192638 */
} EffCntRec;

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

void effCopyVertRows(EffChan *dst, f32 *src) {
    f32 *s;
    u32 i;
    f32 *d2;
    f32 *d1;

    s = src;
    i = 0;
    src += 16;
    d2 = (f32 *)((u8 *)dst->unk8 + 0x58);
    d1 = dst->unk8;
    do {
        i++;
        d1[0] = s[0];
        d1[1] = s[1];
        d1[2] = s[2];
        s += 4;
        d1 += 4;
        *d2 = *src++;
        d2++;
    } while (i < 4);
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

s32 effAdvancePrimCursor(void *arg0, EffPrim *arg1) {
    s32 ret = 1;
    f32 pos = arg1->cursorPosition;
    u32 idx = arg1->cursorIndex;

    func_00192ED0(arg0, arg1, idx, pos);
    pos += arg1->cursorStep;
    if (pos > 1.0f) {
        pos -= 1.0f;
        idx += 1;
    }
    if (idx >= arg1->recordCount - 1) {
        pos = 0.0f;
        idx = 0;
        ret = 0;
    }
    arg1->cursorIndex = idx;
    arg1->cursorPosition = pos;
    return ret;
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

void effBuildAndDispatch(EffPrim *arg0, s32 arg1) {
    void *mem = func_002D03F8(arg0->recordCount * 12);
    void *buf = sdfResourceRetainAddress(mem);

    func_001931E0(buf, arg0->unk10, arg0->recordCount);
    if (arg1 == 0) {
        func_001934E8(arg0, buf);
    } else {
        func_001935B8(arg0, buf);
    }
    func_002D0918(mem);
}


INCLUDE_ASM(const s32, "game/code_00192488", func_001931E0);

INCLUDE_ASM(const s32, "game/code_00192488", func_00193368);

INCLUDE_ASM(const s32, "game/code_00192488", func_001934E8);

INCLUDE_ASM(const s32, "game/code_00192488", func_001935B8);

void *effCreateChannel(void *arg0, u32 arg1) {
    void *buf = NULL;
    void *mem;
    EffChan *p;

    if (arg1 < 4) {
        return buf;
    }
    mem = func_002D03F8(0x18);
    buf = sdfResourceRetainAddress(mem);
    p = buf;
    p->unk0 = mem;
    p->cursorStep = 0.05f;
    p->recordCount = arg1;
    p->unk8 = arg0;
    p->cursorPosition = 0;
    p->cursorIndex = 0;
    return buf;
}

s64 func_00193720(u32 *p) {
    if (p != NULL) {
        func_002D0918((void *)*p);
    }
}

s32 effAdvanceChanCursor(void *arg0, EffChan *arg1) {
    s32 ret = 1;
    f32 pos = arg1->cursorPosition;
    u32 idx = arg1->cursorIndex;

    func_001937E0(arg0, arg1, idx, pos);
    pos += arg1->cursorStep;
    if (pos > 1.0f) {
        pos -= 1.0f;
        idx += 3;
    }
    if (idx >= arg1->recordCount - 1) {
        pos = 0.0f;
        idx = 0;
        ret = 0;
    }
    arg1->cursorIndex = idx;
    arg1->cursorPosition = pos;
    return ret;
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

