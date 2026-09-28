#include "common.h"

extern u64 func_002D3288(u32);
extern u64 func_002EB028(u64, u32 *, u64);

extern u32 D_003BB190;
extern s64 func_00101818(u32);

extern u32 D_003BB18C;

extern u32 D_003BD81C;

extern u64 func_001951C8(u64, u64, u64, u64, u64);

extern u64 func_00195160(u64, u64, u64, u64, u64);

extern s32 D_003BB168;

extern u32 D_003BB170;

extern u32 D_003BB16C;

/* Byte stream read by func_00196478/func_001964A0: base at +0x10, position at +0x18. */
typedef struct TextStream {
    u8 unk0[0x10]; /* 0x0 */
    u8 *unk10;     /* 0x10: base */
    u8 unk14[4];   /* 0x14 */
    s32 unk18;     /* 0x18: position */
} TextStream;

/* 8-byte node header; payload follows (func_00198248/func_00198270). */
typedef struct MemNode {
    u32 unk0;              /* 0x0 */
    struct MemNode *unk4;  /* 0x4 */
} MemNode;

/* Field block split by func_00198038. */
typedef struct MemBlock {
    s32 unk0; /* 0x0 */
    s32 unk4; /* 0x4 */
    u8 unk8[0x10]; /* 0x8 */
    s32 unk18; /* 0x18 */
} MemBlock;

typedef struct MemOut {
    void *unk0; /* 0x0 */
    void *unk4; /* 0x4 */
    void *unk8; /* 0x8 */
} MemOut;

/* Value with u16 pair read by func_001971E0/func_00197200. */
typedef struct Unk6C84Val {
    u8 unk0[0x10]; /* 0x0 */
    u16 unk10;     /* 0x10 */
    u16 unk12;     /* 0x12 */
} Unk6C84Val;

/* 0x24-byte record pointing at the value. */
typedef struct Unk6C84Rec {
    Unk6C84Val *unk0; /* 0x0 */
    u8 unk4[0x20];    /* 0x4 */
} Unk6C84Rec;

extern u32 D_003BB15C;
extern u32 D_003D6E20[];
extern Unk6C84Rec D_003D6C84[];
s32 func_00196B30(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);

u32 func_00196478(TextStream *stream) {
    s32 *ppos = &stream->unk18;
    u8 *p = stream->unk10 + *ppos;
    u32 b = *p;

    *ppos += 2;
    return (b + 0xFF) & 0xFF;
}

INCLUDE_ASM(const s32, "game/code_00196478", func_001964A0);

INCLUDE_ASM(const s32, "game/code_00196478", func_001964F8);

INCLUDE_ASM(const s32, "game/code_00196478", func_001968C0);

void func_00196AB0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_00196B30(arg0, arg1, 0, 0, 0, 0, 0x80, arg2, arg3);
}

void func_00196AE8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    func_00196B30(arg0, arg1, 0, arg2 & 0xFF, arg3 & 0xFF, arg4 & 0xFF, arg5 & 0xFF, arg6, arg7);
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00196B30);

INCLUDE_ASM(const s32, "game/code_00196478", func_00196B88);

u32 func_00196BB0(u32 arg0) {
    return D_003BB15C & arg0;
}

void func_00196BC0(s32 arg0, s32 arg1) {
    D_003D6E20[arg0] = arg1;
}

u32 func_00196BD8(void) {
    return D_003BB16C;
}

u32 func_00196BE0(void) {
    return D_003BB170;
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00196BE8);

INCLUDE_ASM(const s32, "game/code_00196478", func_00196ED0);

INCLUDE_ASM(const s32, "game/code_00196478", func_00197068);

INCLUDE_ASM(const s32, "game/code_00196478", func_00197190);

u16 func_001971E0(s32 arg0) {
    return D_003D6C84[arg0].unk0->unk10;
}

u16 func_00197200(s32 arg0) {
    return D_003D6C84[arg0].unk0->unk12;
}

void func_00197220(s32 arg0) {
    if (arg0 < 1) {
        arg0 = 0x14;
    }
    D_003BB168 = arg0;
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00197238);

INCLUDE_ASM(const s32, "game/code_00196478", func_00197348);

void func_00197378(void) {
    func_001945A8(4);
    func_001945A8(5);
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00197398);

INCLUDE_ASM(const s32, "game/code_00196478", func_001973E8);

void func_001974B8(u64 arg0, u64 arg1, s32 arg2, u64 arg3,
                                    u64 arg4, u64 arg5) {
    u64 temp_v0;

    temp_v0 = func_00195160(arg4, 0, 0, 0, 0);
    func_001953D8(temp_v0, 0x10, 0x12);
    func_00195450(temp_v0, arg0, arg1);
    func_00195460(temp_v0, arg2 << 4);
    func_001954C8(temp_v0, arg3);
    func_001953A8(temp_v0, 0xfffffffffffffffc);
    func_00195B78(arg5, temp_v0, 0);
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00197580);

INCLUDE_ASM(const s32, "game/code_00196478", func_00197708);

void func_00197748(void) {
    func_00197708();
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00197760);

INCLUDE_ASM(const s32, "game/code_00196478", func_001978E8);

INCLUDE_ASM(const s32, "game/code_00196478", func_001979C8);

INCLUDE_ASM(const s32, "game/code_00196478", func_00197A98);

void func_00197B78(u64 arg0, u64 arg1, s32 arg2, u64 arg3,
                                    u64 arg4, u64 arg5) {
    u64 temp_v0;

    temp_v0 = func_001951C8(arg4, 0, 0, 0, 0);
    func_001953D8(temp_v0, 0xc, 0x10);
    func_001953A8(temp_v0, 3);
    func_00195450(temp_v0, arg0, arg1);
    func_00195460(temp_v0, arg2 << 4);
    func_001954C8(temp_v0, arg3);
    func_00195B78(arg5, temp_v0, 0);
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00197C40);

INCLUDE_ASM(const s32, "game/code_00196478", func_00197E08);

INCLUDE_ASM(const s32, "game/code_00196478", func_00197EC8);

u32 func_00198008(void) {
    return 0;
}

void func_00198010(void) {
}

void func_00198018(void) {
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00198020);

INCLUDE_ASM(const s32, "game/code_00196478", func_00198028);

u32 func_00198030(u32 arg0) {
    return arg0;
}

void func_00198038(MemBlock *arg0, MemOut *arg1) {
    s32 v0 = arg0->unk0;
    s32 v1 = v0 + arg0->unk4;
    s32 v2 = v1 + arg0->unk18;

    arg1->unk0 = (u8 *)arg0 + v0;
    arg1->unk4 = (u8 *)arg0 + v1;
    arg1->unk8 = (u8 *)arg0 + v2;
}

u32 func_00198068(u32 arg0) {
    return *(u32 *)(func_00198030(arg0) + 0x10);
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00198088);

INCLUDE_ASM(const s32, "game/code_00196478", func_001981A8);

void *func_00198248(MemNode *queue) {
    MemNode *head = queue->unk4;

    if (head->unk0 == 0) {
        return NULL;
    }
    queue->unk4 = head->unk4;
    head->unk4 = NULL;
    return head + 1;
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00198270);

u32 func_001982A0(s32 arg0) {
    func_002D0918(*(u32 *)(arg0 - 4));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00196478", func_001982C0);

void func_00198308(void) {
    func_002D2CB8(D_003BD81C);
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00198320);

INCLUDE_ASM(const s32, "game/code_00196478", func_001983A8);

void func_00198408(void) {
    func_00194800(D_003BB18C);
    func_00198308();
}

u32 func_00198428(void) {
    s64 temp_v0;
    u32 temp_v1;

    func_00198320();
    temp_v0 = func_00101818(D_003BB190);
    temp_v1 = 0xffffffff;
    if (temp_v0 != 3) {
        temp_v1 = 0;
    }
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00198460);

INCLUDE_ASM(const s32, "game/code_00196478", func_001984C0);

INCLUDE_ASM(const s32, "game/code_00196478", func_00198528);

INCLUDE_ASM(const s32, "game/code_00196478", func_00198580);

INCLUDE_ASM(const s32, "game/code_00196478", func_00198600);

INCLUDE_ASM(const s32, "game/code_00196478", func_00198658);

INCLUDE_ASM(const s32, "game/code_00196478", func_001986A8);

u64 func_001986E0(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;
    u32 temp_v2 [4];

    temp_v0 = func_002EB028(arg0, temp_v2, 0);
    temp_v1 = func_002D3288(temp_v2[0]);
    func_002D0918(temp_v0);
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00198730);

INCLUDE_ASM(const s32, "game/code_00196478", func_00198858);

INCLUDE_ASM(const s32, "game/code_00196478", func_00198990);

INCLUDE_ASM(const s32, "game/code_00196478", func_00198B30);

INCLUDE_ASM(const s32, "game/code_00196478", func_00198C70);

INCLUDE_ASM(const s32, "game/code_00196478", func_00198DF0);

INCLUDE_ASM(const s32, "game/code_00196478", func_00198F58);

INCLUDE_ASM(const s32, "game/code_00196478", func_00199080);

INCLUDE_ASM(const s32, "game/code_00196478", func_001991D0);

INCLUDE_ASM(const s32, "game/code_00196478", func_00199318);

INCLUDE_ASM(const s32, "game/code_00196478", func_00199460);

INCLUDE_ASM(const s32, "game/code_00196478", func_001994D8);

INCLUDE_ASM(const s32, "game/code_00196478", func_00199560);

INCLUDE_ASM(const s32, "game/code_00196478", func_00199638);

u32 func_001997E0(void) {
    return 0;
}

u32 func_001997E8(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00196478", func_001997F0);

INCLUDE_ASM(const s32, "game/code_00196478", func_00199828);
