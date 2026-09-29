#include "common.h"

extern void func_0024DA58(s32 arg0);

extern void func_0024E260(s32, s32, s32, s32, s32, s32);

extern s32 func_002CFEB8(u32);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00254B30);

extern void mnuCallInitWide(s32, s32, s32, s32, s32);

void func_00254C30(s32 x, s32 y, s32 z, s32 value, s32 ctx, s32 param) {
    s32 unit = *(s32 *)(ctx + 0xC);

    **(s32 **)(unit + 0x30) = value;
    mnuCallInitWide(x << 4, y << 3, z, unit, param);
}

INCLUDE_ASM(const s32, "game/code_00254B30", func_00254C68);

extern void *memset(void *, s32, u32);
extern void func_002CD0D8(u32, s32, void *);
extern void func_002CA858(s32, s32, s32, u32, s32, void *, u32, s32);

void func_00254E48(s32 a, s32 b, s32 c, u32 d, u32 e, s32 f) {
    u32 tag = d | 0xA09DC300;
    u32 id = e & 0xFFFF;
    u8 buf[0x20];

    memset(buf, 0, 0x20);
    func_002CD0D8(id, 1, buf);
    func_002CA858(a + 0x35, b + 0x136, c, tag, 4, buf, 0x80000000, f);
}

INCLUDE_ASM(const s32, "game/code_00254B30", func_00254EF0);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00255010);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00255118);

INCLUDE_ASM(const s32, "game/code_00254B30", func_002551B8);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00255368);

typedef struct MenuSlot17 {
    u8 b[17];
} MenuSlot17;

typedef struct MenuSlot19 {
    u8 b[19];
} MenuSlot19;

extern MenuSlot17 *D_003BAA70;
extern MenuSlot19 *D_003BAA78;
extern char D_003BC468[];
extern s32 func_00253608();
extern void func_0024DD90(s32, void *);
extern s32 func_0024F800(s32);
extern void func_003014F0(void *, void *, s32);
extern void func_0024DAE8(s32);
extern void func_0024DA58(s32);
extern void func_0024DAB8(s32);

void func_00255508(void) {
    s32 *slot = (s32 *)func_0024FA18();
    u8 *entry = (u8 *)func_00253608();
    char text[16];

    func_0024DD90(0, &D_003BAA70[*(u16 *)(*(s32 *)slot + 4)]);
    func_0024DD90(1, &D_003BAA78[*(s32 *)((u8 *)slot + 4)]);
    func_0024DD90(2, &D_003BAA78[*(u16 *)(entry + 0xC)]);
    func_003014F0(text, D_003BC468, func_0024F800(*(u16 *)(entry + 0xC)));
    func_0024DD90(3, text);
    func_0024DAE8(0);
    func_0024DA58(0);
    func_0024DAB8(8);
}

void func_002555E8(void) {
    s32 *slot = (s32 *)func_0024FA18();
    u8 *entry = (u8 *)func_00253608();
    char text[16];

    func_0024DD90(0, &D_003BAA70[*(u16 *)(*(s32 *)slot + 4)]);
    func_0024DD90(1, &D_003BAA78[*(s32 *)((u8 *)slot + 4)]);
    func_0024DD90(2, &D_003BAA78[*(u16 *)(entry + 0xC)]);
    func_003014F0(text, D_003BC468, func_0024F800(*(u16 *)(entry + 0xC)));
    func_0024DD90(3, text);
    func_0024DAE8(0);
    func_0024DA58(1);
    func_0024DAB8(8);
}

void func_002556C8(void) {
    s32 *slot = (s32 *)func_0024FA18();
    u8 *entry = (u8 *)func_00253608();
    char text[16];

    func_0024DD90(0, &D_003BAA70[*(u16 *)(*(s32 *)slot + 4)]);
    func_0024DD90(1, &D_003BAA78[*(s32 *)((u8 *)slot + 4)]);
    func_0024DD90(2, &D_003BAA78[*(u16 *)(entry + 0xC)]);
    func_003014F0(text, D_003BC468, func_0024F800(*(u16 *)(entry + 0xC)));
    func_0024DD90(3, text);
    func_0024DA58(2);
}

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

typedef struct Bytes7 {
    s8 b[7];
} Bytes7;

extern Bytes7 D_003BC478[];

void func_00256290(s32 a, s32 b, s32 c, s32 entry, s32 d, s32 e) {
    Bytes7 table = D_003BC478[0];

    func_0024E260(a, b, c, d, table.b[*(u16 *)(entry + 4)], e);
}

INCLUDE_ASM(const s32, "game/code_00254B30", func_002562E8);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00256400);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00256540);

typedef struct DspListNode {
    u8 pad00[0x10];
    struct DspListNode *next; /* 0x10 */
} DspListNode;

typedef struct {
    s32 frameCount;        /* 0x00 */
    u32 pad04;            /* 0x04 */
    DspListNode *first;   /* 0x08 */
} DspListHead;

DspListNode *mnuAllocateDisplayListNode(void) {
    DspListNode *node = (DspListNode *)func_002CFEB8(0x14);

    memset(node, 0, 0x14);
    return node;
}

DspListNode *func_00256B78(DspListHead *head) {
    DspListNode *node = head->first;
    if (node == NULL) {
        node = mnuAllocateDisplayListNode();
        head->first = node;
    } else {
        while (node->next != NULL) {
            node = node->next;
        }
        node->next = mnuAllocateDisplayListNode();
        node = node->next;
    }
    return node;
}

DspListNode *func_00256C00(DspListNode *node) {
    DspListNode *next;

    next = node->next;
    func_002CFF98();
    return next;
}

void mnuReleaseDisplayListNodes(DspListHead *head) {
    DspListNode *node = head->first;

    while (node != NULL) {
        node = func_00256C00(node);
    }
}

extern s32 func_00256400(s32);
extern s32 func_00256540(DspListNode *, s32, s32);

s32 func_00256C58(DspListHead *head, s32 arg1, s32 arg2) {
    s32 *counter = &head->frameCount;
    DspListNode *node = head->first;
    s32 index = 0;

    if (func_00256400(*counter) != 0) {
        *counter = 0;
    } else {
        *counter = *counter + 1;
    }
    if (node == NULL) {
        return 1;
    }
    do {
        s32 hit = func_00256540(node, index, arg2);

        index++;
        if (hit != 0) {
            node = func_00256C00(node);
            head->first = node;
        } else {
            node = node->next;
        }
    } while (node != NULL);
    return 0;
}

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

extern s32 func_0024FA18(void);
extern u32 func_00255FF8(u8 *, s32);

void func_002570A8(s32 obj, s32 arg1, s32 arg2) {
    s32 target = func_0024FA18();
    u8 *entry = *(u8 **)(*(u8 **)(*(u8 **)(obj + 0x484) + 8) + 4);
    u32 flags;
    s32 kind;

    if (entry != NULL) {
        flags = func_00255FF8(entry, target);
        if (flags & 1) {
            kind = 9;
        } else if (flags & 2) {
            kind = 8;
        } else if (flags & 4) {
            kind = 6;
        } else {
            kind = 7;
        }
        func_0024E260(0, 0, 1, arg1, kind, arg2);
    }
}

INCLUDE_SDATA(const s32, "game/code_00254B30", D_003BC448);

INCLUDE_SDATA(const s32, "game/code_00254B30", D_003BC450);

INCLUDE_SDATA(const s32, "game/code_00254B30", D_003BC458);

INCLUDE_SDATA(const s32, "game/code_00254B30", D_003BC460);

INCLUDE_SDATA(const s32, "game/code_00254B30", D_003BC468);

INCLUDE_SDATA(const s32, "game/code_00254B30", D_003BC470);

INCLUDE_SDATA(const s32, "game/code_00254B30", D_003BC478);

INCLUDE_SDATA(const s32, "game/code_00254B30", D_003BC480);

