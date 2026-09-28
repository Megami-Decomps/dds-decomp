#include "common.h"

extern u32 D_0043658C;

extern u32 D_00438F24;

extern u64 func_0019CE78(u64, u64, u64, u64, u64);

extern u64 func_0019CE10(u64, u64, u64, u64, u64);

extern u32 D_0043654C;

extern u32 D_0043655C;

extern u32 D_00436560;

extern u32 D_00436590;

extern s64 func_00101700(u32);

extern u64 func_0032C138(u32);

extern u64 func_00343ED0(u64, u32 *, u64);

/* Byte stream read by func_00196478/func_001964A0: base at +0x10, position at +0x18. */
typedef struct TextStream {
    u8 unk0[0x10]; /* 0x0 */
    u8 *unk10;     /* 0x10: base */
    u8 unk14[4];   /* 0x14 */
    s32 unk18;     /* 0x18: position */
} TextStream;

s32 func_0019E848(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);

extern u32 D_004528C0[];

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

/* 8-byte node header; payload follows (func_00198248/func_00198270). */
typedef struct MemNode {
    u32 unk0;              /* 0x0 */
    struct MemNode *unk4;  /* 0x4 */
} MemNode;

u32 func_0019E138(TextStream *stream) {
    s32 *ppos = &stream->unk18;
    u8 *p = stream->unk10 + *ppos;
    u32 b = *p;

    *ppos += 2;
    return (b + 0xFF) & 0xFF;
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019E160);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019E1B8);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019E5D8);

void func_0019E7C8(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_0019E848(arg0, arg1, 0, 0, 0, 0, 0x80, arg2, arg3);
}

void func_0019E800(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    func_0019E848(arg0, arg1, 0, arg2 & 0xFF, arg3 & 0xFF, arg4 & 0xFF, arg5 & 0xFF, arg6, arg7);
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019E848);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019E8A0);

u32 func_0019E8E0(u32 arg0) {
    return D_0043654C & arg0;
}

void func_0019E8F0(s32 arg0, s32 arg1) {
    D_004528C0[arg0] = arg1;
}

u32 func_0019E908(void) {
    return D_0043655C;
}

u32 func_0019E910(void) {
    return D_00436560;
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019E918);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019EC00);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019EDC0);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019EEE8);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019EF38);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F048);

void func_0019F078(void) {
    func_0019C238(4);
    func_0019C238(5);
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F098);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F0E8);

void func_0019F1B8(u64 arg0, u64 arg1, s32 arg2, u64 arg3,
                                    u64 arg4, u64 arg5) {
    u64 temp_v0;

    temp_v0 = func_0019CE10(arg4, 0, 0, 0, 0);
    func_0019D088(temp_v0, 0x10, 0x12);
    func_0019D100(temp_v0, arg0, arg1);
    func_0019D110(temp_v0, arg2 << 4);
    func_0019D178(temp_v0, arg3);
    func_0019D058(temp_v0, 0xfffffffffffffffc);
    func_0019D848(arg5, temp_v0, 0);
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F280);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F408);

void func_0019F448(void) {
    func_0019F408();
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F460);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F5E8);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F6C8);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F798);

void func_0019F878(u64 arg0, u64 arg1, s32 arg2, u64 arg3,
                                    u64 arg4, u64 arg5) {
    u64 temp_v0;

    temp_v0 = func_0019CE78(arg4, 0, 0, 0, 0);
    func_0019D088(temp_v0, 0xc, 0x10);
    func_0019D058(temp_v0, 0xfffffffffffffffd);
    func_0019D100(temp_v0, arg0, arg1);
    func_0019D110(temp_v0, arg2 << 4);
    func_0019D178(temp_v0, arg3);
    func_0019D848(arg5, temp_v0, 0);
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F940);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F990);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019FA08);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019FC38);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019FE00);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019FEF8);

u32 func_001A0038(void) {
    return 0;
}

void func_001A0040(void) {
}

void func_001A0048(void) {
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0050);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0058);

u32 func_001A0060(u32 arg0) {
    return arg0;
}

void func_001A0068(MemBlock *arg0, MemOut *arg1) {
    s32 v0 = arg0->unk0;
    s32 v1 = v0 + arg0->unk4;
    s32 v2 = v1 + arg0->unk18;

    arg1->unk0 = (u8 *)arg0 + v0;
    arg1->unk4 = (u8 *)arg0 + v1;
    arg1->unk8 = (u8 *)arg0 + v2;
}

u32 func_001A0098(u32 arg0) {
    return *(u32 *)(func_001A0060(arg0) + 0x10);
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A00B8);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A01D8);

void *func_001A0278(MemNode *queue) {
    MemNode *head = queue->unk4;

    if (head->unk0 == 0) {
        return NULL;
    }
    queue->unk4 = head->unk4;
    head->unk4 = NULL;
    return head + 1;
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A02A0);

u32 func_001A02D0(s32 arg0) {
    func_003297C8(*(u32 *)(arg0 - 4));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A02F0);

void func_001A0338(void) {
    func_0032BB68(D_00438F24);
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0350);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A03D8);

void func_001A0438(void) {
    func_0019C490(D_0043658C);
    func_001A0338();
}

u32 func_001A0458(void) {
    s64 temp_v0;
    u32 temp_v1;

    func_001A0350();
    temp_v0 = func_00101700(D_00436590);
    temp_v1 = 0xffffffff;
    if (temp_v0 != 3) {
        temp_v1 = 0;
    }
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0490);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A04F0);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0558);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A05B0);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0630);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0688);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A06D8);

u64 func_001A0710(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;
    u32 temp_v2 [4];

    temp_v0 = func_00343ED0(arg0, temp_v2, 0);
    temp_v1 = func_0032C138(temp_v2[0]);
    func_003297C8(temp_v0);
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0760);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0888);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A09C0);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0B60);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0CA0);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0E20);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0F88);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A10B0);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A1200);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A1348);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A1490);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A1508);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A1590);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A1668);

u32 func_001A1810(void) {
    return 0;
}

u32 func_001A1818(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A1820);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A1858);
