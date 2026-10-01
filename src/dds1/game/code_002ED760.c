#include "common.h"

typedef struct SdfPacDispatchPacket {
    u16 unk00;
    s16 status;
    void *dispatch;
    u8 pad08[0x30];
} SdfPacDispatchPacket;

extern u8 sdfPacDispatchPacket[];


INCLUDE_ASM(const s32, "game/code_002ED760", func_002ED760);

INCLUDE_ASM(const s32, "game/code_002ED760", func_002ED8D0);

INCLUDE_ASM(const s32, "game/code_002ED760", func_002EDAE0);

s32 func_002EDBB8(u8 *work) {
    if (*(u8 *)(work + 0x33) == 0) {
        return 0;
    }
    return *(u8 *)(work + 0x3E) == 0;
}

void func_002EDBD8(SdfPacDispatchPacket *packet, void *dispatch) {
    memset(packet, 0, sizeof(*packet));
    packet->status = -1;
    if (dispatch != NULL) {
        packet->dispatch = dispatch;
    } else {
        packet->dispatch = sdfPacDispatchPacket;
    }
}

/* Prefix of the PAC decode state used by the flag and stream cursors. */
typedef struct {
    u8 state;
    u8 flags;
    u8 pad02[0xE];
    u8 *input;     /* 0x10 */
    s32 available; /* 0x14 */
    s32 consumed;  /* 0x18 */
} PacStatePrefix;

void func_002EDC30(PacStatePrefix *state) {
    state->flags = state->flags | 1;
}

void func_002EDC40(PacStatePrefix *state) {
    state->flags = state->flags | 2;
}

INCLUDE_ASM(const s32, "game/code_002ED760", func_002EDC50);

void func_002EDC88(u8 *marker) {
    *marker = 0xff;
}

void sdfPacAdvanceInput(PacStatePrefix *state, s32 count) {
    state->input = state->input + count;
    state->available = state->available - count;
    state->consumed = state->consumed + count;
}

typedef struct PacCursorBuffer {
    u8 pad00;
    u8 flags;
    u8 pad02[2];
    u32 available;
} PacCursorBuffer;

typedef union PacCallbackState {
    PacStatePrefix prefix;
    struct {
        u8 active;
        u8 flags;
        u16 count;
        s32 (*callback)(void *, s32, void *, s32);
        u8 pad08[8];
        PacCursorBuffer *input;
        s32 available;
        u32 consumed;
        u32 pad1C;
        u32 remainder;
    } work;
} PacCallbackState;

void func_002EDCC0(PacCallbackState *state) {
    PacCursorBuffer *input;
    u32 cursor;
    u32 available;
    u32 bitOffset;
    u32 remainder;
    s32 (*callback)(void *, s32, void *, s32);
    s32 result;

    cursor = state->work.consumed & 0x3F;
    if (cursor != 0) {
        state->work.active = 1;
        state->work.remainder = 0x40 - cursor;
        return;
    }
    input = state->work.input;
    sdfPacAdvanceInput(&state->prefix, 0x10);
    available = input->available;
    state->work.count++;
    bitOffset = (input->flags & 0xF0) + 0x10;
    remainder = available - bitOffset;
    state->work.remainder = remainder;
    callback = state->work.callback;
    result = callback(state, 0, input, remainder);
    if (result == 1) {
        func_002EDC88(&state->work.active);
        return;
    }
    if (result == 2) {
        state->work.active = 1;
        state->work.remainder = (state->work.remainder + 0x3F) & ~0x3F;
    } else {
        state->work.active = 2;
    }
}

typedef struct PacNode {
    struct PacNode *next; /* 0x00 */
    u8 data[0x10];
} PacNode;

/* Decoder object: read callback at +4 and a singly linked list of buffers (head/tail at +0x30/+0x34). */
typedef struct PacDecoder {
    u8 state;      /* 0x00 */
    u8 pad01[3];
    s32 (*read)(struct PacDecoder *, s32, void *); /* 0x04 */
    u8 pad08[0x28];
    PacNode *head; /* 0x30 */
    PacNode *tail; /* 0x34 */
} PacDecoder;

extern void sdfReleaseChipBlock(void *);

/* Poll the decoder's read callback for the newest buffer: result 1 marks the object done, result 4 drops the tail buffer. */
void sdfDecodePacNodeAndAdvanceTail(PacDecoder *decoder) {
    s32 result;
    PacNode *tail;
    PacNode *node;
    PacNode *next;

    result = decoder->read(decoder, 1, (u8 *)decoder->tail + 0x10);
    if (result == 1) {
        func_002EDC88((u8 *)decoder);
        return;
    }
    if (result == 4) {
        tail = decoder->tail;
        node = decoder->head;
        if (node == tail) {
            decoder->tail = NULL;
            decoder->head = NULL;
        } else {
            while ((next = node->next) != tail) {
                node = next;
            }
            node->next = NULL;
        }
        sdfReleaseChipBlock(tail);
    }
    decoder->state = 0;
}

INCLUDE_SDATA(const s32, "game/code_002ED760", D_003BD638);

