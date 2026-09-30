#include "common.h"
#include "pcp_vu0.h"

/* Polygon node with an f32 scale pair and a resource handle at 0xDC.
   (Retail lwc1 at +0xDC belongs to the PolyQuad flavor below, whose
   +0xDC is a float, so the two layouts are distinct node types.) */
typedef struct {
    u8 pad[0xCC]; /* 0x0 */
    f32 unkCC;    /* 0xCC scaled by func_0015DA80/func_0015F2B0/func_0015EBF8 */
    f32 unkD0;    /* 0xD0 scaled by func_0015DA80/func_0015F2B0 */
    u8 padD4[8];  /* 0xD4 */
    u32 unkDC;    /* 0xDC handle released by effPolyDestroyWork/func_0015B918 */
} PolyNode;

/* Node of four f32s scaled together by effPolyScaleFourComponents. */
typedef struct {
    u8 pad[0xC8]; /* 0x0 */
    f32 unkC8;    /* 0xC8 */
    f32 unkCC;    /* 0xCC */
    f32 unkD0;    /* 0xD0 */
    u8 padD4[8];  /* 0xD4 */
    f32 unkDC;    /* 0xDC */
} PolyQuad;

/* Three further node flavors, each destructor releasing its own pair. */
typedef struct {
    u8 pad[0xE0]; /* 0x0 */
    u32 unkE0;    /* 0xE0 */
    u8 padE4[4];  /* 0xE4 */
    void *unkE8;  /* 0xE8 released by func_0015E888 */
} PolyNodeE0;

typedef struct {
    u8 pad[0xF0]; /* 0x0 */
    u32 unkF0;    /* 0xF0 */
    u8 padF4[4];  /* 0xF4 */
    void *unkF8;  /* 0xF8 released by func_0015E0D0 */
} PolyNodeF0;

typedef struct {
    u8 pad[0xF4]; /* 0x0 */
    u32 unkF4;    /* 0xF4 */
    u8 padF8[4];  /* 0xF8 */
    void *unkFC;  /* 0xFC released by func_0015EEC0 */
} PolyNodeF4;

typedef struct {
    s32 value; /* 0x0: inactive sentinel -0xFFFFFF is preserved on reset */
    s32 unk4;  /* 0x4 */
    s32 unk8;  /* 0x8 */
    s32 unkC;  /* 0xC */
    s32 unk10; /* 0x10 */
} PolyEntry; /* 0x14 bytes */

typedef struct {
    u8 pad[0x10];      /* 0x0 */
    u32 entryCount;     /* 0x10 */
    u32 unk14;         /* 0x14 */
    u8 pad18[0x50];    /* 0x18 */
    u32 unk68;         /* 0x68 */
    u32 unk6C;         /* 0x6C */
    u8 pad70[0x50];    /* 0x70 */
    u32 unkC0;         /* 0xC0 step subtracted by func_0015E100/func_0015E8B8 */
    u8 padC4[0x18];    /* 0xC4 */
    u32 unkDC;         /* 0xDC divisor read by func_0015EEF0 */
    u8 padE0[4];       /* 0xE0 */
    u32 *unkE4;        /* 0xE4 stepped by func_0015E8B8 */
    u8 padE8[0xC];     /* 0xE8 */
    u32 *unkF4;        /* 0xF4 stepped by func_0015E100 */
    PolyEntry *entries; /* 0xF8 */
} PolyList;

/* Point buffer of one strip; `count` is copied from the strip's own count. */
typedef struct {
    u32 points;   /* 0x0 */
    u32 unk4;     /* 0x4 */
    s32 count;    /* 0x8 */
    u32 unkC;     /* 0xC */
    u32 unk10;    /* 0x10 */
} PolyStripEntry; /* 0x14 bytes */

typedef struct {
    u8 pad00[8];             /* 0x0 */
    s32 count;               /* 0x8 */
    u8 pad0C[8];             /* 0xC */
    PolyStripEntry *entries; /* 0x14 */
} PolyStrip;

void func_0015B8B8(u32 arg);
void func_0015B918(u32 arg);
void func_0015DAA0(void);
void func_002CFF98(void *arg);
void func_002D0918(void *arg);
extern f32 func_002E78F8(f32 angle);
extern f32 sdfSinPoly(f32 angle);
extern void func_002DD608(f32 angle);
extern void func_002DD968(f32 angle);
extern void func_002DDC50(void);

void effPolyDestroyWork(PolyNode *obj) {
    func_0015B8B8(obj->unkDC);
    func_002CFF98(obj);
}

INCLUDE_ASM(const s32, "effect/polyManager", func_0015DA10);

void func_0015DA80(f32 scale, PolyNode *obj) {
    obj->unkCC = obj->unkCC * scale;
    obj->unkD0 = obj->unkD0 * scale;
}

INCLUDE_ASM(const s32, "effect/polyManager", func_0015DAA0);

void func_0015DC48(PolyNode *obj) {
    func_0015DAA0();
    func_0015B918(obj->unkDC);
}

INCLUDE_ASM(const s32, "effect/polyManager", func_0015DC70);

/* Push each point pair of the strip entry apart along its own direction, by the node's scale. */
void func_0015DDC8(PolyNode *obj, s32 index) {
    PolyStrip *strip = (PolyStrip *)obj->unkDC;
    PolyStripEntry *entry = &strip->entries[index];
    f32 scale[4];
    f32 *p;
    s32 pairs;
    s32 i;

    p = (f32 *)entry->points;
    pairs = strip->count / 2;
    scale[0] = scale[1] = scale[2] = obj->unkD0;
    VU0_LOAD_VF(vf12, scale);
    for (i = 0; i < pairs; i++) {
        VU0_LOAD_VF(vf10, p + 4);
        VU0_LOAD_VF(vf11, p);
        VU0_SUB(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10();
        VU0_MOVE_VF(vf11, vf12);
        VU0_MUL(vf10, vf10, vf11);
        VU0_MOVE_VF(vf11, vf10);
        VU0_LOAD_VF(vf10, p);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, p);
        VU0_LOAD_VF(vf10, p + 4);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, p + 4);
        p += 8;
    }
}

INCLUDE_ASM(const s32, "effect/polyManager", func_0015DE88);

INCLUDE_ASM(const s32, "effect/polyManager", func_0015DFA8);

void func_0015E0D0(PolyNodeF0 *obj) {
    func_0015B8B8(obj->unkF0);
    func_002D0918(obj->unkF8);
}

INCLUDE_ASM(const s32, "effect/polyManager", func_0015E100);

INCLUDE_ASM(const s32, "effect/polyManager", func_0015E148);

#define VEC3_SPLAT(v, x) ((v)[0] = (x), (v)[1] = (x), (v)[2] = (x))

/* Band node: an origin, a transform, the ring's segment count and a strip at 0xF0. */
typedef struct {
    f32 origin[4];      /* 0x0 */
    u8 pad10[0x10];     /* 0x10 */
    f32 matrix[16];     /* 0x20 */
    u8 pad60[0x64];     /* 0x60 */
    u32 segments;       /* 0xC4 */
    f32 unkC8;          /* 0xC8 */
    u8 padCC[0x24];     /* 0xCC */
    PolyStrip *strip;   /* 0xF0 */
} PolyBand;

/* Lay the band's point pairs of strip entry `index` around the ring: an inner and an outer radius, moved to the origin. */
void func_0015E238(PolyBand *obj, s32 index, f32 width)
{
    PolyStrip *strip = obj->strip;
    PolyStripEntry *entry = &strip->entries[index];
    f32 dir[4];
    f32 wide[4];
    f32 narrow[4];
    f32 step;
    f32 angle;
    f32 *out;
    f32 *first;
    s32 pairs;
    s32 i;

    entry->count = strip->count;
    out = (f32 *)entry->points;
    pairs = strip->count / 2;
    VEC3_SPLAT(wide, width);
    VEC3_SPLAT(narrow, width + obj->unkC8);
    step = 3.14159265f * 2.0f / (f32)obj->segments;
    VU0_LOAD_MATRIX(obj->matrix);
    angle = 0.0f;
    VU0_LOAD_VF(vf12, obj->origin);
    for (i = 0; i < pairs - 1; i++) {
        dir[0] = func_002E78F8(angle);
        dir[1] = 0;
        dir[2] = sdfSinPoly(angle);
        VU0_LOAD_VF(vf10, dir);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, dir);
        VU0_LOAD_VF(vf11, wide);
        VU0_MUL(vf10, vf10, vf11);
        VU0_ADD(vf10, vf10, vf12);
        VU0_STORE_VF(vf10, out + 4);
        VU0_LOAD_VF(vf10, dir);
        VU0_LOAD_VF(vf11, narrow);
        VU0_MUL(vf10, vf10, vf11);
        VU0_ADD(vf10, vf10, vf12);
        VU0_STORE_VF(vf10, out);
        out += 8;
        angle += step;
    }
    first = (f32 *)entry->points;
    PCP_COPY_VECTOR(out, first);
    PCP_COPY_VECTOR(out + 4, first + 4);
}
#undef VEC3_SPLAT

typedef struct {
    u32 unk0;
    f32 width;
} PolyRing2Rec; /* 8 bytes */

typedef struct {
    f32 origin[4];      /* 0x0 */
    u8 pad10[0x10];     /* 0x10 */
    f32 matrix[16];     /* 0x20 */
    u8 pad60[0x64];     /* 0x60 */
    u32 segments;       /* 0xC4 */
    u8 padC8[0x14];     /* 0xC8 */
    f32 lift;           /* 0xDC */
    u8 padE0[0x10];     /* 0xE0 */
    PolyStrip *strip;   /* 0xF0 */
    PolyRing2Rec *recs; /* 0xF4 */
} PolyRing2;

/* Lay a ring of point pairs for strip entry `index`, scaled per axis and lifted along y. */
void func_0015E3D8(PolyRing2 *obj, s32 index) {
    PolyStrip *strip = obj->strip;
    PolyRing2Rec *rec = &obj->recs[index];
    PolyStripEntry *entry = &strip->entries[index];
    f32 dir[4];
    f32 scale[4];
    f32 lift[4];
    f32 step;
    f32 angle;
    f32 *out;
    f32 *first;
    s32 pairs;
    s32 i;

    entry->count = strip->count;
    out = (f32 *)entry->points;
    pairs = strip->count >> 1;
    scale[2] = scale[1] = scale[0] = rec->width;
    lift[1] = obj->lift;
    lift[2] = lift[0] = 0;
    step = 3.14159265f * 2.0f / (f32)obj->segments;
    VU0_LOAD_MATRIX(obj->matrix);
    angle = 0.0f;
    for (i = 0; i < pairs - 1; i++) {
        dir[0] = func_002E78F8(angle);
        dir[1] = 0;
        dir[2] = sdfSinPoly(angle);
        VU0_LOAD_VF(vf10, dir);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_LOAD_VF(vf11, scale);
        VU0_MUL(vf10, vf10, vf11);
        VU0_LOAD_VF(vf11, lift);
        VU0_APPLY_MATRIX(vf11, vf11);
        VU0_ADD(vf10, vf10, vf11);
        VU0_MOVE_VF(vf12, vf10);
        VU0_LOAD_VF(vf11, out + 4);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, out + 4);
        VU0_MOVE_VF(vf10, vf12);
        VU0_LOAD_VF(vf11, out);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, out);
        out += 8;
        angle += step;
    }
    first = (f32 *)entry->points;
    PCP_COPY_VECTOR(out, first);
    PCP_COPY_VECTOR(out + 4, first + 4);
}

void effPolyScaleFourComponents(f32 scale, PolyQuad *obj) {
    obj->unkC8 = obj->unkC8 * scale;
    obj->unkDC = obj->unkDC * scale;
    obj->unkCC = obj->unkCC * scale;
    obj->unkD0 = obj->unkD0 * scale;
}

INCLUDE_ASM(const s32, "effect/polyManager", func_0015E5D8);

INCLUDE_ASM(const s32, "effect/polyManager", func_0015E760);

void func_0015E888(PolyNodeE0 *obj) {
    func_0015B8B8(obj->unkE0);
    func_002D0918(obj->unkE8);
}

INCLUDE_ASM(const s32, "effect/polyManager", func_0015E8B8);

INCLUDE_ASM(const s32, "effect/polyManager", func_0015E900);

INCLUDE_ASM(const s32, "effect/polyManager", func_0015E9A0);

void func_0015EBF8(f32 scale, PolyNode *obj) {
    obj->unkCC = obj->unkCC * scale;
}

INCLUDE_ASM(const s32, "effect/polyManager", func_0015EC08);

INCLUDE_ASM(const s32, "effect/polyManager", func_0015ED90);

void func_0015EEC0(PolyNodeF4 *obj) {
    func_0015B8B8(obj->unkF4);
    func_002D0918(obj->unkFC);
}

INCLUDE_ASM(const s32, "effect/polyManager", func_0015EEF0);

INCLUDE_ASM(const s32, "effect/polyManager", func_0015EF50);

#define VEC3_SPLAT(v, x) ((v)[0] = (x), (v)[1] = (x), (v)[2] = (x))

typedef struct {
    u8 pad00[4];
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    f32 unk10;
} PolyRec; /* 0x14 bytes */

/* Band node with its own rotation records at 0xF8 and a strip at 0xF4. */
typedef struct {
    f32 origin[4];      /* 0x0 */
    u8 pad10[0x10];     /* 0x10 */
    f32 matrix[16];     /* 0x20 */
    u8 pad60[0x64];     /* 0x60 */
    u32 segments;       /* 0xC4 */
    f32 unkC8;          /* 0xC8 */
    u8 padCC[0x18];     /* 0xCC */
    f32 unkE4;          /* 0xE4 */
    u8 padE8[0xC];      /* 0xE8 */
    PolyStrip *strip;   /* 0xF4 */
    PolyRec *recs;      /* 0xF8 */
} PolyBandC;

/* Same ring as func_0015E238, but the record's rotation is applied through the second matrix bank first. */
void func_0015F0C0(PolyBandC *obj, s32 index)
{
    PolyStrip *strip = obj->strip;
    PolyRec *rec = &obj->recs[index];
    PolyStripEntry *entry = &strip->entries[index];
    f32 dir[4];
    f32 wide[4];
    f32 narrow[4];
    f32 step;
    f32 angle;
    f32 *out;
    f32 *first;
    s32 pairs;
    s32 i;

    entry->count = strip->count;
    out = (f32 *)entry->points;
    pairs = strip->count >> 1;
    func_002DD608(rec->unk0C);
    func_002DD968(rec->unk10);
    func_002DDC50();
    rec->unk10 += obj->unkE4 * (3.14159265f / 180.0f);
    angle = rec->unk04;
    rec->unk04 = angle + rec->unk08;
    step = 3.14159265f * 2.0f / (f32)obj->segments;
    VU0_LOAD_MATRIX_B(obj->matrix);
    func_002DDC50();
    VEC3_SPLAT(wide, angle);
    VEC3_SPLAT(narrow, angle + obj->unkC8);
    angle = 0.0f;
    VU0_LOAD_VF(vf12, obj->origin);
    for (i = 0; i < pairs - 1; i++) {
        dir[0] = func_002E78F8(angle);
        dir[1] = 0;
        dir[2] = sdfSinPoly(angle);
        VU0_LOAD_VF(vf10, dir);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, dir);
        VU0_LOAD_VF(vf11, wide);
        VU0_MUL(vf10, vf10, vf11);
        VU0_ADD(vf10, vf10, vf12);
        VU0_STORE_VF(vf10, out + 4);
        VU0_LOAD_VF(vf10, dir);
        VU0_LOAD_VF(vf11, narrow);
        VU0_MUL(vf10, vf10, vf11);
        VU0_ADD(vf10, vf10, vf12);
        VU0_STORE_VF(vf10, out);
        out += 8;
        angle += step;
    }
    first = (f32 *)entry->points;
    PCP_COPY_VECTOR(out, first);
    PCP_COPY_VECTOR(out + 4, first + 4);
}
#undef VEC3_SPLAT

void func_0015F2B0(f32 scale, PolyNode *obj) {
    obj->unkCC = obj->unkCC * scale;
    obj->unkD0 = obj->unkD0 * scale;
}

INCLUDE_ASM(const s32, "effect/polyManager", func_0015F2D0);

/* Reset active records but leave inactive sentinel entries untouched. */
void polyResetEntries(PolyList *obj) {
    u32 count;
    PolyEntry *entry;
    u32 i;

    count = obj->entryCount;
    i = 0;
    obj->unk14 = 0xFFFFFFF;
    obj->unk6C = 0;
    obj->unk68 = 0;
    entry = obj->entries;
    if (count != 0) {
        do {
            if (entry->value != -0xFFFFFF) {
                entry->value = 0xFFFFFF0;
            }
            i++;
            entry++;
        } while (i < count);
    }
}
