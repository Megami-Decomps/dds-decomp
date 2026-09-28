#include "common.h"

/* Small channel object (0x18 bytes, created by func_001936A8): float block
 * plus the channel-A cursor (count at +0x4, index at +0xC). */
typedef struct EffChan {
    void *unk0; /* 0x0: mem handle */
    u32 unk4;   /* 0x4: count advanced by func_00193740 */
    void *unk8; /* 0x8: float block copied by func_00192BA0 */
    u32 unkC;   /* 0xC: channel-A index advanced by func_00193740 */
    f32 unk10;  /* 0x10: channel-A position advanced by func_00193740 */
    f32 unk14;  /* 0x14: channel-A step (0.05f) */
} EffChan;

/* Primary work object (created by func_00192CC8): owned buffers plus the
 * channel-B cursor (count at +0x8, index at +0x20). Extends past 0x2C with
 * the rand/emit control words used by func_00192C00. */
typedef struct EffPrim {
    void *unk0;       /* 0x0: buffer freed by func_00192DE0 */
    void *unk4;       /* 0x4: buffer freed by func_00192DE0 */
    u32 unk8;         /* 0x8: count advanced by func_00192E30 */
    u16 unkC;         /* 0xC: flag set by func_00192CC8 */
    u8 unkE[2];       /* 0xE */
    s32 unk10;        /* 0x10 */
    void *unk14;      /* 0x14: buffer or NULL, tested by func_00192DE0 */
    void *unk18;      /* 0x18 */
    void *unk1C;      /* 0x1C */
    u32 unk20;        /* 0x20: channel-B index advanced by func_00192E30 */
    f32 unk24;        /* 0x24: channel-B position advanced by func_00192E30 */
    f32 unk28;        /* 0x28: channel-B step (0.05f) */
    u8 unk2C[0x18];   /* 0x2C */
    u32 unk44;        /* 0x44: rand count for func_00192C00 */
    u32 unk48;        /* 0x48: rand modulus for func_00192C00 */
    u8 unk4C[0x11C];  /* 0x4C */
    struct EffCntRec *unk168; /* 0x168: counter records */
    s32 *unk16C;      /* 0x16C: base for func_0018E200 */
} EffPrim;

/* 8-byte counter record at EffPrim.unk168. */
typedef struct EffCntRec {
    s32 unk0; /* 0x0: rand slot advanced by func_00192C00 */
    u32 unk4; /* 0x4: id released by func_00192638 */
} EffCntRec;

/* 0x38-byte keyframe record addressed by func_0018E200. */
typedef struct EffRec38 {
    u8 unk0[0x30]; /* 0x0 */
    f32 unk30;     /* 0x30: scaled by func_00192C00 */
    f32 unk34;     /* 0x34: read by func_00192C00 */
} EffRec38;

/* Emitter handle for func_00192C00: target primitive at +0x8. */
typedef struct EffEmit {
    u8 unk0[8];    /* 0x0 */
    EffPrim *unk8; /* 0x8: target primitive */
} EffEmit;

extern u32 effMiscRand(void *state);

extern u8 D_003AA868[];

extern s32 func_00195E38(s32 *arg0, s32 arg1);

extern void func_0019A2E0(EffPrim *arg0, u32 arg1);

extern void func_003297C8(void *arg0);

/* Interpolated vertex (x, y, z, w) written by func_00192ED0. */
typedef struct EffVert {
    f32 unk0; /* 0x0 */
    f32 unk4; /* 0x4 */
    f32 unk8; /* 0x8 */
    f32 unkC; /* 0xC */
} EffVert;

extern void func_0019AB08(EffVert *arg0, EffPrim *arg1, s32 arg2, f32 arg3);

extern void *func_003292A8(s32 arg0);

extern void *func_003298F8(void *arg0);

extern void func_0019B418(EffVert *arg0, EffChan *arg1, s32 arg2, f32 arg3);

/* List header defined in game/code_00193C08 (unsized: keeps absolute access). */
extern u8 D_00452360[];

/* Word at D_003D68C0+0x18 (list header defined in game/code_00193C08). */
extern s32 D_00452378[];

INCLUDE_ASM(const s32, "game/code_0019A0C0", func_0019A0C0);

INCLUDE_ASM(const s32, "game/code_0019A0C0", func_0019A270);

INCLUDE_ASM(const s32, "game/code_0019A0C0", func_0019A2E0);

INCLUDE_ASM(const s32, "game/code_0019A0C0", func_0019A5D8);

void func_0019A7D8(EffChan *dst, f32 *src) {
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

void func_0019A838(EffEmit *arg0) {
    EffPrim *e = arg0->unk8;
    u32 mod = e->unk48;
    u32 count = e->unk44;
    EffCntRec *r = e->unk168;
    u32 i = 0;
    EffRec38 *rec;
    s32 t;
    f32 f;

    if (count == 0) {
        return;
    }
    do {
        func_0019A2E0(e, i);
        r->unk0 = effMiscRand(&D_003AA868) % mod;
        rec = (EffRec38 *)func_00195E38(e->unk16C, i);
        i++;
        t = r->unk0;
        f = rec->unk34;
        r->unk0 = t + 1;
        r++;
        rec->unk30 = f * (f32)t;
    } while (i < count);
}

INCLUDE_ASM(const s32, "game/code_0019A0C0", func_0019A900);

void func_0019AA18(EffPrim *arg0) {
    if (arg0 != NULL) {
        if (arg0->unk14 != NULL) {
            func_003297C8(arg0->unk4);
        }
        func_003297C8(arg0->unk0);
    }
}

s32 func_0019AA68(void *arg0, EffPrim *arg1) {
    s32 ret = 1;
    f32 pos = arg1->unk24;
    u32 idx = arg1->unk20;

    func_0019AB08(arg0, arg1, idx, pos);
    pos += arg1->unk28;
    if (pos > 1.0f) {
        pos -= 1.0f;
        idx += 1;
    }
    if (idx >= arg1->unk8 - 1) {
        pos = 0.0f;
        idx = 0;
        ret = 0;
    }
    arg1->unk20 = idx;
    arg1->unk24 = pos;
    return ret;
}

INCLUDE_ASM(const s32, "game/code_0019A0C0", func_0019AB08);

INCLUDE_ASM(const s32, "game/code_0019A0C0", func_0019AC38);

INCLUDE_ASM(const s32, "game/code_0019A0C0", func_0019AD68);

INCLUDE_ASM(const s32, "game/code_0019A0C0", func_0019AD78);

INCLUDE_ASM(const s32, "game/code_0019A0C0", func_0019AD80);

INCLUDE_ASM(const s32, "game/code_0019A0C0", func_0019AE18);

INCLUDE_ASM(const s32, "game/code_0019A0C0", func_0019AFA0);

INCLUDE_ASM(const s32, "game/code_0019A0C0", func_0019B120);

INCLUDE_ASM(const s32, "game/code_0019A0C0", func_0019B1F0);

void *func_0019B2E0(void *arg0, u32 arg1) {
    void *buf = NULL;
    void *mem;
    EffChan *p;

    if (arg1 < 4) {
        return buf;
    }
    mem = func_003292A8(0x18);
    buf = func_003298F8(mem);
    p = buf;
    p->unk0 = mem;
    p->unk14 = 0.05f;
    p->unk4 = arg1;
    p->unk8 = arg0;
    p->unk10 = 0;
    p->unkC = 0;
    return buf;
}

INCLUDE_ASM(const s32, "game/code_0019A0C0", func_0019B358);

s32 func_0019B378(void *arg0, EffChan *arg1) {
    s32 ret = 1;
    f32 pos = arg1->unk10;
    u32 idx = arg1->unkC;

    func_0019B418(arg0, arg1, idx, pos);
    pos += arg1->unk14;
    if (pos > 1.0f) {
        pos -= 1.0f;
        idx += 3;
    }
    if (idx >= arg1->unk4 - 1) {
        pos = 0.0f;
        idx = 0;
        ret = 0;
    }
    arg1->unkC = idx;
    arg1->unk10 = pos;
    return ret;
}

INCLUDE_ASM(const s32, "game/code_0019A0C0", func_0019B418);

void func_0019B520(EffChan *arg0) {
    arg0->unkC = 0;
    arg0->unk10 = 0;
}

void func_0019B530(EffChan *arg0, f32 arg1) {
    arg0->unk14 = arg1;
}

void *func_0019B538(void) {
    return D_00452360;
}

s32 func_0019B548(void) {
    return D_00452378[0];
}

INCLUDE_ASM(const s32, "game/code_0019A0C0", func_0019B558);

INCLUDE_SDATA(const s32, "game/code_0019A0C0", D_00436540);

INCLUDE_SDATA(const s32, "game/code_0019A0C0", D_0043654C);

