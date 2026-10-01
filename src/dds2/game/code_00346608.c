#include "common.h"

typedef struct SdfPacFlags {
    u8 pad00;
    u8 flags;
} SdfPacFlags;

typedef struct SdfPacInput {
    u8 pad00[0x10];
    s32 cursor;
    s32 remaining;
    s32 processed;
} SdfPacInput;

typedef struct SdfPacWork {
    u8 pad00[0x33];
    u8 status33;
    u8 pad34[0xA];
    u8 status3E;
} SdfPacWork;

INCLUDE_ASM(const s32, "game/code_00346608", func_00346608);

INCLUDE_ASM(const s32, "game/code_00346608", func_00346778);

INCLUDE_ASM(const s32, "game/code_00346608", func_00346988);

s32 func_00346A60(SdfPacWork *work) {
    if (work->status33 == 0) {
        return 0;
    }
    return work->status3E == 0;
}

INCLUDE_ASM(const s32, "game/code_00346608", func_00346A80);

void func_00346AD8(SdfPacFlags *packet) {
    packet->flags = packet->flags | 1;
}

void func_00346AE8(SdfPacFlags *packet) {
    packet->flags = packet->flags | 2;
}

INCLUDE_ASM(const s32, "game/code_00346608", func_00346AF8);

void func_00346B30(u8 *status) {
    *status = 0xff;
}

void sdfPacAdvanceInput(SdfPacInput *input, s32 count) {
    input->cursor = input->cursor + count;
    input->remaining = input->remaining - count;
    input->processed = input->processed + count;
}

INCLUDE_ASM(const s32, "game/code_00346608", func_00346B68);

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
void func_00346C40(PacDecoder *decoder) {
    s32 result;
    PacNode *tail;
    PacNode *node;
    PacNode *next;

    result = decoder->read(decoder, 1, (u8 *)decoder->tail + 0x10);
    if (result == 1) {
        func_00346B30((u8 *)decoder);
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

INCLUDE_SDATA(const s32, "game/code_00346608", D_00438D28);

