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

/* Relocation record embedded in the work item's data stream. */
typedef struct PacReloc {
    s32 offset; /* 0x00: displacement from the payload */
    s32 count;  /* 0x04: number of relocation bytes */
    u8 pad08[8];
    u8 payload[1]; /* 0x10 */
} PacReloc;

/* Extended PAC header: the second header's flags precede data at 0x20. */
typedef struct PacExtensionHeader {
    u8 pad00[0x11];
    u8 extensionFlags; /* 0x11 */
    u8 pad12[0xE];
    u8 data[1];        /* 0x20 */
} PacExtensionHeader;

typedef struct PacWork {
    struct PacWork *next; /* 0x0 */
    struct PacState *owner; /* 0x4 */
    s32 resourceHandle; /* 0x8 */
    u8 *dataCursor; /* 0xC */
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

void func_002EE2C0(PacState *arg0, PacHead *arg1);
void func_002EDD98(PacState *arg0);
void func_002EB278(void *arg0, void *arg1, void *arg2, s32 arg3);
void func_002CFF98(void *arg0);
void sdfPacAdvanceInput(PacState *arg0, s32 arg1);
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
void func_002EE3E8(PacState *arg0, PacHead *arg1);
void func_002EE478(PacState *arg0, PacHead *arg1);
void func_002EE508(PacState *arg0, PacHead *arg1);
void func_002EE868(PacState *arg0, PacHead *arg1);
void func_002EEAA0(PacState *arg0, void *arg1);
void func_002DA058(s32 arg0, s32 arg1);
s32 func_002D32A0(void *arg0);
s32 sdfResourceRetainAddress(s32 arg0);
s32 func_002D3288(s32 arg0);
s32 func_002D0518(s32 arg0);
s32 func_002D03F8(s32 arg0);
void sdfReleaseMemorySlot(void *arg0);
void func_002EEE98(void *arg0, void *arg1);
s32 func_002EEAE0(void *arg0, void *arg1, s32 arg2);
extern void *memcpy(void *dst, const void *src, u32 n);


INCLUDE_ASM(const s32, "sdf/sdfPacDecode", func_002EDE48);

PacWork *func_002EDF60(PacState *arg0, PacHead *arg1) {
    s32 size = arg1->unk1 & 0xF0;
    PacWork *node = func_002CFF68(size + 0x20);
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

PacWork *func_002EDFE8(PacWork *arg0) {
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
void *sdfPacGetExtensionData(PacExtensionHeader *header) {
    s32 extensionSize = header->extensionFlags & 0xF0;
    if (extensionSize <= 0) {
        return NULL;
    }
    return header->data;
}

void func_002EE158(PacState *arg0) {
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

void func_002EE1E0(PacState *arg0) {
    s32 n = arg0->inputAvailable;
    s32 r = func_002EEAE0(arg0->decoder, arg0->inputCursor, n);
    sdfPacAdvanceInput(arg0, n - arg0->decoder->remaining);
    if (r == 0) {
        return;
    }
    func_002CFF98(arg0->decoder);
    arg0->onComplete(arg0);
}

void func_002EE258(PacState *arg0) {
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

void func_002EE2C0(PacState *arg0, PacHead *arg1) {
    s32 v = arg1->unkC;
    if (v == 0) {
        v = arg1->unk4 + (arg1->unk1 & 0xF0) - 0x10;
    }
    if (arg0->unk1 & 1) {
        PacWork *node = func_002EDF60(arg0, arg1);
        node->dataCursor = arg1->payload;
        arg0->unk8 = func_002EE258;
    } else {
        PacWork *node;
        arg0->unk0 = 2;
        node = func_002EDF60(arg0, arg1);
        if (arg0->unk1 & 2) {
            node->resourceHandle = func_002D0518(v);
        } else {
            node->resourceHandle = func_002D03F8(v);
        }
        arg0->outputCursor = node->dataCursor = (u8 *)sdfResourceRetainAddress(node->resourceHandle);
        switch (arg1->unk1 & 0xF) {
        case 0:
            arg0->unk8 = func_002EE158;
            break;
        case 1: {
            PacBuf *tmp = func_002CFEB8(0x20);
            arg0->decoder = tmp;
            func_002EEE98(tmp, arg0->outputCursor);
            arg0->unk8 = func_002EE1E0;
            break;
        }
        default:
            break;
        }
    }
}

void func_002EE3E8(PacState *arg0, PacHead *arg1) {
    func_002EE2C0(arg0, arg1);
    arg0->onComplete = func_002EDD98;
}

void func_002EE418(PacState *state) {
    PacWork *work = state->queueTail;
    PacReloc *record = (PacReloc *)work->dataCursor;
    u8 *payload = record->payload;
    s32 count = record->count;
    work->dataCursor = payload;
    if (count != 0) {
        func_002EB278(payload, payload, payload + record->offset, count);
        record->count = 0;
    }
    func_002EDD98(state);
}

void func_002EE478(PacState *arg0, PacHead *arg1) {
    func_002EE2C0(arg0, arg1);
    arg0->onComplete = func_002EE418;
}

void func_002EE4A8(PacState *state) {
    PacWork *work = state->queueTail;
    PacReloc *record = (PacReloc *)work->dataCursor;
    u8 *payload = record->payload;
    s32 count = record->count;
    work->dataCursor = payload;
    if (count != 0) {
        func_002EB278(payload, payload, payload + record->offset, count);
        record->count = 0;
    }
    func_002EDD98(state);
}

void func_002EE508(PacState *arg0, PacHead *arg1) {
    func_002EE2C0(arg0, arg1);
    arg0->onComplete = func_002EE4A8;
}

void func_002EE538(PacState *arg0) {
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
        s->result = func_002D3288(sdfResourceRetainAddress(s->resourceSlot));
        sdfReleaseMemorySlot(&s->resourceSlot);
        arg0->onComplete(arg0);
    }
}

void func_002EE5E0(PacState *arg0) {
    s32 n = arg0->pendingBytes;
    s32 r = func_002EEAE0(arg0->decoder, arg0->inputCursor, n);
    sdfPacAdvanceInput(arg0, n - arg0->decoder->remaining);
    if (r == 0) {
        return;
    }
    {
        PacBuf *s = arg0->buffer;
        s->result = func_002D3288(sdfResourceRetainAddress(s->resourceSlot));
        sdfReleaseMemorySlot(&s->resourceSlot);
    }
    func_002CFF98(arg0->decoder);
    arg0->onComplete(arg0);
}

void func_002EE678(PacState *arg0) {
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
        s->result = func_002D32A0(s->cursor);
        arg0->onComplete(arg0);
    }
}

INCLUDE_ASM(const s32, "sdf/sdfPacDecode", func_002EE6F8);

INCLUDE_ASM(const s32, "sdf/sdfPacDecode", func_002EE828);

void func_002EE868(PacState *arg0, PacHead *arg1) {
    func_002EDF60(arg0, arg1);
    {
        void *p = func_002CFEB8(0x10);
        arg0->unk2C = (PacAlloc *)p;
        func_002EE6F8(arg0, arg1, p);
    }
    arg0->onComplete = func_002EE828;
}


void func_002EE8C0(PacState *arg0) {
    func_002EE6F8(arg0, (u8 *)arg0->unk2C + 0x10, (u8 *)arg0->unk2C + 0x20);
    arg0->onComplete = func_002EE930;
}


INCLUDE_ASM(const s32, "sdf/sdfPacDecode", func_002EE900);

void func_002EE930(PacState *arg0) {
    PacAlloc *p = arg0->unk2C;
    func_002DA058(arg0->queueTail->resourceHandle, p->unk20);
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

void func_002EE9A8(PacState *state) {
    s32 available = state->inputAvailable;
    if (state->pendingBytes < available) {
        available = state->pendingBytes;
    }
    sdfPacAdvanceInput(state, available);
    {
        s32 remaining = state->pendingBytes - available;
        state->pendingBytes = remaining;
        if (remaining != 0) {
            return;
        }
    }
    func_002EE900(state);
}

