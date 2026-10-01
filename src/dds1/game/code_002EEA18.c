#include "common.h"
#include "pcp_vu0.h"

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EEA18);

typedef struct SdfAllocWork {
    u8 unk0[8];
    void (*handler)();
    u8 unkC[0x20];
    void *buffer;
} SdfAllocWork;

extern void sdfPacEnqueuePacket(SdfAllocWork *);
extern void *func_002CFEB8(s32 size);
extern void func_002EEA18();

void sdfQueueAndResetPacketWork(SdfAllocWork *work) {
    sdfPacEnqueuePacket(work);
    work->buffer = func_002CFEB8(0x30);
    work->handler = func_002EEA18;
}

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EEAE0);

typedef struct {
    u8 state;    /* 0x00 */
    u8 pad01[3];
    u32 value;   /* 0x04 */
} SdfWordState;

void sdfStoreWordAndSetState(SdfWordState *work, u32 value) {
    work->value = value;
    work->state = 1;
}

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EEEA8);

extern u8 D_00398470[];
extern void func_002DDD60();
extern void func_002EEEA8();

void func_002EF2B0(void) {
    VU0_LOAD_MATRIX(D_00398470);
    func_002EEEA8();
}

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
extern void sdfDevQueueRead();

void sdfStreamSendChunk(void) {
    s32 length = D_003FF340.length;

    if (length > 0x4000) {
        length = 0x4000;
    }
    sdfDevQueueRead(D_003FF340.dest, D_003FF4C0, length);
}

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EF408);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EF560);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EF698);

extern void func_002EF698();
extern s32 sdfCreateThreadWithAllocatedWorkspace();
extern void _StartThread();
extern s32 GetThreadId(void);
extern void SleepThread(void);
extern s32 D_003BDAC4;

void sdfStartAndSuspendWorkerThread(void) {
    s32 stack = sdfCreateThreadWithAllocatedWorkspace(func_002EF698, 0x1000, 0x4C);

    _StartThread(stack, 0);
    D_003BDAC4 = GetThreadId();
    SleepThread();
}

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EF7C8);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EF858);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EF8D8);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EF958);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EFA58);

#define SDF_POOL_FREE_KIND 0xFFFF

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

void sdfReleasePoolNode(SdfPool *pool, SdfPoolNode *node) {
    if (node->unk4 != 0) {
        if (node->kind == SDF_POOL_FREE_KIND) {
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

void sdfUpdatePoolFreeListByMode(SdfPool *pool, s32 mode, SdfPoolNode *node) {
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

extern void sdfReleasePoolNode();
extern void sdfUpdatePoolFreeListByMode();

void sdfInstallPoolNodeReleaseCallbacks(u32 *work) {
    work[4] = (u32)sdfReleasePoolNode;
    work[5] = (u32)sdfUpdatePoolFreeListByMode;
}

INCLUDE_RODATA(const s32, "game/code_002EEA18", D_003B50C0);

INCLUDE_RODATA(const s32, "game/code_002EEA18", D_003B50D0);

INCLUDE_RODATA(const s32, "game/code_002EEA18", D_003B50E0);

INCLUDE_RODATA(const s32, "game/code_002EEA18", D_003B50F0);

