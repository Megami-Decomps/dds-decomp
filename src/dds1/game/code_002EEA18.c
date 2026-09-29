#include "common.h"

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EEA18);

typedef struct SdfAllocWork {
    u8 unk0[8];
    void (*handler)();
    u8 unkC[0x20];
    void *buffer;
} SdfAllocWork;

extern void func_002EDF60(SdfAllocWork *);
extern void *func_002CFEB8(s32 size);
extern void func_002EEA18();

void func_002EEAA0(SdfAllocWork *work) {
    func_002EDF60(work);
    work->buffer = func_002CFEB8(0x30);
    work->handler = func_002EEA18;
}

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EEAE0);

typedef struct {
    u8 state;    /* 0x00 */
    u8 pad01[3];
    u32 value;   /* 0x04 */
} SdfWordState;

void func_002EEE98(SdfWordState *work, u32 value) {
    work->value = value;
    work->state = 1;
}

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EEEA8);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EF2B0);

extern u8 D_00398470[];
extern void func_002DDD60();
extern void func_002EEEA8();

void func_002EF2E0(s32 a, s32 b, s32 c, s32 d) {
    func_002DDD60(D_00398470);
    func_002EEEA8(a, b, c, d);
}

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EF340);

typedef struct SdfStreamCfg {
    s32 dest;
    u8 unk4[8];
    s32 length;
} SdfStreamCfg;

extern SdfStreamCfg D_003FF340;
extern u8 D_003FF4C0[];
extern void func_002E6D48();

void func_002EF3D0(void) {
    s32 length = D_003FF340.length;

    if (length > 0x4000) {
        length = 0x4000;
    }
    func_002E6D48(D_003FF340.dest, D_003FF4C0, length);
}

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EF408);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EF560);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EF698);

extern void func_002EF698();
extern s32 func_002CF4E0();
extern void _StartThread();
extern s32 GetThreadId(void);
extern void SleepThread(void);
extern s32 D_003BDAC4;

void func_002EF788(void) {
    s32 stack = func_002CF4E0(func_002EF698, 0x1000, 0x4C);

    _StartThread(stack, 0);
    D_003BDAC4 = GetThreadId();
    SleepThread();
}

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EF7C8);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EF858);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EF8D8);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EF958);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EFA58);

typedef struct SdfPoolNode {
    struct SdfPoolNode *next;
    s32 unk4;
    u8 unk8[4];
    s32 kind;
} SdfPoolNode;

typedef struct SdfPool {
    u8 unk0[0xC];
    SdfPoolNode *free;
    u8 unk10[8];
    u8 sub[1];
} SdfPool;

extern void func_002EFA58();
extern void func_002EFC68();

void func_002EFB30(SdfPool *pool, SdfPoolNode *node) {
    if (node->unk4 != 0) {
        if (node->kind == 0xFFFF) {
            node->next = pool->free;
            pool->free = node;
        } else {
            func_002EFA58(pool->sub, node);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EFB88);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EFBF8);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EFC68);

void func_002EFCD0(SdfPool *pool, s32 mode, SdfPoolNode *node) {
    switch (mode) {
    case 0:
        func_002EFC68(pool);
        break;
    case 1:
        if (node->unk4 != 0) {
            node->next = pool->free;
            pool->free = node;
        }
        break;
    }
}

extern void func_002EFB30();
extern void func_002EFCD0();

void func_002EFD30(u32 *work) {
    work[4] = (u32)func_002EFB30;
    work[5] = (u32)func_002EFCD0;
}

INCLUDE_RODATA(const s32, "game/code_002EEA18", D_003B50C0);

INCLUDE_RODATA(const s32, "game/code_002EEA18", D_003B50D0);

INCLUDE_RODATA(const s32, "game/code_002EEA18", D_003B50E0);

INCLUDE_RODATA(const s32, "game/code_002EEA18", D_003B50F0);

