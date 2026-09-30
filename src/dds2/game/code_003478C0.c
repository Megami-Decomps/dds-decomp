#include "common.h"
#include "pcp_vu0.h"

typedef struct SdfRequest {
    u8 active;
    u8 pad01[3];
    u32 value;
} SdfRequest;

extern void sdfReleasePoolNode();

extern void func_00348B78();

typedef struct SdfAllocWork {
    u8 unk0[8];
    void (*handler)();
    u8 unkC[0x20];
    void *buffer;
} SdfAllocWork;

extern void sdfPacEnqueuePacket(SdfAllocWork *);

extern void *func_00328D68(s32 size);

extern void func_003478C0();

extern u8 D_0040B620[];

extern void func_00336C10();

extern void func_00347D50();

typedef struct SdfStreamCfg {
    s32 dest;
    u8 unk4[8];
    s32 length;
} SdfStreamCfg;

extern SdfStreamCfg D_0047BCC0;

extern u8 D_0047BE40[];

extern void sdfDevQueueRead();

extern void func_00348540();

extern s32 func_00328390();

extern void _StartThread();

extern s32 GetThreadId(void);

extern void SleepThread(void);

extern s32 D_00439224;

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

extern void func_00348900();

extern void func_00348B10();

INCLUDE_ASM(const s32, "game/code_003478C0", func_003478C0);

void func_00347948(SdfAllocWork *work) {
    sdfPacEnqueuePacket(work);
    work->buffer = func_00328D68(0x30);
    work->handler = func_003478C0;
}

INCLUDE_ASM(const s32, "game/code_003478C0", func_00347988);

void sdfStoreWordAndSetState(SdfRequest *request, u32 value) {
    request->value = value;
    request->active = 1;
}

INCLUDE_ASM(const s32, "game/code_003478C0", func_00347D50);

void func_00348158(void) {
    VU0_LOAD_MATRIX(D_0040B620);
    func_00347D50();
}

void func_00348188(s32 a, s32 b, s32 c, s32 d) {
    func_00336C10(D_0040B620);
    func_00347D50(a, b, c, d);
}

INCLUDE_ASM(const s32, "game/code_003478C0", func_003481E8);

void sdfStreamSendChunk(void) {
    s32 length = D_0047BCC0.length;

    if (length > 0x4000) {
        length = 0x4000;
    }
    sdfDevQueueRead(D_0047BCC0.dest, D_0047BE40, length);
}

INCLUDE_ASM(const s32, "game/code_003478C0", func_003482B0);

INCLUDE_ASM(const s32, "game/code_003478C0", func_00348408);

INCLUDE_ASM(const s32, "game/code_003478C0", func_00348540);

void func_00348630(void) {
    s32 stack = func_00328390(func_00348540, 0x1000, 0x4C);

    _StartThread(stack, 0);
    D_00439224 = GetThreadId();
    SleepThread();
}

INCLUDE_ASM(const s32, "game/code_003478C0", func_00348670);

INCLUDE_ASM(const s32, "game/code_003478C0", func_00348700);

INCLUDE_ASM(const s32, "game/code_003478C0", func_00348780);

INCLUDE_ASM(const s32, "game/code_003478C0", func_00348800);

INCLUDE_ASM(const s32, "game/code_003478C0", func_00348900);

void sdfReleasePoolNode(SdfPool *pool, SdfPoolNode *node) {
    if (node->unk4 != 0) {
        if (node->kind == SDF_POOL_FREE_KIND) {
            node->next = pool->free;
            pool->free = node;
        } else {
            func_00348900(pool->sub, node);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_003478C0", func_00348A30);

INCLUDE_ASM(const s32, "game/code_003478C0", func_00348AA0);

INCLUDE_ASM(const s32, "game/code_003478C0", func_00348B10);

void func_00348B78(SdfPool *pool, s32 mode, SdfPoolNode *node) {
    switch (mode) {
    case 0:
        func_00348B10(pool);
        break;
    case 1:
        if (node->unk4 != 0) {
            node->next = pool->free;
            pool->free = node;
        }
        break;
    }
}

void func_00348BD8(u32 *work) {
    work[4] = (u32)sdfReleasePoolNode;
    work[5] = (u32)func_00348B78;
}

INCLUDE_RODATA(const s32, "game/code_003478C0", D_0042EDD0);

INCLUDE_RODATA(const s32, "game/code_003478C0", D_0042EDE0);

INCLUDE_RODATA(const s32, "game/code_003478C0", D_0042EDF0);

INCLUDE_RODATA(const s32, "game/code_003478C0", D_0042EE00);

