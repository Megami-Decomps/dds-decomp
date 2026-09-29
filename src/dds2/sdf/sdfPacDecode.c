#include "common.h"

typedef struct PacHead {
    u8 unk0; /* 0x0 */
    u8 unk1; /* 0x1 */
    u8 pad2[2]; /* 0x2 */
    s32 unk4; /* 0x4 */
    u8 pad8[4]; /* 0x8 */
    s32 unkC; /* 0xC */
    u8 payload[1]; /* 0x10: variable-length packet data */
} PacHead;

typedef struct PacWork {
    struct PacWork *next; /* 0x0: packet queue link */
    struct PacState *owner; /* 0x4 */
    s32 unk8; /* 0x8 */
    u8 *unkC; /* 0xC */
    u8 packet[1]; /* 0x10: copied header and packet data */
} PacWork;

typedef struct PacAlloc {
    s32 unk0; /* 0x0 */
    s32 unk4; /* 0x4 */
    u8 pad8[24]; /* 0x8 */
    s32 unk20; /* 0x20 */
} PacAlloc;

typedef struct PacBuf {
    s32 result; /* 0x0 */
    s32 resourceSlot; /* 0x4 */
    u8 *cursor; /* 0x8 */
    s32 remaining; /* 0xC */
} PacBuf;

typedef struct PacState {
    u8 unk0; /* 0x0 */
    u8 unk1; /* 0x1 */
    u8 pad2[2]; /* 0x2 */
    s32 unk4; /* 0x4 */
    void (*unk8)(struct PacState *); /* 0x8 */
    void (*onComplete)(struct PacState *); /* 0xC */
    u8 *inputCursor; /* 0x10 */
    s32 inputAvailable; /* 0x14 */
    s32 unk18; /* 0x18 */
    u8 *outputCursor; /* 0x1C */
    s32 pendingBytes; /* 0x20 */
    PacBuf *decoder; /* 0x24 */
    PacBuf *buffer; /* 0x28 */
    PacAlloc *unk2C; /* 0x2C */
    PacWork *queueHead; /* 0x30 */
    PacWork *queueTail; /* 0x34 */
} PacState;

PacWork *func_00346E08(PacState *arg0, PacHead *arg1);

void *func_00328E18(s32 size);

extern void *memcpy(void *dst, const void *src, u32 n);

void func_00328E48(void *arg0);

void func_00347290(PacState *arg0, void *arg1);

void func_00347320(PacState *arg0, void *arg1);

void func_003473B0(PacState *arg0, void *arg1);

void func_00347710(PacState *arg0, void *arg1);

void func_00347948(PacState *arg0, void *arg1);

void sdfPacAdvanceInput(PacState *arg0, s32 arg1);

s32 func_00347988(void *arg0, void *arg1, s32 arg2);

void func_00347168(PacState *arg0, PacHead *arg1);

void func_00346C40(PacState *arg0);

void func_003472C0(PacState *arg0);

void func_00347350(PacState *arg0);

s32 sdfResourceRetainAddress(s32 arg0);

s32 func_0032C138(s32 arg0);

void sdfReleaseMemorySlot(void *arg0);

s32 func_0032C150(void *arg0);

void *func_00328D68(s32 size);

void func_003475A0(PacState *arg0, PacHead *arg1, PacBuf *arg2);

void func_003476D0(PacState *arg0);

void func_003477D8(PacState *arg0);

INCLUDE_ASM(const s32, "sdf/sdfPacDecode", func_00346CF0);

PacWork *func_00346E08(PacState *arg0, PacHead *arg1) {
    s32 size = arg1->unk1 & 0xF0;
    PacWork *node = func_00328E18(size + 0x20);
    node->owner = arg0;
    memcpy(node->packet, arg1, size + 0x10);
    if (arg0->queueTail == NULL) {
        arg0->queueHead = node;
    } else {
        arg0->queueTail->next = node;
    }
    arg0->queueTail = node;
    return node;
}

PacWork *func_00346E90(PacWork *arg0) {
    PacState *state = arg0->owner;
    PacWork *link = (PacWork *)&state->queueHead;
    PacWork *cur = state->queueHead;
    PacWork *prev = NULL;
    PacWork *next;
    if (cur != arg0) {
        do {
            prev = cur;
            cur = prev->next;
            link = prev;
        } while (cur != arg0);
    }
    next = arg0->next;
    link->next = next;
    if (state->queueTail == arg0) {
        state->queueTail = prev;
    }
    func_00328E48(arg0);
    return next;
}

s32 func_00346F00(PacState *arg0, s32 arg1, PacHead *arg2) {
    if (arg1 == 0) {
        switch (arg2->unk0) {
        case 1:
            func_00347290(arg0, arg2);
            return 0;
        case 2:
            func_00347710(arg0, arg2);
            return 0;
        case 6:
            func_00347320(arg0, arg2);
            return 0;
        case 8:
            func_003473B0(arg0, arg2);
            return 0;
        case 9:
            func_00347948(arg0, arg2);
            return 0;
        case 0xFF:
            return 1;
        default:
            return 3;
        }
    }
    return 0;
}

void *sdfPacGetExtensionData(u8 *arg0) {
    s32 x = arg0[0x11] & 0xF0;
    if (x <= 0) {
        return NULL;
    }
    return arg0 + 0x20;
}

void func_00347000(PacState *arg0) {
    s32 n = arg0->pendingBytes;
    s32 t = arg0->inputAvailable;
    if (t < n) {
        n = t;
    }
    if (n != 0) {
        memcpy(arg0->outputCursor, arg0->inputCursor, n);
        sdfPacAdvanceInput(arg0, n);
        arg0->outputCursor += n;
        {
            s32 r = arg0->pendingBytes - n;
            arg0->pendingBytes = r;
            if (r != 0) {
                return;
            }
        }
        arg0->onComplete(arg0);
    }
}

void func_00347088(PacState *arg0) {
    s32 n = arg0->inputAvailable;
    s32 r = func_00347988(arg0->decoder, arg0->inputCursor, n);
    sdfPacAdvanceInput(arg0, n - arg0->decoder->remaining);
    if (r == 0) {
        return;
    }
    func_00328E48(arg0->decoder);
    arg0->onComplete(arg0);
}

void func_00347100(PacState *arg0) {
    s32 n = arg0->pendingBytes;
    if (arg0->inputAvailable < n) {
        n = arg0->inputAvailable;
    }
    if (n != 0) {
        sdfPacAdvanceInput(arg0, n);
        {
            s32 r = arg0->pendingBytes - n;
            arg0->pendingBytes = r;
            if (r != 0) {
                return;
            }
        }
        arg0->onComplete(arg0);
    }
}

INCLUDE_ASM(const s32, "sdf/sdfPacDecode", func_00347168);

void func_00347290(PacState *arg0, void *arg1) {
    func_00347168(arg0, arg1);
    arg0->onComplete = func_00346C40;
}

INCLUDE_ASM(const s32, "sdf/sdfPacDecode", func_003472C0);

void func_00347320(PacState *arg0, void *arg1) {
    func_00347168(arg0, arg1);
    arg0->onComplete = func_003472C0;
}

INCLUDE_ASM(const s32, "sdf/sdfPacDecode", func_00347350);

void func_003473B0(PacState *arg0, void *arg1) {
    func_00347168(arg0, arg1);
    arg0->onComplete = func_00347350;
}

void func_003473E0(PacState *arg0) {
    PacBuf *s = arg0->buffer;
    s32 n = s->remaining;
    if (arg0->inputAvailable < n) {
        n = arg0->inputAvailable;
    }
    if (n != 0) {
        memcpy(s->cursor, arg0->inputCursor, n);
        sdfPacAdvanceInput(arg0, n);
        s->cursor += n;
        {
            s32 r = s->remaining - n;
            s->remaining = r;
            if (r != 0) {
                return;
            }
        }
        s->result = func_0032C138(sdfResourceRetainAddress(s->resourceSlot));
        sdfReleaseMemorySlot(&s->resourceSlot);
        arg0->onComplete(arg0);
    }
}

void func_00347488(PacState *arg0) {
    s32 n = arg0->pendingBytes;
    s32 r = func_00347988(arg0->decoder, arg0->inputCursor, n);
    sdfPacAdvanceInput(arg0, n - arg0->decoder->remaining);
    if (r == 0) {
        return;
    }
    {
        PacBuf *s = arg0->buffer;
        s->result = func_0032C138(sdfResourceRetainAddress(s->resourceSlot));
        sdfReleaseMemorySlot(&s->resourceSlot);
    }
    func_00328E48(arg0->decoder);
    arg0->onComplete(arg0);
}

void func_00347520(PacState *arg0) {
    PacBuf *s = arg0->buffer;
    s32 n = s->remaining;
    if (arg0->inputAvailable < n) {
        n = arg0->inputAvailable;
    }
    if (n != 0) {
        sdfPacAdvanceInput(arg0, n);
        {
            s32 r = s->remaining - n;
            s->remaining = r;
            if (r != 0) {
                return;
            }
        }
        s->result = func_0032C150(s->cursor);
        arg0->onComplete(arg0);
    }
}

INCLUDE_ASM(const s32, "sdf/sdfPacDecode", func_003475A0);

INCLUDE_ASM(const s32, "sdf/sdfPacDecode", func_003476D0);

void func_00347710(PacState *arg0, void *arg1) {
    func_00346E08(arg0, arg1);
    {
        void *p = func_00328D68(0x10);
        arg0->unk2C = (PacAlloc *)p;
        func_003475A0(arg0, arg1, p);
    }
    arg0->onComplete = func_003476D0;
}

void func_00347768(PacState *arg0) {
    func_003475A0(arg0, (u8 *)arg0->unk2C + 0x10, (u8 *)arg0->unk2C + 0x20);
    arg0->onComplete = func_003477D8;
}

INCLUDE_ASM(const s32, "sdf/sdfPacDecode", func_003477A8);

INCLUDE_ASM(const s32, "sdf/sdfPacDecode", func_003477D8);

INCLUDE_ASM(const s32, "sdf/sdfPacDecode", func_00347850);
