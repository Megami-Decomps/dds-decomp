#include "common.h"

extern void dspStartEntry(s32 signal);

extern void func_0024E260(s32, s32, s32, s32, s32, s32);

extern s32 func_002CFEB8(u32);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00254B30);

extern void mnuCallInitWide(s32, s32, s32, s32, s32);

typedef struct {
    u8 pad0[4];
    u16 index; /* 0x4: index into the display label or marker table */
} DspEntry;

typedef struct {
    u8 pad0[0xC];
    u16 sceneId; /* 0xC: label lookup and displayed scene identifier */
} DspScene;

typedef struct {
    DspEntry *entry;   /* 0x0: selected display entry */
    s32 alternate; /* 0x4: index for the alternate label table */
} DspSelection;

typedef struct {
    u8 pad0[0x30];
    s32 *selectedValue; /* 0x30 */
} DspUnit;

typedef struct {
    u8 pad0[0xC];
    DspUnit *unit; /* 0xC */
} DspWindowContext;

/* Store the selected value in the display unit and open its scaled window. */
void itfDspInitSelectedWindow(s32 x, s32 y, s32 z, s32 value, DspWindowContext *context, s32 parameter) {
    DspUnit *unit = context->unit;

    *unit->selectedValue = value;
    mnuCallInitWide(x << 4, y << 3, z, (s32)unit, parameter);
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
extern s32 mnuGetMantraSourceValue(s32);
extern void func_003014F0(void *, void *, s32);
extern void func_0024DAE8(s32);
extern void dspStartEntry(s32);
extern void evtCaptureMessageWindowSoundMode(s32);

/* Populate four menu labels from the current selection and scene metadata. */
void itfDspPopulatePrimaryLabels(void) {
    DspSelection *selection = (DspSelection *)func_0024FA18();
    DspScene *scene = (DspScene *)fldGetSceneMetadataNode();
    char text[16];

    func_0024DD90(0, &D_003BAA70[selection->entry->index]);
    func_0024DD90(1, &D_003BAA78[selection->alternate]);
    func_0024DD90(2, &D_003BAA78[scene->sceneId]);
    func_003014F0(text, D_003BC468, mnuGetMantraSourceValue(scene->sceneId));
    func_0024DD90(3, text);
    func_0024DAE8(0);
    dspStartEntry(0);
    evtCaptureMessageWindowSoundMode(8);
}

/* Populate the same menu labels, selecting the alternate display signal. */
void itfDspPopulateAlternateLabels(void) {
    DspSelection *selection = (DspSelection *)func_0024FA18();
    DspScene *scene = (DspScene *)fldGetSceneMetadataNode();
    char text[16];

    func_0024DD90(0, &D_003BAA70[selection->entry->index]);
    func_0024DD90(1, &D_003BAA78[selection->alternate]);
    func_0024DD90(2, &D_003BAA78[scene->sceneId]);
    func_003014F0(text, D_003BC468, mnuGetMantraSourceValue(scene->sceneId));
    func_0024DD90(3, text);
    func_0024DAE8(0);
    dspStartEntry(1);
    evtCaptureMessageWindowSoundMode(8);
}

/* Populate menu labels for the third display signal. */
void itfDspPopulateThirdLabels(void) {
    DspSelection *selection = (DspSelection *)func_0024FA18();
    DspScene *scene = (DspScene *)fldGetSceneMetadataNode();
    char text[16];

    func_0024DD90(0, &D_003BAA70[selection->entry->index]);
    func_0024DD90(1, &D_003BAA78[selection->alternate]);
    func_0024DD90(2, &D_003BAA78[scene->sceneId]);
    func_003014F0(text, D_003BC468, mnuGetMantraSourceValue(scene->sceneId));
    func_0024DD90(3, text);
    dspStartEntry(2);
}

void itfDspSignalA(void) {
    dspStartEntry(3);
}

void itfDspSignalB(void) {
    dspStartEntry(4);
}

void itfDspSignalC(void) {
    dspStartEntry(5);
}

void itfDspSignalD(void) {
    dspStartEntry(6);
}

void itfDspSignalE(void) {
    dspStartEntry(7);
}

extern f32 effMiscRandUnitFloat(s32);



typedef struct DspParticle {
    s16 x;         /* 0x00 */
    s16 y;         /* 0x02 */
    s16 timer;     /* 0x04 */
    s16 timerMax;  /* 0x06 */
    s16 z;         /* 0x08 */
    s8 phase;      /* 0x0A */
    u8 size;       /* 0x0B */
} DspParticle;

void func_00255838(DspParticle *spark) {
    spark->timer = spark->timer - 1;
    if (spark->timer < 0) {
        if (spark->phase != 0) {
            spark->phase = 0;
            spark->timer = spark->timerMax = effMiscRandUnitFloat(0) * 30.0f + 1.0f;
        } else if (effMiscRandUnitFloat(0) > 0.7f) {
            spark->timer = spark->timerMax = effMiscRandUnitFloat(0) * 30.0f + 0.0f;
        } else {
            spark->phase = 1;
            spark->timer = spark->timerMax = effMiscRandUnitFloat(0) * 60.0f + 45.0f;
            spark->z = (effMiscRandUnitFloat(0) * 2.0f - 1.0f) * 512.0f + 320.0f;
            spark->size = effMiscRandUnitFloat(0) * 10.0f;
            spark->x = effMiscRandUnitFloat(0) * 512.0f + -256.0f;
            spark->y = effMiscRandUnitFloat(0) * 448.0f + -128.0f;
        }
    }
}



typedef struct {
    s32 countdown;
    s32 period;
    f32 strength;
    DspParticle particles[8];
} DspParticleState;

void func_002559F8(DspParticleState *state) {
    DspParticle *particle;
    s32 duration;
    s32 i;

    state->countdown = state->countdown - 1;
    if (state->countdown < 0) {
        duration = effMiscRandUnitFloat(0) * 60.0f + 60.0f;
        state->period = duration;
        state->countdown = duration;
        state->strength = effMiscRandUnitFloat(0) * 0.20000005f + 0.4f;
    }
    /* Advance every particle, including those waiting for their next phase. */
    particle = state->particles;
    for (i = 7; i >= 0; i--) {
        func_00255838(particle);
        particle++;
    }
}

INCLUDE_ASM(const s32, "game/code_00254B30", func_00255A98);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00255B78);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00255D00);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00255E08);

void itfDspDrawMarksA(s32 scale, s32 context) {
    func_0024E260(0, 0, 0, scale, 0x3A, context);
    func_0024E260(0, 0, 0, scale, 0x38, context);
    func_0024E260(0, 0, 0, scale, 0x39, context);
}

void itfDspDrawMarksB(s32 scale, s32 context) {
    func_0024E260(0, 0, 0, scale, 0xF, context);
    func_0024E260(0, 0, 0, scale, 0x10, context);
    func_0024E260(0, 0, 0, scale, 0x12, context);
}

INCLUDE_ASM(const s32, "game/code_00254B30", func_00255FF8);

void itfDspDrawStrip(s32 x, s32 y, s32 layer, s32 scale, s32 context) {
    func_0024E260(x, y, layer, scale, 4, context);
    func_0024E260(x, y, layer, scale, 5, context);
    func_0024E260(x, y, layer, scale, 0xA, context);
    func_0024E260(x, y, layer, scale, 0xB, context);
    func_0024E260(x, y, layer, scale, 0xC, context);
    func_0024E260(x, y, layer, scale, 0xD, context);
}

INCLUDE_ASM(const s32, "game/code_00254B30", func_002561A0);

typedef struct Bytes7 {
    s8 b[7];
} Bytes7;

extern Bytes7 D_003BC478[];

void func_00256290(s32 x, s32 y, s32 layer, DspEntry *entry, s32 scale, s32 context) {
    Bytes7 table = D_003BC478[0];

    func_0024E260(x, y, layer, scale, table.b[entry->index], context);
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
    sdfReleaseChipBlock();
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
s32 itfAdvanceDisplayList(DspListHead *head, s32 unused, s32 context) {
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
        s32 completed = func_00256540(node, index, context);

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

void itfDspDrawScaledA(s32 unused, s32 scale, s32 context) {
    func_0024E260(0, 0, 0, scale, 0x16, context);
    func_0024E260(0, 0, 0, scale * 0.5f, 0x18, context);
}

void itfDspDrawScaledB(s32 unused, s32 scale, s32 context) {
    func_0024E260(0, 0, 0, scale, 0x17, context);
    func_0024E260(0, 0, 0, scale * 0.5f, 0x19, context);
}

void itfDspDrawPair(s32 x, s32 y, s32 layer, s32 scale, s32 context) {
    func_0024E260(x, y, layer, scale, 0x14, context);
    func_0024E260(x, y, layer, scale, 0x15, context);
}

void func_00256E88(void) {
}

INCLUDE_ASM(const s32, "game/code_00254B30", func_00256E90);

extern s32 func_0024FA18(void);
extern u32 func_00255FF8(u8 *, s32);

typedef struct {
    u8 pad0[4];
    u8 *entry; /* 0x4: entry tested by func_00255FF8 */
} DspEntryLink;

typedef struct {
    u8 pad0[8];
    DspEntryLink *link; /* 0x8 */
} DspEntryContainer;

typedef struct {
    u8 pad0[0x484];
    DspEntryContainer *entries; /* 0x484 */
} DspDisplayObject;

void func_002570A8(DspDisplayObject *obj, s32 scale, s32 context) {
    s32 target = func_0024FA18();
    u8 *entry = obj->entries->link->entry;
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
        func_0024E260(0, 0, 1, scale, kind, context);
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

