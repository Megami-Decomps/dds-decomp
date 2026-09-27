#include "common.h"

typedef struct PacHead {
    u8 unk0; /* 0x0 */
    u8 unk1; /* 0x1 */
    u8 pad2[2]; /* 0x2 */
    s32 unk4; /* 0x4 */
    u8 pad8[4]; /* 0x8 */
    s32 unkC; /* 0xC */
} PacHead;

typedef struct PacWork {
    struct PacWork *unk0; /* 0x0 */
    struct PacState *unk4; /* 0x4 */
    s32 unk8; /* 0x8 */
    u8 *unkC; /* 0xC */
} PacWork;

typedef struct PacAlloc {
    s32 unk0; /* 0x0 */
    s32 unk4; /* 0x4 */
    u8 pad8[24]; /* 0x8 */
    s32 unk20; /* 0x20 */
} PacAlloc;

typedef struct PacBuf {
    s32 unk0; /* 0x0 */
    s32 unk4; /* 0x4 */
    u8 *unk8; /* 0x8 */
    s32 unkC; /* 0xC */
} PacBuf;

typedef struct PacState {
    u8 unk0; /* 0x0 */
    u8 unk1; /* 0x1 */
    u8 pad2[2]; /* 0x2 */
    s32 unk4; /* 0x4 */
    void (*unk8)(struct PacState *); /* 0x8 */
    void (*unkC)(struct PacState *); /* 0xC */
    u8 *unk10; /* 0x10 */
    s32 unk14; /* 0x14 */
    s32 unk18; /* 0x18 */
    u8 *unk1C; /* 0x1C */
    s32 unk20; /* 0x20 */
    PacBuf *unk24; /* 0x24 */
    PacBuf *unk28; /* 0x28 */
    PacAlloc *unk2C; /* 0x2C */
    PacWork *unk30; /* 0x30 */
    PacWork *unk34; /* 0x34 */
} PacState;

void func_002EE2C0(PacState *arg0, PacHead *arg1);
void func_002EDD98(PacState *arg0);
void func_002EB278(void *arg0, void *arg1, void *arg2, s32 arg3);
void func_002CFF98(void *arg0);
void func_002EDC98(PacState *arg0, s32 arg1);
void func_002EDCC0(PacState *arg0);
PacWork *func_002EDF60(PacState *arg0, PacHead *arg1);
void *func_002CFF68(s32 size);
void *func_002CFEB8(s32 size);
void func_002EE6F8(PacState *arg0, PacHead *arg1, PacBuf *arg2);
void func_002EE418(PacState *arg0);
void func_002EE4A8(PacState *arg0);
void func_002EE828(PacState *arg0);
void func_002EE900(PacState *arg0);
void func_002EE930(PacState *arg0);
void func_002EE3E8(PacState *arg0, void *arg1);
void func_002EE478(PacState *arg0, void *arg1);
void func_002EE508(PacState *arg0, void *arg1);
void func_002EE868(PacState *arg0, void *arg1);
void func_002EEAA0(PacState *arg0, void *arg1);
void func_002DA058(s32 arg0, s32 arg1);
s32 func_002D32A0(void *arg0);
s32 func_002D0A48(s32 arg0);
s32 func_002D3288(s32 arg0);
s32 func_002D0518(s32 arg0);
s32 func_002D03F8(s32 arg0);
void func_002D09D8(void *arg0);
void func_002EEE98(void *arg0, void *arg1);
s32 func_002EEAE0(void *arg0, void *arg1, s32 arg2);
extern void *memcpy(void *dst, const void *src, u32 n);


INCLUDE_ASM(const s32, "sdf/sdfPacDecode", func_002EDE48);

PacWork *func_002EDF60(PacState *arg0, PacHead *arg1) {
    s32 size = arg1->unk1 & 0xF0;
    PacWork *node = func_002CFF68(size + 0x20);
    node->unk4 = arg0;
    memcpy((u8 *)node + 0x10, arg1, size + 0x10);
    if (arg0->unk34 == NULL) {
        arg0->unk30 = node;
    } else {
        arg0->unk34->unk0 = node;
    }
    arg0->unk34 = node;
    return node;
}

PacWork *func_002EDFE8(PacWork *arg0) {
    PacState *state = arg0->unk4;
    PacWork *link = (PacWork *)&state->unk30;
    PacWork *cur = state->unk30;
    PacWork *prev = NULL;
    PacWork *next;
    if (cur != arg0) {
        do {
            prev = cur;
            cur = prev->unk0;
            link = prev;
        } while (cur != arg0);
    }
    next = arg0->unk0;
    link->unk0 = next;
    if (state->unk34 == arg0) {
        state->unk34 = prev;
    }
    func_002CFF98(arg0);
    return next;
}

s32 func_002EE058(PacState *arg0, s32 arg1, PacHead *arg2) {
    if (arg1 == 0) {
        switch (arg2->unk0) {
        case 1:
            func_002EE3E8(arg0, arg2);
            return 0;
        case 2:
            func_002EE868(arg0, arg2);
            return 0;
        case 6:
            func_002EE478(arg0, arg2);
            return 0;
        case 8:
            func_002EE508(arg0, arg2);
            return 0;
        case 9:
            func_002EEAA0(arg0, arg2);
            return 0;
        case 0xFF:
            return 1;
        default:
            return 3;
        }
    }
    return 0;
}
void *func_002EE138(u8 *arg0) {
    s32 x = arg0[0x11] & 0xF0;
    if (x <= 0) {
        return NULL;
    }
    return arg0 + 0x20;
}

void func_002EE158(PacState *arg0) {
    s32 n = arg0->unk20;
    s32 t = arg0->unk14;
    if (t < n) {
        n = t;
    }
    if (n != 0) {
        memcpy(arg0->unk1C, arg0->unk10, n);
        func_002EDC98(arg0, n);
        arg0->unk1C += n;
        {
            s32 r = arg0->unk20 - n;
            arg0->unk20 = r;
            if (r != 0) {
                return;
            }
        }
        arg0->unkC(arg0);
    }
}

void func_002EE1E0(PacState *arg0) {
    s32 n = arg0->unk14;
    s32 r = func_002EEAE0(arg0->unk24, arg0->unk10, n);
    func_002EDC98(arg0, n - arg0->unk24->unkC);
    if (r == 0) {
        return;
    }
    func_002CFF98(arg0->unk24);
    arg0->unkC(arg0);
}

void func_002EE258(PacState *arg0) {
    s32 n = arg0->unk20;
    if (arg0->unk14 < n) {
        n = arg0->unk14;
    }
    if (n != 0) {
        func_002EDC98(arg0, n);
        {
            s32 r = arg0->unk20 - n;
            arg0->unk20 = r;
            if (r != 0) {
                return;
            }
        }
        arg0->unkC(arg0);
    }
}

void func_002EE2C0(PacState *arg0, PacHead *arg1) {
    s32 v = arg1->unkC;
    if (v == 0) {
        v = arg1->unk4 + (arg1->unk1 & 0xF0) - 0x10;
    }
    if (arg0->unk1 & 1) {
        PacWork *node = func_002EDF60(arg0, arg1);
        node->unkC = (u8 *)arg1 + 0x10;
        arg0->unk8 = func_002EE258;
    } else {
        PacWork *node;
        arg0->unk0 = 2;
        node = func_002EDF60(arg0, arg1);
        if (arg0->unk1 & 2) {
            node->unk8 = func_002D0518(v);
        } else {
            node->unk8 = func_002D03F8(v);
        }
        arg0->unk1C = node->unkC = (u8 *)func_002D0A48(node->unk8);
        switch (arg1->unk1 & 0xF) {
        case 0:
            arg0->unk8 = func_002EE158;
            break;
        case 1: {
            PacBuf *tmp = func_002CFEB8(0x20);
            arg0->unk24 = tmp;
            func_002EEE98(tmp, arg0->unk1C);
            arg0->unk8 = func_002EE1E0;
            break;
        }
        default:
            break;
        }
    }
}

void func_002EE3E8(PacState *arg0, void *arg1) {
    func_002EE2C0(arg0, arg1);
    arg0->unkC = func_002EDD98;
}

void func_002EE418(PacState *arg0) {
    PacWork *work = arg0->unk34;
    u8 *p = work->unkC;
    u8 *q = p + 0x10;
    s32 n = *(s32 *)(p + 4);
    work->unkC = q;
    if (n != 0) {
        func_002EB278(q, q, q + *(s32 *)p, n);
        *(s32 *)(p + 4) = 0;
    }
    func_002EDD98(arg0);
}

void func_002EE478(PacState *arg0, void *arg1) {
    func_002EE2C0(arg0, arg1);
    arg0->unkC = func_002EE418;
}

void func_002EE4A8(PacState *arg0) {
    PacWork *work = arg0->unk34;
    u8 *p = work->unkC;
    u8 *q = p + 0x10;
    s32 n = *(s32 *)(p + 4);
    work->unkC = q;
    if (n != 0) {
        func_002EB278(q, q, q + *(s32 *)p, n);
        *(s32 *)(p + 4) = 0;
    }
    func_002EDD98(arg0);
}

void func_002EE508(PacState *arg0, void *arg1) {
    func_002EE2C0(arg0, arg1);
    arg0->unkC = func_002EE4A8;
}

void func_002EE538(PacState *arg0) {
    PacBuf *s = arg0->unk28;
    s32 n = s->unkC;
    if (arg0->unk14 < n) {
        n = arg0->unk14;
    }
    if (n != 0) {
        memcpy(s->unk8, arg0->unk10, n);
        func_002EDC98(arg0, n);
        s->unk8 += n;
        {
            s32 r = s->unkC - n;
            s->unkC = r;
            if (r != 0) {
                return;
            }
        }
        s->unk0 = func_002D3288(func_002D0A48(s->unk4));
        func_002D09D8(&s->unk4);
        arg0->unkC(arg0);
    }
}

void func_002EE5E0(PacState *arg0) {
    s32 n = arg0->unk20;
    s32 r = func_002EEAE0(arg0->unk24, arg0->unk10, n);
    func_002EDC98(arg0, n - arg0->unk24->unkC);
    if (r == 0) {
        return;
    }
    {
        PacBuf *s = arg0->unk28;
        s->unk0 = func_002D3288(func_002D0A48(s->unk4));
        func_002D09D8(&s->unk4);
    }
    func_002CFF98(arg0->unk24);
    arg0->unkC(arg0);
}

void func_002EE678(PacState *arg0) {
    PacBuf *s = arg0->unk28;
    s32 n = s->unkC;
    if (arg0->unk14 < n) {
        n = arg0->unk14;
    }
    if (n != 0) {
        func_002EDC98(arg0, n);
        {
            s32 r = s->unkC - n;
            s->unkC = r;
            if (r != 0) {
                return;
            }
        }
        s->unk0 = func_002D32A0(s->unk8);
        arg0->unkC(arg0);
    }
}

INCLUDE_ASM(const s32, "sdf/sdfPacDecode", func_002EE6F8);

INCLUDE_ASM(const s32, "sdf/sdfPacDecode", func_002EE828);

void func_002EE868(PacState *arg0, void *arg1) {
    func_002EDF60(arg0, arg1);
    {
        void *p = func_002CFEB8(0x10);
        arg0->unk2C = (PacAlloc *)p;
        func_002EE6F8(arg0, arg1, p);
    }
    arg0->unkC = func_002EE828;
}


void func_002EE8C0(PacState *arg0) {
    func_002EE6F8(arg0, (u8 *)arg0->unk2C + 0x10, (u8 *)arg0->unk2C + 0x20);
    arg0->unkC = func_002EE930;
}


INCLUDE_ASM(const s32, "sdf/sdfPacDecode", func_002EE900);

void func_002EE930(PacState *arg0) {
    PacAlloc *p = arg0->unk2C;
    func_002DA058(arg0->unk34->unk8, p->unk20);
    {
        s32 c = p->unk4 + 1;
        p->unk4 = c;
        if (c == p->unk0) {
            func_002CFF98(p);
            func_002EDD98(arg0);
        } else {
            func_002EE900(arg0);
        }
    }
}

void func_002EE9A8(PacState *arg0) {
    s32 n = arg0->unk14;
    if (arg0->unk20 < n) {
        n = arg0->unk20;
    }
    func_002EDC98(arg0, n);
    {
        s32 r = arg0->unk20 - n;
        arg0->unk20 = r;
        if (r != 0) {
            return;
        }
    }
    func_002EE900(arg0);
}

