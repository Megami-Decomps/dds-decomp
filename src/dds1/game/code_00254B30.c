#include "common.h"

extern void func_0024DA58(s32 arg0);

extern void func_0024E260(s32, s32, s32, s32, s32, s32);

extern s32 func_002CFEB8(u32);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00254B30);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00254C30);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00254C68);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00254E48);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00254EF0);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00255010);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00255118);

INCLUDE_ASM(const s32, "game/code_00254B30", func_002551B8);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00255368);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00255508);

INCLUDE_ASM(const s32, "game/code_00254B30", func_002555E8);

INCLUDE_ASM(const s32, "game/code_00254B30", func_002556C8);

void itfDspSignalA(void) {
    func_0024DA58(3);
}

void itfDspSignalB(void) {
    func_0024DA58(4);
}

void itfDspSignalC(void) {
    func_0024DA58(5);
}

void itfDspSignalD(void) {
    func_0024DA58(6);
}

void itfDspSignalE(void) {
    func_0024DA58(7);
}

INCLUDE_ASM(const s32, "game/code_00254B30", func_00255838);

INCLUDE_ASM(const s32, "game/code_00254B30", func_002559F8);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00255A98);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00255B78);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00255D00);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00255E08);

void itfDspDrawMarksA(s32 arg0, s32 arg1) {
    func_0024E260(0, 0, 0, arg0, 0x3A, arg1);
    func_0024E260(0, 0, 0, arg0, 0x38, arg1);
    func_0024E260(0, 0, 0, arg0, 0x39, arg1);
}

void itfDspDrawMarksB(s32 arg0, s32 arg1) {
    func_0024E260(0, 0, 0, arg0, 0xF, arg1);
    func_0024E260(0, 0, 0, arg0, 0x10, arg1);
    func_0024E260(0, 0, 0, arg0, 0x12, arg1);
}

INCLUDE_ASM(const s32, "game/code_00254B30", func_00255FF8);

void itfDspDrawStrip(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_0024E260(arg0, arg1, arg2, arg3, 4, arg4);
    func_0024E260(arg0, arg1, arg2, arg3, 5, arg4);
    func_0024E260(arg0, arg1, arg2, arg3, 0xA, arg4);
    func_0024E260(arg0, arg1, arg2, arg3, 0xB, arg4);
    func_0024E260(arg0, arg1, arg2, arg3, 0xC, arg4);
    func_0024E260(arg0, arg1, arg2, arg3, 0xD, arg4);
}

INCLUDE_ASM(const s32, "game/code_00254B30", func_002561A0);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00256290);

INCLUDE_ASM(const s32, "game/code_00254B30", func_002562E8);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00256400);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00256540);

typedef struct {
    u8 pad00[0x10];
    u32 next; /* 0x10 */
} DspListNode;

typedef struct {
    u32 pad00[2];
    s32 first; /* 0x08 */
} DspListHead;

s32 *mnuAllocateDisplayListNode(void) {
    s32 *node = (s32 *)func_002CFEB8(0x14);

    memset(node, 0, 0x14);
    return node;
}

INCLUDE_ASM(const s32, "game/code_00254B30", func_00256B78);

u32 func_00256C00(DspListNode *node) {
    u32 next;

    next = node->next;
    func_002CFF98();
    return next;
}

void mnuReleaseDisplayListNodes(DspListHead *head) {
    s32 node = head->first;

    while (node != NULL) {
        node = func_00256C00(node);
    }
}

INCLUDE_ASM(const s32, "game/code_00254B30", func_00256C58);

void itfDspDrawScaledA(s32 arg0, s32 arg1, s32 arg2) {
    func_0024E260(0, 0, 0, arg1, 0x16, arg2);
    func_0024E260(0, 0, 0, arg1 * 0.5f, 0x18, arg2);
}

void itfDspDrawScaledB(s32 arg0, s32 arg1, s32 arg2) {
    func_0024E260(0, 0, 0, arg1, 0x17, arg2);
    func_0024E260(0, 0, 0, arg1 * 0.5f, 0x19, arg2);
}

void itfDspDrawPair(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_0024E260(arg0, arg1, arg2, arg3, 0x14, arg4);
    func_0024E260(arg0, arg1, arg2, arg3, 0x15, arg4);
}

void func_00256E88(void) {
}

INCLUDE_ASM(const s32, "game/code_00254B30", func_00256E90);

INCLUDE_ASM(const s32, "game/code_00254B30", func_002570A8);

INCLUDE_SDATA(const s32, "game/code_00254B30", D_003BC448);

INCLUDE_SDATA(const s32, "game/code_00254B30", D_003BC450);

INCLUDE_SDATA(const s32, "game/code_00254B30", D_003BC458);

INCLUDE_SDATA(const s32, "game/code_00254B30", D_003BC460);

INCLUDE_SDATA(const s32, "game/code_00254B30", D_003BC468);

INCLUDE_SDATA(const s32, "game/code_00254B30", D_003BC470);

INCLUDE_SDATA(const s32, "game/code_00254B30", D_003BC478);

INCLUDE_SDATA(const s32, "game/code_00254B30", D_003BC480);

