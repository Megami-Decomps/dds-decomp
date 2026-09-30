#include "common.h"

extern void func_0024DA58(s32 arg0);

extern void func_0024E260(s32, s32, s32, s32, s32, s32);

extern s32 func_002CFEB8(u32);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00254B30);

extern void mnuCallInitWide(s32, s32, s32, s32, s32);

/* Store the selected value in the display unit and open its scaled window. */
void itfDspInitSelectedWindow(s32 x, s32 y, s32 z, s32 value, s32 context, s32 parameter) {
    s32 unit = *(s32 *)(context + 0xC);

    **(s32 **)(unit + 0x30) = value;
    mnuCallInitWide(x << 4, y << 3, z, unit, parameter);
}

INCLUDE_ASM(const s32, "game/code_00254B30", func_00254C68);

extern void *memset(void *, s32, u32);
extern void func_002CD0D8(u32, s32, void *);
extern void func_002CA858(s32, s32, s32, u32, s32, void *, u32, s32);

/* Fetch an indexed display record and draw it with the requested tag bits. */
void itfDspDrawIndexedRecord(s32 x, s32 y, s32 layer, u32 attributes, u32 entry, s32 context) {
    u32 tag = attributes | 0xA09DC300;
    u32 id = entry & 0xFFFF;
    u8 buffer[0x20];

    memset(buffer, 0, 0x20);
    func_002CD0D8(id, 1, buffer);
    func_002CA858(x + 0x35, y + 0x136, layer, tag, 4, buffer, 0x80000000, context);
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
extern s32 fldGetSceneMetadataNode();
extern void func_0024DD90(s32, void *);
extern s32 func_0024F800(s32);
extern void func_003014F0(void *, void *, s32);
extern void func_0024DAE8(s32);
extern void func_0024DA58(s32);
extern void func_0024DAB8(s32);

/* Populate four menu labels from the current selection and scene metadata. */
void itfDspPopulatePrimaryLabels(void) {
    s32 *selection = (s32 *)func_0024FA18();
    u8 *scene = (u8 *)fldGetSceneMetadataNode();
    char text[16];

    func_0024DD90(0, &D_003BAA70[*(u16 *)(*(s32 *)selection + 4)]);
    func_0024DD90(1, &D_003BAA78[*(s32 *)((u8 *)selection + 4)]);
    func_0024DD90(2, &D_003BAA78[*(u16 *)(scene + 0xC)]);
    func_003014F0(text, D_003BC468, func_0024F800(*(u16 *)(scene + 0xC)));
    func_0024DD90(3, text);
    func_0024DAE8(0);
    func_0024DA58(0);
    func_0024DAB8(8);
}

/* Populate the same menu labels, selecting the alternate display signal. */
void itfDspPopulateAlternateLabels(void) {
    s32 *selection = (s32 *)func_0024FA18();
    u8 *scene = (u8 *)fldGetSceneMetadataNode();
    char text[16];

    func_0024DD90(0, &D_003BAA70[*(u16 *)(*(s32 *)selection + 4)]);
    func_0024DD90(1, &D_003BAA78[*(s32 *)((u8 *)selection + 4)]);
    func_0024DD90(2, &D_003BAA78[*(u16 *)(scene + 0xC)]);
    func_003014F0(text, D_003BC468, func_0024F800(*(u16 *)(scene + 0xC)));
    func_0024DD90(3, text);
    func_0024DAE8(0);
    func_0024DA58(1);
    func_0024DAB8(8);
}

/* Populate menu labels for the third display signal. */
void itfDspPopulateThirdLabels(void) {
    s32 *selection = (s32 *)func_0024FA18();
    u8 *scene = (u8 *)fldGetSceneMetadataNode();
    char text[16];

    func_0024DD90(0, &D_003BAA70[*(u16 *)(*(s32 *)selection + 4)]);
    func_0024DD90(1, &D_003BAA78[*(s32 *)((u8 *)selection + 4)]);
    func_0024DD90(2, &D_003BAA78[*(u16 *)(scene + 0xC)]);
    func_003014F0(text, D_003BC468, func_0024F800(*(u16 *)(scene + 0xC)));
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

extern f32 func_002E8398(s32);

extern void func_00255838(void *);

void func_002559F8(s32 *state) {
    u8 *particle;
    s32 value;
    s32 i;

    state[0] = state[0] - 1;
    if (state[0] < 0) {
        value = func_002E8398(0) * 60.0f + 60.0f;
        state[1] = value;
        state[0] = value;
        *(f32 *)(state + 2) = func_002E8398(0) * 0.20000005f + 0.4f;
    }
    particle = (u8 *)(state + 3);
    for (i = 7; i >= 0; i--) {
        func_00255838(particle);
        particle += 12;
    }
}

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

DspListNode *mnuAppendDisplayListNode(DspListHead *head) {
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

DspListNode *mnuReleaseDisplayListNodeAndGetNext(DspListNode *node) {
    DspListNode *next;

    next = node->next;
    func_002CFF98();
    return next;
}

void mnuReleaseDisplayListNodes(DspListHead *head) {
    DspListNode *node = head->first;

    while (node != NULL) {
        node = mnuReleaseDisplayListNodeAndGetNext(node);
    }
}

extern s32 func_00256400(s32);
extern s32 func_00256540(DspListNode *, s32, s32);

/* Advance the display list's timer and release completed nodes in order. */
s32 itfAdvanceDisplayList(DspListHead *head, s32 arg1, s32 arg2) {
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
        s32 completed = func_00256540(node, index, arg2);

        index++;
        if (completed != 0) {
            node = mnuReleaseDisplayListNodeAndGetNext(node);
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

