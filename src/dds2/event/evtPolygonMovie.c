#include "common.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"

/* Global event state behind kwlnTaskGetUserValue; the polygon-movie word at 0x0 and
 * the pointer to the shared flag word at 0x8. */
typedef struct EvtGlobal {
    u32 movieFlags; /* 0x0 */
    u32 unk_4;      /* 0x4 */
    u32 *flags;     /* 0x8 */
} EvtGlobal;

/* Polygon-movie clip: time is stored as a float, but compared as whole frames. */
typedef struct PolyMovieClip {
    u8 pad[8];       /* 0x0 */
    f32 duration;    /* 0x8 */
    f32 position;    /* 0xc */
} PolyMovieClip;

typedef struct PolyMovieState {
    u8 pad[4];
    PolyMovieClip *clip;
} PolyMovieState;

typedef struct PolyMovieObject {
    u8 pad[0x18];
    PolyMovieState *state;
} PolyMovieObject;

/* Polygon-movie event parameter blocks blended by the functions below. */
typedef struct EvtBlendA {
    u32 color;    /* 0x00 */
    u32 word;     /* 0x04 */
    f32 f0;       /* 0x08 */
    f32 f1;       /* 0x0C */
    s32 v[2];     /* 0x10 */
    s32 m[2][2];  /* 0x18 */
} EvtBlendA;

typedef struct EvtBlendB {
    u32 u0;       /* 0x00 */
    u32 color;    /* 0x04 */
    u32 word;     /* 0x08 */
    f32 f0;       /* 0x0C */
    f32 f1;       /* 0x10 */
    s32 v[2];     /* 0x14 */
    s32 m[2][2];  /* 0x1C */
} EvtBlendB;

typedef struct EvtBlendD {
    u32 u0;       /* 0x00 */
    s32 v[2];     /* 0x04 */
    u32 color;    /* 0x0C */
    u32 word;     /* 0x10 */
} EvtBlendD;

typedef struct EvtBlendE {
    u32 u0;       /* 0x00 */
    u32 u1;       /* 0x04 */
    f32 f0;       /* 0x08 */
    u32 color;    /* 0x0C */
    u32 word;     /* 0x10 */
    f32 f1;       /* 0x14 */
    f32 f2;       /* 0x18 */
    s32 v[2];     /* 0x1C */
    u32 u2;       /* 0x24 */
    u32 u3;       /* 0x28 */
} EvtBlendE;

typedef struct EvtBlendF {
    u32 u0;       /* 0x00 */
    f32 f0;       /* 0x04 */
    f32 f1;       /* 0x08 */
    u32 color;    /* 0x0C */
    u32 word;     /* 0x10 */
    f32 f2;       /* 0x14 */
    f32 f3;       /* 0x18 */
    f32 f4;       /* 0x1C */
    s32 v[2];     /* 0x20 */
    u32 u1;       /* 0x28 */
} EvtBlendF;

typedef struct EvtBlendG {
    u32 color;    /* 0x00 */
    u32 word;     /* 0x04 */
    s32 m[2][2];  /* 0x08 */
} EvtBlendG;

typedef struct EvtBlendH {
    s32 w[4];     /* 0x00 */
    s32 x;        /* 0x10 */
    u32 word;     /* 0x14 */
    u8 pad[8];    /* 0x18 */
    s32 y[3];     /* 0x20 */
    s32 z[3];     /* 0x2C */
} EvtBlendH;

typedef struct ObjectFlagsTarget {
    u8 pad[0x8C];
    u32 *flags;   /* 0x8C */
} ObjectFlagsTarget;

extern EvtGlobal *kwlnTaskGetUserValue(void);
u32 evtPolygonMovieBlendColor(s32 enable, f32 t, u32 a, u32 b);

extern ObjectFlagsTarget *func_00113318(PolyMovieObject *obj);
extern void dds3SetObjectFlags(PolyMovieObject *obj, s32 flags);
extern void dds3ClearObjectFlags(PolyMovieObject *obj, s32 flags);

extern f32 D_003C9310[4];
extern f32 D_003C9320[4];
extern f32 D_003C9330[4];
extern EvtBlendA D_003C9360;
extern EvtBlendB D_003C9390;
extern EvtBlendG D_003C93C0;
extern EvtBlendD D_003C93E0;
extern EvtBlendE D_003C9410;
extern EvtBlendF D_003C9440;
extern EvtBlendA D_003C9470;

/* "PMD2" resource: a 0x20-byte header followed by 16-byte entries whose
 * offsets are relative to the start of the block. */
typedef struct PmdEntry {
    u32 type;     /* 0x00 */
    u32 unk_04;   /* 0x04 */
    u32 value;    /* 0x08 */
    u32 offset;   /* 0x0C */
} PmdEntry;

typedef struct PmdHeader {
    u8 pad[0x10];
    s32 count;    /* 0x10 */
    s32 kind;     /* 0x14 */
    u8 pad2[8];
    PmdEntry entries[1]; /* 0x20 */
} PmdHeader;

typedef struct PolyMovieWork {
    u32 unk_00;        /* 0x00 */
    s32 res04;         /* 0x04 */
    s32 res08;         /* 0x08 */
    u32 unk_0C;        /* 0x0C */
    PmdHeader *data;   /* 0x10 */
    PmdEntry *entries; /* 0x14 */
    u8 *ptr18;         /* 0x18 */
    u32 unk_1C;        /* 0x1C */
    u8 *ptr20;         /* 0x20 */
    u32 unk_24;        /* 0x24 */
    u8 *ptr28;         /* 0x28 */
    u8 *ptr2C;         /* 0x2C */
    u8 *ptr30;         /* 0x30 */
    u8 *ptr34;         /* 0x34 */
    u32 unk_38;        /* 0x38 */
    u8 *ptr3C;         /* 0x3C */
    u8 *ptr40;         /* 0x40 */
    u32 unk_44;        /* 0x44 */
    u8 *ptr48;         /* 0x48 */
    u8 *ptr4C;         /* 0x4C */
    u8 *ptr50;         /* 0x50 */
    u32 unk_54;        /* 0x54 */
    u8 *ptr58;         /* 0x58 */
    s32 res5C;         /* 0x5C */
    s32 res60;         /* 0x60 */
    u32 unk_64;        /* 0x64 */
    u32 unk_68;        /* 0x68 */
    s32 res6C;         /* 0x6C */
    u32 unk_70;        /* 0x70 */
    PmdHeader *sub;    /* 0x74 */
    PmdEntry *subEntries; /* 0x78 */
    u8 *ptr7C;         /* 0x7C */
    u32 unk_80;        /* 0x80 */
    u8 *ptr84;         /* 0x84 */
    u8 *ptr88;         /* 0x88 */
    u8 *ptr8C;         /* 0x8C */
    PmdHeader *sub2;   /* 0x90 */
    PmdEntry *sub2Entries; /* 0x94 */
    u8 *ptr98;         /* 0x98 */
    u32 unk_9C;        /* 0x9C */
    u32 unk_A0;        /* 0xA0 */
    u8 *ptrA4;         /* 0xA4 */
    u32 unk_A8;        /* 0xA8 */
    u8 *ptrAC;         /* 0xAC */
    u32 unk_B0;        /* 0xB0 */
    u8 *ptrB4;         /* 0xB4 */
    u32 unk_B8;        /* 0xB8 */
    u8 *ptrBC;         /* 0xBC */
    u32 unk_C0;        /* 0xC0 */
    u8 *ptrC4;         /* 0xC4 */
    u32 unk_C8;        /* 0xC8 */
    u8 *ptrCC;         /* 0xCC */
    u32 unk_D0;        /* 0xD0 */
    u8 *ptrD4;         /* 0xD4 */
    u32 unk_D8;        /* 0xD8 */
    u8 *ptrDC;         /* 0xDC */
    u32 unk_E0;        /* 0xE0 */
    u8 *ptrE4;         /* 0xE4 */
    u32 unk_E8;        /* 0xE8 */
    u8 *ptrEC;         /* 0xEC */
    u32 unk_F0;        /* 0xF0 */
    u8 *ptrF4;         /* 0xF4 */
    u32 unk_F8;        /* 0xF8 */
    u8 *ptrFC;         /* 0xFC */
    u32 unk_100;       /* 0x100 */
    s32 handle;        /* 0x104 */
    u32 unk_108[3];    /* 0x108 */
    u32 unk_114;       /* 0x114 */
    s32 *buffer;       /* 0x118 */
} PolyMovieWork;

extern void evtScaleValueByMultiplier(PolyMovieClip *clip, f32 multiplier);
extern void func_00117810(PolyMovieClip *clip);
extern void func_00117820(PolyMovieClip *clip);
extern void *func_00328D68(s32 size);
extern void *memset(void *dst, s32 value, u32 size);
extern void *memcpy(void *dst, const void *src, u32 size);
extern s32 func_003292A8(s32 size);
extern void *sdfResourceRetainAddress(s32 handle);
extern s32 itfMesCreateWindow(u8 *arg);
extern void itfMesDestroyWindowIfPresent(s32 handle);
extern void fileWaitIdle(void);
extern void func_002C7D00(s32 arg);
extern void func_003297C8(s32 arg);
extern s32 mnuQueryTitleSoundBusy(void);
extern void func_002A1308(void);
extern void func_0035B6E0(const char *fmt, ...);
extern void sdfReleaseChipBlock(void *ptr);

s32 evtPolygonMovieTestFlag(void) {
    EvtGlobal *state;
    s32 set;

    state = kwlnTaskGetUserValue();
    set = 0;
    if (state->movieFlags & 8) {
        set = 1;
    }
    return set;
}

f32 func_0024DD48(s32 enable, f32 t, f32 a, f32 b) {
    if (enable) {
        return a * (1.0f - t) + b * t;
    }
    return a;
}

s32 func_0024DD78(s32 enable, f32 t, s32 a, s32 b) {
    s32 result = a;

    if (enable) {
        result = (s32)((f32)a * (1.0f - t) + (f32)b * t);
    }
    return result;
}

u32 func_0024DDB8(s32 enable, f32 t, u32 a, u32 b) {
    if (enable) {
        return (u32)((f32)a * (1.0f - t) + (f32)b * t);
    }
    return a;
}

/* vu0 routine: blend two RGBA8888 colours by t (lerp in float, packed back to RGBA8888) */
u32 evtPolygonMovieBlendColor(s32 enable, f32 t, u32 a, u32 b) {
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;

    if (enable) {
        color1[0] = a;
        EE_MMI_RGBA_UNPACK(color1, 1.0f / 128.0f);
        VU0_SCALE_VF(vf10, 1.0f - t);
        VU0_MOVE_VF(vf11, vf10);
        color2[0] = b;
        EE_MMI_RGBA_UNPACK(color2, 1.0f / 128.0f);
        VU0_SCALE_VF(vf10, t);
        VU0_ADD(vf10, vf10, vf11);
        EE_MMI_RGBA_PACK_UNIT(packed, 128.0f);
        blended[0] = packed;
        return blended[0];
    }
    return a;
}

/* vu0 routine: blend three rows of vectors a and b by t into out (missing input = default rows) */
void evtBlendVectorRows(s32 enable, f32 t, f32 *a, f32 *b, f32 *out) {
    if (enable == 0) {
        t = 0.0f;
    }
    if (a != NULL) {
        VU0_LOAD_VF(vf10, a);
    } else {
        VU0_LOAD_VF(vf10, D_003C9310);
    }
    VU0_SCALAR_OP(1.0f - t, "vmulx.xyzw vf10, vf10, vf2x");
    if (b != NULL) {
        VU0_LOAD_VF(vf11, b);
    } else {
        VU0_LOAD_VF(vf11, D_003C9310);
    }
    VU0_SCALAR_OP(t, "vmulx.xyzw vf11, vf11, vf2x");
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, out);
    if (a != NULL) {
        VU0_LOAD_VF(vf10, a + 4);
    } else {
        VU0_LOAD_VF(vf10, D_003C9320);
    }
    VU0_SCALAR_OP(1.0f - t, "vmulx.xyzw vf10, vf10, vf2x");
    if (b != NULL) {
        VU0_LOAD_VF(vf11, b + 4);
    } else {
        VU0_LOAD_VF(vf11, D_003C9320);
    }
    VU0_SCALAR_OP(t, "vmulx.xyzw vf11, vf11, vf2x");
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, out + 4);
    if (a != NULL) {
        VU0_LOAD_VF(vf10, a + 8);
    } else {
        VU0_LOAD_VF(vf10, D_003C9330);
    }
    VU0_SCALAR_OP(1.0f - t, "vmulx.xyzw vf10, vf10, vf2x");
    if (b != NULL) {
        VU0_LOAD_VF(vf11, b + 8);
    } else {
        VU0_LOAD_VF(vf11, D_003C9330);
    }
    VU0_SCALAR_OP(t, "vmulx.xyzw vf11, vf11, vf2x");
    VU0_ADD(vf10, vf10, vf11);
    VU0_SET_W_ONE(vf10);
    VU0_STORE_VF(vf10, out + 8);
}

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_0024E0A0);

void evtBlendParamsA(s32 enable, f32 t, EvtBlendA *a, EvtBlendA *b, EvtBlendA *out) {
    s32 i;
    s32 j;

    if (enable == 0) {
        t = 0.0f;
    }
    if (a == NULL) {
        a = &D_003C9360;
    }
    if (b == NULL) {
        b = &D_003C9360;
    }
    out->color = evtPolygonMovieBlendColor(enable, t, a->color, b->color);
    out->word = a->word;
    out->f0 = func_0024DD48(enable, t, a->f0, b->f0);
    out->f1 = func_0024DD48(enable, t, a->f1, b->f1);
    for (i = 0; i < 2; i++) {
        out->v[i] = func_0024DD78(enable, t, a->v[i], b->v[i]);
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            out->m[i][j] = func_0024DD78(enable, t, a->m[i][j], b->m[i][j]);
        }
    }
}

void evtBlendParamsB(s32 enable, f32 t, EvtBlendB *a, EvtBlendB *b, EvtBlendB *out) {
    s32 i;
    s32 j;

    if (enable == 0) {
        t = 0.0f;
    }
    if (a == NULL) {
        a = &D_003C9390;
    }
    if (b == NULL) {
        b = &D_003C9390;
    }
    out->u0 = func_0024DDB8(enable, t, a->u0, b->u0);
    out->color = evtPolygonMovieBlendColor(enable, t, a->color, b->color);
    out->word = a->word;
    out->f0 = func_0024DD48(enable, t, a->f0, b->f0);
    out->f1 = func_0024DD48(enable, t, a->f1, b->f1);
    for (i = 0; i < 2; i++) {
        out->v[i] = func_0024DD78(enable, t, a->v[i], b->v[i]);
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            out->m[i][j] = func_0024DD78(enable, t, a->m[i][j], b->m[i][j]);
        }
    }
}

void evtPolygonMovieBlendMatrixParam(s32 enable, f32 t, EvtBlendG *a, EvtBlendG *b, EvtBlendG *out) {
    s32 i;
    s32 j;

    if (enable == 0) {
        t = 0.0f;
    }
    if (a == NULL) {
        a = &D_003C93C0;
    }
    if (b == NULL) {
        b = &D_003C93C0;
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            out->m[i][j] = func_0024DD78(enable, t, a->m[i][j], b->m[i][j]);
        }
    }
    out->color = evtPolygonMovieBlendColor(enable, t, a->color, b->color);
    out->word = a->word;
}

void evtBlendParamsD(s32 enable, f32 t, EvtBlendD *a, EvtBlendD *b, EvtBlendD *out) {
    s32 i;

    if (enable == 0) {
        t = 0.0f;
    }
    if (a == NULL) {
        a = &D_003C93E0;
    }
    if (b == NULL) {
        b = &D_003C93E0;
    }
    out->u0 = func_0024DDB8(enable, t, a->u0, b->u0);
    for (i = 0; i < 2; i++) {
        out->v[i] = func_0024DD78(enable, t, a->v[i], b->v[i]);
    }
    out->color = evtPolygonMovieBlendColor(enable, t, a->color, b->color);
    out->word = a->word;
}

void evtBlendParamsE(s32 enable, f32 t, EvtBlendE *a, EvtBlendE *b, EvtBlendE *out) {
    s32 i;

    if (enable == 0) {
        t = 0.0f;
    }
    if (a == NULL) {
        a = &D_003C9410;
    }
    if (b == NULL) {
        b = &D_003C9410;
    }
    out->u0 = func_0024DDB8(enable, t, a->u0, b->u0);
    out->u1 = func_0024DDB8(enable, t, a->u1, b->u1);
    out->f0 = func_0024DD48(enable, t, a->f0, b->f0);
    out->color = evtPolygonMovieBlendColor(enable, t, a->color, b->color);
    out->word = a->word;
    out->f1 = func_0024DD48(enable, t, a->f1, b->f1);
    out->f2 = func_0024DD48(enable, t, a->f2, b->f2);
    out->u2 = func_0024DDB8(enable, t, a->u2, b->u2);
    for (i = 0; i < 2; i++) {
        out->v[i] = func_0024DD78(enable, t, a->v[i], b->v[i]);
    }
    out->u3 = func_0024DDB8(enable, t, a->u3, b->u3);
}

void evtBlendParamsF(s32 enable, f32 t, EvtBlendF *a, EvtBlendF *b, EvtBlendF *out) {
    s32 i;

    if (enable == 0) {
        t = 0.0f;
    }
    if (a == NULL) {
        a = &D_003C9440;
    }
    if (b == NULL) {
        b = &D_003C9440;
    }
    out->u0 = func_0024DDB8(enable, t, a->u0, b->u0);
    out->f0 = func_0024DD48(enable, t, a->f0, b->f0);
    out->f1 = func_0024DD48(enable, t, a->f1, b->f1);
    out->color = evtPolygonMovieBlendColor(enable, t, a->color, b->color);
    out->word = a->word;
    out->f2 = func_0024DD48(enable, t, a->f2, b->f2);
    out->f3 = func_0024DD48(enable, t, a->f3, b->f3);
    out->f4 = func_0024DD48(enable, t, a->f4, b->f4);
    for (i = 0; i < 2; i++) {
        out->v[i] = func_0024DD78(enable, t, a->v[i], b->v[i]);
    }
    out->u1 = func_0024DDB8(enable, t, a->u1, b->u1);
}

void evtBlendParamsG(s32 enable, f32 t, EvtBlendA *a, EvtBlendA *b, EvtBlendA *out) {
    s32 i;
    s32 j;

    if (enable == 0) {
        t = 0.0f;
    }
    if (a == NULL) {
        a = &D_003C9470;
    }
    if (b == NULL) {
        b = &D_003C9470;
    }
    out->color = evtPolygonMovieBlendColor(enable, t, a->color, b->color);
    out->word = a->word;
    out->f0 = func_0024DD48(enable, t, a->f0, b->f0);
    out->f1 = func_0024DD48(enable, t, a->f1, b->f1);
    for (i = 0; i < 2; i++) {
        out->v[i] = func_0024DD78(enable, t, a->v[i], b->v[i]);
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            out->m[i][j] = func_0024DD78(enable, t, a->m[i][j], b->m[i][j]);
        }
    }
}

void evtBlendParamsH(s32 enable, f32 t, EvtBlendH *a, EvtBlendH *b, EvtBlendH *out) {
    s32 i;

    if (enable == 0) {
        t = 0.0f;
    }
    out->x = func_0024DD78(enable, t, a->x, b->x);
    out->w[0] = func_0024DD78(enable, t, a->w[0], b->w[0]);
    out->w[1] = func_0024DD78(enable, t, a->w[1], b->w[1]);
    out->w[2] = func_0024DD78(enable, t, a->w[2], b->w[2]);
    out->w[3] = func_0024DD78(enable, t, a->w[3], b->w[3]);
    out->word = a->word;
    for (i = 0; i < 3; i++) {
        out->y[i] = func_0024DD78(enable, t, a->y[i], b->y[i]);
        out->z[i] = func_0024DD78(enable, t, a->z[i], b->z[i]);
    }
}

void evtPolygonMovieSetObjectMode(PolyMovieObject *obj, u32 mode, s32 setFlags, s32 clearFlags) {
    switch (mode) {
    case 0:
        *func_00113318(obj)->flags &= ~1;
        break;
    case 1:
        *func_00113318(obj)->flags |= 1;
        break;
    case 2:
        dds3ClearObjectFlags(obj, 0x400);
        dds3ClearObjectFlags(obj, 0x200);
        dds3ClearObjectFlags(obj, 0x4000);
        dds3ClearObjectFlags(obj, 0x8000);
        return;
    case 3:
        dds3SetObjectFlags(obj, 0x400);
        dds3ClearObjectFlags(obj, 0x200);
        if (setFlags != 0) {
            dds3SetObjectFlags(obj, setFlags);
        }
        if (clearFlags != 0) {
            dds3ClearObjectFlags(obj, clearFlags);
        }
        return;
    case 4:
        dds3ClearObjectFlags(obj, 0x400);
        dds3SetObjectFlags(obj, 0x200);
        if (setFlags != 0) {
            dds3SetObjectFlags(obj, setFlags);
        }
        if (clearFlags != 0) {
            dds3ClearObjectFlags(obj, clearFlags);
        }
        break;
    }
}

/* Apply a caller-provided mask to the event state's flag word. */
void evtPolygonMovieSetFlagBits(u32 unused, u32 bits) {
    EvtGlobal *state;

    state = kwlnTaskGetUserValue();
    *state->flags = *state->flags | bits;
}

void evtPolygonMovieClearFlagBits(u32 unused, u32 bits) {
    EvtGlobal *state;

    state = kwlnTaskGetUserValue();
    *state->flags = *state->flags & ~bits;
}

/* Scale the clip to the elapsed fraction of its duration, then apply or undo it. */
s32 evtPolygonMovieScaleByProgress(PolyMovieObject *movie, s32 undo, s32 start, s32 end) {
    PolyMovieClip *clip;
    s32 frame;
    s32 duration;

    frame = end - start;
    if (frame < 0) {
        frame = 0;
    }
    clip = movie->state->clip;
    if (clip != NULL) {
        duration = (s32)clip->duration;
        if (duration <= frame) {
            frame = duration;
        }
        evtScaleValueByMultiplier(clip, (f32)frame / (f32)duration);
        if (undo == 0) {
            func_00117810(clip);
        } else {
            func_00117820(clip);
        }
    }
}

/* Store elapsed frames plus an offset, bounded below by zero and above by clip duration. */
void func_0024F130(PolyMovieObject *movie, s32 unused, s32 start, s32 end, s32 offset) {
    PolyMovieClip *clip;

    clip = movie->state->clip;
    if (clip != NULL) {
        s32 frame = end - start + offset;

        if (frame < 0) {
            frame = 0;
        }
        if ((s32)clip->duration <= frame) {
            frame = (s32)clip->duration;
        }
        clip->position = (f32)frame;
    }
}

/* Allocate and clear a polygon-movie event work block. */
void *evtPolygonMovieAllocWork(void) {
    void *work;

    work = func_00328D68(0x11C);
    if (work == NULL) {
        return work;
    }
    memset(work, 0, 0x11C);
    return work;
}

PolyMovieWork *evtPolygonMovieInitWork(PolyMovieWork *work, PmdHeader *data, PmdHeader *sub, PmdHeader *sub2) {
    s32 i;

    if (data == NULL) {
        return NULL;
    }
    work->unk_114 = 0;
    work->handle = -1;
    work->buffer = func_00328D68(0x1FC);
    for (i = 0; i < 0x7F; i++) {
        work->buffer[i] = 0;
    }
    work->unk_1C = 0;
    work->unk_24 = 0;
    work->unk_38 = 0;
    work->unk_44 = 0;
    work->unk_54 = 0;
    work->entries = data->entries;
    work->data = data;
    work->ptr18 = NULL;
    work->ptr20 = NULL;
    work->ptr28 = NULL;
    work->ptr2C = NULL;
    work->ptr30 = NULL;
    work->ptr34 = NULL;
    work->ptr3C = NULL;
    work->ptr40 = NULL;
    work->ptr48 = NULL;
    work->ptr50 = NULL;
    work->ptr58 = NULL;
    for (i = 0; i < work->data->count; i++) {
        switch (work->entries[i].type) {
        case 2:
            work->ptr20 = (u8 *)data + work->entries[i].offset;
            work->unk_24 = work->entries[i].value;
            break;
        case 10:
            work->ptr28 = (u8 *)data + work->entries[i].offset;
            break;
        case 11:
            work->ptr2C = (u8 *)data + work->entries[i].offset;
            break;
        case 12:
            work->ptr30 = (u8 *)data + work->entries[i].offset;
            break;
        case 3:
            work->ptr34 = (u8 *)data + work->entries[i].offset;
            work->unk_38 = work->entries[i].value;
            break;
        case 9:
            work->ptr3C = (u8 *)data + work->entries[i].offset;
            break;
        case 1:
            work->ptr18 = (u8 *)data + work->entries[i].offset;
            work->unk_1C = work->entries[i].value;
            break;
        case 6:
            work->ptr4C = (u8 *)data + work->entries[i].offset;
            if (work->entries[i].value == 0) {
                work->handle = -1;
            } else {
                work->handle = itfMesCreateWindow(work->ptr4C);
            }
            break;
        case 7:
            work->ptr40 = (u8 *)data + work->entries[i].offset;
            work->unk_44 = work->entries[i].value;
            break;
        case 8:
            work->ptr48 = (u8 *)data + work->entries[i].offset;
            break;
        case 22:
            work->ptr50 = (u8 *)data + work->entries[i].offset;
            work->unk_54 = work->entries[i].value;
            break;
        case 23:
            work->ptr58 = (u8 *)data + work->entries[i].offset;
            break;
        }
    }
    work->sub = sub;
    if (sub != NULL) {
        work->subEntries = sub->entries;
    } else {
        work->subEntries = NULL;
    }
    work->ptr7C = NULL;
    work->unk_80 = 0;
    work->ptr84 = NULL;
    work->ptr88 = NULL;
    work->unk_A0 = 0;
    work->ptrA4 = NULL;
    work->unk_A8 = 0;
    work->ptrAC = NULL;
    work->unk_B0 = 0;
    work->ptrB4 = NULL;
    work->unk_B8 = 0;
    work->ptrBC = NULL;
    work->unk_C0 = 0;
    work->ptrC4 = NULL;
    work->unk_C8 = 0;
    work->ptrCC = NULL;
    work->unk_D0 = 0;
    work->ptrD4 = NULL;
    work->unk_D8 = 0;
    work->ptrDC = NULL;
    work->unk_E0 = 0;
    work->ptrE4 = NULL;
    work->unk_E8 = 0;
    work->ptrF4 = NULL;
    work->unk_F8 = 0;
    work->ptrEC = NULL;
    work->unk_F0 = 0;
    work->ptrFC = NULL;
    work->unk_100 = 0;
    if (sub == NULL) {
        return work;
    }
    for (i = 0; i < work->sub->count; i++) {
        switch (work->subEntries[i].type) {
        case 0:
            work->ptr84 = (u8 *)sub + work->subEntries[i].offset;
            break;
        case 4:
            if (work->sub->kind == 4) {
                work->ptr8C = NULL;
                work->ptr88 = (u8 *)sub + work->subEntries[i].offset;
            } else {
                work->ptr88 = NULL;
                work->ptr8C = (u8 *)sub + work->subEntries[i].offset;
            }
            work->unk_A0 = work->subEntries[i].value;
            break;
        case 1:
            work->ptr7C = (u8 *)sub + work->subEntries[i].offset;
            work->unk_80 = work->subEntries[i].value;
            break;
        case 5:
            work->ptrA4 = (u8 *)sub + work->subEntries[i].offset;
            work->unk_A8 = work->subEntries[i].value;
            break;
        case 13:
            work->ptrAC = (u8 *)sub + work->subEntries[i].offset;
            work->unk_B0 = work->subEntries[i].value;
            break;
        case 14:
            work->ptrB4 = (u8 *)sub + work->subEntries[i].offset;
            work->unk_B8 = work->subEntries[i].value;
            break;
        case 15:
            work->ptrBC = (u8 *)sub + work->subEntries[i].offset;
            work->unk_C0 = work->subEntries[i].value;
            break;
        case 16:
            work->ptrC4 = (u8 *)sub + work->subEntries[i].offset;
            work->unk_C8 = work->subEntries[i].value;
            break;
        case 17:
            work->ptrCC = (u8 *)sub + work->subEntries[i].offset;
            work->unk_D0 = work->subEntries[i].value;
            break;
        case 18:
            work->ptrD4 = (u8 *)sub + work->subEntries[i].offset;
            work->unk_D8 = work->subEntries[i].value;
            break;
        case 19:
            work->ptrDC = (u8 *)sub + work->subEntries[i].offset;
            work->unk_E0 = work->subEntries[i].value;
            break;
        case 20:
            work->ptrE4 = (u8 *)sub + work->subEntries[i].offset;
            work->unk_E8 = work->subEntries[i].value;
            break;
        case 24:
            work->ptrEC = (u8 *)sub + work->subEntries[i].offset;
            work->unk_F0 = work->subEntries[i].value;
            break;
        case 21:
            work->ptrF4 = (u8 *)sub + work->subEntries[i].offset;
            work->unk_F8 = work->subEntries[i].value;
            func_0035B6E0("object table set ok.\n");
            break;
        case 25:
            work->ptrFC = (u8 *)sub + work->subEntries[i].offset;
            work->unk_100 = work->subEntries[i].value;
            break;
        }
    }
    work->sub2 = sub2;
    if (sub2 != NULL) {
        work->sub2Entries = sub2->entries;
    } else {
        work->sub2Entries = NULL;
    }
    work->ptr98 = NULL;
    work->unk_9C = 0;
    if (sub2 == NULL) {
        return work;
    }
    for (i = 0; i < work->sub2->count; i++) {
        if (work->sub2Entries[i].type == 4) {
            work->ptr98 = (u8 *)sub2 + work->sub2Entries[i].offset;
            work->unk_9C = work->sub2Entries[i].value;
        }
    }
    return work;
}

void evtPolygonMovieFreeWork(PolyMovieWork *work) {
    if (work != NULL) {
        if (work->buffer != NULL) {
            sdfReleaseChipBlock(work->buffer);
        }
        if (work->handle >= 0) {
            itfMesDestroyWindowIfPresent(work->handle);
            work->handle = -1;
        }
        fileWaitIdle();
        if (work->res04 != 0) {
            func_002C7D00(work->res04);
        }
        if (work->res5C != 0) {
            func_002C7D00(work->res5C);
        }
        if (work->res08 != 0) {
            func_003297C8(work->res08);
        }
        if (work->res60 != 0) {
            func_003297C8(work->res60);
        }
        if (work->res6C != 0) {
            func_003297C8(work->res6C);
        }
        if (mnuQueryTitleSoundBusy() == 1) {
            func_002A1308();
        }
        func_0035B6E0("sound stop all. \n");
        sdfReleaseChipBlock(work);
    }
}

/* Allocate a resource block holding a fresh "PMD2" header; returns the handle. */
s32 evtPolygonMovieCreateHeader(void **out) {
    s32 header[16] = {0, 0, 0x32444D50, 0, 1, 9, 0, 0, 0, 0x10, 1, 0x30, 0, 999, 1000, 0};
    s32 size;
    s32 handle;
    void *block;

    size = 0x40;
    handle = func_003292A8(size);
    block = sdfResourceRetainAddress(handle);
    memcpy(block, header, size);
    *out = block;
    return handle;
}

